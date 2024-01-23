
#pragma once

#include "RequestGenerator/ollama/OllamaContextRequestGeneratorComponent.h"
#include "RequestGenerator/ollama/OllamaContextRequestGeneratorController.h"
#include "ToolsComponents/EditorComponentAdapter.h"
#include "ToolsComponents/EditorComponentBase.h"

namespace AICore
{
    using OllamaContextRequestGeneratorEditorComponentBase = AzToolsFramework::Components::EditorComponentAdapter<
        OllamaContextRequestGeneratorController,
        OllamaContextRequestGeneratorComponent,
        OllamaBasicPromptConfiguration>;

    class OllamaContextRequestGeneratorEditorComponent : public OllamaContextRequestGeneratorEditorComponentBase
    {
    public:
        AZ_COMPONENT(
            OllamaContextRequestGeneratorEditorComponent,
            "{43bb3b34-0a64-4d2d-93f5-5ff5b4257433}",
            AzToolsFramework::Components::EditorComponentBase);

        OllamaContextRequestGeneratorEditorComponent() = default;
        explicit OllamaContextRequestGeneratorEditorComponent(const OllamaBasicPromptConfiguration& config);
        ~OllamaContextRequestGeneratorEditorComponent() = default;

        static void Reflect(AZ::ReflectContext* context);

        void Activate() override;
        void Deactivate() override;
    };
} // namespace AICore