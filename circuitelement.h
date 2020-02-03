#ifndef CIRCUITELEMENT_H
#define CIRCUITELEMENT_H

#include <QGraphicsItem>
#include <string>

enum class CircuitElementType {
    AND,
    OR,
    NAND,
    NOR,
    XOR,
    XNOR,
    NOT,
    INPUT,
    OUTPUT,
    WIRE,
    OAI3,
    BUF
};

class CircuitElement
{
public:
    CircuitElement();
    CircuitElement(std::string name, CircuitElementType type);
    std::string elementName;
    CircuitElementType elementType;
    bool operator==(const CircuitElement& other);
};

#endif // CIRCUITELEMENT_H
