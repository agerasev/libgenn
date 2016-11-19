#pragma once

#include <genn/genetics.hpp>
#include <cmath>
#include <vector>


struct INode {
	float in;
	float out;
	float bias;
	
	INode() = default;
	
	INode(const Node &node) {
		bias = node.bias;
	}
	
	void update() {
		out = tanhf(in + bias);
		in = 0;
	}
};

struct ILink {
	float weight;
	const INode *src = nullptr;
	INode *dst = nullptr;
	
	ILink() = default;
	
	ILink(const Link &link) {
		weight = link.weight;
	}
	
	void transmit() {
		dst->in += weight*src->out;
	}
};

class INetwork {
public:
	std::vector<INode> nodes;
	std::vector<ILink> links;
	
	void build(const Network &net) {
		int i;
		nodes.resize(net.nodes.size());
		links.resize(net.links.size());
		
		std::map<NodeID, INode *> index;
		i = 0;
		for(const std::pair<NodeID, INode> &pair : net.nodes) {
			nodes[i] = INode(pair.second);
			index.insert(std::make_pair(pair.first, &nodes[i]));
			++i;
		}
		
		i = 0;
		for(const std::pair<LinkID, ILink> &pair : net.links) {
			auto src = index.find(pair.first.src);
			auto dst = index.find(pair.first.dst);
			if(src == index.end() || dst == index.end()) {
				continue;
			}
			links[i] = ILink(pair.second);
			links[i].src = src->second;
			links[i].dst = dst->second;
			++i;
		}
	}
	
	void step() {
		for(INode &n : nodes) {
			n.update();
		}
		for(ILink &l : links) {
			l.transmit();
		}
	}
	
	void clear() {
		for(INode &n : nodes) {
			n.in = 0;
			n.out = 0;
		}
	}
};
