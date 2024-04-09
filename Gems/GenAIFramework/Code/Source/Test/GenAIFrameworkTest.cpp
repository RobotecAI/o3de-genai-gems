/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "GenAIFrameworkTest.h"
#include <AzCore/RTTI/AttributeReader.h>
#include <AzCore/RTTI/BehaviorContext.h>

namespace GenAIFramework
{
    namespace Internal
    {
        void HackEditorBuses(AZ::BehaviorContext* behaviorContext, const AZStd::string& filter)
        {
            AZStd::unordered_set<AZ::Crc32> busesToHack;
            for (auto& busesIter : behaviorContext->m_ebuses)
            {
                auto behaviorEbus = busesIter.second;
                if (!behaviorEbus)
                {
                    continue;
                }

                if (!filter.empty() && !behaviorEbus->m_name.contains(filter))
                {
                    continue;
                }

                AZ::Script::Attributes::ScopeFlags commonScope = AZ::Script::Attributes::ScopeFlags::Common;

                auto scopeAttributePair = AZStd::find_if(
                    behaviorEbus->m_attributes.begin(),
                    behaviorEbus->m_attributes.end(),
                    [](const AZ::AttributePair& pair)
                    {
                        return pair.first == AZ::Script::Attributes::Scope;
                    });
                if (scopeAttributePair != behaviorEbus->m_attributes.end())
                {
                    if (scopeAttributePair->second)
                    {
                        delete scopeAttributePair->second;
                    }
                    behaviorEbus->m_attributes.erase(scopeAttributePair);

                    behaviorEbus->m_attributes.push_back(AZStd::make_pair(
                        AZ::Script::Attributes::Scope,
                        scopeAttributePair->second = aznew AZ::AttributeData<AZ::Script::Attributes::ScopeFlags>(commonScope)));
                    AZ_Printf("HackEditorBuses", "Hacking bus %s done", behaviorEbus->m_name.c_str());
                }
            }
        }
    } // namespace Internal

    bool GenAIFrameworkActionTest::ScriptCall(const AZStd::string& script, AZStd::string& response)
    {
        if (!m_init)
        {
            m_aiContext.m_key = "Test";
            auto* genaiframeworkinterface = GenAIFrameworkActionInterface::Get();
            if (!genaiframeworkinterface)
            {
                AZ_Warning("ScriptCall", false, "No GenAIFramework Interface!");
                return false;
            }
            genaiframeworkinterface->RegisterAIContext(m_aiContext);
            m_init = true;
        }

        return GenAIFrameworkActionInterface::Get()->ScriptCall(script, response, m_aiContext);
    }

    AIContext GenAIFrameworkActionTest::GetAIContext() const
    {
        return m_aiContext;
    }
} // namespace GenAIFramework
