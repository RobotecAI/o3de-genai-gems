
#include <AzCore/Serialization/SerializeContext.h>
#include "GenAIOllamaEditorSystemComponent.h"

#include <GenAIOllama/GenAIOllamaTypeIds.h>

namespace GenAIOllama
{
    AZ_COMPONENT_IMPL(GenAIOllamaEditorSystemComponent, "GenAIOllamaEditorSystemComponent",
        GenAIOllamaEditorSystemComponentTypeId, BaseSystemComponent);

    void GenAIOllamaEditorSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<GenAIOllamaEditorSystemComponent, GenAIOllamaSystemComponent>()
                ->Version(0);
        }
    }

    void GenAIOllamaEditorSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        BaseSystemComponent::GetProvidedServices(provided);
        provided.push_back(AZ_CRC_CE("GenAIOllamaEditorService"));
    }

    void GenAIOllamaEditorSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        BaseSystemComponent::GetIncompatibleServices(incompatible);
        incompatible.push_back(AZ_CRC_CE("GenAIOllamaEditorService"));
    }

    void GenAIOllamaEditorSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        BaseSystemComponent::GetRequiredServices(required);
    }

    void GenAIOllamaEditorSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
        BaseSystemComponent::GetDependentServices(dependent);
    }

    void GenAIOllamaEditorSystemComponent::Activate()
    {
    }

    void GenAIOllamaEditorSystemComponent::Deactivate()
    {
    }

} // namespace GenAIOllama
