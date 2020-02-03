#include "circuitelement.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>

CircuitElement::CircuitElement(){
    elementName = "";
}
CircuitElement::CircuitElement(std::string name, CircuitElementType type)
{
    elementName = name;
    elementType = type;
}
bool CircuitElement::operator==(const CircuitElement& other)
{
    if (this->elementName == other.elementName && this->elementType == other.elementType)
        return true;
    return false;
}



