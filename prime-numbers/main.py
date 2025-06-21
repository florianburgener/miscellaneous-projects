import math
import time

prime_numbers = []


def is_prime_number_1(n):
    n_sqrt = math.sqrt(n)

    for x in prime_numbers:
        if x > n_sqrt:
            break

        if n % x == 0:
            return False

    prime_numbers.append(n)
    return True


def is_prime_number_2(n):
    for x in range(3, int(math.sqrt(n)) + 1, 2):
        if n % x == 0:
            return False

    return True


def find_prime_numbers_1(n):
    A = [True] * n
    A[0] = False

    for i in range(2, int(math.sqrt(n)) + 1):
        if A[i - 1]:
            j = i * i

            while j <= n:
                A[j - 1] = False
                j = j + i

    return A


def main():
    ZZZ = 10000000

    # A = find_prime_numbers_1(ZZZ)
    # acc = 0
    # largest_prime_number = None

    # for i, x in enumerate(A):
    #     if x:
    #         acc += 1
    #         largest_prime_number = i + 1

    # return (acc, largest_prime_number)

    # 2 is a prime number.
    acc = 1
    largest_prime_number = 2

    for x in range(3, ZZZ + 1, 2):
        if is_prime_number_1(x):
            acc += 1
            largest_prime_number = x

    return (acc, largest_prime_number)


if __name__ == "__main__":
    N = 1
    acc = 0

    for _ in range(N):
        start_time = time.time()
        (prime_numbers_found, largest_prime_number) = main()
        acc += time.time() - start_time

        prime_numbers = []

    print(prime_numbers_found)
    print(largest_prime_number)
    print("--- %s seconds ---" % (acc / N))
