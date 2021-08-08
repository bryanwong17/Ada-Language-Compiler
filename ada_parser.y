%{

#include <iostream>
#include <fstream>
#include <stack>
#include "symboltable.hpp"
#include "lex.yy.cpp"

#define Trace(t) if (DebugPrint) printf("======TRACE====== %s\n", t)
#define YYDEBUG_LEXER_TEXT yytext

int DebugPrint = 1; //trace
int DebugDump = 0;
int Opt_D = 1; //symbol table

void yyerror(string s);
SymbolTableList syms;
vector<vector<IDInfo>> fps;
extern char *yytext;

string filename;
ofstream out;

struct Label {
	Label(int n) {
		count = n;
  		flag = -1;
	}
	int count;
	int flag;
};

class LabelAdministrator {
	public:
		LabelAdministrator() {
			count = 0;
		}
    	stack<Label> stacks;
    	void pushSeveralLabels(int n) {
			stacks.push(Label(count));
  			count += n;
		}
    	void severalLabels(int n) {
			stacks.top().count += n;
  			count += n;
		}
    	void removeLabel() {
			stacks.pop();
		}
    	int takeLabel(int n) {
			return stacks.top().count + n;
		}
    	int getLable() {
			return count++;
		}
    	void addFlag() {
			stacks.top().flag = stacks.top().flag + 1;
		}
    	int getFlag() {
			return stacks.top().flag;
		}
	private:
		int count;
};

LabelAdministrator la;

void constant(IDInfo inf)
{
	string temp = "\t\t";

	if(inf.type == int_type) {
		temp += "sipush " + to_string(inf.value.i);
	}
	else if(inf.type == string_type) {
		temp += "ldc \"" + inf.value.s + "\"";
	}
	else if(inf.type == bool_type) {
		if (inf.value.b) temp += "iconst_1";
		else temp += "iconst_0";
	}

	out << temp << endl;
}

void load_static_global_variable(IDInfo inf)
{
	if (inf.flag == const_var_flag) {
		constant(inf);
		return;
	}
	if (inf.scope == local_t) {
		out << "\t\tiload " << to_string(inf.idx) << endl;
		return;
	}

	string temp = "\t\tgetstatic ";

	if(inf.type == int_type) temp += "int ";
	else if(inf.type == real_type) temp += "float ";
	else if(inf.type == bool_type) temp += "bool ";
	else if(inf.type == string_type) temp += "string ";
	else if(inf.type == array_type) temp += "arr ";
	else if(inf.type == void_type) temp += "void ";
	else if(inf.type == void_type) temp += "unk ";
	
	temp += filename + "." + inf.id_name;
	out << temp << endl;
}

void global_declaration(IDInfo inf)
{
	string temp = "\tfield static ";

	if(inf.type == int_type) {
		temp += "int ";
		temp += inf.id_name;
		if (inf.initialized) temp += " = " + to_string(inf.value.i);
	}
	else if(inf.type == bool_type) {
		temp += "bool "; 
		temp += inf.id_name;
		if (inf.initialized) {
			temp += " = ";
			if (inf.value.b) temp += "true";
			else temp += "false";
		}
	}

	out << temp << endl;
}

void local_declaration(IDInfo inf)
{
	string temp = "\t\tsipush ";

	if(inf.type == int_type) {
		temp += to_string(inf.value.i);
	}
	else if(inf.type == bool_type) {
		if (inf.value.b) temp += "true";
		else temp += "false";
	}

	temp += "\t\tistore " + to_string(inf.idx) + "\n";

	out << temp;
}

struct LabelSet
{
	int start = 0;
	int count = 0;
	bool is_loop_label = false;
};

class LS
{
public:
	stack<LabelSet> ls;
	int lc;
	LS() {
		lc = 0;
	}
	void newLabelSet(int n, bool is_loop) {
		LabelSet l;
		l.start = lc;
		l.count = n;
		l.is_loop_label = is_loop;
		lc += n;
		this->ls.push(l);
	}
	int getLabel(int l) {
		return ls.top().start + l;
	}
	void popLabelSet() {
		this->ls.pop();
	}
};

LS ls;

void store(IDInfo inf)
{
	if (inf.scope == local_t) {
		out << "\t\tistore " << to_string(inf.idx) << endl;
		return;
	}

	string temp = "\t\tputstatic ";

	if(inf.type == int_type) temp += "int ";
	else if(inf.type == real_type) temp += "float ";
	else if(inf.type == bool_type) temp += "bool ";
	else if(inf.type == string_type) temp += "string ";
	else if(inf.type == array_type) temp += "arr ";
	else if(inf.type == void_type) temp += "void ";
	else if(inf.type == void_type) temp += "unk ";

	temp += filename + "." + inf.id_name;
	out << temp << endl;
}

void beginning_for(IDInfo inf, IDInfo toplim, IDInfo botlim)
{
	ls.newLabelSet(4, true);

	string l1 = "L" + to_string(ls.getLabel(0)); // Lbegin
	string l2 = "L" + to_string(ls.getLabel(1)); // Lexit
	string l3 = "L" + to_string(ls.getLabel(2)); // Ltrue
	string l4 = "L" + to_string(ls.getLabel(3)); // Lfalse

	constant(toplim);
	store(inf);

	out << l1 << ":\n"; //Lbegin
	load_static_global_variable(inf);
	constant(botlim);

	out << "\t\tisub\n";
	out << "\t\tifle " << l3 << endl; //Ltrue
	out << "\t\ticonst_0\n";
	out << "\t\tgoto " << l4 << endl; //Lfalse

	out << l3 << ":\n"; //Ltrue
	out << "\t\ticonst_1\n";

	out << l4 << ":\n"; //Lfalse
	out << "\t\tifeq " << l2 << endl; //Lexit

}

void end_for(IDInfo inf)
{
	string l1 = "L" + to_string(ls.getLabel(0)); // Lbegin
	string l2 = "L" + to_string(ls.getLabel(1)); // Lexit
	string l3 = "L" + to_string(ls.getLabel(2)); // Ltrue
	string l4 = "L" + to_string(ls.getLabel(3)); // Lfalse

	load_static_global_variable(inf);
	out << "\t\tsipush 1\n";
	out << "\t\tiadd\n";
	store(inf);
	out << "\t\tgoto " << l1 << endl; //Lbegin

	out << l2 << ":\n"; //Lexit

	ls.popLabelSet(); 
}

%}

/* yylval */
%union {
	int i_val;
	string *s_val;
	bool b_val;
	double d_val;
	IDInfo* info;
	int type;
}

/*tokens*/
%token BEGIN_TOKEN BOOLEAN BREAK CHARACTER CASE CONTINUE CONSTANT DECLARE DO ELSE END EXIT FLOAT FOR IF IN INTEGER LOOP PRINT PRINTLN PROCEDURE PROGRAM RANGE READ RETURN STRING THEN WHILE

%token <b_val> BOOLConst
%token <d_val> FLOATConst
%token <s_val> ID
%token <i_val> INTConst
%token <s_val> STRConst

%type <info> const_val expr func_invoc
%type <type> var_type

%right		ASSIGN
%left		OR
%left		AND
%left		NOT
%left		'<' LE '=' GE '>' NE
%left		'+' '-'
%left		'*' '/'
%nonassoc 	Unary_minus

%%
// program unit
program: PROGRAM ID {
						filename = *$2;
						out << "class " << filename << endl << "{" << endl;
					} 
					zero_one_vc_dec zero_more_fp_dec BEGIN_TOKEN {
						out << "\tmethod public static void main(java.lang.String[])" << endl;
  						out << "\tmax_stack 15" << endl;
  						out << "\tmax_locals 15" << endl << "\t{" << endl;;
					} 
					zero_more_stat {
						out << "\t\treturn" << endl << "\t}" << endl;;
					} END ';' END ID {
						Trace("Program Unit");
						if(Opt_D) syms.dump();
						syms.pop();
						out << "}" << endl;
					}
					;

// zero or more variable or constant declaration
zero_one_vc_dec: DECLARE one_more_vc | /* zero */
				;

// one or more variable or constant declaration
one_more_vc: var_const_dec one_more_vc | var_const_dec
				;

// variable constant declaration
var_const_dec: const_dec | var_dec | arr_dec

// constant declaration
const_dec: ID ':' CONSTANT ASSIGN const_val  ';' {
						Trace("Constant Declaration");
						if (!isConstant(*$5)) yyerror("Expression is not a constant\n");
						$5->flag = const_var_flag;
						$5->initialized = true;

						if (syms.get_top_idx() != 0) {							
							$5->scope = local_t;
						}

						if (syms.insert(*$1, *$5) == -1) yyerror("(Constant) variable redefinition");
					}
// constant declaration with type
					| ID ':' CONSTANT ':' var_type ASSIGN const_val ';' {
						Trace("Constant Declaration with Type");
						if (!isConstant(*$7)) yyerror("Expression is not a constant\n");
						if ($5 != $7->type) yyerror("Types do not match\n");
						$7->flag = const_var_flag;
						$7->initialized = true;

						if (syms.get_top_idx() != 0) {
							$7->scope = global_t;
						}

						if (syms.insert(*$1, *$7) == -1) yyerror("(Constant) variable redefinition");
					}
					;

// variable declaration
var_dec: ID ';' 	{
						Trace("Variable Declaration");
						IDInfo* info = new IDInfo();
						info->flag = var_flag;
						info->type = int_type;
						info->initialized = false;
						info->id_name = *$1;

						if (syms.get_top_idx() == 0) {
							info->scope = global_t;
							global_declaration(*info);
						}
						else {
							info->scope = local_t;
						}	

						if (syms.insert(*$1, *info) == -1) yyerror("Variable redefinition\n");
					}
					// variable declaration with type
					| ID ':' var_type ';' {
						Trace("Variable Declaration with Type");
						IDInfo* info = new IDInfo();
						info->flag = var_flag;
						info->type = $3;
						info->initialized = false;
						info->id_name = *$1;

						if (syms.get_top_idx() == 0) {
							info->scope = global_t;
							global_declaration(*info);
						} else {
							info->scope = local_t;
						}	

						if (syms.insert(*$1, *info) == -1) yyerror("Variable redefinition\n");
					}
					// variable declaration with expression
					| ID ASSIGN expr ';' {
						Trace("Variable Declaration with Expression");
						if (!isConstant(*$3)) yyerror("Expression is not a constant value\n");
						$3->flag = var_flag;
						$3->initialized = true;
						$3->id_name = *$1;

						if (syms.get_top_idx() == 0) {
							$3->scope = global_t;
							global_declaration(*$3);
						} else
						{
							$3->scope = local_t;
						}

						if (syms.insert(*$1, *$3) == -1) yyerror("Variable redefinition\n");
					}
					// variable declaration with type and expression
					| ID ':' var_type ASSIGN expr ';'{
						Trace("Variable Declaration with Type and Expression");
						if (!isConstant(*$5)) yyerror("Expression is not a constant value\n");
						if ($3 != $5->type) yyerror("Types do not match\n");
						$5->flag = var_flag;
						$5->initialized = true;
						$5->id_name = *$1;

						if (syms.get_top_idx() == 0) {
							$5->scope = global_t;
							global_declaration(*$5);
						} else {
							$5->scope = local_t;
							local_declaration(*$5);
						}

						if (syms.insert(*$1, *$5) == -1) yyerror("Variable redefinition\n");
					}
					;

// array declaration
arr_dec: ID ':' var_type '[' const_val ']' ';' {
					Trace("Array Declaration");
					if (!isConstant(*$5)) yyerror("Array size not constant");
					if ($5->type != int_type) yyerror("Array size invalid");
					if ($5->value.i < 1) yyerror("Array size < 1");
					if (syms.insert(*$1, $3, $5->value.i) == -1) yyerror("Variable redefinition\n");
				}
				;

// variable type
var_type: INTEGER { $$ = int_type; }
		| STRING { $$ = string_type; }
		| BOOLEAN { $$ = bool_type; }
		| FLOAT { $$ = real_type; }
		;

// zero or more function or procedure declaration
zero_more_fp_dec: one_more_fp_dec | /* zero */
				;

// one or more function or procedure declaration
one_more_fp_dec: fp_dec one_more_fp_dec | fp_dec
				;

// function or procedure declaration
fp_dec: PROCEDURE ID {
						Trace("Function Declaration");
						IDInfo* info = new IDInfo();
						info->flag = function_flag;
						info->initialized = false;
						if (syms.insert(*$2, *info) == -1) yyerror("Procedure redefinition");
						syms.push();
					}
					zero_more_formal_arg {
						IDInfo info = *syms.lookup(*$2);
						out << "\tmethod public static ";
						out << ((info.type == void_type)? "void" : "int");
						out << " " + info.id_name + "(";
						for (int i = 0; i < info.value.args.size(); i++) {
							if (i != 0) out << ", ";
							out << "int";
						}
						out << ")" << endl;
						out << "\tmax_stack 15" << endl;
						out << "\tmax_locals 15" << endl << "\t{" << endl;
					}
					opt_return_type block_stat END endFunc {
						if(syms.lookup(*$2)->type == void_type) out << "\t\treturn" << endl << "\t}" << endl;
						else out << "\t}" << endl;;
						if(Opt_D) syms.dump();
						syms.pop();
					}
					;

// handle end of function
endFunc: ID | ID ';'

// zero or more formal arguments
zero_more_formal_arg: '(' one_more_formal_arg ')' | /* zero */
					;

// one or more formal arguments
one_more_formal_arg	: arg ';' one_more_formal_arg | arg
					;

// argument
arg: ID ':' var_type {
						IDInfo* info = new IDInfo();
						info->flag = var_flag;
						info->type = $3;
						info->initialized = false;
						if (syms.insert(*$1, *info) == -1) yyerror("Variable redefinition");
						syms.addFunctionArgument(*$1, *info);
					}
					;

// optional return type
opt_return_type: RETURN var_type {
						syms.setFunctionType($2);
					}
					| /* zero */ {
						syms.setFunctionType(void_type);
					}
					;

// zero or more statements
zero_more_stat: one_more_stat | /* zero */
			;

// one or more statements
one_more_stat: stmt one_more_stat | stmt
			;

// statement
stmt: bos_stmt | conditional_stat | loop_stat | func_invoc ';'
	;

// block or simple statement
bos_stmt: block_stat | simple_stmt
		;

// simple -> variable assigment
simple_stmt: ID ASSIGN expr ';' {
						Trace("Variable Assignment");
						IDInfo* info = syms.lookup(*$1);
						if (info == NULL) yyerror("Undeclared identifier");
						if (info->flag == const_var_flag) yyerror("Constant variables cannot be reassigned");
						if (info->flag == function_flag || info->flag == procedure_flag) yyerror("Procedures cannot be assigned\n");
						if (info->type != $3->type) yyerror("Types do not match");

						if(info->type == int_type || info->type == bool_type) {
							int idx = syms.getIndex(*$1);
							if(idx == -1) {
								out << "\t\tputstatic int " << filename << "." << *$1 << endl;
							}
							else out << "\t\tistore " << idx << endl;
						}
					}
					// simple -> array assignment
					| ID '[' expr ']' ASSIGN expr ';'{
						Trace("Array Assignment");
						IDInfo* info = syms.lookup(*$1);
						if (info == NULL) yyerror("Undeclared identifier");
						if (info->flag != var_flag) yyerror("Not a variable");
						if (info->type != array_type) yyerror("Not an array");
						if ($3->type != int_type) yyerror("Invalid index");
						if ($3->value.i < 0 || $3->value.i >= info->value.args.size()) yyerror("Index out of range");
						if (info->value.args[0].type != $6->type) yyerror("Types do not match");
					}
					| {
						out << "\t\tgetstatic java.io.PrintStream java.lang.System.out" << endl;
					}
					// simple -> print expression
					PRINT expr ';' {
						Trace("Print Expression");
						if($3->type == string_type) out << "\t\tinvokevirtual void java.io.PrintStream.print(java.lang.String)" << endl;
						else out << "\t\tinvokevirtual void java.io.PrintStream.print(int)" << endl;;
					}
					| {
						out << "\t\tgetstatic java.io.PrintStream java.lang.System.out" << endl;
					// simple -> println expression
					} PRINTLN expr ';' {
						Trace("Println Expression");
						if($3->type == string_type) out << "\t\tinvokevirtual void java.io.PrintStream.println(java.lang.String)" << endl;
						else out << "\t\tinvokevirtual void java.io.PrintStream.println(int)" << endl;
					}
					// read identifier
					| READ ID ';' {
						Trace("Read Identifier");
						IDInfo* info = syms.lookup(*$2);
						if (info == NULL) yyerror("Undeclared identifier");
						if (info->flag != const_var_flag || info->flag != var_flag) yyerror("Not a variable\n");
						if (info->type != string_type) yyerror("Incompatible type");
					// return
					} | RETURN ';'
					{
						Trace("Return");
						out << "\t\treturn" << endl;
					// return expression
					} | RETURN expr ';' {
						Trace("Return Expression");
						out << "\t\tireturn" << endl;
					}
					;

// block statement
block_stat: block_stat_clean ';' | block_stat_clean
			;

// block statement clean (without ;)
block_stat_clean: zero_one_vc_dec BEGIN_TOKEN {
						syms.push();
					}
					one_more_stat END {
						if(Opt_D) syms.dump();
						syms.pop();
					}
				;

// conditional statement: if
conditional_stat: IF expr ifStart THEN bos_stmt END IF ';' {
						Trace("Conditional statement: if");
						if ($2->type != bool_type) yyerror("Condition type does not match");
						out << "L" << la.takeLabel(0) << ":" << endl; //Lexit
  						la.removeLabel();
					}
					// conditional statement: if else
					| IF expr ifStart THEN bos_stmt ELSE {
						out << "\t\tgoto L" << la.takeLabel(1) << endl; //Lexit
  						out << "L" << la.takeLabel(0) << ":" << endl; //Lfalse
					}
					bos_stmt END IF ';' {
						Trace("Conditional statement: if else");
						if ($2->type != bool_type) yyerror("Condition type does not match");
						out << "L" << la.takeLabel(1) << ":" << endl; //Lexit
  						la.removeLabel();
					}
				;

ifStart: {
			la.pushSeveralLabels(2);
  			out << "\t\tifeq L" << la.takeLabel(0) << endl;
		}
		;

// loop statement while
loop_stat: WHILE {
					la.pushSeveralLabels(1);
  					out << "L" << la.takeLabel(0) << ":" << endl; //Lbegin
				} 
				expr {
						la.severalLabels(1);
  						out << "\t\tifeq L" << la.takeLabel(3 + la.getFlag()) << endl; //Ltrue
					} 
				LOOP bos_stmt END LOOP ';' {
						Trace("Loop statement: while");
						if ($3->type != bool_type) yyerror("Condition type does not match");
						out << "\t\tgoto L" << la.takeLabel(la.getFlag()) << endl; //Lfalse
  						out << "L" << la.takeLabel(3 + la.getFlag()) << ":" << endl; //Ltrue
  						la.removeLabel();
					}
					| FOR '(' ID IN const_val RANGE const_val ')' LOOP {
						Trace("loop stmt");

						IDInfo* info = syms.lookup(*$3);
						if (info == NULL) yyerror("Undeclared identifier");

						if ($5->type != int_type || $7->type != int_type) yyerror("Incompatible range");

						beginning_for(*info, *$5, *$7);

					} bos_stmt END LOOP ';' {
						IDInfo* info = syms.lookup(*$3);
						end_for(*info);
					}
					;

// function or procedure invocation
func_invoc: ID {
				fps.push_back(vector<IDInfo>());
				} 
				'(' opt_comma_sep_exprs ')'  {
						Trace("Function or Procedure Invocation");
						IDInfo* info = syms.lookup(*$1);
						if (info == NULL) yyerror("Undeclared identifier");
						if (info->flag != function_flag && info->flag != procedure_flag) yyerror("Not a function nor a procedure, flag " + to_string(info->flag));

						vector<IDInfo> parameters = info->value.args;
						if (parameters.size() != fps[fps.size() - 1].size()) yyerror("Number of parameters do not match");

						for (int i = 0; i < parameters.size(); i++)
						{
							if (parameters[i].type != fps[fps.size() - 1].at(i).type) yyerror("Parameter types do not match");
						}

						out << "\t\tinvokestatic ";
  						out << ((info->type == void_type)? "void" : "int");
  						out << " " + filename + "." + info->id_name + "(";
  						for (int i = 0; i < info->value.args.size(); ++i) {
    						if (i != 0) out << ", ";
    							out << "int";
  						}
  						out << ")" << endl;

						$$ = info;
						fps.pop_back();
					}
			;

// optional comma separator expression
opt_comma_sep_exprs: comma_sep_exprs | /* zero */
					;

// comma seperator expression
comma_sep_exprs: func_expr ',' comma_sep_exprs | func_expr
					;

// function expression
func_expr: expr {
					fps[fps.size() - 1].push_back(*$1);
				}
		;

// constant value
const_val : INTConst {
						Trace("Constant Value");
						$$ = intConstant($1);
					}
			| STRConst {
						Trace("String Constant Value");
						$$ = strConstant($1);
					}
			| BOOLConst {
						$$ = boolConstant($1);
					}
			| FLOATConst {
						$$ = floatConstant($1);
					}
			;

// expression id
expr 				: ID {
						IDInfo* info = syms.lookup(*$1);
						if (info == NULL) yyerror("Undeclared identifier\n");
						load_static_global_variable(*info);
						$$ = info;

					}
					// expression constant value
					| const_val {
						constant(*$1);
					}
					// expression array
					| ID '[' expr ']' {
						IDInfo* info = syms.lookup(*$1);
						if (info == NULL) yyerror("Undeclared identifier\n");
						if (info->type != array_type) yyerror("Not an array\n");
						if ($3->type != int_type) yyerror("Array subscript not an integer\n");
						if ($3->value.i < 0) yyerror("Array subscript invalid\n");
						if ($3->value.i >= info->value.args.size()) yyerror("Array subscript out of range\n");
						$$ = new IDInfo(info->value.args[$3->value.i]);
					}
					// expression function invocation
					| func_invoc {
						Trace("Function Invocation as Expression");
					}
					// expression unary minus
					| '-' expr %prec Unary_minus {
						Trace("Unary minus Expression");
						if (!($2->type == int_type || $2->type == real_type)) yyerror("Operator error\n");
						IDInfo* info = new IDInfo();
						info->flag = var_flag;
						info->type = $2->type;
						$$ = info;

						if ($2->type == int_type) out << "\t\tineg" << endl;
					}

					// multiply
					| expr '*' expr {
						Trace("Expression * Expression");
						if ($1->type != $3->type) yyerror("Operand types do not match\n");
						if (!($1->type == int_type || $1->type == real_type)) yyerror("Operator error\n");
						IDInfo* info = new IDInfo();
						info->flag = var_flag;
						info->type = $1->type;
						$$ = info;

						if ($1->type == int_type) out << "\t\timul" << endl;
					}

					// divide
					| expr '/' expr {
						Trace("Expression / Expression");
						if ($1->type != $3->type) yyerror("Operand types do not match\n");
						if (!($1->type == int_type || $1->type == real_type)) yyerror("Operator error\n");
						IDInfo* info = new IDInfo();
						info->flag = var_flag;
						info->type = $1->type;
						$$ = info;

						if ($1->type == int_type) out << "\t\tidiv" << endl;
					}

					// add
					| expr '+' expr
					{
						Trace("Expression + Expression");
						if ($1->type != $3->type) yyerror("Operand types do not match\n");
						if (!($1->type == int_type || $1->type == real_type)) yyerror("Operator error\n");
						IDInfo* info = new IDInfo();
						info->flag = var_flag;
						info->type = $1->type;
						$$ = info;

						if ($1->type == int_type) out << "\t\tiadd" << endl;
					}

					// minus
					| expr '-' expr {
						Trace("Expression - Expression");
						if ($1->type != $3->type) yyerror("Operand types do not match\n");
						if (!($1->type == int_type || $1->type == real_type)) yyerror("Operator error\n");
						IDInfo* info = new IDInfo();
						info->flag = var_flag;
						info->type = $1->type;
						$$ = info;

						if ($1->type == int_type) out << "\t\tisub" << endl;
					}

					// less than
					| expr '<' expr {
						Trace("Expression < Expression");
						if ($1->type != $3->type) yyerror("Operand types do not match");
						if (!($1->type == int_type || $1->type == real_type)) yyerror("Operator error\n");
						IDInfo* info = new IDInfo();
						info->flag = var_flag;
						info->type = bool_type;
						$$ = info;

						if ($1->type == int_type) {
							out << "\t\tisub" << endl;
  							int lb1 = la.getLable();
  							int lb2 = la.getLable();
							out << "\t\tiflt";
							out << " L" << lb1 << endl; //Ltrue
  							out << "\t\ticonst_0" << endl;
  							out << "\t\tgoto L" << lb2 << endl; //Lfalse
							out << "L" << lb1 << ":" << endl; //Ltrue
							out << "\t\ticonst_1" << endl; 
							out << "L" << lb2 << ":" << endl; //Lfalse
						}
					}

					// less than or equal
					| expr LE expr {
						Trace("Expression <= Expression");
						if ($1->type != $3->type) yyerror("Operand types do not match");
						if (!($1->type == int_type || $1->type == real_type)) yyerror("Operator error\n");
						IDInfo* info = new IDInfo();
						info->flag = var_flag;
						info->type = bool_type;
						$$ = info;

						if ($1->type == int_type) {
							out << "\t\tisub" << endl;
  							int lb1 = la.getLable();
  							int lb2 = la.getLable();
							out << "\t\tifle";
							out << " L" << lb1 << endl; //Ltrue
  							out << "\t\ticonst_0" << endl;
  							out << "\t\tgoto L" << lb2 << endl; //Lfalse
							out << "L" << lb1 << ":" << endl; //Ltrue
							out << "\t\ticonst_1" << endl;
							out << "L" << lb2 << ":" << endl; //Lfalse
						}
					}

					// equal
					| expr '=' expr {
						Trace("Expression = Expression");
						if ($1->type != $3->type) yyerror("Operand types do not match");
						if (!($1->type == int_type || $1->type == real_type)) yyerror("Operator error\n");
						IDInfo* info = new IDInfo();
						info->flag = var_flag;
						info->type = bool_type;
						$$ = info;

						if ($1->type == int_type) {
							out << "\t\tisub" << endl;
  							int lb1 = la.getLable();
  							int lb2 = la.getLable();
							out << "\t\tifeq";
							out << " L" << lb1 << endl; //Ltrue
  							out << "\t\ticonst_0" << endl;
  							out << "\t\tgoto L" << lb2 << endl; //Lfalse
							out << "L" << lb1 << ":" << endl; //Ltrue
							out << "\t\ticonst_1" << endl;
							out << "L" << lb2 << ":" << endl; //Lfalse
						}
					}

					// greater than or equal
					| expr GE expr {
						Trace("Expression => Expression");
						if ($1->type != $3->type) yyerror("Operand types do not match");
						if (!($1->type == int_type || $1->type == real_type)) yyerror("Operator error\n");
						IDInfo* info = new IDInfo();
						info->flag = var_flag;
						info->type = bool_type;
						$$ = info;

						if ($1->type == int_type) {
							out << "\t\tisub" << endl;
  							int lb1 = la.getLable();
  							int lb2 = la.getLable();
							out << "\t\tifge";
							out << " L" << lb1 << endl; //Ltrue
  							out << "\t\ticonst_0" << endl;
  							out << "\t\tgoto L" << lb2 << endl; //Lfalse
							out << "L" << lb1 << ":" << endl; //Ltrue
							out << "\t\ticonst_1" << endl;
							out << "L" << lb2 << ":" << endl; //Lfalse
						}
					}

					// greater than
					| expr '>' expr {
						Trace("Expression > Expression");
						if ($1->type != $3->type) yyerror("Operand types do not match");
						if (!($1->type == int_type || $1->type == real_type)) yyerror("Operator error\n");
						IDInfo* info = new IDInfo();
						info->flag = var_flag;
						info->type = bool_type;
						$$ = info;

						if ($1->type == int_type) {
							out << "\t\tisub" << endl;
  							int lb1 = la.getLable();
  							int lb2 = la.getLable();
							out << "\t\tifgt";
							out << " L" << lb1 << endl; //Ltrue
  							out << "\t\ticonst_0" << endl;
  							out << "\t\tgoto L" << lb2 << endl; //Lfalse
							out << "L" << lb1 << ":" << endl; //Ltrue
							out << "\t\ticonst_1" << endl;
							out << "L" << lb2 << ":" << endl; //Lfalse
						}
					}

					// not equal
					| expr NE expr {
						Trace("Expression /= Expression");
						if ($1->type != $3->type) yyerror("Operand types do not match");
						if (!($1->type == int_type || $1->type == real_type)) yyerror("Operator error\n");
						IDInfo* info = new IDInfo();
						info->flag = var_flag;
						info->type = bool_type;
						$$ = info;

						if ($1->type == int_type || $1->type == bool_type) {
							out << "\t\tisub" << endl;
  							int lb1 = la.getLable();
  							int lb2 = la.getLable();
							out << "\t\tifne";
							out << " L" << lb1 << endl; //Ltrue
  							out << "\t\ticonst_0" << endl;
  							out << "\t\tgoto L" << lb2 << endl; //Lfalse
							out << "L" << lb1 << ":" << endl; //Ltrue
							out << "\t\ticonst_1" << endl;
							out << "L" << lb2 << ":" << endl; //Lfalse
						}
					}

					// NOT
					| NOT expr {
						Trace("NOT Expression");
						if ($2->type != bool_type) yyerror("operator error");
						IDInfo* info = new IDInfo();
						info->flag = var_flag;
						info->type = bool_type;
						$$ = info;

						if ($2->type == bool_type) out << "\t\tldc 1" << endl << "\t\tixor" << endl;
					}

					// AND
					| expr AND expr {
						Trace("expr AND expr");
						if ($1->type != $3->type) yyerror("Operand types do not match");
						if ($1->type != bool_type) yyerror("Operator error\n");
						IDInfo* info = new IDInfo();
						info->flag = var_flag;
						info->type = bool_type;
						$$ = info;

						if ($1->type == bool_type) out << "\t\tiand" << endl;
					}

					// OR
					| expr OR expr {
						Trace("Expression OR Expression");
						if ($1->type != $3->type) yyerror("Operand types do not match");
						if ($1->type != bool_type) yyerror("Operator error\n");
						IDInfo* info = new IDInfo();
						info->flag = var_flag;
						info->type = bool_type;
						$$ = info;

						if ($1->type == bool_type) out << "\t\tior" << endl;
					}

					// (expr) 
					| '(' expr ')'{
						Trace("(expr)");
						$$ = $2;
					}
					;		

%%

void yyerror(string s) {
  cerr << "line error" << line << ": " << s << endl;
  exit(1);
}

int main(int argc, char **argv) {
	
  yyin = fopen(argv[1], "r");
  string source = string(argv[1]);
  int dotLocation = source.find(".");
  filename = source.substr(0, dotLocation);
  out.open(filename + ".jasm");

  yyparse();
  return 0;
}