
#include "OllamaBasicRequestGenerator.h"

namespace AICore
{
    OllamaBasicRequestGenerator::OllamaBasicRequestGenerator(OllamaBasicPromptConfiguration config)
        : RequestGenerator(config){};

    Aws::Utils::Json::JsonValue OllamaBasicRequestGenerator::EncodePrompt(AZStd::string prompt)
    {
        Aws::Utils::Json::JsonValue jsonValue;

        jsonValue.WithString("prompt", prompt.c_str());
        jsonValue.WithString("model", m_config.m_model.c_str());

        jsonValue = jsonValue.WithString("model", m_config.m_model.c_str());
        if (m_config.m_format.has_value())
        {
            jsonValue = jsonValue.WithString("format", m_config.m_format.value().c_str());
        }
        if (m_config.m_options.has_value())
        {
            jsonValue = jsonValue.WithString("options", m_config.m_options.value().c_str());
        }
        if (m_config.m_template.has_value())
        {
            jsonValue = jsonValue.WithString("template", m_config.m_template.value().c_str());
        }
        jsonValue.WithBool("stream", m_config.m_stream.value_or(false));
        jsonValue.WithBool("raw", m_config.m_raw.value_or(false));

        return jsonValue;
    };

    AZStd::string OllamaBasicRequestGenerator::DecodeRequest(Aws::Utils::Json::JsonValue request)
    {
        Aws::Utils::Json::JsonView jsonView = request.View();
        if (jsonView.ValueExists("response"))
        {
            return jsonView.GetString("response").c_str();
        }
        else
        {
            return "";
        }
    };
} // namespace AICore
