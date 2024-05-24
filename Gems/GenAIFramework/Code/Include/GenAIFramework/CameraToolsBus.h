/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <Atom/Feature/Utils/FrameCaptureBus.h>
#include <AzCore/Component/Component.h>
#include <AzCore/Component/EntityId.h>
#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>
#include <AzCore/Math/Uuid.h>
#include <GenAIFramework/GenAIFrameworkTypeIds.h>
#include <GenAIFramework/SystemRegistrationContext/SystemRegistrationContext.h>
#include <future>

namespace GenAIFramework
{
    class CameraToolsRequests
    {
    public:
        AZ_RTTI(CameraToolsRequests, "{370c9f3d-3ce8-49f0-a98e-f61d59e2501a}");
        virtual ~CameraToolsRequests() = default;

        //! Get a base64 encoded image of the current viewport.
        //! @param imageReadyCallback A callback function that will be called when the image is ready. The argument is a base64 encoded ppm
        //! image of the current viewport.
        //! @return A FrameCaptureOutcome object.
        virtual AZ::Render::FrameCaptureOutcome GetViewportBase64ImageCallback(
            AZStd::function<void(AZStd::string)> imageReadyCallback) const = 0;
        virtual AZ::Uuid RequestViewportBase64Image() = 0;
        virtual bool IsImageReady(AZ::Uuid promptId) = 0;
        virtual AZStd::string GetBase64Image(AZ::Uuid promptId) = 0;
    };

    class CameraToolsBusTraits : public AZ::EBusTraits
    {
    public:
        //////////////////////////////////////////////////////////////////////////
        // EBusTraits overrides
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
        using MutexType = AZStd::recursive_mutex;
        //////////////////////////////////////////////////////////////////////////
    };

    using CameraToolsBus = AZ::EBus<CameraToolsRequests, CameraToolsBusTraits>;
} // namespace GenAIFramework
