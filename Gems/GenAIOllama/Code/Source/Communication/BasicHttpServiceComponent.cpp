/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "BasicHttpServiceComponent.h"
#include <GenAIFramework/SystemRegistrationContext/SystemRegistrationContext.h>

#include <AzCore/Component/Component.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/std/string/string.h>
#include <HttpRequestor/HttpRequestorBus.h>
#include <HttpRequestor/HttpTypes.h>

namespace GenAIOllama
{
    void BasicHttpServiceConfiguration::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<BasicHttpServiceConfiguration, AZ::ComponentConfig>()
                ->Version(0)
                ->Field("url", &BasicHttpServiceConfiguration::m_url)
                ->Field("contentType", &BasicHttpServiceConfiguration::m_contentType);

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<BasicHttpServiceConfiguration>("Basic HTTP Service Provider Configuration", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "AI")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("Game"))
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &BasicHttpServiceConfiguration::m_url,
                        "Endpoint URL",
                        "An url with port pointing to an HTTP endpoint (format address:port/endpoint_url)")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &BasicHttpServiceConfiguration::m_contentType,
                        "Content type",
                        "Content type of the request");
            }
        }
    }

    BasicHttpServiceComponent::BasicHttpServiceComponent(const BasicHttpServiceConfiguration& config)
        : m_configuration(config)
    {
    }

    void BasicHttpServiceComponent::Reflect(AZ::ReflectContext* context)
    {
        BasicHttpServiceConfiguration::Reflect(context);

        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<BasicHttpServiceComponent, AZ::Component>()->Version(0)->Field(
                "Configuration", &BasicHttpServiceComponent::m_configuration);

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<BasicHttpServiceComponent>("Basic HTTP Service Provider", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "AI")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &BasicHttpServiceComponent::m_configuration,
                        "Configuration",
                        "Configuration for the Basic HTTP Service Provider")
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly);
            }
        }

        if (auto registrationContext = azrtti_cast<GenAIFramework::SystemRegistrationContext*>(context))
        {
            registrationContext->RegisterGenAIFrameworkServiceProvider<BasicHttpServiceComponent>();
        }
    }

    void BasicHttpServiceComponent::Activate()
    {
        GenAIFramework::AIServiceProviderBus::Handler::BusConnect(GetEntityId());
    }

    void BasicHttpServiceComponent::Deactivate()
    {
        GenAIFramework::AIServiceProviderBus::Handler::BusDisconnect();
    }

    void BasicHttpServiceComponent::SendRequest(
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
