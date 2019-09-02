#include "opendbdialog.h"
#include "ui_opendbdialog.h"
#include "mainwindow.h"

openDBDialog::openDBDialog(MainWindow *parent) :
    QDialog(parent),
    ui(new Ui::openDBDialog)
{
    ui->setupUi(this);
    userNameQ = ui->userNameEdit;
    passwordQ = ui->passwordEdit;
    hostnameQ = ui->hostnameEdit;
    portQ = ui->portEdit;
    dbNameQ = ui->dbNameEdit;
    portQ->setValidator(new QIntValidator(0,65000));
    db = parent->db;
    parentWindow = parent;
}

openDBDialog::~openDBDialog()
{
    delete ui;
}

void openDBDialog::on_buttonBox_rejected()
{
//   this->close();
}

void openDBDialog::on_buttonBox_accepted()
{
    db.setUserName(userNameQ->text());
    db.setPassword(passwordQ->text());
    db.setHostName(hostnameQ->text());
    db.setPort(portQ->text().toInt());
    db.setDatabaseName(dbNameQ->text());
    if (db.open())
    {
        parentWindow->setGetDataBtnEnable(true);
        qDebug() << "Db is opened successfully.";
    }
    else {
        db.setUserName("");
        db.setPassword("");
        db.setHostName("");
        db.setPort(5432);
        db.setDatabaseName("");
        qDebug() << "DB opening is faled.";
    }

}
