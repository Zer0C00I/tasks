
#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMetaType>
#include <QWidget>
#include <QJsonDocument>
#include <qudpsocket.h>
#include <QThread>
#include "src/headers/qhostaddress_metatype.h"
#include <QDebug>


class ScanningThread : public QThread {
    Q_OBJECT
public:
    ScanningThread(QObject* parent = nullptr) : QThread(parent), stopRequested(false) {}

signals:
    void deviceFound(const QVariantMap&, const QHostAddress& senderAddress); 

protected:
    void run() override {
        QUdpSocket udpSocket;
        udpSocket.bind(QHostAddress::AnyIPv4, 23500);

        qDebug() << "Listening for devices...";

        while (!stopRequested) {
            QByteArray datagram;
            QHostAddress senderAddress;
            datagram.resize(udpSocket.pendingDatagramSize());

            if (udpSocket.hasPendingDatagrams()) {
                udpSocket.readDatagram(datagram.data(), datagram.size(), &senderAddress);
                qDebug() << "Received data:" << datagram << "from address:" << senderAddress;

                QJsonDocument jsonDoc = QJsonDocument::fromJson(datagram);
                if (!jsonDoc.isNull() && jsonDoc.isObject()) {
                    QVariantMap deviceInfo = jsonDoc.toVariant().toMap();
                    emit deviceFound(deviceInfo, senderAddress);
                }
            }

            msleep(100);
        }
    }

public slots:
    void stop() {
        stopRequested = true;
    }

private:
    bool stopRequested;
};

class DeviceDiscoveryApp : public QMainWindow {
    Q_OBJECT
public:
    DeviceDiscoveryApp(QWidget* parent = nullptr) : QMainWindow(parent), uniqueDevices() {
        scanningThread = new ScanningThread(this);
        connect(scanningThread, &ScanningThread::deviceFound, this, &DeviceDiscoveryApp::addDeviceToList);

        setWindowTitle("Device Discovery");
        resize(400, 300);

        QVBoxLayout* layout = new QVBoxLayout();

        discoveryLabel = new QLabel("Searching for devices...");
        layout->addWidget(discoveryLabel);

        deviceListWidget = new QListWidget();
        layout->addWidget(deviceListWidget);

        chooseButton = new QPushButton("Choose");
        chooseButton->setEnabled(false);
        layout->addWidget(chooseButton);

        discoverButton = new QPushButton("Start Device Discovery");
        layout->addWidget(discoverButton);

        QWidget* centralWidget = new QWidget();
        centralWidget->setLayout(layout);
        setCentralWidget(centralWidget);

        connect(discoverButton, &QPushButton::clicked, this, &DeviceDiscoveryApp::toggleDiscovery);
        connect(deviceListWidget, &QListWidget::itemClicked, this, &DeviceDiscoveryApp::updateChooseButtonState);
        connect(chooseButton, &QPushButton::clicked, this, &DeviceDiscoveryApp::chooseDevice);
    }

private slots:
    void toggleDiscovery() {
        if (scanningThread->isRunning()) {
            stopDiscovery();
        } else {
            startDiscovery();
        }
    }

    void startDiscovery() {
        discoveryLabel->setText("Listening for devices...");
        discoverButton->setText("Stop Device Discovery");
        scanningThread->start();
    }

    void stopDiscovery() {
        discoveryLabel->setText("Discovery Stopped");
        discoverButton->setText("Start Device Discovery");
        scanningThread->stop();
    }

    void addDeviceToList(const QVariantMap& deviceInfo, const QHostAddress& senderAddress) {
        QString serialNumber = deviceInfo.value("sn").toString();
        QString ipAddress = senderAddress.toString();
        QString deviceInfoStr = "SN: " + serialNumber + ", IP: " + ipAddress;
        QListWidgetItem* item = new QListWidgetItem(deviceInfoStr);
        deviceListWidget->addItem(item);

        uniqueDevices.insert(qMakePair(serialNumber, ipAddress));
    }

    void updateChooseButtonState() {
        chooseButton->setEnabled(!deviceListWidget->selectedItems().isEmpty());
    }

    void chooseDevice() {
        QList<QListWidgetItem*> selectedItems = deviceListWidget->selectedItems();
        if (!selectedItems.isEmpty()) {
            QListWidgetItem* selectedItem = selectedItems.first();
            QString selectedText = selectedItem->text();
            int snStart = selectedText.indexOf("SN: ") + 4;
            int snEnd = selectedText.indexOf(",", snStart);
            QString serialNumber = selectedText.mid(snStart, snEnd - snStart);
            int ipStart = selectedText.indexOf("IP: ") + 4;
            QString ipAddress = selectedText.mid(ipStart);
            stopDiscovery();
            emit deviceSelected(serialNumber, ipAddress);
            close();
        }
    }

signals:
    void deviceSelected(const QString& serialNumber, const QString& ipAddress);

private:
    QLabel* discoveryLabel;
    QListWidget* deviceListWidget;
    QPushButton* chooseButton;
    QPushButton* discoverButton;
    ScanningThread* scanningThread;
    QSet<QPair<QString, QString>> uniqueDevices;
};


#include "main.moc"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);


    qRegisterMetaType<QHostAddress>();

    DeviceDiscoveryApp searchWindow;
    searchWindow.show();
    return app.exec();
}
