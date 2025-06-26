"""
Project : Simplex Algorithm + Branch and Bound
Version : 3.0
Date : April 2023, updated in October 2023
"""
import math
import numpy as np
import sys
import time

# Smaller than 0 : <= -EPS
# Smaller or equal to 0 : < EPS
# Greater than or equal to 0 : > -EPS
# Greater than 0 : >= EPS
EPS = 1e-6
statistics_pivot_count = 0
statistics_processed_node_count = 0
statistics_ignored_node_count = 0
statistics_tree_depth = 0


def parse_problem(filename):
    """Parses a problem.

    Args:
        filename (str): The name of the file where the problem is defined.

    Returns:
        (int, numpy.ndarray, numpy.ndarray, numpy.ndarray): Multiplier of the objective function, the objective function, the matrix A, the vector b.
    """
    with open(filename) as f:
        lines = f.readlines()

    objective_function_multiplicator, objective_function = parse_objective_function(lines[0])
    A, b = parse_constraints(lines[1:], objective_function)
    return objective_function_multiplicator, objective_function, A, b


def parse_objective_function(raw_data):
    """Parses the objective function.

    Args:
        raw_data (str): String separated by semicolons.

    Returns:
        (int, numpy.ndarray): Multiplier of the objective function, the objective function.
    """
    if raw_data[-2] != ";":
        raw_data = raw_data[:-1] + ";" + "\n"

    data = raw_data.split(";")[:-1]
    objective_function_multiplicator = 1
    objective_function = np.array([float(x) for x in data[1:]])

    if data[0] == "max":
        objective_function_multiplicator = -1
        objective_function *= -1

    return objective_function_multiplicator, objective_function


def parse_constraints(raw_constraints, objective_function):
    """Parses the constraints.

    Args:
        raw_constraints (list): List of constraints.
        objective_function (int): The objective function.

    Returns:
        (numpy.ndarray, numpy.ndarray): Matrix A, vector b.
    """
    constraints = []
    b = []

    for raw_data in raw_constraints:
        if raw_data[-2] != ";":
            raw_data = raw_data[:-1] + ";" + "\n"

        data = raw_data.split(";")[:-1]
        coefficients = np.asarray(data[:-2], dtype=float)
        comparison_symbol = data[-2]
        right_operand = float(data[-1])

        if comparison_symbol == "<=" or comparison_symbol == "=":
            constraints.append(coefficients)
            b.append(right_operand)
            check_last_constraint(constraints, objective_function)

        if comparison_symbol == ">=" or comparison_symbol == "=":
            constraints.append(coefficients * -1)
            b.append(-right_operand)
            check_last_constraint(constraints, objective_function)

    return np.array(constraints), np.array([b]).transpose()


def check_last_constraint(constraints, objective_function):
    """Checks the number of coefficients of the last constraint.

    Args:
        constraints (list): The constraint.
        objective_function (int): The objective function.
    """
    if constraints and len(constraints[-1]) != objective_function.size:
        raise Exception("The problem is wrongly defined, one or more constraints do not have the right number of coefficients.")


def create_tableau(A, b, objective_function):
    """Creates the tableau.

    Args:
        A (numpy.ndarray): The matrix A.
        b (numpy.ndarray): The vector b.
        objective_function (numpy.ndarray): The objective function.

    Returns:
        numpy.ndarray: The tableau.
    """
    T = A.copy()
    T = np.hstack((T, np.identity(b.size), b))
    T = np.vstack((T, np.hstack((objective_function, np.zeros(b.size), [0]))))
    return T


def select_pivot_column_index(T):
    """Selects the column to pivot on.

    Args:
        T (numpy.ndarray): The tableau.

    Returns:
        int: The index of the column on which to pivot.
    """
    # # ----------------------------------------------------------------
    # # Rule 1
    # # ----------------------------------------------------------------
    # pivot_column_index_candidates = np.where(T[-1, :-1] <= -EPS)[0]
    # if pivot_column_index_candidates.size == 0:
    #     # No column is selectable.
    #     return -1

    # # Selects the leftmost index with a number < 0.
    # pivot_column_index = pivot_column_index_candidates[0]
    # return pivot_column_index

    # ----------------------------------------------------------------
    # Rule 2 (this rule is the fastest)
    # ----------------------------------------------------------------
    # Selects the index with the lowest number.
    pivot_column_index = T[-1, :-1].argmin()

    if T[-1, pivot_column_index] > -EPS:
        # No column is selectable.
        return -1

    return pivot_column_index


def select_pivot_row_index(T, pivot_column_index, is_auxiliary_tableau):
    """Selects the row to pivot on.

    Args:
        T (numpy.ndarray): The tableau.
        pivot_column_index (int): The index of the column on which to pivot.
        is_auxiliary_tableau (bool): True = the tableau is auxiliary.

    Returns:
        int: The index of the row on which to pivot.
    """
    Z = 1

    if is_auxiliary_tableau:
        Z = 2

    pivot_column = T[:-Z, pivot_column_index].copy()
    lowest_ratio = np.inf
    lowest_ratio_index = -1

    for i, x in enumerate(pivot_column):
        if x < EPS:
            continue

        ratio = T[i, -1] / x

        if ratio < lowest_ratio:
            lowest_ratio = ratio
            lowest_ratio_index = i

    return lowest_ratio_index


def gaussian_elimination(T, pivot_row_index, pivot_column_index):
    """Performs 1 iteration of the Gaussian elimination algorithm.

    Args:
        T (numpy.ndarray): The tableau.
        pivot_row_index (int): The index of the row on which to pivot.
        pivot_column_index (int): The index of the column on which to pivot.
    """
    global statistics_pivot_count
    statistics_pivot_count += 1

    # Divides the row of the pivot by the value of the pivot.
    T[pivot_row_index] /= T[pivot_row_index, pivot_column_index]
    pivot_row = T[pivot_row_index, :]

    for i in range(T.shape[0]):
        multiplier = T[i, pivot_column_index]

        if (multiplier > -EPS and multiplier < EPS) or i == pivot_row_index:
            continue

        T[i] -= pivot_row * multiplier


def solve_tableau(T, selected_variables, is_auxiliary_tableau=False):
    """Solves the tableau.

    Args:
        T (numpy.ndarray): The tableau.
        selected_variables (numpy.ndarray): Selected variables for the solution.
        is_auxiliary_tableau (bool, optional): True = the tableau is auxiliary. Defaults to False.
    """
    while True:
        pivot_column_index = select_pivot_column_index(T)

        if pivot_column_index == -1:
            # There is no more column to pivot on.
            break

        if np.all(T[:-1, pivot_column_index] < EPS) and T[-1, pivot_column_index] <= -EPS:
            raise Exception("The problem has no solution.")

        pivot_row_index = select_pivot_row_index(T, pivot_column_index, is_auxiliary_tableau)
        selected_variables[pivot_row_index] = pivot_column_index
        gaussian_elimination(T, pivot_row_index, pivot_column_index)


def create_auxiliary_tableau(T):
    """Creates the auxiliary tableau.

    Args:
        T (numpy.ndarray): The tableau.

    Returns:
        numpy.ndarray: The auxiliary tableau.
    """
    right = np.hstack((np.identity(T.shape[0] - 1), np.abs(T[:-1, [-1]])))
    right = np.vstack((right, np.zeros(right.shape[1])))
    T_auxiliary = np.hstack((T, right))
    T_auxiliary = np.vstack((T_auxiliary, np.zeros(T_auxiliary.shape[1])))

    for i in range(T.shape[0]):
        if T[i, -1] <= -EPS:
            # If there is a number < 0 on the right, multiply the coefficients of the line by -1.
            T_auxiliary[i, 0 : T.shape[1]] *= -1

    for j in range(T.shape[1] - 1):
        T_auxiliary[-1, j] = -np.sum(T_auxiliary[:-2, j])

    T_auxiliary[-1, T.shape[1] : -1] = 1
    T_auxiliary[-1, -1] = -np.sum(T_auxiliary[:-2, -1])

    return T_auxiliary


def update_selected_variables_auxiliary(selected_variables, T_column_count, T_auxiliary):
    """Updates the selected variables in the auxiliary tableau.

    Args:
        selected_variables (numpy.ndarray): The list of selected variables.
        T_column_count (int): The number of columns in the tableau.
        T_auxiliary (numpy.ndarray): The auxiliary tableau.
    """
    for i in range(selected_variables.size):
        if selected_variables[i] < T_column_count:
            continue
        x = T_auxiliary.shape[1] - selected_variables[i]
        j = T_column_count - x
        selected_variables[i] = j
        gaussian_elimination(T_auxiliary, i, j)


def _find_solution(T, objective_function_multiplicator, objective_function):
    """Finds the solution.

    Args:
        T (numpy.ndarray): The tableau.
        objective_function_multiplicator (int): Multiplier of the objective function.
        objective_function (numpy.ndarray): The objective function.

    Returns:
        (numpy.ndarray, numpy.ndarray, float, numpy.ndarray, list): The tableau, the objective function, the value of the objective function, the list of selected variables.
    """
    selected_variables = None

    if np.sum(T[:-1, -1] <= -EPS):
        # Phase 1.
        T_auxiliary = create_auxiliary_tableau(T)

        if selected_variables is None:
            # Generates an array representing the selected variable.
            selected_variables = np.arange(T_auxiliary.shape[0] - 2) + T.shape[1]

        solve_tableau(T_auxiliary, selected_variables, True)

        if -T_auxiliary[-1, -1] <= -EPS or -T_auxiliary[-1, -1] >= EPS:
            raise Exception("The problem has no solution.")

        update_selected_variables_auxiliary(selected_variables, T.shape[1], T_auxiliary)
        # Extracts the tableau from the solved auxiliary tableau.
        T = T_auxiliary[: T.shape[0], : T.shape[1]]

    # Phase 2.
    if selected_variables is None:
        # If there was no phase 1 generates an array representing the selected variable (the variables selected by default are z1, z2, ...).
        selected_variables = np.arange(T.shape[0] - 1) + objective_function.size

    solve_tableau(T, selected_variables)
    objective_function_value = T[-1, -1] * -objective_function_multiplicator
    return T, objective_function, objective_function_value, selected_variables


def print_solution(T, objective_function, objective_function_value, selected_variables):
    """Displays the solution to the problem.

    Args:
        T (numpy.ndarray): The tableau.
        objective_function (numpy.ndarray): The objective function.
        objective_function_value (float): The value of the objective function.
        selected_variables (numpy.ndarray): The list of selected variables.
    """
    print("Solution :")

    selected_variables_sorted = []

    for i in range(len(selected_variables)):
        selected_variables_sorted.append((selected_variables[i], i))

    selected_variables_sorted = sorted(selected_variables_sorted, key=lambda x: x[0])

    if len(selected_variables_sorted) <= 10:
        print(" ", end="")

        for x in selected_variables_sorted:
            if x[0] < objective_function.size:
                print(("x" + str(x[0])).rjust(10, " "), end="")
            else:
                print(("z" + str(x[0] - objective_function.size)).rjust(10, " "), end="")

        print()
        print("[", end="")

        for x in selected_variables_sorted:
            print("{:10.4f}".format(T[x[1], -1]), end="")

        print("]")
    else:
        for x in selected_variables_sorted:
            if x[0] < objective_function.size:
                print(("x" + str(x[0])).ljust(4, " "), end="")
            else:
                print(("z" + str(x[0] - objective_function.size)).ljust(4, " "), end="")

            print(" = ", end="")
            print("{:10.4f}".format(T[x[1], -1]))

    print("Valeur de la fonction objectif : {:.4f}".format(objective_function_value))


"""
--------- Float Solver ---------
"""


def find_solution(filename):
    objective_function_multiplicator, objective_function, A, b = parse_problem(filename)
    T = create_tableau(A, b, objective_function)
    return _find_solution(T, objective_function_multiplicator, objective_function)


"""
--------- Integer Solver ---------
"""


def insert_constraint(T, selected_variables, non_integer_variable_index, round_upper):
    """Inserts a lower or upper constraint of the non-integer variable.

    Args:
        T (numpy.ndarray): The tableau.
        selected_variables (numpy.ndarray): The selected variables.
        non_integer_variable_index (int): The non-integer variable index.
        round_upper (bool): True inserts a upper constraint, False a lower constraint.

    Returns:
        numpy.ndarray: The new tableau.
    """
    T_copy = T.copy()
    # Inserts the row of the new constraint.
    T_copy = np.insert(T_copy, -1, np.zeros(T_copy.shape[1]), 0)
    # Inserts the column of the new constraint.
    T_copy = np.insert(T_copy, -1, np.zeros(T_copy.shape[0]), 1)

    T_copy[-2, -2] = 1
    constraint_value = T[non_integer_variable_index, -1]
    T_copy[-2, -1] = -math.ceil(constraint_value) if round_upper else math.floor(constraint_value)
    T_copy[-2, selected_variables[non_integer_variable_index]] = -1 if round_upper else 1
    gaussian_elimination(T_copy, non_integer_variable_index, selected_variables[non_integer_variable_index])
    return T_copy


def is_integer(value):
    """Checks if a float value is integer.

    Args:
        value (float): The value.

    Returns:
        bool: True if the value is integer False otherwise.
    """
    return -EPS < (value - round(value)) < EPS


def find_first_non_integer_variable_index(selected_variables, objective_function, T):
    """Finds the first non-integer variable index.

    Args:
        selected_variables (numpy.ndarray): The selected variables.
        objective_function (numpy.ndarray): The objective function.
        T (numpy.ndarray): The tableau.

    Returns:
        int: The first non-integer variable index.
    """
    for i in range(len(selected_variables)):
        if selected_variables[i] < objective_function.size and not is_integer(T[i, -1]):
            return i

    return None


def find_integer_solution(filename):
    """Find the integer solution to the problem.

    Args:
        filename (str): The path of the file containing the problem.

    Returns:
        (numpy.ndarray, numpy.ndarray, float, numpy.ndarray): The tableau, the objective function, the value of the objective function, the list of selected variables.
    """
    global statistics_processed_node_count
    global statistics_ignored_node_count
    global statistics_tree_depth

    objective_function_multiplicator, objective_function, A, b = parse_problem(filename)
    # The variable nodes is a list of tuples where each tuple
    # is structured as follows: (the unsolved tableau of the node, the depth of the tree for the node).
    nodes = [(create_tableau(A, b, objective_function), 0)]
    # Index 0 = the value of the objective function, index 1 = the solved tableau, index 2 = the selected variables.
    solution = [-np.inf, None, None]

    while nodes:
        T_unsolved, depth = nodes.pop()
        statistics_tree_depth = max(statistics_tree_depth, depth)

        try:
            statistics_processed_node_count += 1
            T, _, objective_function_value, selected_variables = _find_solution(T_unsolved, objective_function_multiplicator, objective_function)
        except Exception:
            # There is no solution.
            continue

        if objective_function_value <= solution[0]:
            # This solution is not better than the best solution.
            statistics_ignored_node_count += 1
            continue

        first_non_integer_variable_index = find_first_non_integer_variable_index(selected_variables, objective_function, T)

        if first_non_integer_variable_index is None:
            # All variables are integer, this solution is the new best solution.
            solution = [objective_function_value, T, selected_variables]
            continue

        nodes.append((insert_constraint(T, selected_variables, first_non_integer_variable_index, False), depth + 1))
        nodes.append((insert_constraint(T, selected_variables, first_non_integer_variable_index, True), depth + 1))

    if solution[-1] is None:
        raise Exception("The problem has no solution.")

    return solution[1], objective_function, solution[0], solution[2]


def main():
    if len(sys.argv) != 3:
        print("Usage: python3 simplex.py [float|integer] [FILE]", file=sys.stderr)
        sys.exit(1)

    mode = sys.argv[1]

    if mode != "float" and mode != "integer":
        print('The value of the mode must be either "float" or "integer".', file=sys.stderr)
        sys.exit(1)

    try:
        start_time = time.time()

        if mode == "float":
            T, objective_function, objective_function_value, selected_variables = find_solution(sys.argv[2])
        elif mode == "integer":
            T, objective_function, objective_function_value, selected_variables = find_integer_solution(sys.argv[2])

        execution_time = time.time() - start_time

        print_solution(T, objective_function, objective_function_value, selected_variables)
        print("Nombre de pivots : " + str(statistics_pivot_count))

        if mode == "integer":
            print("Nombre de noeuds traités : " + str(statistics_processed_node_count))
            print("Nombre de noeuds dominés : " + str(statistics_ignored_node_count))
            print("Profondeur de l'arbre : " + str(statistics_tree_depth))

        print("Temps d'exécution (sans l'affichage) : %s ms" % int(execution_time * 1000))
    except Exception as e:
        print(e)


if __name__ == "__main__":
    np.set_printoptions(linewidth=10000, precision=3, suppress=True, threshold=10000)
    main()
