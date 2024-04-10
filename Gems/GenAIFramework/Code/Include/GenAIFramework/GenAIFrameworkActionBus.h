/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <GenAIFramework/AIContext.h>
#include <GenAIFramework/GenAIFrameworkTypeIds.h>
#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>

namespace AZ
{
    class BehaviorContext;
    class BehaviorMethod;
}

namespace GenAIFramework
{
    class GenAIFrameworkActionRequests
    {
    public:
        AZ_RTTI(GenAIFrameworkActionRequests, GenAIFrameworkActionRequestsTypeId);
        virtual ~GenAIFrameworkActionRequests() = default;

        virtual void RegisterAIContext(const AIContext& context) = 0;
        virtual void UnregisterAIContext(const AIContext& context) = 0;
        virtual void UnregisterAIContext(const AZStd::string& key) = 0;

        virtual void RegisterBehaviorMethod(const AZ::BehaviorMethod* method, const AIContext& aiContext) = 0;

        virtual bool ScriptCall(const AZStd::string& script, AZStd::string& response, const AIContext& aiContext) = 0;
    };

    class GenAIFrameworkActionBusTraits : public AZ::EBusTraits
    {
    public:
        //////////////////////////////////////////////////////////////////////////
        // EBusTraits overrides
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        //////////////////////////////////////////////////////////////////////////
    };

    using GenAIFrameworkActionRequestBus = AZ::EBus<GenAIFrameworkActionRequests, GenAIFrameworkActionBusTraits>;
    using GenAIFrameworkActionInterface = AZ::Interface<GenAIFrameworkActionRequests>;
} // namespace GenAIFramework
