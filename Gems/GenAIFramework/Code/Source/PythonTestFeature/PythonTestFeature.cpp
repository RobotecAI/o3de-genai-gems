/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "PythonTestFeature.h"
#include "AzCore/std/string/string.h"
#include <GenAIFramework/Communication/AIModelRequestBus.h>
#include <GenAIFramework/Feature/AIAgentBus.h>
#include <GenAIFramework/Feature/ConversationBus.h>
#include <GenAIFramework/Feature/PythonFeatureBase.h>
#include <GenAIFramework/SystemRegistrationContext/SystemRegistrationContext.h>

#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/Utils/Utils.h>
#include <AzCore/std/containers/vector.h>
#include <AzToolsFramework/API/EditorPythonRunnerRequestsBus.h>
#include <iostream>
#include <string>

namespace GenAIFramework
{

    PythonAssistantFeature::PythonAssistantFeature(AZ::u64 agentId, AZ::u64 conversationId)
        : PythonFeatureBase(agentId, conversationId)
    {
        const auto thisGemPath = AZ::Utils::GetGemPath("GenAIFramework");

        const auto pythonAssistantScriptPath = AZ::IO::Path(thisGemPath) / AZ::IO::Path(PythonAssistantScript);
        m_assistantPythonScriptLocation = pythonAssistantScriptPath.String();

        bool isOk = false;
        AZStd::string_view agentIdString = AZStd::to_string(m_conversationId);
        AZStd::string_view conversationIdString = AZStd::to_string(m_conversationId);

        std::cout << "Executing Python script with agentId: " << agentIdString.cbegin()
                  << " and conversationId: " << conversationIdString.cbegin() << std::endl;

        AZStd::vector<AZStd::string_view> params;
        params.push_back(agentIdString);
        params.push_back(conversationIdString);
        AzToolsFramework::EditorPythonRunnerRequestBus::BroadcastResult(
            isOk,
            &AzToolsFramework::EditorPythonRunnerRequestBus::Events::ExecuteByFilenameWithArgs,
            AZStd::string_view(m_assistantPythonScriptLocation),
            params);
    }

    void PythonAssistantFeature::Reflect(AZ::ReflectContext* context)
    {
        if (auto* serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<PythonAssistantFeature, PythonFeatureBase>()->Version(0);
        }

        if (auto registrationContext = azrtti_cast<GenAIFramework::SystemRegistrationContext*>(context))
        {
            registrationContext->RegisterFeature<PythonAssistantFeature>("O3DE Python assistant feature");
        }
    }

} // namespace GenAIFramework
