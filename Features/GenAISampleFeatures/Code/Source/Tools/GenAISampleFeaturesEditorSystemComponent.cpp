
#include "GenAISampleFeaturesEditorSystemComponent.h"
#include <AzCore/Serialization/SerializeContext.h>
#include <O3DEAssistantPython/O3DEAssistantPythonFeature.h>
#include <SceneGenerationPython/SceneGenerationPythonFeature.h>

#include <GenAISampleFeatures/GenAISampleFeaturesTypeIds.h>

namespace GenAISampleFeatures
{
    AZ_COMPONENT_IMPL(
        GenAISampleFeaturesEditorSystemComponent,
        "GenAISampleFeaturesEditorSystemComponent",
        GenAISampleFeaturesEditorSystemComponentTypeId,
        BaseSystemComponent);

    void GenAISampleFeaturesEditorSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        O3DEAssistantPythonFeature::Reflect(context);
        SceneGenerationPythonFeature::Reflect(context);

        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<GenAISampleFeaturesEditorSystemComponent, GenAISampleFeaturesSystemComponent>()->Version(0);
        }
    }

    GenAISampleFeaturesEditorSystemComponent::GenAISampleFeaturesEditorSystemComponent() = default;

    GenAISampleFeaturesEditorSystemComponent::~GenAISampleFeaturesEditorSystemComponent() = default;

    void GenAISampleFeaturesEditorSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        BaseSystemComponent::GetProvidedServices(provided);
        provided.push_back(AZ_CRC_CE("GenAISampleFeaturesEditorService"));
    }

    void GenAISampleFeaturesEditorSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        BaseSystemComponent::GetIncompatibleServices(incompatible);
        incompatible.push_back(AZ_CRC_CE("GenAISampleFeaturesEditorService"));
    }

    void GenAISampleFeaturesEditorSystemComponent::GetRequiredServices(
        [[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        BaseSystemComponent::GetRequiredServices(required);
    }

    void GenAISampleFeaturesEditorSystemComponent::GetDependentServices(
        [[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
        BaseSystemComponent::GetDependentServices(dependent);
    }

    void GenAISampleFeaturesEditorSystemComponent::Activate()
    {
        GenAISampleFeaturesSystemComponent::Activate();
        AzToolsFramework::EditorEvents::Bus::Handler::BusConnect();
    }

    void GenAISampleFeaturesEditorSystemComponent::Deactivate()
    {
        AzToolsFramework::EditorEvents::Bus::Handler::BusDisconnect();
        GenAISampleFeaturesSystemComponent::Deactivate();
    }

} // namespace GenAISampleFeatures
