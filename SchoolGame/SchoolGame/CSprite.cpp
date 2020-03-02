#include <d3d9.h> //DirectX 헤더 추가1
#include <d3dx9.h> //DirectX 헤더 추가2

#include "CSprite.h"

bool CSprite::Create(LPDIRECT3DDEVICE9 pD3DDevice)
{
	//에러코드
	HRESULT hr = D3DXCreateSprite(pD3DDevice, &m_Sprite);
	if (FAILED(hr))
		return false;

	return true;
}

void CSprite::Draw(LPDIRECT3DTEXTURE9 pTex, const D3DXVECTOR3 cen, const D3DXVECTOR3 pos)
{
	m_Sprite->Begin(D3DXSPRITE_ALPHABLEND);
	m_Sprite->Draw(pTex, NULL, &cen, &pos,
		D3DCOLOR_XRGB(255, 255, 255));
	m_Sprite->End();
}

void CSprite::Release()
{
	if (m_Sprite != NULL)
	{
		m_Sprite->Release();
		m_Sprite = NULL;
	}
}

CSprite::CSprite()
	:m_Sprite(NULL)
{
	m_Sprite = NULL;
}

CSprite::CSprite(LPDIRECT3DDEVICE9 pD3DDevice)
{
	Create(pD3DDevice);
}

CSprite::~CSprite()
{
	Release();
}
