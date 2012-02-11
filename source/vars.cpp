/*---------------------------------------------------------------------------------

    BrainFuck Advanced Intepreter
    Copyright (C) 2012 Quent42340 quent42340@gmail.com

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
#include "globalVars.h"
#include "errors.h"
#include "vars.h"

using namespace std;

vector<uInt> vars;
vector<String> strings;

Attr attrNULL = {{-1, -1}, false, false, -1};
DataRange drNULL = {attrNULL, -1, NULL};
uInt uIntNULL = {NULL, drNULL};
String StringNULL = {NULL, drNULL};

char* catchVarName() {
	string name;
	
	if(sourceCode[i + 1] == '(') {
		i++;
		unsigned int temp = i + 1;
		
		for(i++ ; sourceCode[i] != ')' ; i++) {
			name[i - temp] = sourceCode[i];
		}
		name[i - temp] = '\0';
		
		i++;
		return (char*)name.c_str();
	} else {
		return NULL;
	}
}

bool varExists(uInt var) {
	for(unsigned int j = 0 ; j < vars.size() ; j++) {
		if(strcmp(vars[j].name, var.name) == 0) {
			return true;
		}
	}
	
	return false;
}

void catchOperationResult(uInt &tmp) {
	uInt &var1 = findVar(catchVarName());
	if(var1.name != NULL) {
		while(sourceCode[i] == ' ') i++;
		if(sourceCode[i] == ';') {
			setDataRangeValue(tmp.dr, var1.dr.value);
		} else {
			if((sourceCode[i] == '+') || (sourceCode[i] == '-') || (sourceCode[i] == '*') || (sourceCode[i] == '/') || (sourceCode[i] == '%') || (sourceCode[i] == '^')) {
				char op = sourceCode[i];
				i++;
				while(sourceCode[i] == ' ') i++;
				if(sourceCode[i] == '@') {
					uInt &var2 = findVar(catchVarName());
					if(var2.name != NULL) {
						while(sourceCode[i] == ' ') i++;
						if(sourceCode[i] == ';') {
							switch(op) {
								case '+':
									setDataRangeValue(tmp.dr, var1.dr.value + var2.dr.value);
									break;
								
								case '-':
									setDataRangeValue(tmp.dr, var1.dr.value - var2.dr.value);
									break;
								
								case '*':
									setDataRangeValue(tmp.dr, var1.dr.value * var2.dr.value);
									break;
								
								case '/':
									setDataRangeValue(tmp.dr, var1.dr.value / var2.dr.value);
									break;
								
								case '%':
									setDataRangeValue(tmp.dr, var1.dr.value % var2.dr.value);
									break;
								
								case '^':
									setDataRangeValue(tmp.dr, pow(var1.dr.value, var2.dr.value));
									break;
								
								default:
									error(2, "Unexpected error.");
									break;
							}
						} else {
							error(1, "Expecting a ';' here.");
						}
					} else {
						error(2, "This variable doesn't exist");
					}
				}
			} else {
				error(1, "Expecting a ';' here.");
			}
		}
	} else {
		error(2, "This variable doesn't exist.");
	}
}

uInt catchVar() {
	uInt temp = {NULL};
	temp.name = catchVarName();
	
	uInt &tmp = findVar(temp.name);
	
	whilenotspace();
	if(sourceCode[i] == '=') {
		i++;
		while(sourceCode[i] == ' ') i++;
		if(!varExists(temp)) {
			if(sourceCode[i] == '&') {
				temp.dr = catchDataRange(true);
				while(sourceCode[i] == ' ') i++;
				if(sourceCode[i] == '=') {
					i++;
					while(sourceCode[i] == ' ') i++;
					if(sourceCode[i] == '@') {
						catchOperationResult(temp);
						vars.push_back(temp);
						return temp;
					} else {
						int tmpValue = catchValue(true);
						if(tmpValue != -1) {
							setDataRangeValue(temp.dr, tmpValue);
							vars.push_back(temp);
							return temp;
						} else {
							error(1, "Expecting a value or a variable.");
						}
					}
				} else {
					if(sourceCode[i] == ';') {
						vars.push_back(temp);
						return temp;
					} else {
						error(1, "Expecting a ';' here.");
					}
				}
			} else {
				error(1, "A data range is expected here.");
			}
		} else {
			if(sourceCode[i] == '&') {
				string warn = "Variable @(";
					   warn += tmp.name;
					   warn += ") already assigned.";
				warning(warn);
				tmp.dr = catchDataRange(true);
				tmp.dr.value = getDataRangeValue(tmp.dr);
				while(sourceCode[i] == ' ') i++;
				if(sourceCode[i] == ';') {
					return tmp;
				} else {
					if(sourceCode[i] == '=') {
						i++;
						whilenotspace();
						if(sourceCode[i] == '@') {
							catchOperationResult(tmp);
							return tmp;
						} else {
							error(1, "Expecting a variable here.");
						}
					} else {
						error(1, "Expecting a ';' here.");
					}
				}
			}
			else if(sourceCode[i] == '@') {
				catchOperationResult(tmp);
				return tmp;
			} else {
				setDataRangeValue(tmp.dr, catchValue(true));
				while(sourceCode[i] == ' ') i++;
				if(sourceCode[i] == ';') {
					return tmp;
				} else {
					error(1, "Expecting a ';' here.");
				}
			}
		}
	} else {
		error(1, "Needs an '=' operator here.");
	}
	
	return temp;
}

uInt& findVar(char* name) {
	for(unsigned int j = 0 ; j < vars.size() ; j++) {
		if(strcmp(vars[j].name, name) == 0) {
			return vars[j];
		}
	}
	
	return uIntNULL;
}

bool stringExists(String str) {
	for(unsigned int j = 0 ; j < strings.size() ; j++) {
		if(strcmp(strings[j].name, str.name) == 0) {
			return true;
		}
	}
	
	return false;
}

String catchString() {
	String temp = {NULL};
	temp.name = catchVarName();
	
	String &tmp = findString(temp.name);
	
	while(sourceCode[i] == ' ') i++;
	if(sourceCode[i] == '=') {
		i++;
		while(sourceCode[i] == ' ') i++;
		if(!stringExists(temp)) {
			if(sourceCode[i] == '&') {
				temp.dr = catchDataRange(true);
				while(sourceCode[i] == ' ') i++;
				if(sourceCode[i] == '=') {
					i++;
					setDataRangeStringValue(temp.dr, catchStringValue(true), true);
				}
				while(sourceCode[i] == ' ') i++;
				if(sourceCode[i] == ';') {
					strings.push_back(temp);
					return temp;
				} else {
					error(1, "Expecting a ';' here.");
				}
			} else {
				error(1, "A data range is expected here.");
			}
		} else {
			if(sourceCode[i] == '&') {
				warning("Variable already assigned.");
				tmp.dr = catchDataRange(true);
				tmp.dr.str = getDataRangeStringValue(tmp.dr);
				if(sourceCode[i] == ';') {
					return tmp;
				} else {
					error(1, "Expecting a ';' here.");
				}
			} else {
				setDataRangeStringValue(tmp.dr, catchStringValue(true));
				while(sourceCode[i] == ' ') i++;
				if(sourceCode[i] == ';') {
					return tmp;
				} else {
					error(1, "Expecting a ';' here.");
				}	
			}
		}
	} else {
		error(1, "Needs an '=' operator here.");
	}
	
	return temp;
}

String& findString(char* name) {
	for(unsigned int j = 0 ; j < strings.size() ; j++) {
		if(strcmp(strings[j].name, name) == 0) {
			return strings[j];
		}
	}
	
	return StringNULL;
}

