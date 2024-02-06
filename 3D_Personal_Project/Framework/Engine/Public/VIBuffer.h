#pragma once
#include "Component.h"
#include "MeshData.h"


BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract : public CComponent
{
public:
	enum MODE {MODE_DYNAMIC, MODE_STATIC,MODE_END};

protected:
	CVIBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer(const CVIBuffer& rhs);
	virtual	~CVIBuffer() = default;

public:
	virtual	HRESULT	Initialize_ProtoType() override;	// 원형 초기화
	virtual	HRESULT	Initialize(void* pArg) override;	// 사본 초기화
	HRESULT	Render();									// context로 그리기 작업
public:
	virtual HRESULT	Bind_Buffer();									// context로 바인드 작업
	HRESULT	Create_Buffer(_Inout_ ID3D11Buffer** pBuffer);	// device로 버퍼 생성 작업

public:
	virtual HRESULT	Set_Buffer(const _char* strPath);
	virtual HRESULT	Init_Buffer(CMeshData::MESHDATADESC tMeshData);

public:
	ID3D11Buffer* Get_VertexBuffer() { return m_pVB; }
	ID3D11Buffer* Get_IndexBuffer() { return m_pIB; }
	_uint		Get_VertexStride() { return m_iVertexStride; }
	DXGI_FORMAT	Get_IndexFormat() { return m_eIndexForMat; }
	D3D_PRIMITIVE_TOPOLOGY	Get_Topology() { return m_eTopology; }
	_uint		Get_IndexNum() { return m_iIndexNum; }

protected:
	ID3D11Buffer*	m_pVB = { nullptr };	// 버텍스 버퍼를 저장하는 변수
	ID3D11Buffer*	m_pIB = { nullptr };	// 인덱스 버퍼를 저장하는 변수
protected:
	D3D11_BUFFER_DESC		m_Buffer_Desc;		// 버퍼의 옵션을 설정하는 구조체
	D3D11_SUBRESOURCE_DATA	m_SubResource_Data;	// 버퍼의 초기 값을 설정하는 구조체
protected:
	_uint					m_iVertexNum = { 0 };			// 정점 갯수
	_uint					m_iVertexStride = { 0 };		// 정점 하나의 크기
	_uint					m_iVertexBuffersNum = { 0 };	// 버텍스 버퍼들의 갯수

	_uint					m_iPrimitiveNum = { 0 };		// 삼각형 갯수
	_uint					m_iIndexNum = { 0 };			// 인덱스 갯수
	_uint					m_iIndexStride = { 0 };			// 인덱스 하나의 크기
	DXGI_FORMAT				m_eIndexForMat = { };			// 인덱스 포맷 설정, 인덱스 하나의 크기
	D3D_PRIMITIVE_TOPOLOGY	m_eTopology = { };				// 정점을 이용해서 어떻게 그릴지 설정하는 구조체

protected:
	CMeshData*				m_pMeshData = { nullptr };

public:
	virtual	CComponent* Clone(void* pArg) = 0;
	virtual	void	Free() override;
};

END

/*
	- 형태를 그려내기 위한 클래스들의 부모격 클래스
		- 형태는 사각형, 모델, 지형 등등
	- component를 상속 받기 때문에 프로토 타입 패턴을 사용
	- 그래픽 장치 변수들을 이용해서 버퍼를 만들고, 바인드해서 그린다.
		- 버퍼를 만드는 변수는 device
		- 바인드해서 그리는 변수는 context
	- 버퍼를 만드는데 있어 필요한 구조체들과 변수들을 선언한다.
		- 필요한 구조체
			- 버퍼의 옵션을 설정하는 구조체
			- 버퍼를 초기화 할 때 사용하는 구조체
				- 정점 버퍼는 중간에 값을 바꾸지 못하기 때문에 버퍼를 만들 때 초기 값을 같이 설정해준다.
		- 필요한 변수들
			- 버퍼의 갯수, 버퍼 하나의 크기
			- 인덱스의 갯수, 인덱스 하나의 크기
	- 클라이언트에서 알아야 하기 때문에 ENGINE_DLL
*/