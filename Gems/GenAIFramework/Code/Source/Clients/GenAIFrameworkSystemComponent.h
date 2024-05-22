/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include "Clients/GenAIFrameworkSystemComponentConfiguration.h"
#include "SettingsRegistryManager/SettingsRegistryManager.h"

#include <Atom/Feature/Utils/FrameCaptureBus.h>
#include <Atom/RPI.Public/Base.h>
#include <Atom/RPI.Public/ViewportContext.h>
#include <Atom/RPI.Public/ViewportContextBus.h>
#include <AzCore/Component/Component.h>
#include <AzCore/std/smart_ptr/shared_ptr.h>
#include <GenAIFramework/GenAIFrameworkBus.h>
#include <GenAIFramework/SystemRegistrationContext/SystemRegistrationContext.h>

namespace GenAIFramework
{
    class GenAIFrameworkSystemComponent
        : public AZ::Component
        , protected GenAIFrameworkRequestBus::Handler
        , private AZ::RPI::ViewportContextManagerNotificationsBus::Handler
    {
    public:
        AZ_COMPONENT_DECL(GenAIFrameworkSystemComponent);

        static void Reflect(AZ::ReflectContext* context);

        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        GenAIFrameworkSystemComponent();
        ~GenAIFrameworkSystemComponent();

    protected:
        ////////////////////////////////////////////////////////////////////////
        // GenAIFrameworkRequestBus interface implementation
        SystemRegistrationContext* GetSystemRegistrationContext() const override;
        AZStd::vector<AZStd::pair<AZStd::string, AZ::Uuid>> GetModelConfigurationNamesAndComponentTypeIds() const override;
        AZStd::vector<AZStd::pair<AZStd::string, AZ::Uuid>> GetServiceProviderNamesAndComponentTypeIds() const override;
        AZStd::vector<AZ::Component*> GetModelConfigurations() const override;
        AZStd::vector<AZ::Component*> GetServiceProviders() const override;
        AZStd::vector<AZStd::string> GetModelConfigurationNames() const override;
        AZStd::vector<AZStd::string> GetServiceProviderNames() const override;
        AZ::Component* CreateModelConfiguration(const AZStd::string& modelConfigurationName, const AZ::Uuid& componentTypeId) override;
        AZ::Component* CreateServiceProvider(const AZStd::string& providerName, const AZ::Uuid& componentTypeId) override;
        void RemoveComponent(AZ::Component* component) override;
        void ActivateEntity(AZStd::shared_ptr<AZ::Entity> entity) override;
        void DeactivateEntity(AZStd::shared_ptr<AZ::Entity> entity) override;
        AZ::Render::FrameCaptureOutcome GetViewportBase64Image(AZStd::function<void(AZStd::string)> imageReadyCallback) const override;
        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        // AZ::Component interface implementation
        void Init() override;
        void Activate() override;
        void Deactivate() override;
        ////////////////////////////////////////////////////////////////////////

        GenAIFrameworkSystemComponentConfiguration m_configuration;
        SettingsRegistryManager m_settingsRegistryManager;

    private:
        static void InitEntities(const EntityIdToEntityMap& entities);
        static void ActivateEntities(const EntityIdToEntityMap& entities);
        static void DeactivateEntities(const EntityIdToEntityMap& entities);

        AZStd::vector<AZStd::pair<AZStd::string, AZ::Uuid>> GetRegisteredComponentsNameAndComponentTypeId(
            const AZStd::set<AZ::Uuid>& registeredComponent) const;
        AZStd::vector<AZ::Component*> GetActiveComponents(const EntityIdToEntityMap& entities) const;
        AZ::Component* CreateNewComponentEntity(const AZStd::string& name, const AZ::Uuid& componentTypeId, EntityIdToEntityMap& entities);

        AZStd::vector<AZStd::string> GetRegisteredComponentNames(const EntityIdToEntityMap& entities) const;
        ////////////////////////////////////////////////////////////////////////
        // AZ::RPI::ViewportContextManagerNotificationsBus interface implementation
        void OnViewportContextAdded(AZ::RPI::ViewportContextPtr viewportContext) override;
        ////////////////////////////////////////////////////////////////////////
        void SetRenderPipeline(AZ::RPI::RenderPipelinePtr pipeline);

        AZ::RPI::ViewportContext::PipelineChangedEvent::Handler m_pipelineChangedHandler;
        AZStd::vector<AZStd::string> m_passHierarchy;
    };
} // namespace GenAIFramework
