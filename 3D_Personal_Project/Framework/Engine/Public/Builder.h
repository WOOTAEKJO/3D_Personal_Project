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
class CDecoratorBuilder;

class ENGINE_DLL CBuilder
{
public:
	CBuilder();
	virtual ~CBuilder();

public:
	template<class NodeType, typename... Args>
	CBuilder Leaf(Args... args)
	{
		m_pRoot = NodeType::Create((args)...);
		m_pRoot->Set_BlackBoard(m_pTree->Get_BlackBoard());
		return *this;
	}

	template<class CompositeType, typename... Args>
	CCompositeBuilder<CBuilder> Composite(Args... args)
	{
		m_pRoot = CompositeType::Create((args)...);
		m_pRoot->Set_BlackBoard(m_pTree->Get_BlackBoard());
		return CCompositeBuilder<CBuilder>(this, (CompositeType*)m_pRoot);
	}

	template<class DecoratorType, typename... Args>
	CDecoratorBuilder<CBuilder> Decorator(Args... args)
	{
		m_pRoot = DecoratorType::Create((args)...);
		m_pRoot->Set_BlackBoard(m_pTree->Get_BlackBoard());
		return CDecoratorBuilder<CBuilder>(this, (DecoratorType*)m_pRoot);
	}
	
public:
	class CNode* Build();

private:
	class CNode*			m_pRoot = { nullptr };
	class CBehaviorTree*	m_pTree = { nullptr };
};

END