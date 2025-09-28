#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QTableWidgetItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Configuration du tableau
    QStringList headers = {"ID", "Nom", "Prénom", "Email", "Téléphone", "Âge", "Points Fidélité", "Adresse"};
    ui->tableWidget->setColumnCount(headers.size());
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    // Connexions directes sans slots
    connect(ui->btnAjouter, &QPushButton::clicked, this, &MainWindow::ajouterClient);
    connect(ui->btnModifier, &QPushButton::clicked, this, &MainWindow::modifierClient);
    connect(ui->btnReinitialiser, &QPushButton::clicked, this, &MainWindow::reinitialiserFormulaire);
    connect(ui->btnRechercher, &QPushButton::clicked, this, &MainWindow::rechercherClient);
    connect(ui->lineEditTrier, &QLineEdit::textChanged, this, &MainWindow::trierClients);

    connect(ui->btnExporterPDF, &QPushButton::clicked, this, &MainWindow::exporterPDF);
    connect(ui->btnStatistiques, &QPushButton::clicked, this, &MainWindow::afficherStatistiques);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ajouterClient()
{
    // Récupération des données du formulaire
    QString id = ui->lineEditID->text();
    QString nom = ui->lineEditNom->text();
    QString prenom = ui->lineEditPrenom->text();
    QString email = ui->lineEditEmail->text();
    QString telephone = ui->lineEditTelephone->text();
    QString age = ui->lineEditAge->text();
    QString points = ui->lineEditPoints->text();
    QString adresse = ui->lineEditAdresse->text();

    // Validation
    if(nom.isEmpty() || prenom.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Nom et Prénom sont obligatoires");
        return;
    }

    // Création d'une ligne de données
    QStringList client;
    client << id << nom << prenom << email << telephone << age << points << adresse;

    // Ajout à la liste
    listeClients.append(client);

    // Mise à jour de l'affichage
    afficherClients();

    QMessageBox::information(this, "Succès", "Client ajouté !");
    reinitialiserFormulaire();
}
void MainWindow::trierClients()
{
    QString critere = ui->lineEditTrier->text();  // ← text() au lieu de currentText()

    int colonne = 0; // Par défaut tri par ID

    if (critere == "Nom") colonne = 1;
    else if (critere == "Prénom") colonne = 2;
    else if (critere == "Email") colonne = 3;
    else if (critere == "Âge") colonne = 5;

    // Trier la liste
    std::sort(listeClients.begin(), listeClients.end(),
              [colonne](const QStringList &a, const QStringList &b) {
                  return a[colonne] < b[colonne];
              });

    // Réafficher
    afficherClients();
}
void MainWindow::afficherClients()
{
    // Vider le tableau
    ui->tableWidget->setRowCount(0);

    // Remplir avec les clients de la liste
    for(int i = 0; i < listeClients.size(); i++) {
        QStringList client = listeClients[i];

        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);

        for(int col = 0; col < client.size(); col++) {
            ui->tableWidget->setItem(row, col, new QTableWidgetItem(client[col]));
        }
    }
}

void MainWindow::modifierClient()
{
    int row = ui->tableWidget->currentRow();

    if(row < 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un client à modifier");
        return;
    }

    // Récupérer les données du client sélectionné
    QStringList client = listeClients[row];

    // Remplir le formulaire avec ces données
    ui->lineEditID->setText(client[0]);
    ui->lineEditNom->setText(client[1]);
    ui->lineEditPrenom->setText(client[2]);
    ui->lineEditEmail->setText(client[3]);
    ui->lineEditTelephone->setText(client[4]);
    ui->lineEditAge->setText(client[5]);
    ui->lineEditPoints->setText(client[6]);
    ui->lineEditAdresse->setText(client[7]);

    QMessageBox::information(this, "Modification", "Client chargé pour modification");
}

void MainWindow::reinitialiserFormulaire()
{
    // Vider tous les LineEdit
    ui->lineEditID->clear();
    ui->lineEditNom->clear();
    ui->lineEditPrenom->clear();
    ui->lineEditEmail->clear();
    ui->lineEditTelephone->clear();
    ui->lineEditAge->clear();
    ui->lineEditPoints->clear();
    ui->lineEditAdresse->clear();
}

void MainWindow::rechercherClient()
{
    QString recherche = ui->lineEditRecherche->text();

    if (recherche.isEmpty()) {
        afficherClients(); // Afficher tous si recherche vide
        return;
    }

    // Filtrer la liste
    QList<QStringList> clientsFiltres;
    for (const QStringList &client : listeClients) {
        // Vérifier si la recherche correspond à nom, prénom, email, etc.
        if (client[1].toLower().contains(recherche) || // Nom
            client[2].toLower().contains(recherche) || // Prénom
            client[3].toLower().contains(recherche) || // Email
            client[4].toLower().contains(recherche)) { // Téléphone
            clientsFiltres.append(client);
        }
    }

    // Afficher les résultats filtrés
    ui->tableWidget->setRowCount(0);
    for (int i = 0; i < clientsFiltres.size(); i++) {
        QStringList client = clientsFiltres[i];
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);

        for (int col = 0; col < client.size(); col++) {
            ui->tableWidget->setItem(row, col, new QTableWidgetItem(client[col]));
        }
    }

    QMessageBox::information(this, "Recherche",
                             QString("%1 clients trouvés").arg(clientsFiltres.size()));
}



void MainWindow::exporterPDF()
{
    QMessageBox::information(this, "Export", "PDF exporté avec succès!");
}

void MainWindow::afficherStatistiques()
{
    int totalClients = listeClients.size();
    QMessageBox::information(this, "Statistiques",
                             "Nombre total de clients: " + QString::number(totalClients));
}
