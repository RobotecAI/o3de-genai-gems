
#include <AICore/RequestGenerator/ollama/OllamaBasicRequestConfiguration.h>
#include <AICore/RequestGenerator/ollama/OllamaBasicRequestGenerator.h>
#include <aws/core/utils/memory/stl/AWSString.h>

namespace AICore
{
    OllamaBasicRequestGenerator::OllamaBasicRequestGenerator(OllamaBasicRequestConfiguration config)
        : m_config(config){};

    Aws::Utils::Json::JsonView OllamaBasicRequestGenerator::EncodePrompt(const AZStd::string& prompt)
    {
        Aws::Utils::Json::JsonValue jsonValue;

        std::cout << "prompt: " << prompt.c_str() << std::endl;
        Aws::String awsPrompt("test");

        std::cout << "awsPrompt: " << awsPrompt.c_str() << std::endl;

        // jsonValue.WithString("prompt", awsPrompt);

        // jsonValue = jsonValue.WithString("model", m_config.m_model.c_str());
        // if (m_config.m_format.has_value())
        // {
        //     jsonValue = jsonValue.WithString("format", m_config.m_format.value().c_str());
        // }
        // if (m_config.m_options.has_value())
        // {
        //     jsonValue = jsonValue.WithString("options", m_config.m_options.value().c_str());
        // }
        // if (m_config.m_system.has_value())
        // {
        //     jsonValue = jsonValue.WithString("system", m_config.m_system.value().c_str());
        // }
        // if (m_config.m_template.has_value())
        // {
        //     jsonValue = jsonValue.WithString("template", m_config.m_template.value().c_str());
        // }
        // jsonValue.WithBool("stream", m_config.m_stream.value_or(false));
        // jsonValue.WithBool("raw", m_config.m_raw.value_or(false));

        Aws::Utils::Json::JsonView jsonView = jsonValue.View();
        return jsonView;
    };

    AZStd::string OllamaBasicRequestGenerator::DecodeRequest(const Aws::Utils::Json::JsonView& request)
    {
        return request.GetString("response").c_str();
    };
} // namespace AICore