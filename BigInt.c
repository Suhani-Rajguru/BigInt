
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_DIGITS 310 // 309 digits for 1024 bits and 1 extra for null terminal
typedef struct 
{
    char digits[MAX_DIGITS + 1];
    int is_negative;//flag
} BigInt;


void initializeBigInt(BigInt *num, const char *str);
void printBigInt(const BigInt *num);
void reverseString(char *str);
int compareMagnitudes(const BigInt *a,const BigInt *b);
BigInt addBigInt(const BigInt *a, const BigInt *b);
BigInt subtractBigInt(const BigInt *a, const BigInt *b);
BigInt multiplyBigInt(const BigInt *a, const BigInt *b);
void takeInput(BigInt *num);

int main() 
{
    BigInt num1, num2, result;

    printf("Enter the first BigInt number: ");
    takeInput(&num1);

    printf("Enter the second BigInt number: ");
    takeInput(&num2);

    printf("\nNumber 1: ");
    printBigInt(&num1);

    printf("Number 2: ");
    printBigInt(&num2);
        
    result = addBigInt(&num1, &num2);
    printf("\nSum: ");
    printBigInt(&result);

    result = subtractBigInt(&num1, &num2);
    printf("\nDifference: ");
    printBigInt(&result);
              
    result = multiplyBigInt(&num1, &num2);
    printf("\nProduct: ");
    printBigInt(&result);
            
    return 0;
}

BigInt addBigInt(const BigInt *a, const BigInt *b) {
    BigInt result;
    result.is_negative = 0; 

    if (a->is_negative && !b->is_negative) {
        BigInt temp_a = *a;
        temp_a.is_negative = 0; 
        return subtractBigInt(b, &temp_a); // b - |a|
    }
    else if (!a->is_negative && b->is_negative) {
        BigInt temp_b = *b;
        temp_b.is_negative = 0; 
        return subtractBigInt(a, &temp_b); // a - |b|
    }

    const char *num1 = a->digits;
    const char *num2 = b->digits;
    int len1 = strlen(num1);
    int len2 = strlen(num2);
    int max_len = (len1 > len2) ? len1 : len2;
    char temp_result[MAX_DIGITS + 2]; 
    int carry = 0, k = 0;

    for (int i = 0; i < max_len || carry; i++) {
        int digit1, digit2;
        //ASCII value of '0' is 48
        if (i < len1) {
            digit1 = num1[len1 - 1 - i] - '0';
        }
        else {
            digit1 = 0;
        }

       if (i < len2) {
            digit2 = num2[len2 - 1 - i] - '0';
        } 
        else {
            digit2 = 0;
        }

        int sum = digit1 + digit2 + carry;
        temp_result[k++] = (sum % 10) + '0'; 
        carry = sum / 10;                   
    }
    
    temp_result[k] = '\0';
    reverseString(temp_result);
    initializeBigInt(&result, temp_result);
    result.is_negative = a->is_negative;
    return result;
}

BigInt subtractBigInt(const BigInt *a, const BigInt *b) {
    BigInt result;

    if (a->is_negative && !b->is_negative) {
        BigInt temp_a = *a;
        temp_a.is_negative = 0; 
        result = addBigInt(&temp_a, b); // |a| + b
        result.is_negative = 1;        
        return result;
    } else if (!a->is_negative && b->is_negative) {
        BigInt temp_b = *b;
        temp_b.is_negative = 0; 
        return addBigInt(a, &temp_b); // a + |b|
    }

    int cmp = compareMagnitudes(a, b);
    if (cmp == 0) {
        initializeBigInt(&result, "0");
        return result;
    }

    const char *num1, *num2;
    int len1, len2;

    if (cmp > 0) { // a > b
        num1 = a->digits;
        num2 = b->digits;
        result.is_negative = a->is_negative;
    } else { // a < b
        num1 = b->digits;
        num2 = a->digits;
        result.is_negative = !b->is_negative;
    }

    len1 = strlen(num1);
    len2 = strlen(num2);

    char temp_result[MAX_DIGITS + 1];
    int borrow = 0, k = 0;

    for (int i = 0; i < len1; i++) {
        int digit1 = num1[len1 - 1 - i] - '0';
        int digit2 = (i < len2) ? num2[len2 - 1 - i] - '0' : 0;

        int diff = digit1 - digit2 - borrow;

        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }

        temp_result[k++] = diff + '0';
    }

    temp_result[k] = '\0';
    reverseString(temp_result);
    initializeBigInt(&result, temp_result);

    return result;
}

BigInt multiplyBigInt(const BigInt *a, const BigInt *b) 
{
    BigInt result;
    initializeBigInt(&result, "0"); 
    const char *num1 = a->digits;
    const char *num2 = b->digits;
    int len1 = strlen(num1);
    int len2 = strlen(num2);

    char temp_result[MAX_DIGITS + 1] = {0};

    for (int i = 0; i < len1; i++) {
        int carry = 0;

        for (int j = 0; j < len2 || carry; j++) {
            int digit1 = num1[len1 - 1 - i] - '0';
            int digit2 = (j < len2) ? num2[len2 - 1 - j] - '0' : 0;
            int product = digit1 * digit2 + carry;

            if (temp_result[i + j] != '\0') {
                product += temp_result[i + j] - '0';
            }

            temp_result[i + j] = (product % 10) + '0';
            carry = product / 10;
        }
    }

    reverseString(temp_result);
    initializeBigInt(&result, temp_result);

    result.is_negative = (a->is_negative != b->is_negative);

    return result;
}

// Function to take input from user
void takeInput(BigInt *num) {
    char input[MAX_DIGITS + 10];
    fgets(input, sizeof(input), stdin); 

    size_t len = strlen(input);
    if (input[len - 1] == '\n') //removing thr new line character
    {
        input[len - 1] = '\0';//replacing it with null terminal
    }

    initializeBigInt(num, input);
}

//function to initialise BigInt
void initializeBigInt(BigInt *num, const char *str) {
    int len = strlen(str);
    int start = 0;

    num->is_negative = (str[0] == '-');
    if (num->is_negative) {
        start = 1;
    }
//removing leading zeros
    while (start < len && str[start] == '0') {
        start++;
    }

    if (start == len) {
        strcpy(num->digits, "0");
        num->is_negative = 0;
    } else {
        strcpy(num->digits, str + start);
    }
}

// Function to print BigInt
void printBigInt(const BigInt *num) {
    if (num->is_negative && strcmp(num->digits, "0") != 0) {
        printf("-");
    }
    printf("%s\n", num->digits);
}

void reverseString(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
    }
}

int compareMagnitudes(const BigInt *a, const BigInt *b) {
    int len1 = strlen(a->digits);
    int len2 = strlen(b->digits);
    if (len1 > len2) {
        return 1;
    } else if (len1 < len2) {
        return -1;
    }
    return strcmp(a->digits, b->digits);
}
