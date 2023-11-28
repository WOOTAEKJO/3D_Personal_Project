#ifndef InputDev_h__
#define InputDev_h__

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CInput_Device : public CBase
{
private:
    explicit CInput_Device(void);
    virtual ~CInput_Device(void);

public:
    _byte   Get_DIKeyState(_ubyte byKeyID)
    {
        return m_byKeyState[byKeyID];
    }

    _byte   Get_DIMouseState(MOUSEKEYSTATE eMouse)
    {
        return m_tMouseState.rgbButtons[eMouse];
    }

    _long   Get_DIMouseMove(MOUSEMOVESTATE eMouseState)
    {
        return *(((_long*)&m_tMouseState) + eMouseState);
    }

public:
    bool   Key_Pressing(_ubyte byKeyID);
    bool   Key_Down(_ubyte byKeyID);
    bool   Key_Up(_ubyte byKeyID);

    bool   Mouse_Pressing(MOUSEKEYSTATE eMouse);
    bool   Mouse_Down(MOUSEKEYSTATE eMouse);
    bool   Mouse_Up(MOUSEKEYSTATE eMouse);

public:
    HRESULT Initialize(HINSTANCE hInst, HWND hWnd);
    void   Update_InputDev(void);

    void   LateUpdate_InputDev(void);

private:
    LPDIRECTINPUT8         m_pInputSDK = nullptr;

private:
    LPDIRECTINPUTDEVICE8   m_pKeyBoard = nullptr;
    LPDIRECTINPUTDEVICE8   m_pMouse = nullptr;

private:
    _byte               m_byKeyState[256];      // 키보드에 있는 모든 키값을 저장하기 위한 변수
    DIMOUSESTATE         m_tMouseState;

    bool               m_byKeyData[256];
    bool               m_tMouseData[DIM_END];

public:
    static CInput_Device* Create(HINSTANCE hInst, HWND hWnd);
    virtual void   Free(void);

};
END
#endif // InputDev_h__