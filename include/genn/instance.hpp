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
		in = 0.0;
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
		net.nodes.iter([&] (NodeID id, const NodeGene &node) {
			nodes[i] = NodeInst(id, node);
			index.insert(std::make_pair(id, &nodes[i]));
			++i;
		});
		
		i = 0;
		net.links.iter([&] (LinkID id, const LinkGene &link) {
			auto src = index.find(id.src);
			auto dst = index.find(id.dst);
			if(src != index.end() && dst != index.end()) {
				links[i] = LinkInst(id, link);
				links[i].src = src->second;
				links[i].dst = dst->second;
				++i;
			}
		});
		links.resize(i);
	}
	
	void upload(NetworkGene *net) {
		for(NodeInst &n : nodes) {
			NodeGene *nptr = net->nodes.find(n.id);
			if (nptr != nullptr) {
				nptr->bias = n.bias;
			}
		}
		for(LinkInst &l : links) {
			LinkGene *lptr = net->links.find(l.id);
			if (lptr != nullptr) {
				lptr->weight = l.weight;
			}
		}
	}
	
	void load_from(const NetworkInst &inst) {
		for(int i = 0; i < int(nodes.size()); ++i) {
			nodes[i].bias = inst.nodes[i].bias;
		}
		for(int i = 0; i < int(links.size()); ++i) {
			links[i].weight = inst.links[i].weight;
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
