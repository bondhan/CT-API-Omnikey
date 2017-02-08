#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ct.h"
#include <QTextStream>
#include <QMessageBox>


int MainWindow::ctn = 1;

bool MainWindow::IsConnected = false;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    IsConnected = false;

    //populate port number
    for (int i = 1; i <= 512; i++)
    {
        ui->portNumbercomboBox->addItem(QString::number(i));
    }
}

MainWindow::~MainWindow()
{
    if (IsConnected)
        on_DisconnectPushButton_clicked();

    delete ui;
}

bool MainWindow::on_connectPushButton_clicked()
{
    QString errMsg = "OK";

    int RC = CT_init(ctn, ui->portNumbercomboBox->currentText().toInt());

    switch (RC)
    {
    case(ERR_INVALID):
        errMsg = "Invalid parameter or value";
        break;
    case(ERR_CT):
        errMsg = "CT error (CT not in operation)";
        break;
    case(ERR_TRANS):
        errMsg = "Non-eliminable transmission error";
        break;
    case(ERR_MEMORY):
        errMsg = "Memory assignment error in HTSI";
        break;
    case(ERR_HTSI):
        errMsg = "HTSI error";
        break;
    case(OK):
        errMsg = "Connected";
        IsConnected = true;
        break;
    default:
        errMsg = "Unknown Error";
        break;
    }

    writeOutput(errMsg);

    return IsConnected;
}


void MainWindow::writeOutput(QString msg){
    ui->outputTextEdit->append(msg);
}

bool MainWindow::on_DisconnectPushButton_clicked()
{
    QString errMsg = "OK";

    int RC = CT_close(ctn);

    switch (RC)
    {
    case(ERR_INVALID):
        errMsg = "Invalid parameter or value";
        break;
    case(ERR_CT):
        errMsg = "CT error (CT not in operation)";
        break;
    case(ERR_TRANS):
        errMsg = "Non-eliminable transmission error";
        break;
    case(ERR_MEMORY):
        errMsg = "Memory assignment error in HTSI";
        break;
    case(ERR_HTSI):
        errMsg = "HTSI error";
        break;
    case(OK):
        errMsg = "Disconnected";
        IsConnected = false;
        break;
    default:
        errMsg = "Unknown Error";
        break;
    }

    writeOutput(errMsg);

    return IsConnected;
}

void MainWindow::on_SendCmdPushButton_clicked()
{
    QString buffer = ui->inputTextEdit->toPlainText();
    QTextStream stream;
    QString status;
    QString tmp;
    QString errMsg;
    QStringList cmdRaw;

    unsigned char commandcode[256] = {'\0'};
    unsigned char sourceAddress = SAD_HOST;
    unsigned char destinationAddress = DAD_ICC1;
    unsigned char responsearray[501] = {'\0'};
    unsigned short lenr = 256;
    int cmdLength = 5;

    int RC = OK;

    status.clear();

    buffer.remove('\r');
    buffer.remove('\t');

    stream.setString(&buffer);

    while(!stream.atEnd())
    {
        tmp.clear();
        tmp = stream.readLine();

        cmdRaw = tmp.split(":");

        if (tmp.isEmpty() || tmp.isNull() || tmp.startsWith("#"))
            continue;

        tmp.remove(0x20);
        tmp.toUpper();

        if (tmp.startsWith("#"))
            continue;

        if (cmdRaw.length() > 4)
        {
            writeOutput("Incorrect command must be SOURCE:DESTINATION:COMMAND:COMMENTS \n");
            continue;
        }

        if (QString::compare(cmdRaw[0], "HOST", Qt::CaseInsensitive) == 0)
            sourceAddress = SAD_HOST;
        else if (QString::compare(cmdRaw[0], "REMOTE", Qt::CaseInsensitive) == 0)
            sourceAddress = SAD_REMOTE_HOST;
        else {
            writeOutput("Incorrect command = \n" + cmdRaw[0]+ " must be HOST or REMOTE");
            continue;
        }

        if (QString::compare(cmdRaw[1], "ICC", Qt::CaseInsensitive) == 0)
            destinationAddress = DAD_ICC1;
        else if (QString::compare(cmdRaw[1], "CT", Qt::CaseInsensitive) == 0)
            destinationAddress = DAD_CT;
        else {
            writeOutput("Incorrect command = " + cmdRaw[1] + " must be ICC or CT");
            continue;
        }

        if ( cmdRaw[2].contains(QRegExp("[^A-F,^a-f,^0-9,\\W+]")) == true  || cmdRaw[2].length() % 2 != 0)
        {
            if (!cmdRaw[2].isNull() && !cmdRaw[2].isEmpty() && !cmdRaw[2].startsWith(QChar('#'), Qt::CaseInsensitive) )
                writeOutput("Incorrect APDU command = \n" + cmdRaw[2]);

            continue;
        }

        cmdLength = asciiHexToArrayChar(cmdRaw[2], false, commandcode);

        RC = CT_data(ctn, &destinationAddress, &sourceAddress, cmdLength, &commandcode[0], &lenr, &responsearray[0]);

        switch (RC)
        {
        case(ERR_INVALID):
            errMsg = "Invalid parameter or value";
            break;
        case(ERR_CT):
            errMsg = "CT error (CT not in operation)";
            break;
        case(ERR_TRANS):
            errMsg = "Non-eliminable transmission error";
            break;
        case(ERR_MEMORY):
            errMsg = "Memory assignment error in HTSI";
            break;
        case(ERR_HTSI):
            errMsg = "HTSI error";
            break;
        case(OK):
            writeOutput("#" + cmdRaw[3]);
            writeOutput("READER->CARD: " + cmdRaw[2]);
            writeOutput("CARD->READER: " + getStringFromUnsignedChar(responsearray, lenr));
            errMsg = "";
            break;
        default:
            errMsg = "Unknown Error";
            break;
        }

        writeOutput(errMsg);

        lenr = 256;
        memset(responsearray, 0x00,256);
    }
}

void MainWindow::on_ClearPushButton_clicked()
{
    ui->outputTextEdit->setText("");
}


//unsigned char MainWindow::getSourceFromComboBox()
//{
//    if (QString::compare(ui->SourceComboBox->currentText(),"HOST", Qt::CaseInsensitive) == 0x00)
//        return 0x02;
//    else if (QString::compare(ui->SourceComboBox->currentText(),"REMOTE_HOST", Qt::CaseInsensitive) == 0x00)
//        return 0x05;
//    else
//        QMessageBox::warning(this, "Error", "Source combo box code is unknown set to HOST(0x02)");

//    return 0x00;
//}

//unsigned char MainWindow::getDestinationFromComboBox()
//{
//    if (QString::compare(ui->DestinationComboBox->currentText(),"ICC", Qt::CaseInsensitive) == 0x00)
//        return 0x00;
//    else if (QString::compare(ui->DestinationComboBox->currentText(),"CT", Qt::CaseInsensitive) == 0x00)
//        return 0x01;
//    else
//        QMessageBox::warning(this, "Error", "Destination combo box code is unknown, set to ICC(0x00)");

//    return 0x00;
//}


int MainWindow::asciiHexToArrayChar(QString data, bool inputIsString, unsigned char *dest)
{
    int len;
    int i, k = 0;
    unsigned char src[GLOBAL_BUFFER_SIZE] = {'\0'};

    QString tmpStr;

    len = data.length();

    if (inputIsString)
    {
        // if it is a string then:
        // 1. calculate the length by devide it with two

        memcpy(src, (data.toLatin1()).toHex().data(), (len*2));
    }
    else // input is ascii hex
    {
        memcpy(src, (data.toLatin1()), (len));
        len = len/2;
    }

    // 2. convert to ascii char
    for (i = 0; i < (len*2); i=i+2){

        unsigned char cPtr1[2];

        cPtr1[0] = src[i];
        cPtr1[1] = src[i+1];

        dest[k] = TwoCharToHex(cPtr1);
        k++;
    }
    dest[k] = '\0';

    tmpStr.clear();
    memset(src, 0x00, GLOBAL_BUFFER_SIZE);

    return len;
}



unsigned char MainWindow::TwoCharToHex(unsigned char *chr)
{
    return ((HexCharToNumber(chr[0]) * 0x10) + HexCharToNumber(chr[1]));
}

unsigned char MainWindow::HexCharToNumber(unsigned char HexChr)
{
    unsigned char rtnChr;

    switch (toupper(HexChr))
    {
    case '0': rtnChr = 0; break;
    case '1': rtnChr = 1; break;
    case '2': rtnChr = 2; break;
    case '3': rtnChr = 3; break;
    case '4': rtnChr = 4; break;
    case '5': rtnChr = 5; break;
    case '6': rtnChr = 6; break;
    case '7': rtnChr = 7; break;
    case '8': rtnChr = 8; break;
    case '9': rtnChr = 9; break;
    case 'A': rtnChr = 10; break;
    case 'B': rtnChr = 11; break;
    case 'C': rtnChr = 12; break;
    case 'D': rtnChr = 13; break;
    case 'E': rtnChr = 14; break;
    case 'F': rtnChr = 15; break;
    }

    return rtnChr;
}

QString MainWindow::getStringFromUnsignedChar( unsigned char *str, const int len )
{
    QString result = "";
    int lengthOfString = len;

    // print string in reverse order
    QString s;
    for( int i = 0; i < lengthOfString; i++ ){
        s = QString( "%1" ).arg( str[i], 0, 16 );

        // account for single-digit hex values (always must serialize as two digits)
        if( s.length() == 1 )
            result.append( "0" );

        result.append( s );
    }

    return result;
}
