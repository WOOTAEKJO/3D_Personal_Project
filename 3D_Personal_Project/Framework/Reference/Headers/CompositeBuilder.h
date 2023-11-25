#pragma once
#include "Engine_Defines.h"

#include "Sequence.h"
#include "Selector.h"
#include "Succeeder.h"
#include "Failer.h"
#include "Inverter.h"
#include "Repeator.h"

BEGIN(Engine)

template<class Parent>
class CDecoratorBuilder;

template<class Parent>
class ENGINE_DLL CCompositeBuilder
{
public:
	CCompositeBuilder() {}
	CCompositeBuilder(Parent* pParent, class CComposite* pNode) : m_pParent(pParent), m_pNode(pNode) 
	{
		//Safe_AddRef(m_pNode);
	}
	virtual	~CCompositeBuilder() 
	{
		//Safe_Release(m_pNode);
	}

public:
	template<class NodeType, typename... Args>
	CCompositeBuilder<Parent> Leaf(Args... args)
	{
		auto child = NodeType::Create((args)...);
		child->Set_BlackBoard(m_pNode->Get_BlackBoard());
		m_pNode->Add_Children(child);
		return *this;
	}

	template<class CompositeType, typename... Args>
	CCompositeBuilder<CCompositeBuilder<Parent>> Composite(Args... args)
	{
		auto child = CompositeType::Create((args)...);
		child->Set_BlackBoard(m_pNode->Get_BlackBoard());
		m_pNode->Add_Children(child);
		return CCompositeBuilder<CCompositeBuilder<Parent>>(this, (CompositeType*)child);
	}

	template<class DecoratorType, typename... Args>
	CDecoratorBuilder<CCompositeBuilder<Parent>> Decorator(Args... args)
	{
		auto child = DecoratorType::Create((args)...);
		child->Set_BlackBoard(m_pNode->Get_BlackBoard());
		m_pNode->Add_Children(child);
		return CDecoratorBuilder<CCompositeBuilder<Parent>>(this, (DecoratorType*)child);
	}

public:
	Parent& End() { return *m_pParent; }

private:
	Parent*				m_pParent = { nullptr };
	class CComposite*	m_pNode = { nullptr };
};

END