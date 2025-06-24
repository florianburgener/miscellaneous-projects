import math
import time

NUM_RUNS = 20
MAX_PRIME_LIMIT = 1000000


# This function does not expect to receive an even number.
def is_prime_number_1(n):
    for x in range(3, int(math.sqrt(n)) + 1, 2):
        if n % x == 0:
            return False

    return True


# This algorithm checks all numbers between 3 and the square root of the number being evaluated.
# Even numbers except 2 are skipped.
def find_prime_numbers_1(n):
    return [2] + [x for x in range(3, n + 1, 2) if is_prime_number_1(x)]


# This function does not expect to receive an even number.
def is_prime_number_2(n, memoization):
    n_sqrt = math.sqrt(n)

    for x in memoization:
        if x > n_sqrt:
            break

        if n % x == 0:
            return False

    memoization.append(n)
    return True


# This algorithm keeps track of prime numbers that have already been found so that they can be used to find the next prime number more quickly,
# since the i-th prime number can only be expressed using smaller prime numbers.
def find_prime_numbers_2(n):
    memoization = [2]
    return [2] + [x for x in range(3, n + 1, 2) if is_prime_number_2(x, memoization)]


# Sieve of Eratosthenes
def find_prime_numbers_3(n):
    A = [True] * n
    A[0] = False

    for i in range(2, int(math.sqrt(n)) + 1):
        if A[i - 1]:
            j = i * i

            while j <= n:
                A[j - 1] = False
                j = j + i

    return [i + 1 for i, x in enumerate(A) if x]


def main():
    algorithms = [
        {"name": "Algorithm 1", "func": find_prime_numbers_1},
        {"name": "Algorithm 2", "func": find_prime_numbers_2},
        {"name": "Sieve of Eratosthenes", "func": find_prime_numbers_3},
    ]

    for algorithm in algorithms:
        total_execution_time = 0

        for _ in range(NUM_RUNS):
            start_time = time.time()
            prime_numbers = algorithm["func"](MAX_PRIME_LIMIT)
            total_execution_time += time.time() - start_time

        print(algorithm["name"] + ":")
        print("  Number of prime numbers: " + str(len(prime_numbers)))
        print(f"  Largest prime number less than or equal to {MAX_PRIME_LIMIT}: " + str(prime_numbers[-1]))
        print(f"  Mean execution time ({NUM_RUNS} runs): %.4f ms" % (total_execution_time / NUM_RUNS * 1000))


if __name__ == "__main__":
    main()
