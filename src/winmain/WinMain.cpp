// Copyright 2020 Simon Bates
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "../lib/ProcdrawApp.h"
#include <Shlobj.h>
#include <Windows.h>
#include <plog/Log.h>
#include <string>

#define PROCDRAW_EXIT_UNABLE_TO_CREATE_PROCDRAW_APPDATA_DIR 1;
#define PROCDRAW_EXIT_EXCEPTION 2;

int CALLBACK WinMain(
    _In_ HINSTANCE hInstance,
    _In_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    PWSTR localAppDataPath;
    HRESULT hr = SHGetKnownFolderPath(
        FOLDERID_LocalAppData, KF_FLAG_DEFAULT, nullptr, &localAppDataPath);
    if (FAILED(hr)) {
        return PROCDRAW_EXIT_UNABLE_TO_CREATE_PROCDRAW_APPDATA_DIR;
    }

    std::wstring procdrawDir{std::wstring(localAppDataPath) + L"\\Procdraw"};

    CoTaskMemFree(localAppDataPath);

    if (CreateDirectory(procdrawDir.c_str(), nullptr)
        || GetLastError() == ERROR_ALREADY_EXISTS) {
        std::wstring logFile{procdrawDir + L"\\Procdraw_Log.txt"};
        plog::init(plog::debug, logFile.c_str(), 100 * 1024, 2);
    }
    else {
        return PROCDRAW_EXIT_UNABLE_TO_CREATE_PROCDRAW_APPDATA_DIR;
    }

    PLOG_INFO << "Procdraw started";

    try {
        Procdraw::ProcdrawApp app(hInstance, nCmdShow);
        return app.MainLoop();
    }
    catch (const std::exception& ex) {
        PLOG_ERROR << ex.what();
        return PROCDRAW_EXIT_EXCEPTION;
    }
}
