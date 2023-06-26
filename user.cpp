#include "user.h"

User::User(QString username, QString vorname, QString nachname, QString pass)
{
    this->vorname = vorname;
    this->nachname = nachname;
    this->username = username;
    this->pass = pass;
}

User::~User()
{

}

int User::getId()
{
 return this->id;
}

QString User::getUsername()
{
    return this->username;
}

QString User::getVorname()
{
    return this->vorname;
}

QString User::getNachname()
{
    return this->nachname;
}

QString User::getPass()
{
    return this->pass;
}

QString User::toString()
{
    return "Username: " + this->username + "- Vorname: " + this->vorname + " Nachname: " + this->nachname;
}

void User::setPass(QString pass)
{
    this->pass = pass;
}

void User::setId(int id)
{
    this->id = id;
}
