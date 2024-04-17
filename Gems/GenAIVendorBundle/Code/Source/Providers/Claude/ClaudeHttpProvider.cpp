/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "ClaudeHttpProvider.h"

#include <AzCore/Component/Component.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/std/string/string.h>
#include <GenAIFramework/SystemRegistrationContext/SystemRegistrationContext.h>
#include <HttpRequestor/HttpRequestorBus.h>
#include <HttpRequestor/HttpTypes.h>
#include <aws/core/client/ClientConfiguration.h>
#include <cstdlib>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

namespace GenAIClaude
{

    void ClaudeHttpProviderConfiguration::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<ClaudeHttpProviderConfiguration, AZ::ComponentConfig>()
                ->Version(0)
                ->Field("url", &ClaudeHttpProviderConfiguration::m_url)
                ->Field("contentType", &ClaudeHttpProviderConfiguration::m_contentType)
                ->Field("model", &ClaudeHttpProviderConfiguration::m_model)
                ->Field("timeout", &ClaudeHttpProviderConfiguration::m_timeout);

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<ClaudeHttpProviderConfiguration>("Anthropic HTTP requester config", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "Anthropic")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &ClaudeHttpProviderConfiguration::m_url,
                        "EndpointURL",
                        "An url/ip pointing to an HTTP endpoint")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &ClaudeHttpProviderConfiguration::m_contentType,
                        "Content type",
                        "Content type of the request")
                    ->DataElement(AZ::Edit::UIHandlers::Default, &ClaudeHttpProviderConfiguration::m_model, "Model", "Model name")
                    ->DataElement(AZ::Edit::UIHandlers::Default, &ClaudeHttpProviderConfiguration::m_timeout, "Request timeout (ms)", "")
                    ->UIElement(AZ::Edit::UIHandlers::Label, "API key detection")
                    ->Attribute(AZ::Edit::Attributes::Visibility, &ClaudeHttpProviderConfiguration::IsKeyInEnvironment)
                    ->Attribute(
                        AZ::Edit::Attributes::ValueText,
                        "The API key was not detected in the environment variables. Please set the ANTHROPIC_API_KEY environment "
                        "variable.");
            }
        }
    }

    ClaudeHttpProviderConfiguration::ClaudeHttpProviderConfiguration()
    {
        if (getenv("ANTHROPIC_API_KEY") != nullptr)
        {
            m_xAPIKey = getenv("ANTHROPIC_API_KEY");
        }
        else
        {
            m_xAPIKey = "";
        }
    }

    bool ClaudeHttpProviderConfiguration::IsKeyInEnvironment()
    {
        return m_xAPIKey.empty();
    }

    ClaudeHttpProvider::ClaudeHttpProvider(const ClaudeHttpProviderConfiguration& config)
        : m_configuration(config)
    {
    }

    void ClaudeHttpProvider::Reflect(AZ::ReflectContext* context)
    {
        ClaudeHttpProviderConfiguration::Reflect(context);

        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<ClaudeHttpProvider, AZ::Component>()->Version(0)->Field(
                "Configuration", &ClaudeHttpProvider::m_configuration);

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<ClaudeHttpProvider>("Claude HTTP Service Provider", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "AI")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &ClaudeHttpProvider::m_configuration,
                        "Configuration",
                        "Configuration for the Claude HTTP provider")
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly);
            }
        }

        if (auto registrationContext = azrtti_cast<GenAIFramework::SystemRegistrationContext*>(context))
        {
            registrationContext->RegisterGenAIFrameworkServiceProvider<ClaudeHttpProvider>();
        }
    }

    void ClaudeHttpProvider::Activate()
    {
        GenAIFramework::AIServiceProviderBus::Handler::BusConnect(GetEntityId());
    }

    void ClaudeHttpProvider::Deactivate()
    {
        GenAIFramework::AIServiceProviderBus::Handler::BusDisconnect();
    }

    void ClaudeHttpProvider::SendRequest(
        const AZStd::string& request, AZStd::function<void(AZ::Outcome<AZStd::string, AZStd::string>)> callback)
    {
        Aws::Utils::Json::JsonValue jsonRequest(request.c_str());
        rapidjson::Document rapidDocument;
        rapidDocument.SetObject();

        rapidDocument.Parse<0>(request.c_str());

        HttpRequestor::Headers headers;
        headers["content-type"] = m_configuration.m_contentType.c_str();
        headers["x-api-key"] = m_configuration.m_xAPIKey.c_str();
        if (rapidDocument.HasMember("anthropic_version"))
        {
            headers["anthropic-version"] = rapidDocument["anthropic_version"].GetString();
            rapidDocument.RemoveMember("anthropic_version");
        }

        rapidDocument.AddMember("stream", false, rapidDocument.GetAllocator()); // Disable stream as HTTP requestor does not support it
        rapidjson::Value modelValue(m_configuration.m_model.c_str(), rapidDocument.GetAllocator());
        rapidDocument.AddMember("model", modelValue, rapidDocument.GetAllocator());

        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        rapidDocument.Accept(writer);

        AZStd::string requestNoStream = buffer.GetString();

        Aws::Client::ClientConfiguration clientConfiguration;
        clientConfiguration.connectTimeoutMs = m_configuration.m_timeout;
        clientConfiguration.requestTimeoutMs = m_configuration.m_timeout;
        clientConfiguration.httpRequestTimeoutMs = m_configuration.m_timeout;

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
                if (jsonView.KeyExists("type") && jsonView.GetString("type") == "error")
                {
                    outcome = AZ::Failure(AZStd::string::format(
                        "Request failed with code %d and response\n%s", static_cast<int>(responseCode), jsonView.WriteReadable().c_str()));
                }
                else
                {
                    outcome = AZ::Failure(AZStd::string::format("Request failed with code %d", static_cast<int>(responseCode)));
                }
            }
            callback(outcome);
        };

        std::cout << requestNoStream.c_str() << std::endl;

        HttpRequestor::HttpRequestorRequestBus::Broadcast(
            &HttpRequestor::HttpRequestorRequests::AddRequestWithHeadersBodyAndClientConfiguration,
            m_configuration.m_url,
            Aws::Http::HttpMethod::HTTP_POST,
            headers,
            requestNoStream,
            innerCallback,
            clientConfiguration);
    }

} // namespace GenAIClaude
