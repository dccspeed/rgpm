#include "BasicEmbeddingSpace.h"
#include <gsl/gsl_math.h>
#include <gsl/gsl_cdf.h>
#include <unordered_map>
#include <limits>
#include <utils.h>
#include <omp.h>

template <class T, class A>
EmbeddingSpace<T,A>::EmbeddingSpace(int s, Graph *g):currEmbedding(g), emb_neighbor_cacheg(MAX_CACHE_SIZE), INITIAL_EMBEDDING(g) {  
	srand (time(NULL));
	size=s;
	this->g = g;
	snSize = 1;
	isTransitive=false;  
	isSingleGroup=false;
	numFlushes=0; 
	setStackSize();
};

template <class T, class A>
EmbeddingSpace<T,A>::EmbeddingSpace(int s, int sns, Graph *g):EmbeddingSpace(s,g) { 
	snSize = sns;
}

template <class T, class A>
EmbeddingSpace<T,A>::EmbeddingSpace(int s, int sns, Graph *g, std::string conf):EmbeddingSpace(s,sns,g) {
	Config::load(conf);
	Config::print();
	checkConfigFile();
	emb_neighbor_cacheg.setCapacity(MAX_CACHE_SIZE);
}

template <class T, class A>
void EmbeddingSpace<T,A>::checkConfigFile() {
	if (Config::existKey(std::string("MAX_EX_STEPS"))) 
		MAX_EX_STEPS = Config::getKeyAsInt(std::string("MAX_EX_STEPS"));
	if (Config::existKey(std::string("MAX_EX_BUFF"))) 
		MAX_EX_BUFF = Config::getKeyAsInt(std::string("MAX_EX_BUFF"));
	if (Config::existKey(std::string("MAX_RW_STEPS"))) 
		MAX_RW_STEPS = Config::getKeyAsInt(std::string("MAX_RW_STEPS"));
	if (Config::existKey(std::string("INIT_RW_STEPS"))) 
		INIT_RW_STEPS = Config::getKeyAsInt(std::string("INIT_RW_STEPS"));
	if (Config::existKey(std::string("MAX_NUM_TOURS"))) 
		MAX_NUM_TOURS = Config::getKeyAsInt(std::string("MAX_NUM_TOURS"));
	if (Config::existKey(std::string("MAX_TOUR_STEPS"))) 
		MAX_TOUR_STEPS = Config::getKeyAsInt(std::string("MAX_TOUR_STEPS"));
	if (Config::existKey(std::string("MAX_INIT_ATTEMPT"))) 
		MAX_INIT_ATTEMPT = Config::getKeyAsInt(std::string("MAX_INIT_ATTEMPT"));
	if (Config::existKey(std::string("MAX_CACHE_SIZE"))) 
		MAX_CACHE_SIZE = Config::getKeyAsInt(std::string("MAX_CACHE_SIZE"));
	if (Config::existKey(std::string("MAX_AGG_SIZE"))) 
		MAX_AGG_SIZE = Config::getKeyAsInt(std::string("MAX_AGG_SIZE"));
	if (Config::existKey(std::string("SET_TOUR"))) 
		SET_TOUR = Config::getKeyAsBoolean(std::string("SET_TOUR"));
	if (Config::existKey(std::string("SET_SUPERNODE_RW"))) 
		SET_SUPERNODE_RW = Config::getKeyAsBoolean(std::string("SET_SUPERNODE_RW"));
	if (Config::existKey(std::string("RW_NO_RETURN"))) 
		RW_NO_RETURN = Config::getKeyAsBoolean(std::string("RW_NO_RETURN"));
	if (Config::existKey(std::string("SET_QLTY_ANALYSIS"))) 
		SET_QLTY_ANALYSIS = Config::getKeyAsBoolean(std::string("SET_QLTY_ANALYSIS"));
	if (Config::existKey(std::string("INITIAL_NODE"))) 
		INITIAL_NODE = Config::getKeyAsInt(std::string("INITIAL_NODE"));
	if (Config::existKey(std::string("INITIAL_PAIR_NODES"))) { 
		INITIAL_PAIR_NODES = Config::getKeyAsIntPair(std::string("INITIAL_PAIR_NODES"));
		std::cout << INITIAL_PAIR_NODES.first << " BLAA " << INITIAL_PAIR_NODES.second << std::endl;
	}
	if (Config::existKey(std::string("INITIAL_EMBEDDING"))) { 
		std::string INITIAL_EMBEDDING_STRING = Config::getKeyAsString(std::string("INITIAL_EMBEDDING"));
		INITIAL_EMBEDDING.loadFromString(INITIAL_EMBEDDING_STRING);
	}
	if (Config::existKey(std::string("INPUT_EMB_FILE"))) {
		std::string filename = Config::getKeyAsString(std::string("INPUT_EMB_FILE"));
		INPUT_EMB_FILE.open(filename, std::ifstream::in); 
		if (!INPUT_EMB_FILE.is_open()) {
			std::cout << "error: cant not open input embeddings file! " << filename << std::endl;
			exit(1);
		}
	}
	if (Config::existKey(std::string("OUTPUT_EMB_FILE"))) {
		std::string filename = Config::getKeyAsString(std::string("OUTPUT_EMB_FILE"));
		OUTPUT_EMB_FILE.open(filename, std::ofstream::out); 
		if (!OUTPUT_EMB_FILE.is_open()){ 
			std::cout << "error: cant not open output embeddings file! " << filename <<  std::endl;
			exit(1);
		}
	}
}


template <class T, class A>
bool EmbeddingSpace<T,A>::getValidEmbeddingBiased(T &e) {
	e.reset();
	//get init node
	srand(time(NULL));
	int n = e.getRandomWordBiased(); 
	std::cout << "init rw : first node " << n << std::endl;
	e.addWord(n);

	std::unordered_set<int> expansions = e.getValidElementsForExpansion();

	if (expansions.empty()) {
		std::cout << "expansions empty!" << std::endl;
	}

	while (!expansions.empty() && e.getNumWords() < size) {
		int n = Randness::instance().get_a_random_number(0, expansions.size());

		//get element 
		std::unordered_set<int>::iterator it = expansions.begin();
		for (int k = 0; k < n; k++)
			it++;

		std::cout << "init rw : new word " << *it << std::endl;
		e.addWord(*it);
		expansions = e.getValidElementsForExpansion();
		if (expansions.empty()) {
			std::cout << "expansions empty!" << std::endl;
		}
	}
	e.print();
	std::cout << "num words " << e.getNumWords() << " size " << size << std::endl;
	return e.getNumWords() == size;
}

template <class T, class A>
bool EmbeddingSpace<T,A>::getValidEmbedding() {
	currEmbedding.reset();
	//get init node
	srand(time(NULL));
	int n = Randness::instance().get_a_random_number(0, currEmbedding.getTotalNumWords());
	std::cout << "init rw : first node " << n << std::endl;
	currEmbedding.addWord(n);

	std::unordered_set<int> expansions = currEmbedding.getValidElementsForExpansion();

	if (expansions.empty()) {
		std::cout << "expansions empty!" << std::endl;
	}

	while (!expansions.empty() && currEmbedding.getNumWords() < size) {
		int n = Randness::instance().get_a_random_number(0, expansions.size());

		//get element 
		std::unordered_set<int>::iterator it = expansions.begin();
		for (int k = 0; k < n; k++)
			it++;

		std::cout << "init rw : new word " << *it << std::endl;
		currEmbedding.addWord(*it);
		expansions = currEmbedding.getValidElementsForExpansion();
		if (expansions.empty()) {
			std::cout << "expansions empty!" << std::endl;
		}
	}
	currEmbedding.print();
	std::cout << "num words " << currEmbedding.getNumWords() << " size " << size << std::endl;
	return currEmbedding.getNumWords() == size;
}

template <class T, class A>
bool EmbeddingSpace<T,A>::getValidEmbeddingWithNode(int n) {
        currEmbedding.reset();
        //get init node
        if (n >= (int) currEmbedding.getTotalNumWords() || n < 0) {
        	std::cout << "error: init rw invalid first node: " << n << std::endl;
		exit(1);
	}
        std::cout << "init rw : first node " << n << std::endl;
        currEmbedding.addWord(n);

        std::unordered_set<int> expansions = currEmbedding.getValidElementsForExpansion();

        if (expansions.empty()) {
                std::cout << "expansions empty!" << std::endl;
        }

        while (!expansions.empty() && currEmbedding.getNumWords() < size) {
                int n = Randness::instance().get_a_random_number(0, expansions.size());

                //get element 
                std::unordered_set<int>::iterator it = expansions.begin();
                for (int k = 0; k < n; k++)
                        it++;

                std::cout << "init rw : new word " << *it << std::endl;
                currEmbedding.addWord(*it);
                expansions = currEmbedding.getValidElementsForExpansion();
                if (expansions.empty()) {
                        std::cout << "expansions empty!" << std::endl;
                }
        }
        currEmbedding.print();
        std::cout << "num words " << currEmbedding.getNumWords() << " size " << size << std::endl;
        return currEmbedding.getNumWords() == size;
}

template <class T, class A>
bool EmbeddingSpace<T,A>::getValidEmbeddingWithNodes(std::vector<int> &ns) {
        currEmbedding.reset();

	for (int n : ns) { 
	        std::cout << "init rw : node " << n << std::endl;
        	currEmbedding.addWord(n);
	}

        std::unordered_set<int> expansions = currEmbedding.getValidElementsForExpansion();
        if (expansions.empty()) {
                std::cout << "expansions empty!" << std::endl;
        }

        while (!expansions.empty() && currEmbedding.getNumWords() < size) {
                int n = Randness::instance().get_a_random_number(0, expansions.size());

                //get element 
                std::unordered_set<int>::iterator it = expansions.begin();
                for (int k = 0; k < n; k++)
                        it++;

                std::cout << "init rw : new word " << *it << std::endl;
                currEmbedding.addWord(*it);
                expansions = currEmbedding.getValidElementsForExpansion();
                if (expansions.empty()) {
                        std::cout << "expansions empty!" << std::endl;
                }
        }
        currEmbedding.print();
        std::cout << "num words " << currEmbedding.getNumWords() << " size " << size << std::endl;
        return currEmbedding.getNumWords() == size;
}

template <class T, class A>
bool EmbeddingSpace<T,A>::init_rw() {
	currEmbedding.reset();
	int i = 0;
	bool r = false;
	
	if (!INITIAL_EMBEDDING.isEmpty()) {
		currEmbedding = INITIAL_EMBEDDING;
		if (isValidEmbedding(currEmbedding)) {
			return true;
		}
		else {
        		std::cout << "error: init rw invalid initial embedding: " << std::endl;
			currEmbedding.print();		
			exit(1);
		}
	} 

	if (INITIAL_NODE!=-1) {
		while (i < MAX_INIT_ATTEMPT) {
			if (getValidEmbeddingWithNode(INITIAL_NODE)) {
				r = true;
				break;
			}
			i++;
		}
	}
	else if (INITIAL_PAIR_NODES.first!=-1 && INITIAL_PAIR_NODES.second!=-1) {
		std::vector<int> path = g->getPathBetweenNodes(INITIAL_PAIR_NODES.first, INITIAL_PAIR_NODES.second, size);
		if ((int)path.size()!=0) {
			if (getValidEmbeddingWithNodes(path)) {
				r=true;
			}
		}
	}
	else {
		while (i < MAX_INIT_ATTEMPT) {
			if (getValidEmbedding()) {
				r = true;
				break;
			}
			i++;
		}
	}

	if (r) {
		//search for a better initial embedding.
		for(int i = 0; i < INIT_RW_STEPS; i++) {
			if (i%10000==0) std::cout << "init rw step: " << i << std::endl;
			std::pair<Mod,bool> mod = getNextRandomModification(currEmbedding);
			//std::cout << "bla: " << currEmbedding << " " << mod.first << std::endl; 
			if (!mod.second) {
				std::cout << "warning: no neighbors were found! aborting..." << std::endl;
				return r;
			}
			//std::cout << "ble: " <<  currEmbedding << " " << mod.first << std::endl; 
			currEmbedding.replaceWord(mod.first.rmId,mod.first.addId);
			//if emb is not valid, return to the previous one.	
			if (!isValidEmbedding(currEmbedding)) {
				currEmbedding.replaceWord(mod.first.addId,mod.first.rmId);
				i++; //counts as a step
			}
		}
	}

	return r;
}

template <class T, class A>
void EmbeddingSpace<T,A>::getAllEmbeddingsRec(T &e, int wordId, std::vector<T> &embs) {
	e.addWord(wordId);

	if (e.getNumWords()==size && !filterEmbedding(e)) {
		//e.print();
		embs.push_back(e);
	}
	else {
		std::unordered_set<int> expansions = e.getValidElementsForExpansion();
		for (std::unordered_set<int>::iterator it = expansions.begin(); it!= expansions.end(); it++) {	
			if (e.isCanonicalEmbeddingWithWord(*it)) {
				getAllEmbeddingsRec(currEmbedding, *it, embs);
			}
		}
	}
	e.removeLastWord();		
}


template <class T, class A>
std::vector<T> EmbeddingSpace<T,A>::getAllEmbeddings() {
	std::vector<T> embs;
	currEmbedding.reset();
	int numWords = currEmbedding.getTotalNumWords();
	for (int i = 0; i < numWords; i++) {
		getAllEmbeddingsRec(currEmbedding, i, embs);
	}
	return embs;
}

template <class T, class A> 
void EmbeddingSpace<T,A>::updateGroupStats(T &e, T &eRef, TourStats &stats) {
	std::vector<int> groupKeys = getGroupKeys(e, eRef);
	double degree = getEmbeddingDegree(eRef);
	double degreeGroup = degree;
	//std::cout << "Update stats => degree: " << degree << " degreeGroup: " << degreeGroup << std::endl;
	//e.print();
	//eRef.print();
	
	if (degreeGroup <= 0) {
		std::cout << "error:embedding degree is not valid!" << std::endl;
		exit(1);
	}
	for (int i = 0; i < (int) groupKeys.size(); i++) {
		std::unordered_map<int, int>::iterator it = stats.groups.find(groupKeys[i]);
		if (it==stats.groups.end()) {
			int nextKey = stats.groups.size();
			stats.groups.insert(std::make_pair(groupKeys[i], nextKey));
			stats.groupsDegree.push_back(degree/degreeGroup);				
			stats.groupsSize.push_back(1./degreeGroup);
		} else {
			stats.groupsDegree[it->second]+=degree/degreeGroup;				
			stats.groupsSize[it->second]+=1./degreeGroup;
		}
	}
}

template <class T, class A> 
void EmbeddingSpace<T,A>::updateGroupStatsExact(T &e, T &eRef, TourStats &stats) {
	size_t ecode = eRef.getHash(); 
	std::vector<int> groupKeys = getGroupKeys(e, eRef);
	double degree = getEmbeddingDegree(eRef);
	//double degreeGroup = degree;
	//std::cout << "Update stats => degree: " << degree << " degreeGroup: " << degreeGroup << std::endl;
	//std::cout << "Update stats => degree: " << e << " " << eRef << std::endl;
	
	for (int i = 0; i < (int) groupKeys.size(); i++) {
		std::unordered_map<int, int>::iterator it = stats.groups.find(groupKeys[i]);
		if (it==stats.groups.end()) {
			int nextKey = stats.groups.size();
			stats.groups.insert(std::make_pair(groupKeys[i], nextKey));
			stats.groupsDegree.push_back(degree);				
			stats.groupsSize.push_back(1.);
			stats.groupsOwner.push_back(ecode);
		} else {
			stats.groupsDegree[it->second]+=degree;				
			stats.groupsSize[it->second]+=1.;
			if (ecode>stats.groupsOwner[it->second])	
				stats.groupsOwner[it->second]=ecode;
		}
	}
}

template <class T, class A>
void EmbeddingSpace<T,A>::getExactGroupStatsBFS(T &e, TourStats &stats, std::set<size_t> &reach){

	std::queue<T> q;
	q.push(e);
	size_t ecode = e.getHash(); 
	reach.insert(ecode);

	while (!q.empty()) {
		std::vector<Mod> mods = computeAllEmbeddingNeighborhood(q.front());
		T embeddingCopy = q.front();

		//std::cout << "checking emb." << embeddingCopy << std::endl;
		for (int i = 0; i < (int) mods.size(); i++) {
			stats.steps+=1;
			//for sake of safety
			if (stats.steps >= MAX_EX_STEPS) { 
				std::cout << "warning: number of steps is too high." << std::endl;
				stats.ret = false;
				return;
			}

			if ( (int) reach.size() >= MAX_EX_BUFF) { 
				std::cout << "warning: memory issue!" << reach.size() << " embeddings reached." << std::endl;
				stats.ret = false;
				return;
			}

			//processing the neighbor
			embeddingCopy.replaceWord(mods[i].rmId, mods[i].addId);	
			//std::cout << "--checking neibh." << embeddingCopy << std::endl;
			size_t ecode = embeddingCopy.getHash(); 
			if (reach.find(ecode) != reach.end()) {
				embeddingCopy.replaceWord(mods[i].addId, mods[i].rmId);	
				continue;
			}
			reach.insert(ecode);
			//std::cout << "new emb." << std::endl;

			bool sameGroup = false;
			bool isValid=false;
			
			if (!isTransitive) {
				sameGroup = isSameGroup(e, embeddingCopy);
				if (sameGroup) updateGroupStatsExact(e, embeddingCopy, stats);
				isValid = isValidEmbeddingGroup(e, embeddingCopy);
			}	
			else { 
				sameGroup = isSameGroup(q.front(), embeddingCopy);
				if (sameGroup) updateGroupStatsExact(q.front(), embeddingCopy, stats);
				isValid = isValidEmbeddingGroup(q.front(), embeddingCopy);
			}
			if (sameGroup || isValid ) {
				T embQueue = embeddingCopy;
				q.push(embQueue);
				//std::cout << "add queue." << std::endl;
			}
			embeddingCopy.replaceWord(mods[i].addId, mods[i].rmId);	
		}
		q.pop();
	}
}

template <class T, class A>
//void EmbeddingSpace<T,A>::getExactGroupStatsDFS(T &e, T &eRef, TourStats &stats, NeighborhoodSet &reach, bool &ret){
void EmbeddingSpace<T,A>::getExactGroupStatsDFS(T &e, T &eRef, TourStats &stats, std::set<size_t> &reach){
	size_t ecode = eRef.getHash(); 
	stats.steps+=1;
	if (reach.find(ecode) != reach.end()) return;

	if (stats.steps >= MAX_EX_STEPS) { 
		std::cout << "warning: number of steps is too high." << std::endl;
		stats.ret = false;
		return;
	}

	if ( (int) reach.size() >= MAX_EX_BUFF) { 
		std::cout << "warning: memory issue!" << reach.size() << " embeddings reached." << std::endl;
		stats.ret = false;
		return;
	}

	bool sameGroup = false;
	bool isValid=false;

	sameGroup = isSameGroup(e, eRef);
	isValid = isValidEmbeddingGroup(e, eRef);

	if (sameGroup || isValid) {
		reach.insert(ecode);
		if (sameGroup) { 
			updateGroupStatsExact(e, eRef, stats);
			//std::cout << "- SAME GROUP - " << std::endl;
			//e.print();
			//eRef.print();
			//std::cout << "- - - - -" << std::endl;
		}
		std::vector<Mod> mods = computeAllEmbeddingNeighborhood(eRef);
		T embeddingCopy = eRef;
		for (int i = 0; i < (int) mods.size(); i++) {
			embeddingCopy.replaceWord(mods[i].rmId, mods[i].addId);	
			if (!isTransitive) {
				getExactGroupStatsDFS(e, embeddingCopy, stats, reach);	
				if (!stats.ret) return;
			}
			else {
				getExactGroupStatsDFS(eRef, embeddingCopy, stats, reach);	
				if (!stats.ret) return;
			}
			embeddingCopy.replaceWord(mods[i].addId, mods[i].rmId);	
		}
	}
}

template <class T, class A>
TourStats EmbeddingSpace<T,A>::getExactGroupStats(T &e){
	//std::cout << "Getting Exact Stats: " << e << std::endl;
	TourStats stats;	
	stats.steps=1;
	//std::unordered_set<size_t> reach;
	//NeighborhoodSet reach;
	std::set<size_t>  reach;

	size_t ecode = e.getHash(); 
	reach.insert(ecode);
	std::vector<Mod> mods = computeAllEmbeddingNeighborhood(e);

	if (isSingleGroup) {
		//add the own embedding as a group
		std::vector<int> keys = getGroupKeys(e, e);
		for (int i = 0; i < (int) keys.size(); i++) {
			int nextKey = stats.groups.size();
			stats.groups.insert(std::make_pair(keys[i], nextKey));
			stats.groupsDegree.push_back((double) mods.size());				
			stats.groupsSize.push_back(1.);				
			stats.groupsOwner.push_back(ecode);
		}
		return stats;
	}

	//DFS
	/*T embeddingCopy = e;
	for (int i = 0; i < (int) mods.size(); i++) {
		embeddingCopy.replaceWord(mods[i].rmId, mods[i].addId);	
		getExactGroupStatsDFS(e, embeddingCopy, stats, reach);
		if (!stats.ret) break;
		embeddingCopy.replaceWord(mods[i].addId, mods[i].rmId);	
	}
	if (!stats.empty()) {	
		//add the own embedding in current groups
		for (std::unordered_map<int, int>::iterator it = stats.groups.begin(); it!=stats.groups.end(); it++) {
			stats.groupsDegree[it->second]+=(double) mods.size();
			stats.groupsSize[it->second]+=1.;
			if (ecode>stats.groupsOwner[it->second])        
				stats.groupsOwner[it->second]=ecode;
		}
	}
	else {
		//add the own embedding as a group
		std::vector<int> keys = getGroupKeys(e, e);
		for (int i = 0; i < (int) keys.size(); i++) {
			int nextKey = stats.groups.size();
			stats.groups.insert(std::make_pair(keys[i], nextKey));
			stats.groupsDegree.push_back((double) mods.size());				
			stats.groupsSize.push_back(1.);				
			stats.groupsOwner.push_back(ecode);
		}
	}
	reach.clear();
	std::cout << "DFS: " << stats << std::endl;
	stats.reset();	*/


	//BFS
	getExactGroupStatsBFS(e, stats, reach);
        if (!stats.empty()) {
                //add the own embedding in current groups
                for (std::unordered_map<int, int>::iterator it = stats.groups.begin(); it!=stats.groups.end(); it++) {
                        stats.groupsDegree[it->second]+=(double) mods.size();
                        stats.groupsSize[it->second]+=1.;
                        if (ecode>stats.groupsOwner[it->second])
                                stats.groupsOwner[it->second]=ecode;
                }
        }
        else {
                //add the own embedding as a group
                std::vector<int> keys = getGroupKeys(e, e);
                for (int i = 0; i < (int) keys.size(); i++) {
                        int nextKey = stats.groups.size();
                        stats.groups.insert(std::make_pair(keys[i], nextKey));
                        stats.groupsDegree.push_back((double) mods.size());
                        stats.groupsSize.push_back(1.);
                        stats.groupsOwner.push_back(ecode);
                }
        }

	//std::cout << "BFS: " << stats << std::endl;

	LOG(debug) << "EXFINAL: " << e << " " << stats;

	return stats;
}

template <class T, class A>
void EmbeddingSpace<T,A>::filterGroupStatsByOwner(T &e, TourStats &stats){
	size_t ecode = e.getHash(); 
	for (std::unordered_map<int, int>::iterator it = stats.groups.begin(); it!=stats.groups.end();) {
		if (ecode!=stats.groupsOwner[it->second])
			it = stats.groups.erase(it); 
		else it++;
	}
}

template <class T, class A>
void EmbeddingSpace<T,A>::expand_embedding_group(T &e, int wordId, int *nValid, int *nTotal) {
	e.addWord(wordId);
	if (e.getNumWords()==size) {
		(*nTotal)++;
		if ((*nTotal)%1000==0) {
			std::cout << *nTotal << " embeddings were produced! " << *nValid << " embeddings are valid!" << std::endl;
			//agg.print();
		}

		if (!filterEmbedding(e)) {

		TourStats stats = getExactGroupStats(e);
		filterGroupStatsByOwner(e, stats);
		if (!stats.empty()) aggregateEmbeddingClass(e, stats, agg);
		(*nValid)++;

		if ((*nValid)%1000==0) {
			std::cout << *nValid << " embeddings were processed!" << std::endl;
		}
		if ((*nValid)%10000==0) {
			std::cout << "EXSTEP: " << "{ processed: " << *nValid << " patts: " << agg.getMapSize() << " }"<< std::endl;
			//agg.print();
		}

#ifdef FLUSH_AGGREGATOR
		if (MAX_AGG_SIZE == agg.getMapSize()) { 
			std::string output = aggOutput + std::to_string(numFlushes);
			//std::cout << "---FLUSH to file--- " << output << std::endl;
			std::ofstream os (output, std::ofstream::out);
			agg.flush(os);
			numFlushes++;
			os.close();
			//std::cout << "-----------------" << output << std::endl;
			agg.clear();
		}
#endif
		}
	}
	else {
		std::unordered_set<int> expansions = e.getValidElementsForExpansion();
		for (std::unordered_set<int>::iterator it = expansions.begin(); it!= expansions.end(); it++) {	
			if (e.isCanonicalEmbeddingWithWord(*it)) {
				expand_embedding_group(currEmbedding, *it, nValid, nTotal);
			}
		}
	}
	e.removeLastWord();		
}


template <class T, class A>
void EmbeddingSpace<T,A>::run_exact_recursive() {
	currEmbedding.reset();
	int numWords = currEmbedding.getTotalNumWords();
	int nValid = 0;
	int nTotal = 0;
	for (int i = 0; i < numWords; i++) {
		expand_embedding_group(currEmbedding, i, &nValid, &nTotal);
	}

#ifdef FLUSH_AGGREGATOR
	if (agg.getMapSize()!=0) { 
		std::string output = aggOutput + std::to_string(numFlushes);
		//std::cout << "---FLUSH to file--- " << output << std::endl;
		std::ofstream os (output, std::ofstream::out);
		agg.flush(os);
		numFlushes++;
		os.close();
		//std::cout << "-----------------" << output << std::endl;
		agg.clear();
	}
#endif

	std::cout << "EXSTEPFINAL: " << nTotal << " valid: " << nValid << " filtered: " << nTotal-nValid-1 << " NUM PATTs: " << agg.getMapSize() << std::endl;
	std::cout.setf(std::ios::fixed);
	agg.print();
} 

/*template <class T, class A>
  void EmbeddingSpace<T,A>::run_rw_query(Graph &pattern) {
  pattern_to_supp.clear();

  if (!init_rw()) {
  std::cout << "init fail!" << std::endl;
  return;
  }

  size_t patternCode = pattern.getBlissCodeHashValue();
  std::cout << "Searching for pattern: \n";
  pattern.print();
  std::cout << "Pattern Code: " << patternCode <<  "\n";

  double factorPositivePartial = 0.0;
  double factorPositive = 0.0;

//TODO: fix
SuperEmbedding<T> se;//= create(currEmbedding, snSize);
std::pair<T, bool> randomNeigh = getRandomNeighborSN(se);

double snConst = 0;
std::vector<T> embs = se.getEmbeddings();
for (int i = 0; i < embs.size(); i++) {
size_t currPattern =  embs[i].getBlissCodeHashValue();
if (currPattern==patternCode) {
snConst += 1.0;
}
}
std::cout << "SN constant: " << snConst << std::endl;

if (!randomNeigh.second) {
std::cout << "Impossible to find a neighbor of SN!" <<std::endl;
exit(1);
}	

currEmbedding = randomNeigh.first;

// steps of one tour
Uint it = 0;
Uint itTotal = 0;
Uint numTours = 0;
do {
size_t currPattern =  currEmbedding.getBlissCodeHashValue();
if(currPattern == patternCode) {
int degree = getNumberOfNeighbors(currEmbedding);
factorPositivePartial += 1.0/(double)degree;
factorPositive += 1000.0/(double)degree;
}

it++;
itTotal++;
// modification to current embedding
std::pair<Mod,double> mod = getNextRandomModification(currEmbedding);
currEmbedding.addWord(mod.first.addId);
currEmbedding.removeWord(mod.first.rmId);

//std::cout << "next mod " << mod.first.addId << " " << mod.first.rmId << std::endl;	
if (se.hasEmbedding(currEmbedding)) {
randomNeigh = getRandomNeighborSN(se);
if (!randomNeigh.second) {
std::cout << "Impossible to find a neighbor of SN!" <<std::endl;
exit(1);
}	
currEmbedding = randomNeigh.first;

std::cout.setf(std::ios::fixed);
std::cout << "@RW Tour number: " << numTours << " steps: " << it << " totalsteps: " << itTotal <<  std::endl;
numTours++;
double f1Partial = (double)se.getNumberOfExternalEdges()*factorPositivePartial;
double f1 = (double)se.getNumberOfExternalEdges()/((double)numTours)*factorPositive;
std::cout << "@Pattern estimated frequency tour: " << f1Partial << std::endl;
std::cout << "@Pattern estimated frequency tour with const: " << f1Partial + snConst << std::endl;
std::cout << "@Pattern estimated frequency all tours: " << f1/1000. << std::endl;
std::cout << "@Pattern estimated frequency all tours with const: " <<  f1/1000. + snConst << std::endl;
it=0;
factorPositivePartial=0;
}
if (itTotal%10000==0)
	std::cout << "RW Tour number: " << numTours << " steps: " << it << " totalsteps: " << itTotal <<  std::endl;
	} while (numTours < MAX_RW_STEPS);
}*/

template <class T, class A>
bool EmbeddingSpace<T,A>::getNextValidEmbeddingFromFile(T &embedding) {
	embedding.reset();
	if (!INPUT_EMB_FILE.is_open()) {
		std::cout << "warning: input embedding file is not opened! filename: " << Config::getKeyAsString(std::string("INPUT_EMB_FILE")) <<  std::endl;
		return false;
	}
	std::string line;
	if (!std::getline(INPUT_EMB_FILE, line)) {
		std::cout << "warning: there is no lines in input embedding file! filename: " << Config::getKeyAsString(std::string("INPUT_EMB_FILE")) << std::endl;
		return false;
	}
	embedding.loadFromString(line);	

	return true;
}

template <class T, class A>
bool EmbeddingSpace<T,A>::getNextValidEmbeddingRW(T &embedding) {
	std::pair<Mod,bool> mod = getNextRandomModification(embedding);
	if (!mod.second) {
		std::cout << "warning: no neighbors were found! aborting..." << std::endl;
		return false;
	}
	currEmbedding.replaceWord(mod.first.rmId,mod.first.addId);
	//if emb is not valid, return to the previous valid one.	
	if (!isValidEmbedding(currEmbedding)) {
		currEmbedding.replaceWord(mod.first.addId,mod.first.rmId);
	}

	return true;
}

template <class T, class A>
bool EmbeddingSpace<T,A>::getNextValidEmbedding(T &embedding) {
	if (INPUT_EMB_FILE.is_open()) {
		return getNextValidEmbeddingFromFile(embedding);
	}

	return getNextValidEmbeddingRW(embedding);
}

template <class T, class A>
void EmbeddingSpace<T,A>::run_rw() {
	if (!Config::existKey(std::string("INPUT_EMB_FILE"))) {
		if (!init_rw()) {
			std::cout << "init fail!" << std::endl;
			return;
		}
		std::cout << "init function has finished\n"; 
	}
	else {
		std::cout << "init function is not necessary! reading embs from file.\n"; 
	}	

	int nValid = 0;
	int i = 0;
	while (i < MAX_RW_STEPS) {
		i++;
		if (i%1000==0) {
			std::cout << i << " embeddings were produced! " << nValid << " embeddings are valid!" << std::endl;
		}

		if (filterEmbedding(currEmbedding)) continue;
		if (OUTPUT_EMB_FILE.is_open()) currEmbedding.writeWordsToFile(OUTPUT_EMB_FILE);		

		nValid++;
		if (nValid%1000==0) {
			std::cout << nValid << " embeddings were processed!" << std::endl;
		}
		if (nValid%10000==0) {
			std::cout << "RWSTEP: " << "{ processed: " << nValid << " patts: " << agg.getMapSize() << " }"<< std::endl;
			std::string output = this->aggOutput;
			std::ofstream os (output, std::ofstream::out | std::ofstream::app);
			os << "RWSTEP: " << "{ processed: " << nValid << " patts: " << agg.getMapSize() << " }"<< std::endl;
			this->agg.flush(os);
			os.close();
		}

		if (SET_QLTY_ANALYSIS) {
			groupQualityAnalysis(currEmbedding);
			continue;	
		}

		TourStats r;
		if (SET_TOUR) {
			//create supernode
			if (SET_SUPERNODE_RW) {
				SuperEmbedding<T> se = createSNTopEmbeddingsGroup(currEmbedding, snSize);
				LOG(debug) << se;	
				r = getGroupStats(currEmbedding, se, MAX_NUM_TOURS);
			}
			else  {
				SuperEmbedding<T> se = createBFSGroup(currEmbedding, snSize);
				LOG(debug) << se;	
				r = getGroupStats(currEmbedding, se, MAX_NUM_TOURS);
			}
		}
		else {
			r = getExactGroupStats(currEmbedding);
		}

		if (!r.ret) {
			std::cout << "Problem detected! over tour! " << std::endl;
			currEmbedding.print();
			exit(1);
		}

		aggregateEmbeddingClass(currEmbedding, r, agg);

#ifdef FLUSH_AGGREGATOR
		if (MAX_AGG_SIZE == agg.getMapSize()) { 
			std::string output = aggOutput + std::to_string(numFlushes);
			//std::cout << "---FLUSH to file--- " << output << std::endl;
			std::ofstream os (output, std::ofstream::out);
			agg.flush(os);
			numFlushes++;
			os.close();
			//std::cout << "-----------------" << output << std::endl;
			agg.clear();
		}
#endif
		if (!getNextValidEmbedding(currEmbedding)) break;
	}

#ifdef FLUSH_AGGREGATOR
	if (agg.getMapSize()!=0) { 
		std::string output = aggOutput + std::to_string(numFlushes);
		//std::cout << "---FLUSH to file--- " << output << std::endl;
		std::ofstream os (output, std::ofstream::out);
		agg.flush(os);
		numFlushes++;
		os.close();
		//std::cout << "-----------------" << output << std::endl;
		agg.clear();
	}
#endif
	std::cout << "RWSTEPFINAL: " << i << " valid: " << nValid << " filtered: " << i-nValid << " NUM PATTs: " << agg.getMapSize() << std::endl;

	std::cout.setf(std::ios::fixed);
	agg.print();
} 

template <class T, class A>
TourStats EmbeddingSpace<T,A>::getGroupStats(T &embedding, SuperEmbedding<T> &se, int nTours) {
#ifdef DEBUG 
	TourStats exactStats = getExactGroupStats(embedding);
	std::vector<int> exactGroupKeys;
	for ( auto& it : exactStats.groups) {
		exactGroupKeys.push_back(it.first);
	}
#endif
	LOG(debug) << "getting group stats for: " << embedding;

	if (se.isEmpty()) {
		std::cout << "SuperNode is empty!" << std::endl;
		exit(1);
	}

	embedding.setDegree(getEmbeddingDegree(embedding));
	double external = se.getExternalDegree();
	//std::cout << "External:" << se.getExternalDegree();

	//size_t pcode =  embedding.getBlissCodeHashValue();
	//size_t ecode = embedding.getHash();
	//std::cout << "Embedding Code: " << ecode<< " with Pattern: " << pcode << std::endl;
	//se.print();

	//const groups
	std::unordered_map<int, int> groupsConst;
	std::vector<double> groupsSizeConst;
	std::vector<double> groupsDegreeConst;

	//std::cout << "computing groups const" << std::endl;
	std::vector<T> &embs = se.getEmbeddings();
	if (!isTransitive) {
		for (int i = 0; i < (int) embs.size(); i++) {
			if (!embedding.isSameEmbedding(embs[i]) && isSameGroup(embedding, embs[i])) {
				std::vector<int> groupKeys = getGroupKeys(embedding, embs[i]);
				//double degree = getEmbeddingDegree(embs[i]);
				double degree = embs[i].getDegree();
				for (int i = 0; i < (int) groupKeys.size(); i++) {
					std::unordered_map<int, int>::iterator it = groupsConst.find(groupKeys[i]);
					if (it==groupsConst.end()) {
						int nextKey = groupsConst.size();
						groupsConst.insert(std::make_pair(groupKeys[i], nextKey));
						groupsDegreeConst.push_back(degree);
						groupsSizeConst.push_back(1.);				
					} 
					else {
						groupsDegreeConst[it->second]+=degree;
						groupsSizeConst[it->second]+=1.;
					}
				}
			}
		}
	}
	else {
		for (int i = 0; i < (int) embs.size(); i++) {
			if (!embedding.isSameEmbedding(embs[i])) {
				std::vector<int> groupKeys = getGroupKeys(embedding, embs[i]);
				//double degree = getEmbeddingDegree(embs[i]);
				double degree = embs[i].getDegree();
				for (int i = 0; i < (int) groupKeys.size(); i++) {
					std::unordered_map<int, int>::iterator it = groupsConst.find(groupKeys[i]);
					if (it==groupsConst.end()) {
						int nextKey = groupsConst.size();
						groupsConst.insert(std::make_pair(groupKeys[i], nextKey));
						groupsDegreeConst.push_back(degree);
						groupsSizeConst.push_back(1.);				
					} else {
						groupsDegreeConst[it->second]+=degree;
						groupsSizeConst[it->second]+=1.;
					}
				}
			}	
		}			
	}

#ifdef DEBUG
	//print for group analysis
	std::cout << "SN constant: " << std::endl;   
	std::cout.setf(std::ios::fixed);
	for (std::unordered_map<int, int>::iterator it = groupsConst.begin(); it!=groupsConst.end(); it++ ) {
		std::cout << "@GroupConst: [" << it->first;
		std::cout << "]" << " degree: " << groupsDegreeConst[it->second] << " size: " << groupsSizeConst[it->second] <<  std::endl;
	}
#endif

	TourStats finalTourStats = groupEstimateAllTours(embedding, se, nTours);


	//if the source embedding may not be in the subgraph class
	double sourceDegree = embedding.getDegree(); 
	double sourceSize = 1;
	if (!isSameGroup(embedding, embedding)) {
		sourceDegree = 0.;
		sourceSize = 0.;
	}

	//computing avg estimate
	for (std::unordered_map<int, int>::iterator it = finalTourStats.groups.begin(); it!=finalTourStats.groups.end(); it++) {
		finalTourStats.groupsDegree[it->second] = (double) external/(double)nTours*finalTourStats.groupsDegree[it->second] + sourceDegree;
		finalTourStats.groupsSize[it->second] = (double) external/(double)nTours*finalTourStats.groupsSize[it->second] + sourceSize;
		std::unordered_map<int, int>::iterator itC = groupsConst.find(it->first);
		if (itC != groupsConst.end()) {
			finalTourStats.groupsDegree[it->second]+=groupsDegreeConst[itC->second];
			finalTourStats.groupsSize[it->second]+=groupsSizeConst[itC->second];
		}
	}
	//computing for that ones with only const parts	
	for (std::unordered_map<int, int>::iterator itC = groupsConst.begin(); itC!=groupsConst.end(); itC++) {
		std::unordered_map<int, int>::iterator it = finalTourStats.groups.find(itC->first);
		if (it == finalTourStats.groups.end()) {
			int nextKey = finalTourStats.groups.size();
			finalTourStats.groups.insert(std::make_pair(itC->first, nextKey));
			finalTourStats.groupsDegree.push_back(groupsDegreeConst[itC->second] + sourceDegree);				
			finalTourStats.groupsSize.push_back(groupsSizeConst[itC->second] + sourceSize);
		}
	}
	//define group degree and size avgs
	if (finalTourStats.groups.size()==0) {
		std::vector<int> keys = getGroupKeys(embedding, embedding);
		for (int i = 0; i < (int) keys.size(); i++) {
			int nextKey = finalTourStats.groups.size();
			finalTourStats.groups.insert(std::make_pair(keys[i], nextKey));
			finalTourStats.groupsDegree.push_back(sourceDegree);				
			finalTourStats.groupsSize.push_back(sourceSize);				
		}
	}
#ifdef DEBUG	
	//print groups analysis
	std::cout << "Groups analysis: " << std::endl;
	std::cout.setf(std::ios::fixed);
	for (std::unordered_map<int, int>::iterator it = finalTourStats.groups.begin(); it!=finalTourStats.groups.end(); it++ ) {
		std::cout << "#PATT: " << code << " GroupFinal: [" << it->first;
		std::cout << "]" << " degree: " << finalTourStats.groupsDegree[it->second] << " size: " << finalTourStats.groupsSize[it->second] <<  std::endl;
	}
	std::cout << "PATT: " << code << " RW Tour Total Steps: " <<  finalTourStats.steps << std::endl;
#endif

#ifdef DEBUG
	std::cout << "stats estimate by tours." <<std::endl;
	finalTourStats.print();
#endif

	LOG(debug) << "FINAL: " << embedding << " " << se << " " << finalTourStats;

	return finalTourStats;
}

template <class T, class A>
TourStats EmbeddingSpace<T,A>::groupEstimateAllTours(T &embedding, SuperEmbedding<T> &se, int nTours) {
	//groups
	TourStats finalTourStats;

	if (se.getHasAll())
		return finalTourStats;

	for (int i = 0; i < nTours; i++) {
		TourStats stats = groupEstimateUsingTour(embedding, se, i);
		finalTourStats += stats;
	}

	return finalTourStats;
}

template <class T, class A>
TourStats EmbeddingSpace<T,A>::groupEstimateUsingTour(T embedding, SuperEmbedding<T> &se, int tourId) {
	TourStats tourStats;
	if (isSingleGroup || se.getHasAll()) {	
		return tourStats;
	}

	//std::cout << "Estimate group for embedding " << embedding << std::endl;
	//tour starts as invalid
	tourStats.ret = false;

	//get a random neighbor of the current SN (out of SN)
	std::pair<T, bool> randomNeigh = se.getBiasedEmbeddingWithExternalDegree();
	if (!randomNeigh.second) {
		std::cout << "Warning: impossible to find a neighbor of SN Group!" <<std::endl;
		tourStats.ret = true;	
		return tourStats;
	}

	T embeddingCopy, previousEmbedding;
	embeddingCopy = randomNeigh.first;
	previousEmbedding = randomNeigh.first;

	//get a valid previous node	
	/*std::vector<T> &embs = se.getEmbeddings();
	  int i = 0; 
	  while (i < (int) embs.size()) {
	  if (embs[i].isNeighbor(embeddingCopy)) {
	  previousEmbedding = embs[i];
	  break;
	  }
	  i++;
	  }
	  if (i == (int) embs.size()) {
	  std::cout << "ERROR: not neighbor of supernode!" << std::endl;
	  exit(1);
	  }

	  previousMod = embeddingCopy.getModificationTo(previousEmbedding);	
	  if (!previousMod.second) {
	  std::cout << "ERROR: previous modification not found!" << std::endl;
	  exit(1);
	  }*/

	/*mods = se.getModificationsToSN(embeddingCopy);
	  if (mods.empty()) {
	  std::cout << "ERROR: previous modification not found! " << embeddingCopy << std::endl;
	  std::_Exit( EXIT_FAILURE );
	  }
	  previousEmbedding.replaceWord(mods.begin()->rmId,mods.begin()->addId);*/

	ModSet mods = se.getModificationsToSN(randomNeigh.first);
	//std::cout << "mods to avoid: " << std::endl;
	//for (ModSet::iterator it = mods.begin(); it!=mods.end(); it ++) std::cout << *it << std::endl;

	std::pair<Mod, bool> mod = getNextRandomModification(randomNeigh.first, mods);
	embeddingCopy.replaceWord(mod.first.rmId,mod.first.addId);
	mods = ModSet({mod.first.inverse()});
	//std::cout << "random neigh out SN: " << embeddingCopy << std::endl;

	//TOUR
	int steps = 1;
	while (steps < MAX_TOUR_STEPS && !se.hasEmbedding(embeddingCopy)) {
		//std::cout << "----- step " << steps <<  " --------" << std::endl;
		//std::cout << "current emb in tour: " << embeddingCopy << std::endl;
		//std::cout << "mods to avoid: " << std::endl;
		//for (ModSet::iterator it = mods.begin(); it!=mods.end(); it ++) std::cout << *it << std::endl;
		//std::cout << "last mod: " << mod.first <<  std::endl;

		bool sameGroup = false;
		bool isValid=false;
		if (!isTransitive) {
			sameGroup = isSameGroup(embedding, embeddingCopy);
			isValid = isValidEmbeddingGroup(embedding, embeddingCopy);
		}
		else {
			sameGroup = isSameGroup(previousEmbedding, embeddingCopy);
			isValid = isValidEmbeddingGroup(previousEmbedding, embeddingCopy);
		}
		if (sameGroup) {
			if (!isTransitive) updateGroupStats(embedding, embeddingCopy, tourStats);
			else updateGroupStats(previousEmbedding, embeddingCopy, tourStats);
			LOG(debug) << "Tour: " << tourId << " Embedding: " << embeddingCopy << " Degree: " << getEmbeddingDegree(embeddingCopy) ;
		}

		if (!isValid) {
			//return to the previous
			embeddingCopy = previousEmbedding;
			mods = ModSet({mod.first});
			//std::cout << "emb is not valid! returning to the previous one! "<< std::endl;
			//std::cout << "next mods to avoid: " << std::endl;
			//for (ModSet::iterator it = mods.begin(); it!=mods.end(); it ++) std::cout << *it << std::endl;
		}
		else { // the user defined as a valid one but not from the group
			//next embedding
			if (RW_NO_RETURN==false) mod = getNextRandomModification(embeddingCopy);
			else mod = getNextRandomModification(embeddingCopy, mods);
			if (!mod.second) {
				std::cout << "error: no neighbors were found in group! aborting..." << std::endl;
				tourStats.ret = false;
				return tourStats;
			}
			previousEmbedding = embeddingCopy;
			mods = ModSet({mod.first.inverse()});
			embeddingCopy.replaceWord(mod.first.rmId,mod.first.addId);
			//std::cout << "emb is valid! going to the next one! "<< std::endl;
			//std::cout << "selected mod: " << mod.first << std::endl;
			//std::cout << "next mods to avoid: " << std::endl;
			//for (ModSet::iterator it = mods.begin(); it!=mods.end(); it ++) std::cout << *it << std::endl;
		}
		//std::cout << "next emb in tour: " << embeddingCopy << std::endl;
		//std::cout << "------------------" << std::endl;
		steps++;
	}

	tourStats.steps = steps;
	tourStats.ret = (steps < MAX_TOUR_STEPS);

#ifdef DEBUG
	std::cout << "---Estimating Stats---" <<std::endl;	
	tourStats.print();
	std::cout << "----------------" << std::endl;
#endif 

	LOG(debug) << "PARTIAL: " << embedding << " "<< tourStats;

	return tourStats;
}

template <class T, class A>
void EmbeddingSpace<T,A>::test() {

	if (!init_rw()) {
		std::cout << "init fail!" << std::endl;
		return;
	}

	//currEmbedding.reset();

	//author 10807 18514 6940
	//currEmbedding.addWord(10807);
	//currEmbedding.addWord(18514);
	//currEmbedding.addWord(6940);

	//author 14738 5845 5153 
	//currEmbedding.addWord(5845);
	//currEmbedding.addWord(14738);
	//currEmbedding.addWord(5153);

	//author 16326 12207 933 20975
	//currEmbedding.addWord(16326);		
	//currEmbedding.addWord(12207);		
	//currEmbedding.addWord(933);
	//currEmbedding.addWord(20975);

	//3872 3830 3821
	//2730 2729 2728 2483 
	//currEmbedding.addWord(2730);	
	//currEmbedding.addWord(2729);	
	//currEmbedding.addWord(2728);	
	//currEmbedding.addWord(2483);	

	//edge citeseer 3335 3050 2362
	//currEmbedding.addWord(3335);
	//currEmbedding.addWord(3050);
	//currEmbedding.addWord(2362);

	//vertex citeseer 2076 2066 2102		
	//vertex citeseer 2109 2096 2055
	//currEmbedding.addWord(2076);
	//currEmbedding.addWord(2066);
	//currEmbedding.addWord(2102);
	//currEmbedding.addWord(998); 
	//currEmbedding.addWord(28 );
	//currEmbedding.addWord(999); 
	//Embedding: { vertices: [ 14252 7656 58918 ] edges: [ 333069 207347 ] }
	//currEmbedding.addWord(14252);
	//currEmbedding.addWord(7656);
	//currEmbedding.addWord(58918);

	//Embedding: { vertices: [ 41693 332 77892 38195 ] edges: [ 704255 704404 12384 739669 ] }
	//currEmbedding.addWord(41693);
	//currEmbedding.addWord(332);
	//currEmbedding.addWord(77892);
	//currEmbedding.addWord(38195);

	//currEmbedding.addWord(7656); 
	//currEmbedding.addWord(66699);
	//currEmbedding.addWord(52588);
	//currEmbedding.addWord(46099);

	//vertex authors
	//currEmbedding.addWord(4655);
	//currEmbedding.addWord(354);
	//currEmbedding.addWord(10729);
	//currEmbedding.addWord(18831);
	//currEmbedding.addWord(12258); 
	//currEmbedding.addWord(14779); 

	//yeast 
	//Embedding: { vertices: [ 63 827 387 ] edges: [ 3037 467 ] } HASH: 784585540586335
	//currEmbedding.reset();
	//currEmbedding.addWord(63);
	//currEmbedding.addWord(827);
	//currEmbedding.addWord(387);

	//yeastbigcomp 280 456 278
	//currEmbedding.reset();
	//currEmbedding.addWord(280);
	//currEmbedding.addWord(456);
	//currEmbedding.addWord(278);

	//g->print();
	//currEmbedding.addWord(0);
	//currEmbedding.addWord(1);
	//currEmbedding.addWord(2);
	//currEmbedding.addWord(3);
	//currEmbedding.addWord(4);
	//currEmbedding.addWord(5);

	//currEmbedding.addWord(4939);
	//currEmbedding.addWord(6636);
	//currEmbedding.addWord(1998);

	//currEmbedding.addWord(2303); 
	//currEmbedding.addWord(5119);

	//4939 2303 5119
	//currEmbedding.addWord(4939);
	//currEmbedding.addWord(872);
	//currEmbedding.addWord(6425);

	//currEmbedding.addWord(4939);
	//currEmbedding.addWord(6768);
	//currEmbedding.addWord(6594);

	//currEmbedding.addWord(8724);
	//currEmbedding.addWord(4931);
	//currEmbedding.addWord(191);

	while (1) {	
		if (!getNextValidEmbedding(currEmbedding)) break;
		if (filterEmbedding(currEmbedding)) { 
			std::cout << "emb filtered!" << std::endl;
			continue;
		}	
		LOG(debug) << "checking" <<  currEmbedding << std::endl;
		std::cout << "group estimation number for emb:" << currEmbedding.getHash() << std::endl;
		currEmbedding.print();
		if (!EmbeddingUtils::isConnected(currEmbedding)) { 
			std::cout << "emb not connected!" << std::endl;
			exit(1);
		}	

		std::cout << "Degree: " << getEmbeddingDegree(currEmbedding) << std::endl;
		//std::cout << "SE1" << std::endl;
		//SuperEmbedding<T> se = createSNTopEmbeddingsGroup(currEmbedding, snSize);
		//se.print();
		//std::cout << "SE2" << std::endl;
		SuperEmbedding<T> se2 = createBFSGroup(currEmbedding, snSize);
		//se2.print();
	
		TourStats stats3  = getGroupStats(currEmbedding, se2, MAX_NUM_TOURS);
		TourStats stats2 =  getExactGroupStats(currEmbedding);

		//groupQualityAnalysis(currEmbedding);
		//stats1.print();
		stats3.print();
		stats2.print();
		break;
	}

}

template <class T, class A>
void EmbeddingSpace<T,A>::groupQualityAnalysis(T &embedding) {
	std::vector<int> snSizes = {100, 1000, 10000, 100000};	
	for ( auto& i : snSizes) {
		SuperEmbedding<T> se = createBFSGroup(embedding, i);
		for (int k = 0; k < 10000; k++) {
	  	TourStats stats = getGroupStats(embedding, se, 1);
	  	LOG(info) << "=Approx= " << embedding << " SnSize: " << i << " NumTours: " << 1 << " " << se << " " << stats;
		}	
	}

	/*std::vector<int> nToursSizes = {1, 10, 1000};	
	  SuperEmbedding<T> se = createBFSGroup(embedding, 500000);
	  for ( auto& i : nToursSizes) {
	  for (int k = 0; k < 10000/i; k++) {
	  TourStats stats = getGroupStats(embedding, se, i);
	  LOG(info) << "=Approx= " << embedding << " SnSize: " << 500000 << " NumTours: " << i << " " << se << " " << stats;
	  }	
	  }*/

	TourStats stats = getExactGroupStats(embedding);
	LOG(info) << "=Exact= " << embedding << " " << stats;
}


template <class T, class A>
SuperEmbedding<T> EmbeddingSpace<T,A>::createBFSGroup(T &embedding, int size) {
	SuperEmbedding<T> se;
	se.setQueueMaxSize(size);

	T embeddingCopy, previousEmbedding;
#ifdef DEBUG
	std::cout << "creating SUPERNODE GROUP with top embeddings" << std::endl;
	std::cout << "SuperNode Queue MaxSize: " << se.getQueueMaxSize() << std::endl;
#endif

	if (isSingleGroup) se.setHasAll(true);

	if (size > 1 && !isSingleGroup) {
		std::queue<T> queue;
		embeddingCopy = embedding;
		queue.push(embeddingCopy);

		int attempt=0;
		while (attempt!=std::max(MAX_INIT_ATTEMPT,size) && !queue.empty()) {
			//std::cout << "attempt: " << attempt << " queue size: " << se.getQueueSize() << std::endl;
			//std::cout << "emb number of neighbors: " << mod.second << std::endl;
			embeddingCopy = queue.front();
			//std::cout << "emb pop: " << embeddingCopy << std::endl;
			previousEmbedding = embeddingCopy;
			std::vector<Mod> mods = computeAllEmbeddingNeighborhood(embeddingCopy);
			for (int j = 0; j < (int) mods.size(); j++) {
				//mods[j].print();
				embeddingCopy.replaceWord(mods[j].rmId,mods[j].addId);
				//std::cout << "--emb neigh: " << embeddingCopy << std::endl;

				bool isValid = false;
				if (!isTransitive) {
					isValid = isValidEmbeddingGroup(embedding, embeddingCopy);
				}
				else {
					isValid = isValidEmbeddingGroup(previousEmbedding, embeddingCopy);
				}

				//put the embedding in the SE if it is not there already
				if (isValid && !se.hasEmbedding(embeddingCopy)) {
					queue.push(embeddingCopy);
					embeddingCopy.setDegree(getEmbeddingDegree(embeddingCopy));
					se.insertEmbeddingQueue(embeddingCopy);
				}
#ifdef DEBUG
				if (attempt%10000==0) {
					std::cout << "supernode step " << attempt << std::endl; 
					std::cout << std::flush;
				}
#endif		
				//return the embedding to its original form
				embeddingCopy.replaceWord(mods[j].addId,mods[j].rmId);
				attempt++;
				if (attempt>=std::max(MAX_INIT_ATTEMPT,size)) break;
			}
			queue.pop();

		}
		se.setHasAll((attempt < std::max(MAX_INIT_ATTEMPT, size)));
	}

	//std::cout << "computing supernode external and internal degrees..." << std::endl;
	//insert the origin embedding by force	
	if (se.getNumberOfEmbeddings() >= se.getQueueMaxSize()) {
		se.removeEmbeddingQueue();	
	}
	embeddingCopy = embedding;
	embeddingCopy.setDegree(getEmbeddingDegree(embeddingCopy));
	se.insertEmbeddingQueue(embeddingCopy);

	//Update embeddings
	se.updateEmbeddingCodeIndex();
	se.updateDegrees();

#ifdef DEBUG
	std::cout.setf(std::ios::fixed);
	std::cout << "SuperNode Size: " << se.getNumberOfEmbeddings() << " External Degree: " << se.getExternalDegree() << " Internal Degree: " << se.getInternalDegree()  << std::endl;
#endif
	//std::cout << "SuperNode Size: " << se.getNumberOfEmbeddings() << " External Degree: " << se.getExternalDegree() << " Internal Degree: " << se.getInternalDegree()  << " Has all: " << se.getHasAll() << std::endl;

	return se;
}

template <class T, class A>
SuperEmbedding<T> EmbeddingSpace<T,A>::createSNTopEmbeddingsGroup(T &embedding, int size) {
	SuperEmbedding<T> se;
	se.setQueueMaxSize(size);
	T embeddingCopy, previousEmbedding;

#ifdef DEBUG
	std::cout << "creating SUPERNODE GROUP with top embeddings" << std::endl;
	std::cout << "SuperNode Queue MaxSize: " << se.getQueueMaxSize() << std::endl;
#endif

	if (isSingleGroup) se.setHasAll(true);

	if (size > 1 && !isSingleGroup) {
		embeddingCopy = embedding;
		previousEmbedding = embeddingCopy;
		std::pair<Mod,bool> mod = getNextRandomModification(embeddingCopy);
		if (!mod.second) {
			std::cout << "warning: no neighbors were found to create supernode!" << std::endl;
		}

		int attempt=0;
		while (attempt!=std::max(MAX_INIT_ATTEMPT,size) && mod.second) {
			//std::cout << "attempt: " << attempt << " queue size: " << se.getQueueSize() << std::endl;
			//std::cout << "emb number of neighbors: " << mod.second << std::endl;
			embeddingCopy.replaceWord(mod.first.rmId,mod.first.addId);
			bool isValid = false;
			if (!isTransitive) {
				isValid = isValidEmbeddingGroup(embedding, embeddingCopy);
			}
			else {
				isValid = isValidEmbeddingGroup(previousEmbedding, embeddingCopy);
			}

			if (embeddingCopy.isSameEmbedding(embedding)) {
				mod = getNextRandomModification(embeddingCopy);
				previousEmbedding = embeddingCopy;
			}	
			else if (isValid) {
				mod = getNextRandomModification(embeddingCopy);
				if (!mod.second) {
					std::cout << "warning: no neighbors were found to create supernode!" << std::endl;
				}
				embeddingCopy.setDegree(getEmbeddingDegree(embeddingCopy));
				se.insertEmbeddingQueue(embeddingCopy);
				previousEmbedding = embeddingCopy;
			}
			else {
				embeddingCopy.replaceWord(mod.first.addId,mod.first.rmId);
				mod = getNextRandomModification(embeddingCopy);
			}
#ifdef DEBUG
			if (attempt%10000==0) {
				std::cout << "supernode step " << attempt << std::endl; 
				std::cout << std::flush;
			}
#endif
			attempt++;
		}
	}

	//std::cout << "computing supernode external and internal degrees..." << std::endl;
	//insert the origin embedding by force	
	if (se.getNumberOfEmbeddings() >= se.getQueueMaxSize()) {
		se.removeEmbeddingQueue();	
	}
	embeddingCopy = embedding;
	embeddingCopy.setDegree(getEmbeddingDegree(embeddingCopy));
	se.insertEmbeddingQueue(embeddingCopy);

	//Update embeddings
	se.updateEmbeddingCodeIndex();
	se.updateDegrees();

	//if (se.getNumberOfEmbeddings() == 1 || isSingleGroup) {
	//	se.setExternalDegree(embeddingCopy.getDegree());			
	//	se.setInternalDegree(0);		
	//	return se;		
	//}


#ifdef DEBUG
	std::cout.setf(std::ios::fixed);
	std::cout << "SuperNode Size: " << se.getNumberOfEmbeddings() << " External Degree: " << se.getExternalDegree() << " Internal Degree: " << se.getInternalDegree()  << std::endl;
#endif
	return se;
}

template <class T, class A>
double EmbeddingSpace<T,A>::getEmbeddingDegree(T &e, ModSet &mods) {
#ifdef CACHE
	if (emb_neighbor_cacheg.getAccess()%10000==0) {
		std::cout << "getEmbeddingDegree ";
		emb_neighbor_cacheg.printResume();
	}
	size_t hash = e.getHash();
	boost::optional<e_cache&> t = emb_neighbor_cacheg.get(hash);
	if (t) {
		return t->degree;
	}
#endif
	e_cache r;
	bool hasNeighbor = computeEmbeddingNeighborhood(e,r,mods);
	if (!hasNeighbor) {
		std::cout << "warning: no neighbors were found!" << std::endl;	
	}
#ifdef CACHE
	if (hasNeighbor)
		emb_neighbor_cacheg.insert(hash, r);
#endif

	return r.degree;
}

template <class T, class A>
double EmbeddingSpace<T,A>::getEmbeddingDegree(T &e) {
	ModSet empty_modset;
	return getEmbeddingDegree(e, empty_modset);
}

template <class T, class A>
std::pair<Mod,bool> EmbeddingSpace<T,A>::getNextRandomModification(T &e, ModSet &mods) {
#ifdef CACHE
	if (emb_neighbor_cacheg.getAccess()%10000==0) {
		std::cout << "getNextRandomModification cache ";
		emb_neighbor_cacheg.printResume();
	}
	size_t hash = e.getHash();
	boost::optional<e_cache &> t = emb_neighbor_cacheg.get(hash);
	if (t) {
		if (t->used_mods<e_cache::NUM_EXTRA_NEIGHBORS) {
			std::pair<Mod,double> mod(t->mods[t->used_mods], true);
			t->used_mods++;
			return mod;
		}	
	}
#endif
	std::pair<Mod,bool> mod = getNextRandomModificationByRejection(e, mods);
	if (mod.second) return mod;

#ifdef CACHE
	if (t) {
		//correcting the hit
		emb_neighbor_cacheg.setHit(emb_neighbor_cacheg.getHit()-1);
		emb_neighbor_cacheg.setMiss(emb_neighbor_cacheg.getMiss()+1);
		bool hasNeighbor = computeEmbeddingNeighborhood(e, t.get(), mods);
		std::pair<Mod,double> mod(t->mods[t->used_mods], hasNeighbor);
		t->used_mods++;
		return mod;
	}
#endif

	e_cache r;
	bool hasNeighbor = computeEmbeddingNeighborhood(e, r, mods);
	
	if (!hasNeighbor && mods.empty()) {
		std::cout << "warning: there are no embedding neighbors!" << std::endl;
		//exit(1);
		return std::pair<Mod, bool> (Mod(), false);
	}
	else if (!hasNeighbor && !mods.empty()) {
		std::cout << "warning: there are only embedding neighbors to avoid! getting one of them." << std::endl;
		return getNextRandomModification(e);
	}

	r.used_mods++;

#ifdef CACHE
	if (hasNeighbor)
		emb_neighbor_cacheg.insert(hash, r);
#endif

	return std::pair<Mod,bool>(r.mods[0], hasNeighbor);
}

template <class T, class A>
std::pair<Mod,bool> EmbeddingSpace<T,A>::getNextRandomModification(T &e) {
	ModSet empty_modset = ModSet({});
	return getNextRandomModification(e, empty_modset);
}

//get a neighbor out of the SN
template <class T, class A>
std::pair<T,bool> EmbeddingSpace<T,A>::getRandomNeighborSN(SuperEmbedding<T> &se) {
	std::pair<T,bool> neighEmb(T(), false);	
	std::pair<Mod,bool> mod;

	neighEmb = se.getBiasedEmbeddingWithExternalDegree();
	if (!neighEmb.second) return neighEmb;

	ModSet mods = se.getModificationsToSN(neighEmb.first);
	mod = getNextRandomModification(neighEmb.first,mods);
	neighEmb.first.replaceWord(mod.first.rmId, mod.first.addId);
	neighEmb.second = true;

	return neighEmb;
}

template <class T, class A>
bool EmbeddingSpace<T,A>::isValidEmbedding(T &e) {
	return true;
}

template <class T, class A>
bool EmbeddingSpace<T,A>::filterEmbedding(T &e) {
	return false;
}

template <class T, class A>
void EmbeddingSpace<T,A>::aggregateEmbeddingClass(T &e, TourStats &r,  A &a) {
	double factor=0;
	double totalNumEmbs = 0;
	int totalNumGroups = 0;

	//std::cout << "-- aggregating stats -- " << std::endl;
	//r.print();
	//std::cout << "--------------" << std::endl;

	totalNumGroups = (int) r.groups.size();
	for (int k = 0;  k < (int) r.groups.size(); k++) {
		factor+=1/r.groupsDegree[k];
		totalNumEmbs+=(r.groupsSize[k]/(double)REDUCTION_FACTOR);

	}

	a.aggregate(e.getBlissCodeHashValue(), PatternStats(factor, totalNumEmbs, totalNumGroups));	
}

template class EmbeddingSpace<EdgeInducedEmbedding, AggregatorPatternCounter>;
template class EmbeddingSpace<VertexInducedEmbedding, AggregatorPatternCounter>;
