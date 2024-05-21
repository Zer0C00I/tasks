#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QString>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("Calculator");

    QLabel *display = new QLabel("0");
    display->setAlignment(Qt::AlignRight);
    display->setFixedHeight(50);

    QGridLayout *gridLayout = new QGridLayout;
    QString buttonLabels[16] = {"7", "8", "9", "/",
                                "4", "5", "6", "*",
                                "1", "2", "3", "-",
                                "C", "0", "=", "+"};

    for (int i = 0; i < 16; ++i)
    {
        QPushButton *button = new QPushButton(buttonLabels[i]);
        button->setFixedSize(50, 50);
        gridLayout->addWidget(button, i / 4, i % 4);

        if (buttonLabels[i] == "C")
        {
            QObject::connect(button, &QPushButton::clicked, [=]()
                             { display->setText("0"); });
        }
        else if (buttonLabels[i] == "=")
        {
            QObject::connect(button, &QPushButton::clicked, [=]()
                             {
                QString expression = display->text();
                QRegExp regex("(\\+|\\-|\\*|\\/)");
                QStringList operands = expression.split(regex);
                if (operands.length() == 2) {
                    double result;
                    if (expression.contains("+"))
                        result = operands[0].toDouble() + operands[1].toDouble();
                    else if (expression.contains("-"))
                        result = operands[0].toDouble() - operands[1].toDouble();
                    else if (expression.contains("*"))
                        result = operands[0].toDouble() * operands[1].toDouble();
                    else if (expression.contains("/"))
                        result = operands[0].toDouble() / operands[1].toDouble();
                    display->setText(QString::number(result));
                } });
        }
        else
        {
            QObject::connect(button, &QPushButton::clicked, [=]()
                             {
                if (display->text() == "0" || display->text() == "Error") {
                    display->setText(button->text());
                } else {
                    display->setText(display->text() + button->text());
                } });
        }
    }

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(display);
    layout->addLayout(gridLayout);
    window.setLayout(layout);

    window.show();
    return app.exec();
}
