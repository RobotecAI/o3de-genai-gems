
#pragma once

#include "AzCore/std/string/string.h"
#include <AzCore/Component/ComponentBus.h>
#include <AzCore/EBus/EBus.h>
#include <AzCore/Outcome/Outcome.h>

namespace AICore
{
    template<typename RequestType, typename PromptType>
    class RequestGeneratorRequests : public AZ::ComponentBus
    {
    public:
        static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::ById;
        using BusIdType = AZ::EntityId;

        //! Prepare a request from a specified prompt
        //! @param prompt The prompt to prepare a request from
        //! @return The request to send
        virtual RequestType PrepareRequest(PromptType prompt) = 0;

        //! Process a request and return the result
        //! @param request The request to process
        //! @return The result of the request
        virtual AZ::Outcome<PromptType, AZStd::string> ProcessRequest(RequestType request) = 0;
    };

    template<typename RequestType, typename PromptType>
    using RequestGeneratorBus = AZ::EBus<RequestGeneratorRequests<RequestType, PromptType>>;
} // namespace AICore

namespace AZ
{
    AZ_TYPE_INFO_TEMPLATE(
        AICore::RequestGeneratorRequests, "{8c140e10-e3a2-478b-9886-c7c24eb16816}", AZ_TYPE_INFO_TYPENAME, AZ_TYPE_INFO_TYPENAME);
} // namespace AZ