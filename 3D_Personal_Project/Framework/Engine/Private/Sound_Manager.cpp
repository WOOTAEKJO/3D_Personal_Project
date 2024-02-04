#include "Sound_Manager.h"

CSound_Manager::CSound_Manager()
{
}

HRESULT CSound_Manager::Initialize()
{
	Ready_Sound();

	return S_OK;
}

HRESULT CSound_Manager::Ready_Sound()
{
	FMOD_RESULT	result;

	// ���带 ����ϴ� ��ǥ��ü�� �����ϴ� �Լ�
	result = FMOD_System_Create(&m_pSystem, FMOD_VERSION);

	result = FMOD_System_SetOutput(m_pSystem, FMOD_OUTPUTTYPE_ALSA);

	// 1. �ý��� ������, 2. ����� ����ä�� �� , �ʱ�ȭ ���) 
	result = FMOD_System_Init(m_pSystem, 32, FMOD_INIT_NORMAL, NULL);

	result = FMOD_System_CreateChannelGroup(m_pSystem, "BGM_Group", &m_pChannelGroup[BGM_GROUP]);
	result = FMOD_System_CreateChannelGroup(m_pSystem, "SND1_Group", &m_pChannelGroup[SND1_GROUP]);
	result = FMOD_System_CreateChannelGroup(m_pSystem, "SND2_Group", &m_pChannelGroup[SND2_GROUP]);

	result = FMOD_System_GetMasterChannelGroup(m_pSystem, &m_pMasterChanelGroup);

	/* For. BGM */
	Load_SoundFile_GroupAsync(L"BGM", "../Bin/Resources/Sound/StageBGM/");
	Load_SoundFile_GroupAsync(L"Jack", "../Bin/Resources/Sound/Jack/");
	Load_SoundFile_GroupAsync(L"Character", "../Bin/Resources/Sound/Character/");
	Load_SoundFile_GroupAsync(L"Spooketon", "../Bin/Resources/Sound/Spooketon/");
	Load_SoundFile_GroupAsync(L"Puzzle", "../Bin/Resources/Sound/Puzzle/");
	Load_SoundFile_GroupAsync(L"HelicoScarrow", "../Bin/Resources/Sound/HelicoScarrow/");
	Load_SoundFile_GroupAsync(L"Phantom", "../Bin/Resources/Sound/Phantom/");

	return S_OK;
}

void CSound_Manager::Play_Sound(const wstring& strGroupKey, const wstring& strSoundKey, CHANNELID eID,
	_float fVolume, _bool bLoop)
{
	// ī�װ� Ű
	auto iterCate = m_mapSound.find(strGroupKey);
	if (iterCate == m_mapSound.end())
		return;

	// ���� Ű
	auto iter = iterCate->second->Get_MapSound().find(strSoundKey);


	// ������� ���� �÷���
	FMOD_BOOL bPlay = FALSE;

	if (FMOD_Channel_IsPlaying(m_pChannelArr[eID], &bPlay))
	{
		FMOD_System_PlaySound(m_pSystem, iter->second, m_pChannelGroup[SND1_GROUP], FALSE, &m_pChannelArr[eID]);
	}
	else
	{
		FMOD_Channel_Stop(m_pChannelArr[eID]);
		FMOD_System_PlaySound(m_pSystem, iter->second, m_pChannelGroup[SND1_GROUP], FALSE, &m_pChannelArr[eID]);
	}

	if (true == bLoop)
	{
		FMOD_Channel_SetMode(m_pChannelArr[eID], FMOD_LOOP_NORMAL);
	}
	else
		FMOD_Channel_SetMode(m_pChannelArr[eID], FMOD_LOOP_OFF);


	FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume);//fVolume);

	FMOD_System_Update(m_pSystem);
}

void CSound_Manager::Play_BGM(const wstring& strGroupKey, const wstring& strSoundKey, _float fVolume)
{
	// ī�װ� Ű
	auto iterCate = m_mapSound.find(strGroupKey);
	if (iterCate == m_mapSound.end())
		return;

	// ���� Ű
	auto iter = iterCate->second->Get_MapSound().find(strSoundKey);
	if (iter == iterCate->second->Get_MapSound().end())
		return;


	// ������� ���� �÷���
	Stop_Sound(SOUND_BGM);
	FMOD_System_PlaySound(m_pSystem, iter->second, m_pChannelGroup[BGM_GROUP], FALSE, &m_pChannelArr[SOUND_BGM]);
	FMOD_Channel_SetMode(m_pChannelArr[SOUND_BGM], FMOD_LOOP_NORMAL);
	FMOD_Channel_SetVolume(m_pChannelArr[SOUND_BGM], fVolume);//fVolume);
	FMOD_System_Update(m_pSystem);
}

void CSound_Manager::Stop_Sound(CHANNELID eID)
{
	FMOD_Channel_Stop(m_pChannelArr[eID]);
}

void CSound_Manager::Stop_All()
{
	for (int i = 0; i < static_cast<int>(MAXCHANNEL); ++i)
		FMOD_Channel_Stop(m_pChannelArr[i]);
}

_bool CSound_Manager::Is_SoundFinished(CHANNELID eID)
{
	FMOD_BOOL bPlay = FALSE;

	if (FMOD_Channel_IsPlaying(m_pChannelArr[eID], &bPlay)) /* ��������� Ȯ�� */
	{
		return false; /* ����� */
	}

	return true;;
}

void CSound_Manager::Set_ChannelVolume(CHANNELID eID, _float fVolume)
{
	FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume);

	FMOD_System_Update(m_pSystem);
}

void CSound_Manager::Load_SoundFile(const wstring& pGroupKey, const string& pPath)
{
	// m_mapSound ��ȣ
	m_mtxSound.lock();
	// ����� ī�װ� Ű �����, ������ Ű�� ����� �����̳ʸ� ����� �ش�.
	auto iter = m_mapSound.find(pGroupKey);
	if (iter == m_mapSound.end())
	{
		m_mapSound.emplace(pGroupKey, FSoundData::Create());
	}
	m_mtxSound.unlock();

	// ������� ���� �ε��
	_char sText[128] = "";
	strcpy_s(sText, pPath.c_str());
	strcat_s(sText, "*.*");

	// _finddata_t : <io.h>���� �����ϸ� ���� ������ �����ϴ� ����ü
	_finddata_t fd;

	// _findfirst : <io.h>���� �����ϸ� ����ڰ� ������ ��� ������ ���� ù ��° ������ ã�� �Լ�
	auto handle = _findfirst(sText, &fd);

	if (handle == -1)
		return;

	int iResult = 0;

	char szCurPath[128] = "";
	char szFullPath[128] = "";

	strcpy_s(szCurPath, pPath.c_str());

	// �̰� ���� ��
	using sound_tuple = tuple<FMOD_SOUND*, FMOD_RESULT, string, string>;
	enum ETMP_SOUND : int {
		TMP_SOUND,
		TMP_RESULT,
		TMP_PATH,
		TMP_FILE_NAME
	};

	// �񵿱� ó���� �����带 ����ϱ� ������ ��� �����͸� ������ �����̳ʿ� �����ϰ� ó�����ݴϴ�.
	vector<future<FMOD_RESULT>> vecAsync;
	vector<sound_tuple> vecSoundData;

	// ������ �Էº�
	while (iResult != -1)
	{
		strcpy_s(szFullPath, szCurPath);

		// "../Sound/Success.wav"
		strcat_s(szFullPath, fd.name);
		sound_tuple tpSound;

		// �����忡 �Ѱ��� �����͸� �Է�
		get<TMP_PATH>(tpSound) = string(szFullPath);
		get<TMP_FILE_NAME>(tpSound) = string(fd.name);
		get<TMP_RESULT>(tpSound) = FMOD_OK;
		get<TMP_SOUND>(tpSound) = nullptr;

		vecSoundData.push_back(tpSound);

		//_findnext : <io.h>���� �����ϸ� ���� ��ġ�� ������ ã�� �Լ�, ���̻� ���ٸ� -1�� ����
		iResult = _findnext(handle, &fd);
	}

	// ������ ����
	for (size_t i = 0; i < vecSoundData.size(); i++)
	{
		vecAsync.push_back(async(launch::async, &CSound_Manager::LoadSoundFile_Async, this
			, get<TMP_PATH>(vecSoundData[i]).c_str(), get<TMP_FILE_NAME>(vecSoundData[i]).c_str()
			, ref(get<TMP_RESULT>(vecSoundData[i])), &get<TMP_SOUND>(vecSoundData[i])));
	}

	// ��� �޾ƿ���
	for (size_t i = 0; i < vecSoundData.size(); i++)
	{
		vecAsync[i].get();

		m_mtxSound.lock();
		if (get<TMP_RESULT>(vecSoundData[i]) == FMOD_OK)
		{
			int iLength = (int)strlen(get<TMP_FILE_NAME>(vecSoundData[i]).c_str()) + 1;

			_tchar* pSoundKey = new _tchar[iLength];
			ZeroMemory(pSoundKey, sizeof(_tchar) * iLength);

			// �ƽ�Ű �ڵ� ���ڿ��� �����ڵ� ���ڿ��� ��ȯ�����ִ� �Լ�
			MultiByteToWideChar(CP_ACP, 0, get<TMP_FILE_NAME>(vecSoundData[i]).c_str(), iLength, pSoundKey, iLength);

			m_mapSound[pGroupKey]->Get_MapSound().emplace(pSoundKey, get<TMP_SOUND>(vecSoundData[i]));

			Safe_Delete_Array(pSoundKey);
		}
		m_mtxSound.unlock();
	}

	// FMOD ������Ʈ
	FMOD_System_Update(m_pSystem);

	_findclose(handle);
}

void CSound_Manager::Load_SoundFile_GroupAsync(const wstring& pGroupKey, const string& pPath)
{
	m_vecAsyncSoundGroup.push_back(async(launch::async, &CSound_Manager::Load_SoundFile, this, pGroupKey, pPath));
}

void CSound_Manager::Wait_GroupAsync()
{
	for (auto iter = m_vecAsyncSoundGroup.begin(); iter != m_vecAsyncSoundGroup.end();)
	{
		auto& Future = (*iter);
		auto state = Future.wait_for(1ms);
		if (state == future_status::ready)
		{
			Future.get();
			iter = m_vecAsyncSoundGroup.erase(iter);

			// ó������ ���ư���
			if (iter == m_vecAsyncSoundGroup.end())
			{
				iter = m_vecAsyncSoundGroup.begin();
				continue;
			}
		}
		else if (state == future_status::timeout)
		{
			iter = m_vecAsyncSoundGroup.erase(iter);
			continue;
		}

		if (++iter == m_vecAsyncSoundGroup.end())
			iter = m_vecAsyncSoundGroup.begin();
	}
	m_vecAsyncSoundGroup.clear();
}

FMOD_RESULT CSound_Manager::LoadSoundFile_Async(const string& pPath, const string& pFileName, FMOD_RESULT& hResult, FMOD_SOUND** pSound)
{
	hResult = FMOD_System_CreateSound(m_pSystem, pPath.c_str(), FMOD_DEFAULT, NULL, pSound);

	return hResult;
}

CSound_Manager* CSound_Manager::Create()
{
	CSound_Manager* pInstance = new CSound_Manager();

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CSound_Manager");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSound_Manager::Free()
{
	/*for (auto& pairSoundCtn : m_mapSound)
	{
		for (auto& pairSound : pairSoundCtn.second->Get_MapSound())
		{
			FMOD_Sound_Release(pairSound.second);
		}
		Safe_Release(pairSoundCtn.second);
	}
	m_mapSound.clear();*/

	FMOD_System_Release(m_pSystem);
	FMOD_System_Close(m_pSystem);
}
