//
// Created by DELL on 2019/4/20.
//

#ifndef SPLCOMPILER_SPL_EXCEPTION_H
#define SPLCOMPILER_SPL_EXCEPTION_H

#include <exception>
#include <iostream>
class splException : public std::exception {
public:
    int line;
    int offset;
    std::string errorMessage;
    splException(int line_, int offset_, std::string errorMessage_)
    :line(line_), offset(offset_), errorMessage(errorMessage_){}
};


#endif //SPLCOMPILER_SPL_EXCEPTION_H
