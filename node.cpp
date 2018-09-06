#include "node.h"
#include <iostream>

//Node Functions

Node::Node(int id, int l):id(id),label(l){
}

Node::Node(int id, int l, std::vector<int> e):id(id),label(l),extra(e){
}

int Node::getId() const{
        return this->id;
}

void Node::setId(int id){
        this->id = id;
}

int Node::getLabel() const {
        return this->label;
}

void Node::setLabel(int l){
        this->label = l;
}

std::vector<int> &Node::getExtra() {
        return this->extra;
}

void Node::setExtra(std::vector<int> &e){
        this->extra = e;
}

bool Node::compareIdEqual(const Node &a, const Node &b){
        return (a.id == b.id);
}

bool Node::compareLabelEqual(const Node &a, const Node &b){
        return (a.label == b.label);
}

bool Node::compareNodeEqual(const Node &a, const Node &b) {
        return (compareLabelEqual(a,b) && compareIdEqual(a,b));
}

void Node::print(){
        //std::cout << "====Node Id " <<  id << " label " << label << " index " << idxEdgeBegin << ":" << idxEdgeEnd << std::endl;
        std::cout << "====Node Id " <<  id << " label " << label << std::endl;
}



