#pragma once

#include <random>

#include "genetics.hpp"
#include "instance.hpp"

class RandEngine {
private:
	std::minstd_rand re;
	std::uniform_int_distribution<> ui;
	std::uniform_real_distribution<> ur;
	std::normal_distribution<> nr;
public:
	int int_() { return ui(re); }
	double unif() { return ur(re); }
	double norm() { return nr(re); }
};

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
			while (net->nodes.find(id) == net->nodes.end()) {
				id += 1;
			}
			net->nodes.insert(std::make_pair(id, NodeGene(rand.norm())));
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
		for(auto i = net->nodes.begin(); i != net->nodes.end();) {
			if(mnc <= 0 && (rand.int_() % rn) < dn) {
				net->nodes.erase(i++);
				dn -= 1;
			} else {
				++i;
			}
			rn -= 1;
			mnc -= 1;
		}
		
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
		
		for (auto &ps : net->nodes) {
			for (auto &pd : net->nodes) {
				LinkID lid(ps.first, pd.first);
				if (net->links.find(lid) == net->links.end()) {
					if ((rand.int_() % rl) < al) {
						net->links.insert(std::make_pair(lid, LinkGene(rand.norm())));
						al -= 1;
					}
					rl -= 1;
				}
			}
		}
		
		return count;
	}
	
	int del_rand_links(NetworkGene *net, int count) {
		int rl = net->links.size(); // remaining links
		if (count > rl) {
			count = rl;
		}
		int dl = count; // links to delete
		
		for (auto &ps : net->nodes) {
			for (auto &pd : net->nodes) {
				LinkID lid(ps.first, pd.first);
				if (net->links.find(lid) != net->links.end()) {
					if ((rand.int_() % rl) < dl) {
						net->links.erase(lid);
						dl -= 1;
					}
					rl -= 1;
				}
			}
		}
		
		return count;
	}
	
	void step_rand_weights(NetworkGene *net, double delta) {
		for(auto &p : net->nodes) {
			p.second.bias += delta*rand.norm();
		}
		for(auto &p : net->links) {
			p.second.weight += delta*rand.norm();
		}
	}
};
