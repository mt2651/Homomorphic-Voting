# Homomorphic-Voting

I) Install libhcs:

git clone https://github.com/Tiehuis/libhcs.git

cmake .

make

sudo make install # Will install to /usr/local by default




II)To run this project, we need only need to link against libhcs and libgmp:

1) Installation voting

clang++ -o voting voting.cpp -lhcs -lgmp -lgmpxx


2) Installation time performance

clang++ -o time time.cpp -lhcs -lgmp -lgmpxx
