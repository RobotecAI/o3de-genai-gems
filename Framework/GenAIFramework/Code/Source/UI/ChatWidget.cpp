/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root
 * of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "ChatWidget.h"
#include <GenAIFramework/Communication/AIModelRequestBus.h>
#include <GenAIFramework/Communication/AIServiceProviderBus.h>
#include <GenAIFramework/Communication/AsyncRequestBus.h>
#include <GenAIFramework/Feature/ConversationBus.h>
#include <GenAIFramework/GenAIFrameworkBus.h>
#include <UI/DetailsWidget.h>

#include <AzCore/Component/ComponentApplicationBus.h>
#include <AzCore/Component/Entity.h>
#include <AzCore/Component/TickBus.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/std/containers/vector.h>
#include <AzCore/std/parallel/lock.h>
#include <AzToolsFramework/UI/UICore/WidgetHelpers.h>

#include <QMessageBox>
#include <QPushButton>
#include <QScrollBar>
#include <QSettings>
#include <QStyle>

#include <Source/UI/ui_ChatWidget.h>

namespace GenAIFramework
{
    ChatWidget::ChatWidget(QWidget* parent, QString modelName, QString providerName, QString featureName)
        : QWidget(parent)
        , m_ui(new Ui::ChatWidgetUI)

    {
        m_ui->setupUi(this);

        const QString description = QString("Feature: %1 Model: %2 Provider: %3").arg(featureName, modelName, providerName);
        m_ui->configDescription->setText(description);

        QStyle* style = qApp->style();
        m_ui->closeButton->setIcon(style->standardIcon(QStyle::SP_TitleBarCloseButton));

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
        connect(m_ui->SendBtn, &QPushButton::clicked, this, &ChatWidget::OnRequestButton);
        connect(m_ui->closeButton, &QPushButton::clicked, this, &ChatWidget::OnCloseButton);

        auto featureIdOutcome = GenAIFrameworkInterface::Get()->CreateNewFeatureConversation(
            providerName.toStdString().c_str(), modelName.toStdString().c_str(), featureName.toStdString().c_str());

        if (featureIdOutcome.IsSuccess())
        {
            m_featureId = featureIdOutcome.GetValue();
            ConversationNotificationBus::Handler::BusConnect(m_featureId);
            AZ::TickBus::Handler::BusConnect();
        }
        else
        {
            AZ::SystemTickBus::QueueFunction(
                [=]()
                {
                    QMessageBox::warning(
                        AzToolsFramework::GetActiveWindow(),
                        "ChatWidget",
                        QString(tr("Failed to connect to AI Feature.")),
                        QMessageBox::Ok);
                });
        }
    }

    ChatWidget::~ChatWidget()
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

    void ChatWidget::OnDetailsButton()
    {
        QPushButton* buttonSender = qobject_cast<QPushButton*>(sender());
        AZ_Assert(buttonSender != nullptr, "Cannot open DetailsWidget: event sender not known");
        if (m_chatDetails.contains(buttonSender))
        {
            DetailsWidget* details = new DetailsWidget(m_chatDetails[buttonSender], this);
            details->show();
        }
        else
        {
            AZ_Warning("ChatWidget", false, "Detailed chat message not available.");
        }
    }

    void ChatWidget::OnRequestButton()
    {
        // Prevent sending a new message if the previous one is still being processed
        if (m_isMessageBeingProcessed)
        {
            return;
        }
        // Only disable the button and send the prompt if there is a handler that can enable it later
        if (ConversationNotificationBus::HasHandlers(m_featureId))
        {
            m_isMessageBeingProcessed = true;
            m_ui->SendBtn->setEnabled(false);

            AZStd::string modelInput = m_ui->textEdit->toPlainText().toStdString().c_str();
            m_ui->textEdit->clear();
            UiAppendChatMessage({ modelInput, AZStd::vector<AZStd::string>() });
            ConversationNotificationBus::Event(m_featureId, &ConversationNotificationBus::Events::OnNewMessage, modelInput);
        }
        else
        {
            QMessageBox::warning(
                AzToolsFramework::GetActiveWindow(),
                "AIChatWidget",
                QString("Error: The currently selected models are not connected to the EBuses"),
                QMessageBox::Ok);
            return;
        }
    }

    void ChatWidget::OnFeatureResponse(const AZStd::string& summary, const AZStd::vector<AZStd::string>& detailedResponse)
    {
        AZStd::lock_guard<AZStd::mutex> lock(m_chatMessagesQueueMutex);
        m_chatMessagesQueue.push({ { summary, detailedResponse }, true });
        m_isMessageBeingProcessed = false;
        m_ui->SendBtn->setEnabled(true);
    }

    // This on tick function is required due to the fact that updating the QT UI must be done on the main thread
    // The OnFeatureResponse function can be called from any thread, so messages need to be queued
    void ChatWidget::OnTick(float deltaTime, AZ::ScriptTimePoint time)
    {
        AZ_UNUSED(deltaTime);
        AZ_UNUSED(time);
        if (!m_chatMessagesQueue.empty())
        {
            AZStd::lock_guard<AZStd::mutex> lock(m_chatMessagesQueueMutex);
            auto& message = m_chatMessagesQueue.front();
            UiAppendChatMessage(message.first, message.second);
            m_chatMessagesQueue.pop();
        }
    }

    void ChatWidget::OnCloseButton()
    {
        emit chatClosed();
    }

    void ChatWidget::UiAppendChatMessage(const SummaryDetailedPair& message, const bool response)
    {
        auto label = new QLabel(message.first.c_str());
        label->setWordWrap(true);
        label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

        if (response)
        {
            label->setStyleSheet("QLabel { background-color: #303030; margin-right: 100px; }");
            m_uiChatLayout->addWidget(label);
            if (!message.second.empty())
            {
                QPushButton* detailsButton = new QPushButton("?");
                detailsButton->setFixedSize(26, 24);
                m_uiChatLayout->addWidget(detailsButton);
                m_chatDetails[detailsButton] = AZStd::move(message.second);
                connect(detailsButton, &QPushButton::clicked, this, &ChatWidget::OnDetailsButton);
            }
        }
        else
        {
            label->setStyleSheet("QLabel { background-color: #202020; margin-left: 100px; }");
            m_uiChatLayout->addWidget(label);
        }
    }

    void ChatWidget::UiClearMessages()
    {
        QLayoutItem* item;
        while ((item = m_uiChatLayout->takeAt(0)) != nullptr)
        {
            delete item->widget();
            delete item;
        }
    }

} // namespace GenAIFramework
