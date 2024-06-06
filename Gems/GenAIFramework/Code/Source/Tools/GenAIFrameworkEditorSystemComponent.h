/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include "SettingsRegistryManager/Editor/EditorSettingsRegistryManager.h"
#include <AzCore/Math/Uuid.h>
#include <AzCore/std/string/string_view.h>
#include <AzToolsFramework/API/ToolsApplicationAPI.h>
#include <AzToolsFramework/Entity/EditorEntityContextBus.h>
#include <Clients/GenAIFrameworkSystemComponent.h>
#include <GenAIFramework/GenAIFrameworkBus.h>
#include <GenAIFramework/GenAIFrameworkEditorBus.h>

namespace GenAIFramework
{
    /// System component for GenAIFramework editor
    class GenAIFrameworkEditorSystemComponent
        : public GenAIFrameworkSystemComponent
        , protected AzToolsFramework::EditorEvents::Bus::Handler
        , private AzToolsFramework::EditorEntityContextNotificationBus::Handler
        , protected GenAIFrameworkEditorRequestBus::Handler
        , protected GenAIFrameworkNotificationBus::Handler
    {
        using BaseSystemComponent = GenAIFrameworkSystemComponent;

    public:
        AZ_COMPONENT_DECL(GenAIFrameworkEditorSystemComponent);

        static void Reflect(AZ::ReflectContext* context);

        GenAIFrameworkEditorSystemComponent();
        ~GenAIFrameworkEditorSystemComponent();

    private:
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        // EditorEntityContextNotificationBus overrides
        void OnStartPlayInEditorBegin() override;
        void OnStopPlayInEditor() override;
        // GenAIFrameworkEditorRequestBus::Handler overrides ...
        void SaveSystemConfiguration() override;

        // AzToolsFramework::EditorEvents::Bus::Handler overrides ...
        void NotifyRegisterViews() override;

        // AZ::Component
        void Init() override;
        void Activate() override;
        void Deactivate() override;

        EditorSettingsRegistryManager m_settingsRegistryManager;

        ////////////////////////////////////////////////////////////////////////
        // GenAIFrameworkNotificationBus overrides
        void OnServiceProviderAdded(const AZ::EntityId& serviceProviderId) override;
        void OnServiceProviderRemoved(const AZ::EntityId& serviceProviderId) override;
        void OnModelConfigurationAdded(const AZ::EntityId& modelConfigurationId) override;
        void OnModelConfigurationRemoved(const AZ::EntityId& modelConfigurationId) override;
    };
} // namespace GenAIFramework
