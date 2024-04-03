/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <Clients/GenAIOllamaSystemComponent.h>

namespace GenAIOllama
{
    /// System component for GenAIOllama editor
    class GenAIOllamaEditorSystemComponent : public GenAIOllamaSystemComponent
    {
        using BaseSystemComponent = GenAIOllamaSystemComponent;

    public:
        AZ_COMPONENT_DECL(GenAIOllamaEditorSystemComponent);

        static void Reflect(AZ::ReflectContext* context);

        GenAIOllamaEditorSystemComponent() = default;
        ~GenAIOllamaEditorSystemComponent() = default;

    private:
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        // AZ::Component
        void Activate() override;
        void Deactivate() override;
    };
} // namespace GenAIOllama