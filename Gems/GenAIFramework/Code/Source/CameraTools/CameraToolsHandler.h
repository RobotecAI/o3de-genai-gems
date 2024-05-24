#pragma once

#include "AzCore/Math/Uuid.h"
#include <Atom/RPI.Public/Base.h>
#include <Atom/RPI.Public/ViewportContext.h>
#include <Atom/RPI.Public/ViewportContextBus.h>
#include <GenAIFramework/CameraToolsBus.h>

namespace GenAIFramework
{
    class CameraToolsHandler
        : public CameraToolsBus::Handler
        , private AZ::RPI::ViewportContextManagerNotificationsBus::Handler
    {
    public:
        CameraToolsHandler();
        ~CameraToolsHandler();

        static void Reflect(AZ::ReflectContext* context);

        // GenAIFrameworkRequestBus handler overrides
        AZ::Render::FrameCaptureOutcome GetViewportBase64ImageCallback(
            AZStd::function<void(AZStd::string)> imageReadyCallback) const override;

        AZ::Uuid RequestViewportBase64Image() override;
        bool IsImageReady(AZ::Uuid promptId) override;
        AZStd::string GetBase64Image(AZ::Uuid promptId) override;

    private:
        ////////////////////////////////////////////////////////////////////////
        // AZ::RPI::ViewportContextManagerNotificationsBus interface implementation
        void OnViewportContextAdded(AZ::RPI::ViewportContextPtr viewportContext) override;
        ////////////////////////////////////////////////////////////////////////
        void SetRenderPipeline(AZ::RPI::RenderPipelinePtr pipeline);

        AZ::RPI::ViewportContext::PipelineChangedEvent::Handler m_pipelineChangedHandler;
        AZStd::vector<AZStd::string> m_passHierarchy;

        AZStd::mutex m_imageMutex;
        AZStd::map<AZ::Uuid, AZStd::string> m_readyImages;
    };
} // namespace GenAIFramework
