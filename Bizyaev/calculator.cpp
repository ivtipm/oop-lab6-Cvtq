#include "calculator.h"
#include "const.h"

/* Constructor */
BackEnd::BackEnd(QObject *parent) :QObject(parent) {}

/* Expression block START */
QString BackEnd::expression() const{
    return _expression;
}
void BackEnd::setExpression(QString expression){
    if (_expression == expression) return;
    _expression = expression;
    emit expressionChanged(expression);
}
void BackEnd::addToExpression(QString symbol){
    _expression += symbol;
    lastInExpressionIsOperator = false;
    emit expressionChanged(_expression);
}
void BackEnd::clearExpression(){
    if (expression() == "") return;
    _expression = "";
    emit expressionChanged(expression());
}
/* Expression block END */

/* Fake Expression block START */
QString BackEnd::fakeExpression() const {
    return _fakeExpression;
}
void BackEnd::setFakeExpression(QString expression){
    if (fakeExpression() == expression) return;
    _fakeExpression = expression;
    emit fakeExpressionChanged(fakeExpression());
}
void BackEnd::clearFakeExpression(){
    if (fakeExpression() == "") return;
    _fakeExpression = "";
    emit fakeExpressionChanged(fakeExpression());
};
/* Fake Expression block  END */

/* Input block START */
QString BackEnd::input() const{
    return _input;
}
void BackEnd::setInput(QString input){
    if (_input == input) return;
    _input = input;
    emit inputChanged(_input);
}
void BackEnd::clearInput(){
    if (_input == "") return;
    _input = "";
    emit inputChanged(input());
}
void BackEnd::resetInput(){
    if (input() == "0") return;
    _input = "0";
    emit inputChanged(input());
}
/* Input block END */

/* Result block START */
QString BackEnd::result() const{
    return _result;
}
void BackEnd::setResult(QString result){
    if (_result == result) return;
    _result = result;
    emit resultChanged(_result);
}
void BackEnd::clearResult(){
    if (_result == "") return;
    _result = "";
    emit resultChanged(_result);
}
/* Result block END */

/* Добавляет к результату строку, полученную на входе */
void BackEnd::addToInput(QString symbol){
    if (input() == "0"){
        if (symbol != "0") {
            _input = symbol;
            emit inputChanged(input());
       }
    }
    else {
        _input += symbol;
        emit inputChanged(input());
    }
}
void BackEnd::chopInput(){
    if (input().length() == 1) {
        resetInput();
    }
    else {
        _input.chop(1);
        emit inputChanged(input());
    }
}

/* Добавляет точку в ввод, при условии, что ее еще нет */
void BackEnd::addPoint(){
    if (!input().contains(".")){
        if (input().isEmpty()){
            setInput("0");
            clearResult();
        }
        _input += ".";
        emit inputChanged(input());
    }
}

/* Добавляет функцию*/
void BackEnd::addFunction(QString function){
    if (!input().isEmpty()){
        if (lastInExpressionIsOperator && (lastOperator != "=")){
            lastInExpressionIsOperator = false;
            lastOperatorLength = 0;
            setFakeExpression( fakeExpression() + function+"("+input()+")");
            setExpression(fakeExpression());
            clearInput();
        } else
            if (expression().isEmpty()){
                lastInExpressionIsOperator = false;
                lastOperatorLength = 0;
                setFakeExpression(function+"("+input()+")");
                setExpression(fakeExpression());
                clearInput();
            }
    }
    else {
        if ((lastInExpressionIsOperator) && (lastOperator == "=") && (!result().isEmpty())){
            lastInExpressionIsOperator = false;
            lastOperatorLength = 0;
            setFakeExpression(function+"("+result()+")");
            setExpression(fakeExpression());
            clearInput();
        }

    }
    calculate();
}

/* Добавляет к не настоящему выражению оператор. Если выражение оказалось корректным, оно высчитывается */
void BackEnd::addOperator(QString operatorName){

    /* Вычисление */
    lastOperatorLength = 1;    

    if (input().right(1) == ".") { // Проверка на лишнюю точку в конце ввода
        _input.chop(1);
        //emit inputChanged(input());
    }

    /* Проверка, стоит ли некий оператор в конце выражения */
    if (lastInExpressionIsOperator){
        if (input().isEmpty()){ /* Проверка, пуст ли ввод (ввод не ожидается) */
            //if (lastOperator == operatorName) () нет смысла проводить дальнейшие операции, если оператор тот же. единственное что результат стерт и пересчитать его надо
              _fakeExpression.chop(lastOperatorLength); /* Если нет - последний символ выражения удаляется и добавляется новый оператор (в конце функции)*/
        }
        else { /* Ввод оказался не пустым (ввод ожидается) */
            if (lastOperator == "="){
                setFakeExpression(input());
            } else
                setFakeExpression(fakeExpression() + input());
        }
    }
    else {  /* Оператора в выражении нет, так что он просто добавляется */
            /* Если выражение пусто - сначала заносится ввод (там всегда будет 0), после чего оператор */
        if (fakeExpression().isEmpty()) {
            setFakeExpression(input());
        }
        else{
            setFakeExpression(fakeExpression() + input());
        }
    }

    setExpression(fakeExpression()); // Настоящее выражение останется корректным (за исключением случая, когда там будет одно число)
    setFakeExpression(fakeExpression() + operatorName); // Отображаемое выражение получает оператор в конце
    qDebug() << "В Операторе Выражение: " << expression() << " Отображаемое Выражение: " << fakeExpression() << " Результат: " << result();
    performCalculation(Sender::Operator);  // Высчитывание результата от настоящего выражения

    lastOperator = operatorName;  // Запоминание оператора, который только что был добавлен в выражение
    lastInExpressionIsOperator = true;  // Установка флага, что последнее в выражении - оператор
}

/* Функция берет корректное (настоящее) выражение и считает его, результат заносится в _result */
void BackEnd::calculate(){

    if (!expression().isEmpty()){
     exprtk::symbol_table<double> symbol_table;
     exprtk::expression<double>   expression_exptrk;
     exprtk::parser<double>       parser;
     std::string expression_string = (expression()).toUtf8().constData();
     symbol_table.add_constants();
     expression_exptrk.register_symbol_table(symbol_table);
     parser.compile(expression_string,expression_exptrk);

     double calcResult = expression_exptrk.value();
     setResult(QString::fromStdString(std::to_string(calcResult)).remove(QRegExp("\\.?0+$")));

//        double calcResult = calculator::eval(expression_string);
//        setResult(QString::fromStdString(std::to_string(calcResult)).remove(QRegExp("\\.?0+$")));

    qDebug() << "Выражение: " << expression() << " Отображаемое Выражение: " << fakeExpression() << " Результат: " << result();
    }
}

void BackEnd::performCalculation(Sender sender){

    // Проверка, стоит ли знак равно в конце выражения
    //if (!nowShowingResult){

        switch (sender) {
        case Sender::Operator:
            calculate();
            break;
        case Sender::Equals:
            if ((lastInExpressionIsOperator) && (lastOperator == "=") && (!result().isEmpty())){
                setExpression(result());
                setFakeExpression(result()+"=");
                nowShowingResult = true;
                lastInExpressionIsOperator = true;
                lastOperatorLength = 1;
                lastOperator = "=";
                qDebug() << "Выражение: " << expression() << " Отображаемое выражение: " << fakeExpression() << " Результат: " << result();
                return;
            }
            /* Проверка, пусто ли выражение */
            if (expression().isEmpty()){
                /* Если ввод не пуст (в нем находится число), то оно принимается за ответ */
                if (!input().isEmpty()){
                    setExpression(input());
                    setFakeExpression(expression()+"=");
                    setResult(input());
                    lastOperatorLength = 1;
                    lastInExpressionIsOperator = true;
                    lastOperator = "=";
                }
               // else{
                    // ответа нет, т.к. нет данных. throw exception?
               // }
            }
            else {
                /* Выражение не пустое, тогда вычисляем его */
                if (!input().isEmpty() && lastInExpressionIsOperator && lastOperator != "="){
                   setExpression(fakeExpression() + input());
                   setFakeExpression(expression() + "=");
                   lastOperatorLength = 1;
                   lastInExpressionIsOperator = true;
                   lastOperator = "=";
                }
                else {
                    if (input().isEmpty()){
                        _fakeExpression.chop(lastOperatorLength);
                        setFakeExpression(fakeExpression()+"=");
                        lastOperatorLength = 1;
                        lastInExpressionIsOperator = true;
                        lastOperator = "=";
                    }
                }

                calculate();
            }
            break;
        }
        clearInput(); // Ввод очищается
        nowShowingResult = true;
        qDebug() << "Выражение: " << expression() << " Отображаемое Выражение: " << fakeExpression() << " Результат: " << result();
}

/* Очищает результат, выражение и историю */
void BackEnd::clear(){
    lastInExpressionIsOperator = false;
    lastOperatorLength = 0;
    nowShowingResult = false;
    resetInput();
    clearResult();
    clearExpression();
    clearFakeExpression();
}

// Добавляет символ в выражение
void BackEnd::addSymbol(QString symbol){
    exprtk::symbol_table<double> symbol_table;
    exprtk::expression<double>   expression_exptrk;
    exprtk::parser<double>       parser;
    std::string expression_string = (symbol).toUtf8().constData();
    symbol_table.add_constants();
    expression_exptrk.register_symbol_table(symbol_table);
    parser.compile(expression_string,expression_exptrk);
    clearResult();
    double calcResult = expression_exptrk.value();
    setInput(QString::fromStdString(std::to_string(calcResult)).remove(QRegExp("\\.?0+$")));
}

// Добавление скобки в выражение
void BackEnd::addBracket(QString symbol){
    if (symbol == "("){
        if (lastInExpressionIsOperator && lastOperator != "="){ //&& !input().isEmpty()
            if (input().isEmpty()){
                setFakeExpression(fakeExpression() + "(");
            }
            else {
                    setFakeExpression(fakeExpression() + "(");
            }
        }
//        else {
//            if (fakeExpression().isEmpty()){
//                   setFakeExpression("("+input());
//                   clearInput();
//            }
//        }
    }
    else {
        if (!fakeExpression().isEmpty() && !lastInExpressionIsOperator){
            if (fakeExpression().count("(") > fakeExpression().count(")")){
                setFakeExpression(fakeExpression() + ")");
                setExpression(fakeExpression());
                calculate();
            }
        }
        else {
            if (lastInExpressionIsOperator && lastOperator != "=" && !input().isEmpty()){
                if (fakeExpression().count("(") > fakeExpression().count(")")){
                    setFakeExpression(fakeExpression() + input() + ")");
                    clearInput();
                    setExpression(fakeExpression());
                    calculate();
                }
            }
            else {
                if (!lastInExpressionIsOperator  && !input().isEmpty()){
                    if (fakeExpression().count("(") > fakeExpression().count(")")){
                        setFakeExpression(fakeExpression() + input() + ")");
                        clearInput();
                        setExpression(fakeExpression());
                        calculate();
                    }
                }
            }
        }
        lastInExpressionIsOperator = false;
        lastOperatorLength = 0;
    }
}
/* Обработка событий, потсупаемых из QML */
void BackEnd::submitEvent(QString eventName){

    /* Поиск события из карты цифр */
    {auto it = digitsMapping.find(eventName);
     if (it!=digitsMapping.end()){
        addToInput(it->second);
        clearResult();
        return;
     }
    }

    /* Поиск события из карты операторов */
    {auto it = operationsMapping.find(eventName);
     if (it!=operationsMapping.end()){
       addOperator(it->second);
        return;
     }
    }

    /* Поиск события из карты команд */
    {auto it = commandsMapping.find(eventName);
     if (it!=commandsMapping.end()){
        execute(it->second);
        return;
     }
    }

    /* Поиск события из карты функций */
    {auto it = functionsMapping.find(eventName);
     if (it!=functionsMapping.end()){
        addFunction(it->second);
        return;
     }
    }

    /* Поиск события из карты символов */
    {auto it = symbolsMapping.find(eventName);
     if (it!=symbolsMapping.end()){
        addSymbol(it->second);
        return;
     }
    }

    /* Поиск события из карты скобок */
    {auto it = bracketsMapping.find(eventName);
     if (it!=bracketsMapping.end()){
        addBracket(it->second);
        return;
     }
    }

    /* Поиск события из карты тригонометрических операций */
    {auto it = trigonometryMapping.find(eventName);
     if (it!=trigonometryMapping.end()){
        addFunction(it->second);
        return;
     }
    }

}

void BackEnd::execute(unsigned char command) {
    switch (command) {
        case 0: clear(); break;
        case 1: performCalculation(Sender::Equals); break;
        case 2: addPoint(); break;
        case 3: chopInput(); break;
        default: qDebug() << "Passed command to execute() function was not recognized. Command number: " << command;
    }
}
