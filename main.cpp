#include <stdlib.h>

#include "parser.h"

using namespace std;
using namespace parserLib;

short priorityTable[4][2] = {
    {'+', 1},
    {'-', 1},
    {'*', 2},
    {'/', 2},
};

short getPriority(stToken token) {
    char c = *((char*)token.value);

    for (int i = 0; i < 4; i++) {
        if (priorityTable[i][0] == c) {
            return priorityTable[i][1];
        }
    }
    return 0;
}

void printToken(stToken token) {
    switch (token.type) {
        case INTEGER:
            cout << "Integer Value: " << *(int*)token.value << endl;
            break;
        case FLOAT:
            cout << "Float Value: " << *((float*)token.value) << endl;
            break;
    }
}

void printError(string txt = "Unvalid expression !!!") {
    cout << txt << endl;
    exit(EXIT_FAILURE);
}

bool isOpeningParenthesis(stToken token) {
    if (token.type != PUNCTUATOR) return false;

    char c = *((char*)token.value);

    return c == '(';
}

bool isClosingParenthesis(stToken token) {
    if (token.type != PUNCTUATOR) return false;

    char c = *((char*)token.value);

    return c == ')';
}

stToken doOperation(stToken oper, stToken val1, stToken val2) {
    stToken result;

    float val1_ = *((int*)val1.value);
    float val2_ = *((int*)val2.value);

    char oper_ = *((char*)oper.value);

    float result_;

    switch (oper_) {
        case '+':
            result_ = val1_ + val2_;
            break;
        case '-':
            result_ = val1_ - val2_;
            break;
        case '*':
            result_ = val1_ * val2_;
            break;
        case '/':
            if (!val2_) {
                printError();
            }
            result_ = val1_ / val2_;
            break;
    }

    if (val1.type == FLOAT || val2.type == FLOAT) {
        result.type = FLOAT;
        result.value = malloc(sizeof(float));
        *((float*)result.value) = result_;
    } else {
        result.type = INTEGER;
        result.value = malloc(sizeof(int));
        *((int*)result.value) = result_;
    }

    return result;
}

stToken evalExp(vector<stToken>& tokens) {
    vector<stToken> values;
    vector<stToken> operations;

    stToken current, oper, val1, val2, result;

    for (int i = 0; i < tokens.size(); i++) {
        current = tokens[i];

        if (current.type == INTEGER || current.type == FLOAT) {
            values.push_back(current);
        } else if (isOpeningParenthesis(current)) {
            operations.push_back(current);
        } else if (isClosingParenthesis(current)) {
            // if there are no operations left then there is no open parenthesis
            // and the exp is invalid
            if (operations.empty()) {
                printError("no opening parenthesis for the closing pare");
            }

            if (isOpeningParenthesis(operations.back())) {
                printError("empty expression !!");
            }

            while (!isOpeningParenthesis(operations.back())) {
                oper = operations.back();
                operations.pop_back();

                if (operations.empty()) {
                    printError();
                }

                if (values.size() < 2) {
                    printError();
                }

                val1 = values.back();
                values.pop_back();

                val2 = values.back();
                values.pop_back();

                result = doOperation(oper, val1, val2);

                values.push_back(result);
            }

            operations.pop_back();
        } else if (current.type == OPERATOR) {
            while (!operations.empty()) {
                oper = operations.back();

                if (getPriority(oper) < getPriority(current)) {
                    break;
                }

                operations.pop_back();

                val1 = values.back();
                values.pop_back();

                val2 = values.back();
                values.pop_back();

                result = doOperation(oper, val1, val2);

                values.push_back(result);
            }

            operations.push_back(current);
        }
    }

    while (!operations.empty()) {
        oper = operations.back();

        if (oper.type != OPERATOR) {
            printError();
        }

        operations.pop_back();

        val1 = values.back();
        values.pop_back();

        val2 = values.back();
        values.pop_back();

        result = doOperation(oper, val1, val2);

        values.push_back(result);
    }

    if (values.size() != 1) {
        printError();
    }

    result = values.back();

    return result;
}

void evalFile(string fileName) {
    fstream fileHolder;
    vector<stToken> tokensV;

    fileHolder.open(fileName, ios::in);

    if (fileHolder.is_open()) {
        string line;

        while (getline(fileHolder, line)) {
            tokensV.clear();
            parseLine(line, tokensV);

            if (line != "") printToken(evalExp(tokensV));
        }

        fileHolder.close();
    }
}

int main() { evalFile("exp.txt"); }