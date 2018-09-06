#include "BasicEmbeddingSpacePar.h"
#include <unistd.h>


template <class T, class A>
EmbeddingSpacePar<T,A>::EmbeddingSpacePar(int s, Graph *g): EmbeddingSpace<T,A>(s,g), producer_count(0), consumer_count(0), done(false), nthreads(1) {
};

template <class T, class A>
EmbeddingSpacePar<T,A>::EmbeddingSpacePar(int s, int sns, Graph *g): EmbeddingSpace<T,A>(s, sns, g),  producer_count(0), consumer_count(0), done(false), nthreads(1) {
};

template <class T, class A>
EmbeddingSpacePar<T,A>::EmbeddingSpacePar(int s, int sns, Graph *g, std::string config): EmbeddingSpace<T,A>(s, sns, g, config),  producer_count(0), consumer_count(0), done(false), nthreads(1) {
};

template <class T, class A>
EmbeddingSpacePar<T,A>::EmbeddingSpacePar(int s, int sns, Graph *g, int t): EmbeddingSpace<T,A>(s, sns, g),  producer_count(0), consumer_count(0), done(false), nthreads(t) {
};

template <class T, class A>
EmbeddingSpacePar<T,A>::EmbeddingSpacePar(int s, int sns, Graph *g, std::string config, int t): EmbeddingSpace<T,A>(s, sns, g, config),  producer_count(0), consumer_count(0), done(false), nthreads(t) {
};

template <class T, class A>
void EmbeddingSpacePar<T,A>::producer(void)
{
	std::cout << "Producer needs to produce " << this->MAX_RW_STEPS << " embeddings!" << std::endl;
	int nValid = 0;
	int i = 0; 
	while (i < this->MAX_RW_STEPS) {
		i++;
	        producer_count++;
		if (i%10000==0) {
			std::cout << i << " embeddings were produced! " << nValid << " embeddings are valid!" << std::endl;
    			//std::unordered_map<size_t, PatternStats> pattern_to_supp_local = summarizePatternMap();
		}

                if (this->filterEmbedding(this->currEmbedding)) continue;
		if (this->OUTPUT_EMB_FILE.is_open()) this->currEmbedding.writeWordsToFile(this->OUTPUT_EMB_FILE);		
		
		nValid++;
		while ((int) queue.size() > MAX_QUEUE_SIZE); 
		queue.push(this->currEmbedding);		
		
		if (!this->getNextValidEmbedding(this->currEmbedding)) break;
    	}

	for (int i = 0; i < nthreads; i++) {
		this->currEmbedding.isPoisonPill=true;
		queue.push(this->currEmbedding);		
	}
			
}

template <class T, class A>
void EmbeddingSpacePar<T,A>::consumer(int id) {
        auto t1 = std::chrono::high_resolution_clock::now();

	T embeddingCopy;
    	while (1) {
			queue.pop(embeddingCopy);
			if (embeddingCopy.isPoisonPill) break;
	        	int nValid = consumer_count++;
			
	                if (nValid%1000==0) {
				std::cout << nValid << " embeddings were processed!" << std::endl;
			}
			if (nValid%10000==0) {
				mu.lock();
				summarizeAggregator(this->agg);
				#ifdef FLUSH_AGGREGATOR
				if (this->MAX_AGG_SIZE >= this->agg.getMapSize()) { 
					std::string output = this->aggOutput + std::to_string(this->numFlushes);
					//std::cout << "---FLUSH to file--- " << output << std::endl;
					std::ofstream os (output, std::ofstream::out);
					this->agg.flush(os);
					this->numFlushes++;
					os.close();	
					//std::cout << "-----------------" << output << std::endl;
					this->agg.clear();
				}
				#endif
				std::cout << "RWSTEP: " << "{ processed: " << nValid << " patts: " << this->agg.getMapSize() << " }"<< std::endl;
				std::string output = this->aggOutput;
				std::ofstream os (output, std::ofstream::out | std::ofstream::app);
				os << "RWSTEP: " << "{ processed: " << nValid << " patts: " << this->agg.getMapSize() << " }"<< std::endl;
				this->agg.flush(os);
				os.close();
				
				mu.unlock();
			}

			processEmbedding(embeddingCopy, id);
	    }


	    auto t2 = std::chrono::high_resolution_clock::now();

	    auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
	    std::cout << "Thread finished. Elapsed time:  " << duration << " ms " << std::endl;
}

template <class T, class A>
void EmbeddingSpacePar<T,A>::run_rw() {
        if (!Config::existKey(std::string("INPUT_EMB_FILE"))) {
                if (!this->init_rw()) {
                        std::cout << "init fail!" << std::endl;
                        return;
                }
                std::cout << "init function has finished\n";
        }
        else {
                std::cout << "init function is not necessary! reading embs from file.\n";
        }

	
	LOG(info) << "creating " << nthreads << " local aggregators"; 
	//LOG(info) << "aggs size " << aggs.size();
	aggs.resize(nthreads);
	LOG(info) << "aggs size " << aggs.size() << std::endl; 

	mus = new tbb::recursive_mutex[nthreads];

	tbb::task_scheduler_init init(nthreads);

	// setup producer
	tbb::task_group producer_group;
	producer_group.run([&] {producer(); done = true;});
	
	// setup consumers
	tbb::parallel_for(size_t(0), size_t(nthreads),[&] (size_t i) {consumer(i);});

	// ensure producer has finished
	producer_group.wait();

	LOG(info) << "Produced (total) " << producer_count << " objects.";
	LOG(info) << "Consumed (valid) " << consumer_count << " objects.";

	summarizeAggregator(this->agg);

#ifdef FLUSH_AGGREGATOR
	if (this->agg.getMapSize()!=0) { 
		std::string output = this->aggOutput + std::to_string(this->numFlushes);
		//std::cout << "---FLUSH to file--- " << output << std::endl;
		std::ofstream os (output, std::ofstream::out);
		this->agg.flush(os);
		this->numFlushes++;
		os.close();
		//std::cout << "-----------------" << output << std::endl;
		this->agg.clear();
	}
#endif
	std::cout.setf(std::ios::fixed);
	std::cout << "RWSTEPFINAL: " << this->MAX_RW_STEPS << " valid: " << consumer_count << " filtered: " << producer_count-consumer_count << " NUM PATTs: " << this->agg.getMapSize() << std::endl;
	this->agg.print();

	delete[] mus;
}

template <class T, class A>
void EmbeddingSpacePar<T,A>::processEmbedding(T &e, int id) {
	if (this->SET_QLTY_ANALYSIS) {
		this->groupQualityAnalysis(e);
		return;
	}
	
	TourStats r;
	if (this->SET_TOUR)	{
		//create supernode
		if (this->SET_SUPERNODE_RW) {
			SuperEmbedding<T> se = this->createSNTopEmbeddingsGroup(e, this->snSize);
			//LOG(debug) << se;	
			r = this->getGroupStats(e, se, this->MAX_NUM_TOURS);
		}
		else  {
			SuperEmbedding<T> se = this->createBFSGroup(e, this->snSize);
			//LOG(debug) << se;	
			r = this->getGroupStats(e, se, this->MAX_NUM_TOURS);
		}
	}
	else {
		r = this->getExactGroupStats(e);
	}

	if (!r.ret) {
		std::cout << "Problem detected! over tour! " << std::endl;
		exit(1);
	}

	mus[id].lock();
	this->aggregateEmbeddingClass(e, r, aggs[id]);
	mus[id].unlock();
}

template <class T, class A>
void EmbeddingSpacePar<T,A>::groupQualityAnalysis(T &embedding) {
        std::vector<int> snSizes = {100, 1000, 10000, 100000};
        for ( auto& i : snSizes) {
                SuperEmbedding<T> se = this->createBFSGroup(embedding, i);
    		tbb::parallel_for( size_t(0), size_t(10000) , [&]( size_t k ) {
                        TourStats stats = this->getGroupStats(embedding, se, 1);
                        LOG(info) << "=Approx= " << embedding << " SnSize: " << i << " NumTours: " << 1 << " " << se << " " << stats;
	    	} );
        }

        /*std::vector<int> nToursSizes = {1, 10, 1000}; 
        SuperEmbedding<T> se = createBFSGroup(embedding, 500000);
        for ( auto& i : nToursSizes) {
                for (int k = 0; k < 10000/i; k++) {
                        TourStats stats = getGroupStats(embedding, se, i);
                        LOG(info) << "=Approx= " << embedding << " SnSize: " << 500000 << " NumTours: " << i << " " << se << " " << stats;
                }       
        }*/

        TourStats stats = this->getExactGroupStats(embedding);
        LOG(info) << "=Exact= " << embedding << " " << stats;
}

template <class T, class A>
void EmbeddingSpacePar<T,A>::summarizeAggregator(A &a) {
	for (int i = 0; i < (int) aggs.size(); i++) {
		std::cout << "Combining aggregator. " <<  "thread number: " << i << 
			std::endl;
		mus[i].lock();
		a.aggregate(aggs[i]);
		aggs[i].clear();
		mus[i].unlock();
	}
}

template <class T, class A>
TourStats EmbeddingSpacePar<T,A>::groupEstimateAllTours(T &embedding, SuperEmbedding<T> &se, int nTours) {
	TourStats finalTourStats;

	if (se.getHasAll())
		return finalTourStats;

	tbb::task_scheduler_init init(nthreads);
	finalTourStats = tbb::parallel_reduce(
			tbb::blocked_range<int>(0, nTours),
			TourStats(),
			[&] (const tbb::blocked_range<int> &r,
				TourStats temp) -> TourStats {
 				for (int i = r.begin(); i < r.end(); i++) 
					temp += this->groupEstimateUsingTour(embedding, se, i);
				return temp;
			},
			[] (TourStats t1, TourStats t2) -> TourStats {
			return t1 + t2;
			});

	return finalTourStats;
}


template class EmbeddingSpacePar<VertexInducedEmbedding, AggregatorPatternCounter>;
template class EmbeddingSpacePar<EdgeInducedEmbedding, AggregatorPatternCounter>;
