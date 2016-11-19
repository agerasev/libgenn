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

class NetworkMutable : public Network {
public:
	float weight_delta = 1e-1;
	float bias_delta = 1e-1;
};

class Mutator {
public:
	float 
		add_node_prob = 0.002,
		del_node_prob = 0.002,
		add_link_prob = 0.02,
		del_link_prob = 0.02;
	
	int min_nodes = 0;
	
	RandEngine rand;
	
public:
	Mutator(int min_nodes_) {
		min_nodes = min_nodes_;
	}
	
	int mutate_nodes(NetworkMutable *net) {
		int nsize = net->nodes.size();
		bool naddf = rand.unif() < add_node_prob;
		bool ndelf = nsize > min_nodes && rand.unif() < del_node_prob;
		if(naddf || ndelf) {
			NodeID aid = 1, cid = 0;
			NodeID did = 0;
			int dcnt = ndelf ? min_nodes + (rand.int_() % (nsize - min_nodes)) : 0;
			int ccnt = nsize > 0 ? rand.int_() % nsize : 0;
			for(auto &p : net->nodes) {
				if(naddf) {
					if(p.first == aid) {
						aid += 1;
					}
					if(ccnt == 0) {
						cid = p.first;
					}
					ccnt -= 1;
				}
				if(ndelf) {
					if(dcnt == 0) {
						did = p.first;
					}
					dcnt -= 1;
				}
			}
			if(naddf) {
				net->nodes.insert(std::make_pair(aid, Node(rand.norm())));
				if(cid > 0) {
					net->links.insert(std::make_pair(LinkID(aid, cid), Link()));
				}
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
		return int(naddf) + int(ndelf);
	}
	
	int mutate_links(NetworkMutable *net) {
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
					net->links.insert(std::make_pair(aid, Link(rand.norm())));
				}
			}
			if(ldelf) {
				LinkID did(nid[2], nid[3]);
				net->links.erase(did);
			}
		}
		return int(laddf) + int(ldelf);
	}
	
	int mutate(NetworkMutable *net) {
		int ret = 0;
		ret += mutate_nodes(net);
		ret += mutate_links(net);
		
		double base = 1.2;
		double minv = 1e-6;
		double maxv = 1e0;
		
		for(auto &p : net->nodes) {
			p.second.bias += net->bias_delta*rand.norm();
		}
		net->bias_delta *= pow(base, 2*rand.unif() - 1);
		if(net->bias_delta < minv) {
			net->bias_delta = minv;
		} else if(net->bias_delta > maxv) {
			net->bias_delta = maxv;
		}
		
		for(auto &p : net->links) {
			p.second.weight += net->weight_delta*rand.norm();
		}
		net->weight_delta *= pow(base, 2*rand.unif() - 1);
		if(net->weight_delta < minv) {
			net->weight_delta = minv;
		} else if(net->weight_delta > maxv) {
			net->weight_delta = maxv;
		}
		
		return ret;
	}
};
