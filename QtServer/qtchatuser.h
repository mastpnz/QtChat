#pragma once
#include <QString>
#include <QMap>
#include <QCryptographicHash>
#include <QDateTime>
enum UsersRole {
    admin = 501,
    user,
    moderator
};

class QtChatUser
{
public:
    QtChatUser(QString name,
               QString login,
               int role = UsersRole::user,
               QByteArray hashpass = nullptr,
               bool needChangePassFlag = false,
               QDateTime createdDateTime = QDateTime(QDate(1988,06,29),QTime(21,30)),
               QDateTime lastModifyDateTime = QDateTime(QDate(1988,06,29),QTime(21,30))
            );
    QString getName() const {return !mName.isEmpty() ? mName : nullptr; };
    QString getLogin() const {return !mLogin.isEmpty() ? mLogin : nullptr; };
    int getRole() const {return mRole > 500 ? mRole : 0; };
    QByteArray getHashPass() const {return !mHashPass.isEmpty() ? mHashPass : nullptr; };
    QByteArray makeHashPass(const QString &pass);
    bool verifyPass(const QByteArray &hash) const;

    void setName(const QString &str) { mName = QString(str); };
    void setLogin(const QString &str) { mLogin = QString(str); };
    void setRole(const int &role) { mRole = role; };
    void setHashPass(const QByteArray &hashpass) { mHashPass = QByteArray(hashpass); };
    void setChangePass(const bool &flag) { mNeedChangePassFlag = flag; } ;
    void setCreateDateTime(const QDateTime &datetime) { mCreatedDateTime = datetime; };
    void setModifyDateTime(const QDateTime &datetime) { mLastModifyDateTime = datetime; };
private:
    QString mName;
    QString mLogin;
    int mRole;
    QByteArray mHashPass;
    bool mNeedChangePassFlag;
    QDateTime mCreatedDateTime;
    QDateTime mLastModifyDateTime;
};

