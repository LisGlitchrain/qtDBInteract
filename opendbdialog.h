#ifndef OPENDBDIALOG_H
#define OPENDBDIALOG_H

#include <QDialog>
#include <QSql>
#include <QSqlDatabase>
#include "mainwindow.h"
#include <QLineEdit>
#include <QStatusBar>

namespace Ui {
class openDBDialog;
}

class openDBDialog : public QDialog
{
    Q_OBJECT

public:
    explicit openDBDialog(MainWindow *parent);
    ~openDBDialog();

private slots:
    void on_buttonBox_rejected();

    void on_buttonBox_accepted();

private:
    Ui::openDBDialog *ui;
    QSqlDatabase db;
    QLineEdit* userNameQ;
    QLineEdit* passwordQ;
    QLineEdit* hostnameQ;
    QLineEdit* portQ;
    QLineEdit* dbNameQ;
    MainWindow* parentWindow;

};

#endif // OPENDBDIALOG_H
