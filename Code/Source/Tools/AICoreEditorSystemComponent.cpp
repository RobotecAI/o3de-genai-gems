/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "AICoreEditorSystemComponent.h"
#include "AICoreSettingsRegistryManager/AICoreSettingsRegistryManager.h"

#include <AICore/AICoreEditorBus.h>
#include <AICore/AICoreTypeIds.h>
#include <API/ViewPaneOptions.h>
#include <Action/ActionTools/AICoreEditorScriptExecutor.h>
#include <AzCore/Component/Entity.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/std/containers/vector.h>
#include <AzCore/std/smart_ptr/make_shared.h>

namespace AICore
{
    AZ_COMPONENT_IMPL(AICoreEditorSystemComponent, "AICoreEditorSystemComponent", AICoreEditorSystemComponentTypeId, BaseSystemComponent);

    AZStd::unique_ptr<AICoreScriptExecutor> AICoreEditorSystemComponent::MakeScriptExecutor([[maybe_unused]] const AIContext& aiContext)
    {
        return AZStd::make_unique<AICoreEditorScriptExecutor>();
    }

    void AICoreEditorSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        //AICoreEditorScriptExecutor::Reflect(context);
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<AICoreEditorSystemComponent, AICoreSystemComponent>()->Version(0);
        }
    }

    AICoreEditorSystemComponent::AICoreEditorSystemComponent()
    {
        if (AICoreEditorInterface::Get() == nullptr)
        {
            AICoreEditorInterface::Register(this);
        }
    }

    AICoreEditorSystemComponent::~AICoreEditorSystemComponent()
    {
        if (AICoreEditorInterface::Get() == this)
        {
            AICoreEditorInterface::Unregister(this);
        }
    };

    void AICoreEditorSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        BaseSystemComponent::GetProvidedServices(provided);
        provided.push_back(AZ_CRC_CE("AICoreEditorService"));
    }

    void AICoreEditorSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        BaseSystemComponent::GetIncompatibleServices(incompatible);
        incompatible.push_back(AZ_CRC_CE("AICoreEditorService"));
    }

    void AICoreEditorSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        BaseSystemComponent::GetRequiredServices(required);
    }

    void AICoreEditorSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
        BaseSystemComponent::GetDependentServices(dependent);
    }

    void AICoreEditorSystemComponent::SaveSystemConfiguration()
    {
        m_settingsRegistryManager.SaveSystemConfiguration(
            m_configuration,
            [](AZ::Outcome<void, void> result)
            {
                if (result.IsSuccess() == false)
                {
                    AZ_Error("AICoreEditorSystemComponent", false, "Failed to save system configuration")
                }
            });
    }

    void AICoreEditorSystemComponent::Init()
    {
        BaseSystemComponent::Init();
    }

    void AICoreEditorSystemComponent::Activate()
    {
        AzToolsFramework::EditorEvents::Bus::Handler::BusConnect();
        m_actionRequestHandler.Connect();
    }

    void AICoreEditorSystemComponent::Deactivate()
    {
        m_actionRequestHandler.Disconnect();
        AzToolsFramework::EditorEvents::Bus::Handler::BusDisconnect();
    }

    void AICoreEditorSystemComponent::OnStartPlayInEditorBegin()
    {
        AICoreSystemComponent::Activate();
        AZ_Printf("AICoreEditorSystemComponent", "OnStartPlayInEditorBegin");
    }
    void AICoreEditorSystemComponent::OnStopPlayInEditor()
    {
        AZ_Printf("AICoreEditorSystemComponent", "OnStopPlayInEditor");
        AICoreSystemComponent::Deactivate();
    }
    void AICoreEditorSystemComponent::NotifyRegisterViews()
    {
        AzToolsFramework::ViewPaneOptions options;
        options.paneRect = QRect(100, 100, 700, 400);
        options.showOnToolsToolbar = true;
        options.isDeletable = false;

        AzToolsFramework::RegisterViewPane<AICoreWidget>("AICore", "AICore", options);
    }

} // namespace AICore
