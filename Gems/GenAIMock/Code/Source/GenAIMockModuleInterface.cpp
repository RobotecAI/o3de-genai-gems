
#include "GenAIMockModuleInterface.h"
#include <AzCore/Memory/Memory.h>

#include <GenAIMock/GenAIMockTypeIds.h>

#include <Clients/GenAIMockSystemComponent.h>

namespace GenAIMock
{
    AZ_TYPE_INFO_WITH_NAME_IMPL(GenAIMockModuleInterface,
        "GenAIMockModuleInterface", GenAIMockModuleInterfaceTypeId);
    AZ_RTTI_NO_TYPE_INFO_IMPL(GenAIMockModuleInterface, AZ::Module);
    AZ_CLASS_ALLOCATOR_IMPL(GenAIMockModuleInterface, AZ::SystemAllocator);

    GenAIMockModuleInterface::GenAIMockModuleInterface()
    {
        // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
        // Add ALL components descriptors associated with this gem to m_descriptors.
        // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and EditContext.
        // This happens through the [MyComponent]::Reflect() function.
        m_descriptors.insert(m_descriptors.end(), {
            GenAIMockSystemComponent::CreateDescriptor(),
            });
    }

    AZ::ComponentTypeList GenAIMockModuleInterface::GetRequiredSystemComponents() const
    {
        return AZ::ComponentTypeList{
            azrtti_typeid<GenAIMockSystemComponent>(),
        };
    }
} // namespace GenAIMock