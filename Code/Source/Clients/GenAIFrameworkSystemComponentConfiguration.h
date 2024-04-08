/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include "AzCore/std/smart_ptr/shared_ptr.h"
#include <AzCore/Component/Entity.h>
#include <AzCore/Memory/Memory_fwd.h>
#include <AzCore/RTTI/RTTIMacros.h>
#include <AzCore/Serialization/SerializeContext.h>

namespace GenAIFramework
{
    using EntityIdToEntityMap = AZStd::unordered_map<AZ::EntityId, AZStd::shared_ptr<AZ::Entity>>;
    //! Configuration for the GenAIFrameworkSystemComponent
    //! There should be only one instance of this class in existence. All other classes must be copies of the main one.
    class GenAIFrameworkSystemComponentConfiguration
    {
    public:

        AZ_RTTI(GenAIFrameworkSystemComponentConfiguration, "{6ab6e636-60dc-4377-bd45-2326cf6a0069}");
        AZ_CLASS_ALLOCATOR(GenAIFrameworkSystemComponentConfiguration, AZ::SystemAllocator);

        static void Reflect(AZ::ReflectContext* context);

        GenAIFrameworkSystemComponentConfiguration() = default;
        virtual ~GenAIFrameworkSystemComponentConfiguration() = default;

        EntityIdToEntityMap m_serviceRequesters;
        EntityIdToEntityMap m_modelConfigurations;
    };
} // namespace GenAIFramework
