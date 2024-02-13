/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

// #include "AICoreSettingsRegistryManager/AICoreSettingsRegistryManager.h"
#include <AzToolsFramework/API/ToolsApplicationAPI.h>
#include <Clients/AICoreSystemComponent.h>

// #include <aws/core/utils/json/JsonSerializer.h>

namespace AICore
{
    /// System component for AICore editor
    class AICoreEditorSystemComponent
        : public AICoreSystemComponent
        , protected AzToolsFramework::EditorEvents::Bus::Handler
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

        // AzToolsFramework::EditorEvents::Bus::Handler overrides ...
        void NotifyRegisterViews() override;

        // AZ::Component
        void Activate() override;
        void Deactivate() override;

        // AZStd::vector<AZStd::shared_ptr<RequesterBase>> m_requesters;
        // AICoreSettingsRegistryManager m_setregManager;
    };
} // namespace AICore

