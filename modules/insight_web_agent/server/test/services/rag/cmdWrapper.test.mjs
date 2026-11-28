/*
 * -------------------------------------------------------------------------
 * This file is part of the MindStudio project.
 * Copyright (c) 2026 Huawei Technologies Co.,Ltd.
 * MindStudio is licensed under Mulan PSL v2.
 * -------------------------------------------------------------------------
 */

import assert from "node:assert/strict";
import { readFile } from "node:fs/promises";
import { test } from "node:test";

test("Windows wrapper invokes only the sibling bundled CLI without injecting a data root", async () => {
    const content = await readFile(new URL("../../../../scripts/mindstudio-insight-rag.cmd", import.meta.url), "utf8");

    assert.match(content, /%~dp0rag-cli\.mjs/);
    assert.match(content, /"%~dp0runtime\\node\.exe" "%~dp0rag-cli\.mjs" %\*/);
    assert.match(content, /if not exist "%~dp0runtime\\node\.exe"/);
    assert.match(content, /exit \/b 1/);
    assert.doesNotMatch(content, /^node\s/m);
    assert.doesNotMatch(content, /--data-dir|rag-data|[A-Za-z]:\\/);
});

test("Standalone server bundles use the development wrapper; product packaging replaces it", async () => {
    const development = await readFile(new URL("../../../../scripts/mindstudio-insight-rag.dev.cmd", import.meta.url), "utf8");
    const builder = await readFile(new URL("../../../../scripts/build-server.mjs", import.meta.url), "utf8");

    assert.match(development, /^node "%~dp0rag-cli\.mjs" %\*/m);
    assert.match(builder, /join\(rootDir, "scripts", "mindstudio-insight-rag\.dev\.cmd"\)/);
    assert.doesNotMatch(development, /runtime\\node\.exe/);
});
