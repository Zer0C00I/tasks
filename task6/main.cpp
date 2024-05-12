#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QPushButton>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QMainWindow mainWindow;
    mainWindow.setGeometry(100, 100, 300, 200);

    QLabel label(&mainWindow);
    label.setGeometry(50, 50, 200, 30);

    QPushButton button("Click me", &mainWindow);
    button.setGeometry(100, 100, 100, 30);

    int clickCount = 0;

    
    QObject::connect(&button, &QPushButton::clicked, [&]() {
        clickCount++;
        label.setText("Clicked " + QString::number(clickCount) + " times");
    });

    mainWindow.show();

    return a.exec();
}
