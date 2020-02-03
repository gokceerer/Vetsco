#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mkdir("Converted",0777);
    mkdir("SimulationResults",0777);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this,"Choose file","C://","Verilog files (*.v)");
    if(!file_name.isEmpty()){
        ui->textEdit->setText(file_name);
    }
    if(CurrentCircuit::circ.inputs.size()!=0){
        for(int i = 0 ; i < CurrentCircuit::circ.inputs.size() ; i++){
            std::string inputName = "circinput" + std::to_string(i);
            QTextEdit * edit = findChild<QTextEdit*>(QString(inputName.c_str()));
            QLabel *label = findChild<QLabel*>(QString(inputName.c_str()));
            delete edit;
            delete label;
        }
        QTextEdit * timeedit = findChild<QTextEdit*>(QString("time"));
        QLabel *timelabel = findChild<QLabel*>(QString("time"));
        delete timeedit;
        delete timelabel;
    }
    ui->textEdit_2->clear();
    ui->textEdit_3->clear();
    ui->textEdit_4->clear();

    QLabel *inputlabel = findChild<QLabel*>(QString("inputlabel"));
    delete inputlabel;
    QLabel *supplyvoltagelabel = findChild<QLabel*>(QString("supplyvoltagelabel"));
    QTextEdit * supplyvoltage = findChild<QTextEdit*>(QString("supplyvoltage"));

    delete supplyvoltagelabel;
    delete supplyvoltage;

}

void MainWindow::on_pushButton_2_clicked()
{
    if(CurrentCircuit::circ.elements.size()!=0){
        CurrentCircuit::circ.elements.clear();
        CurrentCircuit::circ.adjacencyList.clear();
    }
    if(ui->textEdit->toPlainText().toUtf8().constData()[0] == '\0'){
        return;
    }
    CurrentCircuit::circ.fillFromFile(ui->textEdit->toPlainText().toUtf8().constData());
    char buffer[256];
    getcwd(buffer, 256 );
    std::string dirname{buffer};
    ui->textEdit_2->setText(QString::fromStdString(dirname + "/" + CurrentCircuit::circ.convertToSpice()));

    QLabel *inputlabel = new QLabel(this);
    inputlabel->setText("Inputs:");
    inputlabel->setObjectName("inputlabel");
    inputlabel->setMaximumHeight(40);
    ui->verticalLayout_13->addWidget(inputlabel);
    inputlabel->show();

    for(int i = 0 ; i < CurrentCircuit::circ.inputs.size() ; i++){
        QHBoxLayout *inputLayout = new QHBoxLayout();
        QTextEdit *edit = new QTextEdit(this);
        QLabel *label = new QLabel(this);
        std::string inputName = "circinput" + std::to_string(i);
        label->setText((CurrentCircuit::circ.inputs.at(i)+":").c_str());
        label->setObjectName(inputName.c_str());
        label->setMinimumWidth(40);

        edit->setObjectName(inputName.c_str());
        edit->setMaximumHeight(31);

        inputLayout->addWidget(label);
        inputLayout->addWidget(edit);

        ui->verticalLayout_13->addLayout(inputLayout);
    }

    QHBoxLayout *timeLayout = new QHBoxLayout();

    QLabel *timelabel = new QLabel(this);
    timelabel->setText("Input change interval (ns):");
    timelabel->setObjectName("time");

    QTextEdit *inputChangeTime = new QTextEdit(this);
    inputChangeTime->setObjectName("time");
    inputChangeTime->setMaximumHeight(31);

    timeLayout->addWidget(timelabel);
    timeLayout->addWidget(inputChangeTime);

    ui->verticalLayout_13->addLayout(timeLayout);

    QHBoxLayout * vsupplyLayout = new QHBoxLayout();
    QLabel *vsupplylabel = new QLabel(this);
    vsupplylabel->setText("Supply voltage (V):");
    vsupplylabel->setObjectName("supplyvoltagelabel");

    QTextEdit *vsupplyvalue = new QTextEdit(this);
    vsupplyvalue->setObjectName("supplyvoltage");
    vsupplyvalue->setMaximumHeight(31);

    vsupplyLayout->addWidget(vsupplylabel);
    vsupplyLayout->addWidget(vsupplyvalue);

    ui->verticalLayout_13->addLayout(vsupplyLayout);

}

void MainWindow::on_pushButton_3_clicked()
{

    std::vector<std::vector<int> > inputValues;
    QTextEdit *time = findChild<QTextEdit*>(QString("time"));

    if(time == NULL){
        return;
    }

    std::string timeString(time->toPlainText().toUtf8().constData());
    if(timeString[0] == '\0'){
        return;
    }

    double inputChangeTime= std::stod(timeString);
    QTextEdit *suppvoltage = findChild<QTextEdit*>(QString("supplyvoltage"));
    std::string suppVoltageString(suppvoltage->toPlainText().toUtf8().constData());
    double supplyvoltage;

    if(suppvoltage == NULL){
        return;
    }
    if(suppVoltageString[0] == '\0'){
        supplyvoltage = 0;
    }
    else{
        supplyvoltage = std::stod(suppVoltageString);
    }


    for(int i = 0 ; i < CurrentCircuit::circ.inputs.size(); i++){
        std::string inputName = "circinput" + std::to_string(i);

        QTextEdit *a =findChild<QTextEdit*>(QString(inputName.c_str()));
        std::string inputLine(a->toPlainText().toUtf8().constData());

        std::vector<int> inputVect;
        for(auto c : inputLine){
            if(c == '0'){
                inputVect.push_back(0);
            }
            else if(c == '1'){
                inputVect.push_back(1);
            }
            else{
                inputVect.push_back(0);
            }
        }

        inputValues.push_back(inputVect);
    }
    std::string newFilePath = CurrentCircuit::circ.addInputOutputLines(inputValues,inputChangeTime,supplyvoltage);
    ui->textEdit_2->setText(newFilePath.c_str());

    std::string file_path(ui->textEdit_2->toPlainText().toUtf8());
    char cwd[1024];
    getcwd(cwd, 1024 );

    std::string sim_file_name = newFilePath.substr(newFilePath.find_last_of('/')+1,newFilePath.find('.')-newFilePath.find_last_of('/')-1);
    std::string dirname{cwd};

    ui->textEdit_3->setText(QString::fromStdString(dirname + "/SimulationResults/" + sim_file_name + "_currentSimulation.txt" ));
    ui->textEdit_4->setText(QString::fromStdString(dirname + "/SimulationResults/" + sim_file_name + "_voltageSimulation.txt" ));

    CurrentCircuit::circ.runSimulation(file_path,ui->textEdit_3->toPlainText().toUtf8().constData(), ui->textEdit_4->toPlainText().toUtf8().constData());



}

void MainWindow::on_pushButton_4_clicked()
{
    std::ifstream sim_file;
    std::string line;
    std::vector<std::string> file_content;

    std::string file_name(ui->textEdit_3->toPlainText().toUtf8().constData());

    if(file_name[0] == '\0'){
        return;
    }

    QTextEdit *suppvoltage = findChild<QTextEdit*>(QString("supplyvoltage"));
    double supplyvoltage;
    if(suppvoltage->toPlainText().toUtf8().constData()[0] ==  '\0'){
        supplyvoltage = 0;
    }
    else{
        std::string suppVoltageString(suppvoltage->toPlainText().toUtf8().constData());
        supplyvoltage = std::stod(suppVoltageString);
    }

    QChartView* power_chart = CurrentCircuit::circ.createPowerGraph(file_name,supplyvoltage);

    power_chart->show();

}

void MainWindow::on_pushButton_5_clicked()
{
    std::vector<std::vector<int> > inputValues;

    for(int i = 0 ; i < CurrentCircuit::circ.inputs.size(); i++){
        std::string inputName = "circinput" + std::to_string(i);

        QTextEdit *a =findChild<QTextEdit*>(QString(inputName.c_str()));
        std::string inputLine(a->toPlainText().toUtf8().constData());

        std::vector<int> inputVect;
        for(auto c : inputLine){
            if(c == '0'){
                inputVect.push_back(0);
            }
            else if(c == '1'){
                inputVect.push_back(1);
            }
        }

        inputValues.push_back(inputVect);
    }

    QTextEdit *suppvoltage = findChild<QTextEdit*>(QString("supplyvoltage"));
    double supplyvoltage;
    if(suppvoltage == NULL){
        return;
    }
    if(suppvoltage->toPlainText().toUtf8().constData()[0] == '\0'){

        supplyvoltage = 0;
    }
    else{
        std::string suppVoltageString(suppvoltage->toPlainText().toUtf8().constData());
        supplyvoltage = std::stod(suppVoltageString);
    }

    QTextEdit *time = findChild<QTextEdit*>(QString("time"));
    std::string timeString(time->toPlainText().toUtf8().constData());
    if(time == NULL){
        return;
    }
    if(timeString[0] == '\0'){
        return;
    }
    double inputChangeTime= std::stod(timeString);

    std::string file_name(ui->textEdit_4->toPlainText().toUtf8().constData());
    if(file_name[0] == '\0'){
        return;
    }

    QChartView* voltage_chart = CurrentCircuit::circ.createVoltageGraph(file_name,inputValues,inputChangeTime,supplyvoltage);

    voltage_chart->show();
}
