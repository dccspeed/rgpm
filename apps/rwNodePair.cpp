#include <vector>    // std::random_shuffle
#include <algorithm>    // std::random_shuffle
#include <gsl/gsl_math.h>
#include <gsl/gsl_cdf.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <thread>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/program_options.hpp> 
#include <boost/algorithm/string.hpp>
#include <limits>
#include "logging.h"
#include "graphSetReader.h"
#include "graph.h"
#include "utils.h"
#include "zeroin.h"
#include "EmbeddingSpaceNodePairPar.h"
//#include "EmbeddingSpaceNodePair.h"

int numthreads=1;

namespace 
{ 
    const size_t ERROR_IN_COMMAND_LINE = 1; 
    const size_t SUCCESS = 0; 
    const size_t ERROR_UNHANDLED_EXCEPTION = 2; 

} // namespace

int main(int argc, char **argv) {

	//parameters
	std::string input, output, config;
	int psize = -1;
	std::pair<int, int> nodep;
	int supernode = 1;
	int thread = 1;
	
        boost::log::trivial::severity_level logSeverity = boost::log::trivial::info;
	try 
	{ 
		/** Define and parse the program options 
		 */ 
		namespace po = boost::program_options; 
		po::options_description desc("Options"); 
		desc.add_options() 
			("help", "Print help messages") 
			("input,i", po::value<std::string>()->required(), "Input file name.") 
			("output,o", po::value<std::string>(), "Output file name.")
			("psize,p", po::value<int>()->required(), "Pattern size.") 
			("pair,r", po::value<std::string>()->required(), "Pair of node ids. ex: 1,2.") 
			("supernode,s", po::value<int>(), "Super Node Size") 
			("loglevel,l", po::value<boost::log::trivial::severity_level>(), "Log level to output")
			("config,c", po::value<std::string>(), "Configuration file") 
			("thread,t", po::value<int>(), "Number of threads");

		po::variables_map vm; 
		try 
		{ 
			po::store(po::parse_command_line(argc, argv, desc),  
					vm); // can throw 

			/** --help option 
			 */ 
			if ( vm.count("help")) { 
				std::cout << "Basic Command Line Parameter App" << std::endl 
					<< desc << std::endl; 
				return SUCCESS; 
			} 
			if ( vm.count("input")) {
				input = vm["input"].as<std::string>();
			}
			if ( vm.count("output")) {
				output = vm["output"].as<std::string>();
			}
			if ( vm.count("psize")) {
				psize = vm["psize"].as<int>();
			}
			if ( vm.count("pair")) {
				std::vector<std::string> tokenList;
		                boost::split(tokenList, vm["pair"].as<std::string>(), boost::is_any_of(",;"));
				if (tokenList.size()!=2) {
					po::error e("Pair of nodes not defined!");
					throw e;
				}
				else {
					nodep.first = std::stoi(tokenList[0]);
					nodep.second = std::stoi(tokenList[1]);
				}
			}
			if ( vm.count("supernode")) {
				supernode = vm["supernode"].as<int>();
			}
			if ( vm.count("loglevel")) {
				logSeverity = vm["loglevel"].as<boost::log::trivial::severity_level>();
			}
			if ( vm.count("config")) {
				config = vm["config"].as<std::string>();
			}
			if ( vm.count("thread")) {
				thread = vm["thread"].as<int>();
			}

			po::notify(vm); // throws on error, so do after help in case 

			if (psize<=0) {
				po::error e("Pattern size value greatter than 0!");
				throw e;
			}
			if (supernode <= 0) {
				po::error e("Supernode size must be greater than zero!");
				throw e;
			}
			if (thread <= 0) {
				po::error e("Number of threads be greater than zero!");
				throw e;
			}

			//std::cout << "parameters : "  << input << " " << espace << " " << prob << std::endl;
			po::notify(vm); // throws on error, so do after help in case 
			// there are any problems 
		} 
		catch(po::error& e) 
		{ 
			std::cerr << "ERROR: " << e.what() << std::endl << std::endl; 
			std::cerr << desc << std::endl; 
			return ERROR_IN_COMMAND_LINE; 
		} 

		// application code here // 
	}

	catch(std::exception& e) 
	{ 
		std::cerr << "Unhandled Exception reached the top of main: " 
			<< e.what() << ", application will now exit" << std::endl; 
		return ERROR_UNHANDLED_EXCEPTION; 

	} 
        
        // set log level
        init_logging(logSeverity);

	//output file.
	std::ofstream ofs (output, std::ofstream::out);

	/* initialize random seed: */
	srand (time(NULL));

	GraphSetReader gphSetReader;
	gphSetReader.openData(input);


	std::cout << "config " << config << std::endl; 
	uint graphId = 0;
	while(true) {
		//std::cout << "blah\n";
		std::pair<Graph, bool> g = gphSetReader.readTransactionData();
		bool ok = g.second;

		//std::cout << "creatting neighborhood vertex index...\n";
		g.first.createNeighborhoodIndex();
		//std::cout << "creatting neighborhood edge index...\n";
		g.first.createNeighborhoodEdgeIndex();
		g.first.setId(graphId);
		g.first.printResume();
		
		//if (thread > 1) { 
		      EmbeddingSpaceNodePairPar<VertexInducedEmbedding> es(nodep, psize, supernode, &g.first, config, thread);
		      es.setAggOutput(output);
		      es.run_rw();	
		//}
		//else {
		//	EmbeddingSpaceNodePair<VertexInducedEmbedding> es(nodep, psize, supernode, &g.first, config);
		//        es.setAggOutput(output);
		//	es.run_rw();	
		//}
		
		if(!ok) break;
	}
	gphSetReader.closeData();


	return 0;
}
