/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "AzCore/Debug/Trace.h"
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
                ->Field("SelectedServiceProviderName", &AIComponentBase::m_serviceProviderName)
                ->Field("SelectedModelConfigurationName", &AIComponentBase::m_modelConfigurationName);

            if (auto* editContext = serializeContext->GetEditContext())
            {
                editContext->Class<AIComponentBase>("AI Component Base", "Base component for AI")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, AZ::Edit::PropertyRefreshLevels::EntireTree)
                    ->DataElement(
                        AZ::Edit::UIHandlers::ComboBox,
                        &AIComponentBase::m_serviceProviderName,
                        "Service",
                        "Name of the service to be used for AI requests")
                    ->Attribute(AZ::Edit::Attributes::StringList, &AIComponentBase::GetServiceProviderNames)
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, &AIComponentBase::UpdateComponent)
                    ->DataElement(
                        AZ::Edit::UIHandlers::ComboBox,
                        &AIComponentBase::m_modelConfigurationName,
                        "Model",
                        "Name of the model to be used for AI prompt generation")
                    ->Attribute(AZ::Edit::Attributes::StringList, &AIComponentBase::GetModelConfigurationNames)
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, &AIComponentBase::UpdateComponent);
            }
        }
    }

    void AIComponentBase::Activate()
    {
        UpdateComponent();
    }

    void AIComponentBase::Deactivate()
    {
    }

    AZStd::vector<AZStd::string> AIComponentBase::GetServiceProviderNames() const
    {
        return GetNames(GetServiceProviders());
    }

    AZStd::vector<AZStd::string> AIComponentBase::GetModelConfigurationNames() const
    {
        return GetNames(GetModelConfigurations());
    }

    AZStd::vector<AZStd::string> AIComponentBase::GetNames(const AZStd::vector<AZ::Component*>& components) const
    {
        AZStd::vector<AZStd::string> names;
        for (auto* component : components)
        {
            names.push_back(component->GetEntity()->GetName());
        }
        return names;
    }

    AZStd::vector<AZ::Component*> AIComponentBase::GetServiceProviders() const
    {
        auto interface = GenAIFrameworkInterface::Get();
        return interface ? interface->GetServiceProviders() : AZStd::vector<AZ::Component*>();
    }
    AZStd::vector<AZ::Component*> AIComponentBase::GetModelConfigurations() const
    {
        auto interface = GenAIFrameworkInterface::Get();
        return interface ? interface->GetModelConfigurations() : AZStd::vector<AZ::Component*>();
    }

    void AIComponentBase::UpdateComponent()
    {
        auto frameworkInterface = GenAIFrameworkInterface::Get();
        auto agentOutcome = frameworkInterface->CreateModelAgent(m_serviceProviderName, m_modelConfigurationName);
        if (agentOutcome.IsSuccess())
        {
            frameworkInterface->RemoveModelAgent(m_agentId);
            m_agentId = agentOutcome.GetValue();
        }
        else
        {
            AZ_Warning("AIComponentBase", false, "Failed to create model agent: %s");
        }
    }

} // namespace GenAIFramework
