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

    GenAIFramework::ModelAPIRequest PassthroughModelComponent::PrepareRequest(const AZStd::string& prompt)
    {
        // This mock model passes the data to the AI service provider unchanged
        return prompt;
    }

    GenAIFramework::ModelAPIResponse PassthroughModelComponent::ExtractResult(const GenAIFramework::ModelAPIResponse& modelAPIResponse)
    {
        // The mock model returns the data that does not need additional modifications
        return modelAPIResponse;
    }
} // namespace GenAIMock