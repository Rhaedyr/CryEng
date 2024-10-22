// Copyright 2001-2016 Crytek GmbH / Crytek Group. All rights reserved.

// -------------------------------------------------------------------------
//  File name:   SubstitutionProxy.h
//  Version:     v1.00
//  Created:     7/6/2005 by Timur.
//  Compilers:   Visual Studio.NET 2003
//  Description:
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __SubstitutionProxy_h__
#define __SubstitutionProxy_h__
#pragma once

//////////////////////////////////////////////////////////////////////////
// Description:
//    Implements base substitution proxy class for entity.
//////////////////////////////////////////////////////////////////////////
struct CEntityComponentSubstitution : IEntitySubstitutionComponent
{
	CRY_ENTITY_COMPONENT_CLASS(CEntityComponentSubstitution,IEntitySubstitutionComponent,"CEntityComponentSubstitution",0xF60DBB948860494A,0x93586DE8C953B324);

	CEntityComponentSubstitution();
	virtual ~CEntityComponentSubstitution();

public:
	//////////////////////////////////////////////////////////////////////////
	// IEntityComponent interface implementation.
	//////////////////////////////////////////////////////////////////////////
	virtual void Initialize() final {};
	virtual void ProcessEvent(SEntityEvent& event) final;
	virtual uint64 GetEventMask() const final;; // Need nothing
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// IEntityComponent interface implementation.
	//////////////////////////////////////////////////////////////////////////
	virtual EEntityProxy GetProxyType() const final                                     { return ENTITY_PROXY_SUBSTITUTION; }
	virtual void         Release() final                                          { delete this; }
	virtual void         GameSerialize(TSerialize ser) final;
	virtual bool         NeedGameSerialize() final;
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// IEntitySubstitutionComponent interface.
	//////////////////////////////////////////////////////////////////////////
	virtual void         SetSubstitute(IRenderNode* pSubstitute) final;
	virtual IRenderNode* GetSubstitute() final { return m_pSubstitute; }
	//////////////////////////////////////////////////////////////////////////

	virtual void GetMemoryUsage(ICrySizer* pSizer) const final
	{
		pSizer->AddObject(this, sizeof(*this));
	}

private:
	void Done();
private:
	IRenderNode* m_pSubstitute = nullptr;
};

#endif // __SubstitutionProxy_h__
