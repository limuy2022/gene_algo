import os

urls = [
    "train-images-idx3-ubyte.gz",
    "train-labels-idx1-ubyte.gz",
    "t10k-images-idx3-ubyte.gz",
    "t10k-labels-idx1-ubyte.gz"
]

try:
    os.mkdir("../data")
except:
    pass
os.chdir("../data")

for i in urls:
    os.system("wget http://yann.lecun.com/exdb/mnist/%s" % i)
    os.system("gzip -d %s" % i)
