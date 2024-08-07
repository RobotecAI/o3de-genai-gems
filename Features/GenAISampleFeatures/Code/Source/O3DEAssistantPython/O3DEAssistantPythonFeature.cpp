/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "O3DEAssistantPythonFeature.h"

#include <GenAIFramework/SystemRegistrationContext/SystemRegistrationContext.h>

#include <AzCore/Component/TickBus.h>
#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/Utils/Utils.h>
#include <AzCore/std/containers/vector.h>
#include <AzToolsFramework/API/EditorPythonRunnerRequestsBus.h>

namespace GenAISampleFeatures
{

    O3DEAssistantPythonFeature::O3DEAssistantPythonFeature(AZ::u64 agentId, AZ::u64 conversationId)
        : PythonFeatureBase(agentId, conversationId)
    {
        const auto thisGemPath = AZ::Utils::GetGemPath("GenAISampleFeatures");

        const auto pythonScriptPath = AZ::IO::Path(thisGemPath) / AZ::IO::Path(O3DEAssistantPythonScript);
        m_pythonScriptLocation = pythonScriptPath.String();

        AZ::SystemTickBus::QueueFunction(
            [this]()
            {
                AZStd::string agentIdString = AZStd::to_string(m_conversationId);
                AZStd::string conversationIdString = AZStd::to_string(m_conversationId);

                AZStd::vector<AZStd::string_view> params = { agentIdString, conversationIdString };

                bool isOk = false;
                AzToolsFramework::EditorPythonRunnerRequestBus::BroadcastResult(
                    isOk,
                    &AzToolsFramework::EditorPythonRunnerRequestBus::Events::ExecuteByFilenameWithArgs,
                    AZStd::string_view(m_pythonScriptLocation),
                    params);
            });
    }

    void O3DEAssistantPythonFeature::Reflect(AZ::ReflectContext* context)
    {
        if (auto* serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<O3DEAssistantPythonFeature, GenAIFramework::PythonFeatureBase>()->Version(0);
        }

        if (auto registrationContext = azrtti_cast<GenAIFramework::SystemRegistrationContext*>(context))
        {
            registrationContext->RegisterFeature<O3DEAssistantPythonFeature>("O3DE Assistant (Python)");
        }
    }

} // namespace GenAISampleFeatures
