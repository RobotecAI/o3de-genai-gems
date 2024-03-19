/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "PromptComponent.h"
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/std/string/string.h>

#include <GenAIFramework/Communication/AIServiceRequesterBus.h>
#include <GenAIFramework/ModelConfiguration/ModelConfigurationBus.h>
namespace GenAIFramework
{
    void PromptComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto* serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<PromptComponent, AIComponentBase>()
                ->Field("ModelInput", &PromptComponent::m_modelInput)
                ->Field("ModelOutput", &PromptComponent::m_modelOutput);

            if (AZ::EditContext* editContext = serializeContext->GetEditContext())
            {
                editContext->Class<PromptComponent>("Test Prompt Component", "Test Prompt Component for AI")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "AI")
                    ->Attribute(AZ::Edit::Attributes::Icon, "Icons/Components/AI.png")
                    ->Attribute(AZ::Edit::Attributes::ViewportIcon, "Icons/Components/Viewport/AI.png")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("Game", 0x232b318c))
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->DataElement(AZ::Edit::UIHandlers::Default, &PromptComponent::m_modelInput, "Model Input", "Input to the model")
                    ->UIElement(AZ::Edit::UIHandlers::Button, "Prompt input")
                    ->Attribute(AZ::Edit::Attributes::ButtonText, "Prompt input")
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, &PromptComponent::PromptInput)
                    ->DataElement(AZ::Edit::UIHandlers::Default, &PromptComponent::m_modelOutput, "Model Output", "Output from the model")
                    ->Attribute(AZ::Edit::Attributes::ReadOnly, true);
            }
        }
    }

    void PromptComponent::Activate()
    {
    }

    void PromptComponent::Deactivate()
    {
    }

    AZ::Crc32 PromptComponent::PromptInput()
    {
        UpdateNamedServiceRequestorId();
        UpdateNamedModelConfigurationId();
        AZ_Printf("PromptComponent", "Test button pressed");
        AZ_Printf(
            "PromptComponent",
            "Service Requestor: %s; Service Requestor Id: %s",
            m_selectedServiceRequestorName.c_str(),
            m_selectedServiceRequestorId.ToString().c_str());
        AZ_Printf(
            "PromptComponent",
            "Model Configuration: %s; Model Configuration Id: %s",
            m_selectedModelConfigurationName.c_str(),
            m_selectedModelConfigurationId.ToString().c_str());

        AZStd::mutex mutex;
        // prepare the request
        AZStd::string preparedRequest;
        GenAIFramework::ModelConfigurationBus::EventResult(preparedRequest,
            m_selectedModelConfigurationId, &GenAIFramework::ModelConfigurationBus::Events::PrepareRequestWithStringPrompt, m_modelInput);
        // send the request
        mutex.lock();
        AZStd::function<void(AZ::Outcome<AZStd::string, AZStd::string>)> callback =
            [this, &mutex](AZ::Outcome<AZStd::string, AZStd::string> outcome)
        {
            mutex.unlock();
            if (outcome.IsSuccess())
            {
                AZ_Warning("PromptComponent", false, "Got a response from the model!");
                m_modelOutput = outcome.GetValue();
            }
            else
            {
                AZ_Warning("PromptComponent", false, "Cannot get a response from the model: %s", outcome.GetError().c_str());
            }
        };
        AZ_Error("PromptComponent", m_selectedServiceRequestorId.IsValid(), "m_selectedServiceRequestorId is not valid!");

        AZ_Printf("PromptComponent", "Prepared request: %s", preparedRequest.c_str());

        AIServiceRequesterBus::Event(m_selectedServiceRequestorId, &AIServiceRequesterBus::Events::SendRequest, preparedRequest, callback);
        mutex.lock();

        return AZ::Edit::PropertyRefreshLevels::EntireTree;
    }

} // namespace GenAIFramework