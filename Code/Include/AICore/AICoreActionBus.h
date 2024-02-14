/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AICore/AIContext.h>
#include <AICore/AICoreTypeIds.h>
#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>
#include <AzCore/RTTI/BehaviorContext.h>

namespace AICore
{
    class AICoreActionRequests
    {
    public:
        AZ_RTTI(AICoreActionRequests, AICoreActionRequestsTypeId);
        virtual ~AICoreActionRequests() = default;

        virtual void RegisterBehaviorMethod(const AZ::BehaviorMethod* method, const AIContext& aiContext) = 0;
        virtual void ScriptCall(const AZStd::string& script) = 0;
        virtual AZStd::string CallRegisteredMethod(
            const AZ::BehaviorMethod* method,
            AZStd::span<AZ::BehaviorArgument> arguments,
            const AIContext& aiContext,
            AZ::BehaviorArgument* result = nullptr) = 0;
    };

    class AICoreActionBusTraits : public AZ::EBusTraits
    {
    public:
        //////////////////////////////////////////////////////////////////////////
        // EBusTraits overrides
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
        //////////////////////////////////////////////////////////////////////////
    };

    using AICoreActionRequestBus = AZ::EBus<AICoreActionRequests, AICoreActionBusTraits>;
    using AICoreActionInterface = AZ::Interface<AICoreActionRequests>;
} // namespace AICore
