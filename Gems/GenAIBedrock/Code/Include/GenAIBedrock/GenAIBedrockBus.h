
#pragma once

#include <GenAIBedrock/GenAIBedrockTypeIds.h>

#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>

namespace GenAIBedrock
{
    class GenAIBedrockRequests
    {
    public:
        AZ_RTTI(GenAIBedrockRequests, GenAIBedrockRequestsTypeId);
        virtual ~GenAIBedrockRequests() = default;
        // Put your public methods here
    };

    class GenAIBedrockBusTraits : public AZ::EBusTraits
    {
    public:
        //////////////////////////////////////////////////////////////////////////
        // EBusTraits overrides
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
        //////////////////////////////////////////////////////////////////////////
    };

    using GenAIBedrockRequestBus = AZ::EBus<GenAIBedrockRequests, GenAIBedrockBusTraits>;
    using GenAIBedrockInterface = AZ::Interface<GenAIBedrockRequests>;

} // namespace GenAIBedrock
