#pragma once

#include <random>

#include "genetics.hpp"

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
	float 
		add_node_prob = 0.002,
		del_node_prob = 0.001,
		add_link_prob = 0.01,
		del_link_prob = 0.01;
	
	float 
		weight_delta = 1e-1,
		bias_delta = 1e-1;
	
	RandEngine rand;
	
public:
	void mutate_nodes(Network *net) {
		int nsize = net->nodes.size();
		bool naddf = rand.unif() < add_node_prob;
		bool ndelf = nsize > 0 && rand.unif() < del_node_prob;
		if(naddf || ndelf) {
			NodeID aid = 1;
			NodeID did = 0;
			int dcnt = ndelf ? rand.int_() % nsize : 0;
			for(auto &p : net->nodes) {
				if(naddf && p.first == aid) {
					aid += 1;
				}
				if(ndelf) {
					if(dcnt == 0) {
						did = p.first;
					}
					dcnt -= 1;
				}
			}
			if(naddf) {
				net->nodes.insert(std::make_pair(aid, Node()));
			}
			if(ndelf) {
				net->nodes.erase(did);
				for(auto ii = net->links.begin(); ii != net->links.end();) {
					if(ii->first.src != did && ii->first.dst != did) {
						++ii;
					} else {
						net->links.erase(ii++);
					}
				}
			}
		}
	}
	
	void mutate_links(Network *net) {
		int nsize = net->nodes.size();
		bool laddf = rand.unif() < add_link_prob;
		bool ldelf = rand.unif() < del_link_prob;
		if(nsize > 0 && (laddf || ldelf)) {
			NodeID nid[4];
			int ncnt[4];
			for(int i = 0; i < 4; ++i) { ncnt[i] = rand.int_() % nsize; }
			for(auto &p : net->nodes) {
				for(int i = 0; i < 4; ++i) {
					if(ncnt[i] == 0) {
						nid[i] = p.first;
					}
					ncnt[i] -= 1;
				}
			}
			if(laddf) {
				LinkID aid(nid[0], nid[1]);
				if(net->links.find(aid) == net->links.end()) {
					net->links.insert(std::make_pair(aid, Link()));
				}
			}
			if(ldelf) {
				LinkID did(nid[2], nid[3]);
				net->links.erase(did);
			}
		}
	}
	
	void mutate(Network *net) {
		mutate_nodes(net);
		mutate_links(net);
		
		for(auto &p : net->nodes) {
			p.second.bias += bias_delta*rand.norm();
		}
		
		for(auto &p : net->links) {
			p.second.weight += weight_delta*rand.norm();
		}
	}
};
