/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <GenAIFramework/GenAIFrameworkTypeIds.h>

#include <AzCore/Component/ComponentBus.h>
#include <AzCore/EBus/EBus.h>
#include <AzCore/Name/Name.h>
#include <AzCore/Outcome/Outcome.h>
#include <AzCore/RTTI/RTTIMacros.h>
#include <AzCore/std/any.h>
#include <AzCore/std/containers/vector.h>
#include <AzCore/std/string/string.h>

namespace GenAIFramework
{
    enum class Role
    {
        User = 1,
        Assistant = 2,
        System = 3
    };

    using AIContent = AZStd::vector<AZStd::any>; //!< The type of content sent and received by the AI
    using AIMessage = AZStd::pair<Role, AIContent>; //!< The type of message sent and received by the AI
    using AIMessages = AZStd::vector<AIMessage>; //!< The array of messages received and sent to the AI
    using ModelAPIRequest = AZStd::string; //!< The type of request to send to the model provider
    using ModelAPIResponse = AZ::Outcome<AZStd::string, AZStd::string>; //!< The type of response from the model provider
    using ModelAPIExtractedResponse = AZ::Outcome<AIMessage, AZStd::string>; //!< The type of extracted response from the model
    using AIHistory = AIMessages; //!< The type of AI history

    class AIModelRequests : public AZ::ComponentBus
    {
    public:
        AZ_RTTI(AIModelRequests, AIModelRequestsTypeId, AZ::ComponentBus);

        static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::ById;
        using BusIdType = AZ::EntityId;

        //! Generates a ready-to-send request to the Language Model created from the prompt.
        //! The request is any string that can be sent to the model to generate a response.
        //! @param prompt The prompt to use.
        //! @return The request to send to the model - it should be ready to use the model's API request.
        virtual ModelAPIRequest PrepareRequest(const AIMessages& prompt) = 0;

        //! Extract the actual response from the model API response
        //! @param modelAPIResponse The response from the model, can be a JSON string that API around the model produced.
        //! @return The response from the model, or an error message if the response could not be extracted.
        virtual ModelAPIExtractedResponse ExtractResult(const ModelAPIResponse& modelAPIResponse) = 0;

        //! Reset the model history. This will clear any history that the model has stored.
        virtual void ResetModelHistory()
        {
            return;
        }

        //! Set whether the model should remember the history of the conversation.
        //! @param enableHistory True if the model should remember the history, false otherwise.
        virtual void EnableModelHistory(bool enableHistory)
        {
            AZ_UNUSED(enableHistory);
            return;
        };

        //! Optional method to set custom model parameters (e.g. stop sequence)
        //! @param parameterName The name of the parameter to set.
        //! @param parameterValue The value to set the parameter.
        //! @return True if the parameter was set, false otherwise.
        //! @note This method is optional and can be implemented by the model configuration component - if the model configuration
        //! component does not have particular parameters to set, it should return failure.
        virtual AZ::Outcome<void, AZStd::string> SetModelParameter(
            [[maybe_unused]] const AZ::Name& parameterName, [[maybe_unused]] const AZStd::string& parameterValue)
        {
            return AZ::Failure("This model does not have any parameters to set.");
        }
    };
    using AIModelRequestBus = AZ::EBus<AIModelRequests>;
} // namespace GenAIFramework
