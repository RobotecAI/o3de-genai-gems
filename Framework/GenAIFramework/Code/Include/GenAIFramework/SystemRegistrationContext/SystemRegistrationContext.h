/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <GenAIFramework/Communication/AIModelRequestBus.h>
#include <GenAIFramework/Communication/AIServiceProviderBus.h>
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
    //! This class manages all components of GenAIFramework that are available in the system.
    class SystemRegistrationContext : public AZ::ReflectContext
    {
    public:
        SystemRegistrationContext() = default;
        ~SystemRegistrationContext() = default;

        AZ_CLASS_ALLOCATOR(SystemRegistrationContext, AZ::SystemAllocator);
        AZ_RTTI(SystemRegistrationContext, SystemRegistrationContextTypeId, AZ::ReflectContext);

        //! Register a service provider component. The component needs to implement a AIServiceProviderBus::Handler.
        //! The components are stored within this class.
        //! @tparam C service provider component type
        template<class C>
        void RegisterServiceProvider()
        {
            static_assert(AZStd::is_base_of<GenAIFramework::AIServiceProviderBus::Handler, C>::value);
            m_registeredServiceProviders.emplace(C::RTTI_Type());
        }

        //! Register a model configuration component. The component needs to implement a AIModelRequestBus::Handler.
        //! The components are stored within this class.
        //! @tparam C model configuration component type
        template<class C>
        void RegisterModelConfiguration()
        {
            static_assert(AZStd::is_base_of<GenAIFramework::AIModelRequestBus::Handler, C>::value);
            m_registeredModelConfigurations.emplace(C::RTTI_Type());
        }

        //! Register a feature component. The component needs to inherit a FeatureBase.
        //! The components are stored within this class.
        //! @tparam C feature component type
        template<class C>
        void RegisterFeature(const AZStd::string& name)
        {
            static_assert(AZStd::is_base_of<FeatureBase, C>::value);
            if (m_featuresFactory.find(name) == m_featuresFactory.end())
            {
                m_featuresFactory[name] = [](AZ::u64 agentId, AZ::u64 conversationId)
                {
                    return AZStd::make_shared<C>(agentId, conversationId);
                };
            }
            else
            {
                AZ_Error("SystemRegistrationContext", false, "Feature %s already registered", name.c_str());
            }
        }

        //! Create a feature of a certain uuid type based on the internal map of registered features.
        //! @param name name of the feature to create
        //! @param agentId id of the agent connected to the feature
        //! @param conversationId id of the conversation connected to the feature
        //! @return a pointer to a newly created feature or nullptr when creating feature is not possible
        inline AZStd::shared_ptr<FeatureBase> CreateFeature(const AZStd::string& name, AZ::u64 agentId, AZ::u64 conversationId) const
        {
            auto it = m_featuresFactory.find(name);
            if (it != m_featuresFactory.end())
            {
                return it->second(agentId, conversationId);
            }
            return nullptr;
        }

        //! Function for creating a shared pointer of a feature object
        using FeatureMakeShared = AZStd::function<AZStd::shared_ptr<FeatureBase>(AZ::u64, AZ::u64)>;

        //! Get a reference to the internal map of the registered features; each element consist of the feature name and its uuid
        //! @return a reference to the internal map of the features
        inline const AZStd::unordered_map<AZStd::string, FeatureMakeShared>& GetFeatureFactory() const
        {
            return m_featuresFactory;
        }

        //! Get a reference to the internal list (set) of the registered service provider uuids
        //! @return a reference to the set of the registered service provider uuids
        inline const AZStd::unordered_set<AZ::Uuid>& GetRegisteredServiceProviders() const
        {
            return m_registeredServiceProviders;
        }

        //! Get a reference to the internal list (set) of the registered model configuration uuids
        //! @return a reference to the set of the registered model configuration uuids
        inline const AZStd::unordered_set<AZ::Uuid>& GetRegisteredModelConfigurations() const
        {
            return m_registeredModelConfigurations;
        }

    private:
        AZStd::unordered_set<AZ::Uuid> m_registeredServiceProviders;
        AZStd::unordered_set<AZ::Uuid> m_registeredModelConfigurations;
        AZStd::unordered_map<AZStd::string, FeatureMakeShared> m_featuresFactory;
    };
} // namespace GenAIFramework
