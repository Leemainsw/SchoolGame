#pragma once


class CTexture
{
protected:
	LPDIRECT3DTEXTURE9 m_pTexture;

public:
	bool Create(LPDIRECT3DDEVICE9 pD3DDevice, const TCHAR* filename);

	LPDIRECT3DTEXTURE9 GetTexture() const
	{
		return m_pTexture;
	}

	void Release();

	CTexture();
	CTexture(LPDIRECT3DDEVICE9 pD3DDevice, const TCHAR* filename);
	virtual ~CTexture();
};

