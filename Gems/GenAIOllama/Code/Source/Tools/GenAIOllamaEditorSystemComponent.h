
#pragma once

#include <Clients/GenAIOllamaSystemComponent.h>

namespace GenAIOllama
{
    /// System component for GenAIOllama editor
    class GenAIOllamaEditorSystemComponent : public GenAIOllamaSystemComponent
    {
        using BaseSystemComponent = GenAIOllamaSystemComponent;
    public:
        AZ_COMPONENT_DECL(GenAIOllamaEditorSystemComponent);

        static void Reflect(AZ::ReflectContext* context);

        GenAIOllamaEditorSystemComponent() = default;
        ~GenAIOllamaEditorSystemComponent() = default;

    private:
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        // AZ::Component
        void Activate() override;
        void Deactivate() override;
    };
} // namespace GenAIOllama
