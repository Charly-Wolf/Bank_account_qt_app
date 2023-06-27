#ifndef USER_H
#define USER_H

#include <QString>

class User
{
private:
    int id; // TO DO: DO I REALLY NEED THIS? OR IS THERE A WAY TO AVOID IT -- right now it is used to create KontoNr using the User Id as the first digits
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
