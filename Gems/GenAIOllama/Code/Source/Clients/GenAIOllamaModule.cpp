/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "GenAIOllamaSystemComponent.h"
#include <GenAIOllama/GenAIOllamaTypeIds.h>
#include <GenAIOllamaModuleInterface.h>

namespace GenAIOllama
{
    class GenAIOllamaModule : public GenAIOllamaModuleInterface
    {
    public:
        AZ_RTTI(GenAIOllamaModule, GenAIOllamaModuleTypeId, GenAIOllamaModuleInterface);
        AZ_CLASS_ALLOCATOR(GenAIOllamaModule, AZ::SystemAllocator);
    };
} // namespace GenAIOllama

#if defined(O3DE_GEM_NAME)
AZ_DECLARE_MODULE_CLASS(AZ_JOIN(Gem_, O3DE_GEM_NAME), GenAIOllama::GenAIOllamaModule)
#else
AZ_DECLARE_MODULE_CLASS(Gem_GenAIOllama, GenAIOllama::GenAIOllamaModule)
#endif
