#include "usersliste.h"

UsersListe::UsersListe(QString dbDatei)
{
    this->db = new Datenbank(dbDatei);
    this->meineUsers = db->alleUsersHolenAusDB();
//    this->sortieren();
}

UsersListe::~UsersListe()
{

}

int UsersListe::zaehleEintragen()
{
    return meineUsers.size();
}

bool UsersListe::userAnlegen(User* user)
{
    user->setPass(hashPass(user->getPass()));
    meineUsers.append(user);
    db->userEintragenInDB(user);
}

User *UsersListe::holenMitIndex(int index)
{
    return meineUsers[index];
}

User *UsersListe::holenMitUsername(QString username)
{
//    return db->userHolenAusDB(username);

    for(User* u : meineUsers) {
        if(u->getUsername() == username) {
            return u;
        }
    }
}

QString UsersListe::hashPass(QString pass)
{
    QByteArray data = pass.toUtf8();
    // Pass + random QString verschlüsseln anhand SHA-256
    QByteArray hashedData = QCryptographicHash::hash(data, QCryptographicHash::Sha256);
    return hashedData.toHex();
}

void UsersListe::passAendern(QString neuesPass, QString username)
{
    holenMitUsername(username)->setPass(neuesPass);
    db->userPassAendernInDB(neuesPass, username);
}

Datenbank *UsersListe::getDB()
{
    return this->db;
}
