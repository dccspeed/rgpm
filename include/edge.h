#ifndef EDGE_H
#define EDGE_H

#include <stdlib.h>
#include "utils.h"

struct Edge {
	public:
		int id;
		int fromNodeId;
		int toNodeId;
		double weight;
		int label;

	public:
		Edge();
		Edge(int, int, int);
		Edge(int, int, int, double);
		Edge(int, int, int, int);
		Edge(int, int, int, double, int);
		int getId() const;
		void setId(int);
		int getFromNodeId() const;
		void setFromNodeId(int);
		int getToNodeId() const;
		int getDistinctNodeId(int) const;
		void setToNodeId(int);
		int getNodeIdDiffOf(int) const;
		double getWeight() const;
		void setWeight(double);
		int getLabel() const;
		void setLabel(int);
		bool hasVertex(int);
		static bool compareFromNodeId(const Edge &, const Edge &);
		static bool compareToNodeId(const Edge &, const Edge &);
		static bool compareLabel(const Edge &, const Edge &);
		static bool compareWeight(const Edge &, const Edge &);
		static bool compareFromNodeIdEqual(const Edge &, const Edge &);
		static bool compareToNodeIdEqual(const Edge &, const Edge &);
		void print();

		friend std::ostream& operator<<(std::ostream&os, Edge& e)
		{
			os.setf(std::ios::fixed);
        		os << "====edge fromNodeId " <<  e.fromNodeId <<  " toNodeId "  <<  e.toNodeId << " weight " << e.weight << " label " << e.label << std::endl;
			return os;
		};

};

#endif
