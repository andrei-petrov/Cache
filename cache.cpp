#include "pch.h"
#include <cassert>
#include <iostream>
#include <map>

using namespace std;


// 
//	Cache with limited size
//	When we add new element to the cache the least recently added element can by dropped
//



template <typename K, typename V> class Cache
{
protected:
	size_t		m_nMaxSize;
	map<K, V>	m_map;
	//K&			m_rLast;

public:
	Cache(size_t nMaxSize)
	{
		m_nMaxSize = nMaxSize;
	}
	V*		Get(K key);
	void	Put(K key , V value);
};

template <typename K, typename V>
V* Cache<K, V>::Get(K key)
{	
	try
	{
		V& value = m_map.at(key);
		return &value;
	}
	catch (std::exception ex)
	{
		cout << ex.what() << endl;		
	}
	return nullptr;
}


template <typename K, typename V>
void Cache<K, V>::Put(K key, V value)
{
	if (m_map.size() == m_nMaxSize)
	{
		m_map.erase(m_map.begin());
	}
	m_map.insert(pair<K, V>(key, value));
}


int main()
{
	std::cout << "Hello Cache! started\n" << endl;

	Cache <int, int> cache(2);

	cache.Put(1, 1);
	cache.Put(2, 2);

	int* pValue = nullptr;
		
	pValue = cache.Get(1);
	assert(pValue);
	pValue = cache.Get(2);
	assert(pValue);

	pValue = cache.Get(3);
	if (pValue == nullptr)
	{
		std::cout << "The pair(3,3) not inserted yet" << endl;
	}

	cache.Put(3, 3); // 1, 1 will be erased

	pValue = cache.Get(3);
	assert(pValue);
	pValue = cache.Get(1);
	if (pValue == nullptr)
	{
		std::cout << "Least recently inserted pair(1,1) erased" << endl;
	}

	std::cout << endl << "Hello Cache! finished\n" << endl;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
