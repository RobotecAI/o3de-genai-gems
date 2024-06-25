/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "MockModelComponent.h"
#include <GenAIFramework/SystemRegistrationContext/SystemRegistrationContext.h>

#include <AzCore/Component/ComponentBus.h>
#include <AzCore/Outcome/Outcome.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/std/any.h>
#include <AzCore/std/string/string.h>

namespace GenAIMock
{
    void MockModelComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto* serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<MockModelComponent, AZ::Component>()->Version(0);

            if (auto* editContext = serializeContext->GetEditContext())
            {
                editContext->Class<MockModelComponent>("Mock Model", "Pass through for AI requests and AI responses")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "AI");
            }
        }

        if (auto registrationContext = azrtti_cast<GenAIFramework::SystemRegistrationContext*>(context))
        {
            registrationContext->RegisterModelConfiguration<MockModelComponent>();
        }
    }

    void MockModelComponent::Activate()
    {
        GenAIFramework::AIModelRequestBus::Handler::BusConnect(GetEntityId());
    }

    void MockModelComponent::Deactivate()
    {
        GenAIFramework::AIModelRequestBus::Handler::BusDisconnect();
    }

    GenAIFramework::ModelAPIRequest MockModelComponent::PrepareRequest(const GenAIFramework::AIMessages& prompt)
    {
        // Get the last user prompt: reverse iterate the prompts with context
        for (auto it = prompt.rbegin(); it != prompt.rend(); ++it)
        {
            if (it->first == GenAIFramework::Role::User)
            {
                AZStd::string fullPrompt = "";
                for (const auto& promptPart : it->second)
                {
                    if (promptPart.is<AZStd::string>())
                    {
                        fullPrompt += AZStd::any_cast<AZStd::string>(promptPart).c_str();
                    }
                }
                return fullPrompt;
            }
        }

        return {};
    }

    GenAIFramework::ModelAPIExtractedResponse MockModelComponent::ExtractResult(const GenAIFramework::ModelAPIResponse& modelAPIResponse)
    {
        // The mock model returns the data that does not need additional modifications
        AZStd::vector<AZStd::any> modelAPIResponseVector;
        if (modelAPIResponse.IsSuccess())
        {
            modelAPIResponseVector.push_back(AZStd::any(modelAPIResponse.GetValue()));
        }
        else
        {
            modelAPIResponseVector.push_back(AZStd::any(modelAPIResponse.GetError()));
        }

        GenAIFramework::AIMessage message = { GenAIFramework::Role::Assistant, modelAPIResponseVector };
        return message;
    }
} // namespace GenAIMock
