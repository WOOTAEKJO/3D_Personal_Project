#pragma once
#include "VIBuffer_Cell.h"
#include "VIBuffer_DCell.h"
#include "VIBuffer_Rect.h"
#include "VIBuffer_DRect.h"
#include "VIBuffer_Cube.h"
#include "VIBuffer_Terrain.h"
#include "VIBuffer_DTerrain.h"
#include "VIBuffer_Particle_Rect.h"
#include "VIBuffer_Particle_Point.h"
#include "Model_Instancing.h"
#include "Model.h"
#include "Shader.h"
#include "StateMachine.h"
#include "Transform.h"
#include "Texture.h"
#include "AICom.h"
#include "Navigation.h"
#include "RigidBody.h"
#include "Collider.h"
#include "Controller.h"

BEGIN(Engine)

class CComponent_Manager final : public CBase
{
public:
	typedef	map<const wstring, class CComponent*> PROTOTYPE;
private:
	CComponent_Manager();
	virtual	~CComponent_Manager()=default;

public:
	HRESULT	Initialize(const _uint& iLevelNum);
	HRESULT	Add_Component_ProtoType(const _uint& iLevelIndex,const wstring& strProtoTypeTag, class CComponent* pComponent);
	class CComponent*	Add_Component_Clone(const _uint& iLevelIndex, const wstring& strProtoTypeTag,void* pArg);
	void	Clear(_uint iLevelIndex);

public:
	PROTOTYPE Get_Com_ProtoType(const _uint& iLevelIndex);
	
private:
	map<const wstring, class CComponent*>* m_mapCom_ProtoType;
	

private:
	_uint				m_iLevelNum = { 0 };

private:
	class CComponent*	Find_Com_ProtoType(const _uint& iLevelIndex, const wstring& strProtoTypeTag);

public:
	static	CComponent_Manager* Create(const _uint& iLevelNum);
	virtual	void	Free() override;
};

END