/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root
 * of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "AIChatWidget.h"
#include "AIChatMessage.h"
#include <AzCore/Component/Entity.h>
#include <AzCore/Component/TickBus.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/std/containers/vector.h>

#include <AzCore/Component/ComponentApplicationBus.h>
#include <GenAIFramework/Communication/AIModelRequestBus.h>
#include <GenAIFramework/Communication/AIServiceProviderBus.h>
#include <GenAIFramework/Communication/AsyncRequestBus.h>
#include <GenAIFramework/GenAIFrameworkBus.h>
#include <QMessageBox>
#include <QScrollBar>
#include <QSettings>
#include <QStyle>
#include <Source/UI/ui_AIChatWidget.h>

namespace GenAIFramework
{
    AIChatWidget::AIChatWidget(QWidget* parent, QString modelName, QString providerName)
        : QWidget(parent)
        , m_ui(new Ui::AIChatWidgetUI)

    {
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
        // m_ui->scrollArea->setStyleSheet("QLabel { border-radius: 15px; padding: 15px; }");
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

    void AIChatWidget::OnRequestButton()
    {
        AZStd::string modelInput = m_ui->textEdit->toPlainText().toStdString().c_str();
        UiAppendChatMessage(modelInput);
        constexpr bool isAssistantReply = true;
        UiAppendChatMessage("This PoC implementation is not connected to any assistant", isAssistantReply);
    }

    void AIChatWidget::OnCloseButton()
    {
        emit chatClosed();
    }

    void AIChatWidget::UiAppendChatMessage(const AZStd::string& message, const bool response)
    {
        AIChatMessage * chatMessage;
        AZStd::vector<AZStd::string> details;
        details.push_back("Detail 1");
        details.push_back("Detail 2");
        if(response)
        {
          chatMessage = new AIChatMessage(this, message, details, response);
        }else{
          chatMessage = new AIChatMessage(this, message, {}, response);
        }
          m_uiChatLayout->addWidget(chatMessage);
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
