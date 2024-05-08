/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "PromptComponent.h"
#include "GenAIFramework/GenAIFrameworkBus.h"
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/std/string/string.h>

#include <AzCore/Component/TickBus.h>
#include <AzToolsFramework/UI/UICore/WidgetHelpers.h>
#include <GenAIFramework/Communication/AIModelRequestBus.h>
#include <GenAIFramework/Communication/AIServiceProviderBus.h>
#include <QMessageBox>

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
                editContext->Class<PromptComponent>("Test Prompt Component", "Test Prompt Component for Gen AI")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "AI")
                    ->Attribute(AZ::Edit::Attributes::Icon, "Editor/Icons/Components/AI.svg")
                    ->Attribute(AZ::Edit::Attributes::ViewportIcon, "Editor/Icons/Components/Viewport/AI.svg")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("Game"))
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->DataElement(AZ::Edit::UIHandlers::Default, &PromptComponent::m_modelInput, "Model Input", "Input to the model")
                    ->UIElement(AZ::Edit::UIHandlers::Button, "")
                    ->Attribute(AZ::Edit::Attributes::ButtonText, "Prompt input")
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, &PromptComponent::PromptInput)
                    ->DataElement(AZ::Edit::UIHandlers::Default, &PromptComponent::m_modelOutput, "Model Output", "Output from the model")
                    ->Attribute(AZ::Edit::Attributes::ReadOnly, true);
            }
        }
    }

    void PromptComponent::Activate()
    {
        AIComponentBase::Activate();
    }

    void PromptComponent::Deactivate()
    {
        AIComponentBase::Deactivate();
    }

    AZ::Crc32 PromptComponent::PromptInput()
    {
        // prepare the request
        ModelAPIPrompt prompt;

        prompt.push_back(AZStd::any(m_modelInput));

        // send the request
        auto callback = [this](GenAIFramework::ModelAPIExtractedResponse outcome)
        {
            if (outcome.IsSuccess())
            {
                AZStd::string responseString = "";
                for (const auto& element : outcome.GetValue())
                {
                    responseString += AZStd::any_cast<AZStd::string>(element).c_str();
                }

                m_modelOutput = responseString;
                AZ::SystemTickBus::QueueFunction(
                    [=]()
                    {
                        QMessageBox::information(
                            AzToolsFramework::GetActiveWindow(), "PromptComponent", QString(m_modelOutput.c_str()), QMessageBox::Ok);
                    });
            }
            else
            {
                m_modelOutput = "Error: " + outcome.GetError();
                AZ_Warning("PromptComponent", false, "Cannot get a response from the model: %s", m_modelOutput.c_str());
                AZ::SystemTickBus::QueueFunction(
                    [=]()
                    {
                        QMessageBox::warning(
                            AzToolsFramework::GetActiveWindow(), "PromptComponent", QString(m_modelOutput.c_str()), QMessageBox::Ok);
                    });
            }
        };

        GenAIFrameworkInterface::Get()->SendPromptToModelAgent(m_agentId, prompt, callback);

        return AZ::Edit::PropertyRefreshLevels::EntireTree;
    }
} // namespace GenAIFramework
