#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QSortFilterProxyModel>
#include <unordered_set>

class Bank;
class BankTable;
class Filter;

class MainWindow : public QMainWindow
{
public:
    Q_OBJECT

    QTableView *table_view = nullptr;
    BankTable *bank_table = nullptr;
    QLineEdit *enter_text;
    QPushButton *search_button;
    QComboBox *sorting;
    Filter *filter_sheet = nullptr;
    QPushButton *add_button;
    QPushButton *delete_button;
    QPushButton *edit_button;

public:
    MainWindow(QWidget *parent = nullptr);
    std::vector<Bank> load_data (const QString &way);
    void save_data (const QString &way);
    ~MainWindow();

public slots:
    void load_data_func();
    void save_data_func();
    void search();
    void about_logo();
    void sort_func();
    void recieve_data(Bank &var, size_t &row);
    void add_data_func();
    void delete_func();
    void edit_func();

signals:
    void send_data(Bank &var, size_t &row);
    void send_data_s(Bank &var);

friend class Filter;
};

enum class client_fields
{
    ID,
    CODE_GENDER,
    FLAG_OWN_CAR,
    FLAG_OWN_REALTY,
    CNT_CHILDREN,
    AMT_INCOME_TOTAL,
    NAME_EDUCATION_TYPE,
    COUNT
};

const char *clients_data (client_fields field);

class Bank
{
public:
    std::array<QVariant, (static_cast<int> (client_fields::COUNT))> data;
    friend bool operator==(const Bank &a, const Bank &b) {
        return(a.data[0] == b.data[0] && a.data[1] == b.data[1]);
    }
};

class Filter : public QSortFilterProxyModel
{
   Q_OBJECT
public:
  Filter (QObject *parent, const MainWindow *main_window);
protected:

private:
  const MainWindow *main_window = nullptr;
};

class BankTable : public QAbstractTableModel
{
Q_OBJECT

public:
std::vector<Bank> bank_data;
int rowCount(const QModelIndex &/*parent*/) const override
{
    return bank_data.size ();
}

int columnCount(const QModelIndex &/*parent*/) const override
{
    return static_cast<int> (client_fields::COUNT);
}

void set_data (const std::vector<Bank> &data)
{
  beginResetModel ();
  bank_data.insert(bank_data.begin(), data.begin(), data.end());
  endResetModel ();
  emit layoutChanged ();
}

void add_data(const Bank &data, size_t &row)
{
    if(data.data[0].isNull())
        return;
    beginResetModel ();
    bool flag = true;
    std::vector<Bank>::iterator it = bank_data.begin();
    for(;it != bank_data.end(); it++) {
        if(*it == data) {
            flag = false;
            break;
        }
    }
    if(flag)
        bank_data.insert(bank_data.begin() + row, data);
    endResetModel ();
    emit layoutChanged ();
}

void delete_rows (const std::unordered_set<size_t> &selected_rows)
{
    beginResetModel();
    size_t j = 0;
    for(size_t i = 0; i < bank_data.size(); i++)
        if(selected_rows.find(i) == selected_rows.end())
            bank_data[j++] = bank_data[i];
    bank_data.resize(j);
    endResetModel();
    emit layoutChanged();
}

QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const override;
QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
};

#endif // MAINWINDOW_H

