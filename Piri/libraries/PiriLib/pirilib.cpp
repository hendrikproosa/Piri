#include "pirilib.h"


PiriLib::PiriLib()
{
}


QString generateHash(QString hashBase)
{
    QString hashString;
    hashString = QString(QCryptographicHash::hash(hashBase.toLatin1(),QCryptographicHash::Md5).toHex());
    return hashString;
}
