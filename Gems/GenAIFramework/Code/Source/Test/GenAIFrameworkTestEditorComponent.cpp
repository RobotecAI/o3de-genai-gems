/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "GenAIFrameworkTestEditorComponent.h"
#include "GenAIFrameworkTestComponent.h"
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/EditContextConstants.inl>
#include <AzCore/Serialization/SerializeContext.h>
#include <BehaviorContextUtils/BehaviorContextDump.h>
#include <sstream>

namespace GenAIFramework
{
    namespace Internal
    {
        void LinePrint(const AZStd::string& input)
        {
            std::istringstream stream(input.c_str());
            std::string line;
            while (std::getline(stream, line))
            {
                AZ_Printf("", "%s", line.c_str());
            }
        }
    } // namespace Internal

    void GenAIFrameworkTestEditorComponent::BuildGameEntity(AZ::Entity* gameEntity)
    {
        gameEntity->CreateComponent<GenAIFramework::GenAIFrameworkTestComponent>(m_genAIFrameworkActionTest);
    }

    void GenAIFrameworkTestEditorComponent::Reflect(AZ::ReflectContext* context)
    {
        CustomEditorTest::Reflect(context);
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<GenAIFrameworkTestEditorComponent, AzToolsFramework::Components::EditorComponentBase>()
                ->Version(1)
                ->Field("ClassName", &GenAIFrameworkTestEditorComponent::m_className)
                ->Field("MethodName", &GenAIFrameworkTestEditorComponent::m_methodName)
                ->Field("CommandText", &GenAIFrameworkTestEditorComponent::m_commandText);

            if (AZ::EditContext* ec = serializeContext->GetEditContext())
            {
                ec->Class<GenAIFrameworkTestEditorComponent>("Test AI API Component", "Simple test of AI")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("Game"))
                    ->Attribute(AZ::Edit::Attributes::Category, "AI")
                    ->Attribute(AZ::Edit::Attributes::Icon, "Editor/Icons/Components/AITest.svg")
                    ->Attribute(AZ::Edit::Attributes::ViewportIcon, "Editor/Icons/Components/Viewport/AITest.svg")
                    ->DataElement(AZ::Edit::UIHandlers::Default, &GenAIFrameworkTestEditorComponent::m_className, "", "Class name")
                    ->UIElement(AZ::Edit::UIHandlers::Button, "", "List classes matching filter")
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, &GenAIFrameworkTestEditorComponent::ListClasses)
                    ->Attribute(AZ::Edit::Attributes::ButtonText, "List Classes")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default, &GenAIFrameworkTestEditorComponent::m_methodName, "", "Method or ebus name")
                    ->UIElement(AZ::Edit::UIHandlers::Button, "", "Print behavior methods matching filter")
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, &GenAIFrameworkTestEditorComponent::ListMethods)
                    ->Attribute(AZ::Edit::Attributes::ButtonText, "List Methods")
                    ->UIElement(AZ::Edit::UIHandlers::Button, "", "Print all ebuses matching filter")
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, &GenAIFrameworkTestEditorComponent::ListEbuses)
                    ->Attribute(AZ::Edit::Attributes::ButtonText, "List Ebuses")
                    ->DataElement(
                        AZ::Edit::UIHandlers::MultiLineEdit, &GenAIFrameworkTestEditorComponent::m_commandText, "", "Command text")
                    ->UIElement(AZ::Edit::UIHandlers::Button, "", "Trigger command test")
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, &GenAIFrameworkTestEditorComponent::CommandTest)
                    ->Attribute(AZ::Edit::Attributes::ButtonText, "Command Test");
            }
        }
    }

    void GenAIFrameworkTestEditorComponent::Activate()
    {
        AZ_Printf("GenAIFrameworkTestEditorComponent", "Activate\n");
        AzToolsFramework::Components::EditorComponentBase::Activate();
        if (CustomEditorInterface::Get() == nullptr)
        {
            CustomEditorInterface::Register(&m_customEditorTest);
        }
        m_customEditorTest.Connect();
    }

    void GenAIFrameworkTestEditorComponent::Deactivate()
    {
        AZ_Printf("GenAIFrameworkTestEditorComponent", "Deactivate\n");
        m_customEditorTest.Disconnect();
        if (CustomEditorInterface::Get() == &m_customEditorTest)
        {
            CustomEditorInterface::Unregister(&m_customEditorTest);
        }
        AzToolsFramework::Components::EditorComponentBase::Deactivate();
    }

    void GenAIFrameworkTestEditorComponent::ListClasses()
    {
        BehaviorContextDump b(m_genAIFrameworkActionTest.GetAIContext(), m_className);
        Internal::LinePrint(b.DumpClasses());
    }

    void GenAIFrameworkTestEditorComponent::ListMethods()
    {
        BehaviorContextDump b(m_genAIFrameworkActionTest.GetAIContext(), m_methodName);
        Internal::LinePrint(b.DumpMethods(m_className));
    }

    void GenAIFrameworkTestEditorComponent::ListEbuses()
    {
        BehaviorContextDump b(m_genAIFrameworkActionTest.GetAIContext(), m_methodName);
        Internal::LinePrint(b.DumpEbusses());
    }

    void GenAIFrameworkTestEditorComponent::CommandTest()
    {
        AZ_Printf("GenAIFrameworkTestEditorComponent", "CommandTest\n");
        AZStd::string response;
        bool success = m_genAIFrameworkActionTest.ScriptCall(m_commandText, response);
        AZ_Printf("GenAIFrameworkTestEditorComponent", "[%s]: %s\n", success ? "succeeded" : "failed", response.c_str());
    }
} // namespace GenAIFramework
