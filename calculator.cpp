#include<iostream>
#include<string>
#include<vector>
#include<stack>
#include<queue>
#include<stdlib.h>
#include<exception>
using namespace std;

class Section {
	private:
		int sec;
		bool isNum;
		double d;
	public:
	 	Section(int s, bool _isNum) {
	 		isNum = _isNum;
	 		sec = s;
	 		d = sec;
		}
		Section(double _d) {
			isNum = true;
			d = _d;
			sec = (int)d;
		}
		double getNum() {
			if (!isANum()) throw(string("Can get an operator as number!"));
			return d;
		}
		char getOp() {
			if (isANum()) throw(string("Can get a number as operator!"));
			return (char)sec;
		}
		bool isANum() {
			return isNum;
		}
};
vector<Section> exp;
stack<Section> s1;
stack<Section> s2;

int string2int(string str) {
	int n = atoi(str.c_str());
	return n;
}

void preprocess(string str) {
	bool isnum = true;
	int i = 0;
	while (i < str.length()) {
		string p;
		if (str[i] >= '0' && str[i] <= '9') {
			while(str[i] >= '0' && str[i] <= '9') {
				p = p + str[i];
				i++; 
			}
			int n = string2int(p);
			exp.push_back(Section(n, true));
		} else {
			int n = (int)str[i];
			i++;
			exp.push_back(Section(n, false));
		}
		
	}
}

void printExp() {
	cout << "Exp:\n";
	for (int i = 0; i < exp.size(); i++) {
		if (exp[i].isANum()) {
			cout << exp[i].getNum() << endl;
		} else {
			cout << exp[i].getOp() << endl;
		}
	}
}

void printS1() {
	cout << "s1:\n";
	stack<Section> s(s1);
	while (!s.empty()) {
		if (s.top().isANum()) {
			cout << s.top().getNum() << endl;
		} else {
			cout << s.top().getOp() << endl;
		}
		s.pop();
	}
}

void printS2() {
	cout << "s2:\n";
	stack<Section> s(s2);
	while (!s.empty()) {
		if (s.top().isANum()) {
			cout << s.top().getNum() << endl;
		} else {
			cout << s.top().getOp() << endl;
		}
		s.pop();
	}
}

int countValue(char a) {
	if (a == '+' || a == '-') return 1;
	else return 2; //* /
}
bool bigerThan(char a, char b) {
	int a1 = countValue(a);
	int b1 = countValue(b);
	return a1 > b1;
}
bool smallThan(char a, char b) {
	int a1 = countValue(a);
	int b1 = countValue(b);
	return a1 < b1;
}

void postfix() {
	for (int i = 0; i < exp.size(); i++) {
		if (exp[i].isANum()) {
			s2.push(exp[i]);
		} else {
			if (s1.empty()) {
				s1.push(exp[i]);
			} else
			switch(exp[i].getOp()) {
				case '(': s1.push(exp[i]);break;
				case ')': 
				while(s1.top().getOp() != '(') {
					s2.push(s1.top());
					s1.pop();
				}
				s1.pop();
				break;
				default: 
				if (s1.top().getOp() == '(') {
					s1.push(exp[i]);
				} else {
					if (bigerThan(exp[i].getOp(), s1.top().getOp())) {
						s1.push(exp[i]);
					} else {
						while(!s1.empty()) {
							if (s1.top().getOp() == '(' || smallThan(s1.top().getOp(), exp[i].getOp())) break;
							s2.push(s1.top());
							s1.pop();
							
						}
						s1.push(exp[i]);
					}
					
				}
			}
		}
	}
	while (!s1.empty()) {
		s2.push(s1.top());
		s1.pop();
	}
}


void reverse() {
	while (!s2.empty()) {
		s1.push(s2.top());
		s2.pop();
	}
}

void countPostfix() {
	while (!s1.empty())  {
		if (s1.top().isANum()) {
			s2.push(s1.top());
			s1.pop();
		} else {
			double a = s2.top().getNum(); 
			s2.pop();
			double b = 0;
			if (!s2.empty()) 
				b = s2.top().getNum();
			else {
				if (s1.top().getOp() != '-' && s1.top().getOp() != '+')
					throw(string("Please check your input expression!"));
			}
			s2.pop();
			double result;
			switch(s1.top().getOp()) {
				case '+': result = b + a;break;
				case '-': result = b - a;break;
				case '*': result = b * a;break;
				case '/': if(a == 0) throw(string("Zero can't be the divisor!"));result = b / a;break;
			}
			s2.push(Section(result));
			s1.pop();
		}
	}
} 

void check() {
	int brackets = 0;
	bool preOp = false;
	for (int i = 0; i < exp.size(); i++) {
		if (exp[i].isANum()) {
			preOp = false;
			continue;
		}
		char op = exp[i].getOp();
		if (op == '+' ||op == '-' ||op == '*' ||op == '/') {
			if (preOp) {
				string e = "Two or more operators are neighbor!Position: ";
				char n = i+'0';
				e = e + n + ", Symbol: " + op;
				throw(e);
			}
			preOp = true;
		} else if (op == '(') {
			brackets++;
		} else if (op == ')') {
			brackets--;
			if (brackets < 0) {
				string e = "Brackets are not match!Position: ";
				char n = i+'0';
				e = e + n + ", Symbol: " + op;
				throw(e);
			}
		} else {
			string e = "Input expression exists error symbol.Only accept 0~9+-*/().Position: ";
			char n = i+'0';
			e = e + n + ", Symbol: " + op;
			throw(e);
		}
		
	}
	if (brackets != 0) {
		string e = "Final brackets are not match!Position: ";
		char n =  exp.size() + '0';
		e = e + n;
		throw(e);
	}
}
//31-3*(49+57)
int main() {
	try {
		string str;
		cout << "Please input a expression only for integer,for example: 1+4/5\n";
		cin >> str;
		
		//对算式进行预处理，分辨是否运算符以及识别整数 
		//cout << "preprocess\n";
		preprocess(str);
		//printExp();
		//对算式的合法性进行检测 
		check();
		//将算式转变成逆波兰表达式 
		//cout << "postfix\n";
		postfix();
		//printS1();
		//printS2();
		//简单的将s2中的数据逆序存储到s1中，为求值做准备 
		//cout << "reverse\n";
		reverse();
		//printS1();
		//printS2();
		//通过逆波兰表达式求值 
		//cout << "countPostfix\n";
		countPostfix();
		//printS1();
		//printS2();
		
		cout << "result: " << s2.top().getNum() << endl;
	}
	catch (string s) {
		cout << s << endl;
	} 
	catch (exception e) {
		cout << "Please check your input expression!\n";
	}
	
	return 0;
}

