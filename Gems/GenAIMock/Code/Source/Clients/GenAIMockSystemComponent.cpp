
#include "GenAIMockSystemComponent.h"

#include <GenAIMock/GenAIMockTypeIds.h>

#include <AzCore/Serialization/SerializeContext.h>

namespace GenAIMock
{
    AZ_COMPONENT_IMPL(GenAIMockSystemComponent, "GenAIMockSystemComponent", GenAIMockSystemComponentTypeId);

    void GenAIMockSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<GenAIMockSystemComponent, AZ::Component>()->Version(0);
        }
    }

    void GenAIMockSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("GenAIMockService"));
    }

    void GenAIMockSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("GenAIMockService"));
    }

    void GenAIMockSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
    }

    void GenAIMockSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
    }

    void GenAIMockSystemComponent::Activate()
    {
    }

    void GenAIMockSystemComponent::Deactivate()
    {
    }

} // namespace GenAIMock
