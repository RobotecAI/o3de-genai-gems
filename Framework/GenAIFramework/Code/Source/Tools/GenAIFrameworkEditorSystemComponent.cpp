/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "GenAIFrameworkEditorSystemComponent.h"

#include <O3DEAssistantPythonFeature/O3DEAssistantPythonFeature.h>
#include <SceneGenerationPythonFeature/SceneGenerationPythonFeature.h>
#include <SettingsRegistryManager/SettingsRegistryManager.h>
#include <UI/AIAssistantWidget.h>

#include <API/ViewPaneOptions.h>
#include <AzCore/Component/Entity.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/std/containers/vector.h>
#include <GenAIFramework/GenAIFrameworkBus.h>
#include <GenAIFramework/GenAIFrameworkEditorBus.h>
#include <GenAIFramework/GenAIFrameworkTypeIds.h>

namespace GenAIFramework
{
    AZ_COMPONENT_IMPL(
        GenAIFrameworkEditorSystemComponent,
        "GenAIFrameworkEditorSystemComponent",
        GenAIFrameworkEditorSystemComponentTypeId,
        BaseSystemComponent);

    void GenAIFrameworkEditorSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        O3DEAssistantPythonFeature::Reflect(context);
        SceneGenerationPythonFeature::Reflect(context);

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
        GenAIFrameworkNotificationBus::Handler::BusConnect();
        BaseSystemComponent::Activate();
    }

    void GenAIFrameworkEditorSystemComponent::Deactivate()
    {
        BaseSystemComponent::Deactivate();
        GenAIFrameworkNotificationBus::Handler::BusDisconnect();
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
        options.canHaveMultipleInstances = false;
        options.isDeletable = false;
        options.toolbarIcon = ":/GenAIFramework/AI.svg";

        AzToolsFramework::RegisterViewPane<AIAssistantWidget>("AIAssistant", "AIAssistant", options);
    }

    void GenAIFrameworkEditorSystemComponent::OnServiceProviderAdded([[maybe_unused]] const AZ::EntityId& serviceProviderId)
    {
        AzToolsFramework::ToolsApplicationEvents::Bus::Broadcast(
            &AzToolsFramework::ToolsApplicationEvents::Bus::Events::InvalidatePropertyDisplay,
            AzToolsFramework::Refresh_AttributesAndValues);
    }
    void GenAIFrameworkEditorSystemComponent::OnServiceProviderRemoved([[maybe_unused]] const AZ::EntityId& serviceProviderId)
    {
        AzToolsFramework::ToolsApplicationEvents::Bus::Broadcast(
            &AzToolsFramework::ToolsApplicationEvents::Bus::Events::InvalidatePropertyDisplay,
            AzToolsFramework::Refresh_AttributesAndValues);
    }
    void GenAIFrameworkEditorSystemComponent::OnModelConfigurationAdded([[maybe_unused]] const AZ::EntityId& modelConfigurationId)
    {
        AzToolsFramework::ToolsApplicationEvents::Bus::Broadcast(
            &AzToolsFramework::ToolsApplicationEvents::Bus::Events::InvalidatePropertyDisplay,
            AzToolsFramework::Refresh_AttributesAndValues);
    }
    void GenAIFrameworkEditorSystemComponent::OnModelConfigurationRemoved([[maybe_unused]] const AZ::EntityId& modelConfigurationId)
    {
        AzToolsFramework::ToolsApplicationEvents::Bus::Broadcast(
            &AzToolsFramework::ToolsApplicationEvents::Bus::Events::InvalidatePropertyDisplay,
            AzToolsFramework::Refresh_AttributesAndValues);
    }

} // namespace GenAIFramework
