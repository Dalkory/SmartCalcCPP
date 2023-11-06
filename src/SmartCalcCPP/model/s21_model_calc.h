#ifndef CPP3_S21_SMARTCALC_SRC_S21_MODEL_CALC_H_
#define CPP3_S21_SMARTCALC_SRC_S21_MODEL_CALC_H_

namespace s21 {

typedef struct Lexeme {
  double value;
  int type;
  int priority;
} Lexeme;

typedef enum LexemeType {
  NUMBER,
  VARIABLE_X,
  LEFT_BRACKET,
  RIGHT_BRACKET,
  PLUS,
  MINUS,
  MULT,
  DIV,
  EXP,
  MOD,
  COS,
  SIN,
  TAN,
  ACOS,
  ASIN,
  ATAN,
  SQRT,
  LN,
  LOG
} LexemeType;

typedef struct Node {
  Lexeme lexeme;
  struct Node *next;
} Node;

typedef struct Stack {
  Node *top;
} Stack;

class Model {
 public:
  Model();
  Model(char *expression);
  ~Model();
  int Calculate(char *str, double *result, double x);

 private:
  char *expression_;
  Node *operation_;
  Node *number_;

  void DeleteQueue(Node **queue);
  void InitStack(Stack *stack);
  int Push(Stack *stack, Lexeme lexeme);
  double Pop(Stack *stack);
  int IsEmpty(Stack *stack);
  void DestroyStack(Stack *stack);
  int ValidateExpression(char *expression);
  int ValidateDots(char *expression);
  int ValidateBrackets(char *expression);
  int ValidateBracketsPosition(char *expression);
  int ValidateOperators(char *expression);
  int ValidateTrigFunctions(char *expression);
  int ValidateOtherFunctions(char *expression);
  int ValidateVariableX(char *expression);
  int SetZeroPriority(char *expression, Stack *stack, int i);
  int SetFirstPriority(char *expression, Stack *stack, int i, int *unary_minus,
                       int *unary_plus);
  int SetSecondPriority(char *expression, Stack *stack, int *i);
  int SetThirdPriority(char *expression, Stack *stack, int i);
  int SetFourthPriority(char *expression, Stack *stack, int *i);
  int ParseToStack(char *expression, Stack *stack);
  int ReverseStack(Stack *input_stack, Stack *reversed_stack);
  int GetNumber(char *expression, Stack *stack, int *i, int *unary_minus,
                int *unary_plus);
  int GetPostfix(Stack *infix_stack, Stack *buffer_stack, Stack *postfix_stack);
  int CalculateResult(Stack *rpn, double x, double *result);
  double CalculateTrigFunction(Lexeme *operation, Lexeme *number);
  double CalculateSimpleOperation(Lexeme *operation, double first_number,
                                  double second_number);
};

}  // namespace s21

#endif  // CPP3_S21_SMARTCALC_SRC_S21_MODEL_CALC_H_