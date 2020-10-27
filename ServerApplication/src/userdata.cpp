#include "userdata.h"

UserData::UserData()
{   }

UserData::UserData(const QString &username)
    : username(username)
{   }


QDataStream& UserData::toStream(QDataStream &stream) const {
    stream << username << password;
    return stream;
}

QDataStream& UserData::fromStream(QDataStream &stream) {
    stream >> username >> password;
    return stream;
}

DataType UserData::type() const {
    return currentType;
}



void UserData::setUsername(const QString &username) {
    this->username = username;
}


void UserData::setPassword(const QString &password) {
    this->password = password;
}


void UserData::setType(const DataType type) {
    currentType = type;
}

QString UserData::getUsername() const {
    return username;
}


QString UserData::getPassword() const {
    return password;
}

bool operator <(const UserData &tmp1, const UserData &tmp2) {
    return tmp1.getUsername() < tmp2.getUsername();
}
