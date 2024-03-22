/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */
#include "MockPromptGeneratorComponent.h"
#include <GenAIFramework/SystemRegistrationContext/SystemRegistrationContext.h>

#include <cstdlib>

#include <AzCore/Component/ComponentBus.h>
#include <AzCore/Outcome/Outcome.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>

namespace GenAIMock
{
    void MockPromptGeneratorComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto* serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<MockPromptGeneratorComponent, AZ::Component>()->Version(0);

            if (auto* editContext = serializeContext->GetEditContext())
            {
                editContext->Class<MockPromptGeneratorComponent>("Mock Prompt Generator", "Generates prompts for the Mock system")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "AI");
            }
        }

        if (auto registrationContext = azrtti_cast<GenAIFramework::SystemRegistrationContext*>(context))
        {
            registrationContext->RegisterModelConfiguration<MockPromptGeneratorComponent>();
        }
    }

    void MockPromptGeneratorComponent::Activate()
    {
        GenAIFramework::AIModelRequestBus::Handler::BusConnect(GetEntityId());
    }

    void MockPromptGeneratorComponent::Deactivate()
    {
        GenAIFramework::AIModelRequestBus::Handler::BusDisconnect();
    }

    GenAIFramework::ModelAPIRequest MockPromptGeneratorComponent::PrepareRequest(const AZStd::string& prompt)
    {
        // This request generator accepts three types of prompts:
        // 1) restart - restart the counter and get the first mock outcome from the model
        // 2) next - get the next mock outcome from the model
        // 3) [0..9] - get the n-th mock outcome from the model
        if (prompt == "restart")
        {
            m_lastCompleted = 0;
            return "0";
        }
        else if (prompt == "next")
        {
            m_lastCompleted++;
            return AZStd::to_string(m_lastCompleted);
        }

        int index = std::atoi(prompt.c_str());
        if (index < 0)
        {
            m_lastCompleted = 0;
            return "0";
        }
        else
        {
            m_lastCompleted = index;
            return AZStd::to_string(index);
        }
    }

    GenAIFramework::ModelAPIResponse MockPromptGeneratorComponent::ExtractResult(const GenAIFramework::ModelAPIResponse& modelAPIResponse)
    {
        // The mock model returns the data that does not need additional modifications
        return modelAPIResponse;
    }
} // namespace GenAIMock
