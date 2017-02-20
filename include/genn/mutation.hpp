#pragma once

#include "genetics.hpp"
#include "instance.hpp"
#include "random.hpp"


class Mutator {
public:
	int min_nodes = 0;
	RandEngine rand;
	
public:
	Mutator(int min_nodes_) {
		min_nodes = min_nodes_;
	}
	
	int add_rand_nodes(NetworkGene *net, int count) {
		NodeID id = 1;
		for (int i = 0; i < count; ++i) {
			while (net->nodes.find(id) == nullptr) {
				id += 1;
			}
			net->nodes.add(id, NodeGene(rand.norm()));
			id += 1;
		}
		
		return count;
	}
	
	int del_rand_nodes(NetworkGene *net, int count) {
		if (net->nodes.size() - count < min_nodes) {
			count = net->nodes.size() - min_nodes;
		}
		
		int mnc = min_nodes;
		int dn = count; // nodes to delete
		int rn = net->nodes.size(); // remaining nodes
		net->nodes.del_iter([&] (NodeID id, const NodeGene &node) -> bool {
			bool df = false;
			if(mnc <= 0 && (rand.int_() % rn) < dn) {
				dn -= 1;
				df = true;
			}
			rn -= 1;
			mnc -= 1;
			return df;
		});
		
		net->del_hanging_links();
		
		return count;
	}
	
	int add_rand_links(NetworkGene *net, int count) {
		int nn = net->nodes.size();
		int nl = net->links.size();
		int rl = nn*nn - nl; // remaining links
		if (count > rl) {
			count = rl;
		}
		int al = count; // links to add
		
		net->nodes.iter([&] (NodeID sid, const NodeGene &src) {
			net->nodes.iter([&] (NodeID did, const NodeGene &dst) {
				LinkID lid(sid, did);
				if (net->links.find(lid) == nullptr) {
					if ((rand.int_() % rl) < al) {
						net->links.add(lid, LinkGene(rand.norm()));
						al -= 1;
					}
					rl -= 1;
				}
			});
		});
		
		return count;
	}
	
	int del_rand_links(NetworkGene *net, int count) {
		int rl = net->links.size(); // remaining links
		if (count > rl) {
			count = rl;
		}
		int dl = count; // links to delete
		
		net->nodes.iter([&] (NodeID sid, const NodeGene &src) {
			net->nodes.iter([&] (NodeID did, const NodeGene &dst) {
				LinkID lid(sid, did);
				if (net->links.find(lid) != nullptr) {
					if ((rand.int_() % rl) < dl) {
						net->links.del(lid);
						dl -= 1;
					}
					rl -= 1;
				}
			});
		});
		
		return count;
	}
	
	void step_rand_weights(NetworkInst *net, double delta) {
		for (NodeInst &node : net->nodes) {
			node.bias += delta*rand.norm();
		}
		for (LinkInst &link : net->links) {
			link.weight += delta*rand.norm();
		}
	}
};
