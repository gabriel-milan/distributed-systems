import matplotlib.pyplot as plt

X = [2, 4, 8, 16, 32, 64, 128]
y = [
  200.169,
  400.339,
  800.707,
  1601.553,
  3203.045,
  6405.874,
  12813.326,
]

plt.figure (figsize=(20,20))
plt.title ("Tempo vs. nº processos")
plt.plot(X, y)
plt.xlabel("Number of processes")
plt.ylabel("Execution time (s)")
plt.show()
