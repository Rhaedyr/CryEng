// Copyright 2001-2016 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include "ActionExecuteAudioTrigger.h"
#include <CryDynamicResponseSystem/IDynamicResponseSystem.h>
#include <CryDynamicResponseSystem/IDynamicResponseAction.h>
#include <CrySerialization/Decorators/ResourcesAudio.h>

static const char* ActionPlaySoundId = "ExecuteAudioTriggerAction";

DRS::IResponseActionInstanceUniquePtr CActionExecuteAudioTrigger::Execute(DRS::IResponseInstance* pResponseInstance)
{
	CryAudio::ControlId audioStartTriggerID;
	if (gEnv->pAudioSystem->GetTriggerId(m_AudioTriggerName.c_str(), audioStartTriggerID))
	{
		IEntity* pEntity = pResponseInstance->GetCurrentActor()->GetLinkedEntity();
		if (pEntity)
		{
			IEntityAudioComponent* pEntityAudioProxy = pEntity->GetOrCreateComponent<IEntityAudioComponent>();

			CryAudio::AuxObjectId auxProxyId = pResponseInstance->GetCurrentActor()->GetAuxAudioObjectID();
			if (auxProxyId == CryAudio::InvalidAuxObjectId)
				auxProxyId = CryAudio::DefaultAuxObjectId;

			if (m_bWaitToBeFinished)
			{
				DRS::IResponseActionInstanceUniquePtr pActionInstance(new CActionExecuteAudioTriggerInstance(pResponseInstance->GetCurrentActor(), audioStartTriggerID));
				CryAudio::SRequestUserData const userData(CryAudio::ERequestFlags::DoneCallbackOnExternalThread, (void* const)pActionInstance.get(), (void* const)ActionPlaySoundId, (void* const)pActionInstance.get());

				if (pEntityAudioProxy->ExecuteTrigger(audioStartTriggerID, auxProxyId, userData))
				{
					return pActionInstance;
				}
			}
			else  //Fire and forget sound, no need to listen to the 'finished'-callback
			{
				pEntityAudioProxy->ExecuteTrigger(audioStartTriggerID, auxProxyId);
			}
		}
	}
	return nullptr;
}

//--------------------------------------------------------------------------------------------------
string CActionExecuteAudioTrigger::GetVerboseInfo() const
{
	return string().Format("'%s'", m_AudioTriggerName.c_str());
}

//--------------------------------------------------------------------------------------------------
void CActionExecuteAudioTrigger::Serialize(Serialization::IArchive& ar)
{
	ar(Serialization::AudioTrigger(m_AudioTriggerName), "audioTriggerName", "^ TriggerName");
	ar(m_bWaitToBeFinished, "WaitForFinish", "^ WaitForTriggerToFinish");
}

//--------------------------------------------------------------------------------------------------
CActionExecuteAudioTriggerInstance::CActionExecuteAudioTriggerInstance(DRS::IResponseActor* pActor, CryAudio::ControlId audioStartTriggerID)
	: m_pActor(pActor)
	, m_audioStartTriggerID(audioStartTriggerID)
{
	gEnv->pAudioSystem->AddRequestListener(&CActionExecuteAudioTriggerInstance::OnAudioTriggerFinished, this, CryAudio::ESystemEvents::TriggerExecuted | CryAudio::ESystemEvents::TriggerFinished);
}

//--------------------------------------------------------------------------------------------------
CActionExecuteAudioTriggerInstance::~CActionExecuteAudioTriggerInstance()
{
	gEnv->pAudioSystem->RemoveRequestListener(&CActionExecuteAudioTriggerInstance::OnAudioTriggerFinished, this);
}

//--------------------------------------------------------------------------------------------------
DRS::IResponseActionInstance::eCurrentState CActionExecuteAudioTriggerInstance::Update()
{
	if (m_pActor)
	{
		return DRS::IResponseActionInstance::CS_RUNNING;
	}
	else
	{
		return DRS::IResponseActionInstance::CS_FINISHED;
	}
}

//--------------------------------------------------------------------------------------------------
void CActionExecuteAudioTriggerInstance::OnAudioTriggerFinished(const CryAudio::SRequestInfo* const pInfo)
{
	if (pInfo->pUserData == ActionPlaySoundId)
	{
		if (pInfo->requestResult == CryAudio::ERequestResult::Failure &&
		    (pInfo->systemEvent == CryAudio::ESystemEvents::TriggerExecuted ||
		     pInfo->systemEvent == CryAudio::ESystemEvents::TriggerFinished))
		{
			CRY_ASSERT(pInfo->pUserDataOwner != nullptr);
			CActionExecuteAudioTriggerInstance* pEndedInstance = reinterpret_cast<CActionExecuteAudioTriggerInstance*>(pInfo->pUserDataOwner);
			pEndedInstance->SetFinished();
		}
	}
}

//--------------------------------------------------------------------------------------------------
void CActionExecuteAudioTriggerInstance::Cancel()
{
	if (m_pActor)
	{
		if (IEntity* pEntity = m_pActor->GetLinkedEntity())
		{
			if (IEntityAudioComponent* pEntityAudioProxy = pEntity->GetComponent<IEntityAudioComponent>())  //we refetch the audio component here, because during entity-destruction it is not guaranteed that the DRS component is deleted first.
			{
				pEntityAudioProxy->StopTrigger(m_audioStartTriggerID);
				return;
			}
		}
	}
	m_pActor = nullptr;  //we failed to stop the trigger, therefore we hard-cancel this action instance
}

//--------------------------------------------------------------------------------------------------
void CActionExecuteAudioTriggerInstance::SetFinished()
{
	m_pActor = nullptr;
}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

DRS::IResponseActionInstanceUniquePtr CActionSetAudioSwitch::Execute(DRS::IResponseInstance* pResponseInstance)
{
	CryAudio::ControlId switchID;
	if (gEnv->pAudioSystem->GetSwitchId(m_switchName.c_str(), switchID))
	{
		CryAudio::SwitchStateId switchStateID;
		if (gEnv->pAudioSystem->GetSwitchStateId(switchID, m_stateName.c_str(), switchStateID))
		{
			IEntity* pEntity = pResponseInstance->GetCurrentActor()->GetLinkedEntity();
			if (pEntity)
			{
				IEntityAudioComponent* pEntityAudioProxy = pEntity->GetOrCreateComponent<IEntityAudioComponent>();
				pEntityAudioProxy->SetSwitchState(switchID, switchStateID, pResponseInstance->GetCurrentActor()->GetAuxAudioObjectID());
			}
		}
	}
	return nullptr;
}

//--------------------------------------------------------------------------------------------------
string CActionSetAudioSwitch::GetVerboseInfo() const
{
	return string().Format(" '%s' to state '%s'", m_switchName.c_str(), m_stateName.c_str());
}

//--------------------------------------------------------------------------------------------------
void CActionSetAudioSwitch::Serialize(Serialization::IArchive& ar)
{
	ar(Serialization::AudioSwitch(m_switchName), "switch", "^ Switch");
	ar(Serialization::AudioSwitchState(m_stateName), "switchState", "^ SwitchState");
}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

DRS::IResponseActionInstanceUniquePtr CActionSetAudioParameter::Execute(DRS::IResponseInstance* pResponseInstance)
{
	CryAudio::ControlId parameterId;
	if (gEnv->pAudioSystem->GetParameterId(m_audioParameter.c_str(), parameterId))
	{
		IEntity* pEntity = pResponseInstance->GetCurrentActor()->GetLinkedEntity();
		if (pEntity)
		{
			IEntityAudioComponent* pEntityAudioProxy = pEntity->GetOrCreateComponent<IEntityAudioComponent>();
			pEntityAudioProxy->SetParameter(parameterId, m_valueToSet, pResponseInstance->GetCurrentActor()->GetAuxAudioObjectID());
		}
	}
	return nullptr;
}

//--------------------------------------------------------------------------------------------------
string CActionSetAudioParameter::GetVerboseInfo() const
{
	return string().Format(" '%s' to value '%f'", m_audioParameter.c_str(), m_valueToSet);
}

//--------------------------------------------------------------------------------------------------
void CActionSetAudioParameter::Serialize(Serialization::IArchive& ar)
{
	ar(Serialization::AudioRTPC(m_audioParameter), "audioParameter", "^ AudioParameter");
	ar(m_valueToSet, "value", "^ Value");
}
