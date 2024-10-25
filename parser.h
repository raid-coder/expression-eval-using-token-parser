#pragma once

#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

const char operators[] = {'+', '-', '*', '/', '=', '%',
                          '>', '<', '^', '&', '|'};

const char punctuators[] = {'(', ')', '[', ']', ',', ';', '{', '}'};

enum enTokenType {
    IDENTIFIER,
    STRING,
    INTEGER,
    FLOAT,
    OPERATOR,
    PUNCTUATOR,
    KEYWORD
};

namespace parserLib {

struct stToken {
    enTokenType type;
    void* value;
};

stToken newToken(enTokenType type, string value) {
    stToken token;
    token.type = type;

    switch (type) {
        case IDENTIFIER:
        case KEYWORD:
        case STRING:
            token.value = (void*)new string;
            *(string*)token.value = value;
            break;
        case INTEGER:
            token.value = (void*)new int;
            *(int*)token.value = stoi(value);
            break;
        case FLOAT:
            token.value = (void*)new float;
            *(float*)token.value = stod(value);
            break;
        case OPERATOR:
        case PUNCTUATOR:
            token.value = (void*)new char;
            *(char*)token.value = value[0];
            break;
    }

    return token;
}

bool isKeyWord(string& word) {
    switch (word[0]) {
        case 'a':
            return (word == "asm" || word == "auto");
        case 'b':
            return (word == "break");
        case 'c':
            return (word == "case" || word == "catch" || word == "char" ||
                    word == "class" || word == "const" || word == "continue");
        case 'd':
            return (word == "default" || word == "delete" || word == "do" ||
                    word == "double");
        case 'e':
            return (word == "else" || word == "enum" || word == "extern");
        case 'f':
            return (word == "float" || word == "for" || word == "friend");
        case 'g':
            return (word == "goto");
        case 'i':
            return (word == "if" || word == "inline" || word == "int");
        case 'l':
            return (word == "long");
        case 'n':
            return (word == "new");
        case 'o':
            return (word == "operator");
        case 'p':
            return (word == "private" || word == "protected" ||
                    word == "public");
        case 'r':
            return (word == "register" || word == "return");
        case 's':
            return (word == "short" || word == "signed" || word == "sizeof" ||
                    word == "static" || word == "struct" || word == "switch");
        case 't':
            return (word == "template" || word == "this" || word == "throw" ||
                    word == "try" || word == "typedef");
        case 'u':
            return (word == "union" || word == "unsigned");
        case 'v':
            return (word == "virtual" || word == "void" || word == "volatile");
        default:
            return (word == "while");
    }
}

bool isOperator(char c) {
    for (char c2 : operators) {
        if (c == c2) return true;
    }
    return false;
}

bool isPunctuator(char c) {
    for (char c2 : punctuators) {
        if (c == c2) return true;
    }
    return false;
}

void parseLine(string str, vector<stToken>& tokensV) {
    string word;
    int j, length = str.length();
    int index = 0;
    while (index < length) {
        if (str[index] == ' ') {
            index++;
            continue;
        }

        // integer or float
        if (isdigit(str[index])) {
            j = index++;
            while (isdigit(str[index]) && index < length) {
                index++;
            }
            if (str[index] == '.') {
                index++;
                while (isdigit(str[index]) && index < length) {
                    index++;
                }
                word = str.substr(j, index - j);
                tokensV.push_back(newToken(FLOAT, word));
            } else {
                word = str.substr(j, index - j);
                tokensV.push_back(newToken(INTEGER, word));
            }
            continue;
        }

        // identifier or keyword
        if (islower(str[index]) || isupper(str[index]) || str[index] == '_') {
            j = index++;
            while ((islower(str[index]) || isupper(str[index]) ||
                    isdigit(str[index]) || str[index] == '_') &&
                   index < length) {
                index++;
            }
            word = str.substr(j, index - j);
            if (isKeyWord(word)) {
                tokensV.push_back(newToken(KEYWORD, word));
            } else {
                tokensV.push_back(newToken(IDENTIFIER, word));
            }
            continue;
        }

        // operators
        if (isOperator(str[index])) {
            word = str[index++];
            tokensV.push_back(newToken(OPERATOR, word));
            continue;
        }

        // punctuators
        if (isPunctuator(str[index])) {
            word = str[index++];
            tokensV.push_back(newToken(PUNCTUATOR, word));
            continue;
        }

        // string values
        if (str[index] == '\"') {
            j = ++index;
            while (str[index] != '\"') {
                index++;
            }
            word = str.substr(j, index - j);
            tokensV.push_back(newToken(STRING, word));
            index++;  // skip the second "
            continue;
        }

        index++;
    }
}
}  // namespace parserLib