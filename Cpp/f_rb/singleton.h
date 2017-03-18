/*
 * Singleton.h
 *
 *  Created on: 2016??6??23??
 *      Author: duxiaohua
 */

#ifndef SRC_INC_SINGLETON_H_
#define SRC_INC_SINGLETON_H_

#include <memory>
#include <mutex>

using namespace std;

template<class T>
class Singleton
{
public:
	static inline T *instance()
	{
		if ( 0 == _instance.get())
		{
			lock_guard<std::mutex> lck(__locker);
			if ( 0 == _instance.get())
			{
				_instance.reset(new T);
			}
		}
		return _instance.get();
	}

private:
	Singleton()
	{
	}

	Singleton(const Singleton &)
	{
	}

	Singleton &operator=(const Singleton &)
	{
	}

	virtual ~Singleton()
	{
	}

	static unique_ptr<T> _instance;
	static mutex __locker;
};

template<class T>
mutex Singleton<T>::__locker;

template<class T>
unique_ptr<T> Singleton<T>::_instance;

#define DECLARE_SIGNLETON_CLASS(type) \
    friend class Singleton<type>; \
    friend class unique_ptr<type>;

#endif /* SRC_INC_SINGLETON_H_ */
