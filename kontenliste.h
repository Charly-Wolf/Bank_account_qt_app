#ifndef KONTENLISTE_H
#define KONTENLISTE_H

#include <QVector>
#include "konto.h"
#include "datenbank.h"

class KontenListe
{
    private:
        QVector<Konto*> alleKonten;
//        QVector<Konto*> loggedUserKonten;
        QVector<Girokonto*> loggedUserGirokonten;
        QVector<Sparkonto*> loggedUserSparkonten;
        Datenbank *db;
        double gesamtKontenstand;
        double gesamtGirokontenstand;
        double gesamtSparkontenstand;

    public:
//        KontenListe(QString dbDatei);
        KontenListe(Datenbank* db, QString inhaberUsername);
        KontenListe(Datenbank* db); // TODO check if I can make it more performant by loading all accounts at once, and then separating the ones only for the logged user
        ~KontenListe();

        void kontenZuordnen(QString inhaberUsername);

        int zaehleKonten();
        int zaehleGirokonten();
        int zaehleSparkonten();

        Konto* kontoHolen(int index);
        Girokonto* girokontoHolen(int index);
        Sparkonto* sparkontoHolen(int index);

        int girokontoAnlegen(double startKapital, double dispo, QString inhaberUsername); //Rückgabewert = KontoNr
        int sparkontoAnlegen(double startKapital, QString inhaberUsername); //Rückgabewert = KontoNr

        bool sparkontoLetzteAuszahlungAendern(QDate letzteAuszahlung, int KontoNr);
        double kontostandAendern(double betrag, int KontoNr);//Rückgabewert = neuer Kontostand

        double getGesamtKontenstand();
        double getGesamtGirokontenstand();
        double getGesamtSparkontenstand();

        void kontenlisteLeeren();

        QString outputError(); //DEBUG
};

#endif // KONTENLISTE_H