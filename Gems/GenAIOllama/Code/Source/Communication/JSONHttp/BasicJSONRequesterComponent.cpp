/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "BasicJSONRequesterComponent.h"

#include <AzCore/Component/Component.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/std/string/string.h>
#include <GenAIFramework/SystemRegistrationContext/SystemRegistrationContext.h>
#include <HttpRequestor/HttpRequestorBus.h>
#include <HttpRequestor/HttpTypes.h>

namespace GenAIOllama
{

    void BasicJSONRequesterConfiguration::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<BasicJSONRequesterConfiguration, AZ::ComponentConfig>()
                ->Version(0)
                ->Field("url", &BasicJSONRequesterConfiguration::m_url)
                ->Field("contentType", &BasicJSONRequesterConfiguration::m_contentType);

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext
                    ->Class<BasicJSONRequesterConfiguration>("Basic JSON requester configuration", "Basic JSON requester configuration")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "AICore")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("Game"))
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &BasicJSONRequesterConfiguration::m_url,
                        "EndpointURL",
                        "An url with port pointing to an HTTP endpoint (format address:port)")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &BasicJSONRequesterConfiguration::m_contentType,
                        "Content type",
                        "Content type of the request");
            }
        }
    }

    BasicJSONRequesterComponent::BasicJSONRequesterComponent(const BasicJSONRequesterConfiguration& config)
        : m_configuration(config)
    {
    }

    void BasicJSONRequesterComponent::Reflect(AZ::ReflectContext* context)
    {
        BasicJSONRequesterConfiguration::Reflect(context);

        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<BasicJSONRequesterComponent, AZ::Component>()->Version(0)->Field(
                "Configuration", &BasicJSONRequesterComponent::m_configuration);

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<BasicJSONRequesterComponent>("Basic JSON requester component", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "AICore")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &BasicJSONRequesterComponent::m_configuration,
                        "Configuration",
                        "Configuration for the basic JSON requester")
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly);
            }
        }

        if (auto registrationContext = azrtti_cast<GenAIFramework::SystemRegistrationContext*>(context))
        {
            registrationContext->RegisterGenAIFrameworkServiceRequester<BasicJSONRequesterComponent>();
        }
    }

    void BasicJSONRequesterComponent::Activate()
    {
        GenAIFramework::AIServiceRequesterBus::Handler::BusConnect(GetEntityId());
    }

    void BasicJSONRequesterComponent::Deactivate()
    {
        GenAIFramework::AIServiceRequesterBus::Handler::BusDisconnect();
    }

    void BasicJSONRequesterComponent::SendRequest(
        const AZStd::string& request, AZStd::function<void(AZ::Outcome<AZStd::string, AZStd::string>)> callback)
    {
        HttpRequestor::Headers headers;
        headers["Content-Type"] = m_configuration.m_contentType.c_str();

        HttpRequestor::Callback innerCallback =
            [callback](const Aws::Utils::Json::JsonView& jsonView, Aws::Http::HttpResponseCode responseCode)
        {
            AZ::Outcome<AZStd::string, AZStd::string> outcome;
            if (responseCode == Aws::Http::HttpResponseCode::OK)
            {
                AZStd::string jsonViewString = jsonView.WriteCompact().c_str();
                outcome = AZ::Success(jsonViewString);
            }
            else
            {
                outcome = AZ::Failure(AZStd::string::format("Request failed with code: %d", static_cast<int>(responseCode)));
            }
            callback(outcome);
        };

        HttpRequestor::HttpRequestorRequestBus::Broadcast(
            &HttpRequestor::HttpRequestorRequests::AddRequestWithHeadersAndBody,
            m_configuration.m_url,
            Aws::Http::HttpMethod::HTTP_POST,
            headers,
            request,
            innerCallback);
    }

} // namespace GenAIOllama
