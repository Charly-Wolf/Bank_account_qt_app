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

        void kontoEintragenInDB(int kontoNr, double startkapital, QString inhaberUsername, QString kontotyp);
        void girokontoEintragenInDB(int kontoNr, double startkapital, double dispo, QString inhaberUsername);
        void sparkontoEintragenInDB(int kontoNr, double startkapital, QString inhaberUsername);

        bool letzteAuszahlungAendernInDB(QDate letzteAuszahlung, int KontoNr);
        bool kontostandAendernInDB(double kontostand, int KontoNr); // TO DO: Ein-, Auszahlung und Überweisung Methoden erstellen

        QVector<User*> alleUsersHolenAusDB();

        User* userHolenAusDB(QString username);// TO DO: DO I NEED THESE TWO DIFFERENT METHODS?
        void userEintragenInDB(User* user);

        QString getLetzterError(); //DEBUG
};

#endif // DATENBANK_H
