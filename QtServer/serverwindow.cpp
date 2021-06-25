#include "serverwindow.h"
#include "ui_serverwindow.h"
#include "chatserver.h"
#include <QMessageBox>
#include <QDebug>
ServerWindow::ServerWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ServerWindow)
    , m_chatServer(new ChatServer(this))
    , dlgUserManage(new DialogUserManage(&users,this))
{
    ui->setupUi(this);
    ui->btnManageUsers->setDisabled(true);
    m_srvSettings.port = 3000;
    m_srvSettings.dbname = "";
    m_srvSettings.dbuser = "";
    m_srvSettings.dbpass = "";
    connect(ui->startStopButton, &QPushButton::clicked, this, &ServerWindow::toggleStartServer);
    connect(m_chatServer, &ChatServer::logMessage, this, &ServerWindow::logMessage);
    connect(dlgUserManage,&DialogUserManage::newUserAdded,this,&ServerWindow::addNewUserToDB);
    connect(dlgUserManage,&DialogUserManage::updateUser,this,&ServerWindow::updateUserToDB);
    connect(dlgUserManage,&DialogUserManage::deleteUser,this,&ServerWindow::deleteUserFromDB);
}

ServerWindow::~ServerWindow()
{
    delete ui;
    if(m_chatServer->isListening()){
        m_chatServer->stopServer();
        m_chatServer->closeConnectionDB();
    }
}

void ServerWindow::toggleStartServer()
{
    if (m_chatServer->isListening()) {
        m_chatServer->stopServer();
        m_chatServer->closeConnectionDB();
        ui->btnManageUsers->setDisabled(true);
        ui->startStopButton->setText(tr("Start Server"));
        logMessage(tr("Server Stopped"));
    } else {
        StartServerDialog srvSettingsDlg(m_srvSettings);
        if(srvSettingsDlg.exec() == srvSettingsDlg.Accepted)
        {
            if (!m_chatServer->listen(QHostAddress::Any, m_srvSettings.port)) {
                QMessageBox::critical(this, tr("Error"), tr("Unable to start the server"));
                return;
            }
            m_chatServer->setDBSettings(&m_srvSettings);
            m_chatServer->connectToDB();
            ui->btnManageUsers->setEnabled(true);
            logMessage(tr("Server Started"));
            ui->startStopButton->setText(tr("Stop Server"));
        }
    }
}

void ServerWindow::logMessage(const QString &msg)
{
    ui->logEditor->appendPlainText((msg + QLatin1Char('\n')));
}

void ServerWindow::on_btnMakeCommand_clicked()
{
}

void ServerWindow::on_btnManageUsers_clicked()
{
    if (m_chatServer->isListening()){
        m_chatServer->getDbUsers(&users);
        dlgUserManage->refreshTable(&users);
        dlgUserManage->exec();
    }
}

void ServerWindow::addNewUserToDB(QtChatUser newUser)
{
    if(!m_chatServer->createNewUser(newUser))
        QMessageBox::information(this,"Ошибка","Ошибка добавления пользователя");
    else
    {
        QMessageBox::information(this,"Добавление пользователя",QString("Пользователь %1(%2) добавлен").arg(newUser.getName()).arg(newUser.getLogin()));
    }
    m_chatServer->getDbUsers(&users);
    dlgUserManage->refreshTable(&users);
}

void ServerWindow::updateUserToDB(QtChatUser upUser)
{
    if(!m_chatServer->updateUser(upUser))
        QMessageBox::information(this,"Ошибка","Ошибка обновления профиля пользователя");
    else
    {
        QMessageBox::information(this,"Изменение пользователя",QString("Профиль пользователя %1(%2) обновлен").arg(upUser.getName()).arg(upUser.getLogin()));
    }
    m_chatServer->getDbUsers(&users);
    dlgUserManage->refreshTable(&users);
}

void ServerWindow::deleteUserFromDB(QtChatUser delUser)
{
    if(!m_chatServer->deleteUser(delUser))
        QMessageBox::information(this,"Ошибка","Ошибка удаления профиля пользователя");
    else
    {
        QMessageBox::information(this,"Удаление пользователя",QString("Профиль пользователя %1(%2) удален").arg(delUser.getName()).arg(delUser.getLogin()));
    }
    m_chatServer->getDbUsers(&users);
    dlgUserManage->refreshTable(&users);
}

