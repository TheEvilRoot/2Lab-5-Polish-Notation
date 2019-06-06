#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
	char payload;
	struct node *next;
};

struct stack {
  struct node* root;
};

typedef struct stack Stack;
typedef struct node Node;
typedef unsigned int uint;

Node * newNode(char value) {
    Node *n = (Node*) calloc(1, sizeof(Node));
    if (n != NULL) {
        n->next = NULL;
        n->payload = value;
    }
    return n;
}

Stack * newStack() {
    Stack *s = (Stack*) calloc(1, sizeof(Stack));
    if (s != NULL) {
        s->root = NULL;
    }
    return s;
}

void stackPush(Stack *stack, char payload) {
    if (stack == NULL) return;

    if (stack->root == NULL) {
        stack->root = newNode(payload);
    } else {
        Node *n = newNode(payload);
        if (n == NULL) return;

        n->next = stack->root;
        stack->root = n;
    }
}
char stackPop(Stack *stack, int *failFlag) {
    if (stack == NULL || stack->root == NULL) {
        if (failFlag != NULL) failFlag[0] = 1;
        return 0;
    }

    char ret = stack->root->payload;
    stack->root = stack->root->next;

    return  ret;
}

char stackPeek(Stack *stack, int *failFlag) {
    if (stack == NULL || stack->root == NULL) {
        if (failFlag != NULL) failFlag[0] = 1;
        return 0;
    }

    return stack->root->payload;
}

uint isValid(char c) {
    return  (c >= 97 && c <= 122) || // [a-z]
            (c >= 65 && c <= 90) || // [A-Z]
            (c >= 40 && c <= 43) || //[ ( ) + * ]
            c == 94 || // ^
            c == 45 || // -
            c == 61 || // =
            c == 47;   // /
}

int priority(char ch) {
    switch (ch) {
        case '^':
            return 4;
        case '*': case'/':
            return 3;
        case '+': case '-':
            return 2;
        case '(':
            return 1;
        default:
            return 0;
    }
}


char * enterInfix(char *message) {
    int index = 0;
    char *infix = (char*) calloc(1, sizeof(char));

    if(infix == NULL) return NULL;

    printf("%s", message);

    char c;
    while ((c = getchar()) != -1) {
        if (c == '\n') {
            infix[index] = '\0';
            break;
        } else if(isValid(c)) {
            infix[index++] = c;
        } else {
            printf("Invalid symbol found on index %d - '%c'\n", index, c);
            free(infix);
            return NULL;
        }
    }

    if (index == 0 || infix[index - 1] != '=') {
        free(infix);
        return NULL;
    }
    for (int i = 1; i < index; i++) {
        if (infix[i] == infix[i - 1])  {
            free(infix);
            return NULL;
        }
    }

    return infix;
}

char* infixToPostfix(char* infix, int length) {
    Stack *stack = newStack();

    char* pnString = (char*) calloc(length + 1, sizeof(char));
    int index = 0;
    int pnIndex = 0;
    int errorFlag=0;
    if (pnString == NULL) {
        return NULL;
    }

    while (infix[index] != '=') {
        char c = infix[index];
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
            pnString[pnIndex] = infix[index];
            pnIndex++;
        } else {
            if (infix[index] == ')') {
                if (stack->root == NULL) {
                    return NULL;
                }
                errorFlag = 0;
                while (stackPeek(stack, &errorFlag) != '(' && !errorFlag) {
                    pnString[pnIndex++] = stackPop(stack, NULL);
                }
                stackPop(stack, NULL);
            }
            else {
                if (infix[index] == '(') {
                    stackPush(stack, infix[index]);
                    length -= 2;
                }
                else {
                    if (infix[index] == '^' ||
                        infix[index] == '+' ||
                        infix[index] == '-' ||
                        infix[index] == '*' ||
                        infix[index] == '/') {
                        if (stack->root == NULL) {
                            stackPush(stack, infix[index]);
                        }
                        else {
                            if (stack->root != NULL && (priority(stackPeek(stack, NULL)) >= priority(infix[index]))) {
                                pnString[pnIndex++] = stackPop(stack, NULL);
                                stackPush(stack, infix[index]);
                            }
                            else {
                                stackPush(stack, infix[index]);
                            }
                        }
                    }
                }
            }
        }
        index++;
    }

    while (stack->root != NULL) {
        if (stackPeek(stack, NULL) == '(') {
            return NULL;
        }
        pnString[pnIndex++] = stackPop(stack, NULL);
    }
    pnString[pnIndex++] = '=';
    pnString[pnIndex] = '\0';
    return pnString;
}

void printForm(char* postfix) {
    int i = 0;
    printf("Postfix form: \n");
    while (postfix[i] != '\0') {
        printf("%c", postfix[i]);
        i++;
    }
    printf("\n");
}
int main() {
    char* classicExpression = enterInfix("Enter infix form: ");
    if (classicExpression == NULL) {
        printf("Invalid input.\n");
        return 0;
    }
    int length = strlen(classicExpression);
    printForm(infixToPostfix(classicExpression, length));
	return 0;
}





