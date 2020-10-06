import matplotlib.pyplot as plt

data = {
  1: {
    (1, 1) : 6.274645810597576,
    (2, 1) : 6.468804593296954,
    (4, 1) : 6.574014662398258,
    (8, 1) : 6.604240996198496,
    (16,1) : 6.628235584497451,
    (1, 2) : 4.215760689496529,
    (1, 4) : 3.710197771602543,
    (1, 8) : 3.698875323397806,
    (1,16) : 3.7194846995000264,
  },
  2: {
    (1, 1) : 4.605861424998148,
    (2, 1) : 5.154702678503236,
    (4, 1) : 5.494890148902778,
    (8, 1) : 5.369842817797325,
    (16,1) : 5.338877402502112,
    (1, 2) : 2.6676929477020166,
    (1, 4) : 2.0312221530999524,
    (1, 8) : 2.373183822503779,
    (1,16) : 2.349348051799461,
  },
  4: {
    (1, 1) : 4.095674105000216,
    (2, 1) : 5.147023267904297,
    (4, 1) : 5.378732551896246,
    (8, 1) : 5.273013718699803,
    (16,1) : 5.275664518703707,
    (1, 2) : 2.3448924871976486,
    (1, 4) : 1.6711500748991965,
    (1, 8) : 1.977809672500007,
    (1,16) : 2.2132157785992606,
  },
  16: {
    (1, 1) : 4.090659649099689,
    (2, 1) : 5.202900149399648,
    (4, 1) : 5.432632463402115,
    (8, 1) : 5.324755800602725,
    (16,1) : 5.258976852503838,
    (1, 2) : 2.367942911700811,
    (1, 4) : 1.7973453193961177,
    (1, 8) : 2.2311260781018065,
    (1,16) : 2.2691946229024325,
  },
  32: {
    (1, 1) : 4.095506394904805,
    (2, 1) : 5.175703358295141,
    (4, 1) : 5.342044799099677,
    (8, 1) : 5.2985242502996694,
    (16,1) : 5.268460953800241,
    (1, 2) : 2.3813159354031086,
    (1, 4) : 1.7957859887974337,
    (1, 8) : 2.243157755996799,
    (1,16) : 2.279153504798887,
  },
}

# Generate line plot for producers
plt.figure (figsize=(20,20))
plt.title ("Tempo vs. #Produtores")
legend = []
for N in data.keys():
  X = [1, 2, 4, 8, 16]
  y = [data[N][(x,1)] for x in X]
  plt.plot(X, y)
  legend.append("N={}".format(N))
plt.legend(legend)
plt.show()

# Generate line plot for consumers
plt.figure (figsize=(20,20))
plt.title ("Tempo vs. #Consumidores")
legend = []
for N in data.keys():
  X = [1, 2, 4, 8, 16]
  y = [data[N][(1,x)] for x in X]
  plt.plot(X, y)
  legend.append("N={}".format(N))
plt.legend(legend)
plt.show()
