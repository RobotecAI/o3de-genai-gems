/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AzCore/Component/Component.h>
#include <AzCore/Component/Entity.h>
#include <AzCore/Math/Uuid.h>
#include <AzCore/std/containers/unordered_map.h>
#include <AzCore/std/parallel/mutex.h>
#include <AzCore/std/string/string.h>
#include <GenAIFramework/Communication/AsyncRequestBus.h>

namespace GenAIFramework
{
    class GenAIAsyncRequestSystemComponent
        : public AZ::Component
        , private AsyncRequestBus::Handler
    {
    public:
        AZ_COMPONENT_DECL(GenAIAsyncRequestSystemComponent);

        static void Reflect(AZ::ReflectContext* context);
        GenAIAsyncRequestSystemComponent();
        virtual ~GenAIAsyncRequestSystemComponent();

    protected:
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);

        // AZ::Component overrides
        void Activate() override;
        void Deactivate() override;

    private:
        // AysncRequestBus::Handler
        void SetRequestorAndModel(AZ::EntityId selectedModelConfigurationId, AZ::EntityId selectedRequestorId) override;
        bool SetServiceRequesterByName(const AZStd::string& requestorName) override;
        bool SetModelConfigurationByName(const AZStd::string& modelConfigurationName) override;
        AZ::Uuid SendPromptToLLM(const AZStd::string& prompt) override;
        bool IsResponseReady(AZ::Uuid promptId) override;
        AZStd::string GetResponse(AZ::Uuid promptId) override;
        void ResetModelHistory() override;
        void EnableModelHistory(bool enableHistory) override;

        bool SetEntityIdByName(const AZStd::vector<AZ::Component*>& components, const AZStd::string& entityName, AZ::EntityId& entityId);

        AZ::EntityId m_selectedModelConfigurationId = AZ::EntityId(); //! Selected model configuration entity id
        AZ::EntityId m_selectedRequestorId = AZ::EntityId(); //! Selected requestor entity id
        AZStd::mutex m_promptMutex; //! Mutex to protect the prompt responses
        AZStd::unordered_map<AZ::Uuid, AZStd::string> m_promptResponses; //! Map of ready to be read responses
    };
} // namespace GenAIFramework
