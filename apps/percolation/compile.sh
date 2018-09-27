g++ -std=c++11 -Wall -DSFMT_MEXP=19937 -O3 -I./../../bliss-0.73/  -I./../../SFMT-src-1.5.1/ -I./../../pcg-c-0.94/include/ -I./../../include/ -L/usr/lib/ -L./../../   -c EmbeddingSpacePercolation.cpp -o EmbeddingSpacePercolation.o
g++ -std=c++11 -Wall -DSFMT_MEXP=19937 -O3 -I./../../bliss-0.73/  -I./../../SFMT-src-1.5.1/ -I./../../pcg-c-0.94/include/ -I./../../include/ -L/usr/lib/ -L./../../   -c EmbeddingSpacePercolationPar.cpp -o EmbeddingSpacePercolationPar.o
g++ -std=c++11 -DBOOST_LOG_DYN_LINK -fPIC -DSFMT_MEXP=19937  -Wall  -O3 -Wall -DSFMT_MEXP=19937 -O3 -I./../../bliss-0.73/  -I./../../SFMT-src-1.5.1/ -I./../../pcg-c-0.94/include/ -I./../../include/ -L/usr/lib/ -L./../../ -L./../../bliss-0.73/ -L./../../SFMT-src-1.5.1/ -L./../../pcg-c-0.94/src/ EmbeddingSpacePercolation.o EmbeddingSpacePercolationPar.o rwPerco.cpp -o perco  -lrgpm -lpcg_random -lsfmt -lm -lboost_program_options -lstdc++ -lbliss  -lboost_log -lboost_system -ltbb -lpthread
