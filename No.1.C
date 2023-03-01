#include <stdio.h>

double function(double x) {
    return x * x;
}

double integral(double a, double b, int n) {
    double h = (b - a) / n;
    double sum = (function(a) + function(b)) / 2.0;

    for (int i = 1; i < n; i++) {
        double x = a + i * h;
        sum += function(x);
    }

    return h * sum;
}

int main() {
    double a = 0.0;  
    double b = 2.0;  
    int n = 100; 

    double result = integral(a, b, n);

    printf("The integral is: %lf\n", result);

    return 0;
}
