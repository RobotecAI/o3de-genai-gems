
#pragma once

#include <AzCore/Component/ComponentBus.h>
#include <AzCore/EBus/EBus.h>
#include <AzCore/Outcome/Outcome.h>
#include <AzCore/RTTI/RTTIMacros.h>
#include <AzCore/std/string/string.h>

namespace AICore
{
    class ModelConfigurationRequests : public AZ::ComponentBus
    {
    public:
        AZ_RTTI(ModelConfigurationRequests, "{8c140e10-e3a2-478b-9886-c7c24eb16816}", AZ::ComponentBus);

        static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::ById;
        using BusIdType = AZ::EntityId;

        //! Get the default prompt input
        //! @return The default prompt input
        virtual AZStd::string GetDefaultPromptInput() = 0;

        //! Prepare a request from a specified prompt
        //! @param prompt The full json for the prompt (with the configuration)
        //! @return The request to send
        virtual AZStd::string PrepareRequest(AZStd::string prompt) = 0;

        //! Prepare a request from a specified prompt
        //! This creates a prompt with all the necessary configuration. Ready for sending.
        //! @param prompt The string prompt to use
        //! @return The request to send
        virtual AZStd::string PrepareRequestWithStringPrompt(AZStd::string prompt) = 0;

        //! Process a request and return the result
        //! @param request The request to process
        //! @return The result of the request
        virtual AZ::Outcome<AZStd::string, AZStd::string> ProcessRequest(AZStd::string request) = 0;
    };

    using ModelConfigurationBus = AZ::EBus<ModelConfigurationRequests>;
} // namespace AICore
