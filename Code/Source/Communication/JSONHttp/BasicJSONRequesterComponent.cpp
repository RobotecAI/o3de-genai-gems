
#include "BasicJSONRequesterComponent.h"

namespace AICore
{
    BasicJSONRequesterComponent::BasicJSONRequesterComponent(const BasicJSONRequesterConfiguration& config)
        : BasicJSONRequesterComponentBase(config)
    {
    }

    void BasicJSONRequesterComponent::Reflect(AZ::ReflectContext* context)
    {
        BasicJSONRequesterComponentBase::Reflect(context);

        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<BasicJSONRequesterComponent, BasicJSONRequesterComponentBase>()->Version(0);
        }
    }

    void BasicJSONRequesterComponent::Activate()
    {
        BasicJSONRequesterComponentBase::Activate();
    }

    void BasicJSONRequesterComponent::Deactivate()
    {
        BasicJSONRequesterComponentBase::Deactivate();
    }
} // namespace AICore