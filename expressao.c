#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "expressao.h"

typedef struct NodeStr {
    char data[512];
    struct NodeStr *next;
} NodeStr;

void pushStr(NodeStr **top, char *data) {
    NodeStr *node = (NodeStr *)malloc(sizeof(NodeStr));
    strcpy(node->data, data);
    node->next = *top;
    *top = node;
}

char *popStr(NodeStr **top) {
    if (*top == NULL) return NULL;
    NodeStr *temp = *top;
    char *data = strdup(temp->data);
    *top = temp->next;
    free(temp);
    return data;
}

int isOperator(char *token) {
    return (
        strcmp(token, "+") == 0 ||
        strcmp(token, "-") == 0 ||
        strcmp(token, "*") == 0 ||
        strcmp(token, "/") == 0 ||
        strcmp(token, "%") == 0 ||
        strcmp(token, "^") == 0
    );
}

int isFunction(char *token) {
    return (
        strcmp(token, "sen") == 0 ||
        strcmp(token, "cos") == 0 ||
        strcmp(token, "tg") == 0 ||
        strcmp(token, "log") == 0 ||
        strcmp(token, "raiz") == 0
    );
}

int precedence(char *op) {
    if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0) return 1;
    if (strcmp(op, "*") == 0 || strcmp(op, "/") == 0 || strcmp(op, "%") == 0) return 2;
    if (strcmp(op, "^") == 0) return 3;
    if (isFunction(op)) return 4;
    return 0;
}


char *getFormaPosFixa(char *Str) {
    static char output[512];
    output[0] = '\0';

    NodeStr *stack = NULL;
    char temp[512];
    strcpy(temp, Str);

    char *token = strtok(temp, " ");
    while (token != NULL) {
        if (isOperator(token) || isFunction(token)) {
            while (stack && precedence(stack->data) >= precedence(token)) {
                strcat(output, popStr(&stack));
                strcat(output, " ");
            }
            pushStr(&stack, token);
        } else if (strcmp(token, "(") == 0) {
            pushStr(&stack, token);
        } else if (strcmp(token, ")") == 0) {
            while (stack && strcmp(stack->data, "(") != 0) {
                strcat(output, popStr(&stack));
                strcat(output, " ");
            }
            popStr(&stack); 
        } else {
            strcat(output, token);
            strcat(output, " ");
        }
        token = strtok(NULL, " ");
    }

    while (stack) {
        strcat(output, popStr(&stack));
        strcat(output, " ");
    }

    output[strlen(output) - 1] = '\0'; 
    return output;
}

char *getFormaInFixa(char *Str) {
    static char output[512];
    NodeStr *stack = NULL;
    output[0] = '\0';

    char temp[512];
    strcpy(temp, Str);

    char *token = strtok(temp, " ");
    while (token != NULL) {
        if (isOperator(token)) {
            char *b = popStr(&stack);
            char *a = popStr(&stack);
            char exp[512];
            sprintf(exp, "(%s %s %s)", a, token, b);
            pushStr(&stack, exp);
            free(a);
            free(b);
        } else if (isFunction(token)) {
            char *a = popStr(&stack);
            char exp[512];
            sprintf(exp, "%s(%s)", token, a);
            pushStr(&stack, exp);
            free(a);
        } else {
            pushStr(&stack, token);
        }
        token = strtok(NULL, " ");
    }

    strcpy(output, popStr(&stack));
    return output;
}

typedef struct Node {
    float data;
    struct Node *next;
} Node;

void push(Node **top, float data) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->data = data;
    node->next = *top;
    *top = node;
}

float pop(Node **top) {
    if (*top == NULL) return 0;
    Node *temp = *top;
    float data = temp->data;
    *top = temp->next;
    free(temp);
    return data;
}

float getValorPosFixa(char *StrPosFixa) {
    Node *stack = NULL;
    char temp[512];
    strcpy(temp, StrPosFixa);

    char *token = strtok(temp, " ");
    while (token != NULL) {
        if (isOperator(token)) {
            float b = pop(&stack);
            float a = pop(&stack);
            if (strcmp(token, "+") == 0) push(&stack, a + b);
            else if (strcmp(token, "-") == 0) push(&stack, a - b);
            else if (strcmp(token, "*") == 0) push(&stack, a * b);
            else if (strcmp(token, "/") == 0) push(&stack, a / b);
            else if (strcmp(token, "%") == 0) push(&stack, fmod(a, b));
            else if (strcmp(token, "^") == 0) push(&stack, pow(a, b));
        } else if (isFunction(token)) {
            float a = pop(&stack);
            if (strcmp(token, "sen") == 0) push(&stack, sin(a * M_PI / 180));
            else if (strcmp(token, "cos") == 0) push(&stack, cos(a * M_PI / 180));
            else if (strcmp(token, "tg") == 0) push(&stack, tan(a * M_PI / 180));
            else if (strcmp(token, "log") == 0) push(&stack, log10(a));
            else if (strcmp(token, "raiz") == 0) push(&stack, sqrt(a));
        } else {
            push(&stack, atof(token));
        }
        token = strtok(NULL, " ");
    }
    return pop(&stack);
}

float getValorInFixa(char *StrInFixa) {
    char *pos = getFormaPosFixa(StrInFixa);
    return getValorPosFixa(pos);
}
