#include "Input_Device.h"


Engine::CInput_Device::CInput_Device(void)
{
    ZeroMemory(m_byKeyData, sizeof(m_byKeyData));
}

Engine::CInput_Device::~CInput_Device(void)
{
    Free();
}


HRESULT Engine::CInput_Device::Initialize(HINSTANCE hInst, HWND hWnd)
{
    // DInput �İ�ü�� �����ϴ� �Լ�
    if (FAILED(DirectInput8Create(hInst,
        DIRECTINPUT_VERSION,
        IID_IDirectInput8,
        (void**)&m_pInputSDK,
        NULL)))
        return E_FAIL;

    // Ű���� ��ü ����
    if (FAILED(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr)))
        return E_FAIL;

    // ������ Ű���� ��ü�� ���� ������ �� ��ü���� �����ϴ� �Լ�
    m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);

    // ��ġ�� ���� �������� �������ִ� �Լ�, (Ŭ���̾�Ʈ�� ���ִ� ���¿��� Ű �Է��� ������ ������ �����ϴ� �Լ�)
    m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

    // ��ġ�� ���� access ������ �޾ƿ��� �Լ�
    m_pKeyBoard->Acquire();


    // ���콺 ��ü ����
    if (FAILED(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
        return E_FAIL;

    // ������ ���콺 ��ü�� ���� ������ �� ��ü���� �����ϴ� �Լ�
    m_pMouse->SetDataFormat(&c_dfDIMouse);

    // ��ġ�� ���� �������� �������ִ� �Լ�, Ŭ���̾�Ʈ�� ���ִ� ���¿��� Ű �Է��� ������ ������ �����ϴ� �Լ�
    m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

    // ��ġ�� ���� access ������ �޾ƿ��� �Լ�
    m_pMouse->Acquire();



    return S_OK;
}

void Engine::CInput_Device::Update_InputDev(void)
{
    m_pKeyBoard->GetDeviceState(256, m_byKeyState);
    m_pMouse->GetDeviceState(sizeof(m_tMouseState), &m_tMouseState);
}

bool CInput_Device::Key_Pressing(_ubyte byKeyID)
{
    if (m_byKeyState[byKeyID] & 0x80)
        return true;

    return false;
}

bool CInput_Device::Key_Down(_ubyte byKeyID)
{
    // �������� ���� ���� ���� ���� ������ ���
    if (!m_byKeyData[byKeyID] && (m_byKeyState[byKeyID] & 0x80))
    {
        m_byKeyData[byKeyID] = !m_byKeyData[byKeyID]; //true
        return true;
    }

    return false;
}

bool CInput_Device::Key_Up(_ubyte byKeyID)
{
    // �������� ���� ���� �ְ� ���� ������ �ʾ��� ���
    if (m_byKeyData[byKeyID] && !(m_byKeyState[byKeyID] & 0x80))
    {
        m_byKeyData[byKeyID] = !m_byKeyData[byKeyID];
        return true;
    }

    return false;
}

bool CInput_Device::Mouse_Pressing(MOUSEKEYSTATE eMouse)
{
    if (m_tMouseState.rgbButtons[eMouse] & 0x80)
        return true;

    return false;
}

bool CInput_Device::Mouse_Down(MOUSEKEYSTATE eMouse)
{
    // �������� ���� ���� ���� ���� ������ ���
    if (!m_tMouseData[eMouse] && (m_tMouseState.rgbButtons[eMouse] & 0x80))
    {
        m_tMouseData[eMouse] = !m_tMouseData[eMouse]; //true
        return true;
    }
    return false;
}

bool CInput_Device::Mouse_Up(MOUSEKEYSTATE eMouse)
{
    // �������� ���� ���� �ְ� ���� ������ �ʾ��� ���
    if (m_tMouseData[eMouse] && !(m_tMouseState.rgbButtons[eMouse] & 0x80))
    {
        m_tMouseData[eMouse] = !m_tMouseData[eMouse];
        return true;
    }

    return false;
}

void CInput_Device::LateUpdate_InputDev(void)
{
    for (int i = 0; i < 256; ++i) //������ �ְ� ���� ������ ���� �� false��?
    {
        if (m_byKeyData[i] && !(m_byKeyState[i] & 0x80))
            m_byKeyData[i] = !m_byKeyData[i];

        if (!m_byKeyData[i] && (m_byKeyState[i] & 0x80))
            m_byKeyData[i] = !m_byKeyData[i];
    }

    for (int i = 0; i < DIM_END; ++i) //������ �ְ� ���� ������ ���� �� false��?
    {
        if (m_tMouseData[i] && !(m_tMouseState.rgbButtons[i] & 0x80))
            m_tMouseData[i] = !m_tMouseData[i];

        if (!m_tMouseData[i] && (m_tMouseState.rgbButtons[i] & 0x80))
            m_tMouseData[i] = !m_tMouseData[i];
    }
}


CInput_Device* CInput_Device::Create(HINSTANCE hInst, HWND hWnd)
{
    CInput_Device* pInstance = new CInput_Device();

    if (FAILED(pInstance->Initialize(hInst, hWnd)))
    {
        MSG_BOX("Failed to Created : CInput_Device");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void Engine::CInput_Device::Free(void)
{
    Safe_Release(m_pKeyBoard);
    Safe_Release(m_pMouse);
    Safe_Release(m_pInputSDK);
}
