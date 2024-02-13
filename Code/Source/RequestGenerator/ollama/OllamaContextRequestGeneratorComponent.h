
#pragma once

#include "RequestGenerator/ollama/OllamaContextRequestGeneratorController.h"
#include <AzCore/Component/Component.h>
#include <AzFramework/Components/ComponentAdapter.h>

namespace AICore
{
    using OllamaContextRequestGeneratorComponentBase =
        AzFramework::Components::ComponentAdapter<OllamaContextRequestGeneratorController, OllamaBasicPromptConfiguration>;

    class OllamaContextRequestGeneratorComponent : public OllamaContextRequestGeneratorComponentBase
    {
    public:
        AZ_COMPONENT(OllamaContextRequestGeneratorComponent, "{3782988d-058f-4943-9862-874ebc90a240}", AZ::Component);

        OllamaContextRequestGeneratorComponent() = default;
        OllamaContextRequestGeneratorComponent(const OllamaBasicPromptConfiguration& other);
        ~OllamaContextRequestGeneratorComponent() = default;

        static void Reflect(AZ::ReflectContext* context);

        //////////////////////////////////////////////////////////////////////////
        // Component overrides
        void Activate() override;
        void Deactivate() override;
        //////////////////////////////////////////////////////////////////////////
    };
} // namespace AICore