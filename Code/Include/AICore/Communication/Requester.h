
#pragma once

#include <AzCore/std/functional.h>
#include <AzCore/std/string/string.h>

namespace AICore
{
    template<typename RequestType>
    class Requester
    {
    public:
        Requester() = default;
        virtual ~Requester() = default;

        virtual void SendRequest(RequestType request, AZStd::function<void(RequestType, AZStd::optional<AZStd::string>)> callback) = 0;

        using Request = RequestType;
    };
} // namespace AICore
