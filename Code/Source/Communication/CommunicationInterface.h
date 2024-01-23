
#pragma once

#include <AzCore/RTTI/TypeInfo.h>
#include <AzCore/std/any.h>

namespace AICore
{
    template<typename R>
    class CommunicationInterface
    {
    public:
        // CommunicationInterface() = default;
        virtual ~CommunicationInterface(){};

        virtual void Request(const R& request, AZStd::function<void(const R&, bool)> callback) = 0;
    };
} // namespace AICore