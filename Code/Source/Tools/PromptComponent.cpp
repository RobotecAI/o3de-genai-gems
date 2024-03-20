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
#include <GenAIFramework/Communication/AIModelRequestBus.h>
#include <QMessageBox>

namespace GenAIFramework {
    void PromptComponent::Reflect(AZ::ReflectContext *context) {
        if (auto *serializeContext = azrtti_cast<AZ::SerializeContext *>(context)) {
            serializeContext->Class<PromptComponent, AIComponentBase>()
                    ->Field("ModelInput", &PromptComponent::m_modelInput)
                    ->Field("ModelOutput", &PromptComponent::m_modelOutput);

            if (AZ::EditContext *editContext = serializeContext->GetEditContext()) {
                editContext->Class<PromptComponent>("Test Prompt Component", "Test Prompt Component for AI")
                        ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                        ->Attribute(AZ::Edit::Attributes::Category, "AI")
                        ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("Game"))
                        ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                        ->DataElement(AZ::Edit::UIHandlers::Default, &PromptComponent::m_modelInput, "Model Input",
                                      "Input to the model")
                        ->UIElement(AZ::Edit::UIHandlers::Button, "Prompt input")
                        ->Attribute(AZ::Edit::Attributes::ButtonText, "Prompt input")
                        ->Attribute(AZ::Edit::Attributes::ChangeNotify, &PromptComponent::PromptInput)
                        ->DataElement(AZ::Edit::UIHandlers::Default, &PromptComponent::m_modelOutput, "Model Output",
                                      "Output from the model")
                        ->Attribute(AZ::Edit::Attributes::ReadOnly, true);
            }
        }
    }

    void PromptComponent::Activate() {
    }

    void PromptComponent::Deactivate() {
    }

    AZ::Crc32 PromptComponent::PromptInput() {
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
        GenAIFramework::ModelAPIRequest preparedRequest;
        GenAIFramework::AIModelRequestBus::EventResult(preparedRequest,
                                                       m_selectedModelConfigurationId,
                                                       &GenAIFramework::AIModelRequestBus::Events::PrepareRequest,
                                                       m_modelInput);

        // send the request
        AZ_Printf("PromptComponent", "Prepared request: %s", preparedRequest.c_str());

        auto callback =
                [this](GenAIFramework::ModelAPIResponse outcome) {

                    AZ::Outcome<AZStd::string, AZStd::string> extractedResponse;
                    GenAIFramework::AIModelRequestBus::EventResult(extractedResponse,
                                                                   m_selectedModelConfigurationId,
                                                                   &GenAIFramework::AIModelRequestBus::Events::ExtractResult,
                                                                   outcome);
                    if (extractedResponse.IsSuccess()) {

                        m_modelOutput = extractedResponse.GetValue();
                        AZ_Warning("PromptComponent", false, "Got a response from the model! %s ",
                                   m_modelOutput.c_str());
                    } else {
                        m_modelOutput = "Error: " + extractedResponse.GetError();
                        AZ_Warning("PromptComponent", false, "Cannot get a response from the model: %s",
                                   m_modelOutput.c_str());
                    }
                    AZStd::thread t{
                        [=]{
                            QMessageBox msgBox;
                            msgBox.setText(m_modelOutput.c_str());
                            msgBox.exec();
                        }
                    };
                    t.detach();
                };


        AIServiceRequesterBus::Event(m_selectedServiceRequestorId, &AIServiceRequesterBus::Events::SendRequest,
                                     preparedRequest, callback);

        return AZ::Edit::PropertyRefreshLevels::EntireTree;
    }
} // namespace GenAIFramework