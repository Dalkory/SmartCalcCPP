#include "s21_model_calc.h"

#include <ctype.h>
#include <math.h>
#include <string.h>

namespace s21 {

#define OK 0
#define CALCULATION_ERROR 1
#define MEMORY_ERROR 2
#define INCORRECT_INPUT 3
#define NAN_RESULT 4
#define INF_RESULT 5

int s21::Model::Calculate(char *expression, double *result, double x) {
  int status = OK;
  *result = 0;
  Stack infix;
  Stack reversed;
  Stack buffer;

  status = ValidateExpression(expression);

  if (status == OK) {
    InitStack(&infix);
    status = ParseToStack(expression, &infix);
  }
  if (status == OK) {
    InitStack(&reversed);
    status = ReverseStack(&infix, &reversed);
  }
  if (status == OK) {
    InitStack(&buffer);
    status = GetPostfix(&reversed, &buffer, &infix);
  }
  if (status == OK) {
    status = ReverseStack(&infix, &reversed);
  }
  if (status == OK) {
    status = CalculateResult(&reversed, x, result);
    if (isnan(*result)) {
      *result = NAN;
      status = NAN_RESULT;
    }
    if (isinf(*result)) {
      *result = INFINITY;
      status = INF_RESULT;
    }
  }

  return status;
}

int s21::Model::ParseToStack(char *expression, Stack *stack) {
  int status = OK;
  int unary_plus = 0, unary_minus = 0;
  int len = (int)strlen(expression);

  for (int i = 0; i < len && status == OK; i++) {
    status = GetNumber(expression, stack, &i, &unary_minus, &unary_plus);
    if (status == OK) {
      SetZeroPriority(expression, stack, i);
      SetFirstPriority(expression, stack, i, &unary_minus, &unary_plus);
      SetSecondPriority(expression, stack, &i);
      SetThirdPriority(expression, stack, i);
      SetFourthPriority(expression, stack, &i);
    }
  }

  return status;
}

int s21::Model::GetNumber(char *expression, Stack *stack, int *i,
                          int *unary_minus, int *unary_plus) {
  char buffer[256] = {0};
  int status = OK;
  double num = 0;

  if (isdigit(expression[*i])) {
    int dot_count = 0;
    int buffer_index = 0;
    while ((isdigit(expression[*i])) || (expression[*i] == '.')) {
      if (expression[*i] == '.') {
        dot_count++;
      }
      if (dot_count <= 1) {
        buffer[buffer_index++] = expression[(*i)++];
      }
    }
    num = atof(buffer);
    if (*unary_minus == 1) {
      num *= -1;
    }
    Lexeme tmp_lexeme = {num, NUMBER, 0};
    status = Push(stack, tmp_lexeme);
  }

  *unary_minus = 0;
  *unary_plus = 0;

  return status;
}

int s21::Model::ReverseStack(Stack *input_stack, Stack *reversed_stack) {
  int status = OK;

  while (input_stack->top != NULL) {
    status = Push(reversed_stack, input_stack->top->lexeme);
    Pop(input_stack);
  }

  return status;
}

int s21::Model::GetPostfix(Stack *infix, Stack *buffer, Stack *postfix) {
  int status = OK;

  while (infix->top != NULL && status == OK) {
    if (infix->top->lexeme.type == NUMBER ||
        infix->top->lexeme.type == VARIABLE_X) {
      status = Push(postfix, infix->top->lexeme);
    } else if (infix->top->lexeme.priority == 4 ||
               infix->top->lexeme.type == LEFT_BRACKET) {
      status = Push(buffer, infix->top->lexeme);
    } else if (infix->top->lexeme.type == RIGHT_BRACKET) {
      while (buffer->top != NULL && buffer->top->lexeme.type != LEFT_BRACKET) {
        status = Push(postfix, buffer->top->lexeme);
        Pop(buffer);
      }
      if (buffer->top != NULL) {
        Pop(buffer);
      }
    } else if (buffer->top == NULL ||
               (infix->top->lexeme.priority > buffer->top->lexeme.priority)) {
      status = Push(buffer, infix->top->lexeme);
    } else if ((buffer->top != NULL) &&
               (infix->top->lexeme.priority <= buffer->top->lexeme.priority) &&
               infix->top->lexeme.type != RIGHT_BRACKET) {
      if (buffer->top->lexeme.priority == 3 &&
          infix->top->lexeme.priority == 3) {
        status = Push(buffer, infix->top->lexeme);
      } else {
        while ((buffer->top != NULL) &&
               (infix->top->lexeme.priority <= buffer->top->lexeme.priority)) {
          status = Push(postfix, buffer->top->lexeme);
          Pop(buffer);
        }
        status = Push(buffer, infix->top->lexeme);
      }
    }
    Pop(infix);
  }
  if (infix->top == NULL && buffer->top != NULL) {
    while (buffer->top != NULL) {
      status = Push(postfix, buffer->top->lexeme);
      Pop(buffer);
    }
  }

  return status;
}

int s21::Model::CalculateResult(Stack *rpn, double x, double *result) {
  int status = OK;
  Stack buffer;
  InitStack(&buffer);

  while (rpn->top != NULL && status == OK) {
    if (rpn->top->lexeme.type == NUMBER ||
        rpn->top->lexeme.type == VARIABLE_X) {
      if (rpn->top->lexeme.type == VARIABLE_X) {
        Lexeme tmp_lexeme = {x, NUMBER, 0};
        status = Push(&buffer, tmp_lexeme);
      } else {
        status = Push(&buffer, rpn->top->lexeme);
      }
      Pop(rpn);
    } else if (rpn->top->lexeme.type != NUMBER) {
      double result = 0;
      if (rpn->top->lexeme.priority == 4) {
        Lexeme operation = rpn->top->lexeme;
        Lexeme number = buffer.top->lexeme;
        result = CalculateTrigFunction(&operation, &number);
        Pop(&buffer);
        Pop(rpn);
        Lexeme tmp_lexeme = {result, NUMBER, 0};
        status = Push(&buffer, tmp_lexeme);
      } else {
        Lexeme operation = rpn->top->lexeme;
        double first_number = Pop(&buffer);
        double second_number = Pop(&buffer);
        result =
            CalculateSimpleOperation(&operation, first_number, second_number);
        Pop(rpn);
        Lexeme tmp_lexeme = {result, NUMBER, 0};
        Push(&buffer, tmp_lexeme);
      }
    }
  }

  *result = buffer.top->lexeme.value;
  Pop(&buffer);

  return status;
}

double s21::Model::CalculateTrigFunction(Lexeme *operation, Lexeme *number) {
  double result = 0;

  if (operation->type == SIN) {
    result = sin(number->value);
  }
  if (operation->type == ASIN) {
    result = asin(number->value);
  }
  if (operation->type == COS) {
    result = cos(number->value);
  }
  if (operation->type == ACOS) {
    result = acos(number->value);
  }
  if (operation->type == TAN) {
    result = tan(number->value);
  }
  if (operation->type == ATAN) {
    result = atan(number->value);
  }
  if (operation->type == SQRT) {
    result = sqrt(number->value);
  }
  if (operation->type == LOG) {
    result = log10(number->value);
  }
  if (operation->type == LN) {
    result = log(number->value);
  }

  return result;
}

double s21::Model::CalculateSimpleOperation(Lexeme *operation,
                                            double first_number,
                                            double second_number) {
  double result = 0;

  if (operation->type == PLUS) {
    result = first_number + second_number;
  }
  if (operation->type == MINUS) {
    result = second_number - first_number;
  }
  if (operation->type == MULT) {
    result = first_number * second_number;
  }
  if (operation->type == DIV) {
    result = second_number / first_number;
  }
  if (operation->type == MOD) {
    result = fmod(second_number, first_number);
  }
  if (operation->type == EXP) {
    result = pow(second_number, first_number);
  }

  return result;
}

int s21::Model::SetFirstPriority(char *expression, Stack *stack, int i,
                                 int *minus_sign, int *plus_sign) {
  int status = OK;

  if (expression[i] == '+') {
    if (i == 0 || (expression[i - 1] == '(')) {
      *plus_sign = 1;
    } else {
      Lexeme tmp_lexeme = {'+', PLUS, 1};
      status = Push(stack, tmp_lexeme);
    }
  } else if (expression[i] == '-') {
    if (i == 0 || (expression[i - 1] == '(')) {
      Lexeme tmp_lexeme = {0, NUMBER, 0};
      status = Push(stack, tmp_lexeme);
      tmp_lexeme.value = '-';
      tmp_lexeme.type = MINUS;
      tmp_lexeme.priority = 1;
      status = Push(stack, tmp_lexeme);
      *minus_sign = 0;
    } else {
      Lexeme tmp_lexeme = {'-', MINUS, 1};
      status = Push(stack, tmp_lexeme);
    }
  }

  return status;
}

int s21::Model::SetSecondPriority(char *expression, Stack *stack, int *i) {
  int status = OK;

  if (expression[*i] == '*') {
    Lexeme tmp_lexeme = {'*', MULT, 2};
    status = Push(stack, tmp_lexeme);
  } else if (expression[*i] == '/') {
    Lexeme tmp_lexeme = {'/', DIV, 2};
    status = Push(stack, tmp_lexeme);
  } else if (expression[*i] == 'm') {
    Lexeme tmp_lexeme = {'m', MOD, 2};
    status = Push(stack, tmp_lexeme);
    *i = *i + 2;
  }

  return status;
}

int s21::Model::SetThirdPriority(char *expression, Stack *stack, int i) {
  int status = OK;

  if (expression[i] == '^') {
    Lexeme tmp_lexeme = {'^', EXP, 3};
    status = Push(stack, tmp_lexeme);
  }

  return status;
}

int s21::Model::SetFourthPriority(char *expression, Stack *stack, int *i) {
  int status = OK;

  if (expression[*i] == 'c') {
    Lexeme tmp_lexeme = {'c', COS, 4};
    status = Push(stack, tmp_lexeme);
    *i += 2;
  } else if (expression[*i] == 't') {
    Lexeme tmp_lexeme = {'t', TAN, 4};
    status = Push(stack, tmp_lexeme);
    *i += 2;
  } else if (expression[*i] == 's') {
    if (expression[*i + 1] == 'i') {
      Lexeme tmp_lexeme = {'s', SIN, 4};
      status = Push(stack, tmp_lexeme);
      *i += 2;
    } else {
      Lexeme tmp_lexeme = {'q', SQRT, 4};
      status = Push(stack, tmp_lexeme);
      *i += 3;
    }
  } else if (expression[*i] == 'l') {
    if (expression[*i + 1] == 'o') {
      Lexeme tmp_lexeme = {'L', LOG, 4};
      status = Push(stack, tmp_lexeme);
      *i += 2;
    } else {
      Lexeme tmp_lexeme = {'l', LN, 4};
      status = Push(stack, tmp_lexeme);
      *i += 1;
    }
  } else if (expression[*i] == 'a') {
    if (expression[*i + 1] == 's') {
      Lexeme tmp_lexeme = {'S', ASIN, 4};
      status = Push(stack, tmp_lexeme);
    } else if (expression[*i + 1] == 'c') {
      Lexeme tmp_lexeme = {'C', ACOS, 4};
      status = Push(stack, tmp_lexeme);
    } else {
      Lexeme tmp_lexeme = {0, ATAN, 4};
      status = Push(stack, tmp_lexeme);
    }
    *i += 3;
  }

  return status;
}

int s21::Model::SetZeroPriority(char *expression, Stack *stack, int i) {
  int status = OK;

  if (expression[i] == '(') {
    Lexeme tmp_lexeme = {'(', LEFT_BRACKET, 0};
    status = Push(stack, tmp_lexeme);
  } else if (expression[i] == ')') {
    Lexeme tmp_lexeme = {')', RIGHT_BRACKET, 0};
    status = Push(stack, tmp_lexeme);
  } else if (expression[i] == 'x') {
    Lexeme tmp_lexeme = {'x', VARIABLE_X, 0};
    status = Push(stack, tmp_lexeme);
  }

  return status;
}

void s21::Model::InitStack(Stack *stack) { stack->top = NULL; }

int s21::Model::Push(Stack *stack, Lexeme add_lexeme) {
  int status = OK;
  Node *new_node = (Node *)malloc(sizeof(Node));

  if (new_node != NULL) {
    new_node->lexeme = add_lexeme;
    new_node->next = stack->top;
    stack->top = new_node;
  } else {
    status = MEMORY_ERROR;
  }

  return status;
}

double s21::Model::Pop(Stack *stack) {
  double rtn = 0.0;

  if (stack->top != NULL) {
    rtn = stack->top->lexeme.value;
    Node *temp = stack->top;
    stack->top = stack->top->next;
    free(temp);
  }

  return rtn;
}

int s21::Model::ValidateExpression(char *expression) {
  int status = OK;

  if (strlen(expression) == 0 || strlen(expression) > 255) {
    status = INCORRECT_INPUT;
  }
  if (status == OK) {
    if (ValidateBrackets(expression))
      status = INCORRECT_INPUT;
    else if (ValidateDots(expression))
      status = INCORRECT_INPUT;
    else if (ValidateOperators(expression))
      status = INCORRECT_INPUT;
    else if (ValidateTrigFunctions(expression))
      status = INCORRECT_INPUT;
    else if (ValidateOtherFunctions(expression))
      status = INCORRECT_INPUT;
    else if (ValidateVariableX(expression))
      status = INCORRECT_INPUT;
  }

  return status;
}

int s21::Model::ValidateBrackets(char *expression) {
  int left_bracket = 0;
  int right_bracket = 0;
  int status = OK;

  for (int i = 0; expression[i] != '\0'; i++) {
    if (expression[i] == '(') {
      left_bracket++;
      if (expression[i + 1] == ')') {
        status = INCORRECT_INPUT;
      }
    }
    if (expression[i] == ')') {
      right_bracket++;
      if (right_bracket > left_bracket) {
        status = INCORRECT_INPUT;
      }
    }
  }
  if (right_bracket != left_bracket) {
    status = INCORRECT_INPUT;
  }
  if (status == OK) {
    status = ValidateBracketsPosition(expression);
  }

  return status;
}

int s21::Model::ValidateBracketsPosition(char *expression) {
  int len = strlen(expression);
  int status = OK;

  for (int i = 0; expression[i] != '\0'; i++) {
    if (i == 0 && expression[i] == ')') {
      status = INCORRECT_INPUT;
    }
    if (i == len - 1 && expression[i] == '(') {
      status = INCORRECT_INPUT;
    }

    if (expression[i] == '(' && i != 0) {
      if (expression[i - 1] == ')' || isdigit(expression[i - 1])) {
        status = INCORRECT_INPUT;
      }
      if (expression[i + 1] == ')') {
        status = INCORRECT_INPUT;
      }
    }
    if (expression[i] == ')' && i != 0) {
      if (expression[i + 1] == '(' || isdigit(expression[i + 1])) {
        status = INCORRECT_INPUT;
      }
      if (expression[i - 1] == '(') {
        status = INCORRECT_INPUT;
      }
    }
  }

  return status;
}

int s21::Model::ValidateDots(char *expression) {
  int status = OK;
  char *ptr = expression;

  while (*ptr != '\0' && status == OK) {
    int dots_count = 0;

    if (isdigit(*ptr) || (*ptr == '.')) {
      while (isdigit(*ptr) || *ptr == '.') {
        if (*ptr == '.') {
          dots_count++;
          if (!isdigit(*(ptr - 1))) {
            status = INCORRECT_INPUT;
          }
        }
        ptr++;
      }
      ptr--;
    }
    if (dots_count > 1) {
      status = INCORRECT_INPUT;
    }
    ptr++;
  }

  return status;
}

int s21::Model::ValidateOperators(char *expression) {
  int status = OK;
  int len = (int)strlen(expression);

  for (int i = 0; i < len && status == OK; i++) {
    if (strchr("+-", expression[i])) {
      if (i == len - 1) {
        status = INCORRECT_INPUT;
      }
      if (i != 0 && i != len - 1) {
        if (i > 0 && (strchr(")/*^", expression[i + 1]) ||
                      strchr("/*^+-", expression[i - 1]))) {
          status = INCORRECT_INPUT;
        }
      }
    }
    if (strchr("/*^", expression[i])) {
      if (i == 0 || i == len - 1) {
        status = INCORRECT_INPUT;
      }
      if (i != 0 && i != len - 1) {
        if (i > 0 && (strchr(")/*^", expression[i + 1]) ||
                      strchr("/*^+-", expression[i - 1]))) {
          status = INCORRECT_INPUT;
        }
      }
    }
    if (expression[i] == 'm') {
      if (i == 0 || i > len - 4) {
        status = INCORRECT_INPUT;
      }
      if (i != 0 || i < len - 4) {
        if (i > 0 && !isdigit(expression[i - 1]) &&
            (strchr(")x", expression[i - 1]) == NULL)) {
          status = INCORRECT_INPUT;
        }
        if (i > 0 && !isdigit(expression[i + 3]) &&
            (strchr("(x", expression[i + 3]) == NULL)) {
          status = INCORRECT_INPUT;
        }
      }
      i += 2;
    }
  }

  return status;
}

int s21::Model::ValidateTrigFunctions(char *expression) {
  int status = OK;
  int len = (int)strlen(expression);

  for (int i = 0; i < len && status == OK; i++) {
    if (expression[i] == 'c' ||
        (expression[i] == 't' && expression[i - 1] != 'r') ||
        (expression[i] == 's' && expression[i + 1] == 'i')) {
      if (i > len - 6) {
        status = INCORRECT_INPUT;
      }
      if (i != 0 || i < len - 6) {
        if (i > 0 &&
            (strchr(")x", expression[i - 1]) || isdigit(expression[i - 1]))) {
          status = INCORRECT_INPUT;
        }
      }
      i += 2;
    }
    if (expression[i] == 'a') {
      if (i > len - 7) {
        status = INCORRECT_INPUT;
      }
      if (i != 0 || i < len - 7) {
        if (i > 0 &&
            (strchr(")x", expression[i - 1]) || isdigit(expression[i - 1]))) {
          status = INCORRECT_INPUT;
        }
      }
      i += 3;
    }
  }

  return status;
}

int s21::Model::ValidateOtherFunctions(char *expression) {
  int status = OK;
  int len = (int)strlen(expression);

  for (int i = 0; i < len && status == OK; i++) {
    if (expression[i] == 'l' && expression[i + 1] == 'n') {
      if (i > len - 5) {
        status = INCORRECT_INPUT;
      }
      if (i != 0 || i < len - 5) {
        if (i > 0 &&
            (strchr(")x", expression[i - 1]) || isdigit(expression[i - 1]))) {
          status = INCORRECT_INPUT;
        }
      }
      i++;
    }
    if (expression[i] == 's' && expression[i + 1] == 'q') {
      if (i > len - 6) {
        status = INCORRECT_INPUT;
      }
      if (i != 0 || i < len - 7) {
        if (i > 0 &&
            (strchr(")x", expression[i - 1]) || isdigit(expression[i - 1]))) {
          status = INCORRECT_INPUT;
        }
      }
      if (expression[i + 5] == '-') {
        status = INCORRECT_INPUT;
      }
      i += 3;
    }
    if (expression[i] == 'l' && expression[i + 1] == 'o') {
      if (i > len - 6) {
        status = INCORRECT_INPUT;
      }
      if (i != 0 || i < len - 6) {
        if (i > 0 &&
            (strchr(")x", expression[i - 1]) || isdigit(expression[i - 1]))) {
          status = INCORRECT_INPUT;
        }
      }
      i += 2;
    }
  }

  return status;
}

int s21::Model::ValidateVariableX(char *expression) {
  int status = OK;
  int len = (int)strlen(expression);

  for (int i = 0; i < len && status == OK; i++) {
    if (expression[i] == 'x' && len != 1) {
      if (i == 0 &&
          (isdigit(expression[i + 1]) || strchr(")x", expression[i + 1]))) {
        status = INCORRECT_INPUT;
      }
      if (i == len - 1 &&
          (isdigit(expression[i - 1]) || strchr(")x", expression[i - 1]))) {
        status = INCORRECT_INPUT;
      }
      if (i > 0 && i < len - 1 &&
          (isdigit(expression[i + 1]) || expression[i + 1] == '(' ||
           expression[i + 1] == 'x')) {
        status = INCORRECT_INPUT;
      }
      if (i > 0 && i < len - 1 &&
          (isdigit(expression[i - 1]) || expression[i - 1] == ')' ||
           expression[i - 1] == 'x')) {
        status = INCORRECT_INPUT;
      }
    }
  }

  return status;
}

void s21::Model::DeleteQueue(Node **queue) {
  Node *prev = NULL;
  if (*queue != NULL) {
    while ((*queue)->next) {
      prev = (*queue);
      (*queue) = (*queue)->next;
      free(prev);
    }
    free(*queue);
  }
}

s21::Model::Model() {
  expression_ = nullptr;
  number_ = nullptr;
  operation_ = nullptr;
}

s21::Model::Model(char *expression) {
  expression_ = expression;
  number_ = nullptr;
  operation_ = nullptr;
}

s21::Model::~Model() {
  DeleteQueue(&number_);
  DeleteQueue(&operation_);
}

}  // namespace s21
