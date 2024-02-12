
#pragma once

#include <AICore/Communication/Requester.h>
#include <AzCore/RTTI/ReflectContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/std/string/string.h>
#include <aws/core/utils/json/JsonSerializer.h>

namespace AICore
{
    class BasicJSONRequester : public Requester<Aws::Utils::Json::JsonValue>
    {
    public:
        AZ_RTTI(BasicJSONRequester, "{77fe39e7-4f18-4b83-a948-825bc7977f93}", Requester<Aws::Utils::Json::JsonValue>)

        BasicJSONRequester(AZStd::string url);
        ~BasicJSONRequester() = default;

        static void Reflect(AZ::ReflectContext* context);

        void SendRequest(
            Aws::Utils::Json::JsonValue request,
            AZStd::function<void(Aws::Utils::Json::JsonValue, AZStd::optional<AZStd::string>)> callback) override;

    private:
        AZStd::string m_url;
    };
} // namespace AICore
