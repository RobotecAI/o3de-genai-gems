
#include "OllamaContextRequestGeneratorComponent.h"

namespace AICore
{

    OllamaContextRequestGeneratorComponent::OllamaContextRequestGeneratorComponent(const OllamaBasicPromptConfiguration& config)
        : OllamaContextRequestGeneratorComponentBase(config)
    {
    }

    void OllamaContextRequestGeneratorComponent::Reflect(AZ::ReflectContext* context)
    {
        OllamaContextRequestGeneratorComponentBase::Reflect(context);

        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<OllamaContextRequestGeneratorComponent, OllamaContextRequestGeneratorComponentBase>()->Version(0);
        }
    }

    void OllamaContextRequestGeneratorComponent::Activate()
    {
        OllamaContextRequestGeneratorComponentBase::Activate();
    }

    void OllamaContextRequestGeneratorComponent::Deactivate()
    {
        OllamaContextRequestGeneratorComponentBase::Deactivate();
    }
} // namespace AICore