#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "opendbdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    getDataBtn = ui->getDataBtn;
    deleteRowBtn = ui->deleteRowBtn;
    addRowBtn = ui->addRowBtn;
    commitBtn = ui->commitBtn;
    connect(getDataBtn, SIGNAL(released()), this, SLOT( handleTextChangeBtn()));
    connect(addRowBtn, SIGNAL(released()), this, SLOT(handleAddRowBtn()));
    db = QSqlDatabase::addDatabase("QPSQL");
    rowSelectedInApp = -1;
    columnSelectedInApp = -1;
    tableMenu = ui->menuTables;
    setBtnEnable(false);
    statusBar = ui->statusBar;
    statusBar->showMessage("App is loaded.");
    tableWidget = ui->tableWidget;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow:: handleTextChangeBtn()
{
    if(!db.isOpen()) return;
    query = new QSqlQuery(db);
    db.tables();
    QString queryRequestString = "SELECT * FROM public.\"";
    queryRequestString.append(currentTable);
    queryRequestString.append("\"");
    qDebug() << query->exec(queryRequestString);
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
        headers->exec("SELECT COLUMN_NAME FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME = 'Cars' ORDER BY ORDINAL_POSITION");
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
            QList<QTableWidgetItem*> tempList;
            for(int i = 0; i < columns;i++)
            {
                QTableWidgetItem* item = new QTableWidgetItem();
                item->setText(query->record().value(i).toString());                
                ui->tableWidget->setItem(j,i,item);
                tempList.push_back(item);
            }
            itemList.push_back(tempList);
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

void MainWindow::setBtnEnable(bool state)
{
    getDataBtn->setEnabled(state);
    addRowBtn->setEnabled(state);
    deleteRowBtn->setEnabled(state);
    commitBtn->setEnabled(state);
    tableMenu->setEnabled(state);
}


void MainWindow::on_actionCloseDB_triggered()
{
    db.close();
    setBtnEnable(false);
    tableWidget->clear();
}

void MainWindow::on_tableWidget_cellChanged(int row, int column)
{
    //query->clear();
}

void MainWindow::on_deleteRowBtn_released()
{
    int currentRow = tableWidget->currentRow();
    if(currentRow < 0) return;
    query->clear();
    deleteString.append("DELETE FROM \"");
    deleteString.append(currentTable);
    deleteString.append("\"");
    deleteString.append(" WHERE id = ");
    deleteString.append(itemList[currentRow][0]->text());
    deleteString.append(";");
    qDebug() << deleteString;
    query->exec(deleteString);
    handleTextChangeBtn();
    deleteString.clear();
    tableWidget->clear();
    handleTextChangeBtn();
}

void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
   rowSelectedInApp = row;
   columnSelectedInApp = column;
   qDebug() << tableWidget->currentRow() << "from table widget";
}

void MainWindow::enableAndLoadTableMenu()
{
    tableMenu->setEnabled(true);
    tables = db.tables();
    if(tables.count() > 0)
    {
        foreach (QString table, tables)
        {
            tableMenu->addAction(table);
        }
        foreach(QAction* action, tableMenu->actions())
        {
             connect(action, &QAction::triggered, [=]() {
               on_actionTable_triggered(action->text());
             });
        }
    }

}

void MainWindow::on_actionTable_triggered(QString tableName)
{
    currentTable = tableName;
    setBtnEnable(true);
}

void MainWindow::handleAddRowBtn()
{
    statusBar->showMessage("Adding.");
    for(int i = 0; i < tableWidget->columnCount(); i++)
    {
        qDebug() << i;
        QTableWidgetItem* tempItem = new QTableWidgetItem();
        tempItem->setText("");
        tableWidget->setItem(tableWidget->rowCount() + 1,i,tempItem);
    }
}
