
#include "OllamaContextRequestGenerator.h"
#include "AzCore/std/containers/vector.h"

#include <aws/core/utils/Array.h>

namespace AICore
{
    OllamaContextRequestGenerator::OllamaContextRequestGenerator(OllamaBasicPromptConfiguration config)
        : OllamaBasicRequestGenerator(config)
    {
        m_context = AZStd::vector<int>();
    }

    Aws::Utils::Json::JsonValue OllamaContextRequestGenerator::EncodePrompt(AZStd::string prompt)
    {
        Aws::Utils::Json::JsonValue jsonValue = OllamaBasicRequestGenerator::EncodePrompt(prompt);

        Aws::Utils::Array<Aws::Utils::Json::JsonValue> contextArray(m_context.size());

        for (int i = 0; i < m_context.size(); i++)
        {
            // contextArray[i].AsInteger(m_context[i]);
        }

        jsonValue.WithArray("context", contextArray);

        return jsonValue;
    };

    AZStd::string OllamaContextRequestGenerator::DecodeRequest(Aws::Utils::Json::JsonValue request)
    {
        Aws::Utils::Json::JsonView jsonView = request.View();
        if (jsonView.ValueExists("response"))
        {
            auto context = jsonView.GetArray("context");
            int contextLength = context.GetLength();
            AZStd::vector<int> contextVector;
            // m_context.clear();
            // m_context.reserve(context.GetLength());
            std::cout << context.GetLength() << std::endl;
            for (int i = 0; i < contextLength; i++)
            {
                int contextPart = context[i].AsInteger();
                std::cout << contextPart << std::endl;
                contextVector.push_back(contextPart);
            }
            m_context = contextVector;

            return jsonView.GetString("response").c_str();
        }
        else
        {
            return "";
        }
    };

} // namespace AICore