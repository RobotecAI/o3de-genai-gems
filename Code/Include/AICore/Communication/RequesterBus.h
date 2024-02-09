
#pragma once

#include "AzCore/Interface/Interface.h"
#include <AzCore/Component/ComponentBus.h>
#include <AzCore/EBus/EBus.h>
#include <AzCore/Outcome/Outcome.h>

namespace AICore
{
    template<class RequestType>
    class RequesterRequests : public AZ::EBusTraits
    {
    public:
        static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
        using BusIdType = AZ::EntityId;

        //! Send a request to a specified endpoint
        //! @param request The request to send
        //! @param callback The callback to be called when the request is complete. The callback will be called with the request and the
        //! result of the request. If the request was successful, the result will be a void. If the request failed, the error string will be
        //! returned.
        virtual void SendRequest(RequestType request, AZStd::function<void(RequestType, AZ::Outcome<void, AZStd::string>)> callback) = 0;
    };

    template<class RequestType>
    using RequesterBus = AZ::EBus<RequesterRequests<RequestType>>;

    template<class RequestType>
    using RequesterInterface = AZ::Interface<RequesterRequests<RequestType>>;
} // namespace AICore
