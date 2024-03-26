/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AzCore/Component/EntityId.h>
#include <AzCore/Component/Component.h>
#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>
#include <AzCore/Math/Uuid.h>
#include <GenAIFramework/AIContext.h>
#include <GenAIFramework/GenAIFrameworkScriptExecutor.h>
#include <GenAIFramework/GenAIFrameworkTypeIds.h>
#include <GenAIFramework/SystemRegistrationContext/SystemRegistrationContext.h>

namespace GenAIFramework
{
    class GenAIFrameworkRequests
    {
    public:
        AZ_RTTI(GenAIFrameworkRequests, GenAIFrameworkRequestsTypeId);
        virtual ~GenAIFrameworkRequests() = default;

        virtual AZStd::unique_ptr<GenAIFrameworkScriptExecutor> MakeScriptExecutor(const AIContext& aiContext) = 0;

        virtual SystemRegistrationContext* GetSystemRegistrationContext() = 0;

        virtual AZStd::vector<AZStd::pair<AZStd::string, AZ::Uuid>> GetRegisteredModelConfigurationsNameAndComponentTypeId() = 0;
        virtual AZStd::vector<AZStd::pair<AZStd::string, AZ::Uuid>> GetRegisteredServiceRequestersNameAndComponentTypeId() = 0;

        virtual AZStd::vector<AZ::Component*> GetActiveModelConfigurations() = 0;
        virtual AZStd::vector<AZ::Component*> GetActiveServiceRequesters() = 0;

        virtual AZ::Component* CreateNewModelConfiguration(
            const AZStd::string& modelConfigurationName, const AZ::Uuid& componentTypeId) = 0;
        virtual AZ::Component* CreateNewServiceRequester(const AZStd::string& requesterName, const AZ::Uuid& componentTypeId) = 0;

        virtual void RemoveComponent(AZ::Component* component) = 0;

        virtual void ActivateEntity(AZStd::shared_ptr<AZ::Entity> entity) = 0;
        virtual void DeactivateEntity(AZStd::shared_ptr<AZ::Entity> entity) = 0;
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
        virtual void OnServiceRequestorAdded(const AZ::EntityId& serviceRequestorId) = 0;

        virtual void OnModelConfigurationRemoved(const AZ::EntityId& modelConfigurationId) = 0;
        virtual void OnServiceRequestorRemoved(const AZ::EntityId& serviceRequestorId) = 0;
    };

    using GenAIFrameworkNotificationBus = AZ::EBus<GenAIFrameworkNotifications>;
} // namespace GenAIFramework
