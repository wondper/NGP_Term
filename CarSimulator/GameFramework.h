#pragma once
#include "Timer.h"
#include "Scene.h"
#include "Player.h"
#include "ShadowMap.h"


class CGameFramework
{
private:
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	int m_nWndClientWidth;
	int m_nWndClientHeight;

	ComPtr<IDXGIFactory4> m_pdxgiFactory;
	//DXGI ���丮 �������̽��� ���� �������̴�.

	ComPtr<IDXGISwapChain3> m_pdxgiSwapChain;
	//���� ü�� �������̽��� ���� �������̴�. �ַ� ���÷��̸� �����ϱ� ���Ͽ� �ʿ��ϴ�.

	ComPtr<ID3D12Device> m_pd3dDevice;
	//Direct3D ����̽� �������̽��� ���� �������̴�. �ַ� ���ҽ��� �����ϱ� ���Ͽ� �ʿ��ϴ�.

	bool m_bMsaa4xEnable = false;
	UINT m_nMsaa4xQualityLevels = 0;
	//MSAA ���� ���ø��� Ȱ��ȭ�ϰ� ���� ���ø� ������ �����Ѵ�.
	
	static const UINT m_nSwapChainBuffers = 2;
	//ComPtr<ID3D12Resource> m_ppd3dSwapChainBackBuffers[m_nSwapChainBuffers];
	//���� ü���� �ĸ� ������ �����̴�.

	UINT m_nSwapChainBufferIndex;
	//���� ���� ü���� �ĸ� ���� �ε����̴�.

	ComPtr<ID3D12Resource> m_ppd3dRenderTargetBuffers[m_nSwapChainBuffers];
	ComPtr<ID3D12DescriptorHeap> m_pd3dRtvDescriptorHeap;
	UINT m_nRtvDescriptorIncrementSize;
	//���� Ÿ�� ����, ������ �� �������̽� ������, ���� Ÿ�� ������ ������ ũ���̴�.

	ComPtr<ID3D12Resource> m_pd3dDepthStencilBuffer;
	ComPtr<ID3D12DescriptorHeap> m_pd3dDsvDescriptorHeap;
	UINT m_nDsvDescriptorIncrementSize;
	//����-���ٽ� ����, ������ �� �������̽� ������, ����-���ٽ� ������ ������ ũ���̴�.

	ComPtr<ID3D12Resource> m_pd3dShaderResourceBuffer;
	ComPtr<ID3D12DescriptorHeap> m_pd3dSrvDescriptorHeap;
	UINT m_nSrvDescriptorIncrementSize;
	//���̴� ���ҽ� ����, ������ �� �������̽� ������, ���̴� ���ҽ� ������ ������ ũ���̴�.

	ComPtr<ID3D12CommandQueue> m_pd3dCommandQueue;
	ComPtr<ID3D12CommandAllocator> m_pd3dCommandAllocator;
	ComPtr<ID3D12GraphicsCommandList> m_pd3dCommandList;
	//��� ť, ��� �Ҵ���, ��� ����Ʈ �������̽� �������̴�.

	ComPtr<ID3D12PipelineState> m_pd3dPipelineState;
	//�׷��Ƚ� ���������� ���� ��ü�� ���� �������̽� �������̴�.
	ComPtr<ID3D12Fence> m_pd3dFence;
	UINT64 m_nFenceValue;
	HANDLE m_hFenceEvent;
	//�潺 �������̽� ������, �潺�� ��, �̺�Ʈ �ڵ��̴�.

	//������ ���� �����ӿ�ũ���� ����� Ÿ�̸��̴�.
	CGameTimer m_GameTimer;

	//������ ������ ����Ʈ�� �� �������� ĸ�ǿ� ����ϱ� ���� ���ڿ��̴�.
	_TCHAR m_pszFrameRate[50];

	//�������� �������̽�
	std::unique_ptr<btDefaultCollisionConfiguration> m_pbtCollisionConfiguration;
	std::unique_ptr<btCollisionDispatcher> m_pbtDispatcher;
	std::unique_ptr<btBroadphaseInterface> m_pbtOverlappingPairCache;
	std::unique_ptr<btSequentialImpulseConstraintSolver> m_pbtSolver;
	std::unique_ptr<btDiscreteDynamicsWorld> m_pbtDynamicsWorld;
	btAlignedObjectArray<btCollisionShape*> m_btCollisionShapes;

	UINT64 m_nFenceValues[m_nSwapChainBuffers];
	std::unique_ptr<CScene> m_pScene;

	std::unique_ptr<CShadowMap> m_pShadowMap[3];

public:
	CCamera* m_pCamera = NULL;

	//�÷��̾� ��ü�� ���� �������̴�.
	std::shared_ptr<CVehiclePlayer> m_pPlayer;
	std::shared_ptr<CGameObject> m_pOtherPlayer[2];
	std::shared_ptr<CGameObject> m_pOtherPlayerBullet[2];

	//���������� ���콺 ��ư�� Ŭ���� ���� ���콺 Ŀ���� ��ġ�̴�.
	POINT m_ptOldCursorPos;

public:
	CGameFramework();
	~CGameFramework();

	CGameTimer GetGameTimer() { return m_GameTimer; }

	bool OnCreate(HINSTANCE hInstance, HWND hMainWnd);
	//�����ӿ�ũ�� �ʱ�ȭ�ϴ� �Լ��̴�(�� �����찡 �����Ǹ� ȣ��ȴ�).

	void OnDestroy();
	void CreateSwapChain();
	void CreateRtvAndDsvDescriptorHeaps();
	void BuildDescriptorHeaps();
	void CreateDirect3DDevice();
	void CreateCommandQueueAndList();
	//���� ü��, ����̽�, ������ ��, ��� ť/�Ҵ���/����Ʈ�� �����ϴ� �Լ��̴�.

	void CreateRenderTargetViews();
	void CreateDepthStencilView();
	//���� Ÿ�� ��� ����-���ٽ� �並 �����ϴ� �Լ��̴�.

	void BuildObjects();
	void ReleaseObjects();
	//�������� �޽��� ���� ��ü�� �����ϰ� �Ҹ��ϴ� �Լ��̴�.

	//�����ӿ�ũ�� �ٽ�(����� �Է�, �ִϸ��̼�, ������)�� �����ϴ� �Լ��̴�.
	void ProcessInput();
	void Update();
	void FrameAdvance();

	//�������� �ʱ�ȭ
	void BulletInit();

	XMFLOAT3 GetPlayerPosition() { return m_pPlayer->GetPosition(); }
	XMFLOAT4 GetPlayerRotation()
	{
		XMFLOAT4X4 transform = m_pPlayer->GetWorldTransformMatrix();
		float pitch = XMScalarASin(-transform._32);

		XMVECTOR from(XMVectorSet(transform._12, transform._31, 0.0f, 0.0f));
		XMVECTOR to(XMVectorSet(transform._22, transform._33, 0.0f, 0.0f));
		XMVECTOR res(XMVectorATan2(from, to));

		float roll = XMVectorGetX(res);
		float yaw = XMVectorGetY(res);

		auto quaternion = m_pPlayer->GetVehicle()->getChassisWorldTransform().getRotation();

		return XMFLOAT4(quaternion.x(), quaternion.y(), quaternion.z(), quaternion.w());
	}
	int GetPlayerLife() { return m_pPlayer->m_nLife; }
	std::shared_ptr<CBullet> GetPlayerBullet() { return m_pPlayer->GetBullet(); }

	//CPU�� GPU�� ����ȭ�ϴ� �Լ��̴�.
	void WaitForGpuComplete();

	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	//�������� �޽���(Ű����, ���콺 �Է�)�� ó���ϴ� �Լ��̴�.

	void ChangeSwapChainState();
	void MoveToNextFrame();

// ����, ��Ʈ��ũ�� ���õ� ������ �Լ��� �Ʒ��� �ۼ��Ѵ�.
private:
	SOCKET m_clientSocket;

public:
	void InitNetworkSocket(CVehiclePlayer* pPlayer) { }
};