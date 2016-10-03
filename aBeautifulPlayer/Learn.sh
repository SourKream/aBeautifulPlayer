
g++ -O3 -std=c++11 main.cpp -o ReInforcedLearning
for i in {1..10}
do
./ReInforcedLearning 2> Logs/Error$i.txt 1> Logs/Output$i.txt
done
