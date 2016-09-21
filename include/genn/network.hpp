#pragma once

#include <vector>
#include <functional>


struct Node {
	float bias;
	
	Node() = default;
	Node(float b) {
		bias = b;
	}
};

struct Link {
	float weight;
	int src, dst;
	
	Link() = default;
	Link(int s, int d, float w = 0.0f) {
		src = s;
		dst = d;
		weight = w;
	}
};

class Network {
public:
	std::vector<Node> nodes;
	std::vector<Link> links;
	int nodes_count = 0, links_count = 0;
	int reserved = 0;
	
	std::vector<float> acts, prev_acts;
	
	Network() = default;
	Network(int res) {
		reserved = res;
		nodes_count = reserved;
		nodes.resize(nodes_count, Node(0.0f));
		
		acts.resize(nodes_count, 0.0f);
		prev_acts.resize(nodes_count, 0.0f);
	}
	Network(const Network &net) = default;
	
	Network &operator = (const Network &net) = default;
	
	void addNode(Node n = Node(0.0f)) {
		nodes.push_back(n);
		acts.push_back(0.0f);
		prev_acts.push_back(0.0f);
		nodes_count = nodes.size();
	}
	
	void delNode(int id) {
		if(id < reserved)
			return;
		
		for(int i = id + 1; i < int(nodes.size()); ++i) {
			nodes[i - 1] = nodes[i];
			acts[i - 1] = acts[i];
			prev_acts[i - 1] = prev_acts[i];
		}
		nodes.pop_back();
		acts.pop_back();
		prev_acts.pop_back();
		nodes_count = nodes.size();
		
		for(int i = 0, j = 0; i < int(links.size()); ++i) {
			Link &l = links[i];
			if(l.src == id || l.dst == id)
				continue;
			
			if(l.src > id)
				l.src -= 1;
			if(l.dst > id)
				l.dst -= 1;
			
			links[j] = l;
			++j;
		}
		links.resize(j);
		links_count = links.size();
	}
	
	void addLink(Link l) {
		links.push_back(l);
		links_count = links.size();
	}
	
	void delLink(int id) {
		for(int i = id + 1; i < int(links.size()); ++i) {
			links[i - 1] = links[i];
		}
		links.pop_back();
		links_count = links.size();
	}
};

template <typename TFRandInt, typename TFRandUnif, typename TFRandNorm>
class Mutator {
	float 
		add_node_prob,
		del_node_prob,
		add_link_prob,
		del_link_prob;
	
	float 
		weight_delta,
		bias_delta;
	
	void mutate_topology() {
		if(TFRandUnif() < add_node_prob)
			net->addNode();
		
		if(TFRandUnif() < del_node_prob && net->reserved < net->nodes_count)
			net->delNode((TFRandInt() % (net->nodes_count - net->reserved)) + net->reserved);
		
		if(TFRandUnif() < add_link_prob)
			net->addLink(Link(TFRandInt() % net->nodes_count, TFRandInt() % net->nodes_count));
		
		if(TFRandUnif() < del_link_prob)
			net->delLink(TFRandInt() % net->links_count);
	}
	
	void mutate(Network *net) {
		
		mutate_topology();
		
		for(Node &n : net->nodes) {
			n.bias += bias_delta*TFRandNorm();
		}
		
		for(Link &l : net->links) {
			l.weight += weight_delta*TFRandNorm();
		}
	}
};
