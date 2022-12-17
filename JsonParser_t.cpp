//
// Created by Sandrino Becirbegovic on 17/12/2022.
//

#include "JsonParser_t.h"

JsonParser_t::JsonParser_t(std::string messageBody) : messageBody{messageBody}
{
    document.Parse(messageBody.c_str());
}
