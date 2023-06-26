#ifndef USER_H
#define USER_H

#include <QString>

class User
{
private:
    QString username;
    QString vorname;
    QString nachname;
    QString pass;
public:
    User(QString username = "", QString vorname = "", QString nachname = "", QString pass = "");
    ~User();

    QString getUsername();
    QString getVorname();
    QString getNachname();
    QString getPass();
    QString toString();
    void setPass(QString pass);
};

#endif // USER_H
