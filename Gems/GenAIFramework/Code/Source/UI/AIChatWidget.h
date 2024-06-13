/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#if !defined(Q_MOC_RUN)
#include <QVBoxLayout>
#include <QWidget>

#include <AzCore/Component/Entity.h>
#include <AzCore/Component/TickBus.h>
#include <AzCore/std/parallel/mutex.h>
#include <GenAIFramework/GenAIFrameworkBus.h>
#endif

namespace Ui
{
    class AIChatWidgetUI;
}

namespace GenAIFramework
{
    class AIChatWidget
        : public QWidget
        , private GenAIFramework::ConversationNotificationBus::Handler
        , private AZ::TickBus::Handler
    {
        Q_OBJECT
    public:
        explicit AIChatWidget(QWidget* parent = nullptr, QString modelName = "", QString providerName = "", QString featureName = "");
        ~AIChatWidget();

    signals:
        void chatClosed();

    private slots:
        void OnRequestButton();
        void OnCloseButton();

    private:
        void UiAppendChatMessage(const AZStd::string& message, const bool response = false);
        void UiClearMessages();

        // AZ::TickBus::Handler
        void OnTick(float deltaTime, AZ::ScriptTimePoint time) override;
        // GenAIFramework::ConversationNotificationBus::Handler
        void OnFeatureResponse(const AZStd::string& summary, const AZStd::vector<AZStd::string>& detailedResponse) override;

        Ui::AIChatWidgetUI* m_ui;
        QVBoxLayout* m_uiChatLayout;

        AZ::u64 m_featureId = 0;
        using SummaryDetailedPair = AZStd::pair<AZStd::string, AZStd::vector<AZStd::string>>;
        AZStd::queue<AZStd::pair<SummaryDetailedPair, bool>> m_chatMessagesQueue;
        AZStd::mutex m_chatMessagesQueueMutex;
    };
} // namespace GenAIFramework
