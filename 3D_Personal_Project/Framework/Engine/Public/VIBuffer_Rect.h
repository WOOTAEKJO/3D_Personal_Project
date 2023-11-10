#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Rect final : public CVIBuffer
{
private:
	CVIBuffer_Rect(ID3D11Device* pDevice,ID3D11DeviceContext* pContext);
	CVIBuffer_Rect(const CVIBuffer_Rect& rhs);
	virtual	~CVIBuffer_Rect() = default;
public:
	virtual	HRESULT	Initialize_ProtoType() override;
	virtual	HRESULT	Initialize(void* pArg) override;
private:

public:
	static	CVIBuffer_Rect* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual	CComponent*		Clone(void* pArg) override;
	virtual	void			Free() override;
};

END

/*
	- 사각형을 그리는 클래스
	- 프로토 타입 클래스를 상속 받음
	- 클라이언트에서 알고 있어야 함
*/