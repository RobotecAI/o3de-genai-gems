/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AzToolsFramework/API/ToolsApplicationAPI.h>

#include <Clients/GenAIBedrockSystemComponent.h>

namespace GenAIBedrock
{
    /// System component for GenAIBedrock editor
    class GenAIBedrockEditorSystemComponent
        : public GenAIBedrockSystemComponent
        , protected AzToolsFramework::EditorEvents::Bus::Handler
    {
        using BaseSystemComponent = GenAIBedrockSystemComponent;

    public:
        AZ_COMPONENT_DECL(GenAIBedrockEditorSystemComponent);

        static void Reflect(AZ::ReflectContext* context);

        GenAIBedrockEditorSystemComponent();
        ~GenAIBedrockEditorSystemComponent();

    private:
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        // AZ::Component
        void Activate() override;
        void Deactivate() override;
    };
} // namespace GenAIBedrock
