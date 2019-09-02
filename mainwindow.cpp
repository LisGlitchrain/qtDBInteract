#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "opendbdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    getDataBtn = ui->getDataBtn;
    getDataBtn->setEnabled(false);
    deleteRowBtn = ui->deleteRowBtn;
    deleteRowBtn->setEnabled(false);
    addRowBtn = ui->addRowBtn;
    addRowBtn->setEnabled(false);
    commitBtn = ui->commitBtn;
    commitBtn->setEnabled(false);
    connect(getDataBtn, SIGNAL(released()), this, SLOT( handleTextChangeBtn()));
    db = QSqlDatabase::addDatabase("QPSQL");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow:: handleTextChangeBtn()
{
    qDebug() << "step7";
    if(!db.isOpen()) return;
    qDebug() << "step8";
    query = new QSqlQuery(db);
    qDebug() << "Resuls received:";
    qDebug() << query->exec("SELECT * FROM public.\"Cars\"");
    if (query->next())
    {
        int columns =  query->record().count();
        int rows = query->size();
        ui->tableWidget->setColumnCount(columns);
        ui->tableWidget->setRowCount(rows);
        ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        QSqlQuery* headers = new QSqlQuery (db);
        QStringList headersNames;
        qDebug() << "Trying to get columns";
        qDebug() << headers->exec("SELECT COLUMN_NAME FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME = 'Cars' ORDER BY ORDINAL_POSITION");
        if (headers->next())
        {
            for(int i = 0; i < columns;i++)
            {
                headersNames.append(headers->value(0).toString());
                headers->next();
            }
        }

        ui->tableWidget->setHorizontalHeaderLabels(headersNames);
        for (int j = 0; j <= rows;j++)
        {
            for(int i = 0; i < columns;i++)
            {
                QTableWidgetItem* item = new QTableWidgetItem();
                item->setText(query->record().value(i).toString());
                ui->tableWidget->setItem(j,i,item);
            }
            query->next();
        }
    }

}



void MainWindow::on_actionOpenDB_triggered()
{
    qDebug() << "step1";
    openDBDialog openDBDiag(this);
    openDBDiag.setModal(true);
    openDBDiag.exec();
    qDebug() << "step2";
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::setBtnEnable(bool state)
{
    getDataBtn->setEnabled(state);
    deleteRowBtn->setEnabled(state);
    commitBtn->setEnabled(state);
}


void MainWindow::on_actionCloseDB_triggered()
{
    db.close();
    setBtnEnable(false);
}

void MainWindow::on_tableWidget_cellChanged(int row, int column)
{
    //query->clear();
}

void MainWindow::on_deleteRowBtn_released()
{
    qDebug() << rowSelectedInApp;
    qDebug() << tableWidget.item(tableWidget.currentIndex().row(),0)->data(0).toInt();
    if(rowSelectedInApp >= 0) return;
    query->clear();
    deleteString.append("DELETE FROM \"");
    deleteString.append(db.databaseName());
    deleteString.append("\"");
    deleteString.append(" WHERE id = ");
    deleteString.append("");
    deleteString.append(";");
    qDebug() << deleteString;
    query->exec(deleteString);
    handleTextChangeBtn();
    deleteString.clear();
}

void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
   rowSelectedInApp = row;
   columnSelectedInApp = column;
}
