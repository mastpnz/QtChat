#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QTcpServer>
#include <QList>
#include "startserverdialog.h"
#include <QSqlDatabase>
#include "qtchatuser.h"

class QThread;
class ServerWorker;

class ChatServer : public QTcpServer

{
    Q_OBJECT
    Q_DISABLE_COPY(ChatServer)
public:
    explicit ChatServer(QObject *parent = nullptr);
    ~ChatServer();
    void setDBSettings(ServerSettings *srvSet) {
        m_srvSet = srvSet;
    };
    bool createNewUser(QtChatUser newUser);
    bool updateUser(QtChatUser upUser);
    bool deleteUser(QtChatUser delUser);
    bool getDbUsers(QList<QtChatUser> *dbUsers);
    bool checkPass(QString userName, QByteArray userPass);

protected:
    void incomingConnection(qintptr socketDescriptor) override;
signals:
    void logMessage(const QString &msg);
public slots:
    void stopServer();
    bool connectToDB();
    void closeConnectionDB();
private slots:
    void broadcast(const QJsonObject &message, ServerWorker *exclude);
    void jsonReceived(ServerWorker *sender, const QJsonObject &doc);
    void userDisconnected(ServerWorker *sender);
    void userError(ServerWorker *sender);
private:
    void jsonFromLoggedOut(ServerWorker *sender, const QJsonObject &doc);
    void jsonFromLoggedIn(ServerWorker *sender, const QJsonObject &doc);
    void sendJson(ServerWorker *destination, const QJsonObject &message);
    QVector<ServerWorker *> m_clients;
    ServerSettings *m_srvSet;
    QSqlDatabase db;
};

#endif // CHATSERVER_H
