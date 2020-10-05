import os
import timeit

for N in [1, 2, 4, 16, 32]:
  print ("**************** N = {} ****************".format(N))
  for Np in [2, 4, 8, 16]:
    print ("(Np, Nc) = ({}, 1)".format(Np))
    print (timeit.timeit('import os; os.system("./main {} 1 {} >> /dev/null")'.format(Np, N), number=10)/10)
  for Nc in [2, 4, 8, 16]:
    print ("(Np, Nc) = (1, {})".format(Nc))
    print (timeit.timeit('import os; os.system("./main 1 {} {} >> /dev/null")'.format(Nc, N), number=10)/10)
