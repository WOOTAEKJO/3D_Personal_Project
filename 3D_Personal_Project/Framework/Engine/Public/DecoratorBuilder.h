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
class CCompositeBuilder;

template<class Parent>
class ENGINE_DLL CDecoratorBuilder
{
public:
	CDecoratorBuilder() {}
	CDecoratorBuilder(Parent* pParent, class CDecorator* pNode):m_pParent(pParent), m_pNode(pNode)
	{
		//Safe_AddRef(m_pNode);
	}
	virtual	~CDecoratorBuilder() 
	{
		//Safe_Release(m_pNode);
	}

public:
	template<class NodeType, typename... Args>
	CDecoratorBuilder<Parent> Leaf(Args... args)
	{
		auto child = NodeType::Create((args)...);
		child->Set_BlackBoard(m_pNode->Get_BlackBoard());
		m_pNode->Add_Child(child);
		return *this;
	}

	template<class CompositeType, typename... Args>
	CCompositeBuilder<CDecoratorBuilder<Parent>> Composite(Args... args)
	{
		auto child = CompositeType::Create((args)...);
		child->Set_BlackBoard(m_pNode->Get_BlackBoard());
		m_pNode->Add_Child(child);
		return CCompositeBuilder<CDecoratorBuilder<Parent>>(this, (CompositeType*)child);
	}

	template<class DecoratorType, typename... Args>
	CDecoratorBuilder<CDecoratorBuilder<Parent>> Decorator(Args... args)
	{
		auto child = DecoratorType::Create((args)...);
		child->Set_BlackBoard(m_pNode->Get_BlackBoard());
		m_pNode->Add_Child(child);
		return CDecoratorBuilder<CDecoratorBuilder<Parent>>(this,(DecoratorType*)child);
	}

public:
	Parent& End() { return *m_pParent; }

private:
	Parent*				m_pParent = { nullptr };
	class CDecorator*	m_pNode = { nullptr };
};

END