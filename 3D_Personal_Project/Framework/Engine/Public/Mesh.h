#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class CMesh final : public CVIBuffer
{
private:
	CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMesh(const CMesh& rhs);
	virtual	~CMesh() = default;

public:
	virtual	HRESULT	Initialize_ProtoType(const aiMesh* pMesh);
	virtual	HRESULT	Initialize(void* pArg);

private:

public:
	static	CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const aiMesh* pMesh);
	virtual	CComponent* Clone(void* pArg) override;
	virtual	void	Free() override;
};

END