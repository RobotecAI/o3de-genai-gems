
#pragma once

#include <AzCore/Component/ComponentBus.h>
#include <AzCore/EBus/EBus.h>
#include <AzCore/Outcome/Outcome.h>
#include <AzCore/RTTI/RTTIMacros.h>
#include <AzCore/std/string/string.h>

namespace GenAIFramework
{
    using ModelResponse = AZ::Outcome<AZStd::string, AZStd::string>; //! The type answer from the model
    using ModelAPIRequest = AZStd::string; //! The type of request to send to the model
    using ModelAPIResponse = AZ::Outcome<AZStd::string, AZStd::string>; //! The type of response from the model

    class AIModelRequest : public AZ::ComponentBus
    {
    public:

        AZ_RTTI(AIModelRequest, "{8c140e10-e3a2-478b-9886-c7c24eb16816}", AZ::ComponentBus);

        static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::ById;
        using BusIdType = AZ::EntityId;

        //! Generates a ready-to-send request to the Language Model created from the prompt.
        //! The request is any string that can be sent to the model to generate a response.
        //! @param prompt The prompt to use
        //! @return The request to send to the model - it should be ready to use the model's API request.
        virtual ModelAPIRequest PrepareRequest(const AZStd::string& prompt) = 0;

        //! Extract the actual response from the model API response
        //! @param modelAPIResponse The response from the model, can be a JSON string that API around the model produced.
        //! @return The response from the model, or an error message if the response could not be extracted.
        virtual AZ::Outcome<AZStd::string, AZStd::string> ExtractResult(const ModelAPIResponse& modelAPIResponse) = 0;

        //! Optional method to set custom model parameters (e.g. stop sequence)
        //! @param parameterName The name of the parameter to set
        //! @param parameterValue The value to set the parameter.
        //! @return True if the parameter was set, false otherwise.
        //! @note This method is optional and can be implemented by the model configuration component - if the model configuration
        //! component does not have particular parameters to set, it should return failure.
        virtual AZ::Outcome<void, AZStd::string> SetModelParameter(const AZ::Name& parameterName, const AZStd::string& parameterValue)
        {
            return AZ::Failure("This model does not have any parameters to set.");
        }
    };
    using AIModelRequestBus = AZ::EBus<AIModelRequest>;
} // namespace GenAIFramework
