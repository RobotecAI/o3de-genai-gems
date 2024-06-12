/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root
 * of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "AIChatMessage.h"
#include <AzCore/std/containers/vector.h>
#include <AzCore/std/string/string.h>

#if !defined(Q_MOC_RUN)
#include <QLabel>
#include <QWidget>
#include <QPushButton>
#include <QVector>
#include <QPainter>
#endif

namespace GenAIFramework
{
    AIChatMessage::AIChatMessage(QWidget* parent, const AZStd::string& message, const AZStd::vector<AZStd::string>& details, bool isAssistantReply)
        : QWidget(parent)
        , message(message)
        , details(details)
        , isAssistantReply(isAssistantReply)

    {

        m_layout = new QVBoxLayout();
        setStyleSheet("");
        if( isAssistantReply ){
            m_layout->setAlignment(Qt::AlignLeft);
            auto msg_label = new QLabel(message.c_str());
            setStyleSheet("border-radius: 15px; padding: 15px; background-color: #303030");
            m_layout->addWidget(msg_label);
        } else {
            m_layout->setAlignment(Qt::AlignRight);
            auto msg_label = new QLabel(message.c_str());
            setStyleSheet("border-radius: 15px; padding: 15px; background-color: #202020");
            m_layout->addWidget(msg_label);
        }

        for (const auto& detail : details)
        {
            auto detail_label = new QLabel(detail.c_str());
            m_layout->addWidget(detail_label);
            detail_labels.push_back(detail_label);
            detail_label->hide();
        }

        details_button = new QPushButton("Details");
        details_button->setStyleSheet("max-width: 100px; min-width: 100px; padding: 0px; margin: 5px;");
        m_layout->addWidget(details_button);
        show_details = false;
        setLayout(m_layout);
        connect(details_button, &QPushButton::clicked, this, &AIChatMessage::OnDetailsButton);
        show();
    }

    void AIChatMessage::OnDetailsButton()
    {
      show_details = !show_details;
      if (show_details){
        for (auto detail_label : detail_labels)
        {
          detail_label->show();
        }
        details_button->setText("Hide");
      }else{
        for (auto detail_label : detail_labels)
        {
          detail_label->hide();
        }
        details_button->setText("Details");
      }
    }

    AIChatMessage::~AIChatMessage()
    {
    }

    void AIChatMessage::paintEvent (QPaintEvent *)
    {
        QStyleOption opt;
        opt.init(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }
}
