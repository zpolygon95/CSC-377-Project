import argparse
import random
import os
import os.path as path


def main(args):
    inc = [
        f for f in os.listdir(args.include)
        if path.isfile(path.join(args.include, f)) and f not in args.out.name
    ]

    def random_inst():
        return random.choice(list(inst_set.values()))()

    def random_fork():
        return f'F 1\nR {random.choice(inc)}' if inc else random_inst()

    maxint = (1 << 16) - 1
    inst_set = {
        'S': lambda: f'S {random.randint(0, maxint)}',
        'A': lambda: f'A {random.randint(0, maxint)}',
        'D': lambda: f'D {random.randint(0, maxint)}',
        'B': lambda: 'B',
        'F': random_fork
    }
    last = '\nE\n'
    instructions = [random_inst() for i in range(args.lines)]
    args.out.write('\n'.join(instructions) + last)


if __name__ == '__main__':
    a = argparse.ArgumentParser()
    a.add_argument('-l', '--lines', nargs='?', type=int, default=16)
    a.add_argument('-i', '--include', nargs='?', default='programs')
    a.add_argument('out', type=argparse.FileType('w'))
    main(a.parse_args())
