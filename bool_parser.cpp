#include <iostream>
#include <vector>
#include <string>
#include <cmath>

std::vector<bool *> data;
bool t = true;
bool f = false;

bool negation(bool a, bool b)
{
	return !a;
}

bool disjunction(bool a, bool b)
{
	return (a || b);
}

bool conjuction(bool a, bool b)
{
	return (a && b);
}

bool implication(bool a, bool b)
{
	return (b ? b : (a == b));
}

bool equivalence(bool a, bool b)
{
	return (a == b);
}

using boolfunc = bool(*)(bool, bool);

class Function
{
	boolfunc func;
	int get_indexes[2];
	int put_index;
public:
	Function();
	Function(boolfunc f, int* gi, int pi);
	void execute();
};

Function::Function()
{
	func = nullptr;
	put_index = get_indexes[0] = get_indexes[1] = -1;
}

Function::Function(boolfunc f, int * gi, int pi)
{
	func = f;
	for (int i{}; i < 2; ++i)
		get_indexes[i] = gi[i];
	put_index = pi;
}

void Function::execute()
{
	if (func(*(data[get_indexes[0]]), *(data[get_indexes[1]])))
		data[put_index] = &t;
	else
		data[put_index] = &f;
}

std::vector<Function> functions_stack;

bool p;
bool q;
bool r;

int data_index{};
std::string variables;
int var_counter{};


bool find(std::string & s, char c)
{
	for (int i{}; i < s.size(); ++i)
		if (s[i] == c)
			return true;
	return false;
}

void set_stack(int put_index, std::string & s)
{
	char c;
	c = s[0];
	s.erase(0, 1);
	while (c == ')' || c == ',')
	{
		c = s[0];
		s.erase(0, 1);
	}
	switch (c)
	{
	case 'E':
	case 'I':
	case 'D':
	case 'C':
	{
		s.erase(0, 1);
		data.push_back(&f);
		data.push_back(&f);
		data.push_back(&f);
		data_index += 2;
		int gi[2]{ data_index - 1, data_index };
		if (c == 'E')
			functions_stack.push_back(Function{ &equivalence, gi, put_index });
		if (c == 'I')
			functions_stack.push_back(Function{ &implication, gi, put_index });
		if (c == 'D')
			functions_stack.push_back(Function{ &disjunction, gi, put_index });
		if (c == 'C')
			functions_stack.push_back(Function{ &conjuction, gi, put_index });
		set_stack(gi[0], s);
		set_stack(gi[1], s);
	}
	break;
	case 'N':
	{
		s.erase(0, 1);
		data.push_back(&f);
		data.push_back(&f);
		data_index += 1;
		int gi[2]{ data_index, 0 };
		functions_stack.push_back(Function{ &negation, gi, put_index });
		set_stack(gi[0], s);
	}
	break;
	case 'p':
		if (!find(variables, 'p'))
		{
			variables += 'p';
			++var_counter;
		}
		data[put_index] = &p;
		break;
	case 'q':
		if (!find(variables, 'q'))
		{
			variables += 'q';
			++var_counter;
		}
		data[put_index] = &q;
		break;
	case 'r':
		if (!find(variables, 'r'))
		{
			variables += 'r';
			++var_counter;
		}
		data[put_index] = &r;
		break;
	default:
		std::cout << "You entered a wrong expresion!\n";
		s.clear();
	}
}

int main()
{
	using std::cout;
	using std::cin;

	bool p_vals[8]{ 0, 1, 0, 1, 0, 1, 0, 1 };
	bool q_vals[8]{ 0, 0, 1, 1, 0, 0, 1, 1 };
	bool r_vals[8]{ 0, 0, 0, 0, 1, 1, 1, 1 };
	std::vector<int> false_expresion_vals;

	bool is_tatology;

	std::string s;
	cout << "Enter the expresion (Q to exit):\n";
	while (cin.peek() != 'Q')
	{
		is_tatology = true;
		getline(cin, s);
		set_stack(0, s);
		if (data.size() != 0 && functions_stack.size() != 0)
		{
			for (int i{}; i < pow(2, var_counter); ++i)
			{
				if (var_counter >= 1)
				{
					p = p_vals[i];
					if (var_counter >= 2)
					{
						q = q_vals[i];
						if (var_counter == 3)
							r = r_vals[i];
					}
				}
				for (int j = functions_stack.size() - 1; j >= 0; --j)
					functions_stack[j].execute();
				if (*(data[0]) == false)
				{
					is_tatology = false;
					false_expresion_vals.push_back(i);
				}
			}

			if (is_tatology)
			{
				cout << "It's tautology!\n";
			}
			else
			{
				cout << "It's not true for:\n";
				for (int i{}; i < false_expresion_vals.size(); ++i)
				{
					if (var_counter >= 1)
					{
						cout << "p:" << p_vals[false_expresion_vals[i]];
						if (var_counter >= 2)
						{
							cout << ", q:" << q_vals[false_expresion_vals[i]];
							if (var_counter == 3)
								cout << ", r:" << r_vals[false_expresion_vals[i]];
						}
					}
					cout << std::endl;
				}
			}
			data.clear();
			functions_stack.clear();
			false_expresion_vals.clear();
			data_index = 0;
			variables.clear();
			var_counter = 0;
		}
		cout << "Enter the expresion (Q to exit):\n";
	}

	return 0;
}