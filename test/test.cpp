#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "boost/pool/object_pool.hpp"
#include "boost/pool/pool.hpp"

using std::vector;

struct mystruct {
public:
	int a;
	char b;
	double c;
} _Mystruct;

template<typename Char_t>
class Node {
public:
	Node() {}
	~Node() {
		printf("~Node() called.\n");
	}

	Char_t m_char;
	Node *m_pNext;
};

int main(int argc, char* argv[])
{
	std::vector<Node<char>, std::allocator<Node<char> > > v;
	std::vector<Node<char>, std::allocator<Node<char> > >::iterator it;

	printf("Pool allocators test\n");

	int size = 100;

	//boost::object_pool<mystruct> pool;
	boost::pool<> pool(sizeof(Node<char>));
	Node<char> *p;
	Node<char> *previous_p;
	for (size_t i=0;i < size;i++)
	{
		p = (Node<char> *)pool.malloc();
		p->m_char = 'a';
		if (previous_p != NULL)
			p->m_pNext = previous_p;

		previous_p = p;
	}

	pool.purge_memory();

    return 0;
}
