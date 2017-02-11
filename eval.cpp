#include <stack>
#include <string>
#include <iostream>
#include <cassert>
using namespace std;

bool isNumber(char a);
bool isOperator(char a);
char applyOperator(char op1, char op2, char v, const bool values[]);
bool higherOrEqualPrec(char a, char b);
bool convert(string infix, string& postfix);
int evaluate(string infix, const bool values[], string& postfix, bool& result);

int evaluate(string infix, const bool values[], string& postfix, bool& result)
{
	stack <char> myStack;
	if (!convert(infix, postfix)) //if conversion was not possible. runs the conversion
		return 1;

	for (int i = 0; i < postfix.size(); i++)
	{
		if (isNumber(postfix[i]))
			myStack.push(postfix[i]);

		else if (isOperator(postfix[i]))
		{
			char op2 = myStack.top(); //for operand 2 and operand 1 
			myStack.pop();
			char op1;

			if (postfix[i] == '!')
				op1 = ' ';
			else
			{
				op1 = myStack.top();
				myStack.pop();
			}

			char a = applyOperator(op1, op2, postfix[i], values);
			myStack.push(a);
		}
	}
	if (myStack.size() == 1)
	{
		if (myStack.top() == 't')
			result = true;
		else if (myStack.top() == 'f')
			result = false;
		else
			result = values[myStack.top() - '0']; //what goes wrong with (2) here?? bug. 
	}

	return 0;
}

bool isNumber(char a)
{
	char numbers[] = { '0','1','2','3','4','5','6','7','8','9' };
	for (int i = 0; i < 10; i++)
	{
		if (a == numbers[i])
			return true;
	}
	return false;
}

bool isOperator(char a)
{
	char operators[] = {'|', '&', '!'}; //don't treat !  as an operator? 
	for (int i = 0; i < 3; i++)
	{
		if (a == operators[i])
			return true;
	}
	return false;
}


bool higherOrEqualPrec(char a, char b) // order: ! > & > |
{
	if (a == '!')
		return true; //a is greater >= b
	else if (a == '&' && (b == '|' || b == '&')) // or b! = '!'
		return true;
	else if (a == '|' && b == '|')
		return true;
	else //in any b> a cases return false
		return false;

}


bool convert(string infix, string& postfix) //postfix does not need blanks	
{
	stack <char> opStack; //operator stack
	postfix = "";
	char prevChar = ' ';

	if (infix.size() == 0)
		return false;

	for (int i = 0; i < infix.size(); i++)		//STEP 1 
	{
		if (isNumber(infix[i]))					//STEP 2 	
		{
			if (isNumber(prevChar) || prevChar == ')') //if number, previous cannot be a # or ). 
				return false;
			postfix += (infix[i]);
			if (prevChar == '!')		//***do I have to? TAKE CARE OF CASE WHERE !!(4&3) NOT OPERATOR COMES BEFORE PARANTHESES  
			{
				while (!opStack.empty() && opStack.top() == '!')
				{
					postfix += opStack.top();
					opStack.pop();
				}
			}
		}
				
		else if (infix[i] == '(')				//STEP 3 
		{
			if (isNumber(prevChar) || prevChar == ')') //before a (, there cannot be a # or a ) 
				return false;
			opStack.push(infix[i]);
		}

		else if (infix[i] == '!')
		{
			if (prevChar == ')' || isNumber(prevChar)) //before ! cannot be a ) or a number
				return false;
			opStack.push(infix[i]);
		}

		else if (isOperator(infix[i]) && infix[i]!= '!') //& or | 
		{
			if (prevChar == '(' || isOperator(prevChar)) //before & or |, there can't be a ( or an operator. numbs and ) are okay. 
				return false; 

			if (opStack.empty()) 	 //STEP 4 
				opStack.push(infix[i]);
			else 						//STEP 5 
			{
				while (!opStack.empty() && higherOrEqualPrec(opStack.top(), infix[i]) && opStack.top() != '(')
				{
					postfix += opStack.top();
					opStack.pop();
				}
				opStack.push(infix[i]);
			}
		}
			
		else if (infix[i] == ')')	//STEP 6
		{
			if (isOperator(prevChar) || prevChar == '(' ) //if ), previous cannot be an operator or a ( 
				return false;	

			while (!opStack.empty() && opStack.top() != '(')
			{
				postfix += opStack.top();
				opStack.pop();
			}

			if (opStack.empty()) //quit while loop - didn't find the opening parantheses (
				return false;

			else if (opStack.top() == '(') //quit while loop - found the opening parantheses ( 
				opStack.pop();
		}

		else if (infix[i] != ' ')
			return false; //the char wasn't a number, (), an operator, or a blank space. 

		if (infix[i]!= ' ')
			prevChar = infix[i]; //keep track of what the previous char was, disregarding spaces. 

	} //STEP 7 - GO BACK TO STEP 2

	//if (opStack.empty()) //didn't enter the for loop and/or pushed nothing to the stack? double check this. (())
	//	return false;

	while (!opStack.empty())		//STEP 8 
	{
		if (opStack.top() == '(') //if there were extra opening parantheses ( that didn't get popped from corresponding ) 
			return false;
		postfix += opStack.top(); 
		opStack.pop();
	}

	//check the last element in the infix string. the infix string can only end with a # or a ), or space but we ignore that.
	if (!isNumber(prevChar) && prevChar != ')')
		return false;

	return true;
}

char applyOperator(char op1, char op2, char v, const bool values[])  
{
	if (v== '!') //the operator was !, so you only need op2. op1 was dummy
	{
		if (isNumber(op2) && values[op2-'0']) //if value in array is true, return f for false. 
			return 'f';
		else if (isNumber(op2) && !values[op2 - '0'] ) //if value in array is false, return t for true. 
			return 't';
		else if (op2 == 't') //if given value is t for true, return f for false. 
			return 'f';
		else if (op2 == 'f') //if given value is f for false, return t for true. 
			return 't'; 
	}

	else if (v == '&')  
	{
		if (isNumber(op2) && isNumber(op1) && values[op2 - '0'] && values[op1 - '0']) //both true and numbers
			return 't';
		else if (isNumber(op2) && values[op2 - '0'] && op1 == 't')	//both true and op1 = 11 
			return 't';
		else if (isNumber(op1) && values[op1 - '0'] && op2 == 't') //both true and op2 = 11 
			return 't';
		else if (op2 == 't' && op1 == 't')			//both true and both op1 and op2 = 11
			return 't'; 
		else
			return 'f';						//else, return 10 for false
	}

	else if (v == '|')
	{
		if (op1 == 't' || op2 == 't')
			return 't';
		else if ((isNumber(op1) && values[op1 - '0']) || (isNumber(op2) && values[op2 - '0']))
			return 't';
		else
			return 'f';
	}

	return 'f'; //shouldn't ever be called
}


// Evaluates a boolean expression
// Postcondition: If infix is a syntactically valid infix boolean
//   expression, then postfix is set to the postfix form of that
//   expression, result is set to the value of the expression (where
//   in that expression, each digit k represents element k of the
//   values array), and the function returns zero.  If infix is not a
//   syntactically valid expression, the function returns 1.  (In that
//   case, postfix may or may not be changed, but result must be
//   unchanged.)

int main()
{
	bool ba[10] = {
		//  0      1      2      3      4      5      6      7      8      9
		true,  true,  true,  false, false, false, true,  false, true,  false
	};
	string pf;
	bool answer;
	assert(evaluate("2| 3", ba, pf, answer) == 0 && pf == "23|" &&  answer);
	assert(evaluate("8|", ba, pf, answer) == 1);
	assert(evaluate("4 5", ba, pf, answer) == 1);
	assert(evaluate("01", ba, pf, answer) == 1);
	assert(evaluate("()", ba, pf, answer) == 1);
	assert(evaluate("2(9|8)", ba, pf, answer) == 1);
	assert(evaluate("2(&8)", ba, pf, answer) == 1);
	assert(evaluate("(6&(7|7)", ba, pf, answer) == 1);
	assert(evaluate("", ba, pf, answer) == 1);
	assert(evaluate("4  |  !3 & (0&3) ", ba, pf, answer) == 0 && pf == "43!03&&|"  &&  !answer);
	assert(evaluate(" 9  ", ba, pf, answer) == 0 && pf == "9"  &&  !answer);

	assert(evaluate("1&!(9|1&1|9) | !!!(9&1&9)", ba, pf, answer) == 0 && answer);
	assert(evaluate("2", ba, pf, answer) == 0 && pf == "2" &&  answer);
	assert(evaluate("(3)", ba, pf, answer) == 0 && pf == "3" &&  !answer);
	assert(evaluate("0&!9", ba, pf, answer) == 0 && pf == "09!&" &&  answer);
	assert(evaluate("2&(3)", ba, pf, answer) == 0 && pf == "23&" &&  !answer);
	assert(evaluate("!(7|8)", ba, pf, answer) == 0 && pf == "78|!" &&  !answer);
	assert(evaluate("!7|8", ba, pf, answer) == 0 && pf == "7!8|" &&  answer);
	assert(evaluate("6|4&5", ba, pf, answer) == 0 && pf == "645&|" &&  answer);
	ba[2] = false;
	ba[9] = true;
	assert(evaluate("((9))", ba, pf, answer) == 0 && pf == "9"  &&  answer);
	assert(evaluate("2| 3", ba, pf, answer) == 0 && pf == "23|" &&  !answer);

	////  0      1      2      3      4      5      6      7      8      9
		//true, true, false, false, false, false, true, false, true, true

	assert(evaluate("!4&(5|!7)", ba, pf, answer) == 0 && pf == "4!57!|&" && answer);
	assert(evaluate("!!!3&1", ba, pf, answer) == 0 && pf == "3!!!1&" && answer);

	bool succeeded = convert("2&!!!3", pf);
		cout << "Infix was: " << "2&!!!3" << endl;
		
		if (succeeded)
			cout << "Conversion succeeded. The postfix is " << endl << pf;
		else
			cout << "Conversion failed. I converted this much:" << endl << pf;

	cout << "Passed all tests" << endl;
	string x; 
	cin >> x;
}

/*
test cases 
1&!(9|1&1|9) | !!!(9&1&9)
(!9)&2&3
!9&2&3
4  |  !3 & (0&3)
2&(
(9))
2&(9))
2&((9)
2&!((9))&(0|3)
2!3
2&!!3
2&!!(4&3)
!!(9)|!!!!2
2&!!(3&!9)

2                          evaluates to true
(3)                        evaluates to false
2&(3)                      evaluates to false
0 & !9                     evaluates to true
!(7|8)                     evaluates to false
!7|8                       evaluates to true
6|4&5                      evaluates to true
1&!(9|1&1|9) | !!!(9&1&9)  evaluates to true




*/