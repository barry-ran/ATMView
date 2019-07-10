/***********************************************************************************
*								SystemChecker.cpp
*
* Copyright(C):	�����ǻ��ۿƼ��ɷ����޹�˾
*
* Author:  HJM,TZH
*
* Date: 2018-12
*
* Description: ϵͳMAC��ַ�����̼��ӿڷ�װ
***********************************************************************************/
#include "SystemChecker.h"
#include <Windows.h>
#include <QtNetwork/QNetworkInterface>
#include <QtCore/QFileInfo>
#include <ShlObj.h>
#include <TlHelp32.h>
#include <IPTypes.h>
#include <IPHlpApi.h>
#include <string>

using std::string;

#define AIO_LAUNCHER_NAME "ATMLauncher.exe"
#define AIO_SYS_DEF_KEYBOARD_PROCESS "TabTip.exe"

#pragma comment(lib, "shell32.lib")

QString SystemChecker::getCurrentMacAddr()
{
	QString macAddr;
	QList<QNetworkInterface> netInterfaces = QNetworkInterface::allInterfaces();//��ȡ����������Ϣ  
	for (int i = 0; i < netInterfaces.count(); i++)  {  
		QNetworkInterface netInterface = netInterfaces.at(i);  
		if (netInterface.flags().testFlag(QNetworkInterface::IsUp) 
			&& netInterface.flags().testFlag(QNetworkInterface::IsRunning) 
			&& !netInterface.flags().testFlag(QNetworkInterface::IsLoopBack)) { 
			//��ȡ��ǰ��Ч����  
            //���ѭ������Ҫ�����߿��Գ��Բ������ѭ��������ȡ���ǲ��ǵ�ǰ��Ч��������  
			for (int j = 0; j < netInterface.addressEntries().count(); j++) {
				/* we have an interface that is up, and has an ip address therefore the link is present 
				* we will only enable this check on first positive, all later results are incorrect * * */  
				macAddr = netInterface.hardwareAddress();  
				i = netInterfaces.count(); //�������ѭ��  
				break;  
			}  
		}  
	}  

	if(macAddr.isEmpty()) {//��ʱ���ǻ�ȡ��������������ٴγ��Ի�ȡ  
		foreach(QNetworkInterface netInterface, netInterfaces) {  
			if(!netInterface.flags().testFlag(QNetworkInterface::IsLoopBack)) {  
				macAddr = netInterface.hardwareAddress();  
				break;  
			}  
		}  
	}  
	return macAddr;
}

bool SystemChecker::isAppInstanceExisted() 
{
    string mutexName = "ATMView";
    HANDLE mutex = CreateMutexA(NULL, FALSE, mutexName.c_str());

    DWORD lastError = GetLastError();
    if (ERROR_ALREADY_EXISTS == lastError) {
        wchar_t moduleFileName[MAX_PATH];
        GetModuleFileName(NULL, moduleFileName, MAX_PATH);

        DWORD pid;
        std::wstring name = moduleFileName;
        name = name.substr(name.rfind('\\') + 1);
        if(getProcessIdByName(name.c_str(), &pid)) {
            CloseHandle(mutex);
            mutex = NULL;
            return true;	
        } else {
            //LOG_INFO("MBRClient ALREADY_EXISTS MutexName:%s GetProcessIdByName RetFlg:%d", strMutexName.c_str(), bRet);
        }
    }

    return false;
}

bool SystemChecker::getProcessIdByName(const wchar_t* szProcessname, unsigned long* lpPID) 
{
	bool ret = false;

	PROCESSENTRY32 processEntry;
	ZeroMemory(&processEntry, sizeof(PROCESSENTRY32));
	processEntry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(INVALID_HANDLE_VALUE == snapshot) {
		return ret;
	}

	if(!Process32First(snapshot, &processEntry)) {
		return ret;
	}

	do {
		if(0 == lstrcmpi(processEntry.szExeFile, szProcessname)) {
			*lpPID = processEntry.th32ProcessID;
			CloseHandle(snapshot);
			snapshot = NULL;
			ret = true;
			break;
		}
	} while(Process32Next(snapshot, &processEntry));

	if(NULL != snapshot) {
		CloseHandle(snapshot);
		snapshot = NULL;
	}

	return ret;
}

bool SystemChecker::isLauncherInstanceExisted() 
{
    QString processName;
 
    MultiByteToWideChar(CP_ACP, 0, AIO_LAUNCHER_NAME, -1,   (LPWSTR)processName.toStdWString().c_str(), MAX_PATH);

	DWORD pid;
    if (getProcessIdByName(reinterpret_cast<const wchar_t *>(processName.utf16()),&pid)) {
		return true;
	}

	return false;
}

void SystemChecker::shutdownLauncherInstance()
{
	//�ر�����������
    QString processName;
    MultiByteToWideChar(CP_ACP, 0, AIO_LAUNCHER_NAME, -1, (LPWSTR)processName.toStdWString().c_str(), MAX_PATH);
	
	DWORD pid = 0;
    bool bRet = getProcessIdByName(reinterpret_cast<const wchar_t *>(processName.utf16()), &pid);

	if (bRet) {
		HANDLE hProce = OpenProcess(PROCESS_TERMINATE, FALSE, pid); 
		TerminateProcess(hProce,0);
		hProce = NULL;
	}
}

bool SystemChecker::restartLauncherInstance()
{
    return false;
	//��������������
	char szLauncherExePath[MAX_PATH];
	memset(szLauncherExePath, 0, MAX_PATH);	
	::GetModuleFileNameA(NULL, szLauncherExePath, sizeof(szLauncherExePath));
	char *ptr = strrchr(szLauncherExePath, '\\');
	ptr[1] = '\0';

	strcat(szLauncherExePath,AIO_LAUNCHER_NAME);
	HINSTANCE hInst = ShellExecuteA(NULL,"open",szLauncherExePath, NULL, NULL,SW_HIDE);

	if ((int)hInst > 32) {
		return true;
	}

	return false;
}

bool SystemChecker::isSystemDefKeyBoardInstanceExisted()
{
    QString processName;

    MultiByteToWideChar(CP_ACP, 0, AIO_SYS_DEF_KEYBOARD_PROCESS, -1, (LPWSTR)processName.toStdWString().c_str(), MAX_PATH);

	DWORD pid;
    if (getProcessIdByName(reinterpret_cast<const wchar_t *>(processName.utf16()),&pid)) {
		return true;
	}

	return false;
}

void SystemChecker::shutdownSystemDefKeyBoardInstance()
{
	//�ر�ϵͳĬ�ϼ��̵Ľ���
    QString processName;
    MultiByteToWideChar(CP_ACP, 0, AIO_SYS_DEF_KEYBOARD_PROCESS, -1, (LPWSTR)processName.toStdWString().c_str(), MAX_PATH);

	DWORD pid = 0;
    bool bRet = getProcessIdByName(reinterpret_cast<const wchar_t *>(processName.utf16()), &pid);

	if (bRet) {
		HANDLE hProce = OpenProcess(PROCESS_TERMINATE, FALSE, pid); 
		TerminateProcess(hProce,0);
		hProce = NULL;
	}
}

bool SystemChecker::restartSystemDefKeyBoardInstance()
{
	//����ϵͳĬ�ϼ��̵Ľ���
	char szExePath[MAX_PATH] = {0};
	strcpy(szExePath,"C:\\Program Files\\Common Files\\Microsoft Shared\\ink\\TabTip.exe");
	HINSTANCE hInst = ShellExecuteA(NULL,"open",szExePath, NULL, NULL,SW_SHOWNORMAL);

	if ((int)hInst > 32) {
		return true;
	}

	return false;
}