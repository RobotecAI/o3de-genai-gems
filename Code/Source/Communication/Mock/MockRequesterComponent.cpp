/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "MockRequesterComponent.h"
#include <GenAIFramework/SystemRegistrationContext/GenAIFrameworkSystemRegistrationContext.h>

#include <AzCore/Component/Component.h>
#include <AzCore/Outcome/Outcome.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/EditContextConstants.inl>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/std/string/string.h>

namespace GenAIFramework
{
    void MockRequesterComponentConfiguration::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<MockRequesterComponentConfiguration, AZ::ComponentConfig>()->Version(0)->Field(
                "ModelId", &MockRequesterComponentConfiguration::m_modelId);

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<MockRequesterComponentConfiguration>("Mock Requester configuration", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "AI")
                    ->DataElement(AZ::Edit::UIHandlers::Default, &MockRequesterComponentConfiguration::m_modelId, "Model Id", "");
            }
        }
    }

    void MockRequesterComponentComponent::Reflect(AZ::ReflectContext* context)
    {
        MockRequesterComponentConfiguration::Reflect(context);
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<MockRequesterComponentComponent, AZ::Component>()->Version(0)->Field(
                "Configuration", &MockRequesterComponentComponent::m_configuration);

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<MockRequesterComponentComponent>("Mock Requester controller", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "AI")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &MockRequesterComponentComponent::m_configuration,
                        "Configuration",
                        "Configuration for the Mock requester")
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly);
            }
        }

        if (auto registrationContext = azrtti_cast<GenAIFrameworkSystemRegistrationContext*>(context))
        {
            registrationContext->RegisterGenAIFrameworkServiceRequester<MockRequesterComponentComponent>();
        }
    }

    MockRequesterComponentComponent::MockRequesterComponentComponent(const MockRequesterComponentConfiguration& config)
        : m_configuration(config)
    {
    }

    void MockRequesterComponentComponent::Init()
    {
    }

    void MockRequesterComponentComponent::Activate()
    {
        AIServiceRequesterBus::Handler::BusConnect(GetEntityId());
    }

    void MockRequesterComponentComponent::Deactivate()
    {
        AIServiceRequesterBus::Handler::BusDisconnect();
    }

    void MockRequesterComponentComponent::SetConfiguration(const MockRequesterComponentConfiguration& config)
    {
        m_configuration = config;
    }

    const MockRequesterComponentConfiguration& MockRequesterComponentComponent::GetConfiguration() const
    {
        return m_configuration;
    }

    void MockRequesterComponentComponent::SendRequest(
        const AZStd::string& request, AZStd::function<void(AZ::Outcome<AZStd::string, AZStd::string>)> callback)
    {
        if (request == "test")
        {
            AZ_Warning("JHDEBUG", false, "request == test");
            AZStd::string azStringResult("tested test");
            AZ::Outcome<AZStd::string, AZStd::string> outcomeResult = AZ::Success(azStringResult);
            callback(outcomeResult);
        }
        else
        {
            AZ_Warning("JHDEBUG", false, "request != test");
            AZStd::string azStringResult("not tested test");
            AZ::Outcome<AZStd::string, AZStd::string> outcomeError = AZ::Failure(azStringResult);
            callback(outcomeError);
        }
    }
} // namespace GenAIFramework