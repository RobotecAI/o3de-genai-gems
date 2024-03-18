
#include "GenAIBedrockModuleInterface.h"
#include <AzCore/Memory/Memory.h>

#include <GenAIBedrock/GenAIBedrockTypeIds.h>

#include <Clients/GenAIBedrockSystemComponent.h>

namespace GenAIBedrock
{
    AZ_TYPE_INFO_WITH_NAME_IMPL(GenAIBedrockModuleInterface,
        "GenAIBedrockModuleInterface", GenAIBedrockModuleInterfaceTypeId);
    AZ_RTTI_NO_TYPE_INFO_IMPL(GenAIBedrockModuleInterface, AZ::Module);
    AZ_CLASS_ALLOCATOR_IMPL(GenAIBedrockModuleInterface, AZ::SystemAllocator);

    GenAIBedrockModuleInterface::GenAIBedrockModuleInterface()
    {
        // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
        // Add ALL components descriptors associated with this gem to m_descriptors.
        // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and EditContext.
        // This happens through the [MyComponent]::Reflect() function.
        m_descriptors.insert(m_descriptors.end(), {
            GenAIBedrockSystemComponent::CreateDescriptor(),
            });
    }

    AZ::ComponentTypeList GenAIBedrockModuleInterface::GetRequiredSystemComponents() const
    {
        return AZ::ComponentTypeList{
            azrtti_typeid<GenAIBedrockSystemComponent>(),
        };
    }
} // namespace GenAIBedrock
