#include "edge.h"
//Edge Functions

Edge::Edge():id(0), fromNodeId(0), toNodeId(0), weight(1), label(0){
}

Edge::Edge(int id, int fId, int tId, double w):id(id), fromNodeId(fId), toNodeId(tId), weight(w), label(0){
}

Edge::Edge(int id, int fId, int tId, int l):id(id), fromNodeId(fId), toNodeId(tId), weight(1), label(l){
}

Edge::Edge(int id, int fId, int tId, double w, int l):id(id), fromNodeId(fId), toNodeId(tId), weight(w), label(l){
}

int Edge::getId()const{
        return this->id;
}

void Edge::setId(int id){
        this->id = id;
}

int Edge::getFromNodeId()const{
        return this->fromNodeId;
}

int Edge::getDistinctNodeId(int id)const{
	if (this->fromNodeId!=id) return this->fromNodeId;
	else return this->toNodeId;
}

void Edge::setFromNodeId(int id){
        this->fromNodeId = id;
}

int Edge::getToNodeId()const{
        return this->toNodeId;
}

void Edge::setToNodeId(int id){
        this->toNodeId = id;
}

int Edge::getNodeIdDiffOf(int v) const {
	if (this->fromNodeId!=v) return this->fromNodeId;
	return this->toNodeId;
}

double Edge::getWeight()const{
        return this->weight;
}

void Edge::setWeight(double w){
        this->weight = w;
}

int Edge::getLabel()const{
        return this->label;
}

void Edge::setLabel(int l){
        this->label = l;
}

bool Edge::hasVertex(int v) {
	if (fromNodeId==v || toNodeId==v) return true;
	return false;
}

bool Edge::compareFromNodeId(const Edge &a, const Edge &b){
        return (a.fromNodeId < b.fromNodeId);
}

bool Edge::compareToNodeId(const Edge &a, const Edge &b){
        return (a.toNodeId < b.toNodeId);
}

bool Edge::compareToNodeIdEqual(const Edge &a, const Edge &b){
        return (a.toNodeId == b.toNodeId);
}

bool Edge::compareLabel(const Edge &a, const Edge &b){
        return (a.label < b.label);
}

bool Edge::compareWeight(const Edge &a, const Edge &b){
        return (a.weight < b.weight);
}

void Edge::print(){
        std::cout << "====edge fromNodeId " <<  fromNodeId <<  " toNodeId "  <<  toNodeId << " weight " << weight << " label " << label << std::endl;
}


