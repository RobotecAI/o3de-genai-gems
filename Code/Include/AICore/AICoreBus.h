/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AICore/AIContext.h>
#include <AICore/AICoreScriptExecutor.h>
#include <AICore/AICoreTypeIds.h>
#include <AICore/SystemRegistrationContext/AICoreSystemRegistrationContext.h>
#include <AzCore/Component/Component.h>
#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>
#include <AzCore/Math/Uuid.h>

namespace AICore
{
    class AICoreRequests
    {
    public:
        AZ_RTTI(AICoreRequests, AICoreRequestsTypeId);
        virtual ~AICoreRequests() = default;

        virtual AZStd::unique_ptr<AICoreScriptExecutor> MakeScriptExecutor(const AIContext& aiContext) = 0;

        virtual AICoreSystemRegistrationContext* GetSystemRegistrationContext() = 0;

        virtual AZStd::vector<AZStd::pair<AZStd::string, AZ::Uuid>> GetRegisteredGeneratorsNameAndComponentTypeId() = 0;
        virtual AZStd::vector<AZStd::pair<AZStd::string, AZ::Uuid>> GetRegisteredRequestersNameAndComponentTypeId() = 0;

        virtual AZStd::vector<AZ::Component*> GetActiveGenerators() = 0;
        virtual AZStd::vector<AZ::Component*> GetActiveRequesters() = 0;

        virtual AZ::Component* CreateNewGenerator(const AZStd::string& generatorName, const AZ::Uuid& componentTypeId) = 0;
        virtual AZ::Component* CreateNewRequester(const AZStd::string& requesterName, const AZ::Uuid& componentTypeId) = 0;

        virtual void RemoveComponent(AZ::Component* component) = 0;

        virtual void ActivateEntity(AZ::Entity* entity) = 0;
        virtual void DeactivateEntity(AZ::Entity* entity) = 0;
    };

    class AICoreBusTraits : public AZ::EBusTraits
    {
    public:
        //////////////////////////////////////////////////////////////////////////
        // EBusTraits overrides
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
        //////////////////////////////////////////////////////////////////////////
    };

    using AICoreRequestBus = AZ::EBus<AICoreRequests, AICoreBusTraits>;
    using AICoreInterface = AZ::Interface<AICoreRequests>;
} // namespace AICore
