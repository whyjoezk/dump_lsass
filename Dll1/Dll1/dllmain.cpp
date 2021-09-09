#include <windows.h>
#include <iostream>
#include <process.h>
#include <Dbghelp.h>
//#pragma comment(lib, "dbghelp")
using namespace std;

typedef HRESULT(WINAPI* _MiniDumpWriteDump)(
    HANDLE                            hProcess,
    DWORD                             ProcessId,
    HANDLE                            hFile,
    MINIDUMP_TYPE                     DumpType,
    PMINIDUMP_EXCEPTION_INFORMATION   ExceptionParam,
    PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
    PMINIDUMP_CALLBACK_INFORMATION    CallbackParam
    );
HANDLE OpenFileHandle(LPCTSTR FileName)
{
    HANDLE FileHandle;

    FileHandle = CreateFile(
        FileName,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_HIDDEN,
        NULL
    );
    if (FileHandle == INVALID_HANDLE_VALUE)
    {
        std::cout << "Failed to get file handle" << std::endl;
        std::cout << GetLastError() << std::endl;
    }

    return FileHandle;
}

//bool MiniDumpProc(DWORD procid, LPCTSTR FileName)
bool MiniDumpProc()
{
    DWORD procid = (DWORD)_getpid();
    LPCTSTR FileName = L"C:\\windows\\temp\\1.bin";
    HANDLE FileHandle;
    HANDLE ProcHandle;
    _MiniDumpWriteDump MiniDumpWriteDump;

    // Get a file handle
    FileHandle = OpenFileHandle(FileName);
    // Get a proc handle 
    ProcHandle = GetCurrentProcess();
    MiniDumpWriteDump = (_MiniDumpWriteDump)GetProcAddress(
        LoadLibrary(L"Dbghelp.dll"), "MiniDumpWriteDump");
    https://docs.microsoft.com/en-us/windows/win32/api/minidumpapiset/nf-minidumpapiset-minidumpwritedump
    if (!MiniDumpWriteDump(
        ProcHandle,
        procid,
        FileHandle,
        MiniDumpWithFullMemory,
        NULL,
        NULL,
        NULL
    ))
    {
        std::cout << GetLastError() << std::endl;
        return FALSE;
    }
    CloseHandle(FileHandle);
    CloseHandle(ProcHandle);
    return TRUE;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    try {
        switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH:
            MiniDumpProc();
            break;
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
        }
    }
    catch (bad_alloc& e) {
        return FALSE;
    }
    return TRUE;
}
