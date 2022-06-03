#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS

// Network
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

// C++
#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
using namespace std;

// DirectX
#include <DirectXMath.h>
#include <DirectXCollision.h>
using namespace DirectX;

// ��Ŷ
#define GAME_START		0b00001
#define GAME_OVER		0b00010
#define PLAYER_UPDATE	0b00100
#define PLAYER_HIT		0b01000
#define BULLET_DELETED	0b10000

// ���� ��Ʈ
#define SERVER_PORT		9000

// ����ü ����
#pragma pack(1)
struct PlayerData
{
	XMFLOAT3 position; 			// �÷��̾� ��ġ
	XMFLOAT4 rotate;			// �÷��̾� ȸ�� ����(roll, pitch, yaw)
	int		 life;				// �÷��̾��� ��� ��
	bool	 hasBullet;			// �Ѿ� ����
	XMFLOAT3 bulletPosition;	// �Ѿ� ��ġ

	PlayerData() : position{}, rotate{}, life{}, hasBullet{}, bulletPosition{}
	{

	}

	PlayerData(XMFLOAT3 _position, XMFLOAT4 _rotate, int _life, bool _hasBullet, XMFLOAT3 _bulletPosition)
		: position{ _position }, rotate{ _rotate }, life{ _life }, hasBullet{ _hasBullet }, bulletPosition{ _bulletPosition }
	{

	}
};

struct ThreadFuncParam
{
	int     id;
	SOCKET  sock;
};

// ������ �Լ�
DWORD WINAPI ProcessClientData(LPVOID arg);
DWORD WINAPI CheckGameOver(LPVOID arg);

// ���� �Լ�
int RecvN(const SOCKET& socket, char* buffer, int length, int flags);
void RecvPlayerInfo(ThreadFuncParam* param);

// �۽� �Լ�
void SendGameStart(ThreadFuncParam* param);
void SendPlayerInfo(ThreadFuncParam* param, int msg);

// �� �� �Լ�
bool isCollided(int playerIndex, int bulletIndex);
bool isBulletHit(int index);
bool isPlayerHit(int index);
bool isGameOver();