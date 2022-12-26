//
// Created by Sandrino Becirbegovic on 17/12/2022.
//

#ifndef SIMPLEHTTPSERVER2_JSONPARSER_T_H
#define SIMPLEHTTPSERVER2_JSONPARSER_T_H

// System Headers
#include <string>
#include <iostream>
#include <vector>

// Project Headers
#include "include/rapidjson/document.h"

static const char* kTypeNames[] =
		{ "Null", "False", "True", "Object", "Array", "String", "Number" };

class JsonParser_t
{
public:
	JsonParser_t(std::string messageBody);

	~JsonParser_t() = default;

	// Stringify whole json object
	std::string dumpJson();

	rapidjson::Value& get(std::string element);

	//----------------------------------------------------------------------------
	// std::move is not needed because compiler will I think implicitly move the return
	template<typename T>
	T getVal(const char* element)
	{
		if constexpr (std::is_same<T, const char*>::value)
		{
			return document[element].GetString();
		}
		else if constexpr (std::is_same<T, int>::value)
		{
			return document[element].GetInt();
		}
		else if constexpr (std::is_same<T, bool>::value)
		{
			return document[element].GetBool();
		}
		else if constexpr (std::is_same<T, std::vector<int>>::value)
		{
			auto a = document[element].GetArray();
			std::vector<int> tmp;
			for (rapidjson::Value::ConstValueIterator itr = a.Begin(); itr != a.End(); ++itr)
			{
				tmp.emplace_back(itr->GetInt());
			}

			return tmp;
		}
	}

	//----------------------------------------------------------------------------
	template<typename U>
	void extractValue(const char* source, U& target)
	{
		if constexpr (std::is_same<U, std::string>::value)
		{
			target = getVal<const char*>(source);
		}
		else if constexpr (std::is_same<U, int>::value)
		{
			target = getVal<int>(source);
		}
		else if constexpr (std::is_same<U, bool>::value)
		{
			target = getVal<bool>(source);
		}
		else if constexpr (std::is_same<U, std::vector<int>>::value)
		{
			target = getVal<std::vector<int>>(source);
		}
	}

	// Get value type of element {"element": "value"}
	//----------------------------------------------------------------------------
	template<typename T>
	std::string getTypeOfElement(T element)
	{
		if (!document.HasMember(element))
		{
			std::cout << "Element not Found" << std::endl;
			return std::string();
		}

		return kTypeNames[document[element].GetType()];
	}

private:
	std::string messageBody;
	rapidjson::Document document;
};

#endif //SIMPLEHTTPSERVER2_JSONPARSER_T_H
