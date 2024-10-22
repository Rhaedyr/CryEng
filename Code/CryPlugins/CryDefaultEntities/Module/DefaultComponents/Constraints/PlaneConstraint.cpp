#include "StdAfx.h"
#include "PlaneConstraint.h"

namespace Cry
{
	namespace DefaultComponents
	{
		void CPlaneConstraintComponent::Register(Schematyc::CEnvRegistrationScope& componentScope)
		{
			// Functions
			{
				auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&CPlaneConstraintComponent::ConstrainToEntity, "{761D04E2-672B-4EAD-885E-B7B056001DFA}"_cry_guid, "ConstrainToEntity");
				pFunction->SetDescription("Adds a constraint, tying this component's physical entity to the specified entity");
				pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
				pFunction->BindInput(1, 'enti', "Target Entity", "Defines the entity we want to be constrained to, or the point itself if id is 0", Schematyc::ExplicitEntityId());
				pFunction->BindInput(2, 'igno', "Ignore Collisions With", "Whether or not to ignore collisions between this entity and the target", false);
				pFunction->BindInput(3, 'arot', "Allow Rotation", "Whether or not to allow rotations when the constraint is active", true);
				componentScope.Register(pFunction);
			}
			{
				auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&CPlaneConstraintComponent::ConstrainToPoint, "{2A1E5BF3-C98F-40B3-86C6-FF21CB35F4A9}"_cry_guid, "ConstrainToPoint");
				pFunction->SetDescription("Adds a constraint, tying this component's physical entity to the point");
				pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
				pFunction->BindInput(1, 'arot', "Allow Rotation", "Whether or not to allow rotations when the constraint is active", true);
				componentScope.Register(pFunction);
			}
			{
				auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&CPlaneConstraintComponent::Remove, "{F3E7141F-508F-479D-AD72-2BD838C09905}"_cry_guid, "Remove");
				pFunction->SetDescription("Removes the constraint");
				pFunction->SetFlags(Schematyc::EEnvFunctionFlags::Construction);
				componentScope.Register(pFunction);
			}
		}


		void CPlaneConstraintComponent::ReflectType(Schematyc::CTypeDesc<CPlaneConstraintComponent>& desc)
		{
			desc.SetGUID(CPlaneConstraintComponent::IID());
			desc.SetEditorCategory("Physics Constraints");
			desc.SetLabel("Plane Constraint");
			desc.SetDescription("Constrains the physical object to a plan");
			//desc.SetIcon("icons:ObjectTypes/object.ico");
			desc.SetComponentFlags({ IEntityComponent::EFlags::Transform, IEntityComponent::EFlags::Socket, IEntityComponent::EFlags::Attach });

			desc.AddMember(&CPlaneConstraintComponent::m_bActive, 'actv', "Active", "Active", "Whether or not the constraint should be added on component reset", true);
			desc.AddMember(&CPlaneConstraintComponent::m_axis, 'axis', "Axis", "Axis", "Axis around which the physical entity is constrained", Vec3(0.f, 0.f, 1.f));

			desc.AddMember(&CPlaneConstraintComponent::m_limitMin, 'lmin', "LimitMinX", "Minimum Limit X", nullptr, 0.f);
			desc.AddMember(&CPlaneConstraintComponent::m_limitMax, 'lmax', "LimitMaxX", "Maximum Limit X", nullptr, 1.f);
			desc.AddMember(&CPlaneConstraintComponent::m_limitMinY, 'lmiy', "LimitMinY", "Minimum Limit Y", nullptr, 0.f);
			desc.AddMember(&CPlaneConstraintComponent::m_limitMaxY, 'lmay', "LimitMaxY", "Minimum Limit Y", nullptr, 1.f);
	
			desc.AddMember(&CPlaneConstraintComponent::m_damping, 'damp', "Damping", "Damping", nullptr, 0.f);
		}

		CPlaneConstraintComponent::~CPlaneConstraintComponent()
		{
			Remove();
		}

		void CPlaneConstraintComponent::Initialize()
		{
			Reset();
		}

		void CPlaneConstraintComponent::Reset()
		{
			if (m_bActive)
			{
				ConstrainToPoint(true);
			}
			else
			{
				Remove();
			}
		}

		void CPlaneConstraintComponent::ProcessEvent(SEntityEvent& event)
		{
			if (event.event == ENTITY_EVENT_START_GAME)
			{
				Reset();
			}
			else if (event.event == ENTITY_EVENT_COMPONENT_PROPERTY_CHANGED)
			{
				m_pEntity->UpdateComponentEventMask(this);

				Reset();
			}
		}

		uint64 CPlaneConstraintComponent::GetEventMask() const
		{
			uint64 bitFlags = m_bActive ? BIT64(ENTITY_EVENT_START_GAME) : 0;
			bitFlags |= BIT64(ENTITY_EVENT_COMPONENT_PROPERTY_CHANGED);

			return bitFlags;
		}
	}
}