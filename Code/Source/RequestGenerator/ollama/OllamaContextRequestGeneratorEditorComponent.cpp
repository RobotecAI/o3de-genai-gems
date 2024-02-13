
#include "OllamaContextRequestGeneratorEditorComponent.h"
#include "AzCore/Serialization/EditContext.h"

namespace AICore
{
    OllamaContextRequestGeneratorEditorComponent::OllamaContextRequestGeneratorEditorComponent(const OllamaBasicPromptConfiguration& config)
        : OllamaContextRequestGeneratorEditorComponentBase(config)
    {
    }

    void OllamaContextRequestGeneratorEditorComponent::Reflect(AZ::ReflectContext* context)
    {
        OllamaContextRequestGeneratorEditorComponentBase::Reflect(context);

        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<OllamaContextRequestGeneratorEditorComponent, OllamaContextRequestGeneratorEditorComponentBase>()
                ->Version(0);

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<OllamaContextRequestGeneratorEditorComponent>("Ollama Context Request Generator", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "AICore")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("Game"));
            }
        }
    }

    void OllamaContextRequestGeneratorEditorComponent::Activate()
    {
        OllamaContextRequestGeneratorEditorComponentBase::Activate();
    }

    void OllamaContextRequestGeneratorEditorComponent::Deactivate()
    {
        OllamaContextRequestGeneratorEditorComponentBase::Deactivate();
    }
} // namespace AICore