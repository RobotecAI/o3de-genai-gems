/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "GenAIAsyncRequestEditorSystemComponent.h"
#include "SettingsRegistryManager/SettingsRegistryManager.h"

#include <API/ViewPaneOptions.h>
#include <AzCore/Component/Entity.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/std/containers/vector.h>
#include <AzCore/std/smart_ptr/make_shared.h>
#include <GenAIFramework/GenAIFrameworkEditorBus.h>
#include <GenAIFramework/GenAIFrameworkTypeIds.h>

namespace GenAIFramework
{
    AZ_COMPONENT_IMPL(
        GenAIAsyncRequestEditorSystemComponent,
        "GenAIAsyncRequestEditorSystemComponent",
        GenAIAsyncRequestEditorSystemComponentTypeId,
        BaseSystemComponent);

    void GenAIAsyncRequestEditorSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<GenAIAsyncRequestEditorSystemComponent, GenAIAsyncRequestSystemComponent>()->Version(0);
        }
    }

    void GenAIAsyncRequestEditorSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        BaseSystemComponent::GetProvidedServices(provided);
        provided.push_back(AZ_CRC_CE("GenAIAsyncRequestEditorSystemComponentService"));
    }

    void GenAIAsyncRequestEditorSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        BaseSystemComponent::GetIncompatibleServices(incompatible);
        incompatible.push_back(AZ_CRC_CE("GenAIAsyncRequestEditorSystemComponentService"));
    }

    void GenAIAsyncRequestEditorSystemComponent::GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        BaseSystemComponent::GetRequiredServices(required);
    }

    void GenAIAsyncRequestEditorSystemComponent::Init()
    {
        BaseSystemComponent::Init();
    }

    void GenAIAsyncRequestEditorSystemComponent::Activate()
    {
        BaseSystemComponent::Activate();
    }

    void GenAIAsyncRequestEditorSystemComponent::Deactivate()
    {
        BaseSystemComponent::Deactivate();
    }

    void GenAIAsyncRequestEditorSystemComponent::OnStartPlayInEditorBegin()
    {
        BaseSystemComponent::Activate();
    }
    void GenAIAsyncRequestEditorSystemComponent::OnStopPlayInEditor()
    {
        BaseSystemComponent::Deactivate();
    }
} // namespace GenAIFramework
