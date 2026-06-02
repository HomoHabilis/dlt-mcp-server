/* Copyright (C) 2026, Mikhail Nikonov <michael.n.nikonov@gmail.com>
 *
 * This Source Code Form is subject to the terms of the
 * Mozilla Public License (MPL), v. 2.0.
 * If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef DASHBOARD_H
#define DASHBOARD_H

#include "config.h"
#include <QWidget>

class QLabel;
class QPushButton;
class QSettings;

class Dashboard : public QWidget {
    Q_OBJECT

  public:
    explicit Dashboard(QWidget* parent = nullptr);
    ~Dashboard() override = default;

    void setSettings(QSettings* settings);

  signals:
    void openSettings();

  protected:
    void showEvent(QShowEvent* event) override;
    void timerEvent(QTimerEvent* ev) override;

  private:
    void updateContextWarning();

    QLabel* statusLabel_;
    QLabel* contextWarningLabel_;
    QPushButton* sseCopyBtn_;
    QPushButton* httpCopyBtn_;
    QPushButton* settingsBtn_;
    QSettings* settings_ = nullptr;
    int port_ = DefaultPort;
    int restoreTimerId_ = 0;
    QPushButton* pressedBtn_ = nullptr;
    QString originalText_;
};

#endif // DASHBOARD_H
