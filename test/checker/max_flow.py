from test.lib import cli, runner
from random import randint
from collections import defaultdict


def main():
    args = cli.args.parse_args()
    iter = args.iterations
    for _ in range(iter):
        stdin = []

        n = randint(1, 500)
        m = randint(1, 1000)
        stdin.append(f"{n} {m}")
        for _ in range(m):
            a = randint(1, n)
            while True:
                b = randint(1, n)
                if b != a:
                    break
            c = randint(1, 10**9)
            stdin.append(f"{a} {b} {c}")

        proc_ans, sol_ans = runner.run(args.program, args.solution, "\n".join(stdin))
        assert proc_ans == sol_ans


if __name__ == "__main__":
    main()
