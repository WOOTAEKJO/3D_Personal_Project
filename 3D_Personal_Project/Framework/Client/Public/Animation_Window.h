#pragma once
#include "ImGui_Window.h"

BEGIN(Client)

class CAnimMesh_Demo;
class CSubObject_Demo;
class CParticle_Demo;

class CAnimation_Window final : public CImGui_Window
{
public:
	enum ANIMATIONMODE { MODE_ANIMATION, MODE_SUBOBJECT,MODE_PARTICLE, MODE_END };

private:
	CAnimation_Window(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	~CAnimation_Window() = default;

public:
	virtual	HRESULT	Initialize(void* pArg) override;
	virtual	void	Tick() override;
	virtual	HRESULT	Render() override;
	virtual	void	Set_Variable(void* pArg);
	virtual	void	Terrain_Picked(_float4 vPickPoint) override;
	virtual	void	Demo_Picked() override;
	virtual	string	Get_Path() override;
	virtual	HRESULT	Save_Data(const _char* strFilePath) override;
	virtual	HRESULT	Load_Data(const _char* strFilePath) override;

public:
	virtual void Write_Json(json& Out_Json) override;
	virtual void Load_FromJson(const json& In_Json) override;

private:
	vector<wstring>		m_vecSubPrototypeTag;
	wstring				m_strPickSubModelTag;
	vector<CSubObject_Demo*>		m_vecSubObjectDemo;

	_uint				m_iCurrentDemoIndex = { 0 };
	_uint				m_iCurrentAnimModelBoneIndex = { 0 };

	ANIMATIONMODE		m_eCurrentMode = { MODE_END };

	CAnimMesh_Demo*		m_pCurrentAnimModel = { nullptr };


	vector<_uint>		m_vecAnimationIndex;
	_uint	m_iCurrent_Anim_Count = { 0 };
	_bool	m_bReserve = { false };

private:
	vector<wstring>				m_vecParticlePrototypeTag;
	wstring						m_strPickParticleTag;
	vector<CParticle_Demo*>		m_vecParticleDemo;
	_uint						m_iCurrentParticleIndex = { 0 };

	_bool			m_bUpdate = { false };
	_bool			m_bTrans = { false };


private:
	vector<wstring>					m_vecAnimPrototypeTag;
	wstring							m_strPickModelTag;
	_uint							m_iCurrentAnimation = { 0 };

private:
	_int	m_iTransformRadioButton = { 0 };

private:
	_bool	m_bNonBlend = { false };

private:
	void	Animation();
	void	SubObject();
	void	Particle();

private:
	void	Create_Animation_Model(const wstring& strModelTag);
	void	Create_Sub_Model(const wstring& strSubModelTag);
	void	Create_Particle(const wstring& strParticleTag);
	void	Delete_Particle();

	_bool	Reserve_Animation();

public:
	static CAnimation_Window* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,void* pArg);
	virtual	void	Free() override;
};

END