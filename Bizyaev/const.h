#ifndef CONST_H
#define CONST_H

#include <map>
#include <functional>
#include <QString>
#include <calculator.h>

enum class Sender {Operator, Equals};

const std::map <QString, QString> digitsMapping =  {
    {"DIGIT.0","0"},
    {"DIGIT.1","1"},
    {"DIGIT.2","2"},
    {"DIGIT.3","3"},
    {"DIGIT.4","4"},
    {"DIGIT.5","5"},
    {"DIGIT.6","6"},
    {"DIGIT.7","7"},
    {"DIGIT.8","8"},
    {"DIGIT.9","9"}
};

const std::map <QString, QString> trigonometryMapping =  {
    {"TRIG.SIN","sin"},
    {"TRIG.COS","cos"},
    {"TRIG.TAN","tan"}
};

const std::map <QString, QString> functionsMapping =  {
    {"FUNC.EXP","exp"},
    {"FUNC.LN","log"},
    {"FUNC.SQRT","sqrt"}

};

const std::map <QString, QString> operationsMapping =  {
    {"OPER.PLUS","+"},
    {"OPER.MINUS","-"},
    {"OPER.DIV","/"}, //÷
    {"OPER.MULTIPLY","*"},
    {"FUNC.CARET","^"}     // power
};

const std::map <QString, unsigned char> commandsMapping =  {
    {"C",0},
    {"EQUALS",1},
    {"POINT",2},
    {"CLEAR",3},
};

const std::map <QString, QString> symbolsMapping =  {
    {"π","pi"},
    {"e","exp(1)"}
};

const std::map <QString, QString> bracketsMapping =  {
    {"BRACKET.LEFT","("},
    {"BRACKET.RIGHT",")"}
};

#endif // CONST_H
