# Polynomial Calculator

A command-line calculator for polynomials written in C. It parses string inputs
representing mathematical operations on polynomials, calculating sums and
products, and maintains a running track of the results in an accumulator.

This was a straightforward project I built a while ago when I was first getting
my feet wet with C programming. It served as a foundational exercise in
memory-safe programming and manual string parsing under strict environmental
constraints.

## Building and Execution

Run the program:

```bash
make polynomials
./polynomials < input.txt
```

Run the test suite:

```bash
# Run the test suite (fast)
./test.sh off

# Run the test suite with memory checks (slower)
./test.sh
```

### Usage Example

The calculator starts with an accumulator value of `0`. You can pass a `+` to
add a polynomial or `*` to multiply. Terminate the input with a period `.`.

**Input:**

```text
+ 2x^4 + 1
+ 4x^5 - 6x^4 - x^2 + 2
* -x + 15
.
```

**Output:**

```text
2x^4 + 1
4x^5 - 4x^4 - x^2 + 3
-4x^6 + 64x^5 - 60x^4 + x^3 - 15x^2 - 3x + 45
```

## Engineering Highlights

Instead of over-engineering the application, I kept the solution simple to
specifically navigate the rigid constraints set by the assignment:

* **Domain-Restricted Memory:** 

    The assignment strictly capped input lines at 1,000 characters and
    polynomial degrees at a maximum of 10. Given these hard bounds, I
    intentionally avoided dynamic allocation (malloc/free). The program relies
    entirely on stack memory, keeping it lightweight and ensuring compliance
    with the strict -Wvla compiler flag.

* **Zero-Dependency Parsing:** 

    I implemented a custom string parser to read and evaluate the polynomial
    expressions according to the assignment's strict extended BNF grammar,
    without relying on external libraries. The BNF grammar:

    ```
    <polynomial> ::= "0" | [ "-" ] <monomial> { <operation> <monomial> }
    <operation> ::= "+" | "-"
    <monomial> ::= "1" | <number> | [ <number> ] "x" [ "^" <number> ]
    <number> ::= "1" <digit> { <digit> } | <digit from 2 to 9> { <digit> }
    <digit> ::= "0" | "1" | <digit from 2 to 9>
    <digit from 2 to 9> ::= "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9"
    ```


* **Strict Compilation Rules:** 

    The project was compiled under C23 with a heavy suite of GCC flags (`-Wall`,
    `-Wextra`, `-pedantic`, and `-Werror`). To guarantee clean execution, it was
    required to run flawlessly under Valgrind configured with `--leak-check=full`.
    
    
## Testing Infrastructure & Community Adoption

Since I wrote the parsing and math logic from scratch, I built a custom
automated setup to verify it:

* **Brute-Force Generator:** 

    I wrote a Python script [generate_tests.py](./generate_tests.py) that
    algorithmically generates valid polynomial inputs based on the project's
    grammar rules, allowing me to spin up a stress-test suite of 2,000 edge cases.

* **Peer Validation:** 

    I exported these test cases using the class-standard `.in`
    and `.out` file structure. Because it followed this standardized format, I
    shared the massive test suite with my peers, who were able to seamlessly plug
    the cases into their own custom test scripts to cross-validate their
    independent implementations.

* **Tooling Setup:** 

    During development, I personally relied on
    [toster](https://github.com/MikolajKolek/toster) (a popular competetive
    programming testing tool) for rapid validation. Before publishing, I wrote
    a lightweight bash wrapper script [test.sh](./test.sh) to allow for
    seamless local execution and validation of the entire suite without needing
    external dependencies.

## Acknowledgments & License

This project was originally developed for the **Wstęp do Programowania (WDP)**
(Introductory Programming) course at MIMUW (Faculty of Mathematics, Informatics
and Mechanics of the University of Warsaw).

* Course Code: [1000-211bWPI](https://usosweb.mimuw.edu.pl/kontroler.php?_action=katalog2%2Fprzedmioty%2FpokazPrzedmiot&prz_kod=1000-211bWPI&lang=en)

The source code in this repository is my own work and is licensed under the [MIT License](./LICENSE).
