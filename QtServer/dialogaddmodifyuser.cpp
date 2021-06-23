#include "dialogaddmodifyuser.h"
#include "ui_dialogaddmodifyuser.h"
#include <QMessageBox>
#include <QRegExpValidator>

DialogAddModifyUser::DialogAddModifyUser(QtChatUser *user, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAddModifyUser)
{
    ui->setupUi(this);
    mUser = user;
    ui->PassLine->setDisabled(true);
    if(user->getLogin().isEmpty()) {
        ui->NameLine->clear();
        ui->LoginLine->clear();
        ui->PassLine->clear();
    }
    else
    {
        ui->LoginLine->setDisabled(true);
        ui->NameLine->setText(user->getName());
        ui->LoginLine->setText(user->getLogin());
    }
}

DialogAddModifyUser::~DialogAddModifyUser()
{
    delete ui;
}

void DialogAddModifyUser::on_buttonBox_accepted()
{
    mUser->setName(ui->NameLine->text());
    mUser->setLogin(ui->LoginLine->text());
    if(ui->checkBox->isChecked())
        mUser->setHashPass(mUser->makeHashPass(ui->PassLine->text()));
}

void DialogAddModifyUser::on_checkBox_stateChanged(int arg1)
{
    if(ui->checkBox->isChecked())
        ui->PassLine->setEnabled(true);
    else
        ui->PassLine->setDisabled(true);
}
