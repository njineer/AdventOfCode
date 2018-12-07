from itertools import count, islice

def sum_spiral():
    a, i, j = {(0,0) : 1}, 0, 0
    for s in count(1, 2):
        for (ds, di, dj) in [(0,1,0),(0,0,-1),(1,-1,0),(1,0,1)]:
            for _ in range(s+ds):
                i += di; j += dj
                a[i,j] = sum(a.get((k,l), 0) for k in range(i-1,i+2)
                                             for l in range(j-1,j+2))
                yield i,j,a[i,j]


for i,j,a in islice(sum_spiral(), 20):
    print("(i) {},\t(j) {},\t(a) {}".format(i,j,a))

