#ifndef CIRCUIT_H
#define CIRCUIT_H
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <utility>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <unordered_set>
#include <unistd.h>
#include <algorithm>
#include <string>
#include <QtCharts>
#include "circuitelement.h"

class Circuit
{
public:
    std::map<std::string, CircuitElementType> mapOfNodeTypes = {
        {"and",CircuitElementType::AND},
        {"or",CircuitElementType::OR},
        {"nand",CircuitElementType::NAND},
        {"nor",CircuitElementType::NOR},
        {"xor",CircuitElementType::XOR},
        {"xnor",CircuitElementType::XNOR},
        {"not",CircuitElementType::NOT},
        {"buf",CircuitElementType::BUF},
        {"oai3",CircuitElementType::OAI3},

    };
    std::vector< std::pair< CircuitElement,std::vector<CircuitElement> > > adjacencyList;
    std::vector<CircuitElement> elements;
    std::string circuitName;
    void addEdge(CircuitElement element1, CircuitElement element2);
    void printGraph();
    bool fillFromFile(std::string fileName);
    std::string convertToSpice();
    std::string addInputOutputLines(std::vector<std::vector<int> > inputValues, double inputChangeTime, double supplyvoltage);
    QChartView* createPowerGraph(std::string file_name, double supplyvoltage);
    std::vector<std::string> outputs;
    std::vector<std::string> inputs;
    QChartView *createVoltageGraph(std::string file_name, std::vector<std::vector<int> > inputValues, double inputChangeTime, double supplyvoltage);
    void runSimulation(std::string file_path, std::string power_sim_file_name, std::string voltage_sim_file_name);
    std::string createFileName(std::string main_circuit_file_name);
};

#endif // CIRCUIT_H
