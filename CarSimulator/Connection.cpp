#include "Connection.h"

DWORD WINAPI TransportData(LPVOID arg)
{
    SOCKET sock{ (SOCKET)arg };
    int msg{};

    // ���� ��ȣ�� ��ٸ�
    while (TRUE)
    {
        RecvN(sock, (char*)&msg, sizeof(int), 0);
        if (msg & GAME_START)
        {
            RecvGameStart(sock);
            break;
        }
    }

    while (TRUE)
    {
        // �ۼ��� ������ ���ʰ� �� ������ ���
        WaitForSingleObject(g_events[1], INFINITE);

        // ������ �÷��̾� ���� �۽�
        SendPlayerInfo(sock);

        // �����κ��� �޽��� ����
        RecvN(sock, (char*)&msg, sizeof(int), 0);
        gGameFramework.m_pPlayer->SetNextFrameMessage(msg);

        // �ٸ� �÷��̾� ������ �������� ���
        if (msg & PLAYER_UPDATE)
        {
            RecvPlayerInfo(sock);
        }
        
        // ���� ���Ḧ �������� ���
        if (msg & GAME_OVER)
        {
            RecvGameOver(sock);
            ResetEvent(g_events[1]);
            SetEvent(g_events[0]);
            break;
        }

        ResetEvent(g_events[1]);
        SetEvent(g_events[0]);
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}

void SendPlayerInfo(const SOCKET& sock)
{
    // �÷��̾� ���� :: ��ġ, ȸ��, ���, �Ѿ���ȿ, �Ѿ���ǥ
    PlayerData playerInfo{ gGameFramework.GetPlayerPosition(), gGameFramework.GetPlayerRotation(), gGameFramework.GetPlayerLife(), FALSE, { 0, 0, 0 } };
    if (std::shared_ptr<CBullet> bullet = gGameFramework.GetPlayerBullet())
    {
        playerInfo.m_bHasBullet = TRUE;
        playerInfo.m_bulletPosition = bullet->GetPosition();
    }

    // ����� ���� ��� �������� ����
    if (gGameFramework.GetPlayerLife() == 0)
        playerInfo.m_position = { 0, 0, -300 };

    // PLAYER_UPDATE ��Ŷ�� �Բ� �۽�
    int msg{ PLAYER_UPDATE };
    send(sock, (char*)&msg, sizeof(int), 0);
    send(sock, (char*)&playerInfo, sizeof(PlayerData), 0);
}

int RecvN(const SOCKET& socket, char* buffer, int length, int flags)
{
    char* ptr{ buffer };
    int received{ 0 }, left{ length };

    while (left > 0)
    {
        received = recv(socket, ptr, left, flags);
        if (received == SOCKET_ERROR)
            return SOCKET_ERROR;
        if (received == 0)
            break;
        left -= received;
        ptr += received;
    }
    return length - left;
}

void RecvGameStart(const SOCKET& sock)
{
    // ���ӽ����� �˸�
    g_bGameStarted = true;

    // ������ǥ ���� �� �÷��̾� ��ġ ����
    XMFLOAT3 StartPos;
    RecvN(sock, (char*)&StartPos, sizeof(XMFLOAT3), 0);
    gGameFramework.m_pPlayer->SetRigidBodyPosition(StartPos);
    gGameFramework.m_pPlayer->SetSpawPosition(StartPos);
}

void RecvPlayerInfo(const SOCKET& sock)
{
    RecvN(sock, (char*)&g_otherPlayersData[0], sizeof(PlayerData), 0);
    RecvN(sock, (char*)&g_otherPlayersData[1], sizeof(PlayerData), 0);
}

void RecvGameOver(const SOCKET& sock)
{
    // ���� ���Ḧ �˸�
    g_bGameOver = true;

    int msg{ GAME_OVER };
    send(sock, (char*)&msg, sizeof(int), 0);

    if (gGameFramework.GetPlayerLife() > 0)
        MessageBox(NULL, TEXT("�¸�"), TEXT("��������"), 0);
    else
        MessageBox(NULL, TEXT("�й�"), TEXT("��������"), 0);
}

void InitConnection()
{
    // ���������
    LPWSTR* szArgList; int argCount;
    //�������� ����� ���ڸ� �޾� ������ �����Ͽ� ����    //szArgList = CommandLineToArgvW(GetCommandLine(), &argCount);
    // ����� ���ڷκ��� ip, port���
    char ip[15 + 1], port[4 + 1];
    //wcstombs(ip, szArgList[1], wcslen(szArgList[1]) + 1);
    //wcstombs(port, szArgList[2], wcslen(szArgList[2]) + 1);
    //���� �Կ� �� ĸó�� ���� �ӽ� ��Ʈ��ȣ�� ���� ip�ּҸ� ����
    strcpy(ip, "127.0.0.1");
    strcpy(port, "9000");
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        MessageBox(NULL, TEXT("���� �ʱ�ȭ ����"), TEXT("����"), 0);
        exit(0);
    }

    SOCKET sock{ socket(AF_INET, SOCK_STREAM, 0) };
    if (sock == INVALID_SOCKET)
    {
        MessageBox(NULL, TEXT("���� �ʱ�ȭ ����"), TEXT("����"), 0);
        exit(0);
    }

    // ���̱� �˰��� ��
    BOOL flag{ TRUE };
    setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(BOOL));

    SOCKADDR_IN serveraddr{};
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(ip);
    serveraddr.sin_port = htons(atoi(port));
    if (connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr)) == SOCKET_ERROR)
    {
        MessageBox(NULL, TEXT("���� ���� ����"), TEXT("����"), 0);
        exit(0);
    }
    CreateThread(NULL, 0, TransportData, (LPVOID)sock, 0, NULL);
}