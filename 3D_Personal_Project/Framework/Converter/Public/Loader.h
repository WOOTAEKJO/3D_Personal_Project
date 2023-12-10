#pragma once

#include "Converter_Defines.h"
#include "Base.h"

/* �����带 �����Ѵ�. */
/* ������ ������� �ʿ��� ������ �ڿ��� �ε��Ѵ�. */

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Converter)

class CLoader final : public CBase
{
private:
	CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLoader() = default;

public:

	_bool isFinished() const {
		return m_isFinished;  
	}
	
public:	
	HRESULT Initialize(LEVEL eNextLevelID);

	void Print_LoadingText();

public:
	HRESULT Loading();
	HRESULT Loading_For_Converter_Level();

private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };
	CGameInstance*			m_pGameInstance = { nullptr };
	// �� ������ �������� ����
private:
	HANDLE					m_hThread;				// ���� ������
	CRITICAL_SECTION		m_CriticalSection;		// ũ��Ƽ�� ����

private:
	LEVEL					m_eNextLevelID = { LEVEL_END };		// � ������ �ڿ��� �ε��ؾ� �ϴ����� �˷���
	_tchar					m_szLoadingText[MAX_PATH] = TEXT("");
	_bool					m_isFinished = { false }; // ���� ������ ���� �ڿ��� ��� �ε��ߴ����� �Ǵ�

public:
	static CLoader * Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, LEVEL eNextLevelID);
	virtual void Free() override;
	
};

END