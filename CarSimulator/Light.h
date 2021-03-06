#pragma once
#include "GameObject.h"

#define SPOT_LIGHT 1
#define DIRECTIONAL_LIGHT 2
#define POINT_LIGHT 3

class CLight : public CGameObject
{
public:
    int m_itype;
    XMFLOAT3 m_xmf3Strength;
    float m_fFalloffStart;
    XMFLOAT3 m_xmf3Direction;
    float m_fFalloffEnd;
    float m_fSpotPower;

public:
    CLight() = default;
    CLight(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, int type, XMFLOAT3 xmf3Strength, float fFalloffStart, XMFLOAT3 xmf3Direction, float fFalloffEnd, XMFLOAT3 xmf3Position, float fSpotPower);
    ~CLight();
    XMFLOAT3 GetDirection() { return m_xmf3Direction; }
    virtual void Update(float fTimeElapsed, XMFLOAT3 xmf3PlayerPosition);
};