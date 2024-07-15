
#pragma once

#include <AzToolsFramework/API/ToolsApplicationAPI.h>

#include <Clients/GenAISampleFeaturesSystemComponent.h>

namespace GenAISampleFeatures
{
    /// System component for GenAISampleFeatures editor
    class GenAISampleFeaturesEditorSystemComponent
        : public GenAISampleFeaturesSystemComponent
        , protected AzToolsFramework::EditorEvents::Bus::Handler
    {
        using BaseSystemComponent = GenAISampleFeaturesSystemComponent;
    public:
        AZ_COMPONENT_DECL(GenAISampleFeaturesEditorSystemComponent);

        static void Reflect(AZ::ReflectContext* context);

        GenAISampleFeaturesEditorSystemComponent();
        ~GenAISampleFeaturesEditorSystemComponent();

    private:
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        // AZ::Component
        void Activate() override;
        void Deactivate() override;
    };
} // namespace GenAISampleFeatures
