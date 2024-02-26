/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <AICore/AICoreActionBus.h>
#include <AzCore/RTTI/AttributeReader.h>
#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/Script/ScriptContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/std/containers/set.h>
#include <BehaviorContextUtils/BehaviorContextDump.h>

#pragma once

namespace AICore
{
    class AICoreActionTest
    {
    public:
        void HackEditorBuses(AZ::BehaviorContext* behaviorContext, const AZStd::string& filter);

        bool ScriptCall(const AZStd::string& script, AZStd::string& response);

        AZStd::string CallRegisteredMethod(
            const AZ::BehaviorMethod* method,
            AZStd::span<AZ::BehaviorArgument> arguments,
            const AIContext& aiContext,
            AZ::BehaviorArgument* result = nullptr);

        const AZ::BehaviorMethod* GetMethod(
            const AZStd::string& reflectedName, const AZStd::string& className, const AZ::BehaviorContext* behaviorContext);

        void SetMethodArguments(const AZ::BehaviorMethod* method, AZStd::span<AZ::BehaviorArgument>& arguments);

        void CallCreateLevelNoPrompt(const AZ::BehaviorContext* behaviorContext);

        void CallIsSimulation(const AZ::BehaviorContext* behaviorContext);

        void Test();

    private:
        bool m_init{ false };
        AIContext m_aiContext;
        AZStd::set<AZStd::string> m_registeredMethods;
    };
} // namespace AICore
