#ifndef DATENBANK_H
#define DATENBANK_H

#include <QtSql>
#include "konto.h"
#include "girokonto.h"
#include "sparkonto.h"
#include "user.h"

class Datenbank
{
    private:
        QSqlDatabase db;
        QString letzterError;
        bool ready;
        QSqlQuery *abfrage(QString sql);
    public:
        Datenbank(QString dbDatei);
        ~Datenbank();

        QVector<Konto*> alleKontenHolenAusDB();
//        QVector<Konto*> loggedUserKontenHolenAusDB(QString username);
        void girokontoEintragenInDB(int kontoNr, double startkapital, double dispo, QString inhaberUsername);
        void sparkontoEintragenInDB(int kontoNr, double startkapital, QString inhaberUsername);

        bool sparkontoLetzteAuszahlungAendernInDB(QDate letzteAuszahlung, int KontoNr);
        bool kontostandAendernInDB(double kontostand, int KontoNr);

        QVector<User*> alleUsersHolenAusDB();
        User* userHolenAusDB(QString username);
        void userEintragenInDB(User* user);

        void userPassAendernInDB(QString neuesPass, QString userName);

        QString getLetzterError(); //DEBUG
};

#endif // DATENBANK_H
