/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "OllamaModel.h"
#include <GenAIFramework/SystemRegistrationContext/SystemRegistrationContext.h>

#include <AzCore/Component/Component.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/std/containers/vector.h>
#include <AzCore/std/string/string.h>

#include <aws/core/utils/json/JsonSerializer.h>

namespace GenAIVendorBundle
{

    void OllamaModelConfiguration::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<OllamaModelConfiguration, AZ::ComponentConfig>()
                ->Version(0)
                ->Field("Model", &OllamaModelConfiguration::m_model)
                ->Field("Format", &OllamaModelConfiguration::m_format)
                ->Field("Options", &OllamaModelConfiguration::m_options)
                ->Field("System", &OllamaModelConfiguration::m_system)
                ->Field("Template", &OllamaModelConfiguration::m_template)
                ->Field("Stream", &OllamaModelConfiguration::m_stream)
                ->Field("Raw", &OllamaModelConfiguration::m_raw)
                ->Field("KeepAlive", &OllamaModelConfiguration::m_keepAlive)
                ->Field("useDefaultFormat", &OllamaModelConfiguration::m_useDefaultFormat)
                ->Field("useDefaultOptions", &OllamaModelConfiguration::m_useDefaultOptions)
                ->Field("useDefaultSystem", &OllamaModelConfiguration::m_useDefaultSystem)
                ->Field("useDefaultTemplate", &OllamaModelConfiguration::m_useDefaultTemplate)
                ->Field("useDefaultKeepAlive", &OllamaModelConfiguration::m_useDefaultKeepAlive);

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<OllamaModelConfiguration>("Ollama Model Configuration", "Configuration for the Ollama model")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "AI")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("Game"))
                    ->DataElement(AZ::Edit::UIHandlers::Default, &OllamaModelConfiguration::m_model, "Model", "Model to use for the prompt")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &OllamaModelConfiguration::m_format,
                        "Format",
                        "The format to return a response in. Currently the only accepted value is json")
                    ->Attribute(AZ::Edit::Attributes::ReadOnly, &OllamaModelConfiguration::m_useDefaultFormat)
                    ->DataElement(
                        AZ::Edit::UIHandlers::MultiLineEdit,
                        &OllamaModelConfiguration::m_options,
                        "Options",
                        "Additional model parameters listed in the documentation for the Modelfile such as temperature")
                    ->Attribute(AZ::Edit::Attributes::ReadOnly, &OllamaModelConfiguration::m_useDefaultOptions)
                    ->DataElement(
                        AZ::Edit::UIHandlers::MultiLineEdit,
                        &OllamaModelConfiguration::m_system,
                        "System",
                        "System message to (overrides what is defined in the Modelfile)")
                    ->Attribute(AZ::Edit::Attributes::ReadOnly, &OllamaModelConfiguration::m_useDefaultSystem)
                    ->DataElement(
                        AZ::Edit::UIHandlers::MultiLineEdit,
                        &OllamaModelConfiguration::m_template,
                        "Template",
                        "The prompt template to use (overrides what is defined in the Modelfile)")
                    ->Attribute(AZ::Edit::Attributes::ReadOnly, &OllamaModelConfiguration::m_useDefaultTemplate)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &OllamaModelConfiguration::m_stream,
                        "Stream (disabled as only false is supported)",
                        "If false the response will be returned as a single response object, rather than a stream of objects. Currently "
                        "the only accepted value is false")
                    ->Attribute(AZ::Edit::Attributes::ReadOnly, true)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &OllamaModelConfiguration::m_raw,
                        "Raw (disabled)",
                        "If true no formatting will be applied to the prompt. You may choose to use the raw parameter if you are "
                        "specifying a full templated prompt in your request to the API")
                    ->Attribute(AZ::Edit::Attributes::ReadOnly, true)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &OllamaModelConfiguration::m_keepAlive,
                        "Keep Alive",
                        "Controls how long the model will stay loaded into memory following the request (default: 5m)")
                    ->Attribute(AZ::Edit::Attributes::ReadOnly, &OllamaModelConfiguration::m_useDefaultKeepAlive)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &OllamaModelConfiguration::m_useDefaultFormat,
                        "Use Default Format",
                        "Use the default format")
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, AZ::Edit::PropertyRefreshLevels::AttributesAndValues)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &OllamaModelConfiguration::m_useDefaultOptions,
                        "Use Default Options",
                        "Use the default options")
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, AZ::Edit::PropertyRefreshLevels::AttributesAndValues)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &OllamaModelConfiguration::m_useDefaultSystem,
                        "Use Default System",
                        "Use the default system")
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, AZ::Edit::PropertyRefreshLevels::AttributesAndValues)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &OllamaModelConfiguration::m_useDefaultTemplate,
                        "Use Default Template",
                        "Use the default template")
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, AZ::Edit::PropertyRefreshLevels::AttributesAndValues)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &OllamaModelConfiguration::m_useDefaultKeepAlive,
                        "Use Default Keep Alive",
                        "Use the default keep alive")
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, AZ::Edit::PropertyRefreshLevels::AttributesAndValues);
            }
        }
    }

    OllamaModel::OllamaModel(const OllamaModelConfiguration& config)
        : m_configuration(config)
    {
    }

    void OllamaModel::Reflect(AZ::ReflectContext* context)
    {
        OllamaModelConfiguration::Reflect(context);

        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<OllamaModel, AZ::Component>()->Version(0)->Field("Configuration", &OllamaModel::m_configuration);

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<OllamaModel>("Ollama Model", "Generates prompts and extracts results for the Ollama model")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "Ollama")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("Game"))
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &OllamaModel::m_configuration,
                        "Configuration",
                        "The configuration attached to the prompts")
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly);
            }
        }

        if (auto registrationContext = azrtti_cast<GenAIFramework::SystemRegistrationContext*>(context))
        {
            registrationContext->RegisterModelConfiguration<OllamaModel>();
        }
    }

    void OllamaModel::Activate()
    {
        GenAIFramework::AIModelRequestBus::Handler::BusConnect(GetEntityId());
    }

    void OllamaModel::Deactivate()
    {
        GenAIFramework::AIModelRequestBus::Handler::BusDisconnect();
    }

    GenAIFramework::ModelAPIRequest OllamaModel::PrepareRequest(const GenAIFramework::ModelAPIPrompt& prompt)
    {
        Aws::Utils::Json::JsonValue jsonValue;

        if (!m_configuration.m_useDefaultFormat)
        {
            jsonValue.WithString("format", m_configuration.m_format.c_str());
        }
        if (!m_configuration.m_useDefaultKeepAlive)
        {
            jsonValue.WithString("keep_alive", m_configuration.m_keepAlive.c_str());
        }
        if (!m_configuration.m_useDefaultOptions)
        {
            jsonValue.WithString("options", m_configuration.m_options.c_str());
        }
        if (!m_configuration.m_useDefaultSystem)
        {
            jsonValue.WithString("system", m_configuration.m_system.c_str());
        }
        if (!m_configuration.m_useDefaultTemplate)
        {
            jsonValue.WithString("template", m_configuration.m_template.c_str());
        }

        jsonValue.WithBool("stream", m_configuration.m_stream);
        AZStd::string promptString = "";
        for (const auto& element : prompt)
        {
            promptString += AZStd::any_cast<AZStd::string>(element).c_str();
        }
        jsonValue.WithString("prompt", promptString.c_str());

        jsonValue.WithString("model", m_configuration.m_model.c_str());

        return jsonValue.View().WriteReadable().c_str();
    }

    GenAIFramework::ModelAPIExtractedResponse OllamaModel::ExtractResult(const GenAIFramework::ModelAPIResponse& modelAPIResponse)
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

        return AZ::Success(response);
    }

} // namespace GenAIVendorBundle
