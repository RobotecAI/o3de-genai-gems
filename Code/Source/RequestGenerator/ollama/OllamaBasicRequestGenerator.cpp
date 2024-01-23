
#include "OllamaBasicRequestGenerator.h"

namespace AICore
{
    Aws::Utils::Json::JsonView OllamaBasicRequestGenerator::EncodePrompt(const AZStd::string& prompt)
    {
        Aws::Utils::Json::JsonValue jsonValue;

        jsonValue.WithString("prompt", prompt.c_str());
        jsonValue.WithString("model", "mistral");
        jsonValue.WithBool("stream", false);
        Aws::Utils::Json::JsonView jsonView = jsonValue.View();
        return jsonView;
    };

    AZStd::string OllamaBasicRequestGenerator::DecodePrompt(const Aws::Utils::Json::JsonView& request)
    {
        // Aws::Utils::Json::JsonView jsonView = AZStd::any_cast<Aws::Utils::Json::JsonView>(request);
        return request.GetString("response").c_str();
    };
} // namespace AICore