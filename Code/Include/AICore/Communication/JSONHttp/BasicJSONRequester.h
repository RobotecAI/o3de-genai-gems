
#pragma once

#include <AzCore/RTTI/TypeInfo.h>
#include <AzCore/std/string/string.h>

#include <aws/core/utils/json/JsonSerializer.h>

namespace AICore
{
    class BasicJSONRequester
    {
    public:
        BasicJSONRequester(AZStd::string url);
        ~BasicJSONRequester() = default;

        void Request(const Aws::Utils::Json::JsonView& request, AZStd::function<void(const Aws::Utils::Json::JsonView&, bool)> callback);

    private:
        AZStd::string m_url;
    };
} // namespace AICore