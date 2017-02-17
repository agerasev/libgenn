#pragma once

#include <map>
#include <utility>

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

template <typename I, typename T>
struct GeneMap {
	std::map<I, T> map;
	
	int size() const {
		return map.size();
	}
	
	bool add(I id, T elem) {
		auto r = map.insert(std::make_pair(id, elem));
		return r.second;
	}
	
	bool del(I id) {
		auto r = map.erase(id);
		return r > 0;
	}
	
	T *find(I id) {
		auto it = map.find(id);
		if (it != map.end()) {
			return &it->second;
		}
		return nullptr;
	}
	
	const T *find(I id) const {
		auto it = map.find(id);
		if (it != map.end()) {
			return &it->second;
		}
		return nullptr;
	}
	
	T &get(I id) {
		return map.find(id)->second;
	}
	
	const T &get(I id) const {
		return map.find(id)->second;
	}
	
	template <typename Fn>
	void iter(Fn func) {
		for (auto &p : map) {
			func(p.first, p.second);
		}
	}
	
	template <typename Fn>
	void iter(Fn func) const {
		for (auto &p : map) {
			func(p.first, p.second);
		}
	}
	
	template <typename Fn>
	void del_iter(Fn func) {
		for (auto i = map.begin(); i != map.end();) {
			if (func(i->first, i->second)) {
				map.erase(i++);
			} else {
				++i;
			}
		}
	}
};

struct NetworkGene {
	GeneMap<NodeID, NodeGene> nodes;
	GeneMap<LinkID, LinkGene> links;
	
	int del_hanging_links() {
		int cnt = 0;
		
		links.del_iter([&] (LinkID id, LinkGene link) -> bool {
			if (nodes.find(id.src) == nullptr || nodes.find(id.dst) == nullptr) {
				cnt += 1;
				return true;
			}
			return false;
		});
		
		return cnt;
	}
	
	/*
	void hybrid(const NetworkGene &net) {
		
	}
	*/
};
