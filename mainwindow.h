#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QtSql>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QtDebug>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QAction>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
private slots:
    void handleTextChangeBtn();
    void on_actionOpenDB_triggered();
    void on_actionExit_triggered();

    void on_actionCloseDB_triggered();

    void on_tableWidget_cellChanged(int row, int column);

    void on_deleteRowBtn_released();

    void on_tableWidget_cellClicked(int row, int column);

private:
    QPushButton* getDataBtn;
    QTableWidget tableWidget;
    QSqlQuery* query;
    QString updateString;
    QString insertString;
    QString deleteString;
    QPushButton* deleteRowBtn;
    QPushButton* addRowBtn;
    QPushButton* commitBtn;
    int rowSelectedInApp;
    int columnSelectedInApp;
public:
    QSqlDatabase db;
public slots:
    void setBtnEnable(bool state);
};

#endif // MAINWINDOW_H
