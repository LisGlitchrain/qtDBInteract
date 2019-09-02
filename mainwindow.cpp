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
    connect(getDataBtn, SIGNAL(released()), this, SLOT( handleTextChangeBtn()));
    db = QSqlDatabase::addDatabase("QPSQL");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow:: handleTextChangeBtn()
{
    if(!db.isOpen()) return;

    query = new QSqlQuery(db);
    qDebug() << "Resuls received:";
    qDebug() << query->exec("SELECT * FROM public.\"Cars\"");
    if (query->next())
    {

        int columns =  query->record().count();
        int rows = query->size();
        ui->tableWidget->setColumnCount(columns);
        ui->tableWidget->setRowCount(rows);
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
    openDBDialog openDBDiag(this);
    openDBDiag.setModal(true);
    openDBDiag.exec();
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::setGetDataBtnEnable(bool state)
{
    getDataBtn->setEnabled(state);
}


void MainWindow::on_actionCloseDB_triggered()
{
    db.close();
    getDataBtn->setEnabled(false);
}
