
#include "BasicJSONRequesterEditorComponent.h"

namespace AICore
{
    BasicJSONRequesterEditorComponent::BasicJSONRequesterEditorComponent(const BasicJSONRequesterConfiguration& config)
        : BasicJSONRequesterEditorComponentBase(config)
    {
    }

    void BasicJSONRequesterEditorComponent::Reflect(AZ::ReflectContext* context)
    {
        BasicJSONRequesterEditorComponentBase::Reflect(context);
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<BasicJSONRequesterEditorComponent, BasicJSONRequesterEditorComponentBase>()->Version(0);

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<BasicJSONRequesterEditorComponent>("Basic JSON requester", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "AICore")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("Game"));
            }
        }
    }

    void BasicJSONRequesterEditorComponent::Activate()
    {
        BasicJSONRequesterEditorComponentBase::Activate();
    }

    void BasicJSONRequesterEditorComponent::Deactivate()
    {
        BasicJSONRequesterEditorComponentBase::Deactivate();
    }
} // namespace AICore
