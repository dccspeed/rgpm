#include "utils.h"
#include <math.h>
#include <iostream>

StrStrMap Config::configMap;

void Config::load(std::string conf_file) {
	std::ifstream is_file(conf_file, std::ifstream::in);
	std::string line;
	while( std::getline(is_file, line) )
	{
		std::istringstream is_line(line);
		std::string key;
		if( std::getline(is_line, key, '=') )
		{
			std::string value;
			if( std::getline(is_line, value))
				configMap.insert(std::make_pair(key, value));
		}
	}
};

void Config::reload( std::string conf_file) {
	configMap.clear();
	load(conf_file);
};

bool Config::isActive() {
	return (!configMap.empty());
};

int Config::getKeyAsInt(std::string key) {
	StrStrMap::iterator it = Config::configMap.find(key);
	return atoi(it->second.c_str());
};

std::pair<int,int> Config::getKeyAsIntPair(std::string key) {
	StrStrMap::iterator it = Config::configMap.find(key);

	std::vector<std::string> tokenList;
	boost::split(tokenList, it->second, boost::is_any_of(" ,;"));
          
	 if ((int) tokenList.size() != 2) {
                std::cout << "error: getKeyAsIntPair() " << std::endl;
		exit(1);
        }
	
	std::pair<int, int> p; 
	p.first = atoi(tokenList[0].c_str());
	p.second = atoi(tokenList[1].c_str());
	
	return p;
};

double Config::getKeyAsDouble(std::string key) {
	StrStrMap::iterator it = Config::configMap.find(key);
	return atof(it->second.c_str());
};

std::string Config::getKeyAsString(std::string key) {
	StrStrMap::iterator it = Config::configMap.find(key);
	return it->second;
};

bool Config::getKeyAsBoolean(std::string key) {
	StrStrMap::iterator it = Config::configMap.find(key);
	if (it->second.compare("false")==0)
		return false;
	else if (it->second.compare("true")==0)
		return true;
	else {
		std::cout << "warning: setting tour to default value: true." << std::endl; 
		return true;
	}
	
};

bool Config::existKey(std::string key) {
	StrStrMap::iterator it = configMap.find(key);
	return  (it!=configMap.end());
};

void Config::print() {
	std::cout << "==Config File==" << std::endl;
	for (StrStrMap::iterator it = configMap.begin(); it != configMap.end(); it++) {
		std::cout << "key: " << it->first << " value: " << it->second << std::endl;
	}
}

void mapIdToLabels(const StrIntMap& label2id, IntStrMap& id2label)
{
	StrIntMap::const_iterator v;

	for(v = label2id.begin(); v != label2id.end(); ++v) {

		IntStrMap::const_iterator u = id2label.find(v->second);
		if (u==id2label.end()) {
			const std::string& label = v->first;
			const int& id = v->second;
			id2label[id] = label;
		}
		//std::cout << id << " " << id2label[id] << std::endl; 
	}
}

bool isPrime(int n) {
	if (n <= 2) {return n == 2;}
	if (n % 2 == 0){return false;}

	// rule out odds up to the square root of n
	for (int i = 3, end = (int)sqrt((float)n);i <= end; i += 2)
	{
		if (n % i == 0) {return false;}
	}
	return true;
}

void printIntList(std::list<int> &l) {
	std::list<int>::iterator i;
	std::cout << "[ ";
	for( i = l.begin(); i != l.end(); ++i)
		std::cout << *i << " ";
	std::cout << "] " << std::endl;
}

bool compare_pair(const std::pair<int, double> &first, const std::pair<int, double> &second){
	return ( first.second > second.second );
}


void setStackSize() {
    const rlim_t kStackSize = 1000 * 1024 * 1024;   // min stack size = 16 MB
    struct rlimit rl;
    int result;

    result = getrlimit(RLIMIT_STACK, &rl);
    if (result == 0)
    {
        if (rl.rlim_cur < kStackSize)
        {
            rl.rlim_cur = kStackSize;
            result = setrlimit(RLIMIT_STACK, &rl);
            if (result != 0) {
                std::cout << "error: setrlimit returned result :" <<  result << std::endl;
		exit(1);
            }
	    else {
                std::cout << "ok. setrlimit returned result :" <<  result << std::endl;
	    }
        }
	else { 
                std::cout << "ok. current stack size is large enough! :" <<  result << std::endl;

	}
    }
    else {
                std::cout << "error: setrlimit returned result :" <<  result << std::endl;
		exit(1);
    }
}

