
#pragma once

#include <AzCore/Component/Component.h>

namespace GenAIBedrock
{
    class GenAIBedrockSystemComponent : public AZ::Component
    {
    public:
        AZ_COMPONENT_DECL(GenAIBedrockSystemComponent);

        static void Reflect(AZ::ReflectContext* context);

        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        GenAIBedrockSystemComponent() = default;
        ~GenAIBedrockSystemComponent() = default;

    protected:
        ////////////////////////////////////////////////////////////////////////
        // AZ::Component interface implementation
        void Activate() override{};
        void Deactivate() override{};
        ////////////////////////////////////////////////////////////////////////
    };

} // namespace GenAIBedrock
