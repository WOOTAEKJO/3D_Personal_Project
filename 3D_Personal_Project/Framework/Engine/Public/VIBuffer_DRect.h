#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_DRect final : public CVIBuffer
{
public:
	enum DIRECTION { POSTIVE_X, NEGATIVE_X ,POSTIVE_Y, NEGATIVE_Y,CENTER,DIRECTION_END};

private:
	CVIBuffer_DRect(ID3D11Device* pDevice,ID3D11DeviceContext* pContext);
	CVIBuffer_DRect(const CVIBuffer_DRect& rhs);
	virtual	~CVIBuffer_DRect() = default;
public:
	virtual	HRESULT	Initialize_ProtoType() override;
	virtual	HRESULT	Initialize(void* pArg) override;

public:
	void	Update_Vertex(DIRECTION eDirection);

private:

public:
	static	CVIBuffer_DRect* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual	CComponent*		Clone(void* pArg) override;
	virtual	void			Free() override;
};

END

/*
	- �簢���� �׸��� Ŭ����
	- ������ Ÿ�� Ŭ������ ��� ����
	- Ŭ���̾�Ʈ���� �˰� �־�� ��
*/