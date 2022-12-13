import sys
import functools

def cmplists(l, r):
    if isinstance (l, int) and isinstance(r, int):
        return l - r
    if isinstance(l, int):
        l = [l]
    elif isinstance(r, int):
        r = [r]
    for il, ir in zip(l, r):
        rv = cmplists(il, ir)
        if rv != 0:
            return rv
    return len(l) - len(r)

def part1(fname):
    with open(fname) as f:
        input = [ [ eval(item)  for item in pair.split("\n") ] for pair in [ datum.strip() for datum in f.read().split("\n\n") ] ]
    return sum([ idx + 1 for idx, pair in enumerate(input) if cmplists(*pair) <= 0 ])

def part2(fname):
    with open(fname) as f:
        input = [ eval(datum.strip()) for datum in f.read().split("\n") if datum ]
    k1 = [[2]]
    k2 = [[6]]
    input.append(k1)
    input.append(k2)
    s = sorted(input, key = functools.cmp_to_key(cmplists))

    return (s.index(k1) + 1) * (s.index(k2) + 1)


if __name__ == "__main__":
    print(f"part1: {part1('input.txt')}")
    print(f"part2: {part2('input.txt')}")
