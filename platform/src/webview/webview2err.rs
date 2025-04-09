/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#[cfg(windows)]
use std::ffi::CString;
#[cfg(windows)]
use std::fs::read_to_string;

enum HWND__ {}
const MB_ICONERROR: u32 = 0x00000010;

extern "system" {
    /// Displays a modal dialog box that contains a system icon, 
    /// a set of buttons, and a brief application-specific message, 
    /// such as status or error information.
    /// The message box returns an integer value
    /// that indicates which button the user clicked.
    /// 
    /// ### FFI Signature
    /// ```c++
    /// int MessageBoxA(
    ///   [in, optional] HWND   hWnd,
    ///   [in, optional] LPCSTR lpText,
    ///   [in, optional] LPCSTR lpCaption,
    ///   [in]           UINT   uType
    /// );
    /// ```
    #[allow(non_snake_case)]
    fn MessageBoxA(
        hWnd: *mut HWND__,
        lpText: *const i8,
        lpCaption: *const i8,
        uType: u32,
    ) -> i32;
}

const RUNTIME_URL_CONFIG_PATH: &str = "config/runtime_url_config";

#[cfg(windows)]
pub fn show_webview_err_message() {
    let mut runtime_url = String::new();
    match read_to_string(RUNTIME_URL_CONFIG_PATH) {
        Ok(content) => {
            if content.len() > 100 {
                return;
            }
            runtime_url = content
        }
        Err(e) => eprintln!("read webview2 runtime url config failed: {e}"),
    }

    let message = format!("Please install from {}", runtime_url);
    let title = "Missing Dependencies";
    let message = CString::new(message).expect("CString::new failed");
    let title = CString::new(title).expect("CString::new failed");
    /// # Safety
    /// 
    /// This FFI call requires the following invariants to be upheld:
    /// 
    /// ## Parameters Safety
    /// 1. `hWnd` - Null pointer is explicitly passed as we don't need a parent window
    /// 2. `lpText` - 
    ///    - Pointer must remain valid for the duration of the call
    ///    - `CString` ensures proper null-terminated UTF-8 encoding
    /// 3. `lpCaption` - 
    ///    - Same safety guarantees as `lpText`
    /// 
    /// ## Memory Safety
    /// - Both `CString` instances will be dropped after this function call returns.
    unsafe {
        MessageBoxA(
            std::ptr::null_mut(),
            message.as_ptr(),
            title.as_ptr(),
            MB_ICONERROR,
        );
    }
}
