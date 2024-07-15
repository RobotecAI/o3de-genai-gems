
#pragma once

#include <GenAISampleFeatures/GenAISampleFeaturesTypeIds.h>

#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>

namespace GenAISampleFeatures
{
    class GenAISampleFeaturesRequests
    {
    public:
        AZ_RTTI(GenAISampleFeaturesRequests, GenAISampleFeaturesRequestsTypeId);
        virtual ~GenAISampleFeaturesRequests() = default;
        // Put your public methods here
    };

    class GenAISampleFeaturesBusTraits
        : public AZ::EBusTraits
    {
    public:
        //////////////////////////////////////////////////////////////////////////
        // EBusTraits overrides
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
        //////////////////////////////////////////////////////////////////////////
    };

    using GenAISampleFeaturesRequestBus = AZ::EBus<GenAISampleFeaturesRequests, GenAISampleFeaturesBusTraits>;
    using GenAISampleFeaturesInterface = AZ::Interface<GenAISampleFeaturesRequests>;

} // namespace GenAISampleFeatures
