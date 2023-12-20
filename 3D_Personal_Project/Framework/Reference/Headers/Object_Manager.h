#pragma once
#include "Base.h"

BEGIN(Engine)

class CObject_Manager final : public CBase
{
private:
	CObject_Manager();
	virtual ~CObject_Manager() = default;

public: 
	HRESULT	Initialize(_uint iNumLevel); // ���⼭ �纻��ü���� �����̳� �����迭
	HRESULT	Add_ProtoType(const wstring& strProtoTypeTag,class CGameObject* pGameObeject);
	HRESULT	Add_Clone(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strProtoTypeTag, CGameObject** ppOut, void* pArg);
	CGameObject* Add_Independent_Clone(const wstring& strProtoTypeTag, void* pArg);
	void	Priority_Tick(_float fTimeDelta);
	void	Tick(_float fTimeDelta);
	void	Late_Tick(_float fTimeDelta);
	void	Clear(_uint iLevelIndex);

public:
	_uint	Get_Current_Level() { return m_iCurrentLevel; }
	void	Set_Current_Level(_uint iLevel) { m_iCurrentLevel = iLevel; }

private:
	map<const wstring, class CGameObject*> m_mapProtoType;				// ���� ��ü ����
	map<const wstring, class CLayer*>* m_mapLayer = {nullptr};		   // �纻 ��ü ����
														   // �����迭�� �Ҵ��� ���̱� ������ ������������ ����
	typedef map<const wstring, class CLayer*> LAYERS;
private:
	_uint	m_iNumLevel = {0};

private:
	_uint	m_iCurrentLevel = { 0 };

private:
	class CGameObject* Find_ProtoType(const wstring& strProtoTypeTag);
	class CLayer* Find_Layer(_uint iLevelIndex, const wstring& strLayerTag);
	// �ߺ� Ȯ��
public:
	static	CObject_Manager*	Create(_uint iNumLevel);
	virtual	void				Free() override;
};

END

/*
	����
		- ������ü�� ���� �� ����
		- �纻��ü�� ���� �� ����
		- �纻��ü���� Tick ����
		- Ư�� ������ �纻��ü�� ����
*/