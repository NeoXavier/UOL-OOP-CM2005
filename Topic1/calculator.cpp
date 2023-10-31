#include <iostream>

double add(double a, double b);
double subtract(double a, double b);
double multiply(double a, double b);
double divide(double a, double b);

int main(){
    double num1, num2, result;
    char operation;
    
    std::cout << "Enter first number: ";
    std::cin >> num1;
    
    std::cout << "Enter second number: ";
    std::cin >> num2;
    
    std::cout << "Enter operation (+, -, *, /): ";
    std::cin >> operation;

    switch(operation){
        case '+':
            result = add(num1, num2);
            break;
        case '-':
            result = subtract(num1, num2);
            break;
        case '*':
            result = multiply(num1, num2);
            break;
        case '/':
            result = divide(num1, num2);
            break;
        default:
            std::cout << "Invalid operator!" << std::endl;
            return 1;
    }
    
    std::cout << "Result: " << result << std::endl;
    
    return 0;
}

double add(double a, double b){
    return a + b;
}

double subtract(double a, double b){
    return a - b;
}

double multiply(double a, double b){
    return a * b;
}

double divide(double a, double b){
    return a/b;
}
