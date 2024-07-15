
#include <GenAISampleFeatures/GenAISampleFeaturesTypeIds.h>
#include <GenAISampleFeaturesModuleInterface.h>
#include "GenAISampleFeaturesSystemComponent.h"

namespace GenAISampleFeatures
{
    class GenAISampleFeaturesModule
        : public GenAISampleFeaturesModuleInterface
    {
    public:
        AZ_RTTI(GenAISampleFeaturesModule, GenAISampleFeaturesModuleTypeId, GenAISampleFeaturesModuleInterface);
        AZ_CLASS_ALLOCATOR(GenAISampleFeaturesModule, AZ::SystemAllocator);
    };
}// namespace GenAISampleFeatures

#if defined(O3DE_GEM_NAME)
AZ_DECLARE_MODULE_CLASS(AZ_JOIN(Gem_, O3DE_GEM_NAME), GenAISampleFeatures::GenAISampleFeaturesModule)
#else
AZ_DECLARE_MODULE_CLASS(Gem_GenAISampleFeatures, GenAISampleFeatures::GenAISampleFeaturesModule)
#endif
