#ifndef NODE_H
#define NODE_H

#include <stdlib.h>
#include <vector>
#include "utils.h"

struct Node {
        private:
        int id;
        int label;
        std::vector<int> extra;
//        int idxEdgeBegin;
//        int idxEdgeEnd;

        public:
        Node(int, int);
        Node(int, int, std::vector<int>);
        int getId() const;
        void setId(int);
        int getLabel() const;
        void setLabel(int);
        std::vector<int> &getExtra();
        void setExtra(std::vector<int>&);
        void print();
        static bool compareNodeEqual(const Node &, const Node &);
        static bool compareLabelEqual(const Node &, const Node &);
        static bool compareIdEqual(const Node &, const Node &);
};

#endif
