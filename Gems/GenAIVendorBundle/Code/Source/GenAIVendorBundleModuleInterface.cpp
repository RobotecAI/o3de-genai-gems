/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "GenAIVendorBundleModuleInterface.h"
#include "Models/Claude/ClaudeModelMessagesAPI.h"
#include "Models/Claude/ClaudeModelTextCompletions.h"
#include "Models/Ollama/OllamaModelComponent.h"
#include "Providers/Claude/ClaudeHttpProvider.h"
#include "Providers/Ollama/OllamaHttpProvider.h"
#include <AzCore/Memory/Memory.h>

#include <GenAIVendorBundle/GenAIVendorBundleTypeIds.h>

namespace GenAIVendorBundle
{
    AZ_TYPE_INFO_WITH_NAME_IMPL(
        GenAIVendorBundleModuleInterface, "GenAIVendorBundleModuleInterface", GenAIVendorBundleModuleInterfaceTypeId);
    AZ_RTTI_NO_TYPE_INFO_IMPL(GenAIVendorBundleModuleInterface, AZ::Module);
    AZ_CLASS_ALLOCATOR_IMPL(GenAIVendorBundleModuleInterface, AZ::SystemAllocator);

    GenAIVendorBundleModuleInterface::GenAIVendorBundleModuleInterface()
    {
        // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
        // Add ALL components descriptors associated with this gem to m_descriptors.
        // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and EditContext.
        // This happens through the [MyComponent]::Reflect() function.
        m_descriptors.insert(
            m_descriptors.end(),
            {
                GenAIClaude::ClaudeModelTextCompletions::CreateDescriptor(),
                GenAIClaude::ClaudeModelMessagesAPI::CreateDescriptor(),
                GenAIClaude::ClaudeHttpProvider::CreateDescriptor(),
                GenAIOllama::OllamaHttpServiceComponent::CreateDescriptor(),
                GenAIOllama::OllamaModelComponent::CreateDescriptor(),
            });
    }

    AZ::ComponentTypeList GenAIVendorBundleModuleInterface::GetRequiredSystemComponents() const
    {
        return AZ::ComponentTypeList{};
    }
} // namespace GenAIVendorBundle
