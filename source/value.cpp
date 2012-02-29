/*---------------------------------------------------------------------------------

    WordSphere Intepreter
    Copyright (C) 2012 Quent42340 <quent42340@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

---------------------------------------------------------------------------------*/
#include "header.h"
#include "expression.h"
#include "function.h"
#include "parser.tab.hpp"

using namespace std;

Value::Value() {
	m_type = typeVoid;
	m_value = string("(null)");
}

Value::Value(int value) {
	m_type = typeInt;
	m_value = value;
}

Value::Value(string value) {
	m_type = typeStr;
	m_value = value;
}

Value::Value(char *value) {
	m_type = typeStr;
	m_value = string(value);
}

Value::Value(bool value) {
	m_type = typeInt;
	m_value = value;
}

Value::Value(Type type, boost::any value) {
	m_type = type;
	m_value = value;
}

Value::Value(boost::any *value) {
	m_value = *value;
	
	if(int *pi = boost::any_cast<int>(&m_value)) {
		m_type = typeInt;
	}
	else if(string *pstr = boost::any_cast<string>(&m_value)) {
		m_type = typeStr;
	}
	else if(bool *pb = boost::any_cast<bool>(&m_value)) {
		m_type = typeInt;
	} else {
		m_type = typeVoid;
	}
}

Value::Value(Variable *var) {
	m_value = var->value()->m_value;
	
	if(int *pi = boost::any_cast<int>(&m_value)) {
		m_type = typeInt;
	}
	else if(string *pstr = boost::any_cast<string>(&m_value)) {
		m_type = typeStr;
	}
	else if(bool *pb = boost::any_cast<bool>(&m_value)) {
		m_type = typeInt;
	} else {
		m_type = typeVoid;
	}
}

void Value::print() {
	if(int *pi = boost::any_cast<int>(&m_value)) {
		cout << *pi;
	}
	else if(string *pstr = boost::any_cast<string>(&m_value)) {
		cout << *pstr;
	}
	else if(bool *pb = boost::any_cast<bool>(&m_value)) {
		cout << ((*pb) ? "true" : "false");
	}
	else if(m_type == typeVoid) {
		cout << "(null)";
	}
}

