import os
import timeit

n = 2
r = 10
k = 1

print (timeit.timeit('import os; os.system("../client/client {} {} {}")'.format(r, k, n), number=1))