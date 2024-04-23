/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "ClaudeModelTextCompletions.h"
#include "ClaudeModelConfiguration.h"
#include <AzCore/Component/Component.h>
#include <AzCore/RTTI/RTTIMacros.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/std/any.h>
#include <AzCore/std/string/string_view.h>
#include <GenAIFramework/Communication/AIModelRequestBus.h>
#include <GenAIFramework/SystemRegistrationContext/SystemRegistrationContext.h>
#include <aws/core/utils/json/JsonSerializer.h>
#include <sstream>

namespace GenAIVendorBundle
{
    ClaudeModelTextCompletions::ClaudeModelTextCompletions(const ClaudeModelConfiguration& config)
        : m_configuration(config)
    {
    }

    void ClaudeModelTextCompletions::Reflect(AZ::ReflectContext* context)
    {
        ClaudeModelConfiguration::Reflect(context);

        if (auto* serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<ClaudeModelTextCompletions, AZ::Component>()->Version(1)->Field(
                "Default Configuration", &ClaudeModelTextCompletions::m_configuration);

            if (auto* editContext = serializeContext->GetEditContext())
            {
                editContext->Class<ClaudeModelTextCompletions>("Claude Text Completions Model", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "AI")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &ClaudeModelTextCompletions::m_configuration,
                        "Default Configuration",
                        "The default configuration to use when generating prompts")
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly);
            }
        }

        if (auto registrationContext = azrtti_cast<GenAIFramework::SystemRegistrationContext*>(context))
        {
            registrationContext->RegisterModelConfiguration<ClaudeModelTextCompletions>();
        }
    }

    void ClaudeModelTextCompletions::Activate()
    {
        GenAIFramework::AIModelRequestBus::Handler::BusConnect(GetEntityId());
    }

    void ClaudeModelTextCompletions::Deactivate()
    {
        GenAIFramework::AIModelRequestBus::Handler::BusDisconnect();
    }

    GenAIFramework::ModelAPIRequest ClaudeModelTextCompletions::PrepareRequest(const GenAIFramework::ModelAPIPrompt& prompt)
    {
        std::stringstream oss;
        oss << "Human: \"";
        for (const auto& element : prompt)
        {
            oss << AZStd::any_cast<AZStd::string>(element).c_str();
        }
        oss << "\" Assistant: ";
        Aws::Utils::Json::JsonValue jsonPrompt;
        jsonPrompt.WithString("prompt", oss.str().c_str());

        jsonPrompt.WithInteger("max_tokens_to_sample", m_configuration.m_maxTokensToSample);

        if (!m_configuration.m_useDefaultTemperature)
        {
            jsonPrompt.WithDouble("temperature", m_configuration.m_temperature);
        }
        if (!m_configuration.m_useDefaultTopP)
        {
            jsonPrompt.WithDouble("top_p", m_configuration.m_topP);
        }
        if (!m_configuration.m_useDefaultTopK)
        {
            jsonPrompt.WithInteger("top_k", m_configuration.m_topK);
        }
        if (!m_configuration.m_useDefaultStopSequence)
        {
            jsonPrompt.WithString("stop_sequence", m_configuration.m_stopSequence.c_str());
        }
        Aws::String jsonString = jsonPrompt.View().WriteReadable();
        AZ_Printf("ClaudeModelTextCompletions", "Prepared request: %s", jsonString.c_str());
        return jsonString.c_str();
    }

    GenAIFramework::ModelAPIResponse ClaudeModelTextCompletions::ExtractResult(const GenAIFramework::ModelAPIRequest& modelAPIRequest)
    {
        Aws::Utils::Json::JsonValue jsonPrompt(modelAPIRequest.c_str());
        if (jsonPrompt.WasParseSuccessful())
        {
            auto view = jsonPrompt.View();
            auto completion = AZStd::any(AZStd::string(view.GetString("completion").c_str()));
            AZStd::vector<AZStd::any> completionsVector;
            completionsVector.push_back(completion);
            return AZ::Success(completionsVector);
        }
        else
        {
            return AZ::Failure(AZStd::string::format("Failed to parse the response %s", modelAPIRequest.c_str()));
        }
    };
} // namespace GenAIVendorBundle
