/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "OllamaGenerateModel.h"
#include <GenAIFramework/SystemRegistrationContext/SystemRegistrationContext.h>

#include <AzCore/Component/Component.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>

#include <aws/core/utils/json/JsonSerializer.h>

namespace GenAIVendorBundle
{

    void OllamaGenerateModelConfiguration::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<OllamaGenerateModelConfiguration, AZ::ComponentConfig>()
                ->Version(0)
                ->Field("Model", &OllamaGenerateModelConfiguration::m_model)
                ->Field("Format", &OllamaGenerateModelConfiguration::m_format)
                ->Field("Options", &OllamaGenerateModelConfiguration::m_options)
                ->Field("System", &OllamaGenerateModelConfiguration::m_system)
                ->Field("Template", &OllamaGenerateModelConfiguration::m_template)
                ->Field("Stream", &OllamaGenerateModelConfiguration::m_stream)
                ->Field("Raw", &OllamaGenerateModelConfiguration::m_raw)
                ->Field("KeepAlive", &OllamaGenerateModelConfiguration::m_keepAlive)
                ->Field("useDefaultFormat", &OllamaGenerateModelConfiguration::m_useDefaultFormat)
                ->Field("useDefaultOptions", &OllamaGenerateModelConfiguration::m_useDefaultOptions)
                ->Field("useDefaultSystem", &OllamaGenerateModelConfiguration::m_useDefaultSystem)
                ->Field("useDefaultTemplate", &OllamaGenerateModelConfiguration::m_useDefaultTemplate)
                ->Field("useDefaultKeepAlive", &OllamaGenerateModelConfiguration::m_useDefaultKeepAlive);

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<OllamaGenerateModelConfiguration>("Ollama Model Configuration", "Configuration for the Ollama model")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "AI")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("Game"))
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default, &OllamaGenerateModelConfiguration::m_model, "Model", "Model to use for the prompt")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &OllamaGenerateModelConfiguration::m_format,
                        "Format",
                        "The format to return a response in. Currently the only accepted value is json")
                    ->Attribute(AZ::Edit::Attributes::ReadOnly, &OllamaGenerateModelConfiguration::m_useDefaultFormat)
                    ->DataElement(
                        AZ::Edit::UIHandlers::MultiLineEdit,
                        &OllamaGenerateModelConfiguration::m_options,
                        "Options",
                        "Additional model parameters listed in the documentation for the Modelfile such as temperature")
                    ->Attribute(AZ::Edit::Attributes::ReadOnly, &OllamaGenerateModelConfiguration::m_useDefaultOptions)
                    ->DataElement(
                        AZ::Edit::UIHandlers::MultiLineEdit,
                        &OllamaGenerateModelConfiguration::m_system,
                        "System",
                        "System message to (overrides what is defined in the Modelfile)")
                    ->Attribute(AZ::Edit::Attributes::ReadOnly, &OllamaGenerateModelConfiguration::m_useDefaultSystem)
                    ->DataElement(
                        AZ::Edit::UIHandlers::MultiLineEdit,
                        &OllamaGenerateModelConfiguration::m_template,
                        "Template",
                        "The prompt template to use (overrides what is defined in the Modelfile)")
                    ->Attribute(AZ::Edit::Attributes::ReadOnly, &OllamaGenerateModelConfiguration::m_useDefaultTemplate)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &OllamaGenerateModelConfiguration::m_stream,
                        "Stream (disabled as only false is supported)",
                        "If false the response will be returned as a single response object, rather than a stream of objects. Currently "
                        "the only accepted value is false")
                    ->Attribute(AZ::Edit::Attributes::ReadOnly, true)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &OllamaGenerateModelConfiguration::m_raw,
                        "Raw (disabled)",
                        "If true no formatting will be applied to the prompt. You may choose to use the raw parameter if you are "
                        "specifying a full templated prompt in your request to the API")
                    ->Attribute(AZ::Edit::Attributes::ReadOnly, true)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &OllamaGenerateModelConfiguration::m_keepAlive,
                        "Keep Alive",
                        "Controls how long the model will stay loaded into memory following the request (default: 5m)")
                    ->Attribute(AZ::Edit::Attributes::ReadOnly, &OllamaGenerateModelConfiguration::m_useDefaultKeepAlive)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &OllamaGenerateModelConfiguration::m_useDefaultFormat,
                        "Use Default Format",
                        "Use the default format")
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, AZ::Edit::PropertyRefreshLevels::AttributesAndValues)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &OllamaGenerateModelConfiguration::m_useDefaultOptions,
                        "Use Default Options",
                        "Use the default options")
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, AZ::Edit::PropertyRefreshLevels::AttributesAndValues)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &OllamaGenerateModelConfiguration::m_useDefaultSystem,
                        "Use Default System",
                        "Use the default system")
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, AZ::Edit::PropertyRefreshLevels::AttributesAndValues)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &OllamaGenerateModelConfiguration::m_useDefaultTemplate,
                        "Use Default Template",
                        "Use the default template")
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, AZ::Edit::PropertyRefreshLevels::AttributesAndValues)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &OllamaGenerateModelConfiguration::m_useDefaultKeepAlive,
                        "Use Default Keep Alive",
                        "Use the default keep alive")
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, AZ::Edit::PropertyRefreshLevels::AttributesAndValues);
            }
        }
    }

    OllamaGenerateModel::OllamaGenerateModel(const OllamaGenerateModelConfiguration& config)
        : m_configuration(config)
    {
    }

    void OllamaGenerateModel::Reflect(AZ::ReflectContext* context)
    {
        OllamaGenerateModelConfiguration::Reflect(context);

        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<OllamaGenerateModel, AZ::Component>()->Version(0)->Field(
                "Configuration", &OllamaGenerateModel::m_configuration);

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext
                    ->Class<OllamaGenerateModel>("Ollama Generate Model", "Generates prompts and extracts results for the Ollama model")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "Ollama")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("Game"))
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &OllamaGenerateModel::m_configuration,
                        "Configuration",
                        "The configuration attached to the prompts")
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly);
            }
        }

        if (auto registrationContext = azrtti_cast<GenAIFramework::SystemRegistrationContext*>(context))
        {
            registrationContext->RegisterModelConfiguration<OllamaGenerateModel>();
        }
    }

    void OllamaGenerateModel::Activate()
    {
        GenAIFramework::AIModelRequestBus::Handler::BusConnect(GetEntityId());
    }

    void OllamaGenerateModel::Deactivate()
    {
        GenAIFramework::AIModelRequestBus::Handler::BusDisconnect();
    }

    GenAIFramework::ModelAPIRequest OllamaGenerateModel::PrepareRequest(const GenAIFramework::AIMessages& prompt)
    {
        Aws::Utils::Json::JsonValue jsonRequest;
        AZStd::string systemMessage = "";
        AZStd::string promptString = "";

        for (const auto& element : prompt)
        {
            switch (element.first)
            {
            case GenAIFramework::Role::System:
                for (const auto& promptPart : element.second)
                {
                    if (promptPart.is<AZStd::string>())
                    {
                        systemMessage += AZStd::any_cast<AZStd::string>(promptPart).c_str();
                    }
                }
                break;
            case GenAIFramework::Role::User:
                for (const auto& promptPart : element.second)
                {
                    if (promptPart.is<AZStd::string>())
                    {
                        promptString += AZStd::any_cast<AZStd::string>(promptPart).c_str();
                    }
                }
                break;
            case GenAIFramework::Role::Assistant:
                break;
            }
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
        if (!systemMessage.empty())
        {
            jsonRequest.WithString("system", systemMessage.c_str());
        }
        else if (!m_configuration.m_useDefaultSystem)
        {
            jsonRequest.WithString("system", m_configuration.m_system.c_str());
        }
        if (!m_configuration.m_useDefaultTemplate)
        {
            jsonRequest.WithString("template", m_configuration.m_template.c_str());
        }

        jsonRequest.WithBool("stream", m_configuration.m_stream);
        jsonRequest.WithString("prompt", promptString.c_str());
        jsonRequest.WithString("model", m_configuration.m_model.c_str());

        return jsonRequest.View().WriteReadable().c_str();
    }

    GenAIFramework::ModelAPIExtractedResponse OllamaGenerateModel::ExtractResult(const GenAIFramework::ModelAPIResponse& modelAPIResponse)
    {
        if (!modelAPIResponse.IsSuccess())
        {
            return AZ::Failure(AZStd::string::format("Failed to get a response from the model: %s", modelAPIResponse.GetError().c_str()));
        }

        AZStd::vector<AZStd::any> response;

        Aws::Utils::Json::JsonValue jsonRequest(modelAPIResponse.GetValue().c_str());
        auto jsonRequestView = jsonRequest.View();

        if (jsonRequestView.ValueExists("response"))
        {
            response.push_back(AZStd::any(AZStd::string(jsonRequestView.GetString("response").c_str())));
        }
        else
        {
            return AZ::Failure("Failed to parse the response: response field not found in the response JSON.");
        }

        GenAIFramework::AIMessage responseMessage = { GenAIFramework::Role::Assistant, response };

        return AZ::Success(responseMessage);
    }

} // namespace GenAIVendorBundle
