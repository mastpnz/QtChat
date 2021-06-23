#include "chatserver.h"
#include "serverworker.h"
#include <QThread>
#include <functional>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QTimer>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "qtchatuser.h"
#include <QCryptographicHash>
#include <QString>
#include <QSqlError>
#include <QDateTime>

ChatServer::ChatServer(QObject *parent)
    : QTcpServer(parent)
{
    db = QSqlDatabase::addDatabase("QMYSQL");
}

ChatServer::~ChatServer()
{
}

void ChatServer::incomingConnection(qintptr socketDescriptor)
{
    ServerWorker *worker = new ServerWorker(this);
    if (!worker->setSocketDescriptor(socketDescriptor)) {
        worker->deleteLater();
        return;
    }
    connect(worker, &ServerWorker::disconnectedFromClient, this, std::bind(&ChatServer::userDisconnected, this, worker));
    connect(worker, &ServerWorker::error, this, std::bind(&ChatServer::userError, this, worker));
    connect(worker, &ServerWorker::jsonReceived, this, std::bind(&ChatServer::jsonReceived, this, worker, std::placeholders::_1));
    connect(worker, &ServerWorker::logMessage, this, &ChatServer::logMessage);
    m_clients.append(worker);
    emit logMessage(tr("Подключён новый клиент"));
}

void ChatServer::sendJson(ServerWorker *destination, const QJsonObject &message)
{
    Q_ASSERT(destination);
    destination->sendJson(message);
}

void ChatServer::broadcast(const QJsonObject &message, ServerWorker *exclude)
{
    for (ServerWorker *worker : m_clients) {
        Q_ASSERT(worker);
        if (worker == exclude)
            continue;
        sendJson(worker, message);
    }
}

void ChatServer::jsonReceived(ServerWorker *sender, const QJsonObject &doc)
{
    Q_ASSERT(sender);
    emit logMessage(QLatin1String("Получил JSON ") + QString::fromUtf8(QJsonDocument(doc).toJson()));
    if (sender->userName().isEmpty())
        return jsonFromLoggedOut(sender, doc);
    jsonFromLoggedIn(sender, doc);
}

void ChatServer::userDisconnected(ServerWorker *sender)
{
    m_clients.removeAll(sender);
    const QString userName = sender->userName();
    if (!userName.isEmpty()) {
        QJsonObject disconnectedMessage;
        disconnectedMessage[QStringLiteral("type")] = QStringLiteral("userdisconnected");
        disconnectedMessage[QStringLiteral("username")] = userName;
        broadcast(disconnectedMessage, nullptr);
        emit logMessage(userName + QLatin1String(" disconnected"));
    }
    sender->deleteLater();
}

void ChatServer::userError(ServerWorker *sender)
{
    Q_UNUSED(sender)
    emit logMessage(QLatin1String("Ошибка из-за ") + sender->userName());
}

void ChatServer::stopServer()
{
    for (ServerWorker *worker : m_clients) {
        worker->disconnectFromClient();
    }
    close();
}

bool ChatServer::connectToDB()
{
    db.setHostName(m_srvSet->address);
    db.setUserName(m_srvSet->dbuser);
    db.setPassword(m_srvSet->dbpass);
    if (!db.open()) {
        qDebug() << "Cannot open database";
        qDebug() << "Unable to establish a database connection.";
        return false;
    }
    return true;
}

void ChatServer::closeConnectionDB()
{
    db.close();
}

bool ChatServer::createNewUser(QtChatUser newUser)
{
    QSqlQuery query;
    query.prepare("insert into dbRuscontrol.users(login,name,passhash,role,needchangepass,createddatetime,lastchanged) values(?,?,?,?,?,?,?)");
    query.bindValue(0, newUser.getLogin());
    query.bindValue(1, newUser.getName());
    query.bindValue(2, QString::fromUtf8(newUser.getHashPass()));
    query.bindValue(3, newUser.getRole());
    query.bindValue(4, false);
    query.bindValue(5, QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    query.bindValue(6, QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    return query.exec();
}

bool ChatServer::updateUser(QtChatUser upUser)
{
    QSqlQuery query;
    query.prepare(QString("UPDATE dbRuscontrol.users SET name=:name,role=:role,needchangepass=:ncp,lastchanged=:lchange,passhash=:phash WHERE login=:login;"));
    query.bindValue(":name", upUser.getName());
    query.bindValue(":phash", QString::fromUtf8(upUser.getHashPass()));
    query.bindValue(":role", upUser.getRole());
    query.bindValue(":ncp", false);
    query.bindValue(":lchange", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    query.bindValue(":login", upUser.getLogin());
    return query.exec();
}

bool ChatServer::deleteUser(QtChatUser delUser)
{
    QSqlQuery query;
    query.prepare(QString("DELETE FROM dbRuscontrol.users WHERE login=:login;"));
    query.bindValue(":login", delUser.getLogin());
    return query.exec();
}

bool ChatServer::getDbUsers(QList<QtChatUser> *dbUsers)
{
    Q_ASSERT(dbUsers);
    dbUsers->clear();
     QSqlQuery query;
     query.exec("SELECT login,name,passhash,role,createddatetime,lastchanged FROM dbRuscontrol.users");
     qDebug() << query.lastError().text();
     while (query.next()) {
         QtChatUser user(query.value(1).toString(),
                             query.value(0).toString(),
                             query.value(3).toInt(),
                             query.value(2).toByteArray(),
                             false,
                             query.value(4).toDateTime(),
                             query.value(5).toDateTime());
            dbUsers->append(user);
         }
     return true;
}

bool ChatServer::checkPass(QString userLogin, QByteArray userPass)
{
    QSqlQuery query;
    query.prepare("SELECT passhash FROM dbRuscontrol.users WHERE login=:login;");
    query.bindValue(":login", userLogin);
    query.exec();
    while (query.next()) {
        if(userPass == query.value(0).toByteArray().left(userPass.size()))
            return true;
    }
    return false;
}

void ChatServer::jsonFromLoggedOut(ServerWorker *sender, const QJsonObject &docObj)
{
    Q_ASSERT(sender);
    const QJsonValue typeVal = docObj.value(QLatin1String("type"));
    if (typeVal.isNull() || !typeVal.isString())
        return;
    if (typeVal.toString().compare(QLatin1String("login"), Qt::CaseInsensitive) != 0)
        return;
    const QJsonValue usernameVal = docObj.value(QLatin1String("username"));
    if (usernameVal.isNull() || !usernameVal.isString())
        return;
    const QString newUserName = usernameVal.toString().simplified();
    if (newUserName.isEmpty())
        return;
    const QJsonValue userpassVal = docObj.value(QLatin1String("userpass"));
    if (usernameVal.isNull() || !usernameVal.isString())
        return;
    const QByteArray newUserPass = userpassVal.toString().toUtf8();
    if (newUserPass.isEmpty())
        return;

    if(!checkPass(newUserName,newUserPass)){
        QJsonObject message;
        message[QStringLiteral("type")] = QStringLiteral("login");
        message[QStringLiteral("success")] = false;
        message[QStringLiteral("reason")] = QStringLiteral("wrong password");
        sendJson(sender, message);
        return;
    }

    for (ServerWorker *worker : qAsConst(m_clients)) {
        if (worker == sender)
            continue;
        if (worker->userName().compare(newUserName, Qt::CaseInsensitive) == 0) {
            QJsonObject message;
            message[QStringLiteral("type")] = QStringLiteral("login");
            message[QStringLiteral("success")] = false;
            message[QStringLiteral("reason")] = QStringLiteral("duplicate username");
            sendJson(sender, message);
            return;
        }
    }
    sender->setUserName(newUserName);
    QJsonObject successMessage;
    successMessage[QStringLiteral("type")] = QStringLiteral("login");
    successMessage[QStringLiteral("success")] = true;
    sendJson(sender, successMessage);
    QJsonObject connectedMessage;
    connectedMessage[QStringLiteral("type")] = QStringLiteral("newuser");
    connectedMessage[QStringLiteral("username")] = newUserName;
    broadcast(connectedMessage, sender);
}

void ChatServer::jsonFromLoggedIn(ServerWorker *sender, const QJsonObject &docObj)
{
    Q_ASSERT(sender);
    const QJsonValue typeVal = docObj.value(QLatin1String("type"));
    if (typeVal.isNull() || !typeVal.isString())
        return;
    if (typeVal.toString().compare(QLatin1String("message"), Qt::CaseInsensitive) != 0)
        return;
    const QJsonValue textVal = docObj.value(QLatin1String("text"));
    if (textVal.isNull() || !textVal.isString())
        return;
    const QString text = textVal.toString().trimmed();
    if (text.isEmpty())
        return;
    QJsonObject message;
    message[QStringLiteral("type")] = QStringLiteral("message");
    message[QStringLiteral("text")] = text;
    message[QStringLiteral("sender")] = sender->userName();
    broadcast(message, sender);
}
