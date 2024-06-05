/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AzCore/EBus/EBus.h>
#include <AzCore/RTTI/RTTI.h>
#include <AzCore/std/any.h>
#include <GenAIFramework/Communication/AIModelRequestBus.h>

namespace GenAIFramework
{
    class GenAIFrameworkModelAgentRequests : public AZ::EBusTraits
    {
    public:
        AZ_RTTI(GenAIFrameworkModelAgentRequests, "{e66d5979-457b-4dfd-a749-ae722101649c}");
        virtual ~GenAIFrameworkModelAgentRequests() = default;

        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::ById;
        using BusIdType = AZ::u64;

        //! Send a prompt to the model agent.
        //! @param prompt The prompt to send.
        //! The response will be sent to the OnPromptResponse notification.
        virtual void SendPrompt(const AZStd::vector<AZStd::any>& prompt) = 0;

        //! Get the history of the model agent.
        //! @return The history of the model agent.
        virtual AIHistory GetHistory() const = 0;
    };

    class GenAIFrameworkModelAgentNotifications : public AZ::EBusTraits
    {
    public:
        AZ_RTTI(GenAIFrameworkModelAgentNotifications, "{c317848a-4d75-4e3d-a3a0-e86e621db350}");
        virtual ~GenAIFrameworkModelAgentNotifications() = default;

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

    using GenAIFrameworkModelAgentBus = AZ::EBus<GenAIFrameworkModelAgentRequests>;
    using GenAIFrameworkModelAgentNotificationBus = AZ::EBus<GenAIFrameworkModelAgentNotifications>;
} // namespace GenAIFramework
