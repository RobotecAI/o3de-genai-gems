
#pragma once

#include <AICore/Communication/Requester.h>
#include <AzCore/std/string/string.h>
#include <aws/core/utils/json/JsonSerializer.h>

namespace AICore
{
    class BasicJSONRequester : public Requester<Aws::Utils::Json::JsonValue>
    {
    public:
        BasicJSONRequester(AZStd::string url);
        ~BasicJSONRequester() = default;

        void SendRequest(
            Aws::Utils::Json::JsonValue request,
            AZStd::function<void(Aws::Utils::Json::JsonValue, AZStd::optional<AZStd::string>)> callback) override;

    private:
        AZStd::string m_url;
    };
} // namespace AICore
