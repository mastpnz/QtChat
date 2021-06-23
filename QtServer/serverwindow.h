#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QWidget>
#include "startserverdialog.h"
#include "qtchatuser.h"
#include "dialogusermanage.h"

namespace Ui {
class ServerWindow;
}
class ChatServer;
class ServerWindow : public QWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(ServerWindow)
public:
    explicit ServerWindow(QWidget *parent = nullptr);
    ~ServerWindow();
    ServerSettings m_srvSettings;

private:
    Ui::ServerWindow *ui;
    ChatServer *m_chatServer;
    QList<QtChatUser> users;
    DialogUserManage *dlgUserManage;

public slots:
    void addNewUserToDB(QtChatUser newUser);
    void updateUserToDB(QtChatUser upUser);
    void deleteUserFromDB(QtChatUser delUser);

private slots:
    void toggleStartServer();
    void logMessage(const QString &msg);
    void on_btnMakeCommand_clicked();
    void on_btnManageUsers_clicked();
};

#endif // SERVERWINDOW_H
