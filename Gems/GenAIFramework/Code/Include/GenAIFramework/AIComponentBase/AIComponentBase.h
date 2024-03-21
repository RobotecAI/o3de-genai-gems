/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AzCore/Component/Component.h>
#include <AzCore/Component/EntityId.h>
#include <AzCore/std/containers/vector.h>
#include <AzCore/std/string/string.h>
#include <AzToolsFramework/ToolsComponents/EditorComponentBase.h>

namespace GenAIFramework
{
    class AIComponentBase : public AzToolsFramework::Components::EditorComponentBase
    {
    public:
        AZ_COMPONENT(AIComponentBase, "{43956388-0819-4a78-9307-1e485220ee9c}", AzToolsFramework::Components::EditorComponentBase);

        AIComponentBase() = default;
        ~AIComponentBase() = default;

        static void Reflect(AZ::ReflectContext* context);

        AZStd::vector<AZStd::string> GetServiceRequestorsNames() const;
        AZStd::vector<AZStd::string> GetModelConfigurationsNames() const;

        void Activate() override;
        void Deactivate() override;

    protected:
        AZStd::string m_selectedServiceRequestorName = "";
        AZStd::string m_selectedModelConfigurationName = "";

        AZ::EntityId m_selectedServiceRequestorId;
        AZ::EntityId m_selectedModelConfigurationId;

        void UpdateNamedServiceRequestorId();
        void UpdateNamedModelConfigurationId();

    private:
        AZStd::vector<AZStd::string> GetNames(AZStd::vector<AZ::Component*> components) const;
        AZStd::vector<AZ::Component*> GetServiceRequestors() const;
        AZStd::vector<AZ::Component*> GetModelConfigurations() const;

        void GetNamedId(const AZStd::string& name, AZStd::vector<AZ::Component*> components, AZ::EntityId& id);
    };
} // namespace GenAIFramework