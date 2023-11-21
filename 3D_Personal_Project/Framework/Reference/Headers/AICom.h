#pragma once
#include "Component.h"

#include "Builder.h"
#include "CompositeBuilder.h"
#include "DecoratorBuilder.h"

BEGIN(Engine)

class CNode;

class ENGINE_DLL CAICom final : public CComponent
{
public:
	typedef	struct tagAI_Desc
	{
		CNode**		ppNodes;
		wstring*	pstrNodeTags;
		_uint		iNodeNum;

	}AIDESC;

private:
	CAICom(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CAICom(const CAICom& rhs);
	virtual	~CAICom() = default;

public:
	virtual	HRESULT	Initialize_ProtoType();
	virtual	HRESULT	Initialize(void* pArg);

public:
	virtual void	Priority_Tick(_float fTimeDelta) {};
	virtual void	Tick(_float fTimeDelta) {
		if (m_pCurrentTree != nullptr)
			m_pCurrentTree->Tick();
	};
	virtual void	Late_Tick(_float fTimeDelta) {};

public:
	HRESULT	Set_Tree(const wstring& strTreeTag);

private:
	map<const wstring, CNode*>	m_mapTree;
	CNode*						m_pCurrentTree = { nullptr };

private:
	CNode* Find_Tree(const wstring& strTreeTag);

public:
	static	CAICom*		Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual	CComponent* Clone(void* pArg) override;
	virtual	void		Free() override;
};

END