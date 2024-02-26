/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "AICoreTestEditorComponent.h"
#include "AICoreTestComponent.h"
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/EditContextConstants.inl>
#include <AzCore/Serialization/SerializeContext.h>
#include <BehaviorContextUtils/BehaviorContextDump.h>

namespace AICore
{
    void AICoreTestEditorComponent::BuildGameEntity(AZ::Entity* gameEntity)
    {
        gameEntity->CreateComponent<AICore::AICoreTestComponent>(m_aiCoreActionTest);
    }

    void AICoreTestEditorComponent::Reflect(AZ::ReflectContext* context)
    {
        CustomEditorTest::Reflect(context);
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<AICoreTestEditorComponent, AzToolsFramework::Components::EditorComponentBase>()
                ->Version(1)
                ->Field("ClassName", &AICoreTestEditorComponent::m_className)
                ->Field("MethodName", &AICoreTestEditorComponent::m_methodName)
                ->Field("CommandText", &AICoreTestEditorComponent::m_commandText);

            if (AZ::EditContext* ec = serializeContext->GetEditContext())
            {
                ec->Class<AICoreTestEditorComponent>("AI Test Editor Component", "Simple test of AI")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("Game"))
                    ->Attribute(AZ::Edit::Attributes::Category, "AI")
                    ->DataElement(AZ::Edit::UIHandlers::Default, &AICoreTestEditorComponent::m_className, "", "Class name")
                    ->UIElement(AZ::Edit::UIHandlers::Button, "", "List classes matching filter")
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, &AICoreTestEditorComponent::ListClasses)
                    ->Attribute(AZ::Edit::Attributes::ButtonText, "List Classes")
                    ->DataElement(AZ::Edit::UIHandlers::Default, &AICoreTestEditorComponent::m_methodName, "", "Method or ebus name")
                    ->UIElement(AZ::Edit::UIHandlers::Button, "", "Print behavior methods matching filter")
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, &AICoreTestEditorComponent::ListMethods)
                    ->Attribute(AZ::Edit::Attributes::ButtonText, "List Methods")
                    ->UIElement(AZ::Edit::UIHandlers::Button, "", "Print all ebuses matching filter")
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, &AICoreTestEditorComponent::ListEbuses)
                    ->Attribute(AZ::Edit::Attributes::ButtonText, "List Ebuses")
                    ->DataElement(AZ::Edit::UIHandlers::MultiLineEdit, &AICoreTestEditorComponent::m_commandText, "", "Command text")
                    ->UIElement(AZ::Edit::UIHandlers::Button, "", "Trigger command test")
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, &AICoreTestEditorComponent::CommandTest)
                    ->Attribute(AZ::Edit::Attributes::ButtonText, "Command Test");
            }
        }
    }

    void AICoreTestEditorComponent::Activate()
    {
        AZ_Printf("AICoreTestEditorComponent", "Activate\n");
        AzToolsFramework::Components::EditorComponentBase::Activate();
        if (CustomEditorInterface::Get() == nullptr)
        {
            CustomEditorInterface::Register(&m_customEditorTest);
        }
        m_customEditorTest.Connect();
    }

    void AICoreTestEditorComponent::Deactivate()
    {
        AZ_Printf("AICoreTestEditorComponent", "Deactivate\n");
        m_customEditorTest.Disconnect();
        if (CustomEditorInterface::Get() == &m_customEditorTest)
        {
            CustomEditorInterface::Unregister(&m_customEditorTest);
        }
        AzToolsFramework::Components::EditorComponentBase::Deactivate();
    }

    void AICoreTestEditorComponent::ListClasses()
    {
        BehaviorContextDump b(true, m_className);
        AZ_Printf("AICoreTestEditorComponent", "\n%s", b.ClassesDump().c_str());
    }

    void AICoreTestEditorComponent::ListMethods()
    {
        BehaviorContextDump b(true, m_methodName);
        AZ_Printf("AICoreTestEditorComponent", "\n%s", b.MethodsDump(m_className).c_str());
    }

    void AICoreTestEditorComponent::ListEbuses()
    {
        BehaviorContextDump b(true, m_methodName);
        AZ_Printf("AICoreTestEditorComponent", "\n%s", b.EbusesDump().c_str());
    }

    void AICoreTestEditorComponent::CommandTest()
    {
        AZ_Printf("AICoreTestEditorComponent", "CommandTest\n");
        AZStd::string response;
        bool success = m_aiCoreActionTest.ScriptCall(m_commandText, response);
        AZ_Printf("AICoreTestEditorComponent", "[%s]: %s\n", success ? "succeeded" : "failed", response.c_str());
    }
} // namespace AICore
