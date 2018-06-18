#pragma once
#pragma once
#ifndef WINDOWS_H
#define WINDOWS_H
#include<Windows.h>
#endif

#ifndef WTSAPI32_h
#define WTSAPI32_H
//#include<Wtsapi32.h>
#endif

BOOL EnableShutDownPriv()
{
	HANDLE hToken = NULL;
	TOKEN_PRIVILEGES tkp = { 0 };
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
		return FALSE;
	}
	if (!LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid)) {
		CloseHandle(hToken);
		return FALSE;
	}
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(TOKEN_PRIVILEGES), NULL, NULL)) {
		CloseHandle(hToken);
		return FALSE;
	}
	return TRUE;
}

void ShutdownHost(void) {
	EnableShutDownPriv();
	ExitWindowsEx(EWX_SHUTDOWN, NULL);
	return;
}