#pragma once

#include <map>

typedef int NodeID;

struct Node {
	float bias = 0.0f;
	Node(float b = 0.0f) {
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

struct Link {
	float weight = 0.0f;
	Link(float w = 0.0f) {
		weight = w;
	}
};

struct Network {
	std::map<NodeID, Node> nodes;
	std::map<LinkID, Link> links;
	
	void hybridize(const Network &net) {
		// ...
	}
};
