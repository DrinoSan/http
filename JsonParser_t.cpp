//
// Created by Sandrino Becirbegovic on 17/12/2022.
//

// Project Headers
#include "include/rapidjson/writer.h"
#include "include/rapidjson/stringbuffer.h"

#include "JsonParser_t.h"

JsonParser_t::JsonParser_t(std::string messageBody) : messageBody{messageBody}
{
    document.Parse(messageBody.c_str());
}

std::string JsonParser_t::dumpJson()
{
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);

    // Output {"project":"rapidjson","stars":11}
    return buffer.GetString();
}

rapidjson::Value &JsonParser_t::get(std::string element)
{
    if (!document.HasMember(element.c_str()))
    {
        std::cout << "Element not Found" << std::endl;
    }

    return document[element.c_str()];
}
