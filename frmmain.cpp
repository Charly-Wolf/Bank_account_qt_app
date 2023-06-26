#include "frmmain.h"
#include "ui_frmmain.h"
#include <QMessageBox>
#include <QGraphicsOpacityEffect>
#include <QKeyEvent>

#include <QFileDialog> // Für das Exportieren auf CSV
#include <QLocale> // Für die Formatierung von N<ummern --->> TODO: do that in the model toString method

FrmMain::FrmMain(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FrmMain)
{
    ui->setupUi(this);

    QString datenbankDatei = "dbBankkonto2.sqlite";
    setWindowTitle("Bankkonto App");
    this->users = new UsersListe(datenbankDatei);
    initialWidgetsState();
}

FrmMain::~FrmMain()
{
    delete ui;
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

        tableItem = new QTableWidgetItem(kontoNrFormatieren(girokonto->toString()));
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

        tableItem = new QTableWidgetItem(kontoNrFormatieren(sparkonto->toString()));
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

void FrmMain::opInHistorieHinzufuegen(int kontoNr, QString operation, double betrag, double kontostand)
{
    //TODO: User the tblOperationen from DB and create a class and list to keep track of operations during time
    int anzZeilen = ui->tableHistorie->rowCount();

    QString kontoNrStr = QString::number(kontoNr);
    int anzCharsKontoNr = kontoNrStr.length() - 1;
    kontoNrStr = kontoNrStr.insert(anzCharsKontoNr-1, "-").rightJustified(8, '0');
    QString betragStr = QLocale().toString(betrag) + " €";
    QString kontostandStr = QLocale().toString(kontostand) + " €";
    QTableWidgetItem* tableItem;

    ui->tableHistorie->insertRow(anzZeilen);

    tableItem = new QTableWidgetItem(kontoNrStr);
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

void FrmMain::initialWidgetsState()
{
    activeWidget = ui->widgetLogin;

    ui->radBtnKeinKonto->setVisible(false);
    ui->btnKontoAnlegen->setEnabled(false);
    ui->sbStartKapAnlegen->setEnabled(false);
    ui->sbStartKapAnlegen->setValue(0);
    ui->sbDispoAnlegen->setValue(0);
    ui->lblPflichtfeldKontoAnlegen->setVisible(false);

    ui->lblPassAnlegen2->setVisible(false);
    ui->lePassAnlegen2->setVisible(false);
    ui->widgetHome->setVisible(false);
    ui->widgetTschuess->setVisible(false);
    ui->lblMarkKontostand->setVisible(false);
    ui->lblMarkKontostandTitel->setVisible(false);
    ui->leUserName->setFocus();
    ui->btnAusloggen->setVisible(false);
    ui->btnUserVerwalten->setVisible(false);
    ui->btnBankkonten->setVisible(false);
    ui->widgetUserVerwalten->setVisible(false);
    ui->radBtnGiro->setChecked(false);
    ui->radBtnSpar->setChecked(false);
    ui->radBtnKeinKonto->setChecked(true);
//    ui->widgetLogin->setVisible(true);
    fadeInGuiElement(ui->widgetLogin, 600);
    ui->lblUserTitel->clear();
    ui->cBoxEmpfaenger->setVisible(false);
    ui->lblEmpf->setVisible(false);
    ui->widgetNeuerUser->setVisible(false);
    ui->widgetPassAendern->setVisible(false);
    ui->btnUserVerwalten->setEnabled(true);
    ui->btnBankkonten->setEnabled(false);
    ui->lblBetrag->setVisible(false);
    ui->lblBetragEuro->setVisible(false);
    ui->sboxBetrag->setVisible(false);
    ui->btnAbbrechen->setVisible(false);
    ui->btnOk->setVisible(false);
    ui->lblDispoAnlegen->setVisible(false);
    ui->sbDispoAnlegen->setVisible(false);
    ui->lblBetragEuroDispoKontoAnlegen->setVisible(false);
    ui->tabWidgetKontotypInfos->setVisible(false);
    ui->lblEmojiUser->setVisible(false);
    ui->btnAuszahlung->setEnabled(false);
    ui->btnEinzahlung->setEnabled(false);
    ui->btnUeberw->setEnabled(false);


    ui->lblMarkiertesKonto->setText("Markieren Sie ein Konto \naus der Konten-Tabellen");

    markierteGirokonto = NULL;
    markierteSparkonto = NULL;
    markierteKontoNr = NULL;

    ui->tabWidgetKonten->setCurrentIndex(0);
    ui->tabWidgetOperationen->setCurrentIndex(0);

    ui->tableGirokonten->setColumnWidth(0, 110);
    ui->tableGirokonten->setColumnWidth(1, 115);
    ui->tableGirokonten->setColumnWidth(2, 155);
    ui->tableSparkonten->setColumnWidth(0, 115);
    ui->tableSparkonten->setColumnWidth(1, 130);
    ui->tableSparkonten->setColumnWidth(2, 185);

    ui->tableHistorie->setColumnWidth(0, 110);
    ui->tableHistorie->setColumnWidth(1, 300);
    ui->tableHistorie->setColumnWidth(2, 135);
    ui->tableHistorie->setColumnWidth(3, 115);


    // Animation für den Banknamen
    //    fadeInGuiElement(ui->lblBankName, 1700);
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


QString FrmMain::kontoNrFormatieren(QString kontoStr)
{
    return kontoStr.right(8); //Nur KontoNr mit Formatierung (ohne Kontoart)
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
    ui->cBoxEmpfaenger->clear();
    ui->tableGirokonten->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableSparkonten->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->cBoxEmpfaenger->setVisible(false);
    ui->lblBetrag->setVisible(false);
    ui->lblBetragEuro->setVisible(false);
    ui->sboxBetrag->setVisible(false);
    ui->lblEmpf->setVisible(false);
}

void FrmMain::operationModusAktivieren()
{
    //TODO
}

void FrmMain::empfaengerKontenLaden(int MarkiertesKontoNr)
{
    ui->cBoxEmpfaenger->clear();
    ui->btnUeberw->setEnabled(true);

    int anz = konten->zaehleKonten();
    for(int i = 0; i < anz; i++) {
        if(konten->kontoHolen(i)->getKontoNr() != MarkiertesKontoNr)
            ui->cBoxEmpfaenger->addItem(konten->kontoHolen(i)->toString() + " -- " + konten->kontoHolen(i)->getInhaberUsername());
    }
}

void FrmMain::kontoAnlegenGuiDeaktivieren()
{
    ui->lblStartKapAnlegen->setEnabled(false);
    ui->sbStartKapAnlegen->setEnabled(false);
    ui->btnKontoAnlegen->setEnabled(false);
    ui->radBtnGiro->setChecked(false);
    ui->radBtnSpar->setChecked(false);
    ui->radBtnKeinKonto->setChecked(true);
    ui->lblDispoAnlegen->setVisible(false);
    ui->sbDispoAnlegen->setVisible(false);
    ui->lblBetragEuroDispoKontoAnlegen->setVisible(false);
}

void FrmMain::on_tableGirokonten_itemSelectionChanged()
{
    int index = ui->tableGirokonten->currentRow();
    markierteGirokonto = konten->girokontoHolen(index);
    markierteKontoNr = markierteGirokonto->getKontoNr();

    ui->lblMarkiertesKonto->setText(markierteGirokonto->toString());
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
    ui->lblMarkiertesKonto->setText(markierteSparkonto->toString());
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
    //TODO: simplify gui functions
    ui->btnOk->setEnabled(true);
    ui->btnAbbrechen->setEnabled(true);
    ui->tableGirokonten->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableSparkonten->setSelectionMode(QAbstractItemView::NoSelection);
    ui->btnEinzahlung->setEnabled(false);
    ui->btnAuszahlung->setEnabled(false);
    ui->btnUeberw->setEnabled(false);
    ui->cBoxEmpfaenger->setVisible(true);
    ui->lblEmpf->setVisible(true);
    ui->lblBetrag->setVisible(true);
    ui->lblBetragEuro->setVisible(true);
    ui->sboxBetrag->setVisible(true);
    ui->btnAbbrechen->setVisible(true);
    ui->btnOk->setVisible(true);
    ui->btnOk->setText("✔  Überweisen");

    opModus = Ueberweisung;
}

void FrmMain::on_btnAbbrechen_clicked()
{
    ui->sboxBetrag->setValue(0);
    operationModusDeaktivieren();
    ui->btnAbbrechen->setVisible(false);
    ui->btnOk->setVisible(false);
    ui->lblMarkiertesKonto->setText("Markieren Sie ein Konto \naus der Konten-Tabellen");
    ui->lblMarkKontostand->setVisible(false);
    ui->lblMarkKontostandTitel->setVisible(false);
}

void FrmMain::on_btnEinzahlung_clicked()
{

    //TODO: simplify gui functions
    ui->tableGirokonten->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableSparkonten->setSelectionMode(QAbstractItemView::NoSelection);
    ui->btnEinzahlung->setEnabled(false);
    ui->btnAuszahlung->setEnabled(false);
    ui->btnUeberw->setEnabled(false);
    ui->btnOk->setEnabled(true);
    ui->btnAbbrechen->setEnabled(true);

    ui->lblBetrag->setVisible(true);
    ui->lblBetragEuro->setVisible(true);
    ui->sboxBetrag->setVisible(true);
    ui->btnAbbrechen->setVisible(true);
    ui->btnOk->setVisible(true);
    ui->btnOk->setText("✔  Einzahlen");

    opModus = Einzahlung;
}


void FrmMain::on_btnAuszahlung_clicked()
{
    //TODO: simplify gui functions
    ui->tableGirokonten->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableSparkonten->setSelectionMode(QAbstractItemView::NoSelection);
    ui->btnEinzahlung->setEnabled(false);
    ui->btnAuszahlung->setEnabled(false);
    ui->btnUeberw->setEnabled(false);
    ui->btnOk->setEnabled(true);
    ui->btnAbbrechen->setEnabled(true);
    ui->lblBetrag->setVisible(true);
    ui->lblBetragEuro->setVisible(true);
    ui->sboxBetrag->setVisible(true);
    ui->btnAbbrechen->setVisible(true);
    ui->btnOk->setVisible(true);
    ui->btnOk->setText("✔  Auszahlen");

    opModus = Auszahlung;
}


void FrmMain::on_btnOk_clicked()
{
    bool ok = false;
    if (ui->sboxBetrag->value() > 0) {
        double betrag = ui->sboxBetrag->value();
        double neuerKontostand;
        ui->sboxBetrag->setValue(0);

        switch (opModus) {
            case Einzahlung:
                neuerKontostand = konten->kontostandAendern(betrag, markierteKontoNr);
                opInHistorieHinzufuegen(markierteKontoNr,"Einzahlung", betrag, neuerKontostand);
                break;
            case Auszahlung:
                ok = meldungenBeimAuszahlen(betrag);
                if (ok) { // BUG: even when there is a warning or a question and we press no, the operation will be executed.... it should not be like that
                    neuerKontostand = konten->kontostandAendern(-betrag, markierteKontoNr);
                    opInHistorieHinzufuegen(markierteKontoNr,"Auszahlung", -betrag, neuerKontostand);
                }
                break;
            case Ueberweisung:
                ok = meldungenBeimAuszahlen(betrag);
                if(ok) {
                    double neuerKontostandSender = konten->kontostandAendern(-betrag, markierteKontoNr);
                    double neuerKontostandEmpfpaenger = konten->kontostandAendern(betrag, empfKontoNr);
                    opInHistorieHinzufuegen(markierteKontoNr,"Überweisung (senden)", -betrag, neuerKontostandSender);
                    opInHistorieHinzufuegen(empfKontoNr,"Überweisung (empfangen)", betrag, neuerKontostandEmpfpaenger);
                }
                break;
        }
        kontenAnzeigen();
        operationModusDeaktivieren();
        ui->btnAbbrechen->setVisible(false);
        ui->btnOk->setVisible(false);
        ui->lwDebug->addItem("NEUER GIROKONTO STAND: " + QString::number(markierteGirokonto->getKontostand()));//DEBUG

    }
    else QMessageBox::warning(this, "Operationen - Fehler", "Bitte einen Betrag eingeben");
}


void FrmMain::on_btnKontoAnlegen_clicked()
{
    if(ui->sbStartKapAnlegen->value() >= 0) {

        double startKapital = ui->sbStartKapAnlegen->value();

        if(startKapital == 0 && (ui->sbDispoAnlegen->value() > 0 || ui->radBtnSpar->isChecked())) {
            QMessageBox::StandardButton reply = QMessageBox::question(this, "Ausloggen", "Wollen Sie ein neues Konto <b>ohne Startkapital</b> anlegen?", QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::No) return;
        }

        int kontoNr = 0;

        if(ui->radBtnGiro->isChecked()) {
            if(ui->sbDispoAnlegen->value() > 0) {
                double dispo = ui->sbDispoAnlegen->value();
                kontoNr = konten->girokontoAnlegen(startKapital, dispo, loggedUser->getUsername());
                ui->sbDispoAnlegen->setValue(0);
                ui->tableGirokonten->scrollToBottom();
            }
            else{
                QMessageBox::warning(this, "Konto anlegen - Fehler", "Bitte Dispo eingeben");
                return;
            }
        }
        else if (ui->radBtnSpar->isChecked()) {
            kontoNr = konten->sparkontoAnlegen(startKapital, loggedUser->getUsername());
            ui->tableSparkonten->scrollToBottom();
        }

        ui->sbStartKapAnlegen->setValue(0);
        kontoAnlegenGuiDeaktivieren();
        kontenAnzeigen();
        opInHistorieHinzufuegen(kontoNr,"Anlegen",startKapital,startKapital);
        QMessageBox::information(this, "Konto anlegen", "Konto " + QString::number(kontoNr).insert(QString::number(kontoNr).length()-2, "-").rightJustified(8, '0') + " erfolgreich angelegt!"); // TODO: create a function for formatting KontoNr (in Model)
        ui->tabWidgetKonten->setCurrentIndex(0);
    }
    else {
        QMessageBox::warning(this, "Konto anlegen - Fehler", "Bitte Startkapital eingeben");
    }
}


void FrmMain::on_radBtnGiro_clicked()
{
    toggleBtnSichtbarkeitWennGroesserAlsNull(ui->sbDispoAnlegen->value(), ui->btnKontoAnlegen);


    ui->lblStartKapAnlegen->setEnabled(true);
    ui->sbStartKapAnlegen->setEnabled(true);
    ui->lblDispoAnlegen->setVisible(true);
    ui->sbDispoAnlegen->setVisible(true);

    fadeInGuiElement(ui->lblDispoAnlegen, 300);
    fadeInGuiElement(ui->sbDispoAnlegen, 300);
    fadeInGuiElement(ui->lblBetragEuroDispoKontoAnlegen,300);
    fadeInGuiElement(ui->lblPflichtfeldKontoAnlegen, 300);

//BUG: when first cliking on sparkonto radbtn and then comming back to giro, if dispo is null, the butons for abb und ok are enabled... they should be disabled
//    toggleBtnSichtbarkeitWennGroesserAlsNull(ui->sbStartKapAnlegen->value(), ui->btnKontoAnlegen);
//    toggleBtnSichtbarkeitWennGroesserAlsNull(ui->sbStartKapAnlegen->value(), ui->btnAbbKontoAnl);
}


void FrmMain::on_radBtnSpar_clicked()
{
    ui->btnKontoAnlegen->setEnabled(true);
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

        QString username = ui->leUserName->text(); //TODO: Add validation, length, characters, etc
        QString pass = ui->lePass->text(); //TODO: Add validation, length, characters, etc
        pass = users->hashPass(pass);

        User* user = users->holenMitUsername(username);

        if(user && user->getPass() == pass)
        {
            ui->widgetLogin->setVisible(false);
            ui->leUserName->clear();
            ui->lePass->clear();

            ui->btnAusloggen->setVisible(true);
            ui->widgetLogin->setVisible(false);
            ui->widgetNeuerUser->setVisible(false);
            ui->btnBankkonten->setVisible(true);
            ui->btnUserVerwalten->setVisible(true);

            ui->btnLoginDebugOhneUsername->setVisible(false);

            loggedUser = user;

            this->konten = new KontenListe(users->getDB(), loggedUser->getUsername());
            kontenAnzeigen();

//            debugMessage("LOGGED USER: " + loggedUser->getUsername());
//            debugMessage("DB ERROR: " + konten->outputError());


            ui->lblUsernameStammdaten->setText(loggedUser->getUsername());
            ui->lblVornameStammdaten->setText(loggedUser->getVorname());
            ui->lblNachnameStammdaten->setText(loggedUser->getNachname());

            userNameUndWillkommenAnimieren();

            ui->widgetHome->setVisible(true);

            fadeInGuiElement(ui->lblUserTitel, 1400);
            fadeInGuiElement(ui->lblEmojiUser, 1500);
            fadeInGuiElement(ui->btnBankkonten, 1500);
            ui->btnBankkonten->setEnabled(false);
            fadeInGuiElement(ui->btnUserVerwalten, 1500);
            fadeInGuiElement(ui->btnAusloggen, 1500);
            fadeInGuiElement(ui->tabWidgetOperationen, 1600);
            fadeInGuiElement(ui->btnCsvExport, 1700);
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
    QMessageBox::StandardButton reply;
    QMessageBox msgBox;

    reply = msgBox.question(this, "Ausloggen", "Wollen Sie sich wirklich ausloggen?", QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {

        konten->kontenlisteLeeren();

        //BUG: When logging out, logging in and then logging out again-> widgetTschuess does not appear
        ui->widgetTschuess->setVisible(true);
        ui->widgetHome->setVisible(false);

        // TODO: simplify
        ui->lblUserTitel->setVisible(false);
        ui->lblEmojiUser->setVisible(false);
        ui->btnAusloggen->setVisible(false);
        ui->btnBankkonten->setVisible(false);
        ui->btnUserVerwalten->setVisible(false);
        ui->widgetUserVerwalten->setVisible(false);

        operationModusDeaktivieren();

        ui->lblTschuess->setText("Auf Wiedersehen\n" + loggedUser->getVorname() + "!");
        fadeInGuiElement(ui->lblTschuess,600);
        activeWidget = ui->widgetTschuess;

        fadeOutGuiElement(ui->lblNextTitel, 600);
        fadeOutGuiElement(ui->lblBankTitel, 600);
        fadeOutGuiElement(ui->lblGeldEmoji, 600);

        QTimer::singleShot(1500, [=]() { // Lambda Function -> widgetTschuess für 1,5 Sekunden anzeigen
            fadeOutGuiElement(ui->widgetTschuess, 700);

            activeWidget = ui->widgetTschuess;
            ui->btnLoginDebugOhneUsername->setVisible(true);
            initialWidgetsState();
            ui->btnLogin->setEnabled(false);
            activeWidget = ui->widgetLogin;
        });
    }
}


void FrmMain::on_btnUserVerwalten_clicked()
{
    activeWidget = ui->widgetUserVerwalten;

    ui->widgetHome->setVisible(false);
    fadeInGuiElement(ui->widgetUserVerwalten, 300);

    ui->btnUserVerwalten->setEnabled(false);
    ui->btnBankkonten->setEnabled(true);
    ui->widgetUserVerwalten->setVisible(true);
    ui->lblAnzGiroUsrVer->setText("(" + QString::number(konten->zaehleGirokonten()) + ")");
    ui->lblAnzSparUsrVer->setText("(" + QString::number(konten->zaehleSparkonten()) + ")");
    ui->lblAnzKontenUsrVer->setText("(" + QString::number(konten->zaehleGirokonten() + konten->zaehleSparkonten()) + ")");

    debugMessage("Anz Sparkonten: " + QString::number(konten->zaehleSparkonten()));

    ui->lblGesamtkontenstand->setText(QString::number(konten->getGesamtKontenstand(), 'f', 2) + " €"); // TODO: use / change toString method
    ui->lblGesamtGirokontenstand->setText(QString::number(konten->getGesamtGirokontenstand(), 'f', 2) + " €");
    ui->lblGesamtSparkontenstand->setText(QString::number(konten->getGesamtSparkontenstand(), 'f', 2) + " €");
}


void FrmMain::on_btnBankkonten_clicked()
{
    ui->widgetHome->setVisible(true);
    fadeInGuiElement(ui->tabWidgetOperationen, 300);
    ui->btnUserVerwalten->setEnabled(true);
    ui->btnBankkonten->setEnabled(false);
//    ui->widgetUserVerwalten->setVisible(false);
    fadeOutGuiElement(activeWidget, 100);
}


void FrmMain::on_btnNeuerUser_clicked()
{
    fadeOutGuiElement(ui->widgetLogin,500);
    activeWidget = ui->widgetNeuerUser;

//    ui->widgetNeuerUser->setVisible(true);
    fadeInGuiElement(activeWidget, 500);
    ui->leUserName->clear();
    ui->lePass->clear();
}


void FrmMain::on_btnAbbUserAnlegen_clicked()
{
    activeWidget = ui->widgetLogin;
    fadeOutGuiElement(ui->widgetNeuerUser, 500);
    fadeInGuiElement(ui->widgetLogin,500);
    ui->leUserNameAnlegen->clear();
    ui->leVornameAnlegen->clear();
    ui->leNachameAnlegen->clear();
    ui->lePassAnlegen->clear();
    ui->lePassAnlegen2->clear();
}

void FrmMain::on_btnUserAnlegen_clicked()
{
    QString username = ui->leUserNameAnlegen->text(); //TODO: Add validation, length, characters, etc
    QString vorname = ui->leVornameAnlegen->text(); //TODO: Add validation, length, characters, etc
    QString nachname = ui->leNachameAnlegen->text(); //TODO: Add validation, length, characters, etc
    QString pass = ui->lePassAnlegen->text(); //TODO: Add validation, length, characters, etc
    QString pass2 = ui->lePassAnlegen2->text(); //TODO: Add validation, length, characters, etc

    //TODO: check if username already exists
    if(ui->leUserNameAnlegen->text().isEmpty() ||
            ui->lePassAnlegen->text().isEmpty() ||
            ui->lePassAnlegen2->text().isEmpty() ||
            ui->leVornameAnlegen->text().isEmpty() ||
            ui->leNachameAnlegen->text().isEmpty()) {
        QMessageBox::warning(this, "User Anlegen - Fehler", "Bitte alle Felder ausfühlen");
    }
    else if(pass == pass2) {
        QMessageBox::information(this, "User anlegen - Erfolg", username + " wurde erfolgreich als neuer User angelegt");

        users->userAnlegen(new User(username, vorname, nachname, pass));

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

        userNameUndWillkommenAnimieren();

        fadeInGuiElement(ui->lblEmojiUser, 1500);
        fadeInGuiElement(ui->tabWidgetOperationen, 1500);
    } else QMessageBox::warning(this, "Login - Fehler", "Mind. 1 User anlegen");
}


void FrmMain::on_btnPassAendern_clicked()
{
    ui->widgetPassAendern->setVisible(true);
}


void FrmMain::on_btnAbbPassAendern_clicked()
{
    ui->widgetPassAendern->setVisible(false);
    ui->leNeuesPass->clear();
    ui->leAltesPass->clear();
    ui->leNeuesPass2->clear();
}


void FrmMain::on_btnOkPassAendern_clicked()
{

//    QString altesPass =  ui->leAltesPass->text();
//    QString neuesPass = ui->leNeuesPass->text();
//    QString neuesPass2 = ui->leNeuesPass2->text();

//    if(altesPass.isEmpty() || neuesPass.isEmpty() || neuesPass2.isEmpty()) {
//        QMessageBox::warning(this, "Neues Pass - Fehler", "Bitte alle Felder ausfüllen!!");
//    }
//    else {
//        altesPass =  users->hashPass(altesPass);
//        ui->lwDebug->addItem("ALTES PASS FROM LINE: " + altesPass);
//        ui->lwDebug->addItem("ALTES PASS FROM LIST:  " + loggedUser->getPass());

//        neuesPass = users->hashPass(neuesPass);
//        ui->lwDebug->addItem("Neues PASS1:  " + neuesPass);
//        neuesPass2 = users->hashPass(neuesPass2);
//        ui->lwDebug->addItem("Neues PASS2:  " + neuesPass2);

////        if(altesPass == loggedUser->getPass() /*&& altesPass != neuesPass && neuesPass == neuesPass2*/) {
//            ui->lwDebug->addItem("Altes Pass des eingeloggten Users: " + loggedUser->getPass());
//            loggedUser->setPass(neuesPass);
//            users->passAendern(neuesPass, loggedUser->getUsername());

//            ui->lwDebug->addItem("UserNAme: " + loggedUser->getUsername());
//            ui->lwDebug->addItem("Neues Pass des eingeloggten Users: " + loggedUser->getPass());

//            ui->widgetPassAendern->setVisible(false);
//            ui->leNeuesPass->clear();
//            ui->leAltesPass->clear();
//            ui->leNeuesPass2->clear();
////        }

//        if(altesPass != loggedUser->getPass()) {
//           QMessageBox::warning(this, "Neues Pass - Fehler", "Bitte das richtige alte pass eingeben!!");
//           ui->leAltesPass->clear();
//        }
//        else if(altesPass == neuesPass) {
//            QMessageBox::warning(this, "Neues Pass - Fehler", "Neues Pass kann nicht das selbe als das alte sein!!");
//            ui->leNeuesPass->clear();
//            ui->leNeuesPass2->clear();
//        }
//        else if (neuesPass != neuesPass2) {
//            QMessageBox::warning(this, "Neues Pass - Fehler", "Bitte neues Pass richtig wiederholen!");
//            ui->leNeuesPass2->clear();
//        }
//    }
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


void FrmMain::on_btnCsvExport_clicked() // TODO: MOVE IT TO THE MODEL, SHOULD NOT BE IN THE VIEW!!
{
    QMessageBox::StandardButton reply;
    QMessageBox msgBox;
    reply = msgBox.question(this, "CSV exportieren", "Wollen Sie wirklich die Konteninformationen in eine CSV Datei exportieren?", QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {

        QString folderPath = QFileDialog::getExistingDirectory(nullptr, "Select Folder to Save CSV", QDir::homePath());

        if (!folderPath.isEmpty()) {
            QString filePath = folderPath + "/konten_" + loggedUser->getNachname() + "_" + loggedUser->getVorname() + ".csv";

            QFile file(filePath);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                // File opened successfully
                QTextStream stream(&file);

                stream << "KontoNr, Kontostand, KontoTyp, Dispo, LetzteAuszahlung" << endl;;

                int anz = konten->zaehleKonten();
                for (int i = 0; i < anz; i++) {
                    if(konten->kontoHolen(i)->getInhaberUsername() == loggedUser->getUsername()) {
                        if((dynamic_cast<Girokonto*>(konten->kontoHolen(i)) != nullptr)) {
                           stream << konten->kontoHolen(i)->getKontoNr() << ", " << konten->kontoHolen(i)->getKontostand() << ", Girokonto, " << dynamic_cast<Girokonto*>(konten->kontoHolen(i))->getDispo() << ", -" << endl; // Cast into Girokonto
                        }
                        else if((dynamic_cast<Sparkonto*>(konten->kontoHolen(i)) != nullptr)){
                            stream << konten->kontoHolen(i)->getKontoNr() << ", " << konten->kontoHolen(i)->getKontostand() << ", Sparkonto, -" << ", " << dynamic_cast<Sparkonto*>(konten->kontoHolen(i))->getLetzteAuszahlung().toString("dd-MM-yyyy")  << endl; // Cast into Sparkonto
                        }
                    }
                }

                file.close();
                QMessageBox::information(this, "CSV exportieren", "Die Konteninformationen wurden erfolgreich in " + folderPath + "/konten.csv exportiert!");
            } else {
                // Failed to open the file
                // Handle the error
                QString errorMessage = "Error: " + file.errorString();
                QMessageBox::critical(this, "CSV exportieren - Fehler", errorMessage);
//                ui->lwDebug->addItem(errorMessage);
            }
        }
    }

}

void FrmMain::on_sbDispoAnlegen_valueChanged(double arg1)
{
    toggleBtnSichtbarkeitWennGroesserAlsNull(arg1, ui->btnKontoAnlegen);
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
    if (username.isEmpty() || pass.isEmpty()) { // TODO: merge with function "toggleBtnSichtbarkeit.... " now it only works with ints, but make it also work with Strings
        button->setEnabled(false);
    } else button->setEnabled(true);
}

void FrmMain::userNameUndWillkommenAnimieren()
{
    ui->lblUserTitel->setText("Willkommen " + loggedUser->getVorname());
    ui->lblEmojiUser->setVisible(true);
    ui->tableGirokonten->scrollToTop();
    ui->tableSparkonten->scrollToTop();

    // Animation: zuerst der Text "Wilkommen Vorname" fades in, danach fades out, danach der Text "Vorname Nachname" fades in
    fadeInGuiElement(ui->lblUserTitel, 800);
    QTimer::singleShot(1500, [=]() {
        fadeOutGuiElement(ui->lblUserTitel, 900);
        QTimer::singleShot(1000, [=]() {
            ui->lblUserTitel->setText(loggedUser->getVorname() + " " + loggedUser->getNachname());
            fadeInGuiElement(ui->lblUserTitel, 800);
        });
    });
}

bool FrmMain::meldungenBeimAuszahlen(double betrag)
{
    if (markierteKontoNr % 100 == 99){ // If Sparkonto
        if(betrag > markierteSparkonto->getKontostand()) {
            QMessageBox::warning(this, "Auszahlung - Fehler", "Auszahlungsbetrag kann bei Sparkonten nicht größer als Kontostand sein");
        }
        else if (betrag == markierteSparkonto->getKontostand()) {
            QMessageBox::StandardButton reply = QMessageBox::question(this, "Auszahlzung", "Ihr Kontostand wird nach der Auszahlung 0 sein, wollen Sie trotzdem die Auszahlung durchführen?");
            if (reply == QMessageBox::Yes) {
                return true;
            } else {
                return false;
            }
        }
    } else if (markierteKontoNr % 100 == 1) { // If Girokonto
        if(betrag > markierteGirokonto->getKontostand() + markierteGirokonto->getDispo()) {
            QMessageBox::warning(this, "Auszahlung - Fehler", "Auszahlungsbetrag kann bei Girokonto nicht größer als Kontostand + Dispo sein");
        }
        else if (betrag == markierteGirokonto->getKontostand()) {
            QMessageBox::StandardButton reply = QMessageBox::question(this, "Auszahlzung", "Ihr Kontostand wird nach der Auszahlung 0 sein, wollen Sie trotzdem die Auszahlung durchführen?");
            if (reply == QMessageBox::Yes) {
                return true;
            } else return false;
        }
        else if (betrag == markierteGirokonto->getKontostand() + markierteGirokonto->getDispo()) {
            QMessageBox::StandardButton reply = QMessageBox::question(this, "Auszahlzung", "Ihr Kontostand wird nach der Auszahlung das Dispo Limit erreichen, wollen Sie trotzdem die Auszahlung durchführen?");
            if (reply == QMessageBox::Yes) {
                return true;
            } else return false;
        }
        else if (betrag > markierteGirokonto->getKontostand()) {
            QMessageBox::StandardButton reply = QMessageBox::question(this, "Auszahlzung", "Ihr Kontostand wird nach der Auszahlung negativ sein, wollen Sie trotzdem die Auszahlung durchführen?");
            if (reply == QMessageBox::Yes) {
                return true;
            } else return false;
        }
    }
    return false; // TODO: check all control-paths
}

void FrmMain::debugMessage(QString debugString)
{
    ui->lwDebug->addItem(debugString);
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
    //TODO Make a better function
    if (arg1.isEmpty()) { // TODO: merge with function "toggleBtnSichtbarkeit.... " now it only works with ints, but make it also work with Strings
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
