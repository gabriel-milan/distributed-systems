import os
import timeit

r = 1000
k = 0

for n in [2, 4, 8, 16, 32, 64, 128]:
  print ("n = {}, time(s) = {}".format(n, timeit.timeit('import os; os.system("../client/client {} {} {}")'.format(r, k, n), number=1)))
  os.system("mv resultado.txt sca2_n{}_r{}_k{}".format(n, r, k))