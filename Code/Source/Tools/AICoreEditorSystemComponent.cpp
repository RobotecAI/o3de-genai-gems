/*
* Copyright (c) Contributors to the Open 3D Engine Project.
* For complete copyright and license terms please see the LICENSE at the root of this distribution.
*
* SPDX-License-Identifier: Apache-2.0 OR MIT
*
*/

#include "AICoreEditorSystemComponent.h"

#include "AzCore/std/smart_ptr/make_shared.h"
// #include "Communication/JSONHttp/BasicJSONRequester.h"
// #include "UI/AICoreWidget.h"
#include <API/ViewPaneOptions.h>
#include <AzCore/Serialization/SerializeContext.h>

#include <AICore/AICoreTypeIds.h>

// #include "AICoreSettingsRegistryManager/AICoreSettingsRegistryManager.h"

namespace AICore
{
    AZ_COMPONENT_IMPL(AICoreEditorSystemComponent, "AICoreEditorSystemComponent",
        AICoreEditorSystemComponentTypeId, BaseSystemComponent);

    void AICoreEditorSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<AICoreEditorSystemComponent, AICoreSystemComponent>()
                ->Version(0);
        }
    }

    AICoreEditorSystemComponent::AICoreEditorSystemComponent() = default;

    AICoreEditorSystemComponent::~AICoreEditorSystemComponent() = default;

    void AICoreEditorSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        BaseSystemComponent::GetProvidedServices(provided);
        provided.push_back(AZ_CRC_CE("AICoreEditorService"));
    }

    void AICoreEditorSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        BaseSystemComponent::GetIncompatibleServices(incompatible);
        incompatible.push_back(AZ_CRC_CE("AICoreEditorService"));
    }

    void AICoreEditorSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        BaseSystemComponent::GetRequiredServices(required);
    }

    void AICoreEditorSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
        BaseSystemComponent::GetDependentServices(dependent);
    }

    void AICoreEditorSystemComponent::Activate()
    {
        AICoreSystemComponent::Activate();
        AzToolsFramework::EditorEvents::Bus::Handler::BusConnect();

        // m_requesters.push_back(AZStd::make_shared<BasicJSONRequester>());
        // m_requesters.push_back(AZStd::make_shared<BasicJSONRequester>());

        // m_setregManager.SaveSystemConfiguration<AZStd::vector<AZStd::shared_ptr<RequesterBase>>>(
        //     m_requesters,
        //     [](const AZStd::vector<AZStd::shared_ptr<RequesterBase>>& obj1, AICoreSettingsRegistryManager::Result result)
        //     {
        //         if (result == AICoreSettingsRegistryManager::Result::Success)
        //         {
        //             std::cout << "Good" << std::endl;
        //         }
        //         else
        //         {
        //             std::cout << "bad" << std::endl;
        //         }
        //     });
    }

    void AICoreEditorSystemComponent::Deactivate()
    {
        AzToolsFramework::EditorEvents::Bus::Handler::BusDisconnect();
        AICoreSystemComponent::Deactivate();
    }

    void AICoreEditorSystemComponent::NotifyRegisterViews()
    {
        // AzToolsFramework::ViewPaneOptions options;
        // options.paneRect = QRect(100, 100, 500, 400);
        // options.showOnToolsToolbar = true;
        // options.isDeletable = false;

        // AzToolsFramework::RegisterViewPane<AICoreWidget>("AICore", "AICore", options);
    }

} // namespace AICore
