
#pragma once

#include <AzCore/Component/ComponentBus.h>
#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>
#include <AzCore/Outcome/Outcome.h>
#include <AzCore/RTTI/TemplateInfo.h>

namespace AICore
{
    template<class RequestType>
    class RequesterRequests : public AZ::ComponentBus
    {
    public:
        static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::ById;
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
} // namespace AICore

namespace AZ
{
    AZ_TYPE_INFO_TEMPLATE(AICore::RequesterRequests, "{0bcac4e2-09a9-4702-98dd-ef37006664a7}", AZ_TYPE_INFO_CLASS);
}