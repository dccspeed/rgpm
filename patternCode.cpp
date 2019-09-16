#include "patternCode.h"
#include <iostream>
#include "graphSetReader.h"


void PatternItem::print() const {
	std::cout << "( ";
	std::cout << this->srcId << " ";
	//std::cout << GraphSetReader::id2labelNode[this->srcLabel] << " ";
	std::cout << this->srcLabel << " ";
	std::cout << this->destId << " ";
	//std::cout << GraphSetReader::id2labelNode[this->destLabel] << " ";
	std::cout << this->destLabel << " ";
	std::cout << this->type << " ";
	std::cout << ")";
}

bool PatternItem::comparePatternItem(const PatternItem &pItem1, const PatternItem &pItem2) {
	bool isSmaller = false;	
    //std::cout << "comparing codeItems: ";
    //pItem1.print();
    //std::cout << " == ";
    //pItem2.print();
    //std::cout << std::endl;
    
	if (pItem1.srcId == pItem2.srcId  && pItem1.destId == pItem2.destId) {
		if (pItem1.srcLabel == pItem2.srcLabel) {
			if ( pItem1.destLabel < pItem2.destLabel) {
				isSmaller = true;
			}

		}
		else if (pItem1.srcLabel < pItem2.srcLabel) {
			isSmaller = true;
		}
	}

	else {
		//fwd, fwd
		if (pItem1.type==true && pItem2.type==true) {
			//System.out.println("@FOWARD");
			if (pItem1.destId < pItem2.destId)
				isSmaller = true;
			else if (pItem1.destId == pItem2.destId) {
				if (pItem1.srcId > pItem2.srcId)
					isSmaller = true;
			}
		}
		//bwd, bwd
		else if (pItem1.type==false && pItem2.type==false) {
			//System.out.println("@BACKWARD");
			if (pItem1.srcId < pItem2.srcId)
				isSmaller = true;
			if (pItem1.srcId == pItem2.srcId) {
				if (pItem1.destId < pItem2.destId)
					isSmaller = true;
			}
		}

		//fwd, bwd
		else if (pItem1.type==true && pItem2.type==false) {
			//System.out.println("@FOWARD and @BACKWARD");
			if (pItem1.destId <= pItem2.srcId) {
				isSmaller = true;
			}
		}
		//bwd, fwd
		else { 
			//System.out.println("@BACKWARD and @FOWARD");
			if (pItem1.srcId < pItem2.destId) {
				isSmaller = true;
			}
		}
	}

	//pItem1 < pItem2
   
    //std::cout << "is current item smaller: " << std::boolalpha << isSmaller << std::endl; 
	return isSmaller;
}


bool PatternItem::comparePatternItemEqual(const PatternItem &pItem1, const PatternItem &pItem2) {
	if (pItem1.srcId == pItem2.srcId &&
			pItem1.srcLabel == pItem2.srcLabel && 
			pItem1.destId == pItem2.destId && 
			pItem1.destLabel == pItem2.destLabel )
		return true;
	return false;

}

int PatternItem::compareTo(const PatternItem &pItem) const {
	if (comparePatternItemEqual(*this, pItem))
		return 0;
	else if (comparePatternItem(*this, pItem)) return -1;
	else return 1;

};

/*std::vector<Transaction> PatternCode::remove(PatternItem pItem) {
	std::vector<Transaction> ts;
	std::pair<int, bool> newSrc  = getIndexMap(srcId);
	std::pair<int, bool> newDest = getIndexMap(destId);
	
	if (!newSrc.second || !newDest.second) {
			return ts;
	}
        
	//update code
        std::vector<PatternItem>::iterator it = this->begin();
        while (it != this->end()) {
                if (it->compareTo(pItem)==0) {
                        it = this->erase(it);
                }
		else {
			if ()
			
                	it++;
		}
        }
	if (it == this->end()) return ts;
	return true;
}*/


bool PatternCode::add(int srcId, int srcLabel, int destId, int destLabel) {
	std::pair<int, bool> newSrc  = getIndexMap(srcId);
	std::pair<int, bool> newDest = getIndexMap(destId);

	if (!newSrc.second && newDest.second) {
			return false;
	}

	bool type = true;
	//check if the node was inserted yet
	if (newSrc.second && newDest.second) {
		for (int i = 0; i < (int)this->size(); i++) {
			if ((this->at(i).srcId == newSrc.first && this->at(i).destId == newDest.first) ||
					(this->at(i).srcId == newDest.first && this->at(i).destId == newSrc.first))
				return false;
		}
		type=false;
	}
	if (!newSrc.second) {
		newSrc.first = vertexMap.size();
		vertexMap.push_back(srcId);
	}
	if (!newDest.second) {
		//std::cout << "FWD" << std::endl;
		//std::cout << std::endl;
		newDest.first = vertexMap.size();
		vertexMap.push_back(destId);
	}
	//add	
	this->push_back(PatternItem (newSrc.first, srcLabel, newDest.first, destLabel, type));
	//std::cout << "result ";
	//this->print();
	//std::cout << std::endl;
	return true;
}

std::list<int> PatternCode::getRightMostPathExtensibleIds() {
	std::list<int> extensibles;

	if (this->size()==0) 
		return extensibles;

	int size = this->size();
	int lastIndex = 0;
	int lastSourceId = 0;
	//print();	
	for (int i = size; i > 0; i--){
		if (this->at(i-1).type == true) {
			lastIndex = i;
			lastSourceId = this->at(i-1).srcId;
			extensibles.push_back(vertexMap[this->at(i-1).srcId]);
			extensibles.push_back(vertexMap[this->at(i-1).destId]);
			break;
		}
	}

	for (int i = lastIndex; i > 0; i--){
		if (this->at(i-1).type == true && this->at(i-1).destId == lastSourceId) {
			lastSourceId = this->at(i-1).srcId;
			extensibles.push_back(vertexMap[this->at(i-1).srcId]);
		}
	}
	return extensibles;
}

std::pair<int, bool> PatternCode::getIndexMap(int n){
	for (int i = 0; i < (int)vertexMap.size(); i++)  {
		if (vertexMap[i]==n)
			return std::pair<int, bool> (i, true);
	}

	return std::pair<int, bool> (0, false);
}

void PatternCode::removeLast() {
	if (this->size()<=1) {
		this->reset();
		return;
	}
	if (this->back().type==true) {
		this->vertexMap.pop_back();
	}
	this->pop_back();
}

int PatternCode::compareTo(const PatternCode &code) const {

	//		System.out.println("#COMPARING: " + numEdges1 + " " + numEdges2);
	//		for (int j=0;j<numEdges1;j++)
	//			System.out.print(this->j]+"\t");
	//		System.out.println();
	//		for (int j=0;j<numEdges2;j++)
	//			System.out.print(code2[j]+"\t");
	//		System.out.println();
    
	int ret=0;
	int minSize = std::min(this->size(), code.size());
	for (int i = 0; i < minSize; i++) {
		ret = this->at(i).compareTo(code[i]);
		if (ret!=0) {
			break;
		}
	}
	if (ret<0) {
		if (this->size()<code.size())
			return -2;
		else 
			return -3;
	}
	else if (ret>0) {
		if (this->size()<code.size())
			return 2;
		else 
			return 3;
	}
	else {
		if (this->size()==code.size())
			return 0;
		else if (this->size()<code.size())
			return -1;
		else 
			return 1;
	}
}


bool PatternCode::operator==(const PatternCode &other) const
{ 
	if (this->compareTo(other)==0) return 1;
	else return 0;
}

void PatternCode::print(){
	std::cout << "PatternCode: ";
	for (int i = 0; i < (int)this->size(); i++) {
		this->at(i).print();
		std::cout << "\t";
	}
	std::cout << "VertexMap: ";
	for (int i = 0; i < (int)vertexMap.size(); i++) {
		std::cout << "vertexMap[" << i << "]:" << vertexMap[i]  << "\t";
	}

	std::cout << "\n";
}

PatternCode::iterator PatternCode::find(int srcId, int destId) {
	PatternCode::iterator it = this->begin();
	while (it!=this->end()) {
		if (it->srcId == srcId && it->destId==destId)
			break;
		it++;
	}
	return it;
}


