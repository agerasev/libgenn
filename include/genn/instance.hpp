#pragma once

#include <genn/genetics.hpp>
#include <cmath>
#include <vector>

struct NodeOut {
	float out;
};

struct NodeIn {
	float in;
};

struct Node : public NodeIn, public NodeOut {
	float bias;
	
	Node(const Node &gene) {
		bias = gene.bias;
	}
	void update() {
		out = tanhf(in) + bias;
		in = 0;
	}
};

struct Link {
	float weight;
	const NodeOut *src = nullptr;
	NodeIn *dst = nullptr;
	
	Link(const Link &gene) {
		weight = gene.weight;
	}
	
	void transmit() {
		dst->in += weight*src->out;
	}
};

class Network {
public:
	std::vector<Node> nodes;
	std::vector<Link> links;
	
	std::vector<NodeIn *> inputs;
	std::vector<const NodeOut *> outputs;
	
	Network(const Network &gene) {
		std::map<NodeID, Node *> index;
		for(const std::pair<NodeID, Node> &pair : gene.nodes) {
			nodes.push_back(Node(pair.second));
			index.insert(std::make_pair(pair.first, &nodes.back()));
		}
		
		for(const std::pair<LinkID, Link> &pair : gene.links) {
			auto src = index.find(pair.first.src);
			auto dst = index.find(pair.first.dst);
			if(src == index.end() || dst == index.end()) {
				continue;
			}
			links.push_back(Link(pair.second));
			links.back().src = src->second;
			links.back().dst = dst->second;
		}
	}
};
