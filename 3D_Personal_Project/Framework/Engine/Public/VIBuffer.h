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
	virtual	HRESULT	Initialize_ProtoType() override;	// ���� �ʱ�ȭ
	virtual	HRESULT	Initialize(void* pArg) override;	// �纻 �ʱ�ȭ
	HRESULT	Render();									// context�� �׸��� �۾�
public:
	virtual HRESULT	Bind_Buffer();									// context�� ���ε� �۾�
	HRESULT	Create_Buffer(_Inout_ ID3D11Buffer** pBuffer);	// device�� ���� ���� �۾�

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
	ID3D11Buffer*	m_pVB = { nullptr };	// ���ؽ� ���۸� �����ϴ� ����
	ID3D11Buffer*	m_pIB = { nullptr };	// �ε��� ���۸� �����ϴ� ����
protected:
	D3D11_BUFFER_DESC		m_Buffer_Desc;		// ������ �ɼ��� �����ϴ� ����ü
	D3D11_SUBRESOURCE_DATA	m_SubResource_Data;	// ������ �ʱ� ���� �����ϴ� ����ü
protected:
	_uint					m_iVertexNum = { 0 };			// ���� ����
	_uint					m_iVertexStride = { 0 };		// ���� �ϳ��� ũ��
	_uint					m_iVertexBuffersNum = { 0 };	// ���ؽ� ���۵��� ����

	_uint					m_iPrimitiveNum = { 0 };		// �ﰢ�� ����
	_uint					m_iIndexNum = { 0 };			// �ε��� ����
	_uint					m_iIndexStride = { 0 };			// �ε��� �ϳ��� ũ��
	DXGI_FORMAT				m_eIndexForMat = { };			// �ε��� ���� ����, �ε��� �ϳ��� ũ��
	D3D_PRIMITIVE_TOPOLOGY	m_eTopology = { };				// ������ �̿��ؼ� ��� �׸��� �����ϴ� ����ü

protected:
	CMeshData*				m_pMeshData = { nullptr };

public:
	virtual	CComponent* Clone(void* pArg) = 0;
	virtual	void	Free() override;
};

END

/*
	- ���¸� �׷����� ���� Ŭ�������� �θ�� Ŭ����
		- ���´� �簢��, ��, ���� ���
	- component�� ��� �ޱ� ������ ������ Ÿ�� ������ ���
	- �׷��� ��ġ �������� �̿��ؼ� ���۸� �����, ���ε��ؼ� �׸���.
		- ���۸� ����� ������ device
		- ���ε��ؼ� �׸��� ������ context
	- ���۸� ����µ� �־� �ʿ��� ����ü��� �������� �����Ѵ�.
		- �ʿ��� ����ü
			- ������ �ɼ��� �����ϴ� ����ü
			- ���۸� �ʱ�ȭ �� �� ����ϴ� ����ü
				- ���� ���۴� �߰��� ���� �ٲ��� ���ϱ� ������ ���۸� ���� �� �ʱ� ���� ���� �������ش�.
		- �ʿ��� ������
			- ������ ����, ���� �ϳ��� ũ��
			- �ε����� ����, �ε��� �ϳ��� ũ��
	- Ŭ���̾�Ʈ���� �˾ƾ� �ϱ� ������ ENGINE_DLL
*/