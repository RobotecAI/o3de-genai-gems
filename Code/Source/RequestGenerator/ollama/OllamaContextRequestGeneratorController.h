
#pragma once

#include "OllamaBasicPromptConfiguration.h"
#include <AICore/RequestGenerator/RequestGeneratorBus.h>
#include <AzCore/RTTI/RTTIMacros.h>
#include <AzCore/std/containers/vector.h>
#include <aws/core/utils/json/JsonSerializer.h>

namespace AICore
{
    class OllamaContextRequestGeneratorController
        : public RequestGeneratorBus<Aws::Utils::Json::JsonValue, AZStd::pair<AZStd::string, AZStd::vector<long long>>>::Handler
    {
    public:
        AZ_RTTI(OllamaContextRequestGeneratorController, "{c300d1bc-fe3f-4c0e-855b-49420f858fc4}");

        OllamaContextRequestGeneratorController() = default;
        explicit OllamaContextRequestGeneratorController(const OllamaBasicPromptConfiguration& config);
        ~OllamaContextRequestGeneratorController() = default;

        static void Reflect(AZ::ReflectContext* context);

        void Activate(AZ::EntityId entityId);
        void Deactivate();
        void SetConfiguration(const OllamaBasicPromptConfiguration& config);
        const OllamaBasicPromptConfiguration& GetConfiguration() const;

        // RequestGeneratorBus
        Aws::Utils::Json::JsonValue PrepareRequest(AZStd::pair<AZStd::string, AZStd::vector<long long>> prompt) override;
        AZ::Outcome<AZStd::pair<AZStd::string, AZStd::vector<long long>>, AZStd::string> ProcessRequest(
            Aws::Utils::Json::JsonValue request) override;

    private:
        OllamaBasicPromptConfiguration m_configuration;
    };
} // namespace AICore