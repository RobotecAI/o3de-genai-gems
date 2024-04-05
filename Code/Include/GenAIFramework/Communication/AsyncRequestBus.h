/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AzCore/Component/ComponentBus.h>
#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>
#include <AzCore/Outcome/Outcome.h>
#include <AzCore/RTTI/TemplateInfo.h>
#include <uuid/uuid.h>

namespace GenAIFramework
{
    //! EBus interface for non-blocking requests to LLM models.
    class AsyncRequest
    {
    public:
        AZ_RTTI(AsyncRequest, "{018e7a12-5db2-7daa-a764-78d7703c8efa}");
        //! Set the selected model configuration and requestor.
        virtual void SetRequestorAndModel(AZ::EntityId selectedModelConfigurationId, AZ::EntityId selectedRequestorId) = 0;

        //! Non-blocking call to send a prompt to LLM. It returns a unique identifier for the prompt.
        //! The response to the prompt will be sent to the SendPromptToLLM method.
        virtual AZ::Uuid SendPromptToLLM(const AZStd::string& prompt) = 0;

        //! Check if the response to the prompt is ready.
        virtual bool IsResponseReady(AZ::Uuid promptId) = 0;

        //! Get the response to the prompt.
        virtual AZStd::string GetResponse(AZ::Uuid promptId) = 0;

        //! Reset the history of the currently selected model.
        virtual void ResetModelHistory() = 0;

        //! Enable or disable the history of the currently selected model.
        virtual void EnableModelHistory(bool enableHistory) = 0;
    };

    class RequestsUtilsBusTraits : public AZ::EBusTraits
    {
    public:
        // EBusTraits overrides
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
    };

    using AsyncRequestBus = AZ::EBus<AsyncRequest, RequestsUtilsBusTraits>;
    using AsyncRequestBusInterface = AZ::Interface<AsyncRequest>;

} // namespace GenAIFramework
