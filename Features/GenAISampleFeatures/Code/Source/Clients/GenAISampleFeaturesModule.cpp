/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "GenAISampleFeaturesSystemComponent.h"
#include <GenAISampleFeatures/GenAISampleFeaturesTypeIds.h>
#include <GenAISampleFeaturesModuleInterface.h>

namespace GenAISampleFeatures
{
    class GenAISampleFeaturesModule : public GenAISampleFeaturesModuleInterface
    {
    public:
        AZ_RTTI(GenAISampleFeaturesModule, GenAISampleFeaturesModuleTypeId, GenAISampleFeaturesModuleInterface);
        AZ_CLASS_ALLOCATOR(GenAISampleFeaturesModule, AZ::SystemAllocator);
    };
} // namespace GenAISampleFeatures

#if defined(O3DE_GEM_NAME)
AZ_DECLARE_MODULE_CLASS(AZ_JOIN(Gem_, O3DE_GEM_NAME), GenAISampleFeatures::GenAISampleFeaturesModule)
#else
AZ_DECLARE_MODULE_CLASS(Gem_GenAISampleFeatures, GenAISampleFeatures::GenAISampleFeaturesModule)
#endif
