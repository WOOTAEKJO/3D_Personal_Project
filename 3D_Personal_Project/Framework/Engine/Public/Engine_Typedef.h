#ifndef Engine_Typedef_h__
#define Engine_Typedef_h__

namespace Engine
{
	typedef		bool						_bool;

	typedef		signed char					_byte;			// ���ڸ� �����ϴ� �뵵�� �ƴ� ���ڸ� �����ϴ� �뵵
	typedef		unsigned char				_ubyte;			// ��������

	typedef		char						_char;			// ���ڸ� �����ϴ� �뵵
	typedef		wchar_t						_tchar;
	
	typedef		signed short				_short;
	typedef		unsigned short				_ushort;

	typedef		signed int					_int;
	typedef		unsigned int				_uint;

	typedef		signed long					_long;
	typedef		unsigned long				_ulong;

	typedef		float						_float;
	typedef		double						_double;	

	/* ����� ������ Ÿ��. */
	typedef		XMFLOAT4					_float4;
	typedef		XMFLOAT3					_float3;
	typedef		XMFLOAT2					_float2;

	/* ����� ������ Ÿ��. */
	typedef		XMVECTOR					_vector;
	
}

#endif // Engine_Typedef_h__
