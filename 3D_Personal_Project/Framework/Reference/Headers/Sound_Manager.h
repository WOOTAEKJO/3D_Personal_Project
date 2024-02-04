#pragma once

#include "Base.h"
#include "Engine_Defines.h"

#include <mutex>
#include <future>
#include <thread>
#include <io.h>

BEGIN(Engine)

class ENGINE_DLL FSoundData : public CBase
{
private:
	FSoundData() {}
	~FSoundData() {}

public:
	static FSoundData* Create()
	{
		FSoundData* pInstance = new FSoundData();

		return pInstance;
	}
	void Free()
	{
		mapSound.clear();

		delete this;
	}

public:
	using map_sound = unordered_map<wstring, FMOD_SOUND*>;
	map_sound& Get_MapSound() { return mapSound; }

private:
	map_sound mapSound;
};

class ENGINE_DLL CSound_Manager : public CBase
{
private:
	CSound_Manager();
	virtual ~CSound_Manager() = default;

public:
	HRESULT Initialize();
	HRESULT Ready_Sound();

public:
	// ���� ���
	void Play_Sound(const wstring& strGroupKey, const wstring& strSoundKey, CHANNELID eID,_float fVolume = 1.f,
		_bool bLoop = false);
	// ��� ���
	void Play_BGM(const wstring& strGroupKey, const wstring& strSoundKey, _float fVolume = 1.f);
	// ���� ����
	void Stop_Sound(CHANNELID eID);
	// ��� ���� ����
	void Stop_All();
	// ä���� ���� ����
	void Set_ChannelVolume(CHANNELID eID, float fVolume);

	_bool Is_SoundFinished(CHANNELID eID);

private:
	// ���������� �ε��� ���� ���� ������ ������ �ε��ϴ� �Լ�
	void Load_SoundFile(const wstring& pGroupKey, const string& pPath);
	// ���� ������ �׷� ������ �񵿱� �ε��� �� ���̴� �Լ�
	void Load_SoundFile_GroupAsync(const wstring& pGroupKey, const string& pPath);
	// �񵿱� ó���� ��ٸ��� �Լ�
	void Wait_GroupAsync();
	// ���� ���� ���Ϸ� �񵿱� �ε��� �� ���̴� �Լ���
	FMOD_RESULT LoadSoundFile_Async(const string& pPath, const string& pFileName, FMOD_RESULT& hResult, FMOD_SOUND** pSound);

	// ���� ���ҽ� ������ ���� ��ü 
	unordered_map<wstring, FSoundData*>	m_mapSound;
	mutex							m_mtxSound;
	vector<future<void>>			m_vecAsyncSoundGroup;

	// FMOD_CHANNEL : ����ϰ� �ִ� ���带 ������ ��ü 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL];

	// ���� ,ä�� ��ü �� ��ġ�� �����ϴ� ��ü 
	FMOD_SYSTEM* m_pSystem = nullptr;

	FMOD_CHANNELGROUP* m_pChannelGroup[MAX_CHANNEL_GROUP];
	FMOD_CHANNELGROUP* m_pMasterChanelGroup;

private:
	const char* commonPath = "C:/Users/hmbga/OneDrive/3D_Personal/Client/Bin/Resources/Sound/";

public:
	static CSound_Manager* Create();
	virtual void Free() override;
};

END

/*  Engine::Play_Sound(L"Enemy", L"_PipeHeavySwing.mp3", SOUND_ENEMY_NORMAL_EFFECT, m_tSound.m_fSoundVolume); 
���� �̷��� �����־��� */