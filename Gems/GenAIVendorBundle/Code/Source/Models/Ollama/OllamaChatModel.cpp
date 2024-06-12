/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "OllamaChatModel.h"

#include <GenAIFramework/SystemRegistrationContext/SystemRegistrationContext.h>

#include <AzCore/Component/Component.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>

#include <aws/core/utils/Array.h>
#include <aws/core/utils/json/JsonSerializer.h>

namespace GenAIVendorBundle
{

    void OllamaChatModelConfiguration::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<OllamaChatModelConfiguration, AZ::ComponentConfig>()
                ->Version(0)
                ->Field("Model", &OllamaChatModelConfiguration::m_model)
                ->Field("Format", &OllamaChatModelConfiguration::m_format)
                ->Field("Options", &OllamaChatModelConfiguration::m_options)
                ->Field("Stream", &OllamaChatModelConfiguration::m_stream)
                ->Field("KeepAlive", &OllamaChatModelConfiguration::m_keepAlive)
                ->Field("useDefaultFormat", &OllamaChatModelConfiguration::m_useDefaultFormat)
                ->Field("useDefaultOptions", &OllamaChatModelConfiguration::m_useDefaultOptions)
                ->Field("useDefaultKeepAlive", &OllamaChatModelConfiguration::m_useDefaultKeepAlive);

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<OllamaChatModelConfiguration>("Ollama Model Configuration", "Configuration for the Ollama model")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "AI")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("Game"))
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default, &OllamaChatModelConfiguration::m_model, "Model", "Model to use for the prompt")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &OllamaChatModelConfiguration::m_format,
                        "Format",
                        "The format to return a response in. Currently the only accepted value is json")
                    ->Attribute(AZ::Edit::Attributes::ReadOnly, &OllamaChatModelConfiguration::m_useDefaultFormat)
                    ->DataElement(
                        AZ::Edit::UIHandlers::MultiLineEdit,
                        &OllamaChatModelConfiguration::m_options,
                        "Options",
                        "Additional model parameters listed in the documentation for the Modelfile such as temperature")
                    ->Attribute(AZ::Edit::Attributes::ReadOnly, &OllamaChatModelConfiguration::m_useDefaultOptions)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &OllamaChatModelConfiguration::m_stream,
                        "Stream (disabled as only false is supported)",
                        "If false the response will be returned as a single response object, rather than a stream of objects. Currently "
                        "the only accepted value is false")
                    ->Attribute(AZ::Edit::Attributes::ReadOnly, true)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &OllamaChatModelConfiguration::m_keepAlive,
                        "Keep Alive",
                        "Controls how long the model will stay loaded into memory following the request (default: 5m)")
                    ->Attribute(AZ::Edit::Attributes::ReadOnly, &OllamaChatModelConfiguration::m_useDefaultKeepAlive)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &OllamaChatModelConfiguration::m_useDefaultFormat,
                        "Use Default Format",
                        "Use the default format")
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, AZ::Edit::PropertyRefreshLevels::AttributesAndValues)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &OllamaChatModelConfiguration::m_useDefaultOptions,
                        "Use Default Options",
                        "Use the default options")
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, AZ::Edit::PropertyRefreshLevels::AttributesAndValues)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &OllamaChatModelConfiguration::m_useDefaultKeepAlive,
                        "Use Default Keep Alive",
                        "Use the default keep alive")
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, AZ::Edit::PropertyRefreshLevels::AttributesAndValues);
            }
        }
    }

    OllamaChatModel::OllamaChatModel(const OllamaChatModelConfiguration& config)
        : m_configuration(config)
    {
    }

    void OllamaChatModel::Reflect(AZ::ReflectContext* context)
    {
        OllamaChatModelConfiguration::Reflect(context);

        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<OllamaChatModel, AZ::Component>()->Version(0)->Field(
                "Configuration", &OllamaChatModel::m_configuration);

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<OllamaChatModel>("Ollama Chat Model", "Generates prompts and extracts results for the Ollama model")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "Ollama")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("Game"))
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &OllamaChatModel::m_configuration,
                        "Configuration",
                        "The configuration attached to the prompts")
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly);
            }
        }

        if (auto registrationContext = azrtti_cast<GenAIFramework::SystemRegistrationContext*>(context))
        {
            registrationContext->RegisterModelConfiguration<OllamaChatModel>();
        }
    }

    void OllamaChatModel::Activate()
    {
        GenAIFramework::AIModelRequestBus::Handler::BusConnect(GetEntityId());
    }

    void OllamaChatModel::Deactivate()
    {
        GenAIFramework::AIModelRequestBus::Handler::BusDisconnect();
    }

    GenAIFramework::ModelAPIRequest OllamaChatModel::PrepareRequest(const GenAIFramework::AIMessages& prompt)
    {
        Aws::Utils::Json::JsonValue jsonRequest;
        Aws::Utils::Array<Aws::Utils::Json::JsonValue> messagesArray(prompt.size());

        for (int i = 0; i < prompt.size(); i++)
        {
            AZStd::string role;
            switch (prompt[i].first)
            {
            case GenAIFramework::Role::System:
                role = "system";
                break;
            case GenAIFramework::Role::User:
                role = "user";
                break;
            case GenAIFramework::Role::Assistant:
                role = "assistant";
                break;
            }

            AZStd::string fullPrompt = "";
            for (const auto& promptPart : prompt[i].second)
            {
                if (promptPart.is<AZStd::string>())
                {
                    fullPrompt += AZStd::any_cast<AZStd::string>(promptPart).c_str();
                }
            }
            Aws::Utils::Json::JsonValue message;
            message.WithString("role", role.c_str());
            message.WithString("content", fullPrompt.c_str());
            messagesArray[i] = message;
        }

        if (!m_configuration.m_useDefaultFormat)
        {
            jsonRequest.WithString("format", m_configuration.m_format.c_str());
        }
        if (!m_configuration.m_useDefaultKeepAlive)
        {
            jsonRequest.WithString("keep_alive", m_configuration.m_keepAlive.c_str());
        }
        if (!m_configuration.m_useDefaultOptions)
        {
            jsonRequest.WithString("options", m_configuration.m_options.c_str());
        }
        jsonRequest.WithBool("stream", m_configuration.m_stream);
        jsonRequest.WithArray("messages", messagesArray);
        jsonRequest.WithString("model", m_configuration.m_model.c_str());

        return jsonRequest.View().WriteReadable().c_str();
    }

    GenAIFramework::ModelAPIExtractedResponse OllamaChatModel::ExtractResult(const GenAIFramework::ModelAPIResponse& modelAPIResponse)
    {
        if (!modelAPIResponse.IsSuccess())
        {
            return AZ::Failure(AZStd::string::format("Failed to get a response from the model: %s", modelAPIResponse.GetError().c_str()));
        }

        AZStd::vector<AZStd::any> response;

        Aws::Utils::Json::JsonValue jsonRequest(modelAPIResponse.GetValue().c_str());
        auto jsonRequestView = jsonRequest.View();

        if (jsonRequestView.ValueExists("message"))
        {
            response.push_back(AZStd::any(AZStd::string(jsonRequestView.GetObject("message").GetString("content").c_str())));
        }
        else
        {
            return AZ::Failure("Failed to parse the response: response field not found in the response JSON.");
        }

        GenAIFramework::AIMessage responseMessage = { GenAIFramework::Role::Assistant, response };
        return AZ::Success(responseMessage);
    }

} // namespace GenAIVendorBundle
