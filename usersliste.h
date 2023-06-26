#ifndef USERSLISTE_H
#define USERSLISTE_H

#include "user.h"
#include "datenbank.h"

class UsersListe
{
private:
    QVector<User*> meineUsers;
    Datenbank* db;
//    void sortieren();
public:
    UsersListe(QString dbDatei);
    ~UsersListe();

    int zaehleEintragen();
    bool userAnlegen(User* user);
    User* holenMitIndex(int index);
    User* holenMitUsername(QString username);

    QString hashPass(QString pass);
    void passAendern(QString neuesPass, QString username);
    Datenbank *getDB();

};

#endif // USERSLISTE_H
