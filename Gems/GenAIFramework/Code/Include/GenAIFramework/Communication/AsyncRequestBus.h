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
#include <AzCore/Math/Uuid.h>
#include <AzCore/Outcome/Outcome.h>
#include <AzCore/RTTI/TemplateInfo.h>

namespace GenAIFramework
{
    //! EBus interface for non-blocking requests to LLM models.
    class AsyncRequests
    {
    public:
        AZ_RTTI(AsyncRequests, "{018e7a12-5db2-7daa-a764-78d7703c8efa}");

        //! Set the selected model configuration and service provider with entity ids.
        //! @param modelConfigurationId Entity id of the target model configuration.
        //! @param serviceProviderId Entity id of the target service provider.
        virtual void SetProviderAndModel(AZ::EntityId modelConfigurationId, AZ::EntityId serviceProviderId) = 0;

        //! Set the service provider by name. If the service provider does not exist, it will not be set.
        //! @param providerName The name of the target service provider.
        //! @return True if the service provider was set successfully, false otherwise.
        virtual bool SetServiceProviderByName(const AZStd::string& providerName) = 0;

        //! Set the model configuration by name. If the model configuration does not exist, it will not be set.
        //! @param modelConfigurationName The name of the target model configuration.
        //! @return True if the model configuration was set successfully, false otherwise.
        virtual bool SetModelConfigurationByName(const AZStd::string& modelConfigurationName) = 0;

        //! Get a name of the active service provider.
        //! @return The name of the active service provider; empty if not set.
        virtual AZStd::string GetActiveServiceProviderName() = 0;

        //! Get a name of the model configuration.
        //! @return The name of the active model configuration; empty if not set.
        virtual AZStd::string GetActiveModelConfigurationName() = 0;

        //! Non-blocking call to send a prompt to LLM.
        //! @param prompt A prompt to send.
        //! @return A unique identifier for the prompt.
        virtual AZ::Uuid SendPromptToLLM(const AZStd::string& prompt) = 0;

        //! Check if the response to the prompt is ready.
        //! @param promptId A unique identifier for the prompt
        //! @return True if the response for given identifier is ready; false otherwise.
        virtual bool IsResponseReady(AZ::Uuid promptId) = 0;

        //! Get the response to the prompt.
        //! @param promptId A unique identifier for the prompt.
        //! @return Returns the response to the prompt when ready; an empty string otherwise.
        virtual AZStd::string GetResponse(AZ::Uuid promptId) = 0;

        //! Get the registered name of the active model configuration.
        //! @return The registered name of the active model configuration or an empty string if not set.
        virtual AZStd::string GetModelConfigurationTypename() = 0;

        //! Get the registered name of the active service provider.
        //! @return The registered name of the active service provider or an empty string if not set.
        virtual AZStd::string GetServiceProviderTypename() = 0;

        //! Reset the history of the currently selected model.
        virtual void ResetModelHistory() = 0;

        //! Enable or disable the history of the currently selected model.
        //! @param enableHistory A flag to enable/disable the history.
        virtual void EnableModelHistory(bool enableHistory) = 0;

        //! Set custom model parameters.
        //! @param parameterName The name of the parameter to set.
        //! @param parameterValue The value to set the parameter as a string.
        virtual AZStd::string SetModelParameter(const AZStd::string& parameterName, const AZStd::string& parameterValue) = 0;
    };

    class RequestsUtilsBusTraits : public AZ::EBusTraits
    {
    public:
        // EBusTraits overrides
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
    };

    using AsyncRequestBus = AZ::EBus<AsyncRequests, RequestsUtilsBusTraits>;
    using AsyncRequestBusInterface = AZ::Interface<AsyncRequests>;

} // namespace GenAIFramework
