/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "MockRequesterComponent.h"
#include <GenAIFramework/SystemRegistrationContext/SystemRegistrationContext.h>

#include <AzCore/Component/Component.h>
#include <AzCore/Outcome/Outcome.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/EditContextConstants.inl>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/std/string/string.h>

namespace GenAIMock
{
    void MockRequesterComponentConfiguration::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<MockRequesterComponentConfiguration, AZ::ComponentConfig>()->Version(0);

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<MockRequesterComponentConfiguration>("Mock Requester configuration", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "AI");
            }
        }
    }

    void MockRequesterComponent::Reflect(AZ::ReflectContext* context)
    {
        MockRequesterComponentConfiguration::Reflect(context);
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<MockRequesterComponent, AZ::Component>()->Version(0)->Field(
                "Configuration", &MockRequesterComponent::m_configuration);

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<MockRequesterComponent>("Mock Requester component", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "AI")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &MockRequesterComponent::m_configuration,
                        "Configuration",
                        "Configuration for the Mock requester")
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly);
            }
        }

        if (auto registrationContext = azrtti_cast<GenAIFramework::SystemRegistrationContext*>(context))
        {
            registrationContext->RegisterGenAIFrameworkServiceRequester<MockRequesterComponent>();
        }
    }

    MockRequesterComponent::MockRequesterComponent()
    {
        // TODO: this is the very first revision, the hardcoded data will be changed into dynamically loaded json asset
        m_testData = { { "0", "zero" }, { "1", "one" }, { "2", "two" }, { "3", "three" }, { "4", "four" } };
    }

    MockRequesterComponent::MockRequesterComponent(const MockRequesterComponentConfiguration& config)
        : MockRequesterComponent()
    {
        // TODO: moved from initializer list temporarily
        m_configuration = config;
    }

    void MockRequesterComponent::Init()
    {
    }

    void MockRequesterComponent::Activate()
    {
        GenAIFramework::AIServiceRequesterBus::Handler::BusConnect(GetEntityId());
    }

    void MockRequesterComponent::Deactivate()
    {
        GenAIFramework::AIServiceRequesterBus::Handler::BusDisconnect();
    }

    void MockRequesterComponent::SetConfiguration(const MockRequesterComponentConfiguration& config)
    {
        m_configuration = config;
    }

    const MockRequesterComponentConfiguration& MockRequesterComponent::GetConfiguration() const
    {
        return m_configuration;
    }

    void MockRequesterComponent::SendRequest(
        const AZStd::string& request, AZStd::function<void(AZ::Outcome<AZStd::string, AZStd::string>)> callback)
    {
        if (m_testData.contains(request))
        {
            AZ::Outcome<AZStd::string, AZStd::string> outcomeResult = AZ::Success(m_testData.at(request));
            callback(outcomeResult);
        }
        else
        {
            AZStd::string azStringResult("I have no answer for that.");
            AZ::Outcome<AZStd::string, AZStd::string> outcomeError = AZ::Failure(azStringResult);
            callback(outcomeError);
        }
    }
} // namespace GenAIMock
