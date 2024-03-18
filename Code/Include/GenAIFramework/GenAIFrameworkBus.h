/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <GenAIFramework/AIContext.h>
#include <GenAIFramework/GenAIFrameworkScriptExecutor.h>
#include <GenAIFramework/GenAIFrameworkTypeIds.h>
#include <GenAIFramework/SystemRegistrationContext/GenAIFrameworkSystemRegistrationContext.h>
#include <AzCore/Component/Component.h>
#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>
#include <AzCore/Math/Uuid.h>

namespace GenAIFramework
{
    class GenAIFrameworkRequests
    {
    public:
        AZ_RTTI(GenAIFrameworkRequests, GenAIFrameworkRequestsTypeId);
        virtual ~GenAIFrameworkRequests() = default;

        virtual AZStd::unique_ptr<GenAIFrameworkScriptExecutor> MakeScriptExecutor(const AIContext& aiContext) = 0;

        virtual GenAIFrameworkSystemRegistrationContext* GetSystemRegistrationContext() = 0;

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
} // namespace GenAIFramework
