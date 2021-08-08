#pragma once

#include <iostream>
#include <map>
#include <vector>

using namespace std;

enum IDFlag
{
	var_flag,
	function_flag,
	procedure_flag,
	const_val_flag,
	const_var_flag
};

enum varscope
{
	global_t,
	local_t
};

enum type
{
	string_type,
	array_type,
	void_type,
	int_type,
	real_type,
	bool_type
};

struct IDValue;
struct IDInfo;

struct IDValue
{
	int i = 0;
	double d = 0.0;
	bool b = false;
	string s = "";
	vector<IDInfo> args; // for args & arrays
};

struct IDInfo
{
	int idx = 0;
	string id_name = "";
	int type = int_type;
	int flag = var_flag;
	IDValue value;
	varscope scope = local_t;
	bool initialized = false;
};

class SymbolTable
{
public:
	SymbolTable();
	int insert(string id, IDInfo info);
	int insert(string id, int type, int flag, IDValue val, bool init);
	void dump();
	int size();
	IDInfo* lookup(string id);
	bool exists(string id);
	void setFunctionType(int t);
	void addFunctionArgument(string id, IDInfo info);
	int getIndex(string id);
private:
	int idx;
	vector<string> symbols;
	map<string, IDInfo> tablemap;
};

class SymbolTableList
{
public:
	SymbolTableList();
	int insert(string id, IDInfo info);
	int insert(string id, int type, int size);
	void push();
	bool pop();
	IDInfo* lookup(string id);
	void dump();
	void setFunctionType(int type);
	void addFunctionArgument(string id, IDInfo info);
	int getIndex(string id);
	bool isGlobal();
	int get_top_idx();
private:
	vector<SymbolTable> STList;
	int top_idx;
};

bool isConstant(IDInfo info);
IDInfo* boolConstant(bool val);
IDInfo* intConstant(int val);
IDInfo* strConstant(string *val);
IDInfo* floatConstant(double val);
int getValue(IDInfo info);