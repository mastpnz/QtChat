#pragma once

#include <QDialog>
#include "qtchatuser.h"
#include <QList>

namespace Ui {
class DialogUserManage;
}

class DialogUserManage : public QDialog
{
    Q_OBJECT

public:
    explicit DialogUserManage(QList<QtChatUser> *users, QWidget *parent = nullptr);
    ~DialogUserManage();
    void refreshTable(QList<QtChatUser> *users);
signals:
    void newUserAdded(QtChatUser newUser);
    void updateUser(QtChatUser newUser);
    void deleteUser(QtChatUser delUser);

private slots:
    void on_btnAddUser_clicked();

    void on_tableWidget_cellDoubleClicked(int row, int column);

    void on_tableWidget_itemSelectionChanged();

    void on_btnDelUser_clicked();

private:
    Ui::DialogUserManage *ui;
    QList<QtChatUser> *mUsers;
};

