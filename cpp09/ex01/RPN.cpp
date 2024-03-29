#include "./RPN.hpp"

RPN *RPN::instanace = NULL;

RPN::RPN()
{
    
}
RPN::RPN(std::string const &input):input(input)
{
    if(!this->instanace)
        instanace = this;
    else
        this->~RPN();
}


RPN::~RPN()
{
}

RPN *RPN::getRPN(std::string const &input)
{
    if(!instanace)
        instanace = new RPN(input);
    return instanace;
}

double RPN::scanNum(char ch){
   int value;
   value = ch;
   return double(value-'0');
}
bool RPN::isOperator(char ch){
   if(ch == '+'|| ch == '-'|| ch == '*'|| ch == '/')
      return true;
   return false;
}
bool RPN::isOperand(char ch){
   if(ch >= '0' && ch <= '9')
      return true;
   return false;
}
double RPN::operation(double a_int, double b_int, char op)
{
   double b = b_int * 1.0;  
   double a = a_int * 1.0;
   if(op == '+')
      return b+a;
   else if(op == '-')
      return b-a;
   else if(op == '*')
      return b*a;
   else if(op == '/')
   {
      if(a == 0)
         throw ERROR();
      return b/a;
   }
   else
      return INT_MIN;
}
double RPN::start(){
   double a, b;
   validate();
   std::stack<double> stk;
   std::string::iterator it;
   for(it=input.begin(); it!=input.end(); it++){
      if(isOperator(*it) != false){
         a = stk.top();
         stk.pop();
         b = stk.top();
         stk.pop();
         stk.push(operation(a, b, *it));
      }
      else if(isOperand(*it) > 0){
         stk.push(scanNum(*it));
      }
   }
   return stk.top();
}

bool RPN::is_numeric(std::string const &str)
{
    std::string::const_iterator it = str.begin();
    while (it != str.end() && std::isdigit(*it))
        it++;
    return !str.empty() && it == str.end();
    
}

int RPN::operator_count(std::string const &cpy)
{
   std::string str = cpy;
   std::string check;
   int res = 0;
   std::string::iterator it = input.begin();
   while (it != input.end())
    {
        if(isOperator(*it))
            res++;
        ++it;
    }
   return res;
}
int RPN::operand_count(std::string const &cpy)
{
	//size_t position;
   std::string str = cpy;
   std::string check;
   int res = 0;
   std::string::iterator it = str.begin();
	while (it != str.end())
	{
      if(std::isdigit(*it))
         ++res;
      it++;
	}
    return res;
}

std::string RPN::trim(std::string const &str)const 
{
    size_t start_pos = str.find_first_not_of(" \r\n");
    size_t end_pos = str.find_last_not_of(" \t\r\n");

    if (start_pos == std::string::npos || end_pos == std::string::npos) {
        return "";
    }

    return str.substr(start_pos, end_pos - start_pos + 1);
}


const char *RPN::ERROR::what() const throw()
{
    return "ERROR!";
}

void RPN::validate()
{
    if(input.length() < 3)
        throw ERROR();
    std::string::iterator it = input.begin();
    if(!isOperator(trim(input).back()))
      throw ERROR();
    while (it != input.end())
    {
        if(!(std::isdigit(*it) || isOperand(*it) || isOperator(*it) || *it == ' '))
            throw ERROR();
        ++it;
    }
    if(operand_count(input) != (operator_count(input) + 1))
        throw ERROR();
}


