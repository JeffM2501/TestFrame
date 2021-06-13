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
    szTo[wstr.size()] = '\0';
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, szTo, (int)wstr.length(), NULL, NULL);
    strTo = szTo;
    delete[] szTo;
    return strTo;
}

const char* BuildFilter(std::vector<std::pair<std::string, std::string>> filterValues)
{
    size_t count = 0;
    for (auto& filterValue : filterValues)
    {
        count += filterValue.first.size() + 1;
        count += filterValue.first.size() + 3;
    }
    count += 1;
    if (count == 1)
    {
        count += strlen("All Files\t*.*\t");
    }

    char* filter = new char[count];

    if (filterValues.size() == 0)
    {
        memcpy(filter, "All Files\0*.*\0\0", count);
    }
    else
    {
        size_t offset = 0;
        for (auto& filterValue : filterValues)
        {
            const std::string& name = filterValue.first;
            std::string extension = "*." + filterValue.second;

            memcpy(filter + offset, name.c_str(), name.size());
            offset += name.size();

            memcpy(filter + offset, "\0", 1);
            offset += 1;

            memcpy(filter + offset, extension.c_str(), extension.size());
            offset += extension.size();

            memcpy(filter + offset, "\0", 1);
            offset += 1;
        }
        memcpy(filter + offset, "\0", 1);
        offset += 1;
    }

    return filter;
}

std::string OpenFileDialog(const char* filename, std::vector<std::pair<std::string,std::string>> filterValues)
{
    std::string result;

    const char* filter = BuildFilter(filterValues);

    // common dialog box structure, setting all fields to 0 is important
    OPENFILENAME ofn = { 0 };
    TCHAR szFile[260] = { 0 };
    if (filename != nullptr)
        strcpy_s(szFile, 260, filename);

    // Initialize remaining fields of OPENFILENAME structure
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = WindowHandle;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = filter;
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = nullptr;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = nullptr;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn) == TRUE)
    {
        // use ofn.lpstrFile here
        result = ofn.lpstrFile;
    }

    delete[](filter);

    return result;
}

std::string SaveFileDialog(const char* filename, std::vector<std::pair<std::string, std::string>> filterValues)
{
    std::string result;

    const char* filter = BuildFilter(filterValues);

    // common dialog box structure, setting all fields to 0 is important
    OPENFILENAME ofn = { 0 };
    TCHAR szFile[260] = { 0 };
    if (filename != nullptr)
        strcpy_s(szFile, filename);

    // Initialize remaining fields of OPENFILENAME structure
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = WindowHandle;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = filter;
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = nullptr;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = nullptr;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetSaveFileName(&ofn) == TRUE)
    {
        // use ofn.lpstrFile here
        result = ofn.lpstrFile;
    }

    delete[](filter);

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