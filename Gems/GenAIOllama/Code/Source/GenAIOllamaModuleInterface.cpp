
#include "GenAIOllamaModuleInterface.h"
#include <AzCore/Memory/Memory.h>

#include <GenAIOllama/GenAIOllamaTypeIds.h>

#include <Clients/GenAIOllamaSystemComponent.h>

#include "Communication/JSONHttp/BasicJSONRequesterComponent.h"
#include "ModelConfigurations/ollama/OllamaModelConfigurationComponent.h"

namespace GenAIOllama
{
    AZ_TYPE_INFO_WITH_NAME_IMPL(GenAIOllamaModuleInterface, "GenAIOllamaModuleInterface", GenAIOllamaModuleInterfaceTypeId);
    AZ_RTTI_NO_TYPE_INFO_IMPL(GenAIOllamaModuleInterface, AZ::Module);
    AZ_CLASS_ALLOCATOR_IMPL(GenAIOllamaModuleInterface, AZ::SystemAllocator);

    GenAIOllamaModuleInterface::GenAIOllamaModuleInterface()
    {
        // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
        // Add ALL components descriptors associated with this gem to m_descriptors.
        // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and EditContext.
        // This happens through the [MyComponent]::Reflect() function.
        m_descriptors.insert(
            m_descriptors.end(),
            {
                GenAIOllamaSystemComponent::CreateDescriptor(),
                OllamaModelConfigurationComponent::CreateDescriptor(),
                BasicJSONRequesterComponent::CreateDescriptor(),
            });
    }

    AZ::ComponentTypeList GenAIOllamaModuleInterface::GetRequiredSystemComponents() const
    {
        return AZ::ComponentTypeList{
            azrtti_typeid<GenAIOllamaSystemComponent>(),
        };
    }
} // namespace GenAIOllama
