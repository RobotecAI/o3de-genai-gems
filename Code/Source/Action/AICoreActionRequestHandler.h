/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */
#pragma once

#include "AICoreActionHandler.h"
#include <AICore/AIContext.h>
#include <AICore/AICoreActionBus.h>
#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/RTTI/TypeInfo.h>
#include <AzCore/std/containers/unordered_map.h>
#include <AzCore/std/string/string.h>

namespace AICore
{
    // This is meant to exist in both Editor and Launcher scope
    class AICoreActionRequestHandler : public AICoreActionRequestBus::Handler
    {
    public:
        AZ_CLASS_ALLOCATOR(AICoreActionRequestHandler, AZ::SystemAllocator);
        AZ_RTTI(AICoreActionRequestHandler, "{CC8AB7A7-C852-47A0-84CA-F142D9108337}");

        void RegisterAIContext(const AIContext& context) override;
        void UnregisterAIContext(const AIContext& context) override;
        void UnregisterAIContext(const AZStd::string& key) override;

        void RegisterBehaviorMethod(const AZ::BehaviorMethod* method, const AIContext& aiContext) override;
        bool ScriptCall(const AZStd::string& script, AZStd::string& response, const AIContext& m_aiContext) override;

        void Connect();
        void Disconnect();

    private:
        AZStd::unordered_map<AZStd::string, AZStd::shared_ptr<AICoreActionHandler>> m_handlers;
    };
} // namespace AICore
