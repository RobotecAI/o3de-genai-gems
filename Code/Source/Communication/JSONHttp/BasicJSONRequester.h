
#pragma once

#include "AzCore/Memory/Memory_fwd.h"
#include "AzCore/Memory/SystemAllocator.h"
#include <AICore/Communication/Requester.h>
#include <AzCore/RTTI/ReflectContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/std/string/string.h>
#include <aws/core/utils/json/JsonSerializer.h>

namespace AZ
{
    AZ_TYPE_INFO_SPECIALIZE(Aws::Utils::Json::JsonValue, "{61466e55-f27a-4dd5-87e0-d3de11c187ae}");
}

namespace AICore
{
    class BasicJSONRequester : public Requester<Aws::Utils::Json::JsonValue>
    {
    public:
        AZ_RTTI(BasicJSONRequester, "{77fe39e7-4f18-4b83-a948-825bc7977f93}", Requester<Aws::Utils::Json::JsonValue>)
        AZ_CLASS_ALLOCATOR(BasicJSONRequester, AZ::SystemAllocator)

        BasicJSONRequester();
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

