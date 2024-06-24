/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <GenAIFramework/Communication/AIModelRequestBus.h>
#include <GenAIFramework/GenAIFrameworkTypeIds.h>

#include <AzCore/EBus/EBus.h>
#include <AzCore/RTTI/RTTI.h>

namespace GenAIFramework
{
    class AIModelAgentRequests : public AZ::EBusTraits
    {
    public:
        AZ_RTTI(AIModelAgentRequests, AIModelAgentRequestsTypeId);
        virtual ~AIModelAgentRequests() = default;

        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::ById;
        using BusIdType = AZ::u64;

        //! Send a prompt to the model agent.
        //! @param prompt The prompt to send.
        //! The response will be sent to the OnPromptResponse notification.
        virtual void SendPrompt(const AIMessages& prompt) = 0;

        //! Get the history of the model agent.
        //! @return The history of the model agent.
        virtual AIHistory GetHistory() const = 0;
    };

    class AIModelAgentNotifications : public AZ::EBusTraits
    {
    public:
        AZ_RTTI(AIModelAgentNotifications, AIModelAgentNotificationsTypeId);
        virtual ~AIModelAgentNotifications() = default;

        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::ById;
        using BusIdType = AZ::u64;

        //! Notification that the model agent has a response to the prompt.
        //! @param response The response from the model agent.
        virtual void OnPromptResponse(ModelAPIExtractedResponse response)
        {
            AZ_UNUSED(response);
        };
    };

    using AIModelAgentRequestBus = AZ::EBus<AIModelAgentRequests>;
    using AIModelAgentNotificationBus = AZ::EBus<AIModelAgentNotifications>;
} // namespace GenAIFramework