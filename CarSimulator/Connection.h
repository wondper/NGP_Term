#pragma once
#include "stdafx.h"
#include "GameFramework.h"
using namespace std;

// ���� ���� ����
extern CGameFramework gGameFramework;	// ���������ӿ�ũ
extern array<HANDLE, 2> g_events;		// ����, �۽� ������ ����ȭ �̺�Ʈ
extern bool g_bGameStarted;				// ������ ���۵Ǹ� true�� �ٲ�
extern bool g_bGameOver;				// ������ ����Ǹ� true�� �ٲ�

// ������ �Լ�
DWORD WINAPI TransportData(LPVOID arg);

// �۽� �Լ�
void SendPlayerInfo(const SOCKET& sock);

// ���� �Լ�
int RecvN(const SOCKET& socket, char* buffer, int length, int flags);
void RecvGameStart(const SOCKET& sock);
void RecvPlayerInfo(const SOCKET& sock);
void RecvGameOver(const SOCKET& sock);

// ��Ÿ
void InitConnection();