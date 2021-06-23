#include "qtchatuser.h"

QtChatUser::QtChatUser(QString name, QString login, int role, QByteArray hashpass, bool needChangePassFlag, QDateTime createdDateTime, QDateTime lastModifyDateTime)
{
    mName = name;
    mLogin = login;
    mRole = role;
    mHashPass = hashpass;
    mNeedChangePassFlag = needChangePassFlag;
    mCreatedDateTime = createdDateTime;
    mLastModifyDateTime = lastModifyDateTime;
}

QByteArray QtChatUser::makeHashPass(const QString &pass)
{
    if(!pass.isEmpty()){
        mHashPass = QCryptographicHash::hash(pass.toUtf8(),QCryptographicHash::Sha512);
    }
    return mHashPass;
}

bool QtChatUser::verifyPass(const QByteArray &hash) const
{
    return mHashPass == hash? true : false;
}
