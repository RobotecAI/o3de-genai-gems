#include "CameraToolsHandler.h"
#include "AzCore/Math/Uuid.h"
#include "AzCore/std/string/string.h"
#include "GenAIFramework/CameraToolsBus.h"

#include <Atom/RPI.Public/RenderPipeline.h>
#include <Atom/Utils/PpmFile.h>
#include <AzCore/std/ranges/split_view.h>
#include <future>

namespace GenAIFramework
{
    CameraToolsHandler::CameraToolsHandler()
    {
        m_pipelineChangedHandler = AZ::RPI::ViewportContext::PipelineChangedEvent::Handler(
            [this](AZ::RPI::RenderPipelinePtr pipeline)
            {
                this->SetRenderPipeline(pipeline);
            });

        CameraToolsBus::Handler::BusConnect();
        AZ::RPI::ViewportContextManagerNotificationsBus::Handler::BusConnect();
    }

    CameraToolsHandler::~CameraToolsHandler()
    {
        AZ::RPI::ViewportContextManagerNotificationsBus::Handler::BusDisconnect();
        CameraToolsBus::Handler::BusDisconnect();
        m_pipelineChangedHandler.Disconnect();
    }

    void CameraToolsHandler::Reflect(AZ::ReflectContext* context)
    {
        if (auto behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            behaviorContext->EBus<CameraToolsBus>("CameraToolsBus")
                ->Attribute(AZ::Script::Attributes::Category, "CameraToolsBus")
                ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Common)
                ->Attribute(AZ::Script::Attributes::Module, "ai")
                ->Event("GetViewportBase64ImageCallback", &CameraToolsBus::Events::GetViewportBase64ImageCallback)
                ->Event("RequestViewportBase64Image", &CameraToolsBus::Events::RequestViewportBase64Image)
                ->Event("IsImageReady", &CameraToolsBus::Events::IsImageReady)
                ->Event("GetBase64Image", &CameraToolsBus::Events::GetBase64Image);
        }
    }

    void CameraToolsHandler::OnViewportContextAdded(AZ::RPI::ViewportContextPtr viewportContext)
    {
        auto pipeline = viewportContext->GetCurrentPipeline();
        SetRenderPipeline(pipeline);

        viewportContext->ConnectCurrentPipelineChangedHandler(m_pipelineChangedHandler);
    }

    void CameraToolsHandler::SetRenderPipeline(AZ::RPI::RenderPipelinePtr pipeline)
    {
        if (!pipeline)
        {
            m_passHierarchy.clear();
            return;
        }
        auto passes = AZStd::ranges::views::split(pipeline->GetRootPass()->GetPathName().GetStringView(), '.');
        m_passHierarchy.clear();
        for (auto pass : passes)
        {
            m_passHierarchy.push_back(AZStd::string(pass));
        }
        m_passHierarchy.push_back("CopyToSwapChain");
    }

    AZ::Render::FrameCaptureOutcome CameraToolsHandler::GetViewportBase64ImageCallback(
        AZStd::function<void(AZStd::string)> imageReadyCallback) const
    {
        AZStd::vector<AZStd::string> hierarchy;
        AZ::Render::FrameCaptureOutcome outcome;

        AZ::Render::FrameCaptureRequestBus::BroadcastResult(
            outcome,
            &AZ::Render::FrameCaptureRequestBus::Events::CapturePassAttachmentWithCallback,
            [imageReadyCallback](const AZ::RPI::AttachmentReadback::ReadbackResult& result)
            {
                if (result.m_state != AZ::RPI::AttachmentReadback::ReadbackState::Success)
                {
                    return;
                }
                const AZStd::vector<uint8_t> outBuffer = AZ::Utils::PpmFile::CreatePpmFromImageBuffer(
                    *result.m_dataBuffer.get(), result.m_imageDescriptor.m_size, result.m_imageDescriptor.m_format);

                AZStd::string imageBase64 = AZ::StringFunc::Base64::Encode(outBuffer.data(), outBuffer.size());
                imageReadyCallback(imageBase64);
            },
            m_passHierarchy,
            AZStd::string("Output"),
            AZ::RPI::PassAttachmentReadbackOption::Output);

        return outcome;
    }

    AZ::Uuid CameraToolsHandler::RequestViewportBase64Image()
    {
        AZ::Render::FrameCaptureOutcome outcome;

        AZ::Uuid imageId = AZ::Uuid::CreateRandom();

        outcome = GetViewportBase64ImageCallback(
            [this, imageId](AZStd::string base64)
            {
                AZStd::lock_guard<AZStd::mutex> lock(m_imageMutex);
                m_readyImages[imageId] = base64;
            });

        return imageId;
    }

    bool CameraToolsHandler::IsImageReady(AZ::Uuid promptId)
    {
        AZStd::lock_guard<AZStd::mutex> lock(m_imageMutex);
        return m_readyImages.find(promptId) != m_readyImages.end();
    }

    AZStd::string CameraToolsHandler::GetBase64Image(AZ::Uuid promptId)
    {
        AZStd::lock_guard<AZStd::mutex> lock(m_imageMutex);
        auto it = m_readyImages.find(promptId);
        if (it != m_readyImages.end())
        {
            AZStd::string base64 = it->second;
            m_readyImages.erase(it);
            return base64;
        }
        return "";
    }

} // namespace GenAIFramework
