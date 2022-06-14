//Author: Banpreet Aulakh
//Date: June 17th, 2022

#include "Scanner.h"
#include "Stack.h"  // GENERIC STACK

#include <iostream>
#include <stdexcept>

using namespace std;

int operationHelper(Stack<Token>& numstack, Stack<Token>& opstack);
void doOperation(Stack<Token>& numstack, Stack<Token>& opstack, Token& t, int nums, int ops);

//Desc:  Implements an algorithm which evaluates a valid infix expression using 2 stack ADTs.
//PRE:   The input is well formed (in proper infix form). An input such as "+-" is not proper infix form and will result in an exception and the program immediately exiting.
//POST:  The result of the infix expression will be the only number left on the number stack at the end of the algorithm and is printed in the terminal.
int main() {
   Scanner S(cin);
   Token t;
   Stack<Token> numstack, opstack;  // 2x Stacks of type Token


   t = S.getnext();
   unsigned opcount = 0;
   unsigned numcount = 0;
   while (t.tt != eof || !opstack.isEmpty()) {
      try {
         if (t.tt == integer) {
            ++numcount;
            numstack.push(t);
            t = S.getnext();
         }
         else if (t.tt == lptok) {
            opstack.push(t);
            t = S.getnext();
         }
         else if (t.tt == rptok) {
            if (opstack.isEmpty()) { //check for parenthesis match
               throw logic_error("Invalid input");
            }
            if (opstack.peek().tt == lptok) {
               opstack.pop();
               t = S.getnext();
            }
            else {
               doOperation(numstack, opstack, t, numcount, opcount);
               numcount--;
               opcount--;
            }
         }
         else if (t.tt == pltok || t.tt == mitok || t.tt == eof) {
            if (!opstack.isEmpty() && (opstack.peek().text == "+" || opstack.peek().text == "-" || opstack.peek().text == "/" || opstack.peek().text == "*")) {
               doOperation(numstack, opstack, t, numcount, opcount);
               numcount--;
               opcount--;
            }
            else {
               opstack.push(t);
               if (opstack.peek().tt == eof) { //check for parenthesis match
                  throw logic_error("Invalid input");
               }
               ++opcount;
               t = S.getnext();
            }
         }
         else if (t.tt == slashtok || t.tt == asttok) {
            if (!opstack.isEmpty() && (opstack.peek().text == "/" || opstack.peek().text == "*")) {
               doOperation(numstack, opstack, t, numcount, opcount);
               numcount--;
               opcount--;
            }
            else {
               opstack.push(t);
               opcount++;
               t = S.getnext();
            }
         }
         else {
            throw logic_error("Invalid input");
         }
      }
      catch (std::exception e) {
         std::cout << e.what() << " Exiting program" << endl;
         return 0;
      }
   }

   int result = numstack.peek().val;
   std::cout << result << endl;
   return 0;
}
//Desc:  Function that used a try-catch block to catch any errors done using the operation helper and pushes the results onto the number stack
//PRE:   The input is well formed (The input is in proper infix form)
//POST:  Calls operation helper and assigns result of operation to t and pushes it onto the number stack. Throws error if detecting improper infix form or 0 division.
void doOperation(Stack<Token>& numstack, Stack<Token>& opstack, Token& t, int nums, int ops) {
   if (nums != (ops + 1)) {
      throw logic_error("Invalid input");
      return;
   }
   try {
      t.val = operationHelper(numstack, opstack);
   }
   catch (exception e) {
      throw e;
   }
   numstack.push(t);
}

//Desc:  Helper function that takes performs the various integer arithmetic operations using the 2 stacks in the infix algorithm
//PRE:   The input is well formed (The input is in proper infix form)
//POST:  Performs operations on 2 numbers from the stack and returns the result while removing the numbers already used. Throws error for 0 division.
int operationHelper(Stack<Token>& numstack, Stack<Token>& opstack) {
   int num1 = numstack.pop().val;
   int num2 = numstack.pop().val;
   string op = opstack.pop().text;

   switch (op[0]) {
   case '+':
      return num2 + num1;
   case '-':
      return num2 - num1;
   case '*':
      return num2 * num1;
   case '/':
      if (num1 == 0) {
         throw logic_error("Divide by zero!");
      }
      return num2 / num1;
   default:
      throw logic_error("Something went wrong!");
   }
}


