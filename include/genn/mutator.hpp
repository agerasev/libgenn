#pragma once

template <typename fRandNorm>
class NodeMutator {
	
};

template <typename fRandNorm>
class LinkMutator {
	
};

template <typename fRandInt, typename fRandUnif, typename fRandNorm>
class Mutator {
	
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
	
	void mutate_topology(Network *net) {
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
		
		mutate_topology(net);
		
		for(Node &n : net->nodes) {
			n.bias += bias_delta*TFRandNorm();
		}
		
		for(Link &l : net->links) {
			l.weight += weight_delta*TFRandNorm();
		}
	}
};
