#ifndef BACKEND_H
#define BACKEND_H
#include <QObject>

#include <const.h>

class BackEnd : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString expression READ expression WRITE setExpression NOTIFY expressionChanged)
    Q_PROPERTY(QString input READ input WRITE setInput NOTIFY inputChanged)
    Q_PROPERTY(QString result READ result WRITE setResult NOTIFY resultChanged)
    Q_PROPERTY(QString fakeExpression READ fakeExpression WRITE setFakeExpression NOTIFY fakeExpressionChanged)

public:
    explicit BackEnd(QObject *parent = nullptr);

    /* Expression block START */
    QString expression() const;
    void setExpression(QString expression);
    void clearExpression();
    /* Expression block  END */

    /* Expression block START */
    QString fakeExpression() const;
    void setFakeExpression(QString expression);
    void clearFakeExpression();
    /* Expression block  END */

    /* Input block START */
    QString input() const;
    void setInput(QString input);
    void clearInput();
    void resetInput();
    /* Input block END */

    /* Result block START */
    QString result() const;
    void setResult(QString result);
    void clearResult();
    /* Result block END */


    Q_INVOKABLE void submitEvent(QString eventName);

    void addToExpression(QString symbol);
    void addToInput(QString symbol);
    void addPoint();
    void addOperator(QString operatorName);
    void addFunction(QString function);

    void clear();

    bool lastInExpressionIsOperator = false;
    bool nowShowingResult = false;
    QString lastOperator;
    unsigned char lastOperatorLength = 0;

    void calculate();
    void performCalculation(Sender sender);
    void execute(unsigned char command);

    void addSymbol(QString symbol);
    void addBracket(QString bracket);
    void chopInput();

signals:
    void inputChanged(QString);
    void expressionChanged(QString);
    void resultChanged(QString);
    void fakeExpressionChanged(QString);

private:
   QString _expression      =  "";
   QString _fakeExpression  =  "";
   QString _result          =  "";
   QString _input           = "0";
};

#endif // BACKEND_H
