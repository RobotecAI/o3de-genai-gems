/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */
#pragma once

#include "GenAIFrameworkActionHandler.h"
#include <GenAIFramework/AIContext.h>
#include <GenAIFramework/GenAIFrameworkActionBus.h>
#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/RTTI/TypeInfo.h>
#include <AzCore/std/containers/unordered_map.h>
#include <AzCore/std/string/string.h>

namespace GenAIFramework
{
    // This is meant to exist in both Editor and Launcher scope
    class GenAIFrameworkActionRequestHandler : public GenAIFrameworkActionRequestBus::Handler
    {
    public:
        AZ_CLASS_ALLOCATOR(GenAIFrameworkActionRequestHandler, AZ::SystemAllocator);
        AZ_RTTI(GenAIFrameworkActionRequestHandler, "{CC8AB7A7-C852-47A0-84CA-F142D9108337}");

        void RegisterAIContext(const AIContext& context) override;
        void UnregisterAIContext(const AIContext& context) override;
        void UnregisterAIContext(const AZStd::string& key) override;

        void RegisterBehaviorMethod(const AZ::BehaviorMethod* method, const AIContext& aiContext) override;
        bool ScriptCall(const AZStd::string& script, AZStd::string& response, const AIContext& m_aiContext) override;

        void Connect();
        void Disconnect();

    private:
        AZStd::unordered_map<AZStd::string, AZStd::shared_ptr<GenAIFrameworkActionHandler>> m_handlers;
    };
} // namespace GenAIFramework
