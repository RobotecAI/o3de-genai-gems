
#include "GenAIBedrockSystemComponent.h"

#include <GenAIBedrock/GenAIBedrockTypeIds.h>

#include <AzCore/Serialization/SerializeContext.h>

namespace GenAIBedrock
{
    AZ_COMPONENT_IMPL(GenAIBedrockSystemComponent, "GenAIBedrockSystemComponent",
        GenAIBedrockSystemComponentTypeId);

    void GenAIBedrockSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<GenAIBedrockSystemComponent, AZ::Component>()
                ->Version(0)
                ;
        }
    }

    void GenAIBedrockSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("GenAIBedrockService"));
    }

    void GenAIBedrockSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("GenAIBedrockService"));
    }

    void GenAIBedrockSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
    }

    void GenAIBedrockSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
    }

    GenAIBedrockSystemComponent::GenAIBedrockSystemComponent()
    {
        if (GenAIBedrockInterface::Get() == nullptr)
        {
            GenAIBedrockInterface::Register(this);
        }
    }

    GenAIBedrockSystemComponent::~GenAIBedrockSystemComponent()
    {
        if (GenAIBedrockInterface::Get() == this)
        {
            GenAIBedrockInterface::Unregister(this);
        }
    }

    void GenAIBedrockSystemComponent::Init()
    {
    }

    void GenAIBedrockSystemComponent::Activate()
    {
        GenAIBedrockRequestBus::Handler::BusConnect();
        AZ::TickBus::Handler::BusConnect();
    }

    void GenAIBedrockSystemComponent::Deactivate()
    {
        AZ::TickBus::Handler::BusDisconnect();
        GenAIBedrockRequestBus::Handler::BusDisconnect();
    }

    void GenAIBedrockSystemComponent::OnTick([[maybe_unused]] float deltaTime, [[maybe_unused]] AZ::ScriptTimePoint time)
    {
    }

} // namespace GenAIBedrock
