/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <AzCore/Component/Entity.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/EditContextConstants.inl>
#include <AzCore/Serialization/SerializeContext.h>
#include <GenAIFramework/AIComponentBase/AIComponentBase.h>
#include <GenAIFramework/GenAIFrameworkBus.h>

namespace GenAIFramework
{
    void AIComponentBase::Reflect(AZ::ReflectContext* context)
    {
        if (auto* serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<AIComponentBase, AZ::Component>()
                ->Version(0)
                ->Field("SelectedServiceRequestorName", &AIComponentBase::m_selectedServiceRequestorName)
                ->Field("SelectedModelConfigurationName", &AIComponentBase::m_selectedModelConfigurationName)
                ->Field("SelectedServiceRequestorId", &AIComponentBase::m_selectedServiceRequestorId)
                ->Field("SelectedModelConfigurationId", &AIComponentBase::m_selectedModelConfigurationId)
                ->Field("UseNonSystemComponents", &AIComponentBase::m_useNonSystemComponents);

            if (auto* editContext = serializeContext->GetEditContext())
            {
                editContext->Class<AIComponentBase>("AI Component Base", "Base component for AI")
                    ->DataElement(
                        AZ::Edit::UIHandlers::CheckBox,
                        &AIComponentBase::m_useNonSystemComponents,
                        "Use Non-System Components",
                        "Manually define the IDs for the requestor and generator components.")
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, AZ::Edit::PropertyRefreshLevels::EntireTree)
                    ->DataElement(
                        AZ::Edit::UIHandlers::ComboBox,
                        &AIComponentBase::m_selectedServiceRequestorName,
                        "Requestor",
                        "Id of requester to be used for AI requests")
                    ->Attribute(AZ::Edit::Attributes::StringList, &AIComponentBase::GetServiceRequestorsNames)
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, &AIComponentBase::UpdateNamedServiceRequestorId)
                    ->Attribute(AZ::Edit::Attributes::Visibility, &AIComponentBase::UseSystemComponents)
                    ->DataElement(
                        AZ::Edit::UIHandlers::ComboBox,
                        &AIComponentBase::m_selectedModelConfigurationName,
                        "Generator",
                        "Id of generator to be used for AI prompt generation")
                    ->Attribute(AZ::Edit::Attributes::StringList, &AIComponentBase::GetModelConfigurationsNames)
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, &AIComponentBase::UpdateNamedModelConfigurationId)
                    ->Attribute(AZ::Edit::Attributes::Visibility, &AIComponentBase::UseSystemComponents)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &AIComponentBase::m_selectedServiceRequestorId,
                        "Selected Requestor Id",
                        "Id of selected requester")
                    ->Attribute(AZ::Edit::Attributes::Visibility, &AIComponentBase::m_useNonSystemComponents)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &AIComponentBase::m_selectedModelConfigurationId,
                        "Selected Generator Id",
                        "Id of selected generator")
                    ->Attribute(AZ::Edit::Attributes::Visibility, &AIComponentBase::m_useNonSystemComponents);
            }
        }
    }

    void AIComponentBase::Activate()
    {
    }

    void AIComponentBase::Deactivate()
    {
    }

    AZStd::vector<AZStd::string> AIComponentBase::GetServiceRequestorsNames() const
    {
        return GetNames(GetServiceRequestors());
    }

    AZStd::vector<AZStd::string> AIComponentBase::GetModelConfigurationsNames() const
    {
        return GetNames(GetModelConfigurations());
    }

    AZStd::vector<AZStd::string> AIComponentBase::GetNames(AZStd::vector<AZ::Component*> components) const
    {
        AZStd::vector<AZStd::string> names;
        for (auto* component : components)
        {
            names.push_back(component->GetEntity()->GetName());
        }
        return names;
    }

    AZStd::vector<AZ::Component*> AIComponentBase::GetServiceRequestors() const
    {
        auto interface = GenAIFrameworkInterface::Get();
        return interface ? interface->GetActiveServiceRequesters() : AZStd::vector<AZ::Component*>();
    }
    AZStd::vector<AZ::Component*> AIComponentBase::GetModelConfigurations() const
    {
        auto interface = GenAIFrameworkInterface::Get();
        return interface ? interface->GetActiveModelConfigurations() : AZStd::vector<AZ::Component*>();
    }

    void AIComponentBase::GetNamedId(const AZStd::string& name, AZStd::vector<AZ::Component*> components, AZ::EntityId& id)
    {
        for (auto* component : components)
        {
            if (component->GetEntity()->GetName() == name)
            {
                id = component->GetEntityId();
                return;
            }
        }
    }
    void AIComponentBase::UpdateNamedServiceRequestorId()
    {
        AZ_Warning(
            "PromptComponent",
            false,
            "Before - Requestor: %s, Requestor Id: %s, Is valid? %s",
            m_selectedServiceRequestorName.c_str(),
            m_selectedServiceRequestorId.ToString().c_str(),
            (m_selectedServiceRequestorId.IsValid() ? "true" : "false"));
        GetNamedId(m_selectedServiceRequestorName, GetServiceRequestors(), m_selectedServiceRequestorId);
        AZ_Warning(
            "PromptComponent",
            false,
            "After - Requestor: %s, Requestor Id: %s, Is valid? %s",
            m_selectedServiceRequestorName.c_str(),
            m_selectedServiceRequestorId.ToString().c_str(),
            (m_selectedServiceRequestorId.IsValid() ? "true" : "false"));
    }
    void AIComponentBase::UpdateNamedModelConfigurationId()
    {
        GetNamedId(m_selectedModelConfigurationName, GetModelConfigurations(), m_selectedModelConfigurationId);
    }

    bool AIComponentBase::UseSystemComponents()
    {
        return !m_useNonSystemComponents;
    }

} // namespace GenAIFramework