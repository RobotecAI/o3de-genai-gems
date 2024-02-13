
#pragma once

#include <AzCore/Math/Crc.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/EditContextConstants.inl>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzToolsFramework/ToolsComponents/EditorComponentBase.h>

namespace AICore
{
    class PromptTestComponent : public AzToolsFramework::Components::EditorComponentBase
    {
    public:
        AZ_COMPONENT(PromptTestComponent, "{9922e2d3-37c9-4100-b749-154a8bf37030}", AzToolsFramework::Components::EditorComponentBase);

        static void Reflect(AZ::ReflectContext* context);

        // static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        // static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        // static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        // static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        PromptTestComponent() = default;
        ~PromptTestComponent() = default;

        void Activate() override;
        void Deactivate() override;

        AZ::Crc32 TestPrompt();

        AZStd::string m_promptText;
        AZStd::vector<long long> m_context;
        bool m_saveContext = false;
    };
} // namespace AICore