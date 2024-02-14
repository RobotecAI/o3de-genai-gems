/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AICore/AICoreActionBus.h>
#include <AzToolsFramework/API/ToolsApplicationAPI.h>
#include <AzToolsFramework/Entity/EditorEntityContextBus.h>
#include <Clients/AICoreSystemComponent.h>

namespace AICore
{
    /// System component for AICore editor
    class AICoreEditorSystemComponent
        : public AICoreSystemComponent
        , protected AzToolsFramework::EditorEvents::Bus::Handler
        , private AzToolsFramework::EditorEntityContextNotificationBus::Handler
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

        // EditorEntityContextNotificationBus overrides
        void OnStartPlayInEditorBegin() override;
        void OnStopPlayInEditor() override;

        // AZ::Component
        void Activate() override;
        void Deactivate() override;
    };
} // namespace AICore
