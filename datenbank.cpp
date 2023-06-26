#include "datenbank.h"

Datenbank::Datenbank(QString dbDatei)
{
    this->ready = false;
    this->db = (QSqlDatabase::addDatabase("QSQLITE"));
    this->db.setDatabaseName(dbDatei);
    this->ready = db.open();
    this->letzterError = db.lastError().text();
    this->db.close();
}

Datenbank::~Datenbank()
{
    this->db.close();
}

QVector<Konto *> Datenbank::alleKontenHolenAusDB()
{
    QVector<Konto*> konten;

    bool ok = false;
    QSqlQuery* qry = this->abfrage("SELECT tblKonten.KontoNr, tblKonten.Kontostand, tblKonten.Kontotyp, tblKonten.InhaberUsername, "
                                    "tblGirokonten.dispo, tblSparkonten.LetzteAuszahlung "
                                    "FROM tblKonten "
                                    "LEFT JOIN tblGirokonten ON tblKonten.KontoNr = tblGirokonten.KontoNr "
                                    "LEFT JOIN tblSparkonten ON tblKonten.KontoNr = tblSparkonten.KontoNr ");
    if(qry) {
        ok = qry->first();
        while(ok) {
            int kontoNr = qry->value("KontoNr").toInt();
            double kontostand = qry->value("Kontostand").toDouble();
            QString kontotyp = qry->value("Kontotyp").toString();
            QString inhaberUsername = qry->value("InhaberUsername").toString();
            if(kontotyp == "Girokonto") {
                double dispo = qry->value("Dispo").toDouble();
                Girokonto* gk = new Girokonto(kontoNr, kontostand, inhaberUsername, dispo);
                konten.append(gk);

            }
            else {
                QDate letzteAuszahlung = QDate::fromString(qry->value("LetzteAuszahlung").toString(), "yyyy-MM-dd");
                Sparkonto* sk = new Sparkonto(kontoNr, inhaberUsername, kontostand);
                sk->setLetzteAuszahlug(letzteAuszahlung);
                konten.append(sk);
            }
            ok = qry->next();
        } //While
        delete qry;
    } //If
    return konten;
}

void Datenbank::kontoEintragenInDB(int kontoNr, double startkapital, QString inhaberUsername)
{
    QSqlQuery* qry;
    QString sql = "INSERT INTO tblKonten (KontoNr, Kontostand, Kontotyp, InhaberUsername)";
    sql += "VALUES(" + QString::number(kontoNr) + ", " + QString::number(startkapital) + ", 'Girokonto', '" + inhaberUsername +"');";
    qry = this->abfrage(sql);
    delete qry;
}

QSqlQuery *Datenbank::abfrage(QString sql)
{
    this->ready = this->db.open();
    QSqlQuery *qry = new QSqlQuery(this->db);
    this->letzterError = this->db.lastError().text();
    if (this->ready) {
        this->ready = qry->exec(sql);
        this->letzterError = qry->lastError().text();
    }
    return qry;
}

//QVector<Konto *> Datenbank::loggedUserKontenHolenAusDB(QString username)
//{
//    QVector<Konto*> konten;

//    bool ok = false;
//    QSqlQuery* qry = this->abfrage("SELECT tblKonten.KontoNr, tblKonten.Kontostand, tblKonten.Kontotyp, "
//                                    "tblGirokonten.dispo, tblSparkonten.LetzteAuszahlung "
//                                    "FROM tblKonten "
//                                    "LEFT JOIN tblGirokonten ON tblKonten.KontoNr = tblGirokonten.KontoNr "
//                                    "LEFT JOIN tblSparkonten ON tblKonten.KontoNr = tblSparkonten.KontoNr "
//                                    "WHERE InhaberUsername = '" + username + "';");
//    if(qry) {
//        ok = qry->first();
//        while(ok) {
//            int kontoNr = qry->value("KontoNr").toInt();
//            double kontostand = qry->value("Kontostand").toDouble();
//            QString kontotyp = qry->value("Kontotyp").toString();
//            QString inhaberUsername = qry->value("InhaberUsername").toString();
//            if(kontotyp == "Girokonto") {
//                double dispo = qry->value("Dispo").toDouble();
//                Girokonto* gk = new Girokonto(kontoNr, kontostand, inhaberUsername, dispo);
//                konten.append(gk);

//            }
//            else {
//                QDate letzteAuszahlung = QDate::fromString(qry->value("LetzteAuszahlung").toString(), "yyyy-MM-dd");
//                Sparkonto* sk = new Sparkonto(kontoNr, inhaberUsername, kontostand);
//                sk->setLetzteAuszahlug(letzteAuszahlung);
//                konten.append(sk);
//            }
//            ok = qry->next();
//        } //While
//        delete qry;
//    } //If
//    return konten;
//}

QVector<User *> Datenbank::alleUsersHolenAusDB()
{
    QVector<User*>users;

    bool ok;
    QSqlQuery *qry;
    qry = this->abfrage("SELECT * FROM tblUsers");

    if (qry) {
        User *u;
        int id;
        QString username;
        QString vorname;
        QString nachname;
        QString pass;

        ok = qry->first();
        while (ok) {
            id = qry->value("Id").toInt();
            username = qry->value("Username").toString();
            nachname = qry->value("Nachname").toString();
            vorname = qry->value("Vorname").toString();
            pass = qry->value("Pass").toString();
            u = new User(username, vorname, nachname, pass);
            u->setId(id);
            users.append(u);
            ok = qry->next();
        } //while
        delete qry;
    } //if qry
    return users;
}

void Datenbank::girokontoEintragenInDB(int kontoNr, double startkapital, double dispo, QString inhaberUsername)
{
    kontoEintragenInDB(kontoNr, startkapital, inhaberUsername);

    QSqlQuery* qry;
    QString sql = "INSERT INTO tblGirokonten (KontoNr, Dispo) ";
    sql+= "VALUES(" + QString::number(kontoNr) + ", " + QString::number(dispo) + ");";
    qry = this->abfrage(sql);
    delete qry;
}

void Datenbank::sparkontoEintragenInDB(int kontoNr, double startkapital, QString inhaberUsername)
{
    kontoEintragenInDB(kontoNr, startkapital, inhaberUsername);

    QSqlQuery* qry;
    QString sql = "INSERT INTO tblSparkonten (KontoNr) ";
    sql+= "VALUES(" + QString::number(kontoNr) + ");";
    qry = this->abfrage(sql);
    delete qry;
}

bool Datenbank::sparkontoLetzteAuszahlungAendernInDB(QDate letzteAuszahlung, int KontoNr)
{
    QSqlQuery* qry;
    QString letzteAuszahlungStr = letzteAuszahlung.toString("yyyy-MM-dd");
    QString KontoNrStr = QString::number(KontoNr);
    QString sql = "UPDATE tblSparkonten SET LetzteAuszahlung = '" + letzteAuszahlungStr + "' ";
    sql += "WHERE KontoNr = " + KontoNrStr;
    qry = this->abfrage(sql);

    if(qry->lastError().isValid()) return false;
    return true;
    delete qry;
}

bool Datenbank::kontostandAendernInDB(double kontostand, int KontoNr)
{
    QSqlQuery* qry;
    QString kontostandStr = QString::number(kontostand);
    QString KontoNrStr = QString::number(KontoNr);
    QString sql = "UPDATE tblKonten SET Kontostand = '" + kontostandStr + "' ";
    sql += "WHERE KontoNr = " + KontoNrStr;
    qry = this->abfrage(sql);
    if(qry->lastError().isValid()) return false;
    return true;
    delete qry;
}


User *Datenbank::userHolenAusDB(QString username)
{
    User* u = NULL;
    QSqlQuery *qry;
    qry = this->abfrage("SELECT * FROM tblUsers "
            "WHERE Username = '" + username + "'");
    if(qry->first()) {
        int id = qry->value("Id").toInt();
        QString vorname = qry->value("Vorname").toString();
        QString nachname = qry->value("Nachname").toString();
        QString pass = qry->value("Pass").toString();
        u = new User(username, vorname, nachname, pass);
        u->setId(id);
    }
    delete qry;
    return u;
}

void Datenbank::userEintragenInDB(User *user)
{
    QSqlQuery* qry;
    QString sql = "INSERT INTO tblUsers (Username, Vorname, Nachname, Pass) ";
    sql += "VALUES('" + user->getUsername() + "', '" + user->getVorname() + "', '";
    sql += user->getNachname() + "', '" + user->getPass() + "')";

    qry = this->abfrage(sql);
    delete qry;
}

void Datenbank::userPassAendernInDB(QString neuesPass, QString username)
{
    QSqlQuery* qry;
    QString sql = "UPDATE tblKonten SET Pass = '" + neuesPass + "' ";
    sql += "WHERE Username = '" + username + "';";

    qry = this->abfrage(sql);
    delete qry;
}

QString Datenbank::getLetzterError()
{
    return letzterError; // DEBUG
}
