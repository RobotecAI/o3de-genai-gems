/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root
 * of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "AIChatWidget.h"

#include <AzCore/Component/ComponentApplicationBus.h>
#include <AzCore/Component/Entity.h>
#include <AzCore/Component/TickBus.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/std/containers/vector.h>
#include <AzCore/std/parallel/lock.h>
#include <GenAIFramework/Communication/AIModelRequestBus.h>
#include <GenAIFramework/Communication/AIServiceProviderBus.h>
#include <GenAIFramework/Communication/AsyncRequestBus.h>
#include <GenAIFramework/Feature/ConversationBus.h>
#include <GenAIFramework/GenAIFrameworkBus.h>
#include <QMessageBox>
#include <QScrollBar>
#include <QSettings>
#include <QStyle>
#include <Source/UI/ui_AIChatWidget.h>

namespace GenAIFramework
{
    AIChatWidget::AIChatWidget(QWidget* parent, QString modelName, QString providerName, QString featureName)
        : QWidget(parent)
        , m_ui(new Ui::AIChatWidgetUI)

    {
        auto featureIdOutcome = GenAIFrameworkInterface::Get()->CreateNewFeatureConversation(
            providerName.toStdString().c_str(), modelName.toStdString().c_str(), featureName.toStdString().c_str());
        m_featureId = featureIdOutcome.IsSuccess() ? featureIdOutcome.GetValue() : 0;

        ConversationNotificationBus::Handler::BusConnect(m_featureId);
        AZ::TickBus::Handler::BusConnect();

        m_ui->setupUi(this);

        QString description = QString("Model: %1 Provider: %2").arg(modelName, providerName);
        m_ui->configDescription->setText(description);

        QStyle* style = qApp->style();
        QIcon closeIcon = style->standardIcon(QStyle::SP_TitleBarCloseButton);
        m_ui->closeButton->setIcon(closeIcon);

        m_uiChatLayout = new QVBoxLayout();
        m_uiChatLayout->setAlignment(Qt::AlignBottom);
        m_ui->scrollArea->setWidgetResizable(true);
        m_ui->scrollArea->widget()->setLayout(m_uiChatLayout);
        m_ui->scrollArea->setStyleSheet("QLabel { border-radius: 15px; padding: 15px; }");
        m_ui->scrollArea->verticalScrollBar()->connect(
            m_ui->scrollArea->verticalScrollBar(),
            &QScrollBar::rangeChanged,
            [this]()
            {
                m_ui->scrollArea->verticalScrollBar()->setValue(m_ui->scrollArea->verticalScrollBar()->maximum());
            });
        connect(m_ui->SendBtn, &QPushButton::clicked, this, &AIChatWidget::OnRequestButton);
        connect(m_ui->closeButton, &QPushButton::clicked, this, &AIChatWidget::OnCloseButton);
    }

    AIChatWidget::~AIChatWidget()
    {
        if (ConversationNotificationBus::Handler::BusIsConnected())
        {
            ConversationNotificationBus::Handler::BusDisconnect();
        }
        if (AZ::TickBus::Handler::BusIsConnected())
        {
            AZ::TickBus::Handler::BusDisconnect();
        }
        GenAIFrameworkInterface::Get()->RemoveFeatureConversation(m_featureId);
    }

    void AIChatWidget::OnRequestButton()
    {
        AZStd::string modelInput = m_ui->textEdit->toPlainText().toStdString().c_str();
        UiAppendChatMessage(modelInput);
        ConversationNotificationBus::Event(m_featureId, &ConversationNotificationBus::Events::OnNewMessage, modelInput);
    }

    void AIChatWidget::OnFeatureResponse(const AZStd::string& summary, const AZStd::vector<AZStd::string>& detailedResponse)
    {
        AZStd::lock_guard<AZStd::mutex> lock(m_chatMessagesQueueMutex);
        m_chatMessagesQueue.push({ { summary, detailedResponse }, true });
    }

    // This on tick function is required due to the fact that updating the QT UI must be done on the main thread
    // The OnFeatureResponse function can be called from any thread, so messages need to be queued
    void AIChatWidget::OnTick(float deltaTime, AZ::ScriptTimePoint time)
    {
        AZ_UNUSED(deltaTime);
        AZ_UNUSED(time);
        if (!m_chatMessagesQueue.empty())
        {
            AZStd::lock_guard<AZStd::mutex> lock(m_chatMessagesQueueMutex);
            auto message = m_chatMessagesQueue.front();
            UiAppendChatMessage(message.first.first, message.second);
            m_chatMessagesQueue.pop();
        }
    }

    void AIChatWidget::OnCloseButton()
    {
        emit chatClosed();
    }

    void AIChatWidget::UiAppendChatMessage(const AZStd::string& message, const bool response)
    {
        auto label = new QLabel(message.c_str());
        label->setWordWrap(true);
        label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

        if (response)
        {
            label->setStyleSheet("QLabel { background-color: #303030; margin-right: 100px; }");
        }
        else
        {
            label->setStyleSheet("QLabel { background-color: #202020; margin-left: 100px; }");
        }

        m_uiChatLayout->addWidget(label);
    }

    void AIChatWidget::UiClearMessages()
    {
        QLayoutItem* item;
        while ((item = m_uiChatLayout->takeAt(0)) != nullptr)
        {
            delete item->widget();
            delete item;
        }
    }

} // namespace GenAIFramework
