#pragma once

#include <QDialog>
struct ServerSettings {
    quint16 port;
    QString address;
    QString dbname;
    QString dbuser;
    QString dbpass;
};

namespace Ui {
class StartServerDialog;
}

class StartServerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StartServerDialog(ServerSettings &srvSet, QWidget *parent = nullptr);
    ~StartServerDialog();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::StartServerDialog *ui;
    ServerSettings *m_srvSet;
};

