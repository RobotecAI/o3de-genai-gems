/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AIAgent/AIAgent.h>
#include <Clients/GenAIFrameworkSystemComponentConfiguration.h>
#include <GenAIFramework/GenAIFrameworkBus.h>
#include <GenAIFramework/SystemRegistrationContext/SystemRegistrationContext.h>
#include <GenAIFramework/UI/UIConversationBus.h>
#include <SettingsRegistryManager/SettingsRegistryManager.h>

#include <AzCore/Component/Component.h>
#include <AzCore/Component/Entity.h>
#include <AzCore/Component/EntityId.h>
#include <AzCore/base.h>
#include <AzCore/std/smart_ptr/shared_ptr.h>

namespace GenAIFramework
{
    class GenAIFrameworkSystemComponent
        : public AZ::Component
        , protected GenAIFrameworkRequestBus::Handler
        , protected UIConversationsBus::Handler
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
        AZ::Component* CreateModelConfiguration(const AZStd::string& modelConfigurationName, AZ::Uuid componentTypeId) override;
        AZ::Component* CreateServiceProvider(const AZStd::string& providerName, AZ::Uuid componentTypeId) override;
        void RemoveComponent(AZ::Component* component) override;
        void ActivateEntity(AZStd::shared_ptr<AZ::Entity> entity) override;
        void DeactivateEntity(AZStd::shared_ptr<AZ::Entity> entity) override;
        AZ::Outcome<AZ::u64, void> CreateAIAgent(
            const AZStd::string& serviceProviderName, const AZStd::string& modelModelConfigurationName) override;
        bool RemoveAIAgent(AZ::u64 agentId) override;
        AZ::Outcome<AZ::u64, void> CreateNewFeatureConversation(
            const AZStd::string& serviceProviderName,
            const AZStd::string& modelModelConfigurationName,
            const AZStd::string& featureName) override;
        bool RemoveFeatureConversation(AZ::u64 featureConversationId) override;
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
            const AZStd::unordered_set<AZ::Uuid>& registeredComponent) const;
        AZStd::vector<AZ::Component*> GetActiveComponents(const EntityIdToEntityMap& entities) const;
        AZ::Component* CreateNewComponentEntity(const AZStd::string& name, AZ::Uuid componentTypeId, EntityIdToEntityMap& entities);

        AZStd::vector<AZStd::string> GetRegisteredComponentNames(const EntityIdToEntityMap& entities) const;

        AZ::EntityId GetEntityIdByName(const AZStd::string& name, const EntityIdToEntityMap& entities) const;

        void OnNewChatWidgetCreated(
            const AZStd::string& chatName,
            const AZStd::string& modelName,
            const AZStd::string& providerName,
            const AZStd::string& featureName) override;

        void OnChatWidgetClosed(const AZStd::string& chatName) override;

        FeaturesConversationsMap GetStoredChats() override;

        AZStd::unordered_map<AZ::u64, AZStd::shared_ptr<AIAgent>> m_agents;
        AZStd::unordered_map<AZ::u64, AZStd::shared_ptr<FeatureBase>> m_featureConversations;
    };
} // namespace GenAIFramework
