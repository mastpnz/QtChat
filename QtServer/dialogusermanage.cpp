#include "dialogusermanage.h"
#include "ui_dialogusermanage.h"
#include "dialogaddmodifyuser.h"
#include <QDebug>
#include <QMenu>
#include <QMessageBox>

DialogUserManage::DialogUserManage(QList<QtChatUser> *users,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogUserManage)
{
    ui->setupUi(this);
    mUsers = users;
    ui->tableWidget->insertColumn(ui->tableWidget->columnCount());
    ui->tableWidget->insertColumn(ui->tableWidget->columnCount());
    ui->btnDelUser->setDisabled(true);
    refreshTable(users);
}

DialogUserManage::~DialogUserManage()
{
    delete ui;
}

void DialogUserManage::refreshTable(QList<QtChatUser> *users)
{
    if(users->empty())
        return;
    mUsers = users;
    ui->tableWidget->clear();
    ui->tableWidget->clearContents();
    while(ui->tableWidget->rowCount()>0)
        ui->tableWidget->removeRow(0);

    for(int i = 0; i<users->count();i++){
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,0,new QTableWidgetItem(users->at(i).getLogin()));
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,1,new QTableWidgetItem(users->at(i).getName()));
    }
}

void DialogUserManage::on_btnAddUser_clicked()
{
    QtChatUser mNewUser("","");
    DialogAddModifyUser dlgAddUser(&mNewUser);
    if(dlgAddUser.exec() == DialogAddModifyUser::Accepted){
        emit newUserAdded(mNewUser);
    }
}


void DialogUserManage::on_tableWidget_cellDoubleClicked(int row, int column)
{
    DialogAddModifyUser dlgAddUser((QtChatUser*)&mUsers->at(row),this);
    if(dlgAddUser.exec() == DialogAddModifyUser::Accepted){
        emit updateUser(mUsers->at(row));
    }
}


void DialogUserManage::on_tableWidget_itemSelectionChanged()
{
    if(ui->tableWidget->currentRow()>=0)
        ui->btnDelUser->setEnabled(true);
    else
        ui->btnDelUser->setDisabled(true);
}


void DialogUserManage::on_btnDelUser_clicked()
{
    if(QMessageBox::question(this,"Удаление пользователя",QString("Удалить пользователя %1(%2)")
                             .arg(mUsers->at(ui->tableWidget->currentRow()).getName())
                             .arg(mUsers->at(ui->tableWidget->currentRow()).getLogin())) == QMessageBox::Yes )
        emit deleteUser(mUsers->at(ui->tableWidget->currentRow()));
}

