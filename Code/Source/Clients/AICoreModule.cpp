/*
* Copyright (c) Contributors to the Open 3D Engine Project.
* For complete copyright and license terms please see the LICENSE at the root of this distribution.
*
* SPDX-License-Identifier: Apache-2.0 OR MIT
*
*/

#include <AICore/AICoreTypeIds.h>
#include <AICoreModuleInterface.h>
#include "AICoreSystemComponent.h"

namespace AICore
{
    class AICoreModule
        : public AICoreModuleInterface
    {
    public:
        AZ_RTTI(AICoreModule, AICoreModuleTypeId, AICoreModuleInterface);
        AZ_CLASS_ALLOCATOR(AICoreModule, AZ::SystemAllocator);
    };
}// namespace AICore

AZ_DECLARE_MODULE_CLASS(Gem_AICore, AICore::AICoreModule)
