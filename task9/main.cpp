#include <QAbstractTableModel>
#include <QApplication>
#include <QTableView>
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <vector>


struct Contact {
    QString first_name;
    QString last_name;
    QString phone_number;
};


class ContactModel : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit ContactModel(QObject *parent = nullptr)
        : QAbstractTableModel(parent) {}


    int rowCount(const QModelIndex &parent = QModelIndex()) const override {
        return contacts.size();
    }

    int columnCount(const QModelIndex &parent = QModelIndex()) const override {
        return 3;
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
        if (!index.isValid() || role != Qt::DisplayRole)
            return QVariant();

        const Contact &contact = contacts[index.row()];

        switch (index.column()) {
        case 0: return contact.first_name;
        case 1: return contact.last_name;
        case 2: return contact.phone_number;
        default: return QVariant();
        }
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override {
        if (role != Qt::DisplayRole)
            return QVariant();

        if (orientation == Qt::Horizontal) {
            switch (section) {
            case 0: return tr("First Name");
            case 1: return tr("Last Name");
            case 2: return tr("Phone Number");
            default: return QVariant();
            }
        }
        return QVariant();
    }

    void addContact(const Contact &contact) {
        beginInsertRows(QModelIndex(), contacts.size(), contacts.size());
        contacts.push_back(contact);
        endInsertRows();
    }

private:
    std::vector<Contact> contacts;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    ContactModel model;

    QTableView tableView;
    tableView.setModel(&model);

    QVBoxLayout layout;
    layout.addWidget(&tableView);

    QWidget window;
    window.setLayout(&layout);
    window.setWindowTitle("Contact List");
    window.resize(400, 300);
    window.show();

    model.addContact({"John", "Doe", "123-456-7890"});
    model.addContact({"Jane", "Doe", "987-654-3210"});

    return app.exec();
}

#include "main.moc"
