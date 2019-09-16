#include "canonical.h"

tbb::atomic<int> Canonical::hits = 0;
tbb::atomic<int> Canonical::misses = 0;
tbb::concurrent_hash_map<size_t, size_t> Canonical::pattern_to_canonical = tbb::concurrent_hash_map<size_t, size_t> ();
//tbb::concurrent_hash_map<size_t, Canonical::PatternInfo*> Canonical::naives = tbb::concurrent_hash_map<size_t, Canonical::PatternInfo*> ();
//tbb::concurrent_hash_map<size_t, Canonical::PatternInfo*> Canonical::canonicals = tbb::concurrent_hash_map<size_t, Canonical::PatternInfo*> ();

/*size_t Canonical::getHashScratch(bliss::Graph &bg) {
	bliss::Stats stats;
	//size_t N = bg.get_nof_vertices();

	const unsigned int* perm = bg.canonical_form(stats, NULL, NULL);
	bliss::Graph *bc = bg.permute(perm);
	size_t canCode = bc->get_hash();

	delete bc;
	return canCode;
}*/

//get canonical with embedding directly  
size_t Canonical::getHash(BasicEmbedding &e) {
	size_t naiveCode = e.getNaiveCodeHashValue();
	tbb::concurrent_hash_map<size_t, size_t>::accessor a;
	bool r = Canonical::pattern_to_canonical.insert(a, naiveCode);       // creates by default if not exists, acquires lock
	
	if (r) { // it means that it is new
		bliss::Graph bg = e.getBlissGraph();
		bliss::Stats stats;
		const unsigned int* perm = bg.canonical_form(stats, NULL, NULL);
		bliss::Graph *bc = bg.permute(perm);
		a->second = bc->get_hash();
		delete bc;
		Canonical::misses++;
	}	
	else {
		Canonical::hits++;	
	}

	size_t code = a->second;
	a.release();

	if (Canonical::hits+Canonical::misses%10000==0) {
		std::cout << "Caninocal hashmap hits: " << Canonical::hits << " misses: " << Canonical::misses << std::endl;
	}	
	
	return code;
}

/*size_t Canonical::getMyHash(BasicEmbedding &e) {
	size_t naiveCode = e.getNaiveCodeHashValue();
	tbb::concurrent_hash_map<size_t, size_t>::accessor a;
	bool r = Canonical::pattern_to_canonical.insert(a, naiveCode);       // creates by default if not exists, acquires lock
	
	if (r) { // it means that it is new
		

		//build adj-list for edges
		std::vector<int> &edges = e.getEdges();		
		int adjlist[edges.size()][edges.size()+1]; 
		for (uint i = 0; i < edges.size(); i++) {
			adjlist[i][0]=0;
			for (uint j = (i+1); j < edgegs.size(); j++) {
				if (g->isNeighborEdge(edges[i], edges[j])) {
					adjlist[i][++adjlist[i][0]] = j;
					adjlist[j][++adjlist[i][0]] = i;
				}
		
			}
		}

		Canonical::misses++;
	}	
	else {
		Canonical::hits++;	
	}

	size_t code = a->second;
	a.release();

	if (Canonical::hits+Canonical::misses%10000==0) {
		std::cout << "Caninocal hashmap hits: " << Canonical::hits << " misses: " << Canonical::misses << std::endl;
	}	
	
	return code;

}*/

//get canonical with graph directly  
size_t Canonical::getHash(Graph &g) {
	size_t naiveCode = g.getNaiveCodeHashValue();
	tbb::concurrent_hash_map<size_t, size_t>::accessor a;
	bool r = Canonical::pattern_to_canonical.insert(a, naiveCode);       // creates by default if not exists, acquires lock
	
	if (r) { // it means that it is new
		bliss::Graph bg = g.getBlissGraph();
		bliss::Stats stats;
		const unsigned int* perm = bg.canonical_form(stats, NULL, NULL);
		bliss::Graph *bc = bg.permute(perm);
		a->second = bc->get_hash();
		delete bc;
		Canonical::misses++;
	}	
	else {
		Canonical::hits++;	
	}

	size_t code = a->second;
	a.release();

	if (Canonical::hits+Canonical::misses%10000==0) {
		std::cout << "Caninocal hashmap hits: " << Canonical::hits << " misses: " << Canonical::misses << std::endl;
	}	

	return code;
}

size_t Canonical::getHash(bliss::Graph &bg) {
	//return Canonical::getHashScratch(bg);

	size_t naiveCode = bg.get_hash();
	tbb::concurrent_hash_map<size_t, size_t>::accessor a;
	bool r = Canonical::pattern_to_canonical.insert(a, naiveCode);       // creates by default if not exists, acquires lock
	
	PatternInfo *naiveInfo = NULL;
	PatternInfo *canonicalInfo = NULL;

	if (r) { // it means that it is new
		bliss::Stats stats;
		size_t N = bg.get_nof_vertices();
	
		//naiveInfo->init(N);	
		//canonicalInfo->init(N);	
		naiveInfo = new PatternInfo(N);
		canonicalInfo = new PatternInfo(N);
		
		//structures to keep automorphic info
		for (unsigned int i = 0; i < N ; i ++) {
			canonicalInfo->array[i] = i;
		}
		
		//running canonical form func
		//perm[i] is the vertex idx from the original graph keep in position i of the canonical one.
		const unsigned int* perm = bg.canonical_form(stats, &report_aut, canonicalInfo->array);
		memcpy(naiveInfo->array, perm, sizeof(unsigned int) * N);
		bliss::Graph *bc = bg.permute(perm);
		a->second = bc->get_hash();

		//put parent to canonical format
		unsigned int parentAux[N];
		memcpy(parentAux, canonicalInfo->array, sizeof(unsigned int) * N);
		for (unsigned int i = 0; i < N ; i ++) {
			canonicalInfo->array[i] = parentAux[perm[i]];
		}
	
		//bg.write_dimacs(stdout);
		//bc->write_dimacs(stdout);
		//std::cout << "canonical perm: ["; 
		//for (uint i = 0; i < N ; i ++) {
		//	std::cout << " " << naiveInfo->array[i];
		//}
		//std::cout << " ]\n";
		//std::cout << "parent: ["; 
		//for (uint i = 0; i < N ; i ++) {
		//	std::cout << " " << canonicalInfo->array[i];
		//}
		//std::cout << " ]\n";
		
		delete bc;
	}	

	size_t code = a->second;
	a.release();
	
	/*//create accessor to fill other maps
	tbb::concurrent_hash_map<size_t, PatternInfo*>::accessor b;
		
	//fill map naive info
	bool r1 = Canonical::naives.insert(b, naiveCode);       // creates by default if not exists, acquires lock
	if (r1) b->second = naiveInfo;
	else if (naiveInfo) delete naiveInfo;
	b.release();
	
	//fill map canonical info
	bool r2 = Canonical::canonicals.insert(b, code);       // creates by default if not exists, acquires lock
	if (r2) b->second = canonicalInfo;
	else if (canonicalInfo) delete canonicalInfo;
	b.release();

	//sanaty check
	if (r!=r1 || r!=r2) {
		std::cout << "error: canonical hashmaps are incorrect!" << std::endl;
		exit(1);
	}*/

	// canonical code
	return code;
}


/*unsigned int* Canonical::getPermutation(bliss::Graph &bg) {
	size_t naiveCode = bg.get_hash();
        tbb::concurrent_hash_map<size_t, PatternInfo*>::accessor b;
        bool r = Canonical::naives.find(b, naiveCode);       // creates by default if not exists, acquires lock
        if (r) { //means that was found
                unsigned int *array = b->second->array;
		b.release();
		return array;
        }
	else {
		b.release();
		getHash(bg);
		return getPermutation(bg);
	}
	
}*/

/**
 * The hook function that prints the found automorphisms.
 * \a param must be a file descriptor (FILE *).
 */
void Canonical::report_aut(void* param, const unsigned int n, const unsigned int* aut) {
	//fprintf((FILE*)param, "Generator: ");
	//fprintf(stdout, "Generator: ");
	//bliss::print_permutation((FILE*)param, n, aut, 0);
	get_automorphism_map((unsigned int*)param, n, aut, 0);
	/*std::cout << "[";
	  for (int i = 0; i < n ; i ++) {
	  std::cout << " " << aut[i];
	  }
	  std::cout << " ]\n"; */
	//fprintf(stdout, "\n");
}


void Canonical::get_automorphism_map(unsigned int* parent, const unsigned int N, const unsigned int* perm, const unsigned int offset) {
	assert(N > 0);
	assert(perm);

	for(unsigned int i = 0; i < N; i++) {
		unsigned int j = perm[i];
		if(j == i) continue;

		while(parent[i]>j) {
			parent[i] = j;
			j = parent[j];
		}
	}


	/*for(unsigned int i = 0; i < N; i++) {
		unsigned int j = perm[i];
		if(j == i)
			continue;
		bool is_first = true;
		while(j != i) {
			if(j < i) {
				is_first = false;
				break;
			}
			j = perm[j];
		}
		if(!is_first)
			continue;
		fprintf(stdout, "(%u,", i+offset);
		j = perm[i];
		while(j != i) {
			if (parent[j] > i) parent[j] = i;
			fprintf(stdout, "%u", j+offset);
			j = perm[j];
			if(j != i)
				fprintf(stdout, ",");
		}
		fprintf(stdout, ")");
	}*/

	


	/*std::cout << "#####canonical perm####: [";
	for (int i = 0; i < N ; i ++) {
		std::cout << " " << perm[i];
		std::cout << " (" << parent[i] << ") ";

	}
	std::cout << " ]\n";*/

}

/*void Canonical::merge_sets(int *parent, size_t N) {
	for (int i = 0; i < N; i++) {
		std::cout << "checking parent of id " << i << std::endl; 
		if (parent[i] == -1) {
			parent[i] = i;
			continue;
		}
		int j = parent[i];
		//while(j != i) {
		while(parent[j]!=j) {
			j = parent[j];		
			std::cout << "new parent " << j << std::endl;
		}	
		std::cout << "idx " << i << " parent " << j << std::endl;	
		parent[i] = j;
	}
}*/

