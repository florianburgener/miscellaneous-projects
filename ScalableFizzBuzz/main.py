from functools import reduce

VERSION = 3
N = 100

print(f"Executing version {VERSION}...")
rules = [{"number": 3, "text": "Fizz"}, {"number": 5, "text": "Buzz"}]

match VERSION:
    # Version 1:
    case 1:
        for i in range(1, N + 1):
            evals = [i % x["number"] == 0 for x in rules]

            if reduce(lambda acc, x: acc or x, evals):
                print(*[rules[i]["text"] for i, x in enumerate(evals) if x], sep="")
            else:
                print(i)
    # Version 2:
    case 2:
        for i in range(1, N + 1):
            evals = [i % x["number"] == 0 for x in rules]

            if any(evals):
                print(*[rules[i]["text"] for i, x in enumerate(evals) if x], sep="")
            else:
                print(i)
    # Version 3 (best version in terms of performance and readability):
    case 3:
        for i in range(1, N + 1):
            output = ""

            for rule in rules:
                if i % rule["number"] == 0:
                    output += rule["text"]
            
            if not output:
                output += str(i)

            print(output) 
    # Version 4 (more compact version 3):
    case 4:
        for i in range(1, N + 1):
            print("".join(r["text"] for r in rules if i % r["number"] == 0) or str(i))
    # Version 5 (even more compact version 3):
    case 5:
        print("\n".join([("".join(r["text"] for r in rules if i % r["number"] == 0) or str(i)) for i in range(1, N + 1)]))
