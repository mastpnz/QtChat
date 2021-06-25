#include <QCoreApplication>
#include <iostream>
#include "../QtServer/chatserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    std::cout << "QtChatServer console version." << std::endl;
    std::cout << "To start use: qtchatserverconsole --remotedb --dbsrvaddress=<address remote db server> --dbuser=<user name for db> --dbpass=<password for db user> --dbname=<name of db> --port=<srv port>";
    std::cout << "To start use: qtchatserverconsole --localdb --dbuser=<user name for db> --dbpass=<password for db user> --dbname=<name of db> --port=<srv port";
    std::cout << std::endl;

    ServerSettings srvSet;

    if(QCoreApplication::arguments().contains("--remotedb",Qt::CaseInsensitive)) {
        srvSet.dbtype = 1;
    }
    else if(QCoreApplication::arguments().contains("--localdb",Qt::CaseInsensitive)) {
        srvSet.dbtype = 0;
    }
    else
    {
        std::cout << "QtChatServer console version." << std::endl;
        std::cout << "To start use: qtchatserverconsole --remotedb --dbsrvaddress=<address remote db server> --dbuser=<user name for db> --dbpass=<password for db user> --dbname=<name of db>" << std::endl;
        std::cout << "To start use: qtchatserverconsole --localdb --dbuser=<user name for db> --dbpass=<password for db user> --dbname=<name of db>";
        std::cout << std::endl << "No found --remotedb or --localdb" << std::endl;
        return 0;
    }

    foreach(QString mArg, QCoreApplication::arguments())
    {
        if(mArg.contains("--dbsrvaddress=")){
            srvSet.address = mArg.remove("--dbsrvaddress=");
        }
        if(mArg.contains("--dbuser=")){
            srvSet.dbuser = mArg.remove("--dbuser=");
        }
        if(mArg.contains("--dbpass=")){
            srvSet.dbpass = mArg.remove("--dbpass=");
        }
        if(mArg.contains("--dbname=")){
            srvSet.dbname = mArg.remove("--dbname=");
        }
        if(mArg.contains("--port=")){
            srvSet.port = mArg.remove("--port=").toInt();
        }
    }

    if(srvSet.address.isEmpty() && srvSet.isDbRemote()) {
        std::cout << "Remote address of data base not set" << std::endl;
        return 0;
    }
    if(srvSet.dbuser.isEmpty()) {
        std::cout << "Data base user not set" << std::endl;
        return 0;
    }
    if(srvSet.dbpass.isEmpty()) {
        std::cout << "Data base pass not set" << std::endl;
        return 0;
    }
    if(srvSet.dbname.isEmpty()) {
        std::cout << "Data base name not set" << std::endl;
        return 0;
    }
    if(srvSet.port == 0) {
        std::cout << "Port for server not set" << std::endl;
        return 0;
    }

    ChatServer *m_chatServer;
    m_chatServer = new ChatServer();
    m_chatServer->setDBSettings(&srvSet);
    m_chatServer->connectToDB();
    if (!m_chatServer->listen(QHostAddress::Any, m_srvSettings.port)) {
        std::cout << "Start server fail!" << std::endl;
    }
    return a.exec();
}
