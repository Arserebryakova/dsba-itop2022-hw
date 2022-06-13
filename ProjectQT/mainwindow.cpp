#include "mainwindow.h"
#include "show.h"
#include "about.h"
#include "noshow.h"
#include "add.h"
#include "incorrect.h"
#include <QGridLayout>
#include <QMenuBar>
#include <QFileDialog>
#include <fstream>
#include <sstream>
#include <unordered_set>
#include <QDebug>
#include <string>
#include <ctype.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *central_widget = new QWidget (this);

    bank_table = new BankTable();
    setCentralWidget(central_widget);
    table_view = new QTableView(central_widget);
    sorting = new QComboBox();
    filter_sheet = new Filter(this, this);

    QGridLayout *layout = new QGridLayout(central_widget);
    QHBoxLayout *put = new QHBoxLayout();
    QHBoxLayout *put_button = new QHBoxLayout();

    enter_text = new QLineEdit();
    search_button = new QPushButton();
    filter_sheet->setSourceModel(bank_table);
    table_view->setModel(filter_sheet);
    add_button = new QPushButton();
    delete_button = new QPushButton();
    edit_button = new QPushButton();

    sorting->setStyleSheet(QString("font-size: %1px").arg(20));
    search_button->setStyleSheet(QString("font-size: %1px").arg(20));
    enter_text->setStyleSheet(QString("font-size: %1px").arg(20));
    search_button->setText("Search");
    add_button->setStyleSheet(QString("font-size: %1px").arg(20));
    add_button->setText("Add client");
    delete_button->setStyleSheet(QString("font-size: %1px").arg(20));
    delete_button->setText("Delete");
    edit_button->setStyleSheet(QString("font-size: %1px").arg(20));
    edit_button->setText("Edit");

    sorting->addItem("Default");
    sorting->addItem("Gender");
    sorting->addItem("Own car");
    sorting->addItem("Own realty");
    sorting->addItem("Amount of children");
    sorting->addItem("Income ascending");
    sorting->addItem("Income descending");

    layout->addLayout(put, 1, 0);
    layout->addLayout(put_button, 2, 0);
    layout->addWidget(table_view);
    layout->addWidget(add_button);
    layout->addWidget(delete_button);
    layout->addWidget(edit_button);
    put->addWidget(enter_text);
    put->addWidget(sorting);
    put->addStretch();
    put_button->addWidget(search_button);
    put_button->addStretch();

    connect(search_button, SIGNAL(clicked()), this, SLOT(search()));
    connect(sorting, SIGNAL(currentIndexChanged(QString)), SLOT(sort_func()));
    connect(add_button, SIGNAL(clicked()), this, SLOT(add_data_func()));
    connect(delete_button, SIGNAL(clicked()), this, SLOT(delete_func()));
    connect(edit_button, SIGNAL(clicked()), this, SLOT(edit_func()));

    auto file_menu = menuBar ()->addMenu ("File");

    QAction *open_action = new QAction ("Open");
    connect (open_action, &QAction::triggered, this, &MainWindow::load_data_func);
    file_menu->addAction  (open_action);
    QAction *saveas_action = new QAction ("Save As");
    connect (saveas_action, &QAction::triggered, this, &MainWindow::save_data_func);
    file_menu->addAction  (saveas_action);

    QAction *about = new QAction ("Logo");
    connect (about, SIGNAL(triggered()), this, SLOT(about_logo()));
    menuBar ()->addAction  (about);

}

void MainWindow::about_logo()
{
    about a;
    a.exec();
}
void MainWindow::recieve_data(Bank &var, size_t &row)
{
    if(row == size_t(-1)) {
        row++;
        bank_table->add_data(var, row);
    }
    else
    {
        std::unordered_set<size_t> rows_to_delete = {row};
        bank_table->delete_rows(rows_to_delete);
        bank_table->add_data(var, row);
    }
}
void MainWindow::edit_func()
{
    Bank res;
    QItemSelectionModel *selectModel = table_view->selectionModel();
    QModelIndexList indexes = selectModel->selectedIndexes();
    size_t row;
    for(auto &j : indexes)
    {
        row = j.row();
        for(int i = 0 ; i < static_cast<int> (client_fields::COUNT); i++) {
            QModelIndex index = table_view->model()->index(row,i,QModelIndex());
            std::string s = index.data().toString().toStdString();
            res.data[i] = QString(s.c_str());
        }
        break;
    }
    if(res.data[0].isNull())
        return;
    add a;
    connect(this, SIGNAL(send_data(Bank&, size_t&)), &a, SLOT(recieve_data(Bank&, size_t&)));
    connect(&a, SIGNAL(send_data(Bank&, size_t&)), this, SLOT(recieve_data(Bank&, size_t&)));
    emit send_data(res, row);
    a.exec();
}
void MainWindow::add_data_func()
{
    add a;
    connect(&a, SIGNAL(send_data(Bank&, size_t&)), this, SLOT(recieve_data(Bank&, size_t&)));
    a.exec();
}
void MainWindow::delete_func()
{
    std::unordered_set<size_t> rows_to_delete;
    QItemSelectionModel *selectModel = table_view->selectionModel();
    QModelIndexList indexes = selectModel->selectedIndexes();
    for(auto &i : indexes)
        rows_to_delete.insert(i.row());
    if (rows_to_delete.size())
        bank_table->delete_rows(rows_to_delete);
}
void MainWindow::sort_func()
{
    QString text = sorting->currentText();
    if (text == "Default")
        filter_sheet->sort(-1, Qt::AscendingOrder);
    else if (text == "Gender")
        filter_sheet->sort(1, Qt::AscendingOrder);
    else if (text == "Own car")
        filter_sheet->sort(2, Qt::DescendingOrder);
    else if (text == "Own realty")
        filter_sheet->sort(3, Qt::DescendingOrder);
    else if(text == "Amount of children")
        filter_sheet->sort(4, Qt::AscendingOrder);
    else if (text == "Income ascending")
        filter_sheet->sort(5, Qt::AscendingOrder);
    else if (text == "Income descending")
        filter_sheet->sort(5, Qt::DescendingOrder);
    table_view->setModel(filter_sheet);
}
void MainWindow::load_data_func ()
{
    auto way = QFileDialog::getOpenFileName (this, "Open File", QDir::currentPath ());
    if (!way.size ())
        return;
    auto data = load_data(way);
    bank_table->set_data (data);
}
void MainWindow::save_data_func ()
{
  auto way = QFileDialog::getSaveFileName (this, "Save File As", QDir::currentPath ());
  if (!way.size ())
    return;
  save_data (way);
}

std::vector<Bank> MainWindow::load_data (const QString &way)
{
    std::fstream file;
    std::vector<Bank> result;
    file.open(way.toStdString ());
    std::string temp1, temp2;
    int j = 0;
    while(std::getline(file, temp1))
    {
        Bank res;
        int i = 0, k = 0;
        std::stringstream str(temp1);
        std::string temp3 = "";
        bool flag = false;
        while(std::getline(str, temp2, ','))
        {
            if(temp2[0]=='"' || flag == true)
            {
                temp3 += temp2;
                flag = true;
                if(temp2[temp2.size()-1] == '"')
                {
                    flag = false;
                    temp3.erase(temp3.begin());
                    temp3.erase(temp3.end()-1);
                    if((i >= 0 && i <= 5) || i == 7)
                        res.data[k++] = QString(temp3.c_str());
                    temp3="";
                    ++i;
                }
            }
            else
            {
                if((i >= 0 && i <= 5) || i == 7)
                    res.data[k++] = QString(temp2.c_str());
                ++i;
            }
        }
        if(j)
        {
            res.data[5]=std::strtod(res.data[5].toString().toStdString().c_str(), NULL);
            res.data[4]=std::strtod(res.data[4].toString().toStdString().c_str(), NULL);
            result.push_back(res);
        }
        j++;
    }
    file.close();
    return result;
}
void MainWindow::save_data (const QString &way)
{
    std::ofstream file (way.toStdString ());
    for (auto &item : bank_table->bank_data)
    {
        for (int i_column = 0; i_column < 7; ++i_column)
        {
            file << item.data[i_column].toString().toStdString();
            if (i_column < 6)
                file << ",";
            else
                file << "\n";
        }
    }
}
void MainWindow::search()
{
    QString s = enter_text->text();
    if(s.isNull())
        return;
    bool flag = false;
    for(Bank &item : bank_table->bank_data)
        if(item.data[0] == s)
        {
            class show v;
            connect(this, SIGNAL(send_data_s(Bank&)), &v, SLOT(recieveData(Bank&)));
            emit send_data_s(item);
            v.exec();
            flag = true;
            break;
        }
    if(!flag){
        class noshow v;
        v.exec();
    }
}

MainWindow::~MainWindow()
{
}

Filter::Filter (QObject *parent, const MainWindow *main_window) : QSortFilterProxyModel (parent), main_window (main_window)
{
}

QVariant BankTable::data (const QModelIndex &index, int role) const
{
   if (role == Qt::DisplayRole || role == Qt::EditRole)
   {
       const Bank &current_client = bank_data[index.row ()];
       return current_client.data[index.column ()];
   }
   return {};
}

const char *clients_data (client_fields field)
{
    switch (field)
    {
        case client_fields::ID                 : return "ID";
        case client_fields::CODE_GENDER        : return "CODE_GENDER";
        case client_fields::FLAG_OWN_CAR       : return "FLAG_OWN_CAR";
        case client_fields::FLAG_OWN_REALTY    : return "FLAG_OWN_REALTY";
        case client_fields::CNT_CHILDREN       : return "CNT_CHILDREN";
        case client_fields::AMT_INCOME_TOTAL   : return "AMT_INCOME_TOTAL";
        case client_fields::NAME_EDUCATION_TYPE: return "NAME_EDUCATION_TYPE";
        case client_fields::COUNT              : return "";
    }
    return {};
}

QVariant BankTable::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        client_fields field = (client_fields)section;
        return clients_data (field);
    }
    return {};
}
