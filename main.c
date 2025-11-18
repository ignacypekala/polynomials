/*
 * Ten program jest implementacją kalkulatora przeprowadzającego działania na wielomianach.
 * Przyjmuje serię wielomianów poprzedzonych znakiem +, lub *.
 *
 * +2x^4+1
 * +4x^5-6x^4-x^2+2
 * *-x+15
 * .
 *
 * Na wyjściu wypisana zostaje seria wielomianów, będąca reprezentacją stanu akumulatora - zmiennej przechowującej
 * wielomian między operacjami.
 *
 * Autor: Ignacy Pękała
 *
*/

#include <stdio.h>
#include <stdlib.h>

// Stałe
#define MAX_DEG 10
#define EOI '.'

// Struktury i typy
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
    // Index 0 represents the monomial of exponent 0.
    int coefficients[MAX_DEG + 1];
    int degree;
} Polynomial;

// Zmienne globalne
char character; // Przechowuje ostatni wczytany znak, jest uakutalniana przy każdym użyciu getchar()
Polynomial accumulator;

// Funkcje
// Inicjalizacja wielomianu - wypełnia wszystkie pola zerami
void init_polynomial(Polynomial *polynomial) {
    polynomial->degree = 0;
    for (int i = 0; i <= MAX_DEG; i++) {
        polynomial->coefficients[i] = 0;
    }
}

int max(int a, int b) { return (a > b) ? a : b; }

// Oblicza i nadpisuje stopień zadanego wielomianu
void recalculate_degree(Polynomial *polynomial) {
    int degree = -1;
    for (int exponent = 0; exponent <= MAX_DEG; exponent++) {
        if (polynomial->coefficients[exponent] != 0) {
            degree = exponent;
        }
    }
    polynomial->degree = degree;
}

// Wypisuje zadany wielomian
void print_polynomial(Polynomial *polynomial) {
    int printed = 0;
    for (int exponent = polynomial->degree; exponent >= 0; exponent--) {
        int coefficient = polynomial->coefficients[exponent];
        if (coefficient != 0) {
            // <znak>
            Operation operation = (coefficient >= 0) ? addition : subtraction;
            if (printed == 0) {
                if (operation == subtraction) {
                    putchar(subtraction);
                }
            } else {
                printf(" %c ", operation);
            }
            // <wspolczynnik>x^<stopien>
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
    // Jeżeli nie wypisaliśmy żadnych jednomianów, wypisujemy 0
    if (printed == 0) {
        putchar('0');
    }

    putchar('\n');
}

// Wczytuje pojedyńczy znak z wejścia, zapisuje go i zwraca
char read_char() {
    character = (char) getchar();
    return character;
}

// Czyta kolejne znaki z wejścia, dopóki mają one określoną wartość
// Zwraca liczbę wczytanych znaków
int skip(char symbol) {
    int count = 0;
    while (character == symbol) {
        count++;
        read_char();
    }
    return count;
}

// Wczytuje znak z wejścia, zwraca +1 jeśli był to "+" i -1 dla "-"
int load_sign() {
    skip(' ');
    int result = 1;
    if (character == subtraction) {
        result = -1;
    }
    // Jeśli był znak, wczytaj następny
    if (character == subtraction || character == addition) {
        read_char();
    }
    return result;
}

// Wczytuje liczbę bez znaku, jeśli nie znajdzie żadnej liczby, zwraca podaną wartość.
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

// Wczytuje jednomian
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

// Dodaje jednomian do podanego wielomianu (nadpisuje wielomian)
void add_monomial(Polynomial *polynomial, const Monomial *monomial) {
    polynomial->coefficients[monomial->exponent] += monomial->coefficient;
    recalculate_degree(polynomial);
}


// Mnoży wielomian przez jednomian (bez nadpisywania)
// zwraca wynik mnożenia
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

// Główna logika programu
int main(void) {
    init_polynomial(&accumulator);
    read_char();

    while (character != EOI) {
        Operation operation = (Operation) character;

        skip(character);
        skip(' ');

        // Zmienna przechowująca wielomian będący wynikiem mnożenia bez utraty początkowej wartości akumulatora
        Polynomial multi_accumulator;
        init_polynomial(&multi_accumulator);

        // Wczytywanie kolejnych jednomianów (i adekwatne działania)
        while (character != '\n') {
            Monomial input_monomial = load_monomial();
            if (operation == addition) {
                add_monomial(&accumulator, &input_monomial);
            } else /* operation == multiplication */ {
                /*
                 * Tutaj zapisujemy wynik mnożenia w zmiennej,
                 * żeby jednomian po jednomianie dodać go do multi_accumulator
                 */
                Polynomial polynomial = multiply_by_monomial(&accumulator, &input_monomial);
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
