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
#include <limits>
#include "logging.h"
#include "graphSetReader.h"
#include "graph.h"
#include "utils.h"
#include "EmbeddingSpaceEnumerateAll.h"

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
	int supernode = 1;
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
			("output,o", po::value<std::string>()->required(), "Output file name.") 
			("psize,p", po::value<int>()->required(), "Pattern size.") 
			("loglevel,l", po::value<boost::log::trivial::severity_level>(), "Log level to output")
			("config,c", po::value<std::string>(), "Configuration file");

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
			if ( vm.count("supernode")) {
				supernode = vm["supernode"].as<int>();
			}
			if ( vm.count("loglevel")) {
				logSeverity = vm["loglevel"].as<boost::log::trivial::severity_level>();
			}
			if ( vm.count("config")) {
				config = vm["config"].as<std::string>();
			}

			po::notify(vm); // throws on error, so do after help in case 

			if (psize<=0) {
				po::error e("Pattern size value greatter than 0!");
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

	uint graphId = 0;
	while(true) {
		//std::cout << "blah\n";
		std::pair<Graph, bool> g = gphSetReader.readTransactionData();
		bool ok = g.second;

		//std::cout << "creatting neighborhood vertex index...\n";
		g.first.createNeighborhoodIndex();
		g.first.setId(graphId);
		g.first.printResume();
		
		EmbeddingSpaceEnumerateAll<VertexInducedEmbedding> es(psize, supernode, &g.first, config);
    		//es.setAggOutput(output);
		es.run_exact_recursive();
		
		if(!ok) break;
	}
	gphSetReader.closeData();


	return 0;
}
