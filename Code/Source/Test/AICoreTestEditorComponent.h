/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include "AICoreTest.h"
#include "CustomEditorRequests.h"
#include <AzCore/Component/Component.h>
#include <AzToolsFramework/ToolsComponents/EditorComponentBase.h>

namespace AICore
{
    class AICoreTestEditorComponent : public AzToolsFramework::Components::EditorComponentBase
    {
    public:
        AZ_COMPONENT(AICoreTestEditorComponent, "{38622FF0-E4FE-4F8D-A7F8-E889584FA474}", AzToolsFramework::Components::EditorComponentBase);
        AICoreTestEditorComponent() = default;

        // AzToolsFramework::Components::EditorComponentBase overrides
        void BuildGameEntity(AZ::Entity* gameEntity) override;

        static void Reflect(AZ::ReflectContext* context);
        void Activate() override;
        void Deactivate() override;

    private:
        void ListClasses();
        void ListMethods();
        void ListEbuses();
        void APITest();
        void CommandTest();

        CustomEditorTest m_customEditorTest;
        AICoreActionTest m_aiCoreActionTest;
        AZStd::string m_className{ "" };
        AZStd::string m_methodName{ "" };
        AZStd::string m_commandText{ "" };
    };
} // namespace AICore
