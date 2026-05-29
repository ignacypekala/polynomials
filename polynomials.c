/*
 * This program is an implementation of a calculator that performs operations on polynomials.
 * It accepts a series of polynomials, each preceded by a '+' or '*' character
 *
 * Example input:
 * +2x^4+1
 * +4x^5-6x^4-x^2+2
 * *-x+15
 * .
 *
 * The output is a series of polynomials, which represent the state of 
 * the accumulator - the variable storing the polynomial between operations.
 *
 * Author: Ignacy Pękała
 *
 */

#include <stdio.h>
#include <stdlib.h>

// Constants
#define MAX_DEG 10
#define EOI '.'

// Structures and types
typedef enum {
    addition = '+',
    multiplication = '*',
    subtraction = '-'
} Operation;

typedef struct {
    int coefficient;
    int exponent;
} Monomial;

typedef struct {
    // An array of monomials ordered ascendingly by their degree.
    // Index 0 represents the monomial with an exponent of 0.
    int coefficients[MAX_DEG + 1];
    int degree;
} Polynomial;

// Global variables
char character; // Stores the last read character, updated every time getchar() is called
Polynomial accumulator;

// Functions
// Polynomial initialization - fills all fields with zeros
void init_polynomial(Polynomial *polynomial) {
    polynomial->degree = 0;
    for (int i = 0; i <= MAX_DEG; i++) {
        polynomial->coefficients[i] = 0;
    }
}

int max(int a, int b) { return (a > b) ? a : b; }

// Calculates and overwrites the degree of the given polynomial
void recalculate_degree(Polynomial *polynomial) {
    int degree = -1;
    for (int exponent = 0; exponent <= MAX_DEG; exponent++) {
        if (polynomial->coefficients[exponent] != 0) {
            degree = exponent;
        }
    }
    polynomial->degree = degree;
}

// Prints the given polynomial
void print_polynomial(Polynomial *polynomial) {
    int printed = 0;
    for (int exponent = polynomial->degree; exponent >= 0; exponent--) {
        int coefficient = polynomial->coefficients[exponent];
        if (coefficient != 0) {
            // <sign>
            Operation operation = (coefficient >= 0) ? addition : subtraction;
            if (printed == 0) {
                if (operation == subtraction) {
                    putchar(subtraction);
                }
            } else {
                printf(" %c ", operation);
            }
            // <coefficient>x^<degree>
            if (exponent > 0) {
                if (abs(coefficient) != 1) printf("%d", abs(coefficient));
                putchar('x');
                if (exponent != 1) printf("^%d", exponent);
            } else {
                printf("%d", abs(coefficient));
            }
            printed++;
        }
    }
    // If no monomials were printed, print 0
    if (printed == 0) {
        putchar('0');
    }

    putchar('\n');
}

// Reads a single character from input, saves it, and returns it
char read_char() {
    character = (char) getchar();
    return character;
}

// Reads successive characters from input as long as they match a specific value
// Returns the number of read characters
int skip(char symbol) {
    int count = 0;
    while (character == symbol) {
        count++;
        read_char();
    }
    return count;
}

// Reads a character from input, returns +1 if it was "+" and -1 for "-"
int load_sign() {
    skip(' ');
    int result = 1;
    if (character == subtraction) {
        result = -1;
    }
    // If there was a sign, read the next character
    if (character == subtraction || character == addition) {
        read_char();
    }
    return result;
}

// Reads an unsigned number, if no number is found, returns the provided default value.
int load_number(int default_value) {
    skip(' ');
    int number = 0, characters = 0;
    while (character >= '0' && character <= '9') {
        int digit = character - '0';
        number = number * 10 + digit;
        characters++;
        character = read_char();
    }
    if (characters == 0) number = default_value;
    return number;
}

// Reads a monomial
Monomial load_monomial() {
    Monomial monomial;
    int sign = load_sign();
    int absolute_coefficient = load_number(0);
    skip(' ');
    int x = skip('x');
    skip(' ');
    skip('^');
    skip(' ');

    monomial.coefficient = sign * absolute_coefficient;
    monomial.exponent = load_number(1);

    if (x == 0) {
        monomial.exponent = 0;
    } else if (monomial.coefficient == 0) {
        monomial.coefficient = sign;
    }

    return monomial;
}

// Adds a monomial to the given polynomial (overwrites the polynomial)
void add_monomial(Polynomial *polynomial, const Monomial *monomial) {
    polynomial->coefficients[monomial->exponent] += monomial->coefficient;
    recalculate_degree(polynomial);
}


// Multiplies a polynomial by a monomial (without overwriting)
// returns the result of the multiplication
Polynomial multiply_by_monomial(const Polynomial *polynomial, const Monomial *monomial) {
    Polynomial result;
    init_polynomial(&result);
    int m = monomial->exponent;

    // ax^i * bx^j = (a*b)x^(i+j)
    for (int a = 0; a <= accumulator.degree; a++) {
        result.coefficients[a + m] += monomial->coefficient * polynomial->coefficients[a];
    }

    recalculate_degree(&result);
    return result;
}

// Main program logic
int main(void) {
    init_polynomial(&accumulator);
    read_char();

    while (character != EOI) {
        Operation operation = (Operation) character;

        skip(character);
        skip(' ');

        // Variable storing the polynomial resulting from multiplication without
        // losing the initial value of the accumulator
        Polynomial multi_accumulator;
        init_polynomial(&multi_accumulator);

        // Reading subsequent monomials (and performing the adequate operations)
        while (character != '\n') {
            Monomial input_monomial = load_monomial();
            if (operation == addition) {
                add_monomial(&accumulator, &input_monomial);
            } else /* operation == multiplication */ {
                /*
                 * Store the multiplication result in a temporary variable
                 * to add it to multi_accumulator monomial by monomial.
                 */
                Polynomial polynomial = multiply_by_monomial(
                    &accumulator,
                    &input_monomial
                );
                for (int i = 0; i <= polynomial.degree; i++) {
                    Monomial multi_monomial = {
                        polynomial.coefficients[i],
                        i,
                    };
                    add_monomial(&multi_accumulator, &multi_monomial);
                }
            }
        }

        if (operation == multiplication) accumulator = multi_accumulator;

        skip('\n');
        print_polynomial(&accumulator);
    }
    return 0;
}
