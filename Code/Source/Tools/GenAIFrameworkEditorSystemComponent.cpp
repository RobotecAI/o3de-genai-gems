/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "GenAIFrameworkEditorSystemComponent.h"
#include "SettingsRegistryManager/SettingsRegistryManager.h"

#include <API/ViewPaneOptions.h>
#include <Action/ActionTools/GenAIFrameworkEditorScriptExecutor.h>
#include <AzCore/Component/Entity.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/std/containers/vector.h>
#include <AzCore/std/smart_ptr/make_shared.h>
#include <GenAIFramework/GenAIFrameworkEditorBus.h>
#include <GenAIFramework/GenAIFrameworkTypeIds.h>

namespace GenAIFramework
{
    AZ_COMPONENT_IMPL(
        GenAIFrameworkEditorSystemComponent,
        "GenAIFrameworkEditorSystemComponent",
        GenAIFrameworkEditorSystemComponentTypeId,
        BaseSystemComponent);

    AZStd::unique_ptr<GenAIFrameworkScriptExecutor> GenAIFrameworkEditorSystemComponent::MakeScriptExecutor(
        [[maybe_unused]] const AIContext& aiContext)
    {
        return AZStd::make_unique<GenAIFrameworkEditorScriptExecutor>();
    }

    void GenAIFrameworkEditorSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        // GenAIFrameworkEditorScriptExecutor::Reflect(context);
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<GenAIFrameworkEditorSystemComponent, GenAIFrameworkSystemComponent>()->Version(0);
        }
    }

    GenAIFrameworkEditorSystemComponent::GenAIFrameworkEditorSystemComponent()
    {
        if (GenAIFrameworkEditorInterface::Get() == nullptr)
        {
            GenAIFrameworkEditorInterface::Register(this);
        }
    }

    GenAIFrameworkEditorSystemComponent::~GenAIFrameworkEditorSystemComponent()
    {
        if (GenAIFrameworkEditorInterface::Get() == this)
        {
            GenAIFrameworkEditorInterface::Unregister(this);
        }
    };

    void GenAIFrameworkEditorSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        BaseSystemComponent::GetProvidedServices(provided);
        provided.push_back(AZ_CRC_CE("GenAIFrameworkEditorService"));
    }

    void GenAIFrameworkEditorSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        BaseSystemComponent::GetIncompatibleServices(incompatible);
        incompatible.push_back(AZ_CRC_CE("GenAIFrameworkEditorService"));
    }

    void GenAIFrameworkEditorSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        BaseSystemComponent::GetRequiredServices(required);
    }

    void GenAIFrameworkEditorSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
        BaseSystemComponent::GetDependentServices(dependent);
    }

    void GenAIFrameworkEditorSystemComponent::SaveSystemConfiguration()
    {
        m_settingsRegistryManager.SaveSystemConfiguration(
            m_configuration,
            [](AZ::Outcome<void, void> result)
            {
                if (result.IsSuccess() == false)
                {
                    AZ_Error("GenAIFrameworkEditorSystemComponent", false, "Failed to save system configuration")
                }
            });
    }

    void GenAIFrameworkEditorSystemComponent::Init()
    {
        BaseSystemComponent::Init();
    }

    void GenAIFrameworkEditorSystemComponent::Activate()
    {
        AzToolsFramework::EditorEvents::Bus::Handler::BusConnect();
        BaseSystemComponent::Activate();
    }

    void GenAIFrameworkEditorSystemComponent::Deactivate()
    {
        BaseSystemComponent::Deactivate();
        AzToolsFramework::EditorEvents::Bus::Handler::BusDisconnect();
    }

    void GenAIFrameworkEditorSystemComponent::OnStartPlayInEditorBegin()
    {
        GenAIFrameworkSystemComponent::Activate();
        AZ_Printf("GenAIFrameworkEditorSystemComponent", "OnStartPlayInEditorBegin");
    }
    void GenAIFrameworkEditorSystemComponent::OnStopPlayInEditor()
    {
        AZ_Printf("GenAIFrameworkEditorSystemComponent", "OnStopPlayInEditor");
        GenAIFrameworkSystemComponent::Deactivate();
    }
    void GenAIFrameworkEditorSystemComponent::NotifyRegisterViews()
    {
        AzToolsFramework::ViewPaneOptions options;
        options.paneRect = QRect(100, 100, 700, 400);
        options.showOnToolsToolbar = true;
        options.isDeletable = false;

        AzToolsFramework::RegisterViewPane<GenAIFrameworkWidget>("GenAIFramework", "GenAIFramework", options);
    }

} // namespace GenAIFramework
