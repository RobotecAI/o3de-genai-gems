/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <GenAIFramework/GenAIFrameworkTypeIds.h>
#include <GenAIFramework/GenAIFrameworkTypes.h>

#include <AzCore/EBus/EBus.h>
#include <AzCore/RTTI/RTTI.h>
#include <AzCore/std/string/string.h>

namespace GenAIFramework
{
    class UIConversationRequests : public AZ::EBusTraits
    {
    public:
        AZ_RTTI(UIConversationRequests, UIConversationRequestsTypeId);
        virtual ~UIConversationRequests() = default;

        static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Multiple;
        static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;

        //! Get the stored chats model, provider and feature names
        //! @return The map of stored chats, where the key is the chat name and the value is tuple
        //! of model, provider and feature names
        virtual FeaturesConversationsMap GetStoredChats() = 0;

        //! EBus notification form the UI when a new chat widget is created
        //! @param chatName The name of the chat
        //! @param modelName The name of the model
        //! @param providerName The name of the provider
        //! @param featureName The name of the feature
        virtual void OnNewChatWidgetCreated(
            const AZStd::string& chatName,
            const AZStd::string& modelName,
            const AZStd::string& providerName,
            const AZStd::string& featureName) = 0;

        //! EBus notification form the UI when a chat widget is closed
        //! @param chatName The name of the chat
        virtual void OnChatWidgetClosed(const AZStd::string& chatName) = 0;
    };

    using UIConversationsBus = AZ::EBus<UIConversationRequests>;
} // namespace GenAIFramework
