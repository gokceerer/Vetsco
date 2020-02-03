#include "circuit.h"

void Circuit::addEdge(CircuitElement element1, CircuitElement element2)
{
    if (std::find(elements.begin(), elements.end(), element1) == elements.end()) {
        elements.push_back(element1);
        adjacencyList.push_back(std::make_pair(element1, std::vector<CircuitElement>()));

    }
    if (std::find(elements.begin(), elements.end(), element2) == elements.end()) {
        elements.push_back(element2);
        adjacencyList.push_back(std::make_pair(element2, std::vector<CircuitElement>()));
    }
    for (int i = 0; i < adjacencyList.size(); i++) {
        if (adjacencyList[i].first == element1) {
            adjacencyList[i].second.push_back(element2);
        }
    }
}

void Circuit::printGraph()
{

    std::map<CircuitElementType, std::string> mapOfGates = {
        {CircuitElementType::AND,"AND"},
        {CircuitElementType::OR,"OR"},
        {CircuitElementType::NAND,"NAND"},
        {CircuitElementType::NOR,"NOR"},
        {CircuitElementType::XOR,"XOR"},
        {CircuitElementType::XNOR,"XNOR"},
        {CircuitElementType::NOT,"NOT"},
        {CircuitElementType::BUF,"BUF"},
        {CircuitElementType::OAI3,"OAI3"},

    };
    for (auto adj : adjacencyList){
        if (adj.first.elementType == CircuitElementType::INPUT) {
            std::cout << "INPUT " << adj.first.elementName << " is connected to: ";
            for (auto connected : adj.second) {
                std::cout << connected.elementName << " ";

            }
            std::cout << std::endl;

        }
        else if (adj.first.elementType == CircuitElementType::OUTPUT) {
            std::cout << "OUTPUT " << adj.first.elementName << " is the result of: ";
            for (auto a : adjacencyList) {
                for (auto s : a.second) {
                    if (s.elementName == adj.first.elementName) {
                        std::cout << a.first.elementName << " " ;
                    }
                }
            }
            std::cout << std::endl;

        }
        else if (adj.first.elementType == CircuitElementType::WIRE) {
            std::cout << "WIRE " << adj.first.elementName << " is the result of: ";
            for (auto a : adjacencyList) {
                for (auto s : a.second) {
                    if (s.elementName == adj.first.elementName) {
                        std::cout << a.first.elementName << " ";
                    }
                }
            }
            std::cout << "also becomes an input to ";
            for (auto connected : adj.second) {
                std::cout << connected.elementName << " ";
            }
            std::cout << std::endl;
        }
        else {
            std::cout << mapOfGates[adj.first.elementType] << " gate " << adj.first.elementName << " get inputs of: ";
            for (auto a : adjacencyList) {
                for (auto s : a.second) {
                    if (s.elementName == adj.first.elementName) {
                        std::cout << a.first.elementName << " ";
                    }
                }
            }
            std::cout << "and outputs: ";
            for (auto a : adjacencyList) {
                if (a.first.elementName == adj.first.elementName) {
                    for (auto s : a.second) {
                        std::cout << s.elementName << " ";
                    }
                }
            }
            std::cout << std::endl;
        }
    }

}
bool Circuit::fillFromFile(std::string fileName)
{
    inputs.clear();
    outputs.clear();
    std::ifstream inFile;
    inFile.open(fileName);
    if (!inFile) {
        std::cerr << "Unable to open file"<<std::endl;
        return false;
    }
    std::string line;
    std::map<std::string, CircuitElementType> iow;

    while (std::getline(inFile, line)) {
        line.erase(std::remove_if(line.begin(), line.end(), [](char chr) { return chr == ' (' || chr == ' )' || chr == ';'; }), line.end());
        std::replace(line.begin(), line.end(), ',', ' ');
        std::replace(line.begin(), line.end(), '(', ' ');
        std::replace(line.begin(), line.end(), ')', ' ');

        std::istringstream stm(line);
        std::string word;
        std::vector<std::string> tokens;

        while (stm >> word) {
            tokens.push_back(word);
        }
        if (tokens.front() == "module") {
            circuitName = tokens.at(1);
        }
        if (tokens.front() == "input") {
            for (int i = 1; i < tokens.size(); i++) {
                iow[tokens.at(i)] = CircuitElementType::INPUT;
                inputs.push_back(tokens.at(i));
            }
        }
        if (tokens.front() == "output") {
            for (int i = 1; i < tokens.size(); i++) {
                iow[tokens.at(i)] = CircuitElementType::OUTPUT;
                outputs.push_back(tokens.at(i));

            }
        }
        if (tokens.front() == "wire") {
            for (int i = 1; i < tokens.size(); i++) {
                iow[tokens.at(i)] = CircuitElementType::WIRE;
            }
        }
        if (mapOfNodeTypes.find(tokens.front()) != mapOfNodeTypes.end()) {
            CircuitElement gateNode{ tokens.at(1), mapOfNodeTypes[tokens.front()]};

            CircuitElement outputNode{ tokens.at(2), iow[tokens.at(2)] };
            addEdge(gateNode, outputNode);

            for (int i = 3; i < tokens.size(); i++) {
                CircuitElement inputNode{ tokens.at(i) , iow[tokens.at(i)] };
                addEdge(inputNode, gateNode);
            }
        }
    }

    return true;
}
std::string Circuit::convertToSpice()
{

    std::map<CircuitElementType, std::string> mapOfGates = {
        {CircuitElementType::AND,"AND"},
        {CircuitElementType::OR,"OR"},
        {CircuitElementType::NAND,"NAND"},
        {CircuitElementType::NOR,"NOR"},
        {CircuitElementType::XOR,"XOR"},
        {CircuitElementType::XNOR,"XNOR"},
        {CircuitElementType::NOT,"NOT"},
        {CircuitElementType::BUF,"BUF"},
        {CircuitElementType::OAI3,"OAI3"},

    };

    char buffer[256];
    getcwd(buffer, 256 );
    std::string filename = "Converted/" + circuitName + ".sp";
    std::ofstream conversionFile;
    conversionFile.open(filename);
    conversionFile << circuitName << std::endl;
    conversionFile << ".include " << buffer << "/LibNanGate45nm/and2.sp" << std::endl;
    conversionFile << ".include " << buffer << "/LibNanGate45nm/and3.sp" << std::endl;
    conversionFile << ".include " << buffer << "/LibNanGate45nm/and4.sp" << std::endl;
    conversionFile << ".include " << buffer << "/LibNanGate45nm/and5.sp" << std::endl;
    conversionFile << ".include " << buffer << "/LibNanGate45nm/and8.sp" << std::endl;
    conversionFile << ".include " << buffer << "/LibNanGate45nm/and9.sp" << std::endl;
    conversionFile << ".include " << buffer << "/LibNanGate45nm/buf1.sp" << std::endl;
    conversionFile << ".include " << buffer << "/LibNanGate45nm/nand2.sp" << std::endl;
    conversionFile << ".include " << buffer << "/LibNanGate45nm/nand3.sp" << std::endl;
    conversionFile << ".include " << buffer << "/LibNanGate45nm/nand4.sp" << std::endl;
    conversionFile << ".include " << buffer << "/LibNanGate45nm/nand5.sp" << std::endl;
    conversionFile << ".include " << buffer << "/LibNanGate45nm/nand8.sp" << std::endl;
    conversionFile << ".include " << buffer << "/LibNanGate45nm/nor2.sp" << std::endl;
    conversionFile << ".include " << buffer << "/LibNanGate45nm/nor3.sp" << std::endl;
    conversionFile << ".include " << buffer << "/LibNanGate45nm/nor4.sp" << std::endl;
    conversionFile << ".include " << buffer << "/LibNanGate45nm/nor8.sp" << std::endl;
    conversionFile << ".include " << buffer << "/LibNanGate45nm/not1.sp" << std::endl;
    conversionFile << ".include " << buffer << "/LibNanGate45nm/oai3.sp" << std::endl;
    conversionFile << ".include " << buffer << "/LibNanGate45nm/or2.sp" << std::endl;
    conversionFile << ".include " << buffer << "/LibNanGate45nm/or3.sp" << std::endl;
    conversionFile << ".include " << buffer << "/LibNanGate45nm/or4.sp" << std::endl;
    conversionFile << ".include " << buffer << "/LibNanGate45nm/or5.sp" << std::endl;
    conversionFile << ".include " << buffer << "/LibNanGate45nm/xnor2.sp" << std::endl;
    conversionFile << ".include " << buffer << "/LibNanGate45nm/xor2.sp" << std::endl;
    //conversionFile << ".include " << buffer << "/LibNanGate45nm/PMOS_VTL.inc" << std::endl;
    //conversionFile << ".include " << buffer << "/LibNanGate45nm/NMOS_VTL.inc" << std::endl << std::endl;
    conversionFile << ".include " << buffer << "/LibNanGate45nm/techfile45nm.inc" << std::endl;

    conversionFile <<"Vx Vsup Vcc 0"<< std::endl;

    conversionFile << ".control" << std::endl;
    conversionFile << "set filetype = ascii" << std::endl;
    conversionFile << ".endc" << std::endl << std::endl;

    int andcount = 0;
    int orcount = 0;
    int nandcount = 0;
    int norcount = 0;
    int xorcount =0;
    int xnorcount =0;
    int notcount = 0;
    int bufcount =0;
    int oaicount = 0;
    for (auto adj : adjacencyList){
            if(conversionFile.is_open()){
                std::vector<CircuitElement> inputs;
                std::vector<CircuitElement> outputs;
                std::string inputsString;
                std::string outputsString;
                for (auto a : adjacencyList) {
                    for (auto s : a.second) {
                        if (s.elementName == adj.first.elementName && (a.first.elementType == CircuitElementType::WIRE || a.first.elementType == CircuitElementType::INPUT)) {
                            inputs.push_back(a.first);

                        }
                    }
                }
                for (auto a : adjacencyList) {
                    if (a.first.elementName == adj.first.elementName) {
                        for (auto s : a.second) {
                            if(s.elementType == CircuitElementType::OUTPUT || s.elementType == CircuitElementType::WIRE){
                                outputs.push_back(s);                              
                            }
                        }
                    }
                }

                for(auto i: inputs){
                    inputsString += i.elementName + " ";
                }
                for(auto o: outputs){
                    outputsString += o.elementName + " ";
                }
                if(adj.first.elementType == CircuitElementType::AND){
                    std::string andname = "Xand" + std::to_string(andcount);
                    andcount++;
                    conversionFile << andname << " " << inputsString << "Vcc 0 " << outputsString;
                    switch (inputs.size()) {
                        case 2:
                            conversionFile << "and2" << std::endl;
                            break;
                        case 3: conversionFile << "and3" <<  std::endl;
                            break;
                        case 4: conversionFile << "and4" <<  std::endl;
                            break;
                        case 5: conversionFile << "and5" <<  std::endl;
                            break;
                        case 8: conversionFile << "and8" <<  std::endl;
                            break;
                        case 9: conversionFile << "and9" <<  std::endl;
                            break;
                        default:
                            break;
                    }
                }
                else if(adj.first.elementType == CircuitElementType::OR){
                    std::string orname = "Xor" + std::to_string(orcount);
                    orcount++;
                    conversionFile << orname << " " << inputsString << "Vcc 0 " << outputsString;

                    switch (inputs.size()) {
                        case 2: conversionFile << "or2" <<  std::endl;
                            break;
                        case 3: conversionFile << "or3" <<  std::endl;
                            break;
                        case 4: conversionFile << "or4" <<  std::endl;
                            break;
                        case 5: conversionFile << "or5" <<  std::endl;
                            break;
                        default:
                            break;
                    }
                }
                else if(adj.first.elementType == CircuitElementType::NAND){
                    std::string nandname = "Xnand" + std::to_string(nandcount);
                    nandcount++;
                    conversionFile << nandname << " " << inputsString << "Vcc 0 " << outputsString;
                    switch (inputs.size()) {
                        case 2: conversionFile << "nand2" <<  std::endl;
                            break;
                        case 3: conversionFile << "nand3" <<  std::endl;
                            break;
                        case 4: conversionFile << "nand4" <<  std::endl;
                            break;
                        case 5: conversionFile << "nand5" <<  std::endl;
                            break;
                        case 8: conversionFile << "nand8" <<  std::endl;
                            break;
                        default:
                            break;
                    }
                }
                else if(adj.first.elementType == CircuitElementType::NOR){
                    std::string norname = "Xnor" + std::to_string(norcount);
                    norcount++;
                    conversionFile << norname << " " << inputsString << "Vcc 0 " << outputsString;
                    switch (inputs.size()) {
                        case 2: conversionFile << "nor2" <<  std::endl;
                            break;
                        case 3: conversionFile << "nor3" <<  std::endl;
                            break;
                        case 4: conversionFile << "nor4" <<  std::endl;
                            break;
                        case 8: conversionFile << "nor8" <<  std::endl;
                            break;
                        default:
                            break;
                    }
                }
                else if(adj.first.elementType == CircuitElementType::XOR){
                    std::string xorname = "Xxor" + std::to_string(xorcount);
                    xorcount++;
                    conversionFile << xorname << " " << inputsString << "Vcc 0 " << outputsString;
                    switch (inputs.size()) {
                        case 2: conversionFile << "xor2" <<  std::endl;
                            break;
                        default:
                            break;
                    }
                }
                else if(adj.first.elementType == CircuitElementType::XNOR){
                    std::string xnorname = "Xxnor" + std::to_string(xnorcount);
                    xnorcount++;
                    conversionFile << xnorname << " " << inputsString << "Vcc 0 " << outputsString;
                    switch (inputs.size()) {
                        case 2: conversionFile << "xnor2" <<  std::endl;
                            break;
                        default:
                            break;
                    }
                }
                else if(adj.first.elementType == CircuitElementType::NOT){
                    std::string notname = "Xnot" + std::to_string(notcount);
                    notcount++;
                    conversionFile << notname << " " << inputsString << "Vcc 0 " << outputsString;
                    switch (inputs.size()) {
                        case 1: conversionFile << "not1" <<  std::endl;
                            break;
                        default:
                            break;
                    }
                }
                else if(adj.first.elementType == CircuitElementType::BUF){
                    std::string bufname = "Xbuf" + std::to_string(bufcount);
                    bufcount++;
                    conversionFile << bufname << " " << inputsString << "Vcc 0 " << outputsString;
                    switch (inputs.size()) {
                        case 1: conversionFile << "buf1" <<  std::endl;
                            break;
                        default:
                            break;
                    }
                }
                else if(adj.first.elementType == CircuitElementType::OAI3){
                    std::string oainame = "Xoai" + std::to_string(oaicount);
                    oaicount++;
                    conversionFile << oainame << " " << inputsString << "Vcc 0 " << outputsString;
                    switch (inputs.size()) {
                        case 3: conversionFile << "oai3" <<  std::endl;
                            break;
                        default:
                            break;
                    }
                }

            }
    }


    conversionFile.close();
    return filename;
}
std::string Circuit::addInputOutputLines(std::vector<std::vector<int> > inputValues, double inputChangeTime, double supplyvoltage){
    char cwd[1024];
    getcwd(cwd, 1024 );
    std::string curr_work_dir{cwd};
    std::string filename = curr_work_dir + "/Converted/" + circuitName;
    std::string new_file_name = createFileName(filename);
    std::ifstream  main_circuit_file(filename + ".sp", std::ios::binary);
    std::ofstream  conversionFile(new_file_name,   std::ios::binary);

    conversionFile << main_circuit_file.rdbuf();
    main_circuit_file.close();
    conversionFile.close();

    conversionFile.open(new_file_name,std::fstream::app);

    int count = 0;
    for(auto i : outputs){
        conversionFile << "C"+std::to_string(count)+" "+i+" 0 20f"<< std::endl;
        count++;
    }
    conversionFile << "VCC Vsup 0 DC=" + std::to_string(supplyvoltage) << std::endl;
    count =0;
    for(int i=0; i< inputs.size();i++){
        std::string inputName = "circinput" + i;
        std::vector<int> inValues = inputValues.at(i);
        std::string inputString = "Vp"+std::to_string(count)+" "+inputs.at(i) + " 0 0 PWL(";
        double timeval = 0;
        for(auto in : inValues){
            inputString += std::to_string(timeval) + "N " + std::to_string(in*supplyvoltage) + "V ";
            timeval += inputChangeTime;
            inputString += std::to_string(timeval) + "N " + std::to_string(in*supplyvoltage) + "V ";
            timeval+=0.01;
        }
        inputString +=")";
        conversionFile << inputString << std::endl;
        count++;

    }
    conversionFile <<".TRAN 0.1NS " << std::to_string(inputChangeTime*inputValues.at(0).size() + 1) << "NS" << std::endl;
    conversionFile << ".END" << std::endl;
    conversionFile.close();

    return new_file_name;
}

QChartView* Circuit::createPowerGraph(std::string file_name, double supplyvoltage){
    std::ifstream sim_file;
    std::string line;
    std::vector<std::string> file_content;

    sim_file.open(file_name);

    if(!sim_file.is_open()){
        return nullptr;
    }

    while (std::getline(sim_file, line)) {
        file_content.push_back(line);
    }
    sim_file.close();
    file_content.erase(std::remove(file_content.begin(), file_content.end(), ""), file_content.end());
    file_content.erase (file_content.begin(),file_content.begin()+4);
    file_content.erase(std::remove(file_content.begin(), file_content.end(), "Variables:"), file_content.end());
    file_content.erase(std::remove(file_content.begin(), file_content.end(), "Values:"), file_content.end());

    for(int i = 0; i < 2; i++){
        file_content[i].erase(file_content[i].begin(), file_content[i].begin() + file_content[i].find_first_of(':') + 2);
    }

    for(int i = 2; i < file_content.size(); i++){
        if(file_content[i][0] == '\t') {
            file_content[i].erase(file_content[i].begin(), file_content[i].begin() +1);
        }
        else {
            file_content[i].erase(file_content[i].begin(), file_content[i].begin() + file_content[i].find_first_of('\t') +1);
        }
    }

    int variable_count = stoi(file_content[0]);

    QSplineSeries* power_spline = new QSplineSeries();

    int point_count = stoi(file_content[1]);
    int variable_start = variable_count + 2;


    for(int i = 0; i < point_count ; i++){
        power_spline->append(stof(file_content[variable_start])*1000000000 ,stof(file_content[variable_start+1])*supplyvoltage);
        variable_start+=variable_count;
    }

    QChart* power_chart = new QChart();
    power_chart->legend()->hide();
    power_chart->addSeries(power_spline);
    std::string chartName = "Power Simulation Results";
    power_chart->setTitle(chartName.c_str());
    power_chart->createDefaultAxes();
    power_chart->setObjectName("powerchart");


    QChartView* power_chart_view = new QChartView(power_chart);

    power_chart_view->setRenderHint(QPainter::Antialiasing);
    power_chart_view->resize(500,500);

    return power_chart_view;

}

QChartView* Circuit::createVoltageGraph(std::string file_name,std::vector<std::vector<int> > inputValues, double inputChangeTime,double supplyvoltage){
    QLineSeries* voltage_series = new QLineSeries();

    QChart* voltage_chart = new QChart();

    double time_val = 0;
    double padding = 0;
    for(int i = 0 ; i < inputValues.size(); i++){
        QLineSeries *inputSeries = new QLineSeries();
        for(int j = 0 ; j < inputValues.at(i).size(); j++){
            padding = (i+1)*2*supplyvoltage;
            inputSeries->append(time_val,inputValues[i].at(j)*supplyvoltage+padding);

            time_val+=inputChangeTime;
            inputSeries->append(time_val,inputValues[i].at(j)*supplyvoltage+padding);

            time_val+=0.01;
        }
        time_val=0;
        voltage_chart->addSeries(inputSeries);
    }
    voltage_chart->legend()->hide();
    voltage_chart->addSeries(voltage_series);
    std::string chartName = "Voltage Simulation Results";
    voltage_chart->setTitle(chartName.c_str());
    voltage_chart->createDefaultAxes();
    voltage_chart->setObjectName("voltagechart");


    std::ifstream sim_file;
    std::string line;
    std::vector<std::string> file_content;

    sim_file.open(file_name);

    if(!sim_file.is_open()){
        return nullptr;
    }
    while (std::getline(sim_file, line)) {
        file_content.push_back(line);
    }
    sim_file.close();
    file_content.erase(std::remove(file_content.begin(), file_content.end(), ""), file_content.end());
    file_content.erase (file_content.begin(),file_content.begin()+4);
    file_content.erase(std::remove(file_content.begin(), file_content.end(), "Variables:"), file_content.end());
    file_content.erase(std::remove(file_content.begin(), file_content.end(), "Values:"), file_content.end());

    for(int i = 0; i < 2; i++){
        file_content[i].erase(file_content[i].begin(), file_content[i].begin() + file_content[i].find_first_of(':') + 2);
    }

    for(int i = 2; i < file_content.size(); i++){
        if(file_content[i][0] == '\t') {
            file_content[i].erase(file_content[i].begin(), file_content[i].begin() +1);
        }
        else {
            file_content[i].erase(file_content[i].begin(), file_content[i].begin() + file_content[i].find_first_of('\t') +1);
        }
    }

    int variable_count = stoi(file_content[0]);

    int point_count = stoi(file_content[1]);
    int variable_start = variable_count + 2;

    for(int i = 0; i < point_count ; i++){
        voltage_series->append(stof(file_content[variable_start])*1000000000 ,stof(file_content[variable_start+1]));
        variable_start+=variable_count;
    }

    QChartView* voltage_chart_view = new QChartView(voltage_chart);

    voltage_chart_view->setRenderHint(QPainter::Antialiasing);
    voltage_chart_view->resize(500,500);

    return voltage_chart_view;
}

void Circuit::runSimulation(std::string file_path, std::string power_sim_file_name, std::string voltage_sim_file_name){
    char cwd[1024];
    getcwd(cwd, 1024 );
    int ff, p[2];
    if(pipe(p) != 0){
       std::exit(EXIT_FAILURE);
    }

    if ((ff = fork()) == -1 )
    {
        fprintf(stderr, "Fork failed\n");
        std::exit(EXIT_FAILURE);
    }

    if(ff == 0){
        ::close(0);
        ::close(p[1]);
        if ( dup2(p[0], 0) == -1 )
        {
          fprintf(stderr, "dup2(0) failed\n");
          std::exit(EXIT_FAILURE);
        }
        if ( execlp("ngspice", "ngspice", "-p", NULL) == -1 )
        {
               fprintf(stderr, "Exec failed\n");
               std::exit(EXIT_FAILURE);
        }
    }
    ::close(p[0]);
    char msg1[1024] = "source ";
    if(file_path[0] == '\0') {
        return;
    }

    strcat(msg1,file_path.c_str());
    strcat(msg1,"\n");
    char msg2[1024] = "run\n";
    char msg3[1024] = "write ";
    char msg4[1024] = "write ";
    strcat(msg3, (power_sim_file_name + " ").c_str());
    strcat(msg3,"vx#branch");
    strcat(msg4,(voltage_sim_file_name + " ").c_str());

    for(int i = 0 ; i < outputs.size() ; i++ ){
        std::string voltage_node = "V(" + outputs.at(i) + ")";
        strcat(msg4,voltage_node.c_str());
        if(i != outputs.size() - 1) {
            strcat(msg4, " ");
        }
    }

    strcat(msg3, "\n");

    write(p[1], (void *)msg1, strlen(msg1));
    write(p[1], (void *)msg2, strlen(msg2));
    write(p[1],(void *)msg3,strlen(msg3));
    write(p[1],(void *)msg4,strlen(msg4));

    ::close(p[1]);
}

std::string Circuit::createFileName(std::string main_circuit_file_name){
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];
    time (&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer,sizeof(buffer),"%d-%m-%Y_%H-%M-%S",timeinfo);
    std::string str(buffer);
    std::string file_name = main_circuit_file_name + "_" + str + + ".sp";
    return file_name;
}
