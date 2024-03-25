/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "GenAIMockSystemComponent.h"
#include <GenAIMock/GenAIMockTypeIds.h>
#include <GenAIMockModuleInterface.h>

namespace GenAIMock
{
    class GenAIMockModule : public GenAIMockModuleInterface
    {
    public:
        AZ_RTTI(GenAIMockModule, GenAIMockModuleTypeId, GenAIMockModuleInterface);
        AZ_CLASS_ALLOCATOR(GenAIMockModule, AZ::SystemAllocator);
    };
} // namespace GenAIMock

#if defined(O3DE_GEM_NAME)
AZ_DECLARE_MODULE_CLASS(AZ_JOIN(Gem_, O3DE_GEM_NAME), GenAIMock::GenAIMockModule)
#else
AZ_DECLARE_MODULE_CLASS(Gem_GenAIMock, GenAIMock::GenAIMockModule)
#endif
