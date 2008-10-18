#ifndef _FIBONACCI_HEAP_
#define _FIBONACCI_HEAP_

extern "C"
{
#include "fib.h"
#include "fibpriv.h"
}

template<class T> class FibonacciHeap 
{
public:
	FibonacciHeap() 
	{
		init();
	}

	~FibonacciHeap() 
	{
		if(heap) fh_deleteheap(heap);
	}

	

	void clear()
	{
		if(heap) fh_deleteheap(heap);
		init();
	}

	unsigned int size() const 
	{ 
		return heap->fh_n; 
	}

	bool empty() const 
	{ 
		return heap->fh_n == 0; 
	}

	void push( T& item ) 
	{
		T *p = new T();
		*p = item;
		fh_insert(heap, (void *)p);
	}
	
	T& pop()
	{
		T *p = (T*)fh_extractmin(heap);
		temp = *p;
		delete p;
		return temp;
	}

	T& top()
	{
		return *(T*)fh_min(heap);
	}

	

private:
	void init()
	{
		heap = fh_makeheap();
		fh_setcmp(heap, FibonacciHeap::cmp);
	}

	static int cmp(void *x, void *y)
	{
		T &a = *(T*)x, &b = *(T*)y;		

		if (a < b)
			return -1;

		if (a == b)
			return 0;

		return 1;
	}

	T temp;
	struct fibheap *heap;
};

#endif //_FIBONACCI_HEAP_

