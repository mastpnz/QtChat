#include "startserverdialog.h"
#include "ui_startserverdialog.h"
#include <QPointer>

StartServerDialog::StartServerDialog(ServerSettings &srvSet, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartServerDialog)
{
    ui->setupUi(this);
    m_srvSet = &srvSet;
    ui->localPortLineEdit->setText(QString::number(srvSet.port,10));
    ui->addressLineEdit->setText(srvSet.address);
    ui->dbanameLineEdit->setText(srvSet.dbname);
    ui->dbuserLineEdit->setText(srvSet.dbuser);
    ui->dbpasswordLineEdit->setText(srvSet.dbpass);
//    ui->addressLineEdit->setText("109.194.141.141");
    ui->addressLineEdit->setText("lookaw.ru");
    ui->dbanameLineEdit->setText("dbRuscontol");
    ui->dbuserLineEdit->setText("dbuser");
    ui->dbpasswordLineEdit->setText("passwordfordbuser1488");
}

StartServerDialog::~StartServerDialog()
{
    delete ui;
}

void StartServerDialog::on_buttonBox_accepted()
{
    m_srvSet->port = ui->localPortLineEdit->text().toInt(nullptr,10);
    m_srvSet->address = ui->addressLineEdit->text();
    m_srvSet->dbname = ui->dbanameLineEdit->text();
    m_srvSet->dbuser = ui->dbuserLineEdit->text();
    m_srvSet->dbpass = ui->dbpasswordLineEdit->text();
    emit accept();
}


void StartServerDialog::on_buttonBox_rejected()
{
    emit reject();
}

