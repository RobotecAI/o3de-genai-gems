/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <Clients/GenAIAsyncRequestSystemComponent.h>

#include <AzToolsFramework/API/ToolsApplicationAPI.h>
#include <AzToolsFramework/Entity/EditorEntityContextBus.h>

namespace GenAIFramework
{
    class GenAIAsyncRequestEditorSystemComponent
        : public GenAIAsyncRequestSystemComponent
        , private AzToolsFramework::EditorEntityContextNotificationBus::Handler
    {
        using BaseSystemComponent = GenAIAsyncRequestSystemComponent;

    public:
        AZ_COMPONENT_DECL(GenAIAsyncRequestEditorSystemComponent);

        static void Reflect(AZ::ReflectContext* context);

        GenAIAsyncRequestEditorSystemComponent() = default;
        ~GenAIAsyncRequestEditorSystemComponent() = default;

    private:
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);

        // EditorEntityContextNotificationBus overrides
        void OnStartPlayInEditorBegin() override;
        void OnStopPlayInEditor() override;

        // AZ::Component
        void Init() override;
        void Activate() override;
        void Deactivate() override;
    };
} // namespace GenAIFramework
