#include "kontenliste.h"

//KontenListe::KontenListe(QString dbDatei)
KontenListe::KontenListe(Datenbank* db, QString inhaberUsername)
{
    this->db = db;
//    this->alleKonten = this->db->loggedUserKontenHolenAusDB(inhaberUsername);
    this->alleKonten = this->db->alleKontenHolenAusDB();
    kontenZuordnen(inhaberUsername);
}

//KontenListe::KontenListe(QString dbDatei)
//{
//    this->db = new Datenbank(dbDatei);
//    this->alleKonten = this->db->alleKontenHolenAusDB();
//}

void KontenListe::kontenZuordnen(QString inhaberUsername) //Giro- von Sparkkonten trennen und zwei Listen eintragen
{
    int anz = zaehleKonten();
    for(int i = 0; i < anz; i++) {
        if(alleKonten[i]->getInhaberUsername() == inhaberUsername) {
            gesamtKontenstand+= alleKonten[i]->getKontostand();
            if((dynamic_cast<Girokonto*>(alleKonten[i]) != nullptr)) { //Datentyp vom Objekt überprüfen
                loggedUserGirokonten.append(dynamic_cast<Girokonto*>(alleKonten[i])); //Casting (Konto*->Girokonto*)
                gesamtGirokontenstand+= alleKonten[i]->getKontostand();
            }
            else if((dynamic_cast<Sparkonto*>(alleKonten[i]) != nullptr)) { //Datentyp vom Objekt überprüfen
                loggedUserSparkonten.append(dynamic_cast<Sparkonto*>(alleKonten[i])); //Casting (Konto*->Sparkonto*)
                gesamtSparkontenstand+= alleKonten[i]->getKontostand();
            }
        }
    }
}

//void KontenListe::testKontenAnlegen(QVector<Konto *> testKonten)
//{
//    qDebug() << "alleKonten Size: " << testKonten.size();

////    for(Konto* k : testKonten) {
////        girokontoAnlegen(k->getKontostand(), k->getDispo, k->getInhaberUsername(), k->getUserId); //Rückgabewert = KontoNr
////        sparkontoAnlegen(double startKapital, QString inhaberUsername, int userId); //Rückgabewert = KontoNr
////        qDebug() << "alleKonten: " << alleKonten.at(alleKonten.indexOf(k))->getKontoNr();
////    }
//}

int KontenListe::zaehleKonten()
{
    return alleKonten.size();
}

int KontenListe::zaehleGirokonten()
{
    return loggedUserGirokonten.size();
}

int KontenListe::zaehleSparkonten()
{
    return loggedUserSparkonten.size();
}

Konto *KontenListe::kontoHolen(int index)
{
    Konto* tmpKonto = NULL;
    if (index < this->zaehleKonten() && index >=0)
    {
        tmpKonto =  this->alleKonten[index];
    }
    return tmpKonto;
}

Girokonto *KontenListe::girokontoHolen(int index)
{
    Girokonto* tmpKonto = NULL;
    if (index < this->zaehleKonten() && index >=0)
    {
        tmpKonto =  this->loggedUserGirokonten[index];
    }
    return tmpKonto;
}

Sparkonto *KontenListe::sparkontoHolen(int index)
{
    Sparkonto* tmpKonto = NULL;
    if (index < this->zaehleKonten() && index >=0)
    {
        tmpKonto =  this->loggedUserSparkonten[index];
    }
    return tmpKonto;
}

int KontenListe::girokontoAnlegen(double startKapital, double dispo, QString inhaberUsername, int userId)
{
    int anzGirokonten = zaehleGirokonten();
    int kontoNr = userId * 1000101;
    if(anzGirokonten > 0) {
        kontoNr = userId * 1000000 + (loggedUserGirokonten[anzGirokonten-1]->getKontoNr() % 1000000) + 100;
    }
    if(startKapital > 0) { // TO DO : check if this correct
        db->girokontoEintragenInDB(kontoNr, startKapital, dispo, inhaberUsername);
    }
    Girokonto* gk = new Girokonto(kontoNr, startKapital, inhaberUsername ,dispo);
    loggedUserGirokonten.append(gk);
    alleKonten.append(gk);
    //TODO: sortieren
    return kontoNr;
}

int KontenListe::sparkontoAnlegen(double startKapital, QString inhaberUsername, int userId)
{
    int anzSparkonten = zaehleSparkonten();
    int kontoNr = userId * 1000199;
    if(anzSparkonten > 0) {
        kontoNr = userId * 1000000 + (loggedUserSparkonten[anzSparkonten-1]->getKontoNr() % 1000000) + 100;
    }
    if(startKapital > 0) {
        db->sparkontoEintragenInDB(kontoNr, startKapital, inhaberUsername);
    }

    Sparkonto* sk = new Sparkonto(kontoNr, inhaberUsername, startKapital);
    loggedUserSparkonten.append(sk);
    alleKonten.append(sk);
    //TODO: sortieren
    return kontoNr;
}

bool KontenListe::sparkontoLetzteAuszahlungAendern(QDate letzteAuszahlung, int KontoNr)
{
    QDate heute = QDate::currentDate();
    if(letzteAuszahlung <= heute) {
        int anz = zaehleSparkonten();
        for(int i = 0; i < anz; i++) { //TODO: implement method HoleKontoMitKontoNr or smth like that -> this is used in too many other functions, so this could simplify it
            if(loggedUserSparkonten[i]->getKontoNr() == KontoNr) {
                loggedUserSparkonten[i]->setLetzteAuszahlug(letzteAuszahlung);
            }
        }
        return db->sparkontoLetzteAuszahlungAendernInDB(letzteAuszahlung, KontoNr);
    }
    return false;
}

double KontenListe::kontostandAendern(double betrag, int KontoNr)
{
    double kontostand = 0;
    int anz = zaehleKonten();
    for(int i = 0; i < anz; i++) {
        if(alleKonten[i]->getKontoNr() == KontoNr)
        {
            if(betrag < 0) {
                if(alleKonten[i]->getKontoNr()) { // SPARKONTO
                    alleKonten[i]->auszahlen(betrag); // TODO: HANDLE THIS so there is a QMessageBox in case of problems
                   sparkontoLetzteAuszahlungAendern(QDate::currentDate(),KontoNr);// TODO: not necessary to repeat auszahel method, I can use sparkontoletzteauszahlungaender after with an if
                } else if(alleKonten[i]->getKontoNr() % 100 == 01) { // GIROKONTO
                   alleKonten[i]->auszahlen(betrag); // TO DO: HANDLE THIS so there is a QMessageBox in case of problems
                }
            } else alleKonten[i]->einzahlen(betrag);
            kontostand = alleKonten[i]->getKontostand();
        }
    }
    db->kontostandAendernInDB(kontostand, KontoNr);
    return kontostand;
}

double KontenListe::getGesamtKontenstand()
{
    return gesamtKontenstand;
}

double KontenListe::getGesamtGirokontenstand()
{
    return this->gesamtGirokontenstand;
}

double KontenListe::getGesamtSparkontenstand()
{
    return this->gesamtSparkontenstand;
}

void KontenListe::kontenlisteLeeren()
{
    this->alleKonten.clear();
    this->loggedUserGirokonten.clear();
    this->loggedUserSparkonten.clear();
}

QString KontenListe::outputDBError()
{
    return db->getLetzterError(); // DEBUG
}
