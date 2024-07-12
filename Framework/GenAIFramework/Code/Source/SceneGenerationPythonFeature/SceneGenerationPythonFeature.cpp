/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "SceneGenerationPythonFeature.h"
#include <GenAIFramework/Communication/AIModelRequestBus.h>
#include <GenAIFramework/Feature/AIAgentBus.h>
#include <GenAIFramework/Feature/ConversationBus.h>
#include <GenAIFramework/Feature/PythonFeatureBase.h>
#include <GenAIFramework/SystemRegistrationContext/SystemRegistrationContext.h>

#include <AzCore/Component/TickBus.h>
#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/Utils/Utils.h>
#include <AzCore/std/containers/vector.h>
#include <AzToolsFramework/API/EditorPythonRunnerRequestsBus.h>

namespace GenAIFramework
{

    SceneGenerationPythonFeature::SceneGenerationPythonFeature(AZ::u64 agentId, AZ::u64 conversationId)
        : PythonFeatureBase(agentId, conversationId)
    {
        const auto thisGemPath = AZ::Utils::GetGemPath("GenAIFramework");

        const auto pythonSceneGenerationAssistantScriptPath = AZ::IO::Path(thisGemPath) / AZ::IO::Path(PythonSceneGenerationScript);
        m_pythonScriptLocation = pythonSceneGenerationAssistantScriptPath.String();

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

    void SceneGenerationPythonFeature::Reflect(AZ::ReflectContext* context)
    {
        if (auto* serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<SceneGenerationPythonFeature, PythonFeatureBase>()->Version(0);
        }

        if (auto registrationContext = azrtti_cast<GenAIFramework::SystemRegistrationContext*>(context))
        {
            registrationContext->RegisterFeature<SceneGenerationPythonFeature>("Scene Generation (Python)");
        }
    }
} // namespace GenAIFramework
