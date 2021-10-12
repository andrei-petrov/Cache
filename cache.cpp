//--------------------------------------------------------------------------------
//  Copyright: (C) 2021-2021 by Andrey Petrov. All rights reserved
//--------------------------------------------------------------------------------
//  Unit:        
//  Cooperation: 
//  Date:        2021-10-12
//--------------------------------------------------------------------------------
//  Description:
// 
//	CopyRignt LRU Cache with limited size
//	When we add new element to the cache the least recently added element can by dropped
//
//--------------------------------------------------------------------------------


#include "pch.h"
#include <cassert>
#include <iostream>
#include <list>
#include <map>
#include <unordered_map>

using namespace std;

template <typename K, typename V> class Cache
{
protected:
	size_t					m_nMaxSize;
	list<V>					m_list;	
	unordered_map<K, V*>	m_map;
	

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
		V* pVal = m_map.at(key);
		
		if (pVal)
		{
			V value = *pVal; // save it !

			//  forget old position
			typename list<V>::iterator it;  // consider 'using' next time
			it = find(m_list.begin(), m_list.end(), value);
			m_list.erase(it);

			//  set new position
			m_list.push_front(value);
			m_map[key] = &m_list.front();

			pVal = &m_list.front();
		}
		return pVal;
	}
	catch (exception ex)
	{
		cout << ex.what() << endl;		
	}
	return nullptr;
}


template <typename K, typename V>
void Cache<K, V>::Put(K key, V value)
{
	// not present in cache 
	if (m_map.find(key) == m_map.end())
	{
		// cache is full 
		if (m_list.size() == m_nMaxSize)
		{
			//delete least recently used element 
			V value = m_list.back();			
			typename unordered_map<K, V*>::iterator it;			
			for (it = m_map.begin(); it != m_map.end(); it++)
			{
				if ( *(it->second) == value )
				{
					m_map.erase(it);
					break;
				}
			}						
			/* not defined for using inside templates
			it = find_if(m_map.begin(), m_map.end(), 				
													[&value](const pair<K, V*>& p) -> bool
													{
														return ( *(p.second) == value );
													}				
						);			
			m_map.erase(it);
			*/
			m_list.pop_back();
		}
	}

	// present in cache 
	else
	{
		V* pVal = m_map[key];
		typename list<V>::iterator it;
		it = find(m_list.begin(), m_list.end(), (*pVal));		
		m_list.erase( it );
	}

	// update reference 
	m_list.push_front(value);
	m_map[key] = &m_list.front();
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

	pValue = cache.Get(3);
	if (pValue == nullptr)
	{
		std::cout << "The pair(3,3) not inserted yet" << endl;
	}

	cache.Put(3, 3); // 1, 1 will be erased

	pValue = cache.Get(3);
	assert(pValue);
	pValue = cache.Get(1);
	assert(pValue);

	pValue = cache.Get(2);
	if (pValue == nullptr)
	{
		std::cout << "Least recently used pair(2,2) erased" << endl;
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
