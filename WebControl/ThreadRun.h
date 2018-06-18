#pragma once

#ifndef WINDOWS_H
#define WINDOWS_H
#include<Windows.h>
#endif

#ifndef TLHELP32_H
#define TLHELP32_H
#include <TlHelp32.h>
#endif

BOOL KillProcessbyPid(DWORD ProcessId)
{
	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, ProcessId);
	if (hProcess == NULL)
		return FALSE;
	if (!TerminateProcess(hProcess, 0))
		return FALSE;
	return TRUE;
}

bool KillProcessbyName(char *threadname)				//��������Ѱ����֮ƥ��Ľ��̣�ʹ�ý��̹ر�
{
	HANDLE         hProcessSnap;   // ���̿��վ��
	HANDLE         hProcess;       // ���̾��
	PROCESSENTRY32 stcPe32 = { 0 };  // ���̿�����Ϣ
	stcPe32.dwSize = sizeof(PROCESSENTRY32);

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
		return false;

	if (!Process32First(hProcessSnap, &stcPe32))
	{
		CloseHandle(hProcessSnap);
		return false;
	}
	do {
		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE,
			stcPe32.th32ProcessID);
		if (hProcess)
		{
			int nPriority = GetPriorityClass(hProcess);

			CloseHandle(hProcess);                       
		}
		if (strcmp(threadname, stcPe32.szExeFile) == 0) {
			KillProcessbyPid(stcPe32.th32ProcessID);
		}

	} while (Process32Next(hProcessSnap, &stcPe32));
	SetLastError(0);
	CloseHandle(hProcessSnap);
	return true;
}