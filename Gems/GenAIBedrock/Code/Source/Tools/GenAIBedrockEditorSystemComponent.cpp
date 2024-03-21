
#include "GenAIBedrockEditorSystemComponent.h"
#include <AzCore/Serialization/SerializeContext.h>

#include <GenAIBedrock/GenAIBedrockTypeIds.h>

namespace GenAIBedrock
{
    AZ_COMPONENT_IMPL(
        GenAIBedrockEditorSystemComponent,
        "GenAIBedrockEditorSystemComponent",
        GenAIBedrockEditorSystemComponentTypeId,
        BaseSystemComponent);

    void GenAIBedrockEditorSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<GenAIBedrockEditorSystemComponent, GenAIBedrockSystemComponent>()->Version(0);
        }
    }

    GenAIBedrockEditorSystemComponent::GenAIBedrockEditorSystemComponent() = default;

    GenAIBedrockEditorSystemComponent::~GenAIBedrockEditorSystemComponent() = default;

    void GenAIBedrockEditorSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        BaseSystemComponent::GetProvidedServices(provided);
        provided.push_back(AZ_CRC_CE("GenAIBedrockEditorService"));
    }

    void GenAIBedrockEditorSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        BaseSystemComponent::GetIncompatibleServices(incompatible);
        incompatible.push_back(AZ_CRC_CE("GenAIBedrockEditorService"));
    }

    void GenAIBedrockEditorSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        BaseSystemComponent::GetRequiredServices(required);
    }

    void GenAIBedrockEditorSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
        BaseSystemComponent::GetDependentServices(dependent);
    }

    void GenAIBedrockEditorSystemComponent::Activate()
    {
        GenAIBedrockSystemComponent::Activate();
        AzToolsFramework::EditorEvents::Bus::Handler::BusConnect();
    }

    void GenAIBedrockEditorSystemComponent::Deactivate()
    {
        AzToolsFramework::EditorEvents::Bus::Handler::BusDisconnect();
        GenAIBedrockSystemComponent::Deactivate();
    }

} // namespace GenAIBedrock
