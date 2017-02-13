#pragma once

#include <genn/genetics.hpp>
#include <cmath>
#include <vector>


struct NodeInst {
	NodeID id;
	float in;
	float out;
	float bias;
	
	NodeInst() = default;
	
	NodeInst(NodeID id, const NodeGene &node) : id(id) {
		bias = node.bias;
	}
	
	void update() {
		out = tanhf(in + bias);
		in = 0;
	}
};

struct LinkInst {
	LinkID id;
	float weight;
	const NodeInst *src = nullptr;
	NodeInst *dst = nullptr;
	
	LinkInst() = default;
	
	LinkInst(LinkID id, const LinkGene &link) : id(id) {
		weight = link.weight;
	}
	
	void transmit() {
		dst->in += weight*src->out;
	}
};

class NetworkInst {
public:
	std::vector<NodeInst> nodes;
	std::vector<LinkInst> links;
	
	void build(const NetworkGene &net) {
		int i;
		nodes.resize(net.nodes.size());
		links.resize(net.links.size());
		
		std::map<NodeID, NodeInst*> index;
		i = 0;
		for(const std::pair<NodeID, NodeGene> &pair : net.nodes) {
			nodes[i] = NodeInst(pair.first, pair.second);
			index.insert(std::make_pair(pair.first, &nodes[i]));
			++i;
		}
		
		i = 0;
		for(const std::pair<LinkID, LinkGene> &pair : net.links) {
			auto src = index.find(pair.first.src);
			auto dst = index.find(pair.first.dst);
			if(src == index.end() || dst == index.end()) {
				continue;
			}
			links[i] = LinkInst(pair.first, pair.second);
			links[i].src = src->second;
			links[i].dst = dst->second;
			++i;
		}
	}
	
	void upload(NetworkGene &net) {
		for(NodeInst &n : nodes) {
			auto i = net.nodes.find(n.id);
			if (i != net.nodes.end()) {
				i->second.bias = n.bias;
			}
		}
		for(LinkInst &l : links) {
			auto i = net.links.find(l.id);
			if (i != net.links.end()) {
				i->second.weight = l.weight;
			}
		}
	}
	
	void step() {
		for(NodeInst &n : nodes) {
			n.update();
		}
		for(LinkInst &l : links) {
			l.transmit();
		}
	}
	
	void clear() {
		for(NodeInst &n : nodes) {
			n.in = 0;
			n.out = 0;
		}
	}
};
