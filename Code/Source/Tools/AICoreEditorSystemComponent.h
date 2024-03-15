/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include "AICoreSettingsRegistryManager/Editor/AICoreEditorSettingsRegistryManager.h"
#include <AICore/AICoreBus.h>
#include <AICore/AICoreEditorBus.h>
#include <Action/AICoreActionRequestHandler.h>
#include <AzCore/Math/Uuid.h>
#include <AzCore/std/string/string_view.h>
#include <AzToolsFramework/API/ToolsApplicationAPI.h>
#include <AzToolsFramework/Entity/EditorEntityContextBus.h>
#include <Clients/AICoreSystemComponent.h>

#include "UI/AICoreWidget.h"

namespace AICore
{
    /// System component for AICore editor
    class AICoreEditorSystemComponent
        : public AICoreSystemComponent
        , protected AzToolsFramework::EditorEvents::Bus::Handler
        , private AzToolsFramework::EditorEntityContextNotificationBus::Handler
        , protected AICoreEditorRequestBus::Handler
    {
        using BaseSystemComponent = AICoreSystemComponent;

    public:
        AZ_COMPONENT_DECL(AICoreEditorSystemComponent);

        static void Reflect(AZ::ReflectContext* context);

        AICoreEditorSystemComponent();
        ~AICoreEditorSystemComponent();

    private:
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        // AICoreRequestBus
        AZStd::unique_ptr<AICoreScriptExecutor> MakeScriptExecutor(const AIContext& aiContext) override;

        // EditorEntityContextNotificationBus overrides
        void OnStartPlayInEditorBegin() override;
        void OnStopPlayInEditor() override;
        // AICoreEditorRequestBus::Handler overrides ...
        void SaveSystemConfiguration() override;

        // AzToolsFramework::EditorEvents::Bus::Handler overrides ...
        void NotifyRegisterViews() override;

        // AZ::Component
        void Init() override;
        void Activate() override;
        void Deactivate() override;

        AICoreActionRequestHandler m_actionRequestHandler;
        AICoreEditorSettingsRegistryManager m_settingsRegistryManager;
    };
} // namespace AICore

