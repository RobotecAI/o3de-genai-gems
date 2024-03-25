/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "GenAIBedrockSystemComponent.h"
#include <GenAIBedrock/GenAIBedrockTypeIds.h>
#include <GenAIBedrockModuleInterface.h>

namespace GenAIBedrock
{
    class GenAIBedrockModule : public GenAIBedrockModuleInterface
    {
    public:
        AZ_RTTI(GenAIBedrockModule, GenAIBedrockModuleTypeId, GenAIBedrockModuleInterface);
        AZ_CLASS_ALLOCATOR(GenAIBedrockModule, AZ::SystemAllocator);
    };
} // namespace GenAIBedrock

#if defined(O3DE_GEM_NAME)
AZ_DECLARE_MODULE_CLASS(AZ_JOIN(Gem_, O3DE_GEM_NAME), GenAIBedrock::GenAIBedrockModule)
#else
AZ_DECLARE_MODULE_CLASS(Gem_GenAIBedrock, GenAIBedrock::GenAIBedrockModule)
#endif
