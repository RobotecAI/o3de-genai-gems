/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "GenAIOllamaSystemComponent.h"

#include <GenAIOllama/GenAIOllamaTypeIds.h>

#include <AzCore/Serialization/SerializeContext.h>

namespace GenAIOllama
{
    AZ_COMPONENT_IMPL(GenAIOllamaSystemComponent, "GenAIOllamaSystemComponent", GenAIOllamaSystemComponentTypeId);

    void GenAIOllamaSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<GenAIOllamaSystemComponent, AZ::Component>()->Version(0);
        }
    }

    void GenAIOllamaSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("GenAIOllamaService"));
    }

    void GenAIOllamaSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("GenAIOllamaService"));
    }

    void GenAIOllamaSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
    }

    void GenAIOllamaSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
    }

    void GenAIOllamaSystemComponent::Init()
    {
    }

    void GenAIOllamaSystemComponent::Activate()
    {
    }

    void GenAIOllamaSystemComponent::Deactivate()
    {
    }

} // namespace GenAIOllama
