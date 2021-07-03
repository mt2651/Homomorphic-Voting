# Homomorphic-Voting

Instalation libhcs:

git clone https://github.com/Tiehuis/libhcs.git
cmake .
make
sudo make install # Will install to /usr/local by default

To run this project, we need only need to link against libhcs and libgmp:
Instalation voting
clang++ -o voting voting.cpp -lhcs -lgmp -lgmpxx

Instalation time
clang++ -o time time.cpp -lhcs -lgmp -lgmpxx