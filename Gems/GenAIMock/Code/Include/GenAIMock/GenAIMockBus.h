
#pragma once

#include <GenAIMock/GenAIMockTypeIds.h>

#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>

namespace GenAIMock
{
    class GenAIMockRequests
    {
    public:
        AZ_RTTI(GenAIMockRequests, GenAIMockRequestsTypeId);
        virtual ~GenAIMockRequests() = default;
        // Put your public methods here
    };

    class GenAIMockBusTraits
        : public AZ::EBusTraits
    {
    public:
        //////////////////////////////////////////////////////////////////////////
        // EBusTraits overrides
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
        //////////////////////////////////////////////////////////////////////////
    };

    using GenAIMockRequestBus = AZ::EBus<GenAIMockRequests, GenAIMockBusTraits>;
    using GenAIMockInterface = AZ::Interface<GenAIMockRequests>;

} // namespace GenAIMock
