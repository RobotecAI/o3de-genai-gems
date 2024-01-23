
#pragma once

#include "AzCore/std/any.h"
#include "Communication/CommunicationInterface.h"

#include <HttpRequestor/HttpRequestorBus.h>
#include <HttpRequestor/HttpTypes.h>
#include <aws/common/allocator.h>
#include <aws/core/utils/json/JsonSerializer.h>
#include <aws/core/utils/memory/stl/AWSString.h>

namespace AICore
{
    class JSONRequester : public CommunicationInterface<Aws::Utils::Json::JsonView>
    {
    public:
        JSONRequester(AZStd::string url);
        ~JSONRequester() = default;

        void Request(
            const Aws::Utils::Json::JsonView& request, AZStd::function<void(const Aws::Utils::Json::JsonView&, bool)> callback) override;

    private:
        AZStd::string m_url;
    };
} // namespace AICore