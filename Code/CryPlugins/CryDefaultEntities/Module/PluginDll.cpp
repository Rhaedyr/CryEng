// Copyright 2001-2016 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include "PluginDll.h"

#include "DefaultComponents/AI/PathfindingComponent.h"
#include "DefaultComponents/Audio/AudioSpotComponent.h"
#include "DefaultComponents/Audio/AudioAreaComponent.h"
#include "DefaultComponents/Cameras/CameraComponent.h"
#include "DefaultComponents/Constraints/LineConstraint.h"
#include "DefaultComponents/Constraints/PlaneConstraint.h"
#include "DefaultComponents/Constraints/PointConstraint.h"
#include "DefaultComponents/Debug/DebugDrawComponent.h"
#include "DefaultComponents/Effects/DecalComponent.h"
#include "DefaultComponents/Effects/FogComponent.h"
#include "DefaultComponents/Effects/ParticleComponent.h"
#include "DefaultComponents/Geometry/AdvancedAnimationComponent.h"
#include "DefaultComponents/Geometry/AlembicComponent.h"
#include "DefaultComponents/Geometry/AnimatedMeshComponent.h"
#include "DefaultComponents/Geometry/StaticMeshComponent.h"
#include "DefaultComponents/Input/InputComponent.h"
#include "DefaultComponents/Lights/EnvironmentProbeComponent.h"
#include "DefaultComponents/Lights/PointLightComponent.h"
#include "DefaultComponents/Lights/ProjectorLightComponent.h"
#include "DefaultComponents/Physics/BoxPrimitiveComponent.h"
#include "DefaultComponents/Physics/CapsulePrimitiveComponent.h"
#include "DefaultComponents/Physics/CharacterControllerComponent.h"
#include "DefaultComponents/Physics/CylinderPrimitiveComponent.h"
#include "DefaultComponents/Physics/PhysicsPrimitiveComponent.h"
#include "DefaultComponents/Physics/RigidBodyComponent.h"
#include "DefaultComponents/Physics/SpherePrimitiveComponent.h"
#include "DefaultComponents/Physics/AreaComponent.h"
#include "DefaultComponents/Physics/ThrusterComponent.h"
#include "DefaultComponents/Physics/Vehicles/VehicleComponent.h"
#include "DefaultComponents/Physics/Vehicles/WheelComponent.h"
#include "DefaultComponents/Utilities/ChildEntityComponent.h"

#include <CryEntitySystem/IEntityClass.h>

// Included only once per DLL module.
#include <CryCore/Platform/platform_impl.inl>

#include <CryCore/StaticInstanceList.h>

IEntityRegistrator* IEntityRegistrator::g_pFirst = nullptr;
IEntityRegistrator* IEntityRegistrator::g_pLast = nullptr;



CPlugin_CryDefaultEntities::~CPlugin_CryDefaultEntities()
{
	if (gEnv->pSchematyc != nullptr)
	{
		gEnv->pSchematyc->GetEnvRegistry().DeregisterPackage(GetSchematycPackageGUID());
	}

	gEnv->pSystem->GetISystemEventDispatcher()->RemoveListener(this);
}

bool CPlugin_CryDefaultEntities::Initialize(SSystemGlobalEnvironment& env, const SSystemInitParams& initParams)
{
	env.pSystem->GetISystemEventDispatcher()->RegisterListener(this, "CCryPluginManager");

	return true;
}

void CPlugin_CryDefaultEntities::RegisterComponents(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(Cry::DefaultComponents::CPathfindingComponent));
			Cry::DefaultComponents::CPathfindingComponent::Register(componentScope);
		}
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(Cry::DefaultComponents::CEntityAudioSpotComponent));
			Cry::DefaultComponents::CEntityAudioSpotComponent::Register(componentScope);
		}
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(Cry::DefaultComponents::CEntityAudioAreaComponent));
			Cry::DefaultComponents::CEntityAudioAreaComponent::Register(componentScope);
		}
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(Cry::DefaultComponents::CCameraComponent));
			Cry::DefaultComponents::CCameraComponent::Register(componentScope);
		}
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(Cry::DefaultComponents::CLineConstraintComponent));
			Cry::DefaultComponents::CLineConstraintComponent::Register(componentScope);
		}
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(Cry::DefaultComponents::CPlaneConstraintComponent));
			Cry::DefaultComponents::CPlaneConstraintComponent::Register(componentScope);
		}
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(Cry::DefaultComponents::CPointConstraintComponent));
			Cry::DefaultComponents::CPointConstraintComponent::Register(componentScope);
		}
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(Cry::DefaultComponents::CDebugDrawComponent));
			Cry::DefaultComponents::CDebugDrawComponent::Register(componentScope);
		}
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(Cry::DefaultComponents::CDecalComponent));
			Cry::DefaultComponents::CDecalComponent::Register(componentScope);
		}
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(Cry::DefaultComponents::CFogComponent));
			Cry::DefaultComponents::CFogComponent::Register(componentScope);
		}
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(Cry::DefaultComponents::CParticleComponent));
			Cry::DefaultComponents::CParticleComponent::Register(componentScope);
		}
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(Cry::DefaultComponents::CAdvancedAnimationComponent));
			Cry::DefaultComponents::CAdvancedAnimationComponent::Register(componentScope);
		}
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(Cry::DefaultComponents::CAlembicComponent));
			Cry::DefaultComponents::CAlembicComponent::Register(componentScope);
		}
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(Cry::DefaultComponents::CAnimatedMeshComponent));
			Cry::DefaultComponents::CAnimatedMeshComponent::Register(componentScope);
		}
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(Cry::DefaultComponents::CStaticMeshComponent));
			Cry::DefaultComponents::CStaticMeshComponent::Register(componentScope);
		}
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(Cry::DefaultComponents::CInputComponent));
			Cry::DefaultComponents::CInputComponent::Register(componentScope);
		}
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(Cry::DefaultComponents::CEnvironmentProbeComponent));
			Cry::DefaultComponents::CEnvironmentProbeComponent::Register(componentScope);
		}
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(Cry::DefaultComponents::CPointLightComponent));
			Cry::DefaultComponents::CPointLightComponent::Register(componentScope);
		}
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(Cry::DefaultComponents::CProjectorLightComponent));
			Cry::DefaultComponents::CProjectorLightComponent::Register(componentScope);
		}
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(Cry::DefaultComponents::CBoxPrimitiveComponent));
			Cry::DefaultComponents::CBoxPrimitiveComponent::Register(componentScope);
		}
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(Cry::DefaultComponents::CCapsulePrimitiveComponent));
			Cry::DefaultComponents::CCapsulePrimitiveComponent::Register(componentScope);
		}
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(Cry::DefaultComponents::CCharacterControllerComponent));
			Cry::DefaultComponents::CCharacterControllerComponent::Register(componentScope);
		}
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(Cry::DefaultComponents::CCylinderPrimitiveComponent));
			Cry::DefaultComponents::CCylinderPrimitiveComponent::Register(componentScope);
		}
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(Cry::DefaultComponents::CRigidBodyComponent));
			Cry::DefaultComponents::CRigidBodyComponent::Register(componentScope);
		}
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(Cry::DefaultComponents::CSpherePrimitiveComponent));
			Cry::DefaultComponents::CSpherePrimitiveComponent::Register(componentScope);
		}
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(Cry::DefaultComponents::CAreaComponent));
			Cry::DefaultComponents::CAreaComponent::Register(componentScope);
		}
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(Cry::DefaultComponents::CThrusterComponent));
			Cry::DefaultComponents::CThrusterComponent::Register(componentScope);
		}
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(Cry::DefaultComponents::CVehiclePhysicsComponent));
			Cry::DefaultComponents::CVehiclePhysicsComponent::Register(componentScope);
		}
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(Cry::DefaultComponents::CWheelComponent));
			Cry::DefaultComponents::CWheelComponent::Register(componentScope);
		}
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(Cry::DefaultComponents::CChildEntityComponent));
			Cry::DefaultComponents::CChildEntityComponent::Register(componentScope);
		}
	}
}

void CPlugin_CryDefaultEntities::OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam)
{
	switch (event)
	{
	case ESYSTEM_EVENT_REGISTER_SCHEMATYC_ENV:
	{
		// Register legacy components introduced with 5.3 and below
		if (IEntityRegistrator::g_pFirst != nullptr)
		{
			do
			{
				IEntityRegistrator::g_pFirst->Register();

				IEntityRegistrator::g_pFirst = IEntityRegistrator::g_pFirst->m_pNext;
			} while (IEntityRegistrator::g_pFirst != nullptr);
		}

		// Register dummy entities
		IEntityClassRegistry::SEntityClassDesc stdClass;
		stdClass.flags |= ECLF_INVISIBLE;

		stdClass.sName = "AreaBox";
		gEnv->pEntitySystem->GetClassRegistry()->RegisterStdClass(stdClass);

		stdClass.sName = "AreaSphere";
		gEnv->pEntitySystem->GetClassRegistry()->RegisterStdClass(stdClass);

		stdClass.sName = "AreaSolid";
		gEnv->pEntitySystem->GetClassRegistry()->RegisterStdClass(stdClass);

		stdClass.sName = "ClipVolume";
		gEnv->pEntitySystem->GetClassRegistry()->RegisterStdClass(stdClass);

		stdClass.sName = "AreaShape";
		gEnv->pEntitySystem->GetClassRegistry()->RegisterStdClass(stdClass);

		gEnv->pSchematyc->GetEnvRegistry().RegisterPackage(
			stl::make_unique<Schematyc::CEnvPackage>(
				GetSchematycPackageGUID(),
				"EntityComponents",
				"Crytek GmbH",
				"CRYENGINE Default Entity Components",
				[this](Schematyc::IEnvRegistrar& registrar) { RegisterComponents(registrar); }
				)
		);
	}
	break;
	}
}

CRYREGISTER_SINGLETON_CLASS(CPlugin_CryDefaultEntities)

#include <CryCore/CrtDebugStats.h>
