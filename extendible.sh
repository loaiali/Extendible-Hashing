g++ extendible/extendible.cpp extendible/main.cpp -o bin/extendible.out

rm ./db/extendible_*
./bin/extendible.out tests/$1.txt