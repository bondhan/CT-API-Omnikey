#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#define GLOBAL_BUFFER_SIZE 1024

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    bool on_connectPushButton_clicked();
    bool on_DisconnectPushButton_clicked();
    void on_SendCmdPushButton_clicked();
    void writeOutput(QString);

    void on_ClearPushButton_clicked();
//    unsigned char getSourceFromComboBox();
//    unsigned char getDestinationFromComboBox();
    int asciiHexToArrayChar(QString data, bool inputIsString, unsigned char *dest);
    unsigned char TwoCharToHex(unsigned char *chr);
    unsigned char HexCharToNumber(unsigned char HexChr);
    QString getStringFromUnsignedChar( unsigned char *str, const int len );
private:
    Ui::MainWindow *ui;
    static bool IsConnected;
    static int ctn;
};

#endif // MAINWINDOW_H
