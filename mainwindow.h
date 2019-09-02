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

private:
    QPushButton* getDataBtn;
    QTableWidget tableWidget;
    QSqlQuery* query;
public:
    QSqlDatabase db;
public slots:
    void setGetDataBtnEnable(bool state);
};

#endif // MAINWINDOW_H
