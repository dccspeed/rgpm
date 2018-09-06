#include <vector>    // std::random_shuffle
#include <algorithm>    // std::random_shuffle
#include <gsl/gsl_math.h>
#include <gsl/gsl_cdf.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <thread>
#include <boost/program_options.hpp> 
#include <limits>
#include "graphSetReader.h"
#include "graph.h"
#include "utils.h"
#include "zeroin.h"
#include "EdgeInducedEmbedding.h"
#include "EmbeddingSpaceIntra.h"
#include "EmbeddingSpaceIntraPar.h"
#include "EmbeddingSpacePercolation.h"
#include "EmbeddingSpacePercolationPar.h"
//#include "EmbeddingSpaceTarget.h"
#include "EmbeddingSpaceHighDegreePar.h"
#include "EmbeddingSpaceTraditional.h"
#include "Aggregator.h"
#include "AggregatorPatternCounter.h"
#include "strmisc.h"
#include "SFMT.h"

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
            ("output,o", po::value<std::string>()->required(), "Output file name.") 
            ("psize,p", po::value<int>()->required(), "Pattern size.") 
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
    //std::ofstream ofs (output, std::ofstream::out);

    /* initialize random seed: */
    srand (time(NULL));

    GraphSetReader gphSetReader;
    //gphSetReader.openData("test2.gph");
    gphSetReader.openData(input);

    //HashPath hpath(10,50000);
    //MHFamily mhfamily(15,50000);

    std::cout << "reading the graph...\n";
    std::pair<Graph, bool> g = gphSetReader.readGraph();
    //g.first.createEdgeIndexSortedById();
    //g.first.modifyByAddingNodes(1, 1);

//  std::cout << "removing duplicates...\n";
//  g.first.removeDuplicatedEdges();
    g.first.printToFile(input + std::string(".random"));
   
    std::cout << "creatting neighborhood vertex index...\n";
    g.first.createNeighborhoodIndex();
    std::cout << "creatting neighborhood edge index...\n";
    g.first.createNeighborhoodEdgeIndex();
    g.first.printResume();

    //close input file
    gphSetReader.closeData();

    //g.first.printToFileArabesque(input + std::string(".arab"));

    //exit(1);
    //EmbeddingSpaceTraditional<VertexInducedEmbedding> es(psize, supernode, &g.first, config);
    //EmbeddingSpacePercolationPar<VertexInducedEmbedding> es(psize, supernode, &g.first, config, thread);
    EmbeddingSpacePercolation<VertexInducedEmbedding> es(psize, supernode, &g.first, config);
    //EmbeddingSpaceHighDegreePar<VertexInducedEmbedding> es(psize, supernode, &g.first, config, thread);
    //EmbeddingSpaceTraditionalPar<VertexInducedEmbedding> es(psize, supernode, &g.first);
    //EmbeddingSpaceTargetPar<VertexInducedEmbedding> es(psize, supernode, &g.first, 4);
    //EmbeddingSpaceIntraPar<VertexInducedEmbedding> es(psize, supernode, &g.first, config, thread);
    //EmbeddingSpaceIntra<VertexInducedEmbedding> es2(psize, supernode, &g.first, config);
    
    //es.setDegreeThreshold(80);
    //es.setAggOutput(output);

//std::string embString("1,2,3");
//VertexInducedEmbedding emb;
//emb.loadFromString(embString);
//emb.print();

    es.test();
    //es2.test();
    //es.run_exact_recursive();
    //es.run_rw();
    //es.run_rw_traditional();
    //es.run_full_group_search();
    //es.run_full_group_search_recursive();
    
	
    return 0;
}
