#ifndef USER_H
#define USER_H

#include <QString>

class User
{
private:
    int id;
    QString username;
    QString vorname;
    QString nachname;
    QString pass;
public:
    User(QString username = "", QString vorname = "", QString nachname = "", QString pass = "");
    ~User();

    int getId();
    QString getUsername();
    QString getVorname();
    QString getNachname();
    QString getPass();
    QString toString();
    void setPass(QString pass);
    void setId(int id);
};

#endif // USER_H
