/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <GenAIFramework/GenAIFrameworkTypeIds.h>
#include <GenAIFramework/GenAIFrameworkTypes.h>

#include <AzCore/Component/Entity.h>
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
        AZ_RTTI(GenAIFrameworkSystemComponentConfiguration, GenAIFrameworkSystemComponentConfigurationTypeId);
        AZ_CLASS_ALLOCATOR(GenAIFrameworkSystemComponentConfiguration, AZ::SystemAllocator);

        static void Reflect(AZ::ReflectContext* context);

        GenAIFrameworkSystemComponentConfiguration() = default;
        virtual ~GenAIFrameworkSystemComponentConfiguration() = default;

        EntityIdToEntityMap m_serviceProviders;
        EntityIdToEntityMap m_modelConfigurations;

        FeaturesConversationsMap m_FeaturesConversationsMap;
    };
} // namespace GenAIFramework
