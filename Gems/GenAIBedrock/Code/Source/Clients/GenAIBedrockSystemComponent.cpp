/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "GenAIBedrockSystemComponent.h"

#include <GenAIBedrock/GenAIBedrockTypeIds.h>

#include <AzCore/Serialization/SerializeContext.h>

namespace GenAIBedrock
{
    AZ_COMPONENT_IMPL(GenAIBedrockSystemComponent, "GenAIBedrockSystemComponent", GenAIBedrockSystemComponentTypeId);

    void GenAIBedrockSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<GenAIBedrockSystemComponent, AZ::Component>()->Version(0);
        }
    }

    void GenAIBedrockSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("GenAIBedrockService"));
    }

    void GenAIBedrockSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("GenAIBedrockService"));
    }

    void GenAIBedrockSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        required.push_back(AZ_CRC_CE("AWSCoreService"));
    }

    void GenAIBedrockSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
    }

} // namespace GenAIBedrock
