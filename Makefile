CC = g++ -std=c++11 -DBOOST_LOG_DYN_LINK -fPIC -DSFMT_MEXP=19937 

XFLAGS = -Wall #-Wshadow -Wstrict-prototypes -Wmissing-prototypes -DDEBUG -Wredundant-decls
OFLAGS = -O3 #-pthread -fopenmp

INCS_PATH = -I./include/  -I./bliss-0.73/ -I./SFMT-src-1.5.1/ -I./pcg-c-0.94/include/
CINCLUDES = -L/usr/lib/ -L./bliss-0.73/ -L./SFMT-src-1.5.1/ -L./pcg-c-0.94/src/
CLIBS = -lpcg_random -lsfmt -lgsl -lgslcblas -lm -lboost_program_options -lstdc++ -pthread -lbliss -lboost_log -lboost_thread -lboost_system -ltbb 
OBJS_ROOT = logging.o strmisc.o timemisc.o utils.o dataReader.o graph.o graphSetReader.o node.o edge.o random.o canonical.o BasicEmbedding.o EdgeInducedEmbedding.o VertexInducedEmbedding.o EmbeddingUtils.o SuperEmbedding.o BasicEmbeddingSpace.o BasicEmbeddingSpacePar.o  
OBJS = ${OBJS_ROOT}
CFLAGS = ${XFLAGS} ${OFLAGS} ${DXFLAGS} ${INCS_PATH} ${CINCLUDES}  
SUBDIRS = ./pcg-c-0.94 ./SFMT-src-1.5.1 ./bliss-0.73 
RGPMLIB = librgpm.a

all : $(SUBDIRS) $(RGPMLIB)

.PHONY: all $(SUBDIRS) $(RGPMLIB)

${SUBDIRS}:
	$(MAKE) -C $@

$(RGPMLIB): $(OBJS)
	ar cr $@ $^
	ranlib $@

%.o : %.cpp
	${CC} ${CFLAGS} -c $< -o $@     
        
${MAIN} : Main.cpp $(OBJS)
	${CC} ${CFLAGS} $(OBJS) Main.cpp ${CLIBS} -o $@ ${CLIBS}

${MAIN1} : rwTest.cpp $(OBJS)
	${CC} ${CFLAGS} $(OBJS) rwTest.cpp ${CLIBS} -o $@ ${CLIBS}

${MAIN2} : rwTradi.cpp $(OBJS)
	${CC} ${CFLAGS} $(OBJS) rwTradi.cpp ${CLIBS} -o $@ ${CLIBS}

${MAIN3} : rwIntra.cpp $(OBJS)
	${CC} ${CFLAGS} $(OBJS) rwIntra.cpp ${CLIBS} -o $@ ${CLIBS}

# todo: fix me
${MAIN5} : rwTarget.cpp $(OBJS)
	${CC} ${CFLAGS} $(OBJS) rwTarget.cpp ${CLIBS} -o $@ ${CLIBS}

${MAIN6} : rwQuery.cpp $(OBJS)
	${CC} ${CFLAGS} $(OBJS) rwQuery.cpp ${CLIBS} -o $@ ${CLIBS}

# todo: fix me
${MAIN10} : rwDominant.cpp $(OBJS)
	${CC} ${CFLAGS} $(OBJS) rwDominant.cpp ${CLIBS} -o $@ ${CLIBS}

${MAIN11} : genRandomGraph.cpp $(OBJS)
	${CC} ${CFLAGS} $(OBJS) genRandomGraph.cpp ${CLIBS} -o $@ ${CLIBS}

${MAIN12} : rwHighDegree.cpp $(OBJS)
	${CC} ${CFLAGS} $(OBJS) rwHighDegree.cpp ${CLIBS} -o $@ ${CLIBS}

${MAIN13} : rwNodePair.cpp $(OBJS)
	${CC} ${CFLAGS} $(OBJS) rwNodePair.cpp ${CLIBS} -o $@ ${CLIBS}

${MAIN14} : genPatternCode.cpp $(OBJS)
	${CC} ${CFLAGS} $(OBJS) genPatternCode.cpp ${CLIBS} -o $@ ${CLIBS}


.PHONY: debug
#debug: DXFLAGS = -g -pg 
debug: DXFLAGS = -g -pg -lefence
debug: all

.PHONY: clean
clean:
	rm -f *.o
	rm -f ${RGPMLIB}

