#include <d3d9.h>
#include <d3dx9.h>
#include <map>
#include <string>

#include "CTexture.h"

bool CTexture::Create(LPDIRECT3DDEVICE9 pD3DDevice, const TCHAR* filename)
{
	auto hr = D3DXCreateTextureFromFileEx(pD3DDevice, filename
		, 0, 0, 0, 0
		, D3DFMT_A8R8G8B8
		, D3DPOOL_DEFAULT
		, D3DX_FILTER_NONE
		, D3DX_DEFAULT
		, D3DCOLOR_XRGB(0, 0, 0)
		, NULL, NULL
		, &m_pTexture);


	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

void CTexture::Release()
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

CTexture::CTexture() :m_pTexture(NULL)
{

}

CTexture::CTexture(LPDIRECT3DDEVICE9 pD3DDevice, const TCHAR* filename)
{
	Create(pD3DDevice, filename);
}

CTexture::~CTexture()
{
	Release();
}
