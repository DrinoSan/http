//
// Created by Sandrino Becirbegovic on 17/12/2022.
//

#ifndef SIMPLEHTTPSERVER2_JSONPARSER_T_H
#define SIMPLEHTTPSERVER2_JSONPARSER_T_H

// System Headers
#include <string>

// Project Headers
#include "include/rapidjson/document.h"


static const char* kTypeNames[] =
        { "Null", "False", "True", "Object", "Array", "String", "Number" };

class JsonParser_t
{
public:
    JsonParser_t(std::string messageBody);
    ~JsonParser_t() = default;

    template<typename T>
    std::string getTypeOfElement(T element)
    {
        if( ! document.HasMember(element) )
        {
            return std::string("Element not Found!");
        }

        return kTypeNames[ document[ element ].GetType() ];
    }


private:
    std::string messageBody;
    rapidjson::Document document;
};


#endif //SIMPLEHTTPSERVER2_JSONPARSER_T_H
