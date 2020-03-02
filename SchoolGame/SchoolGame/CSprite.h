#pragma once

class CSprite
{
protected:
	//스프라이트 객체생성
	LPD3DXSPRITE m_Sprite;

public:
	bool Create(LPDIRECT3DDEVICE9 pD3DDevice);
	void Draw(LPDIRECT3DTEXTURE9 pTex, const D3DXVECTOR3 cen, const D3DXVECTOR3 pos);
	void Release();

	CSprite();
	CSprite(LPDIRECT3DDEVICE9 pD3DDevice);
	virtual ~CSprite();
};

