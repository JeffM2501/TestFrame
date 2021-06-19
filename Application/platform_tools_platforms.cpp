#include <string>
#include <vector>

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commdlg.h>
#include <shobjidl.h>

HWND WindowHandle;
namespace PlatformTools
{
    void SetWindowHandle(void* handle)
    {
        WindowHandle = HWND(handle);
    }
}

std::string wstrtostr(const std::wstring& wstr)
{
    std::string strTo;
    char* szTo = new char[wstr.length() + 1];
    szTo[wstr.length()] = '\0';
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, szTo, (int)wstr.length(), NULL, NULL);
    strTo = szTo;
    delete[] szTo;
    return strTo;
}

std::wstring strTowstr(const std::string& str)
{
    std::wstring strTo;
    wchar_t* szTo = new wchar_t[str.length() + 1];
    szTo[str.length()] = '\0';
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, szTo, (int)str.length());
    strTo = szTo;
    delete[] szTo;
    return strTo;
}

void copyWCharsRaw(wchar_t* dst, const wchar_t* src, size_t amt) {
    for (size_t i = 0; i < amt; i++) {
        dst[i] = src[i];
    }
}

const wchar_t* BuildFilter(std::vector<std::pair<std::string, std::string>> filterValues) {
    if (filterValues.size() == 0)
    {
        filterValues.push_back({"All Files","*"});
    }
    
    size_t count = 0;
    for (auto& filterValue : filterValues)
    {
        count += filterValue.first.size() + 1;
        count += filterValue.second.size() + 1 + 2; // the 2 for the "*." added later
    }
    count += 1; // second null

    wchar_t* filter = new wchar_t[count];

    size_t offset = 0;
    for (auto& filterValue : filterValues)
    {
        const std::wstring& name = strTowstr(filterValue.first);
        const std::wstring extension = strTowstr("*." + filterValue.second);

        copyWCharsRaw(filter + offset, name.c_str(), name.length());
        offset += name.length();

        filter[offset] = 0;
        offset += 1;

        copyWCharsRaw(filter + offset, extension.c_str(), extension.length());
        offset += extension.length();

        filter[offset] = 0;
        offset += 1;
    }
    filter[offset] = 0;
    offset += 1;

    return filter;
}

OPENFILENAME getDefaultOFN(LPWSTR lpstrFile,const DWORD nMaxFile, LPCWSTR lpstrFilter)
{
    // common dialog box structure, setting all fields to 0 is important
    OPENFILENAME ofn = { 0 };

    // Initialize remaining fields of OPENFILENAME structure
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = WindowHandle;
    ofn.lpstrFile = lpstrFile;
    ofn.nMaxFile = nMaxFile;
    ofn.lpstrFilter = lpstrFilter;
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = nullptr;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = nullptr;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    return ofn;
}

std::string OpenFileDialog(const char* filename, std::vector<std::pair<std::string, std::string>> filterValues)
{
    std::string result;

    const wchar_t* filter = BuildFilter(filterValues);

    constexpr int nMaxFile = 260;
    TCHAR szFile[nMaxFile] = { 0 };
    if (filename != nullptr)
    {
        std::wstring ws = strTowstr(std::string(filename));
        wcsncpy(szFile, ws.c_str(), nMaxFile);
        szFile[nMaxFile - 1] = 0; //safety null termination
    }

    OPENFILENAME ofn = getDefaultOFN(szFile, nMaxFile, filter);

    if (GetOpenFileName(&ofn) == TRUE)
    {
        // use ofn.lpstrFile here
        result = wstrTostr(std::wstring(ofn.lpstrFile));
    }

    delete[] filter;

    return result;
}
std::string SaveFileDialog(const char* filename, std::vector<std::pair<std::string, std::string>> filterValues)
{
    std::string result;

    const wchar_t* filter = BuildFilter(filterValues);
    
    constexpr int nMaxFile = 260;
    TCHAR szFile[nMaxFile] = { 0 };
    if (filename != nullptr)
    {
        std::wstring ws = strTowstr(std::string(filename));
        wcsncpy(szFile, ws.c_str(), nMaxFile);
        szFile[nMaxFile - 1] = 0; //safety null termination
    }

    OPENFILENAME ofn = getDefaultOFN(szFile, nMaxFile, filter);

    if (GetSaveFileName(&ofn) == TRUE)
    {
        // use ofn.lpstrFile here
        result = wstrTostr(std::wstring(ofn.lpstrFile));
    }

    delete[] filter;

    return result;
}

#else

void SetWindowHandle(void*)
{
}

std::string OpenFileDialog(const char*, std::vector<std::pair<std::string, std::string>>)
{
    return std::string();
}

std::string SaveFileDialog(const char*, std::vector<std::pair<std::string, std::string>>)
{
    return std::string();
}

#endif 