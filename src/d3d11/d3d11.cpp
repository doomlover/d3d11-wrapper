/*
    CONTRIBUTORS:
        Sean Pesce

*/

#include "d3d11/main.h"
#include <string>
#include <cstdlib>
#include <windows.h>
#include <tchar.h>
#include <filesystem>


// --> modified by lj begin
BOOL IsWow64()
{
#if 1
	BOOL isWow64 = FALSE;
	if (!IsWow64Process(GetCurrentProcess(), &isWow64)) {
        // error
	}
    return isWow64;
#else
    typedef BOOL(WINAPI* LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
    LPFN_ISWOW64PROCESS fnIsWow64Process = nullptr;
    BOOL bIsWow64 = FALSE;
    //IsWow64Process is not available on all supported versions of Windows.
    //Use GetModuleHandle to get a handle to the DLL that contains the function
    //and GetProcAddress to get a pointer to the function if available.
    fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(TEXT("kernel32")), "IsWow64Process");
    if (NULL != fnIsWow64Process)
    {
        if (!fnIsWow64Process(GetCurrentProcess(), &bIsWow64))
        {
            //handle error
        }
    }
    return bIsWow64;
#endif
}

std::string ParsePath(const std::string& input) {
	std::filesystem::path filePath(input);
	return filePath.parent_path().string();
}

std::string JoinPaths(const std::string& path1, const std::string& path2) {
	std::filesystem::path p1(path1);
	std::filesystem::path p2(path2);
	p1 /= p2;
	return p1.string();
}


// 取得RenderDoc安装路径
std::string GetRenderDocInstalledLocation() {
    // 定义注册表键值和名称
    const TCHAR registryKeyPath[] = _T("SOFTWARE\\Classes\\RenderDoc.RDCCapture.1\\DefaultIcon\\");
    const TCHAR registryValueName[] = _T("");

    HKEY hKey;
    TCHAR installLocation[MAX_PATH] = { 0 };
    DWORD installLocationSize = sizeof(installLocation);
    DWORD valueType = REG_SZ;

    // 打开注册表键
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, registryKeyPath, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        // 从注册表键中读取值
        if (RegQueryValueEx(hKey, registryValueName, nullptr, &valueType, (LPBYTE)installLocation, &installLocationSize) == ERROR_SUCCESS) {
            // 关闭注册表键
            RegCloseKey(hKey);
            return ParsePath(std::string(installLocation));
        }
        // 关闭注册表键
        RegCloseKey(hKey);
    }
    return "";
}

// --< end

namespace d3d11 {

HMODULE chain = NULL;
FARPROC functions[func_count];

void hook_exports()
{
    /*std::string path(user_setting<std::string>("DLL", "d3d11Chain", "").read());
    if (!path.empty())
    {
        SP_LOG("Attempting to chain d3d11 wrapper (%s)... ", path.c_str());
        chain = LoadLibrary(path.c_str());
        SP_LOG(chain ? "Success\n" : "Failed.\n");
    }
    if (!chain)
    {
        SP_LOG("Attempting to load %s from system path... ", filename);
        path = sp::env::system_dir() + "\\" + filename;
        chain = LoadLibrary(path.c_str());
        SP_LOG(chain ? "Success\n" : "Failed.\nERROR: Failed to load %s\nExiting...\n", filename);
    }
    if (!chain)
    {
        MessageBox(NULL, "Unable to locate original d3d11.dll (or compatible library to chain)", "ERROR: Failed to load original d3d11.dll", NULL);
        exit(0);
    }
    
    SP_LOG("Locating exports... ");
    int count = 0;
    for (int i = 0; i < d3d11::func_count; i++)
    {
        FARPROC func = GetProcAddress(chain, func_names[i]);
        if (func)
        {
            count++;
        }
        functions[i] = func;
    }
    SP_LOG("Found %d out of %d.\n", count, d3d11::func_count);*/
    if (IsWow64())
    {
        chain = LoadLibrary("C:\\Windows\\SysWOW64\\d3d11.dll");
    }
    else
    {
        chain = LoadLibrary("C:\\Windows\\System32\\d3d11.dll");
    }
    std::string RenderdocPath = GetRenderDocInstalledLocation();
    std::string RenderdocDllPath = JoinPaths(RenderdocPath, "renderdoc.dll");
    HMODULE renderdoc = LoadLibrary(RenderdocDllPath.c_str());
    if (!renderdoc) {
        MessageBox(NULL, "Unable to locate renderdoc (or compatible library to chain)", "ERROR: Failed to load original d3d11.dll", NULL);
        exit(0);
    }
}

} // namespace d3d11



