/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "DetailsWidget.h"
#include <AzCore/Utils/Utils.h>
#include <QCloseEvent>
#include <QVBoxLayout>

namespace GenAIFramework
{
    DetailsWidget::DetailsWidget(const AZStd::vector<AZStd::string>& messageDetails)
    {
        m_details = new QTextEdit();

        int index = 1;
        QString outputText;
        for (const auto& detail : messageDetails)
        {
            outputText += QStringLiteral("## Detail %1\n").arg(index);
            outputText += QString(detail.c_str()) + "\n\n";
            ++index;
        }
        m_details->setMarkdown(outputText);
        m_details->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        m_details->acceptRichText();
        m_details->setReadOnly(true);
        setWindowTitle(tr("Message details"));
        QVBoxLayout* layout = new QVBoxLayout;
        layout->addWidget(m_details);
        this->setLayout(layout);
    }
} // namespace GenAIFramework
