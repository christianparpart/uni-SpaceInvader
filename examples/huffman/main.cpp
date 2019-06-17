#include <sgfx/io_helper.hpp>

#include <array>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <queue>
#include <string>
#include <vector>
#include <unordered_map>

#include <cstddef>
#include <cstdint>

using sgfx::write_le;
using sgfx::read_le;

std::array<std::size_t,256> count_frequencies(std::ifstream& infile)
{
	std::array<std::size_t,256> counts{};
	using iter_t=std::istreambuf_iterator<char>;
	for(auto it=iter_t(infile); it!=iter_t{};++it)
		++counts[(unsigned char)(*it)];
	return counts;
}

struct huff_tree
{
	static constexpr auto invalid=std::numeric_limits<std::size_t>::max();
	struct huff_node
	{
		std::uint8_t val=0;
		std::size_t freq=0;
		std::size_t left=invalid, right=invalid;
		bool internal=false;
	};
	std::vector<huff_node> nodes;
	std::size_t root=0;
};

huff_tree build_tree(const std::array<std::size_t,256>& freqs)
{
	huff_tree ret_val;
	
	for(int i=0;i<256;++i)
		ret_val.nodes.push_back({static_cast<std::uint8_t>(i),freqs[i],huff_tree::invalid,huff_tree::invalid});
	
	auto comp=[&](std::size_t lhs, std::size_t rhs)
	{
		return ret_val.nodes[lhs].freq>ret_val.nodes[rhs].freq;
	};
	std::priority_queue<std::size_t, std::vector<std::size_t>, decltype(comp)> queue(comp);
	
	for(std::size_t i=0;i<256;++i)
		queue.push(i);
	
	while(queue.size()>1)
	{
		auto left=queue.top();
		queue.pop();
		auto right=queue.top();
		queue.pop();
		
		ret_val.nodes.push_back({0,ret_val.nodes[left].freq+ret_val.nodes[right].freq,left,right,true});
		queue.push(ret_val.nodes.size()-1);
	}
	
	ret_val.root=queue.top();
	
	return ret_val;
}

std::array<std::vector<bool>,256> build_mapping(const huff_tree& tree)
{
	struct treepart { std::size_t root; std::vector<bool> prefix; };
	std::queue<treepart> to_visit;
	to_visit.push({tree.root,{}});
	
	std::array<std::vector<bool>,256> mapping{};
	
	while(!to_visit.empty())
	{
		auto next=to_visit.front();
		to_visit.pop();
		
		if(tree.nodes[next.root].internal)
		{
			auto left_prefix=next.prefix;
			auto right_prefix=next.prefix;
			left_prefix.push_back(0);
			right_prefix.push_back(1);
			to_visit.push({tree.nodes[next.root].left,left_prefix});
			to_visit.push({tree.nodes[next.root].right,right_prefix});
		}
		else
		{
			mapping[tree.nodes[next.root].val]=next.prefix;
		}
	}
	
	return mapping;
}

void huffman_encode(const std::string& infilename, const std::string& outfilename)
{
	std::ifstream in{infilename};
	
	auto freq=count_frequencies(in);
	auto tree=build_tree(freq);
	auto mapping=build_mapping(tree);
	
	std::uint64_t filesize=0;
	for(auto f:freq)
		filesize+=f;
	
	std::ofstream out{outfilename};
	
	write_le(out, filesize);
	
	for(const auto& m:mapping)
	{
		std::uint16_t size=(std::uint16_t)m.size();
		write_le(out,size);
		
		std::uint8_t accumulator=0;
		std::uint8_t bit=0;
		for(bool b:m)
		{
			accumulator|=b<<bit;
			if(bit==7)
			{
				write_le(out,accumulator);
				accumulator=0;
				bit=0;
			}
			else
				++bit;
		}
		if(bit!=0)
			write_le(out,accumulator);
	}
	
	std::uint8_t accumulator=0;
	std::uint8_t bit=0;
	
	in.clear();
	in.seekg(0,std::ios::beg);
	using iter_t=std::istreambuf_iterator<char>;
	for(auto it=iter_t(in); it!=iter_t{};++it)
	{
		auto next=static_cast<std::uint8_t>(*it);
		const auto& m=mapping[next];
		
		for(bool b:m)
		{
			accumulator|=b<<bit;
			if(bit==7)
			{
				write_le(out,accumulator);
				accumulator=0;
				bit=0;
			}
			else
				++bit;
		}
	}
	if(bit!=0)
		write_le(out,accumulator);
}

void huffman_decode(const std::string& infilename, const std::string& outfilename)
{
	std::ifstream in{infilename};
	std::ofstream out{outfilename};
	
	auto filesize=read_le<std::uint64_t>(in);
		
	std::unordered_map<std::vector<bool>,std::uint8_t> mapping;
	for(int i=0;i<256;++i)
	{
		auto size=read_le<std::uint16_t>(in);
		
		std::uint8_t accumulator=0;
		std::vector<bool> result;
		for(;;)
		{
			accumulator=read_le<std::uint8_t>(in);
			for(std::uint8_t i=0;i<8 && size>0; ++i, --size)
			{
				result.push_back(accumulator&1);
				accumulator>>=1;
			}
			if(size==0)
				break;
		}
		
		mapping[result]=(std::uint8_t)i;
	}
	
	std::vector<bool> accumulator;
	using iter_t=std::istreambuf_iterator<char>;
	for(auto it=iter_t(in); it!=iter_t{};++it)
	{
		auto next=static_cast<std::uint8_t>(*it);
		for(std::uint8_t i=0;i<8;++i)
		{
			accumulator.push_back(next&1);
			next>>=1;
			if(auto it=mapping.find(accumulator); it!=mapping.end())
			{
				write_le(out,static_cast<std::uint8_t>(it->second));
				if(--filesize==0)
					return;
				accumulator.clear();
			}
		}
	}
}

int main(int argc, char* argv[])
{
	if(argc<2)
		return 1;
		
	huffman_encode(argv[1],argv[1]+std::string(".huff"));
	huffman_decode(argv[1]+std::string(".huff"),argv[1]+std::string(".huff.decoded"));
	
	return 0;
}
