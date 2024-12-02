/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#![windows_subsystem = "windows"]

#[cfg(windows)]
use std::os::windows::process::CommandExt;
use std::{
    env,
    env::current_exe,
    fs::read,
    path::PathBuf,
    process::{Child, Command},
    sync::{Arc, Mutex},
};

#[cfg(windows)]
use webview2err::show_webview_err_message;
#[cfg(feature = "default")]
#[cfg(target_os = "macos")]
use wry::application::menu::{MenuBar, MenuItem};
#[cfg(feature = "default")]
use wry::{
    application::{
        event::{Event, WindowEvent},
        event_loop::{ControlFlow, EventLoop, EventLoopProxy},
        window::{Window, WindowBuilder},
    },
    http::{header::CONTENT_TYPE, Response},
    webview::{webview_version, WebView, FileDropEvent, WebViewBuilder},
};

use crate::cleanup::handle_close_requested;

mod cleanup;
#[cfg(windows)]
mod webview2err;

const SERVER_RELATIVE_LIST: [&str; 4] =
    ["resources", "profiler", "server", "profiler_server"];
#[cfg(windows)]
const NO_WINDOW_FLAG: u32 = 0x08000000;
const MIMETYPE_HTML: &str = "text/html";

fn server_path(root_path: &PathBuf) -> Option<PathBuf> {
    let mut server_path = root_path.to_path_buf();

    #[cfg(target_os = "macos")]
    {
        env::set_var(
            "DYLD_LIBRARY_PATH",
            server_path
                .join("resources/profiler/server/:$DYLD_LIBRARY_PATH")
                .as_path(),
        );
    }

    #[cfg(target_os = "linux")]
    {
        let ld_library_path =
            env::var_os("LD_LIBRARY_PATH").unwrap_or_default();
        let new_ld_library_path = format!(
            "{}:{}",
            server_path.join("resources/profiler/server/").display(),
            ld_library_path.to_string_lossy()
        );
        env::set_var("LD_LIBRARY_PATH", &new_ld_library_path);
    }

    for tmp in SERVER_RELATIVE_LIST {
        server_path.push(tmp);
    }

    #[cfg(windows)]
    server_path.set_extension("exe");

    if !server_path.exists() {
        return None;
    }

    Some(server_path)
}

fn run_server(
    root_path: &PathBuf,
    cache_path: &PathBuf,
    port: &mut String,
    sid: &String,
) -> Option<Mutex<Child>> {
    let binding = server_path(root_path)?;
    let Some(path) = binding.to_str() else { return None };

    let output = Command::new(path)
        .arg("--scan=9000")
        .arg(format!("--logPath={}", cache_path.display()))
        .output()
        .expect("Failed to start MindStudio Insight server");

    let scan_info = String::from_utf8_lossy(&output.stdout).to_string();

    if scan_info.len() > 0 && scan_info.starts_with("Available port: ") {
        port.push_str(scan_info.replace("Available port: ", "").trim());
    } else {
        return None;
    };

    let mut server_command = Command::new(path);

    #[cfg(windows)]
    server_command.creation_flags(NO_WINDOW_FLAG);

    match server_command
        .arg(format!("--wsPort={}", port))
        .arg(format!("--logPath={}", cache_path.display()))
        .arg(format!("--sid={}", sid))
        .spawn()
    {
        Ok(child) => Some(Mutex::new(child)),
        _ => None,
    }
}

#[cfg(target_os = "macos")]
fn macos_menu() -> MenuBar {
    let mut menu = MenuBar::new();

    let mut window_menu = MenuBar::new();
    window_menu.add_native_item(MenuItem::Minimize);
    window_menu.add_native_item(MenuItem::Hide);
    window_menu.add_native_item(MenuItem::HideOthers);
    window_menu.add_native_item(MenuItem::Separator);
    window_menu.add_native_item(MenuItem::Services);
    window_menu.add_native_item(MenuItem::Separator);
    window_menu.add_native_item(MenuItem::CloseWindow);
    window_menu.add_native_item(MenuItem::Quit);

    menu.add_submenu("Window", true, window_menu);

    let mut edit_menu = MenuBar::new();
    edit_menu.add_native_item(MenuItem::Cut);
    edit_menu.add_native_item(MenuItem::Copy);
    edit_menu.add_native_item(MenuItem::Paste);
    edit_menu.add_native_item(MenuItem::SelectAll);

    menu.add_submenu("Edit", true, edit_menu);

    menu
}

#[cfg(windows)]
fn set_windows_icon(window: &Window, root_path: &PathBuf) {
    use wry::application::{platform::windows::IconExtWindows, window::Icon};

    window.set_window_icon(
        Icon::from_path(
            root_path
                .to_path_buf()
                .join("resources/images/icons/mindstudio.ico"),
            None,
        )
        .ok(),
    );
}

fn extract_mimetype(path: &str) -> &str {
    let mut mimetype = MIMETYPE_HTML;
    if let Some((_, ext)) = path.rsplit_once('.') {
        mimetype = match ext {
            "html" => "text/html",
            "js" => "text/javascript",
            "css" => "text/css",
            "svg" => "image/svg+xml",
            "png" => "image/png",
            "wasm" => "application/wasm",
            _ => MIMETYPE_HTML,
        }
    }

    mimetype
}

fn create_webview(
    window: Window,
    resource_path: Arc<PathBuf>,
    port: &str,
    sid: &String,
    proxy: Arc<EventLoopProxy<PathBuf>>,
) -> wry::Result<WebView> {
    WebViewBuilder::new(window)?
        .with_custom_protocol("wry".into(), move |request| {
            let path = request.uri().path();
            let content = match read(resource_path.join(&path[1..]).as_path()) {
                Ok(a) => a.into(),
                Err(e) => return Err(wry::Error::Io(e)),
            };

            let mimetype = extract_mimetype(path);

            Response::builder()
                .header(CONTENT_TYPE, mimetype)
                .body(content)
                .map_err(Into::into)
        })
        .with_url(format!("wry://localhost/resources/profiler/frontend/index.html?port={}&sid={}", port, sid).as_str())?
        .with_file_drop_handler(move |_, ev| {
            match ev {
                FileDropEvent::Dropped(paths) => {
                    if let Err(e) = proxy.send_event(paths[0].to_owned()) {
                        eprintln!("app closed unexpectedly: {:#?}", e);
                    }
                }
                _ => {}
            }

            true
        })
        .build()
}

fn handle_user_event(webview: &WebView, path: PathBuf) {
    if let Err(e) =
        webview.evaluate_script(&format!("window.handleDrop({:#?})", path))
    {
        eprintln!("drop-file ipc failed: {:#?}", e);
    }
}

fn run_event_loop(
    event_loop: EventLoop<PathBuf>,
    server_process: Arc<Mutex<Child>>,
    webview: WebView,
) {
    event_loop.run(move |event, _, control_flow| {
        *control_flow = ControlFlow::Wait;
        match event {
            Event::WindowEvent {
                event: WindowEvent::CloseRequested, ..
            } => {
                *control_flow = handle_close_requested(server_process.clone());
            }
            Event::UserEvent(path) => handle_user_event(&webview, path),
            _ => (),
        }
    });
}

// run script
#[cfg(feature = "default")]
fn run_script(
    server_process: Arc<Mutex<Child>>,
    root_path: &PathBuf,
    port: &str,
    sid: &String,
) -> wry::Result<()> {
    let event_loop = EventLoop::with_user_event();

    let proxy = Arc::new(event_loop.create_proxy());

    let mut window_builder: WindowBuilder = WindowBuilder::new()
        .with_title("MindStudio Insight")
        .with_maximized(true);

    #[cfg(target_os = "macos")]
    {
        let menu = macos_menu();
        window_builder = window_builder.with_menu(menu);
    }

    let window = window_builder
        .build(&event_loop)
        .expect("Error occurred when create App window");

    let resource_path = Arc::new(root_path.to_path_buf());

    #[cfg(windows)]
    set_windows_icon(&window, root_path);

    let webview = create_webview(window, resource_path, port, sid, proxy)?;

    Ok(run_event_loop(event_loop, server_process, webview))
}

fn home_dir() -> Option<PathBuf> {
    #[cfg(target_os = "windows")]
    {
        env::var("USERPROFILE").ok().map(PathBuf::from)
    }

    #[cfg(not(target_os = "windows"))]
    {
        env::var("HOME").ok().map(PathBuf::from)
    }
}

#[cfg(windows)]
#[link(name = "shell32")]
extern "system" {
    pub fn IsUserAnAdmin() -> bool;
}

#[cfg(windows)]
fn is_admin() -> bool {
    unsafe { IsUserAnAdmin() }
}

#[cfg(windows)]
fn eq_prefix(lhs: &PathBuf, rhs: &PathBuf) -> bool {
    match (lhs.components().next(), rhs.components().next()) {
        (Some(l), Some(r)) => l == r,
        _ => false,
    }
}

#[cfg(feature = "default")]
fn main() {
    let mut cache_path = home_dir()
        .expect("Home dir not exists, check your env variable")
        .join(".mindstudio_insight"); //cache folder generated for each user.
    let root_path = current_exe()
        .expect("Failed to get current exe path")
        .parent()
        .expect("Failed to get parent path of  current exe")
        .to_path_buf();

    #[cfg(windows)]
    {
        // 当用户安装在C盘时，使用user目录
        let mut webview_path = cache_path.clone();
        // 当用户安装在C盘外时，使用安装目录
        if !eq_prefix(&cache_path, &root_path) {
            cache_path = root_path.join(".mindstudio_insight");
            webview_path = cache_path.clone();
        }
        if is_admin() {
            webview_path.push("admin");
        }
        env::set_var("WEBVIEW2_USER_DATA_FOLDER", webview_path.as_path());
    }

    if !cache_path.exists() {
        #[cfg(windows)]
        {
            use std::fs::create_dir_all;

            create_dir_all(cache_path.as_path())
                .expect("no permission to create cache_path");
        }

        #[cfg(unix)]
        {
            use std::{fs::DirBuilder, os::unix::fs::DirBuilderExt};

            DirBuilder::new()
                .recursive(true)
                .mode(0o750)
                .create(cache_path.as_path())
                .expect("no permission to create cache_path");
        }
    }

    if webview_version().is_err() {
        #[cfg(windows)]
        show_webview_err_message();

        return;
    }

    let mut port: String = String::new();
    let sid = uuid::Uuid::new_v4().to_string();

    if let Some(child) = run_server(&root_path, &cache_path, &mut port, &sid) {
        let _ = run_script(Arc::new(child), &root_path, &port.as_str(), &sid);
    }
}

#[cfg(not(feature = "default"))]
fn main() {
    use std::process::{exit, ExitStatus};

    let args: Vec<String> = env::args().collect();
    let profiler_path = current_exe()
        .expect("Failed to get current exe path")
        .parent()
        .expect("Failed to get parent path of  current exe")
        .join("resources")
        .join("profiler")
        .join("start_script.py")
        .to_path_buf();
    let Some(path) = profiler_path.to_str() else {
        return;
    };
    let mut server_command = Command::new("python3");
    server_command.arg(path);
    for arg in args {
        server_command.arg(arg);
    }
    match server_command.output() {
        Ok(output) => {
            let formatted = String::from_utf8(output.stdout).unwrap();
            if !formatted.is_empty() {
                eprintln!("{}", formatted);
            }
            exit(output.status.code().unwrap_or(0))
        }
        Err(e) => {
            eprintln!("Failed to start server.Error:{e:?}")
        }
    };
}
