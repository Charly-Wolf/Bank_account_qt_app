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
        ~KontenListe();

        void kontenZuordnen(QString inhaberUsername);

//        void testKontenAnlegen(QVector<Konto*> testKonten);

        int zaehleKonten();
        int zaehleGirokonten();
        int zaehleSparkonten();

        Konto* kontoHolen(int index);
        Girokonto* girokontoHolen(int index);
        Sparkonto* sparkontoHolen(int index);

        int girokontoAnlegen(double startKapital, double dispo, QString inhaberUsername, int userId); //Rückgabewert = KontoNr
        int sparkontoAnlegen(double startKapital, QString inhaberUsername, int userId); //Rückgabewert = KontoNr

        bool setLetzteAuszahlung(QDate letzteAuszahlung, int KontoNr);

        //TO DO: !!!!!!!!!!!!!!!!!!!!!!!!DO NOT USE KONTOSTAND ÄNDERN!! MAKE TWO METHODS: EINZAHLEN, AUSZAHLEN, (MAYBE ALSO UEBERWEISEN)
        double kontostandAendern(double betrag, int KontoNr);//Rückgabewert = neuer Kontostand

        double getGesamtKontenstand();
        double getGesamtGirokontenstand();
        double getGesamtSparkontenstand();

        void kontenlisteLeeren();

        QString outputDBError(); //DEBUG
};

#endif // KONTENLISTE_H
