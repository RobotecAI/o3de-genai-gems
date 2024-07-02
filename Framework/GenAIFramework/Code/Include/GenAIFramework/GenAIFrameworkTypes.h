/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AzCore/Outcome/Outcome.h>
#include <AzCore/std/any.h>
#include <AzCore/std/containers/vector.h>
#include <AzCore/std/string/string.h>
#include <AzCore/std/utility/pair.h>

namespace GenAIFramework
{
    enum class Role //!< The information who created a certain message
    {
        User,
        Assistant,
        System
    };

    using AIContent = AZStd::vector<AZStd::any>; //!< The type of content sent and received by the AI
    using AIMessage = AZStd::pair<Role, AIContent>; //!< The type of message sent and received by the AI
    using AIMessages = AZStd::vector<AIMessage>; //!< The array of messages received and sent to the AI
    using ModelAPIRequest = AZStd::string; //!< The type of request to send to the model provider
    using ModelAPIResponse = AZ::Outcome<AZStd::string, AZStd::string>; //!< The type of response from the model provider
    using ModelAPIExtractedResponse = AZ::Outcome<AIMessage, AZStd::string>; //!< The type of extracted response from the model
    using AIHistory = AIMessages; //!< The type of AI history
} // namespace GenAIFramework
