/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "GenAIAsyncRequestEditorSystemComponent.h"
#include "GenAIFrameworkEditorSystemComponent.h"
#include "PromptComponent.h"
#include <GenAIFramework/GenAIFrameworkTypeIds.h>
#include <GenAIFrameworkModuleInterface.h>
#include <QtCore/qglobal.h>
void InitQtResources()
{
    // Registration of Qt (GenAIFramework.qrc) resources
    Q_INIT_RESOURCE(GenAIFramework);
}

namespace GenAIFramework
{
    class GenAIFrameworkEditorModule : public GenAIFrameworkModuleInterface
    {
    public:
        AZ_RTTI(GenAIFrameworkEditorModule, GenAIFrameworkEditorModuleTypeId, GenAIFrameworkModuleInterface);
        AZ_CLASS_ALLOCATOR(GenAIFrameworkEditorModule, AZ::SystemAllocator);

        GenAIFrameworkEditorModule()
        {
            InitQtResources();

            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            // Add ALL components descriptors associated with this gem to m_descriptors.
            // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and
            // EditContext. This happens through the [MyComponent]::Reflect() function.
            m_descriptors.insert(
                m_descriptors.end(),
                {
                    GenAIFrameworkEditorSystemComponent::CreateDescriptor(),
                    GenAIAsyncRequestEditorSystemComponent::CreateDescriptor(),
                    PromptComponent::CreateDescriptor(),
                });
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         * Non-SystemComponents should not be added here
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList{ azrtti_typeid<GenAIFrameworkEditorSystemComponent>(),
                                          azrtti_typeid<GenAIAsyncRequestEditorSystemComponent>() };
        }
    };
} // namespace GenAIFramework

AZ_DECLARE_MODULE_CLASS(Gem_GenAIFramework, GenAIFramework::GenAIFrameworkEditorModule)
