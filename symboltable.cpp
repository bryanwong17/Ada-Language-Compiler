#include "symboltable.hpp"

SymbolTable::SymbolTable()
{
	idx = 0;
}

int SymbolTable::insert(string id, IDInfo info)
{
	if(!exists(id))
	{
		symbols.push_back(id);
		tablemap[id] = info;
		tablemap[id].id_name = id;
		tablemap[id].idx = this->idx;
		return idx++;
	}

	cout << id << endl;
	return -1;
}

int SymbolTable::insert(string id, int type, int flag, IDValue val, bool init)
{
	if (!exists(id))
	{
		symbols.push_back(id);
		tablemap[id].initialized = init;
		tablemap[id].idx = this->idx;
		tablemap[id].type = type;
		tablemap[id].value = val;
		tablemap[id].id_name = id;
		tablemap[id].flag = flag;
		return idx++;
	}
	
	return -1;
}

void SymbolTable::dump()
{
	printf("%s\t%s\t%s\t%s\t%s\n", "<IDX>", "<ID>", "<FLAG>", "<TYPE>", "<VALUE>");
	string s;
	for (int i = 0; i < idx; i++)
	{
		IDInfo inf = tablemap[symbols[i]];
		s = to_string(inf.idx) + "\t" + inf.id_name + "\t";
		if(inf.flag == var_flag) s += "var\t";
		else if(inf.flag == function_flag) s += "funct\t";
		else if(inf.flag == procedure_flag) s += "proced\t";
		else if(inf.flag == const_var_flag) s += "const\t";
		else s += "unknown\t";

		if(inf.type == string_type)
		{
			s += "str\t";
			if (inf.initialized) s += inf.value.s;
		}
		else if(inf.type == array_type) s += "arr\t";
		else if(inf.type == void_type) s += "void\t";
		else if(inf.type == int_type)
		{
			s += "int\t";
			if (inf.initialized) s += to_string(inf.value.i);
		}
		else if(inf.type == real_type)
		{
			s += "float\t";
			if (inf.initialized) s += to_string(inf.value.d);
		}
		else if(inf.type == bool_type)
		{
			s += "bool\t";
			if (inf.initialized) s += (inf.value.b) ? "true" : "false";
		}
		else s += "unknown\t";

		if (inf.flag == function_flag || inf.flag == procedure_flag)
		{
			s += "{"; 

			for (int j = 0; j < inf.value.args.size(); j++)
			{
				if(inf.value.args[j].type == string_type) s += "str ";
				else if(inf.value.args[j].type == array_type) s += "arr ";
				else if(inf.value.args[j].type == void_type) s += "void ";
				else if(inf.value.args[j].type == int_type) s += "int ";
				else if(inf.value.args[j].type == real_type) s += "float ";
				else if(inf.value.args[j].type == bool_type) s += "bool ";
				else s += "unknown ";
			}

			s += "}";
		}

		if (inf.type == array_type)
		{
			s += "{"; 
			
			if(inf.value.args[0].type == int_type) 	s += "int, length ";
			else if(inf.value.args[0].type == real_type) s += "float, length ";
			else if(inf.value.args[0].type == bool_type) s += "bool, length ";
			else if(inf.value.args[0].type == string_type) s += "str, length ";
			else if(inf.value.args[0].type == array_type) s += "arr, length ";
			else if(inf.value.args[0].type == void_type) s += "void, length ";
			else s += "unknown, length ";
			
			s += to_string(inf.value.args.size()) + "}";
		}

		cout << s << endl;
	}

	cout << endl;
}

int SymbolTable::size()
{
  return symbols.size();
}

IDInfo* SymbolTable::lookup(string id)
{
	if (!exists(id))
	{
		return NULL;
	}
	return new IDInfo(tablemap[id]);
}

bool SymbolTable::exists(string id)
{
	return tablemap.find(id) != tablemap.end();
}

void SymbolTable::setFunctionType(int t)
{
	tablemap[symbols[idx - 1]].type = t;
}

void SymbolTable::addFunctionArgument(string id, IDInfo info)
{
	tablemap[symbols[idx - 1]].value.args.push_back(info);
}

int SymbolTable::getIndex(string id){
  if (exists(id)) return tablemap[id].idx;
  else return 0;
}

SymbolTableList::SymbolTableList()
{
	top_idx = -1;
	push();
}

int SymbolTableList::insert(string id, IDInfo info)
{
	return STList[top_idx].insert(id, info);
}

int SymbolTableList::insert(string id, int type, int size)
{
	IDValue val;
	val.args = vector<IDInfo>(size);

	for (int i = 0; i < size; i++)
	{
		val.args[i].idx = -1;
		val.args[i].flag = var_flag;
		val.args[i].type = type;
	}

	return STList[top_idx].insert(id, array_type, var_flag, val, false);
}

void SymbolTableList::push()
{
	STList.push_back(SymbolTable()); // add new element at the end of the vector
	++top_idx;
}

bool SymbolTableList::pop()
{
	if (STList.size() < 1) 
	{
		return false;
	}
	else
	{
		STList.pop_back(); // removes the last element in the vector
		--top_idx;
		return true;
	}
}

IDInfo* SymbolTableList::lookup(string id)
{
	for (int i = top_idx; i >= 0; i--)
	{
		if (STList[i].exists(id))
		{
			return STList[i].lookup(id);
		}
	}

	return NULL;
}

void SymbolTableList::dump()
{
	cout << "=======START DUMP=======\n\n";

	for (int i = top_idx; i >= 0; i--)
	{
		if (i==0) cout << "========GLOBAL SCOPE======= " << i << endl;
		else cout << "========LOCAL SCOPE======= " << i << endl;
		STList[i].dump();
	}

	cout << "=======END DUMP=======\n";
}

void SymbolTableList::setFunctionType(int type)
{
	STList[top_idx - 1].setFunctionType(type);
}

void SymbolTableList::addFunctionArgument(string id, IDInfo info)
{
	STList[top_idx - 1].addFunctionArgument(id, info);
}

int SymbolTableList::getIndex(string id)
{
  for (int i = top_idx; i >= 0; i--) {
    if (STList[i].exists(id)) {
      if (i == 0) return -1; //global
      else { //local
        int idx = 0;
        for (int j = 1; j < i; ++j) {
          idx += STList[j].size();
        }
        idx += STList[i].getIndex(id);
        return idx;
      }
    }
  }
  return -2; //not found
}

bool SymbolTableList::isGlobal()
{
  if (top_idx == 0) return true;
  else return false;
}

int SymbolTableList::get_top_idx()
{
	return top_idx;
}
bool isConstant(IDInfo info)
{
	return info.flag == const_var_flag || info.flag == const_val_flag;
};

IDInfo* boolConstant(bool val)
{
	IDInfo* info = new IDInfo();
	info->value.b = val;
	info->flag = const_val_flag;
	info->idx = 0;
	info->type = bool_type;
	return info;
};

IDInfo* intConstant(int val)
{
	IDInfo* info = new IDInfo();
	info->value.i = val;
	info->flag = const_val_flag;
	info->idx = 0;
	info->type = int_type;
	return info;
};

IDInfo* strConstant(string *val)
{
	IDInfo* info = new IDInfo();
	info->value.s = *val;
	info->flag = const_val_flag;
	info->idx = 0;
	info->type = string_type;
	return info;
};

IDInfo* floatConstant(double val)
{
	IDInfo* info = new IDInfo();
	info->value.d = val;
	info->flag = const_val_flag;
	info->idx = 0;
	info->type = real_type;
	return info;
};

int getValue(IDInfo info)
{
  if(info.type == bool_type){
    return info.value.b;
  }
  return info.value.i;
}