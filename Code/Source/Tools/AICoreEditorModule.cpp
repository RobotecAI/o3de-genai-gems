/*
* Copyright (c) Contributors to the Open 3D Engine Project.
* For complete copyright and license terms please see the LICENSE at the root of this distribution.
*
* SPDX-License-Identifier: Apache-2.0 OR MIT
*
*/

#include <AICore/AICoreTypeIds.h>
#include <AICoreModuleInterface.h>
#include "AICoreEditorSystemComponent.h"

#include "Communication/JSONHttp/BasicJSONRequesterEditorComponent.h"
#include "PromptTestComponent/PromptTestComponent.h"
#include "RequestGenerator/ollama/OllamaContextRequestGeneratorEditorComponent.h"

namespace AICore
{
    class AICoreEditorModule
        : public AICoreModuleInterface
    {
    public:
        AZ_RTTI(AICoreEditorModule, AICoreEditorModuleTypeId, AICoreModuleInterface);
        AZ_CLASS_ALLOCATOR(AICoreEditorModule, AZ::SystemAllocator);

        AICoreEditorModule()
        {
            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            // Add ALL components descriptors associated with this gem to m_descriptors.
            // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and EditContext.
            // This happens through the [MyComponent]::Reflect() function.
            m_descriptors.insert(
                m_descriptors.end(),
                {
                    AICoreEditorSystemComponent::CreateDescriptor(),
                    BasicJSONRequesterEditorComponent::CreateDescriptor(),
                    OllamaContextRequestGeneratorEditorComponent::CreateDescriptor(),
                    PromptTestComponent::CreateDescriptor(),
                });
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         * Non-SystemComponents should not be added here
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList {
                azrtti_typeid<AICoreEditorSystemComponent>(),
            };
        }
    };
}// namespace AICore

AZ_DECLARE_MODULE_CLASS(Gem_AICore, AICore::AICoreEditorModule)
