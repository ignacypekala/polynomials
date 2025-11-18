import os
import random
import re

seed = 0


def randint(a, b):
    global seed
    random.seed(seed)
    seed += 1
    return random.randint(a, b)


DIR = 'tests/'
MAX_DEG = 10
MIN_INT = -2147483648
MAX_INT = 2147483647
# MAX_INT = 500
BATCHES = [
    {
        'NAME': 'short',
        'TESTS': 500,
        'MAX_LINES': 1000,
        'BOUNDARIES': {
            '+': 500,
            '*': 50
        }
    },
    {
        'NAME': 'medium',
        'TESTS': 500,
        'MAX_LINES': 1000,
        'BOUNDARIES': {
            '+': 250,
            '*': 25
        }
    }, {
        'NAME': 'long',
        'TESTS': 500,
        'MAX_LINES': 1000,
        'BOUNDARIES': {
            '+': 100,
            '*': 10
        }
    },
    {
        'NAME': 'longer',
        'TESTS': 500,
        'MAX_LINES': 1000,
        'BOUNDARIES': {
            '+': 10,
            '*': 5
        }
    }

]


def init_poly():
    return [0 for x in range(MAX_DEG + 1)]


def stringify_polynomial(coefficients, is_input: bool):
    def spaces(min):
        return ' ' * randint(0, 3) if is_input else ' ' * min

    if not coefficients:
        return "0"  # Handle the empty polynomial case
        return
    terms = []
    degree = len(coefficients) - 1
    for i in range(degree, -1, -1):
        coeff = coefficients[i]
        if coeff == 0:
            continue
        term_parts = {"sign": "", "coeff_str": "", "x_part": "", "exp_str": ""}
        if coeff > 0:
            if len(terms) > 0:
                term_parts["sign"] = spaces(1) + "+" + spaces(1)
        else:
            term_parts["sign"] = spaces(1) + "-" + spaces(1)
            coeff = abs(coeff)
        if coeff == 1 and i != 0:
            term_parts["coeff_str"] = ""
        else:
            if coeff == int(coeff):
                term_parts["coeff_str"] = str(int(coeff))
            else:
                term_parts["coeff_str"] = str(coeff)
        if i > 0:  # Only show 'x' part if exponent is 1 or more
            term_parts["x_part"] = "x"
            if i > 1:
                term_parts["exp_str"] = f"^{spaces(0)}{i}"
            else:
                term_parts["exp_str"] = ""
        term = f"{term_parts['sign']}{spaces(0)}{term_parts['coeff_str']}{spaces(0)}{term_parts['x_part']}{spaces(0)}{term_parts['exp_str']}{spaces(0)}"
        terms.append(term)
    if not terms:
        return "0"
    else:
        full_string = "".join(terms)

        def repl(match):
            return match.group(1)

        return re.sub(r'^ (\+|(-)) +', repl, full_string)


def generate_test(batch):
    boundaries = batch.get('BOUNDARIES')
    accum = init_poly()

    # def is_valid:
    # return all(map(lambda x: -MAX_INT < x < MAX_INT, accum))
    is_valid = True

    input_text = ''
    output_text = ''
    lines = 0
    valid = is_valid
    while valid and lines <= batch.get('MAX_LINES'):
        sign = '+' if randint(0, 1) == 0 else '*'
        operation_poly = init_poly()
        boundary = boundaries.get(sign)

        if sign == '+':
            deg = randint(0, MAX_DEG)
            for exponent in range(deg):
                coeff = randint(-boundary, boundary)
                accum[exponent] += coeff
                operation_poly[exponent] = coeff
                if accum[exponent] < MIN_INT or accum[exponent] > MAX_INT:
                    valid = False
        else:
            accum_copy = accum.copy()
            accum = init_poly()

            def accum_degree():
                i = MAX_DEG
                while i >= 0 and accum_copy[i] == 0:
                    i -= 1
                if i == -1:
                    i = 0
                return i

            deg = randint(0, MAX_DEG - accum_degree())
            for exponent in range(deg + 1):
                coeff = randint(-boundary, boundary)
                for i in range(accum_degree() + 1):
                    multiplied = accum_copy[i] * coeff
                    accum[i + exponent] += multiplied
                    if multiplied < MIN_INT or multiplied > MAX_INT:
                        valid = False
                operation_poly[exponent] = coeff
                if coeff < MIN_INT or coeff > MAX_INT:
                    valid = False
                for coeff in accum:
                    if coeff < MIN_INT or coeff > MAX_INT:
                        valid = False

        if valid:
            input_text += sign + " " * randint(0, 5)
            input_text += stringify_polynomial(operation_poly, True) + "\n"
            output_text += stringify_polynomial(accum, False) + "\n"
            lines += 1

    input_text += '.'

    return [input_text, output_text]


for batch in BATCHES:
    for number in range(batch.get('TESTS')):
        path = os.path.join(DIR, batch.get('NAME'))
        try:
            os.mkdir(path)
        except FileExistsError:
            pass

        filepath = os.path.join(path, batch.get('NAME') + str(number + 1))
        [input_text, output_text] = generate_test(batch)
        input_file = open(filepath + '.in', 'wb')
        output_file = open(filepath + '.out', 'wb')

        input_file.write(bytes(input_text, "UTF-8"))
        output_file.write(bytes(output_text, "UTF-8"))

        input_file.close()
        output_file.close()
