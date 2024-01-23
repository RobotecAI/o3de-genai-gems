/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include "AICoreSettingsRegistryManager/AICoreSettingsRegistryManager.h"
#include "Clients/AICoreSystemComponentConfiguration.h"

#include <AICore/AICoreBus.h>
#include <AICore/AICoreScriptExecutor.h>
#include <Action/AICoreActionRequestHandler.h>
#include <AzCore/std/smart_ptr/shared_ptr.h>
#include <AICore/AICoreBus.h>
#include <AICore/SystemRegistrationContext/AICoreSystemRegistrationContext.h>
#include <AzCore/Component/Component.h>
#include <AzCore/Component/TickBus.h>

namespace AICore
{
    class AICoreSystemComponent
        : public AZ::Component
        , protected AICoreRequestBus::Handler
        , public AZ::TickBus::Handler
    {
    public:
        AZ_COMPONENT_DECL(AICoreSystemComponent);

        static void Reflect(AZ::ReflectContext* context);

        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        AICoreSystemComponent();
        ~AICoreSystemComponent();

    protected:
        ////////////////////////////////////////////////////////////////////////
        // AICoreRequestBus interface implementation
        virtual AZStd::unique_ptr<AICoreScriptExecutor> MakeScriptExecutor(const AIContext& aiContext) override;
        AICoreSystemRegistrationContext* GetSystemRegistrationContext() override;
        AZStd::vector<AZStd::pair<AZStd::string, AZ::Uuid>> GetRegisteredGeneratorsNameAndComponentTypeId() override;
        AZStd::vector<AZStd::pair<AZStd::string, AZ::Uuid>> GetRegisteredRequestersNameAndComponentTypeId() override;
        AZStd::vector<AZ::Component*> GetActiveGenerators() override;
        AZStd::vector<AZ::Component*> GetActiveRequesters() override;
        AZ::Component* CreateNewGenerator(const AZStd::string& generatorName, const AZ::Uuid& componentTypeId) override;
        AZ::Component* CreateNewRequester(const AZStd::string& requesterName, const AZ::Uuid& componentTypeId) override;
        void RemoveComponent(AZ::Component* component) override;
        void ActivateEntity(AZ::Entity* entity) override;
        void DeactivateEntity(AZ::Entity* entity) override;
        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        // AZ::Component interface implementation
        void Init() override;
        void Activate() override;
        void Deactivate() override;
        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        // AZTickBus interface implementation
        void OnTick(float deltaTime, AZ::ScriptTimePoint time) override;
        ////////////////////////////////////////////////////////////////////////

        AICoreSystemComponentConfiguration m_configuration;
        AICoreSettingsRegistryManager m_settingsRegistryManager;

    private:
        void InitEntities(const AZStd::vector<AZ::Entity*>& entities);
        void ActivateEntities(const AZStd::vector<AZ::Entity*>& entities);
        void DeactivateEntities(const AZStd::vector<AZ::Entity*>& entities);
        AICoreActionRequestHandler m_actionRequestHandler;

        AZStd::vector<AZStd::pair<AZStd::string, AZ::Uuid>> GetRegisteredComponentsNameAndComponentTypeId(
            AZStd::vector<AZ::Uuid> componentTypeIds);
        AZStd::vector<AZ::Component*> GetActiveComponents(AZStd::vector<AZ::Entity*> entities);
        AZ::Component* CreateNewComponentEntity(
            const AZStd::string& name, const AZ::Uuid& componentTypeId, AZStd::vector<AZ::Entity*>& entities);
        ;
    };
} // namespace AICore
