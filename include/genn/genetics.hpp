#pragma once

#include <map>

typedef int NodeID;

struct NodeGene {
	float bias = 0.0f;
	NodeGene(float b = 0.0f) {
		bias = b;
	}
};

struct LinkID {
	NodeID src, dst;
	LinkID(NodeID s = 0, NodeID d = 0) {
		src = s;
		dst = d;
	}
	bool operator == (const LinkID &id) const {
		return src == id.src && dst == id.dst;
	}
	bool operator < (const LinkID &id) const {
		return src < id.src || (src == id.src && dst < id.dst);
	}
	bool operator <= (const LinkID &id) const {
		return src < id.src || (src == id.src && dst <= id.dst);
	}
	bool operator != (const LinkID &id) const { return !(*this == id); }
	bool operator >= (const LinkID &id) const { return !(*this < id); }
	bool operator > (const LinkID &id) const { return !(*this <= id); }
};

struct LinkGene {
	float weight = 0.0f;
	LinkGene(float w = 0.0f) {
		weight = w;
	}
};

struct NetworkGene {
	std::map<NodeID, NodeGene> nodes;
	std::map<LinkID, LinkGene> links;
	
	int del_hanging_links() {
		int cnt = 0;
		
		for(auto i = links.begin(); i != links.end();) {
			if(
				nodes.find(i->first.src) != nodes.end() && 
				nodes.find(i->first.dst) != nodes.end()
			) {
				++i;
			} else {
				links.erase(i++);
				cnt += 1;
			}
		}
		
		return cnt;
	}
	
	void hybridize(const NetworkGene &net) {
		// ...
	}
};
