#pragma once

#include <QDialog>
#include "qtchatuser.h"

namespace Ui {
class DialogAddModifyUser;
}

class DialogAddModifyUser : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAddModifyUser(QtChatUser *user, QWidget *parent = nullptr);
    ~DialogAddModifyUser();
signals:
    void deleteUser();

private slots:
    void on_buttonBox_accepted();
    void on_checkBox_stateChanged(int arg1);

private:
    Ui::DialogAddModifyUser *ui;
    QtChatUser *mUser;
};

