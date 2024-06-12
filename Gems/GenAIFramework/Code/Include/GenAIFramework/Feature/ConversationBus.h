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
#include <AzCore/std/string/string.h>

namespace GenAIFramework
{
    class ConversationNotifications : public AZ::EBusTraits
    {
    public:
        AZ_RTTI(ConversationNotifications, "{b7b4645a-30f4-4bbf-92a6-1328c671b15f}");

        ConversationNotifications() = default;
        virtual ~ConversationNotifications() = default;

        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Multiple;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::ById;
        using BusIdType = AZ::u64;

        //! Notification that a new message has been created by the user and needs to be sent to a feature.
        //! @param message The message to send.
        virtual void OnNewMessage(const AZStd::string& message)
        {
            AZ_UNUSED(message);
        };

        //! Notification that a feature has a response to the user's message.
        //! @param summary A summary of the response.
        //! @param detailedResponse A detailed response to the user's message.
        virtual void OnFeatureResponse(const AZStd::string& summary, const AZStd::vector<AZStd::string>& detailedResponse)
        {
            AZ_UNUSED(summary);
            AZ_UNUSED(detailedResponse);
        };
    };

    using ConversationNotificationBus = AZ::EBus<ConversationNotifications>;

} // namespace GenAIFramework
