#pragma once
#include "Converter_Defines.h"
#include "GameObject.h"

BEGIN(Converter)

class CConverter_Model;

class CMesh_Demo final : public CGameObject
{
public:
	typedef struct tagObjectMeshDemoValue
	{
		wstring		strModelTag;
		_float4		vPos;

	}OBDEMOVALUE;

private:
	CMesh_Demo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMesh_Demo(const CMesh_Demo& rhs);
	virtual	~CMesh_Demo() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT	Binarization(const _char* strPath);

	wstring	Get_ModelTag() { return m_strModelTag; }
private:
	CConverter_Model*		m_pModelCom = { nullptr };

private:
	wstring		m_strModelTag;

private:
	 HRESULT	Bind_ShaderResources();
	 HRESULT	Ready_Component() ;

public:
	static	CMesh_Demo*	 Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	CGameObject* Clone(void* pArg) override;
	virtual	void	Free() override;
};

END