
#include "GenAISampleFeaturesSystemComponent.h"

#include <GenAISampleFeatures/GenAISampleFeaturesTypeIds.h>
#include <O3DEAssistant/O3DEAssistantFeature.h>

#include <AzCore/Serialization/SerializeContext.h>

namespace GenAISampleFeatures
{
    AZ_COMPONENT_IMPL(GenAISampleFeaturesSystemComponent, "GenAISampleFeaturesSystemComponent", GenAISampleFeaturesSystemComponentTypeId);

    void GenAISampleFeaturesSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        O3DEAssistantFeature::Reflect(context);

        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<GenAISampleFeaturesSystemComponent, AZ::Component>()->Version(0);
        }
    }

    void GenAISampleFeaturesSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("GenAISampleFeaturesService"));
    }

    void GenAISampleFeaturesSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("GenAISampleFeaturesService"));
    }

    void GenAISampleFeaturesSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
    }

    void GenAISampleFeaturesSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
    }

    GenAISampleFeaturesSystemComponent::GenAISampleFeaturesSystemComponent()
    {
        if (GenAISampleFeaturesInterface::Get() == nullptr)
        {
            GenAISampleFeaturesInterface::Register(this);
        }
    }

    GenAISampleFeaturesSystemComponent::~GenAISampleFeaturesSystemComponent()
    {
        if (GenAISampleFeaturesInterface::Get() == this)
        {
            GenAISampleFeaturesInterface::Unregister(this);
        }
    }

    void GenAISampleFeaturesSystemComponent::Init()
    {
    }

    void GenAISampleFeaturesSystemComponent::Activate()
    {
        GenAISampleFeaturesRequestBus::Handler::BusConnect();
        AZ::TickBus::Handler::BusConnect();
    }

    void GenAISampleFeaturesSystemComponent::Deactivate()
    {
        AZ::TickBus::Handler::BusDisconnect();
        GenAISampleFeaturesRequestBus::Handler::BusDisconnect();
    }

    void GenAISampleFeaturesSystemComponent::OnTick([[maybe_unused]] float deltaTime, [[maybe_unused]] AZ::ScriptTimePoint time)
    {
    }

} // namespace GenAISampleFeatures
