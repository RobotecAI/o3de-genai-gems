
#pragma once

#include <AzToolsFramework/API/ToolsApplicationAPI.h>

#include <Clients/GenAIMockSystemComponent.h>

namespace GenAIMock
{
    /// System component for GenAIMock editor
    class GenAIMockEditorSystemComponent
        : public GenAIMockSystemComponent
        , protected AzToolsFramework::EditorEvents::Bus::Handler
    {
        using BaseSystemComponent = GenAIMockSystemComponent;
    public:
        AZ_COMPONENT_DECL(GenAIMockEditorSystemComponent);

        static void Reflect(AZ::ReflectContext* context);

        GenAIMockEditorSystemComponent();
        ~GenAIMockEditorSystemComponent();

    private:
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        // AZ::Component
        void Activate() override;
        void Deactivate() override;
    };
} // namespace GenAIMock
