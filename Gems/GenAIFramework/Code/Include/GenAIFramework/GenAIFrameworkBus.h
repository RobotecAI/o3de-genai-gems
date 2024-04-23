/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include "AzCore/base.h"
#include "AzCore/std/containers/vector.h"
#include "AzCore/std/string/string.h"
#include <AzCore/Component/Component.h>
#include <AzCore/Component/EntityId.h>
#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>
#include <AzCore/Math/Uuid.h>
#include <GenAIFramework/GenAIFrameworkTypeIds.h>
#include <GenAIFramework/SystemRegistrationContext/SystemRegistrationContext.h>

namespace GenAIFramework
{
    class GenAIFrameworkRequests
    {
    public:
        AZ_RTTI(GenAIFrameworkRequests, GenAIFrameworkRequestsTypeId);
        virtual ~GenAIFrameworkRequests() = default;


        //! Get a system registration context.
        //! @return A pointer to the SystemRegistrationContext object.
        virtual SystemRegistrationContext* GetSystemRegistrationContext() const = 0;

        //! Get a list of registered model configuration names and corresponding type ids.
        //! @return A vector of pairs with registered model configuration name and corresponding type id.
        virtual AZStd::vector<AZStd::pair<AZStd::string, AZ::Uuid>> GetModelConfigurationNamesAndComponentTypeIds() const = 0;
        //! Get a list of registered service provider names and corresponding type ids.
        //! @return A vector of pairs with registered service provider name and corresponding type id.
        virtual AZStd::vector<AZStd::pair<AZStd::string, AZ::Uuid>> GetServiceProviderNamesAndComponentTypeIds() const = 0;

        //! Get a list of registered model configuration components.
        //! @return A vector of registered model configuration components.
        virtual AZStd::vector<AZ::Component*> GetModelConfigurations() const = 0;
        //! Get a list of registered service provider components.
        //! @return A vector of registered service provider components.
        virtual AZStd::vector<AZ::Component*> GetServiceProviders() const = 0;

        //! Get a list of registered model configuration names.
        //! @return A vector of registered model configuration names.
        virtual AZStd::vector<AZStd::string> GetModelConfigurationNames() const = 0;
        //! Get a list of registered service provider names.
        //! @return A vector of registered service provider names.
        virtual AZStd::vector<AZStd::string> GetServiceProviderNames() const = 0;

        //! Create and register a new model configuration.
        //! @param modelConfigurationName Name of the new model configuration.
        //! @param componentTypeId Uuid of the new model configuration component type.
        //! @return A pointer to created component of a requested type (or nullptr when failed).
        virtual AZ::Component* CreateModelConfiguration(const AZStd::string& modelConfigurationName, const AZ::Uuid& componentTypeId) = 0;
        //! Create and register a new service provider.
        //! @param providerName Name of the new service provider.
        //! @param componentTypeId Uuid of the new service provider component type.
        //! @return A pointer to created component of a requested type (or nullptr when failed).
        virtual AZ::Component* CreateServiceProvider(const AZStd::string& providerName, const AZ::Uuid& componentTypeId) = 0;

        //! Unregister and remove a model configuration or a service provider component.
        //! @param component A pointer to the component to be removed.
        virtual void RemoveComponent(AZ::Component* component) = 0;

        //! Activate the entity and its components.
        //! @param entity A pointer to the entity to be activated.
        virtual void ActivateEntity(AZStd::shared_ptr<AZ::Entity> entity) = 0;

        //! Deactivate the entity and its components.
        //! @param entity A pointer to the entity to be deactivated.
        virtual void DeactivateEntity(AZStd::shared_ptr<AZ::Entity> entity) = 0;

        // Model Agent
        virtual AZ::Outcome<AZ::u64, void> CreateModelAgent(
            const AZStd::string& serviceProviderName, const AZStd::string modelModelConfigurationName) = 0;
        virtual bool RemoveModelAgent(AZ::u64 modelAgentId) = 0;
        virtual bool SendPromptToModelAgent(
            AZ::u64 modelAgentId,
            const AZStd::vector<AZStd::any>& prompt,
            const AZStd::function<void(AZ::Outcome<AZStd::vector<AZStd::any>, AZStd::string>)>& callback) = 0;
    };

    class GenAIFrameworkBusTraits : public AZ::EBusTraits
    {
    public:
        //////////////////////////////////////////////////////////////////////////
        // EBusTraits overrides
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
        //////////////////////////////////////////////////////////////////////////
    };

    using GenAIFrameworkRequestBus = AZ::EBus<GenAIFrameworkRequests, GenAIFrameworkBusTraits>;
    using GenAIFrameworkInterface = AZ::Interface<GenAIFrameworkRequests>;

    class GenAIFrameworkNotifications : public AZ::EBusTraits
    {
    public:
        //////////////////////////////////////////////////////////////////////////
        // EBusTraits overrides
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Multiple;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
        //////////////////////////////////////////////////////////////////////////

        virtual ~GenAIFrameworkNotifications() = default;

        virtual void OnModelConfigurationAdded(const AZ::EntityId& modelConfigurationId) = 0;
        virtual void OnServiceProviderAdded(const AZ::EntityId& serviceProviderId) = 0;

        virtual void OnModelConfigurationRemoved(const AZ::EntityId& modelConfigurationId) = 0;
        virtual void OnServiceProviderRemoved(const AZ::EntityId& serviceProviderId) = 0;
    };

    using GenAIFrameworkNotificationBus = AZ::EBus<GenAIFrameworkNotifications>;
} // namespace GenAIFramework
