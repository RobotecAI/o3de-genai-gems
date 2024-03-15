/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AzCore/Component/Entity.h>
#include <AzCore/Memory/Memory_fwd.h>
#include <AzCore/RTTI/RTTIMacros.h>
#include <AzCore/Serialization/SerializeContext.h>

namespace AICore
{
    //! Configuration for the AICoreSystemComponent
    //! There should be only one instance of this class in existence. All other classes must be copies of the main one.
    class AICoreSystemComponentConfiguration
    {
    public:
        AZ_RTTI(AICoreSystemComponentConfiguration, "{6ab6e636-60dc-4377-bd45-2326cf6a0069}");
        AZ_CLASS_ALLOCATOR(AICoreSystemComponentConfiguration, AZ::SystemAllocator);

        static void Reflect(AZ::ReflectContext* context);

        AICoreSystemComponentConfiguration();
        virtual ~AICoreSystemComponentConfiguration();

        AZStd::vector<AZ::Entity*> m_requesters;
        AZStd::vector<AZ::Entity*> m_generators;

    private:
        static int m_instanceCount;
    };
} // namespace AICore
