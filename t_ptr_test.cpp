#include "track_ptr.hpp"
#include <iostream>
#include <vector>
#include <cassert>

struct Widget 
	: public tracked<Widget>
{
	int val;

	Widget() : val(0) {}
	Widget(int v) : val(v) {}
};

void test_1()
{
	std::vector<Widget> comps;
	std::vector<track_ptr<Widget>> ptrs;

	for (int i = 0; i < 1000; ++i)
	{
		comps.emplace_back(i);
		ptrs.emplace_back(&comps.back());

		for (int j = 0; j <= i; ++j)
		{
			assert(ptrs[j]->val == j);
		}
	}

	comps.clear();

	for (auto& ptr : ptrs)
	{
		assert(ptr == nullptr);
	}
}

void test_2()
{
	std::vector<Widget> comps;
	comps.emplace_back(10);

	auto p = comps[0].get_ptr();
	auto p2 = std::move(p);
	auto p3 = p2;

	comps.resize(100);

	assert (p == nullptr);
	assert (p2.get() == &comps.front());
	assert (p2 == p3);

	comps.clear();

	assert (p2 == nullptr);
	assert (p3 == nullptr);
}

void test_3()
{
	Widget c (10);
	track_ptr<Widget> c_p (&c);
	assert (c_p->val == 10);
}

void test_4()
{
	Widget w;
	w.val = 10;
	auto w_p = w.get_ptr();

	Widget w2;
	w2.val = 15;
	w2 = w;

	assert(w_p != nullptr);
	assert(w_p->val == 10);

	w2 = std::move(w);

	assert(w_p != nullptr);
	assert(w_p->val == 10);
}

int main()
{
	test_1();
	test_2();
	test_3();
	test_4();
}