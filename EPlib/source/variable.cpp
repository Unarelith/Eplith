/*---------------------------------------------------------------------------------

    Eplith Intepreter
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
#include "variable.h"
#include "expression.h"

using namespace std;

vector< vector<Variable*> > Variable::vars;

Variable::Variable(string name, Value *value, bool tmp) {
	m_address = getPtrAddr((void*)this);
	m_name = name;
	m_value = value;
	m_scope = Expression::scopes;
	
	if(!tmp) {
		while(vars.size() < m_scope + 1) vars.push_back(vector<Variable*>());
		
		m_id = vars[m_scope].size();
		
		vars[m_scope].push_back(this);
		
#ifdef VAR_DEBUG
		edbg2("Var name: " << m_name << " | Type: " << m_value->type() << " | Value: ", m_value->print(), " | ID: " << m_id << " | Scope: " << m_scope);
#endif
	} else {
		m_id = -1;
	}
}

Variable::~Variable() {
	vars[m_scope].erase(vars[m_scope].begin() + m_id);
	
#ifdef DELVAR_DEBUG
	edbg("Variable \"" << m_name << "\" with ID: " << m_id << " deleted. | vars.size: " << vars.size());
#endif
	
	delete m_value;
}

void Variable::value(Value *value) {
	m_value = value;
	
#ifdef SETVAR_DEBUG
	edbg2("Variable \"" << m_name << "\" has a new value: ", m_value->print(), "");
#endif
}

Variable* Variable::findByName(std::string name) {
	for(int s = vars.size() - 1 ; s >= 0 ; s--) {
		for(int i = vars[s].size() - 1 ; i >= 0 ; i--) {
			if(vars[s][i]->name() == name) {
				return vars[s][i];
			}
		}
	}
	yyerror("Variable '" + name + "' not found");
}

bool Variable::exists(std::string name) {
	for(int s = vars.size() - 1 ; s >= 0 ; s--) {
		for(int i = 0 ; i < vars[s].size() ; i++) {
			if(vars[s][i]->name() == name) {
				return true;
			}
		}
	}
	return false;
}

bool Variable::erase(std::string name) {
	for(int s = vars.size() - 1 ; s >= 0 ; s--) {
		for(int i = 0 ; i < vars[s].size() ; i++) {
			if(vars[s][i]->name() == name) {
				vars[s].erase(vars[s].begin() + i);
				return true;
			}
		}
	}
	return false;
}

void Variable::initNullVar() {
	Value *nullValue = new Value();
	Variable *nullVar = new Variable("<null>", nullValue);
}
