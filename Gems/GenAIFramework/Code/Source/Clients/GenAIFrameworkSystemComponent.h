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

#include <AzCore/Component/Component.h>
#include <AzCore/std/smart_ptr/shared_ptr.h>
#include <GenAIFramework/GenAIFrameworkBus.h>
#include <GenAIFramework/SystemRegistrationContext/SystemRegistrationContext.h>

namespace GenAIFramework
{
    class GenAIFrameworkSystemComponent
        : public AZ::Component
        , protected GenAIFrameworkRequestBus::Handler
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
        SystemRegistrationContext* GetSystemRegistrationContext() override;
        AZStd::vector<AZStd::pair<AZStd::string, AZ::Uuid>> GetRegisteredModelConfigurationsNameAndComponentTypeId() override;
        AZStd::vector<AZStd::pair<AZStd::string, AZ::Uuid>> GetRegisteredServiceProvidersNameAndComponentTypeId() override;
        AZStd::vector<AZ::Component*> GetActiveModelConfigurations() override;
        AZStd::vector<AZ::Component*> GetActiveServiceProviders() override;
        AZ::Component* CreateNewModelConfiguration(const AZStd::string& modelConfigurationName, const AZ::Uuid& componentTypeId) override;
        AZ::Component* CreateNewServiceProvider(const AZStd::string& providerName, const AZ::Uuid& componentTypeId) override;
        void RemoveComponent(AZ::Component* component) override;
        void ActivateEntity(AZStd::shared_ptr<AZ::Entity> entity) override;
        void DeactivateEntity(AZStd::shared_ptr<AZ::Entity> entity) override;
        AZStd::vector<AZStd::string> GetActiveModelConfigurationsNames() const override;
        AZStd::vector<AZStd::string> GetActiveServiceProvidersNames() const override;
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
            const AZStd::set<AZ::Uuid>& registeredComponent);
        AZStd::vector<AZ::Component*> GetActiveComponents(const EntityIdToEntityMap& entities) const;
        AZ::Component* CreateNewComponentEntity(const AZStd::string& name, const AZ::Uuid& componentTypeId, EntityIdToEntityMap& entities);

        AZStd::vector<AZStd::string> GetActiveComponentsNames(const EntityIdToEntityMap& entities) const;
    };
} // namespace GenAIFramework
