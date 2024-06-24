/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <GenAIFramework/Feature/FeatureBase.h>
#include <GenAIFramework/GenAIFrameworkTypeIds.h>

#include <AzCore/Math/Uuid.h>
#include <AzCore/Memory/Memory_fwd.h>
#include <AzCore/RTTI/RTTIMacros.h>
#include <AzCore/RTTI/ReflectContext.h>
#include <AzCore/std/containers/unordered_map.h>
#include <AzCore/std/containers/unordered_set.h>
#include <AzCore/std/smart_ptr/make_shared.h>

namespace GenAIFramework
{
    class SystemRegistrationContext : public AZ::ReflectContext
    {
    public:
        SystemRegistrationContext() = default;
        ~SystemRegistrationContext() = default;

        AZ_CLASS_ALLOCATOR(SystemRegistrationContext, AZ::SystemAllocator);
        AZ_RTTI(SystemRegistrationContext, SystemRegistrationContextTypeId, AZ::ReflectContext);

        template<class C>
        void RegisterServiceProvider()
        {
            m_registeredServiceProviders.emplace(C::RTTI_Type());
        }

        template<class C>
        void RegisterModelConfiguration()
        {
            m_registeredModelConfigurations.emplace(C::RTTI_Type());
        }

        template<class C>
        void RegisterFeature(const AZStd::string& name)
        {
            static_assert(AZStd::is_base_of<FeatureBase, C>::value);
            if (m_featuresFactory.find(C::RTTI_Type()) == m_featuresFactory.end())
            {
                m_featureNames[name] = C::RTTI_Type();
                m_featuresFactory[C::RTTI_Type()] = [](AZ::u64 agentId, AZ::u64 conversationId)
                {
                    return AZStd::make_shared<C>(agentId, conversationId);
                };
            }
            else
            {
                AZ_Error("SystemRegistrationContext", false, "Feature %s already registered", name.c_str());
            }
        }

        inline AZStd::shared_ptr<FeatureBase> CreateFeature(AZ::Uuid feature, AZ::u64 agentId, AZ::u64 conversationId) const
        {
            auto it = m_featuresFactory.find(feature);
            if (it != m_featuresFactory.end())
            {
                return it->second(agentId, conversationId);
            }
            return nullptr;
        }

        inline AZStd::unordered_map<AZStd::string, AZ::Uuid> GetFeatureNamesAndUuids() const
        {
            return m_featureNames;
        }

        inline AZStd::unordered_set<AZ::Uuid> GetRegisteredServiceProviders() const
        {
            return m_registeredServiceProviders;
        }

        inline AZStd::unordered_set<AZ::Uuid> GetRegisteredModelConfigurations() const
        {
            return m_registeredModelConfigurations;
        }

    private:
        AZStd::unordered_set<AZ::Uuid> m_registeredServiceProviders;
        AZStd::unordered_set<AZ::Uuid> m_registeredModelConfigurations;

        AZStd::unordered_map<AZ::Uuid, AZStd::function<AZStd::shared_ptr<FeatureBase>(AZ::u64, AZ::u64)>> m_featuresFactory;
        AZStd::unordered_map<AZStd::string, AZ::Uuid> m_featureNames;
    };
} // namespace GenAIFramework
