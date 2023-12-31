#include "frmmain.h"
#include "ui_frmmain.h"
#include <QMessageBox>
#include <QGraphicsOpacityEffect>
#include <QKeyEvent>

#include <QFileDialog> // Für das Exportieren auf CSV
#include <QLocale> // Für die Formatierung von Nummern --->> TO DO: do that in the model toString method

FrmMain::FrmMain(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FrmMain)
{
    ui->setupUi(this);

    QString datenbankDatei = "dbBankkonto.sqlite";
    setWindowTitle("Bankkonto App");

    this->users = new UsersListe(datenbankDatei);
//    this->konten = new KontenListe(datenbankDatei);
    testUsersAnlegen();
//    testKontenAnlegen();
    initialWidgetsState();}

FrmMain::~FrmMain()
{
    delete ui;
}

//bool FrmMain::testKontenAnlegen()
//{
//    QVector<Konto*> testKonten = {
//        new Girokonto(1000101,100000,testUsers[0]->getUsername(),10000),
//        new Girokonto(1000201,25000,testUsers[0]->getUsername(),3000),
//        new Girokonto(1000301,44555.33,testUsers[0]->getUsername(),15000),
//        new Girokonto(1000401,6000,testUsers[0]->getUsername(),10000),
//        new Girokonto(1000501,0,testUsers[0]->getUsername(),2500.50),
//        new Girokonto(2000601,100514,testUsers[1]->getUsername(),10000),
//        new Girokonto(2000701,100031,testUsers[1]->getUsername(),10400),
//        new Girokonto(3000801,233,testUsers[2]->getUsername(),50000),

//        new Sparkonto(1000199,testUsers[0]->getUsername(),0),
//        new Sparkonto(1000199,testUsers[0]->getUsername(),50),
//        new Sparkonto(2000199,testUsers[1]->getUsername(),33252.47),
//        new Sparkonto(2000199,testUsers[1]->getUsername(),555),
//        new Sparkonto(3000199,testUsers[2]->getUsername(),0)
//    };

//    konten->testKontenAnlegen(testKonten);
//    konten->kontenZuordnen(loggedUser->getUsername());
//    //TO DO: return true/false if error
//}

void FrmMain::testUsersAnlegen()
{
    QVector<User*>testUsers = {
        new User("MaxMust", "Maximilian", "Mustermann", "1234"),
        new User("JohnDoe", "John", "Doe", "1234"),
        new User("AlexSno", "Alexander", "Snowberg", "1234"),
        new User("KatSch22", "Katja", "Schneider", "1234")
    };

    for(User* u : testUsers) {
        users->userAnlegen(u);
    }
}

void FrmMain::kontenAnzeigen()
{
    int anzGirokonten = this->konten->zaehleGirokonten();
    int anzSparkonten = this->konten->zaehleSparkonten();

    ui->tableGirokonten->setRowCount(anzGirokonten);
    ui->tableSparkonten->setRowCount(anzSparkonten);

    ui->tableGirokonten->clearContents();
    ui->tableSparkonten->clearContents();


    QTableWidgetItem* tableItem;

    for (int i = 0; i < anzGirokonten; i++) {

        Girokonto* girokonto = konten->girokontoHolen(i);

        tableItem = new QTableWidgetItem(girokonto->toString());
        ui->tableGirokonten->setItem(i, 0, tableItem);
        tableItem = new QTableWidgetItem(kontoStandFormatieren(girokonto->getKontostand()));
        ui->tableGirokonten->setItem(i, 1, tableItem);
        if(girokonto->getKontostand() < 0) {
            tableItem->font().setBold(true);
            tableItem->setForeground(QColor(Qt::red));
        }
        tableItem = new QTableWidgetItem( QLocale().toString(girokonto->getDispo()) + " €");
        ui->tableGirokonten->setItem(i, 2, tableItem);
        if(girokonto->getDispo() == -girokonto->getKontostand()) {
            tableItem->setText("ERREICHT");
            tableItem->font().setBold(true);
            tableItem->setForeground(QColor(Qt::red));
        }
    }
    for (int i = 0; i < anzSparkonten; i++) {

        Sparkonto* sparkonto = konten->sparkontoHolen(i);

        tableItem = new QTableWidgetItem(sparkonto->toString());
        ui->tableSparkonten->setItem(i, 0, tableItem);
        tableItem = new QTableWidgetItem(kontoStandFormatieren(sparkonto->getKontostand()));
        ui->tableSparkonten->setItem(i, 1, tableItem);
        tableItem = new QTableWidgetItem(letzteAuszahlungFormatieren(sparkonto->getLetzteAuszahlung()));
        ui->tableSparkonten->setItem(i, 2, tableItem);
        if(sparkonto->getLetzteAuszahlung() == QDate::currentDate()) {
            tableItem->font().setBold(true);
            tableItem->setForeground(QColor(Qt::red));
        }
    }

    anzGirokonten > 1 || anzGirokonten == 0? ui->lblTableGiro->setText("Girokonten") : ui->lblTableGiro->setText("Girokonto");
    anzSparkonten > 1 || anzSparkonten == 0? ui->lblTableSpar->setText("Sparkonten") : ui->lblTableSpar->setText("Sparkonto");
}

void FrmMain::opInHistorieHinzufuegen(QString kontoNrString, QString operation, double betrag, double kontostand)
{
    //TO DO NICE TO HAVE: Use the tblOperationen from DB and create a class and list to keep track of operations during time
    int anzZeilen = ui->tableHistorie->rowCount();

    QString betragStr = QLocale().toString(betrag) + " €";
    QString kontostandStr = QLocale().toString(kontostand) + " €";
    QTableWidgetItem* tableItem;

    ui->tableHistorie->insertRow(anzZeilen);

    tableItem = new QTableWidgetItem(kontoNrString);
    ui->tableHistorie->setItem(anzZeilen, 0, tableItem);
    tableItem = new QTableWidgetItem(operation);
    ui->tableHistorie->setItem(anzZeilen, 1, tableItem);
    tableItem = new QTableWidgetItem(betragStr);
    ui->tableHistorie->setItem(anzZeilen, 2, tableItem);
    tableItem = new QTableWidgetItem(kontostandStr);
    ui->tableHistorie->setItem(anzZeilen, 3, tableItem);

    ui->btnClearHistorie->setEnabled(true);
}

void FrmMain::fadeInGuiElement(QWidget* qWidget, double duration)
{
    QGraphicsOpacityEffect *fadeInEffect = new QGraphicsOpacityEffect(qWidget);
    qWidget->setGraphicsEffect(fadeInEffect);
    fadeInEffect->setOpacity(0.0);
    QPropertyAnimation *fadeinAnimation = new QPropertyAnimation(fadeInEffect, "opacity");
    fadeinAnimation->setDuration(duration);
    fadeinAnimation->setStartValue(0.0);
    fadeinAnimation->setEndValue(1.0);


    qWidget->setVisible(true);
    qWidget->setEnabled(true);
    fadeinAnimation->start();
}

void FrmMain::fadeOutGuiElement(QWidget* qWidget, double duration)
{
    QGraphicsOpacityEffect *fadeInEffect = new QGraphicsOpacityEffect(qWidget);
    qWidget->setGraphicsEffect(fadeInEffect);
    fadeInEffect->setOpacity(1.0);
    QPropertyAnimation *fadeinAnimation = new QPropertyAnimation(fadeInEffect, "opacity");
    fadeinAnimation->setDuration(duration);
    fadeinAnimation->setStartValue(1.0);
    fadeinAnimation->setEndValue(0.0);

    qWidget->setEnabled(false);
    connect(fadeinAnimation, &QPropertyAnimation::finished, this, [=]() {
            qWidget->setVisible(false);
        });

    fadeinAnimation->start();
}

void FrmMain::animateBankNameFadeIn()
{
    ui->lblNextTitel->setVisible(false);
    ui->lblBankTitel->setVisible(false);
    ui->lblGeldEmoji->setVisible(false);
    QTimer::singleShot(500, [=]() {
        fadeInGuiElement(ui->lblNextTitel, 500);
        QTimer::singleShot(700, [=]() {
            fadeInGuiElement(ui->lblBankTitel, 2500);
            QTimer::singleShot(900, [=]() {
                fadeInGuiElement(ui->lblGeldEmoji, 2500);
            });
        });
    });
}

void FrmMain::animateHeaderBtnsFadeIn()
{
    QTimer::singleShot(500, [=]() {
        fadeInGuiElement(ui->btnBankkonten, 400);
        ui->btnBankkonten->setEnabled(false);// Necessary bcs with fadein it gets activated
        QTimer::singleShot(700, [=]() {
            fadeInGuiElement(ui->btnUserVerwalten, 400);
            QTimer::singleShot(700, [=]() {
                fadeInGuiElement(ui->btnAusloggen, 400);
            });
        });
    });
}

void FrmMain::initialWidgetsState()
{
    fadeInGuiElement(ui->backgroundImg, 300);
    fadeInGuiElement(ui->widgetDebug, 100);

    enterLoginModus();

    // QWidgets -> unsichtbar (außer Login)
    ui->widgetHome->setVisible(false);
    ui->widgetNeuerUser->setVisible(false);
    ui->widgetTschuess->setVisible(false);
    ui->widgetUserVerwalten->setVisible(false);
    // Header Buttons -> unsichtbar
    ui->btnAusloggen->setVisible(false);
    ui->btnUserVerwalten->setVisible(false);
    ui->btnBankkonten->setVisible(false);
    ui->lblUserTitel->setVisible(false);
    ui->lblEmojiUser->setVisible(false);

    operationModusDeaktivieren();
    ui->tabWidgetKontotypInfos->setVisible(false);
}

QString FrmMain::kontoStandFormatieren(double kontoStand)
{
    QString formKontostand = QLocale().toString(kontoStand);
    return formKontostand += " €";
}

QString FrmMain::letzteAuszahlungFormatieren(QDate letzteAuszahlung)
{
    QString formLetzteAuszahlung;
        letzteAuszahlung == QDate::currentDate() ?
                            formLetzteAuszahlung = "HEUTE"
                          : formLetzteAuszahlung = letzteAuszahlung.toString("dd.MM.yyyy");

        return formLetzteAuszahlung.isEmpty() ? "Noch keine..." : formLetzteAuszahlung;
}

void FrmMain::operationModusDeaktivieren()
{
//    ui->cBoxEmpfaenger->clear();
    ui->tableGirokonten->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableSparkonten->setSelectionMode(QAbstractItemView::SingleSelection);

//    fadeOutGuiElement(ui->lblBetrag, 300);
    ui->lblBetrag->setVisible(false);
//    fadeOutGuiElement(ui->sboxBetrag, 300);
    ui->sboxBetrag->setVisible(false);
//    fadeOutGuiElement(ui->lblBetragEuro, 300);
    ui->lblBetragEuro->setVisible(false);
//    fadeOutGuiElement(ui->lblEmpf, 300);
    ui->lblEmpf->setVisible(false);
//    fadeOutGuiElement(ui->cBoxEmpfaenger, 300);
    ui->cBoxEmpfaenger->setVisible(false);
//    fadeOutGuiElement(ui->btnAbbrechen, 300);
    ui->btnAbbrechen->setVisible(false);
//    fadeOutGuiElement(ui->btn_OkOperation, 300);
    ui->btn_OkOperation->setVisible(false);
}

void FrmMain::operationModusAktivieren() //TO DO: maybe use enum opModus as parameter
{
    ui->tableGirokonten->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableSparkonten->setSelectionMode(QAbstractItemView::NoSelection);

    ui->btnEinzahlung->setEnabled(false);
    ui->btnAuszahlung->setEnabled(false);
    ui->btnUeberw->setEnabled(false);

    ui->lblBetrag->setVisible(true);
    ui->sboxBetrag->setVisible(true);
    ui->lblBetragEuro->setVisible(true);
    ui->btnAbbrechen->setVisible(true);
    ui->btnAbbrechen->setEnabled(true);
    ui->btn_OkOperation->setVisible(true);
    ui->btn_OkOperation->setEnabled(true);

    switch (opModus) {
        case Einzahlung:
            ui->btn_OkOperation->setText("✔  Einzahlen");
            break;
        case Auszahlung:
            ui->btn_OkOperation->setText("✔  Auszahlen");
            break;
        case Ueberweisung:
//            fadeInGuiElement(ui->lblEmpf, 300);
//            fadeInGuiElement(ui->cBoxEmpfaenger, 300);
            ui->lblEmpf->setVisible(true);
            ui->cBoxEmpfaenger->setVisible(true);
            ui->btn_OkOperation->setText("✔  Überweisen");
            break;
    }

    ui->sboxBetrag->setFocus();

    // Commented out because there were problems when trying to enter op modus after creating a new account
//    fadeInGuiElement(ui->lblBetrag, 300);
//    fadeInGuiElement(ui->sboxBetrag, 300);
//    fadeInGuiElement(ui->lblBetragEuro, 300);
//    fadeInGuiElement(ui->btnAbbrechen, 300);
//    fadeInGuiElement(ui->btn_OkOperation, 300);
}

void FrmMain::enterLoginModus()
{
    animateBankNameFadeIn();
    fadeInGuiElement(ui->widgetLogin, 900);
    ui->leUserName->setFocus();
}

void FrmMain::exitLoginModus()
{
    fadeOutGuiElement(ui->widgetLogin,500);
    ui->leUserName->clear();
    ui->lePass->clear();

    ui->widgetLogin->setVisible(false);
    ui->leUserName->clear();
    ui->lePass->clear();

    ui->btnLoginDebugOhneUsername->setVisible(false);
}

void FrmMain::enterHomeModus()
{
    ui->widgetHome->setVisible(true);

    animateUserNameLbl();
    animateHeaderBtnsFadeIn();
    fadeInGuiElement(ui->btnCsvExport, 1700);

    ui->tabWidgetKonten->setCurrentIndex(0);
    ui->tabWidgetOperationen->setCurrentIndex(0);

    ui->lblHinweisKontenMarkieren->setVisible(false);
    ui->lblTableGiro->setVisible(false);
    ui->lblTableSpar->setVisible(false);
    fadeInGuiElement(ui->lblHinweisKontenMarkieren, 700);
    fadeInGuiElement(ui->lblTableGiro, 400);
    fadeInGuiElement(ui->lblTableSpar, 400);
    ui->lblMarkiertesKonto->setVisible(true);
    ui->lblMarkiertesKonto->setText("Markieren Sie ein Konto \naus der Konten-Tabellen");

    // radBtnKeinKonto is only used to uncheck the other 2 radio buttons, it will never be visible
    ui->radBtnKeinKonto->setVisible(false);
    ui->radBtnKeinKonto->setChecked(true);

    ui->btnEinzahlung->setEnabled(false);
    ui->btnAuszahlung->setEnabled(false);
    ui->btnUeberw->setEnabled(false);
    ui->lblMarkKontostandTitel->setVisible(false);
    ui->lblMarkKontostand->setVisible(false);
    ui->lblDispoAnlegen->setVisible(false);
    ui->sbDispoAnlegen->setVisible(false);
    ui->lblBetragEuroDispoKontoAnlegen->setVisible(false);
    ui->sbStartKapAnlegen->setEnabled(false);
    ui->btn_OkKontoAnlegen->setEnabled(false);
    ui->sbStartKapAnlegen->setValue(0);
    ui->sbDispoAnlegen->setValue(0);

    //TO DO: check if this right
    markierteGirokonto = NULL;
    markierteSparkonto = NULL;
    markierteKontoNr = NULL;

    setTabellenWidth();
}

void FrmMain::exitHomeModus()
{
    ui->widgetHome->setVisible(false);

    fadeOutGuiElement(ui->lblHinweisKontenMarkieren, 400);
    fadeOutGuiElement(ui->lblTableGiro, 400);
    fadeOutGuiElement(ui->lblTableSpar, 400);
    fadeOutGuiElement(ui->btnCsvExport, 400);

    operationModusDeaktivieren();
    ui->lblMarkiertesKonto->setVisible(false);

}

void FrmMain::transitionLogout()
{
    // Fadeout Header
    fadeOutGuiElement(ui->btnBankkonten, 400);
    fadeOutGuiElement(ui->btnUserVerwalten, 400);
    fadeOutGuiElement(ui->btnAusloggen, 400);
    fadeOutGuiElement(ui->lblEmojiUser, 400);
    fadeOutGuiElement(ui->lblUserTitel,400);

    if(ui->widgetHome->isVisible()) {
        exitHomeModus();
    } else if (ui->widgetUserVerwalten->isVisible()) {
        ui->widgetUserVerwalten->setVisible(false);
    }

    ui->lblTschuess->setText("Auf Wiedersehen\n" + loggedUser->getVorname() + "!");
    fadeInGuiElement(ui->widgetTschuess, 500);

    QTimer::singleShot(1200, [=]() { // Lambda Function, 1,2 Sek warten und danach Fadeout ausführen
        fadeOutGuiElement(ui->widgetTschuess, 400);
        fadeOutGuiElement(ui->lblNextTitel,400);
        fadeOutGuiElement(ui->lblBankTitel,400);
        fadeOutGuiElement(ui->lblGeldEmoji,400);

        QTimer::singleShot(300, [=]() { // 300 ms warten, und danach -> enter login Modus
            enterLoginModus();
        });
    });
}

void FrmMain::empfaengerKontenLaden(int MarkiertesKontoNr)
{
    ui->cBoxEmpfaenger->clear();
    ui->btnUeberw->setEnabled(true);

    int anz = konten->zaehleKonten();
    for(int i = 0; i < anz; i++) {
        if(konten->kontoHolenMitIndex(i)->getKontoNr() != MarkiertesKontoNr)
            ui->cBoxEmpfaenger->addItem(konten->kontoHolenMitIndex(i)->toString() + " -- " + konten->kontoHolenMitIndex(i)->getInhaberUsername());
    }
}

void FrmMain::kontoAnlegenGuiDeaktivieren()
{
    ui->lblStartKapAnlegen->setEnabled(false);
    ui->sbStartKapAnlegen->setValue(0);
    ui->btn_OkKontoAnlegen->setEnabled(false);
    ui->radBtnKeinKonto->setChecked(true);
    fadeOutGuiElement(ui->lblDispoAnlegen,300);
    fadeOutGuiElement(ui->sbDispoAnlegen,300);
    fadeOutGuiElement(ui->lblBetragEuroDispoKontoAnlegen,300);
    ui->tabWidgetKonten->setCurrentIndex(0);
    fadeOutGuiElement(ui->tabWidgetKontotypInfos, 300);
    fadeInGuiElement(ui->tabWidgetOperationen, 300);
    ui->sbStartKapAnlegen->setEnabled(false);
}

void FrmMain::on_tableGirokonten_itemSelectionChanged()
{
    int index = ui->tableGirokonten->currentRow();
    ui->tableGirokonten->setFocus(); // BUG: if I add this to both functions, it wont work :( So I left it only for this one
    markierteGirokonto = konten->girokontoHolen(index);
    markierteKontoNr = markierteGirokonto->getKontoNr();

    ui->lblMarkiertesKonto->setText("Girokonto " + markierteGirokonto->toString());
    ui->btnEinzahlung->setEnabled(true);
    if(!ui->lblMarkKontostand->isVisible()) ui->lblMarkKontostand->setVisible(true);

    if(!ui->lblMarkKontostandTitel->isVisible()) ui->lblMarkKontostandTitel->setVisible(true);

    ui->lblMarkKontostand->setText(kontoStandFormatieren(markierteGirokonto->getKontostand()));
    if(markierteGirokonto->getKontostand() < 0 ) {
        ui->lblMarkKontostand->setStyleSheet("color: red;");
    }else {
        ui->lblMarkKontostand->setStyleSheet("color: black;");
    }
    if (ui->sboxBetrag->value() != markierteGirokonto->getKontostand() + markierteGirokonto->getDispo()) {
        ui->btnAuszahlung->setEnabled(true);
        empfaengerKontenLaden(markierteGirokonto->getKontoNr());
    }
    else {
        ui->btnAuszahlung->setEnabled(false);
        ui->btnUeberw->setEnabled(false);
    }
    ui->tableSparkonten->clearSelection();
}


void FrmMain::on_tableSparkonten_itemSelectionChanged()
{
    int index = ui->tableSparkonten->currentRow();
    markierteSparkonto = konten->sparkontoHolen(index);
    markierteKontoNr = markierteSparkonto->getKontoNr();

    if(!ui->lblMarkKontostand->isVisible()) ui->lblMarkKontostand->setVisible(true);
    if(!ui->lblMarkKontostandTitel->isVisible()) ui->lblMarkKontostandTitel->setVisible(true);
    ui->lblMarkKontostand->setText(kontoStandFormatieren(markierteSparkonto->getKontostand()));
    ui->lblMarkiertesKonto->setText("Sparkonto " + markierteSparkonto->toString());
    ui->btnEinzahlung->setEnabled(true);

    if(markierteSparkonto->getLetzteAuszahlung() != QDate::currentDate()) { //Wenn heute schon ausgezahlt wurde...
        ui->btnAuszahlung->setEnabled(true);
        empfaengerKontenLaden(markierteSparkonto->getKontoNr());
    } else {
        ui->btnAuszahlung->setEnabled(false);
        ui->btnUeberw->setEnabled(false);
    }
    ui->tableGirokonten->clearSelection();
}

void FrmMain::on_btnUeberw_clicked()
{
    opModus = Ueberweisung;
    operationModusAktivieren();
}

void FrmMain::on_btnAbbrechen_clicked()
{
    ui->sboxBetrag->setValue(0);
    operationModusDeaktivieren();
    ui->btnEinzahlung->setEnabled(true);
    ui->btnAuszahlung->setEnabled(true);
    ui->btnUeberw->setEnabled(true);
//    ui->lblMarkiertesKonto->setText("Markieren Sie ein Konto \naus der Konten-Tabellen");
//    ui->lblMarkKontostand->setVisible(false);
//    ui->lblMarkKontostandTitel->setVisible(false);
    if(markierteKontoNr % 100 == 01) {
        ui->tableGirokonten->setFocus();
    } else if (markierteKontoNr % 100 == 99) {
        ui->tableSparkonten->setFocus();
    }
}

void FrmMain::on_btnEinzahlung_clicked()
{
    opModus = Einzahlung;
    operationModusAktivieren();
}


void FrmMain::on_btnAuszahlung_clicked()
{
    opModus = Auszahlung;
    operationModusAktivieren();
}

void FrmMain::on_radBtnGiro_clicked()
{
    toggleBtnSichtbarkeitWennGroesserAlsNull(ui->sbDispoAnlegen->value(), ui->btn_OkKontoAnlegen);

    ui->lblStartKapAnlegen->setEnabled(true);
    ui->sbStartKapAnlegen->setEnabled(true);
    ui->lblDispoAnlegen->setVisible(true);
    ui->sbDispoAnlegen->setVisible(true);
    fadeInGuiElement(ui->lblDispoAnlegen, 300);
    fadeInGuiElement(ui->sbDispoAnlegen, 300);
    fadeInGuiElement(ui->lblBetragEuroDispoKontoAnlegen,300);
    fadeInGuiElement(ui->lblPflichtfeldKontoAnlegen, 300);

}
    void FrmMain::on_radBtnSpar_clicked()
{
    ui->btn_OkKontoAnlegen->setEnabled(true);
    ui->lblStartKapAnlegen->setEnabled(true);
    ui->sbStartKapAnlegen->setEnabled(true);

    if(ui->lblDispoAnlegen->isEnabled() && ui->sbDispoAnlegen->isEnabled()) {
        fadeOutGuiElement(ui->lblDispoAnlegen, 300);
        fadeOutGuiElement(ui->sbDispoAnlegen, 300);
        fadeOutGuiElement(ui->lblBetragEuroDispoKontoAnlegen,300);
        fadeOutGuiElement(ui->lblPflichtfeldKontoAnlegen, 300);
    }
}

void FrmMain::on_cBoxEmpfaenger_currentIndexChanged(const QString &kontoString)
{
    QString kontoNr = kontoString;
    kontoNr.remove(QRegularExpression("[^0-9]")); // Remove non-numeric characters
    kontoNr.remove(QRegularExpression("^0+"));  // Remove leading zeros
    empfKontoNr = kontoNr.toInt();
}


void FrmMain::on_btnClearHistorie_clicked()
{
    ui->tableHistorie->clearContents();
    ui->tableHistorie->setRowCount(0);
    ui->btnClearHistorie->setEnabled(false);
}


void FrmMain::on_btnLogin_clicked()
{
    if (!ui->leUserName->text().isEmpty() && !ui->lePass->text().isEmpty()) {

        QString username = ui->leUserName->text(); //TO DO: Add validation, length, characters, etc
        QString pass = ui->lePass->text(); //TO DO: Add validation, length, characters, etc
        pass = users->hashPass(pass);

        User* user = users->holenMitUsername(username);

        if(user && user->getPass() == pass)
        {
            loggedUser = user;

            this->konten = new KontenListe(users->getDB(), loggedUser->getUsername());
//            testKontenAnlegen();
            kontenAnzeigen();

            exitLoginModus();
            enterHomeModus();
        }
        else
        {
            QMessageBox::warning(this, "Login - Fehler", "Username und Pass richtig eingeben");
        }
    } else {
        QMessageBox::warning(this, "Login - Fehler", "Bitte alle Felder ausfüllen");
    }
}


void FrmMain::on_btnAusloggen_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Ausloggen", "Wollen Sie sich wirklich ausloggen?", QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {

        konten->kontenlisteLeeren();

        transitionLogout();
    }
}


void FrmMain::on_btnUserVerwalten_clicked()
{
    ui->widgetHome->setVisible(false);
    fadeInGuiElement(ui->widgetUserVerwalten, 300);

    ui->btnUserVerwalten->setEnabled(false);
    ui->btnBankkonten->setEnabled(true);
    ui->widgetUserVerwalten->setVisible(true);
    //Stammdaten:
    ui->lblUsernameStammdaten->setText(loggedUser->getUsername());
    ui->lblVornameStammdaten->setText(loggedUser->getVorname());
    ui->lblNachnameStammdaten->setText(loggedUser->getNachname());
    //Konten Übersicht:
    ui->lblAnzGiroUsrVer->setText("(" + QString::number(konten->zaehleGirokonten()) + ")");
    ui->lblAnzSparUsrVer->setText("(" + QString::number(konten->zaehleSparkonten()) + ")");
    ui->lblAnzKontenUsrVer->setText("(" + QString::number(konten->zaehleGirokonten() + konten->zaehleSparkonten()) + ")");
    ui->lblGesamtkontenstand->setText(QString::number(konten->getGesamtKontenstand(), 'f', 2) + " €"); // TO DO: use / change toString method
    ui->lblGesamtGirokontenstand->setText(QString::number(konten->getGesamtGirokontenstand(), 'f', 2) + " €");
    ui->lblGesamtSparkontenstand->setText(QString::number(konten->getGesamtSparkontenstand(), 'f', 2) + " €");
}


void FrmMain::on_btnBankkonten_clicked()
{
    ui->widgetHome->setVisible(true);
    if(ui->tabWidgetKonten->currentIndex() == 2) {
        ui->tabOperationen->setVisible(false);
        fadeInGuiElement(ui->tabWidgetKontotypInfos,300);
    }
    ui->btnUserVerwalten->setEnabled(true);
    ui->btnBankkonten->setEnabled(false);
    fadeOutGuiElement(ui->widgetUserVerwalten, 100);
}


void FrmMain::on_btnNeuerUser_clicked() // TO DO: choose a better name for this btn, it is confusing with btnUserAnlegen
{
    fadeOutGuiElement(ui->widgetLogin,500);
    fadeInGuiElement(ui->widgetNeuerUser, 500);
//    ui->leUserName->clear();
    ui->lePass->clear();
    ui->lblPassAnlegen2->setVisible(false);
    ui->lePassAnlegen2->setVisible(false);
}


void FrmMain::on_btnAbbUserAnlegen_clicked()
{
    fadeOutGuiElement(ui->widgetNeuerUser, 500);
    fadeInGuiElement(ui->widgetLogin,500);
    ui->leUserNameAnlegen->clear();
    ui->leVornameAnlegen->clear();
    ui->leNachameAnlegen->clear();
    ui->lePassAnlegen->clear();
    ui->lePassAnlegen2->clear();
}

void FrmMain::on_btnUserAnlegen_clicked() // TO DO: TEST if works fine
{
    QString username = ui->leUserNameAnlegen->text(); //TO DO: Add validation, length, characters, etc
    QString vorname = ui->leVornameAnlegen->text(); //TO DO: Add validation, length, characters, etc
    QString nachname = ui->leNachameAnlegen->text(); //TO DO: Add validation, length, characters, etc
    QString pass = ui->lePassAnlegen->text(); //TO DO: Add validation, length, characters, etc
    QString pass2 = ui->lePassAnlegen2->text(); //TO DO: Add validation, length, characters, etc

    if(ui->leUserNameAnlegen->text().isEmpty() ||
            ui->lePassAnlegen->text().isEmpty() ||
            ui->lePassAnlegen2->text().isEmpty() ||
            ui->leVornameAnlegen->text().isEmpty() ||
            ui->leNachameAnlegen->text().isEmpty()) {
        QMessageBox::warning(this, "User Anlegen - Fehler", "Bitte alle Felder ausfühlen");
    }
    else if(pass == pass2) {      
        if (users->userAnlegen(new User(username, vorname, nachname, pass))) {
            QMessageBox::information(this, "User anlegen - Erfolg", username + " wurde erfolgreich als neuer User angelegt");

            ui->leUserNameAnlegen->clear();
            ui->leVornameAnlegen->clear();
            ui->leNachameAnlegen->clear();
            ui->lePassAnlegen->clear();
            ui->lePassAnlegen2->clear();

            ui->leUserName->setText(username);
            fadeOutGuiElement(ui->widgetNeuerUser,500);
            fadeInGuiElement(ui->widgetLogin,500);
        }
        else {
            QMessageBox::warning(this, "User anlegen - Fehler", "Username schon vorhanden! Einen neuen eingeben!");
            ui->leUserNameAnlegen->clear();
            ui->lePassAnlegen->clear();
            ui->lePassAnlegen2->clear();
        }
    }
    else {
        QMessageBox::warning(this, "User Anlegen - Fehler", "Bitte das Passwort richtig wiederholen");
        ui->lePassAnlegen2->clear();
    }
}


void FrmMain::on_btnLoginDebugOhneUsername_clicked()
{
    if(users->zaehleEintragen() > 0) {

        int randIdx = QRandomGenerator::global()->bounded(users->zaehleEintragen());
        loggedUser = users->holenMitIndex(randIdx); // Login als random User

        this->konten = new KontenListe(users->getDB(), loggedUser->getUsername());
        kontenAnzeigen();

        ui->widgetHome->setVisible(true);

        ui->widgetLogin->setVisible(false);
        ui->widgetNeuerUser->setVisible(false);
        ui->leUserName->clear();
        ui->lePass->clear();

        ui->btnAusloggen->setVisible(true);
        ui->widgetLogin->setVisible(false);
        ui->btnBankkonten->setVisible(true);
        ui->btnUserVerwalten->setVisible(true);

        ui->btnLoginDebugOhneUsername->setVisible(false);

        ui->lblUsernameStammdaten->setText(loggedUser->getUsername());
        ui->lblVornameStammdaten->setText(loggedUser->getVorname());
        ui->lblNachnameStammdaten->setText(loggedUser->getNachname());

        animateUserNameLbl();

        fadeInGuiElement(ui->lblEmojiUser, 1500);
        fadeInGuiElement(ui->tabWidgetOperationen, 1500);
    } else QMessageBox::warning(this, "Login - Fehler", "Mind. 1 User anlegen");
}

void FrmMain::on_lePass_returnPressed()
{
    on_btnLogin_clicked();
}


void FrmMain::on_leUserName_returnPressed()
{
    on_btnLogin_clicked();
}

void FrmMain::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape && ui->btnAusloggen->isVisible())
    {
        on_btnAusloggen_clicked();
    }
}


void FrmMain::on_btnCsvExport_clicked()
{
    QMessageBox::StandardButton reply;
    QMessageBox msgBox;
    reply = msgBox.question(this, "CSV exportieren", "Wollen Sie wirklich die Konteninformationen in eine CSV Datei exportieren?",
                            QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        QString folderPath = QFileDialog::getExistingDirectory(nullptr, "Select Folder to Save CSV", QDir::homePath());
        if (!folderPath.isEmpty()) {
            QString filePath = folderPath + "/konten_" + loggedUser->getNachname() + "_" + loggedUser->getVorname() + ".csv";
            if (konten->csvExportieren(filePath, loggedUser->getUsername())) {
                QMessageBox::information(this, "CSV exportieren", "Die Konteninformationen wurden erfolgreich in " + folderPath +
                                         "/konten.csv exportiert!");
            } // if: csv exportieren erfolgreich
            else {
                QMessageBox::critical(this, "CSV exportieren - Fehler", "Fehler beim Speichern der Datei, überprüfen Sie bitte, "
                                      "ob die Datei schon geöffnet ist");
            } // else
        } // if: Dateipfad ausgewählt
    } // if: Antwort vom User == yes
}

void FrmMain::on_sbDispoAnlegen_valueChanged(double arg1)
{
    toggleBtnSichtbarkeitWennGroesserAlsNull(arg1, ui->btn_OkKontoAnlegen);
}

void FrmMain::toggleBtnSichtbarkeitWennGroesserAlsNull(double value, QWidget* button)
{
    if(value > 0 || (typeid(value) != typeid(double))) { // Keine negative Werte oder nicht-double Inputs zulassen
        static_cast<QPushButton*>(button)->setEnabled(true);
    }
    else {
        static_cast<QPushButton*>(button)->setEnabled(false);
    }
}

void FrmMain::toggleSichtbarkeitWennUsernameOderPassNotEmpty(QString username, QString pass, QWidget* button)
{
    if (username.isEmpty() || pass.isEmpty()) { // TO DO: merge with function "toggleBtnSichtbarkeit.... " now it only works with ints, but make it also work with Strings
        button->setEnabled(false);
    } else button->setEnabled(true);
}

void FrmMain::animateUserNameLbl()
{
    ui->lblUserTitel->setText("Willkommen " + loggedUser->getVorname());
    ui->lblEmojiUser->setVisible(true);
    ui->tableGirokonten->scrollToTop();
    ui->tableSparkonten->scrollToTop();

    fadeInGuiElement(ui->lblEmojiUser, 1500); // TO DO: check if this is ok here, before it was outside this function

    // Animation: zuerst der Text "Wilkommen Vorname" fades in, danach fades out, danach der Text "Vorname Nachname" fades in
    fadeInGuiElement(ui->lblUserTitel, 800);
    QTimer::singleShot(1500, [=]() { // Lambda Function, 1,5 Sek warten und danach Fadeout ausführen
        fadeOutGuiElement(ui->lblUserTitel, 900);
        QTimer::singleShot(1000, [=]() { // Lambda Function, 1 Sek warten und danach weitere Statements ausführen
            ui->lblUserTitel->setText(loggedUser->getVorname() + " " + loggedUser->getNachname());
            fadeInGuiElement(ui->lblUserTitel, 800);
        });
    });
}

bool FrmMain::meldungenBeimAuszahlen(double betrag)
{
    bool erfolg = false;

    // SPARKONTO
    if (markierteKontoNr % 100 == 99){
        // FEHLER -> BETRAG > KONTOSTAND
        if(betrag > markierteSparkonto->getKontostand()) {
            QMessageBox::warning(this, "Auszahlung - Fehler", "Auszahlungsbetrag kann bei Sparkonten nicht größer als Kontostand sein");
        }
        // WARNUNG -> KONTOSTAND = 0 nach Auszahlung
        else if (betrag == markierteSparkonto->getKontostand()) {
            QMessageBox::StandardButton reply = QMessageBox::question(this, "Auszahlzung", "Ihr Kontostand wird nach der Auszahlung 0 sein, wollen Sie trotzdem die Auszahlung durchführen?");
            if (reply == QMessageBox::Yes) {
//                return true;
                erfolg = true;
            } else {
//                return false;
                erfolg = false;
            }
        }
        else erfolg = true;
    }
    // GIROKONTO
    else if (markierteKontoNr % 100 == 1) {
        // FEHLER -> BETRAG > DISPO + KONTOSTAND
        if(betrag > markierteGirokonto->getKontostand() + markierteGirokonto->getDispo()) {
            QMessageBox::warning(this, "Auszahlung - Fehler", "Auszahlungsbetrag kann bei Girokonto nicht größer als Kontostand + Dispo sein");
        }
        // WARNUNG -> KONTOSTAND = 0 nach AUSZAHLUNG
        else if (betrag == markierteGirokonto->getKontostand()) {
            QMessageBox::StandardButton reply = QMessageBox::question(this, "Auszahlzung", "Ihr Kontostand wird nach der Auszahlung 0 sein, wollen Sie trotzdem die Auszahlung durchführen?");
            if (reply == QMessageBox::Yes) {
//                return true;
                erfolg = true;
            } else {
//                return false;
                erfolg = false;
            }
        }
        // WARNUNG -> KONTOSTAND = DISPO, nach Auszahlung
        else if (betrag == markierteGirokonto->getKontostand() + markierteGirokonto->getDispo()) {
            QMessageBox::StandardButton reply = QMessageBox::question(this, "Auszahlzung", "Ihr Kontostand wird nach der Auszahlung das Dispo Limit erreichen, wollen Sie trotzdem die Auszahlung durchführen?");
            if (reply == QMessageBox::Yes) {
//                return true;
                erfolg = true;
            } else {
//                return false;
                erfolg = false;
            }
        }
        // WARNUNG -> NEGATIVER KONTOSTAND, nach Auszahlung
        else if (betrag > markierteGirokonto->getKontostand() && markierteGirokonto->getKontostand() >= 0) {
            QMessageBox::StandardButton reply = QMessageBox::question(this, "Auszahlzung", "Ihr Kontostand wird nach der Auszahlung negativ sein, wollen Sie trotzdem die Auszahlung durchführen?");
            if (reply == QMessageBox::Yes) {
//                return true;
                erfolg = true;
            } else {
//                return false;
                erfolg = false;
            }
        }
        else erfolg = true;
    }

    // DEFAULT (positiver Kontostand, nach Auszahlung)
    qDebug() << "ANSWER TO MESSAGEBOX: " << erfolg;
    return erfolgMeldungBeimAuszahlen(erfolg);

}

bool FrmMain::erfolgMeldungBeimAuszahlen(bool reply) // TO DO: Maybe I dont need this method
{
    if (reply) {
        QString opModStr;
        opModus == Auszahlung? opModStr = "Auszahlung" : opModStr = "Überweisung";
        QMessageBox::information(this, opModStr + " - Erfolg", "Die " + opModStr + " wurde erfolgreich durchgeführt");
        return true;
    }
    return false;
}

void FrmMain::setTabellenWidth()
{
    ui->tableGirokonten->setColumnWidth(0, 120);
    ui->tableGirokonten->setColumnWidth(1, 115);
    ui->tableGirokonten->setColumnWidth(2, 155);
    ui->tableSparkonten->setColumnWidth(0, 118);
    ui->tableSparkonten->setColumnWidth(1, 130);
    ui->tableSparkonten->setColumnWidth(2, 185);
    ui->tableHistorie->setColumnWidth(0, 150);
    ui->tableHistorie->setColumnWidth(1, 300);
    ui->tableHistorie->setColumnWidth(2, 135);
    ui->tableHistorie->setColumnWidth(3, 115);
}

void FrmMain::on_tabWidgetKonten_currentChanged(int index)
{
    if (index == 2)
    {
        fadeOutGuiElement(ui->tabWidgetOperationen, 300);
        fadeInGuiElement(ui->tabWidgetKontotypInfos,300);
        ui->tabWidgetKonten->setFixedWidth(700);
    }
    else
    {
        if(index == 1) {
            ui->tabWidgetKonten->setFixedWidth(950);
        }
        else {
            ui->tabWidgetKonten->setFixedWidth(1001);
        }
        if(!ui->tabWidgetOperationen->isEnabled()) {
             fadeInGuiElement(ui->tabWidgetOperationen, 300);
             fadeOutGuiElement(ui->tabWidgetKontotypInfos,300);
        }
    }
}


void FrmMain::on_leUserName_textChanged(const QString &arg1)
{
    toggleSichtbarkeitWennUsernameOderPassNotEmpty(arg1, ui->lePass->text(), ui->btnLogin);
}

void FrmMain::on_lePass_textChanged(const QString &arg1)
{
    toggleSichtbarkeitWennUsernameOderPassNotEmpty(ui->leUserName->text(), arg1, ui->btnLogin);
}


void FrmMain::on_lePassAnlegen_textChanged(const QString &arg1)
{
    //TO DO Make a better function
    if (arg1.isEmpty()) { // TO DO: merge with function "toggleBtnSichtbarkeit.... " now it only works with ints, but make it also work with Strings
        ui->lblPassAnlegen2->setVisible(false);
        ui->lePassAnlegen2->setVisible(false);
    } else {
        ui->lblPassAnlegen2->setVisible(true);
        ui->lePassAnlegen2->setVisible(true);
    }
}


void FrmMain::on_tabWidgetOperationen_currentChanged(int index)
{
    if (index == 0) {
        ui->tabWidgetOperationen->setFixedWidth(421);
    }else if (index == 1) {
        ui->tabWidgetOperationen->setFixedWidth(401);
    }
}

void FrmMain::on_btn_OkKontoAnlegen_clicked()
{
    bool ok = false;
    QString fehlermeldungTitel = "Konto anlegen - Fehler";
    double startKapital = ui->sbStartKapAnlegen->value();
    double dispo = ui->sbDispoAnlegen->value();
    int angelegtesKontoNr = 0; // TO DO : Maybe I do not need it

    if(startKapital > 0) { // Startkapital > 0
        if (ui->radBtnKeinKonto->isChecked()){
            QMessageBox::critical(this, fehlermeldungTitel, "Bitte einen Kontotyp auswählen");
            ok = false;
        }
        else if(ui->radBtnGiro->isChecked()) {   // GIRO
            if(dispo > 0) { // Dispo > 0
                ok = true;
            } else {    // Dipso <= 0
                QMessageBox::warning(this, fehlermeldungTitel, "Bitte einen Dispo betrag eingeben!");
                ok = false;
            }
        }
        else if(ui->radBtnSpar->isChecked()) {  // SPAR
            ok = true;
        }
    }
    else if(startKapital == 0) {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Konto ohne Startkapital anlegen",
                                                                  "Wollen Sie ein neues Konto <b>ohne Startkapital</b> anlegen?",
                                                                  QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::Yes) {
            ok = true;
        }
        else {
            ok = false;
        }
    }
    else {    // Startkapital < 0
        QMessageBox::warning(this, fehlermeldungTitel, "Das Startkapital kann nicht negativ sein!");
        ok = false;
    }
    if (ok) {   // Konto anlegen
        ;
        if(ui->radBtnGiro->isChecked()) {   // GIROKonto anlegen
            angelegtesKontoNr = konten->girokontoAnlegen(startKapital,dispo,loggedUser->getUsername(), loggedUser->getId());
            ui->sbDispoAnlegen->setValue(0);;
        }
        else {  // SPARKonto anlegen
            angelegtesKontoNr = konten->sparkontoAnlegen(startKapital, loggedUser->getUsername(), loggedUser->getId());
        }

        kontoAnlegenGuiDeaktivieren();
        kontenAnzeigen();
        opInHistorieHinzufuegen(konten->kontoHolenMitIndex(konten->zaehleKonten() -1)->toString(),"Anlegen",startKapital,startKapital);

        QMessageBox::information(this, "Konto anlegen", "Konto " + konten->kontoHolenMitIndex(konten->zaehleKonten() -1)->toString() + " erfolgreich angelegt!");
    }
}

void FrmMain::on_btn_OkOperation_clicked()
{
    bool ok = false;
    if (ui->sboxBetrag->value() > 0) {
        double betrag = ui->sboxBetrag->value();
        double neuerKontostand;
        ui->sboxBetrag->setValue(0);

        switch (opModus) {
            case Einzahlung:
                QMessageBox::information(this, "Einzahlung - Erfolg", "Die Einzahlung wurde erfolgreich durchgeführt");
//                neuerKontostand = konten->kontostandAendern(betrag, markierteKontoNr);
                neuerKontostand = konten->einzahlen(markierteKontoNr, betrag);
                opInHistorieHinzufuegen(QString::number(markierteKontoNr),"Einzahlung", betrag, neuerKontostand);
                break;
            case Auszahlung:
                ok = meldungenBeimAuszahlen(betrag);

                qDebug() << markierteGirokonto->getKontostand();
                qDebug() << betrag;

                if (ok) {
//                    neuerKontostand = konten->kontostandAendern(-betrag, markierteKontoNr);
                    neuerKontostand = konten->auszahlen(markierteKontoNr, betrag);
                    opInHistorieHinzufuegen(QString::number(markierteKontoNr),"Auszahlung", -betrag, neuerKontostand);
                }
                break;
            case Ueberweisung:
                ok = meldungenBeimAuszahlen(betrag);
                if(ok) {
//                    double neuerKontostandSender = konten->kontostandAendern(-betrag, markierteKontoNr);
                    double neuerKontostandSender = konten->auszahlen(markierteKontoNr, betrag);

                    double neuerKontostandEmpfpaenger = konten->einzahlen(empfKontoNr, betrag);
                    opInHistorieHinzufuegen(QString::number(markierteKontoNr),"Überweisung (senden)", -betrag, neuerKontostandSender);
                    opInHistorieHinzufuegen(QString::number(empfKontoNr),"Überweisung (empfangen)", betrag, neuerKontostandEmpfpaenger);
                }
                break;
        }
        kontenAnzeigen();
        operationModusDeaktivieren();
        ui->btnAbbrechen->setVisible(false);
        ui->btn_OkOperation->setVisible(false);

    }
    else QMessageBox::warning(this, "Operationen - Fehler", "Bitte einen Betrag eingeben");
}
