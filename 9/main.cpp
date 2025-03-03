/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int precedence(char op) {
    if (op == '+' || op == '-') {
        return 1;
    }else if (op == '*' || op == '/' || op == '%') {
        return 2;
    }

    return 0;
}

int is_operator(char c) {
    if(c == '+' || c == '-' || c == '*' || c == '/' || c == '%'){
        return 1;
    }

    return 0;
}

void infix_to_postfix(char* infix, char* postfix) {
    int poi = 0, i = 0;
    char stack[4001];
    int top = -1;

    for (i = 0; infix[i] != '\0'; i++) {
        if (isdigit(infix[i])) {
            postfix[poi++] = infix[i];

            while (isdigit(infix[i+1])) {
                postfix[poi++] = infix[++i];
            }

            postfix[poi++] = ' ';

        } else if (infix[i] == '(') {
            stack[++top] = infix[i];

        } else if (infix[i] == ')') {

            while (top != -1 && stack[top] != '(') {
                postfix[poi++] = stack[top--];
                postfix[poi++] = ' ';
            }

            top--; // Pop

        } else if (is_operator(infix[i])) {

            while (top != -1 && precedence(stack[top]) >= precedence(infix[i]) && stack[top] != '(') {

                postfix[poi++] = stack[top--];

                postfix[poi++] = ' ';
            }
            stack[++top] = infix[i];
        }
    }

    while (top != -1) {
        postfix[poi++] = stack[top--];
        postfix[poi++] = ' ';
    }

    postfix[poi - 1] = '\0';
}

long long evaluate_postfix(char* postfix) {
    long long stack[4001], n;
    int top = -1, i = 0;
    char *token;

    token = strtok(postfix, " ");
    while (token != NULL) {
        if (isdigit(token[0])) { // If the token is a number
            stack[++top] = atoll(token);
        } else { // The token is an operator
            long long n2 = stack[top--];
            long long n1 = stack[top--];
            switch (token[0]) {
                case '+':
                    stack[++top] = n1 + n2;
                    break;
                case '-':
                    stack[++top] = n1 - n2;
                    break;
                case '*':
                    stack[++top] = n1 * n2;
                    break;
                case '/':
                    stack[++top] = n1 / n2;
                    break;
                case '%':
                    stack[++top] = n1 % n2;
                    break;
            }
        }
        token = strtok(NULL, " ");
    }

    return stack[top];
}

int main() {
    char infix[4001], postfix[4001];
    long long result;

    for(int i =0;i<3;i++){

        scanf("%s", infix);
        infix_to_postfix(infix, postfix);

        for(int i=0; postfix[i]; i++){
            if(postfix[i] == ' '){
                continue;
            }else{
                printf("%c", postfix[i]);
            }
        }
        result = evaluate_postfix(postfix);
        printf("=%lld\n", result);
    }


    return 0;
}*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Stack implementation
char stack[4000];
int top = -1;

void push(char item) {
    stack[++top] = item;
}

char pop() {
    return stack[top--];
}

int precedence(char operator) {
    switch (operator) {
            case '+':
            case '-':
            return 1;
            case '*':
            case '%':
            case '/':
            return 2;
            case '^':
            return 3;
            default:
            return -1;
    }
}

// Function to convert infix expression to postfix
void infix_to_postfix(char* infix, char* postfix) {
    int i = 0, j = 0;
    char item;
    char x;
    push('(');
    postfix[j++] = '&';
    strcat(infix, ")");
    item = infix[i];                         // Initialize before loop

    while (item != '\0') {
        if (item == '(') {
            postfix[j++] = '&';
            push(item);
        } else if (isdigit(item)) {
            postfix[j++] = item;             // Add operand symbol to postfix
        } else if (strchr("+-*/^%", item)) {  // If operator
            postfix[j++] = '&';
            x = pop();
            while (precedence(x) >= precedence(item)) {
                postfix[j++] = x;
                x = pop();
            }
            push(x);
            push(item);                      // Push current operator
        } else if (item == ')') {
            postfix[j++] = '&';
            x = pop();
            while (x != '(') {
                postfix[j++] = x;
                x = pop();
            }
        }
        i++;
        item = infix[i];                     // Move to next token
    }
    postfix[j] = '\0';                       // Add '\0' to make a string
}

long long calculate(long long n1, char item, long long n2) {
    switch (item) {
        case '+':
            return n1 + n2;
        case '-':
            return n1 - n2;
        case '*':
            return n1 * n2;
        case '/':
            if (n2 != 0) return n1 / n2;
        case '%':
            return n1 % n2;
        default:
            printf("\nUnsupported operation.");
            exit(1);
    }
}

long long Postfix_Eval(char* postfix){
    long long S2[4000];

    int top2 = -1;
    for(int i=0; postfix[i]; i++){
        char item = postfix[i];
        if(isdigit(item)){
            long long num = 0;
            while(item!='&'){
                num = num * 10 + (item-'0');
                item = postfix[++i];
            }
            S2[++top2] = num;
        }else if(item == '&'){
            continue;
        }
        else{
            long long n2 = S2[top2--];
            long long n1 = S2[top2--];
            S2[++top2] = calculate(n1, item, n2);
        }
    }
    return S2[top2];
}

void print_result(char postfix[]){
    for(int i=0; postfix[i]; i++){
        if(postfix[i] == '&'){
            continue;
        }else{
            printf("%c", postfix[i]);
        }
    }
    printf("=%lld\n",  Postfix_Eval(postfix));
}

int main() {
    char infix[4001], postfix[4000];
    for(int i=0; i<3; i++){
        scanf("%4001s", &infix);
        infix_to_postfix(infix, postfix);
        print_result(postfix);
        infix[0] = '\0';
        postfix[0] = '\0';
    }
    return 0;
}