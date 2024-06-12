/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "PassthroughModelComponent.h"
#include <GenAIFramework/SystemRegistrationContext/SystemRegistrationContext.h>

#include <AzCore/Component/ComponentBus.h>
#include <AzCore/Outcome/Outcome.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/std/any.h>
#include <AzCore/std/string/string.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

namespace GenAIMock
{
    void PassthroughModelComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto* serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<PassthroughModelComponent, AZ::Component>()->Version(0);

            if (auto* editContext = serializeContext->GetEditContext())
            {
                editContext->Class<PassthroughModelComponent>("Passthrough Model", "Pass through for AI requests and AI responses")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "AI");
            }
        }

        if (auto registrationContext = azrtti_cast<GenAIFramework::SystemRegistrationContext*>(context))
        {
            registrationContext->RegisterModelConfiguration<PassthroughModelComponent>();
        }
    }

    void PassthroughModelComponent::Activate()
    {
        GenAIFramework::AIModelRequestBus::Handler::BusConnect(GetEntityId());
    }

    void PassthroughModelComponent::Deactivate()
    {
        GenAIFramework::AIModelRequestBus::Handler::BusDisconnect();
    }

    GenAIFramework::ModelAPIRequest PassthroughModelComponent::PrepareRequest(const GenAIFramework::AIMessages& prompt)
    {
        rapidjson::Document document;
        document.SetArray();
        rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

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

            rapidjson::Value message(rapidjson::kObjectType);
            message.AddMember("role", rapidjson::Value().SetString(role.c_str(), allocator), allocator);
            message.AddMember("content", rapidjson::Value().SetString(fullPrompt.c_str(), allocator), allocator);
            document.PushBack(message, allocator);
        }

        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        document.Accept(writer);

        return AZStd::string(buffer.GetString());
    }

    GenAIFramework::ModelAPIExtractedResponse PassthroughModelComponent::ExtractResult(
        const GenAIFramework::ModelAPIResponse& modelAPIResponse)
    {
        // The mock model returns the data that does not need additional modifications
        AZStd::vector<AZStd::any> modelAPIResponseVector;
        modelAPIResponseVector.push_back(AZStd::any(modelAPIResponse));
        GenAIFramework::AIMessage message = { GenAIFramework::Role::Assistant, modelAPIResponseVector };
        return message;
    }
} // namespace GenAIMock
