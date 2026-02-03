#include <stdio.h>

int main(void) {
    int choice = 0;
    double c = 0.0, f = 0.0;

    printf("Temperature Converter\n");
    printf("1) Celsius -> Fahrenheit\n");
    printf("2) Fahrenheit -> Celsius\n");
    printf("Select (1 or 2): ");

    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Invalid choice input.\n");
        return 1;
    }

    if (choice == 1) {
        printf("Enter temperature in Celsius: ");
        if (scanf("%lf", &c) != 1) {
            fprintf(stderr, "Invalid temperature input.\n");
            return 1;
        }
        f = (c * 9.0 / 5.0) + 32.0;
        printf("Fahrenheit = %.2f\n", f);
    } else if (choice == 2) {
        printf("Enter temperature in Fahrenheit: ");
        if (scanf("%lf", &f) != 1) {
            fprintf(stderr, "Invalid temperature input.\n");
            return 1;
        }
        c = (f - 32.0) * 5.0 / 9.0;
        printf("Celsius = %.2f\n", c);
    } else {
        fprintf(stderr, "Please run again and choose 1 or 2.\n");
        return 1;
    }

    return 0;
}