///////////////////////////////////////////////////////////////////////////////////
//																				 //
//  CTTree - A template-based class to handle binary height-balanced (AVL) trees //
//	(C) 1999 P. Niklaus, University of Basel, Switzerland						 //
//                                                                               //
//  This program is free software; you can redistribute it and/or modify         //
//  it under the terms of the GNU General Public License as published by         //
//  the Free Software Foundation; either version 2 of the License, or            //
//  (at your option) any later version.                                          //
//                                                                               //
//  This program is distributed in the hope that it will be useful,              //
//  but WITHOUT ANY WARRANTY; without even the implied warranty of               //
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                //
//  GNU General Public License for more details.                                 //
//                                                                               //
//  You should have received a copy of the GNU General Public License            //
//  along with this program; if not, write to the Free Software                  //
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA    //
//                                                                               //
//  P. Niklaus, email: Pascal.Niklaus@gmx.ch, subject "CTTree"                   //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
//                                                                               //
//  This template class wraps original algorithms by Donald Knuth, as            //
//  implemented in C in the avl-library of Ben Pfaff. Ben Pfaff's original       //
//  library as well as AVL-tree related algorithms can be found under			 //
//	http://www.msu.edu/user/pfaffben/avl. His library also implements red-black  //
//  trees, which I did not needed and did not implement therefore...			 //
//																			     //  
///////////////////////////////////////////////////////////////////////////////////
//																			     //
//	History:																	 //
//	20-nov-99: Release of version 1.0								             //
//																			     //
//	Bugs:																	     //
//	none known                                                                   //
//                                                                               //																		     //
///////////////////////////////////////////////////////////////////////////////////

#ifndef _CTTREE_H
#define _CTTREE_H

#include <stdlib.h>
#include <assert.h>

#ifndef TREE_MAX_HEIGHT
#define TREE_MAX_HEIGHT (32)
#endif

// Structure for a node in an AVL tree
template <class KEY, class VALUE> class TreeNode
{   	
public:
    TreeNode<KEY, VALUE> * link[2];			// Subtrees. 
    signed char bal;			// Balance factor. 
    char cache;					// Used during insertion. 

	//data
	KEY key;				
	VALUE value;
};

// Used for traversing an AVL tree. 
template <class KEY, class VALUE> struct TreeTraverser
{
    int init;					// Initialized? 
    int nstack;					// Top of stack.
    TreeNode<KEY, VALUE> *p;		// Used for traversal. 
    TreeNode<KEY, VALUE> *stack[TREE_MAX_HEIGHT];// Descended trees. 
};

// The CTTree template
template <class KEY, class VALUE> class CTTree
{

public:
	typedef int (*TreeCompareFunc) (const KEY& a, const KEY& b);
	typedef void (*TreeCallbackFunc) (const KEY& a);

	CTTree(TreeCompareFunc Func);
	virtual ~CTTree();

	VALUE* Add( const KEY& key, const VALUE& value );
	bool Remove( const KEY& key );
		
	

	VALUE* FindString( const char *key );
	VALUE* FindOrCreate(const KEY& key);
	VALUE* operator[] ( const KEY& key ); 

	KEY **GetKeyArray(); //maks
	VALUE **GetValueArray(); //maks
	
	//KEY * FindClose(KEY *item);

	void Clear(TreeCallbackFunc callback=NULL, TreeNode<KEY, VALUE> * TheRoot = NULL);
	//void Resort(TreeCompareFunc Func);
	
	void ProcessCallback(TreeCallbackFunc callback);
	
	//KEY * MoveSet(KEY *item);
	//KEY * MoveSetClose(KEY *item);

	void SetCompare(TreeCompareFunc Func) {Compare=Func;};

	
	unsigned long size() {return NumItems;} 
	bool empty() {return NumItems == 0;} 


	//-------------- data elements ----------------------------------
	TreeNode<KEY, VALUE> Root;	// Tree root node. 

private:
	unsigned long NumItems;				// Number of nodes in the tree.     

	
	TreeNode<KEY, VALUE> TempRoot;// Used for resort
    TreeCompareFunc Compare;	// Used to compare keys. 
    

	//maks: to avoid iterators and sppedup
	GlDynArray<KEY *>	keyArray; 
	GlDynArray<VALUE *> valueArray;
};


template < class KEY, class VALUE >
class CTTreeIterator
{
  public:
	  CTTreeIterator(const CTTree< KEY, VALUE >& _tree )	
	  { tree = &_tree; Traverser.init=0; item=0; } //maks

	void Begin() //maks		
	{ 
		Traverser.init=0;
		Next();
	}

	void Next()			
	{ 
		/* Uses Knuth's algorithm 2.3.1T (inorder Traverserersal). */
		if (Traverser.init == 0)
		{
			/* T1. */
			Traverser.init = 1;
			Traverser.nstack = 0;
			Traverser.p = tree->Root.link[0];
		}
		else
			/* T5. */
			Traverser.p = Traverser.p->link[1];
		
		for (;;)
		{
			/* T2. */
			while (Traverser.p != NULL)
			{
				/* T3. */
				Traverser.stack[Traverser.nstack++] = Traverser.p;
				Traverser.p = Traverser.p->link[0];
			}
			
			/* T4. */
			if (Traverser.nstack == 0)
			{
				Traverser.init = 0;
				item =  NULL;
				return;
			}

			Traverser.p = Traverser.stack[--Traverser.nstack];
			
			/* T5. */
			item =  Traverser.p;
			return;
		}
	}

	bool Done()			
	{ 
		return item == NULL; 
	} 

	TreeNode<KEY, VALUE> * MoveNext();

	void Current( KEY* key, VALUE* value )
	{
		if ( item )
		{
			*key = item->key;
			*value = item->value;
		}
	}

	KEY *Key() 
	{
		if(item)
			return &item->key;
		else 
			return NULL;
	}

	VALUE *Value() 
	{
		if(item)
			return &item->value;
		else 
			return NULL;
	}

  private:

	const CTTree< KEY, VALUE >* tree;
	TreeNode<KEY, VALUE> *item;

	TreeTraverser<KEY, VALUE> Traverser;
};



template <class KEY, class VALUE> CTTree<KEY, VALUE>::CTTree(TreeCompareFunc Func)
{
	NumItems=0;
	//assert(Func);
	Compare=Func;	
	Root.link[0] = NULL;
	Root.link[1] = NULL; 	
}

template <class KEY, class VALUE> CTTree<KEY, VALUE>::~CTTree()
{
	Clear();	// delete all elements
}

template < class KEY, class VALUE >
KEY **CTTree< KEY, VALUE >::GetKeyArray() //maks
{
	if(!keyArray.Memory())
	{
		//Create keyArray		
		int i = 0;
		keyArray.SetCount(NumItems + 1);

		if(NumItems)
		{
			CTTreeIterator<KEY, VALUE> it(*this);
			
			for(it.Begin(); !it.Done(); it.Next())
			{
				keyArray.SetItem(i++, it.Key());
			}
		}

		//Always include the terminator
		keyArray.SetItem(i++, NULL); 
	}	

	return keyArray.Memory();
}

template < class KEY, class VALUE >
VALUE **CTTree< KEY, VALUE >::GetValueArray() //maks
{
	if(!valueArray.Memory())
	{
		//Create valueArray		
		int i = 0;
		valueArray.SetCount(NumItems + 1);

		if(NumItems)
		{
			CTTreeIterator<KEY, VALUE > it(*this);
			
			for(it.Begin(); !it.Done(); it.Next())
			{
				valueArray.SetItem(i++, it.Value());
			}
		}

		//Always include the terminator
		valueArray.SetItem(i++, NULL); 
	}	

	return valueArray.Memory();
}


template <class KEY, class VALUE> VALUE* CTTree<KEY, VALUE>::Add( const KEY& key, const VALUE& value )
{
	
	// Uses Knuth's Algorithm 6.2.3A (balanced tree search and
	// insertion), but caches results of comparisons.  In empirical
	// tests this eliminates about 25% of the comparisons seen under
	// random insertions.  

	/* A1. */
	TreeNode<KEY, VALUE> *t = &Root;
	TreeNode<KEY, VALUE> *s, *p, *q, *r;

		
	s = p = t->link[0];
	
	if (s == NULL)
    {
		q = t->link[0] = new TreeNode<KEY, VALUE>;
		if(!q) return NULL;
		
		

		q->key = key;
		q->value = value;
		q->link[0] = q->link[1] = NULL;
		q->bal = 0;

		if(keyArray.Memory())
		{
			keyArray.SetCount(NumItems + 2); 
			keyArray.SetItem(NumItems, &q->key);
			keyArray.SetItem(NumItems + 1, NULL);
		}
		
		if(valueArray.Memory())
		{
			valueArray.SetCount(NumItems + 2); 
			valueArray.SetItem(NumItems, &q->value);
			valueArray.SetItem(NumItems + 1, NULL);
		}

		NumItems++;

		return &q->value;
    }
	
	for (;;)
    {
		/* A2. */
		int diff = Compare (key, p->key);
		
		/* A3. */
		if (diff < 0)
		{
			p->cache = 0;
			q = p->link[0];
			if (q == NULL)
			{
				p->link[0] = q = new TreeNode<KEY, VALUE>;
				if(!q) return NULL;
				break;
			}
		}
		/* A4. */
		else if (diff > 0)
		{
			p->cache = 1;
			q = p->link[1];
			if (q == NULL)
			{
				p->link[1] = q = new TreeNode<KEY, VALUE>;
				if(!q) return NULL;
				break;
			}
		}
		else
		{
			/* A2. */
			p->value = value;

			return &p->value;
		}
		
		/* A3, A4. */
		if (q->bal != 0)
			t = p, s = q;
		p = q;
    }

	/* A5. */
	q->key = key;
	q->value = value;
	q->link[0] = q->link[1] = NULL;
	q->bal = 0;

	//maks dont't clear the arrays here
	if(keyArray.Memory())
	{
		keyArray.SetCount(NumItems + 2); 
		keyArray.SetItem(NumItems, &q->key);
		keyArray.SetItem(NumItems + 1, NULL);
	}

	if(valueArray.Memory())
	{
		valueArray.SetCount(NumItems + 2); 
		valueArray.SetItem(NumItems, &q->value);
		valueArray.SetItem(NumItems + 1, NULL);
	}

	NumItems++;
	
	
	
	/* A6. */
	r = p = s->link[(int) s->cache];
	while (p != q)
    {
		p->bal = p->cache * 2 - 1;
		p = p->link[(int) p->cache];
    }
	
	/* A7. */
	if (s->cache == 0)
    {
		/* a = -1. */
		if (s->bal == 0)
		{
			s->bal = -1;
			return &q->value;
		}
		else if (s->bal == +1)
		{
			s->bal = 0;
			return &q->value;
		}
		
		//assert (s->bal == -1);
		if (r->bal == -1)
		{
			/* A8. */
			p = r;
			s->link[0] = r->link[1];
			r->link[1] = s;
			s->bal = r->bal = 0;
		}
		else
		{
			/* A9. */
			//assert (r->bal == +1);
			p = r->link[1];
			r->link[1] = p->link[0];
			p->link[0] = r;
			s->link[0] = p->link[1];
			p->link[1] = s;
			if (p->bal == -1)
				s->bal = 1, r->bal = 0;
			else if (p->bal == 0)
				s->bal = r->bal = 0;
			else 
			{
				//assert (p->bal == +1);
				s->bal = 0, r->bal = -1;
			}
			p->bal = 0;
		}
    }
	else
    {
		/* a == +1. */
		if (s->bal == 0)
		{
			s->bal = 1;
			return &q->value;
		}
		else if (s->bal == -1)
		{
			s->bal = 0;
			return &q->value;
		}
		
		//assert (s->bal == +1);
		if (r->bal == +1)
		{
			/* A8. */
			p = r;
			s->link[1] = r->link[0];
			r->link[0] = s;
			s->bal = r->bal = 0;
		}
		else
		{
			/* A9. */
			//assert (r->bal == -1);
			p = r->link[0];
			r->link[0] = p->link[1];
			p->link[1] = r;
			s->link[1] = p->link[0];
			p->link[0] = s;
			if (p->bal == +1)
				s->bal = -1, r->bal = 0;
			else if (p->bal == 0)
				s->bal = r->bal = 0;
			else 
			{
				//assert (p->bal == -1);
				s->bal = 0, r->bal = 1;
			}
			p->bal = 0;
		}
    }
	
	/* A10. */
	if (t != &Root && s == t->link[1])
		t->link[1] = p;
	else
		t->link[0] = p;
	
	
	return &q->value;
}





/*template <class KEY> KEY * CTTree<KEY>::MoveSetClose(KEY *item)
{
	//Move o ponteiro para o nó se encontrar
	//ou para o nó imediatamente superior

	TreeNode<KEY> *p;
	Traverser.nstack = 0;	
	
	p = Root.link[0];
	if (p == NULL)
		return NULL;
	
	int t;
	for (;;)
    {		
		int diff = Compare (item, p->data);

		if (diff < 0)
		{
			Traverser.stack[Traverser.nstack++] = p;	
			t = 0;				// walk left
		}
		else if (diff > 0)
			t = 1;				// walk right
		else
		{
			Traverser.p = p;
			Traverser.init = 1;
			return p->data;
		}
		
		if (p->link[t])			// if link exists, follow link
			p = p->link[t];
		else
		{			
			if(Traverser.nstack && p->data == Traverser.stack[Traverser.nstack-1]->data)
				--Traverser.nstack;
		
			Traverser.p = p;
			Traverser.init = 1;
		
			KEY *node = p->data;
			while(node && Compare(node, item) < 0)	node = MoveNext();

			return node;
		}		
    }
	
	return NULL;
}*/


/*template <class KEY> KEY * CTTree<KEY>::MoveSet(KEY *item)
{
	//Move o ponteiro para o nó se encontrar
	TreeNode<KEY> *p;
	Traverser.nstack = 0;	
	
	for (p = Root.link[0]; p; )
    {	
		int diff = Compare (item, p->data);

		if (diff < 0)
		{
			Traverser.stack[Traverser.nstack++] = p;
			p = p->link[0];
		}
		else if (diff > 0)
			p = p->link[1];
		else
		{
			Traverser.p = p;
			Traverser.init = 1;
			return p->data;
		}
    }
	
	return NULL;
}*/


// Search the tree an item matching <item>, and return it if found. 
template <class KEY, class VALUE> VALUE * CTTree<KEY, VALUE>::operator[] ( const KEY& key )
{
	TreeNode<KEY, VALUE> *p;
	
	for (p = Root.link[0]; p; )
    {
		int diff = Compare (key, p->key);
		
		if (diff < 0)
			p = p->link[0];
		else if (diff > 0)
			p = p->link[1];
		else
			return &p->value;
    }
	
	return NULL;
}


template <class KEY, class VALUE> VALUE * CTTree<KEY, VALUE>::FindString( const char *key )
{
	TreeNode<KEY, VALUE> *p;
	
	for (p = Root.link[0]; p; )
    {
		int diff = Compare (key, p->key);
		
		if (diff < 0)
			p = p->link[0];
		else if (diff > 0)
			p = p->link[1];
		else
			return &p->value;
    }
	
	return NULL;
}

template <class KEY, class VALUE> VALUE * CTTree<KEY, VALUE>::FindOrCreate( const KEY& key )
{
	TreeNode<KEY, VALUE> *p;
	
	for (p = Root.link[0]; p; )
    {
		int diff = Compare (key, p->key);
		
		if (diff < 0)
			p = p->link[0];
		else if (diff > 0)
			p = p->link[1];
		else
			return &p->value;
    }

	//Not found, add
	VALUE newValue;

	return Add(key, newValue);
}

template <class KEY, class VALUE> void CTTree<KEY, VALUE>::ProcessCallback(TreeCallbackFunc callback)
{
	/* Uses Knuth's algorithm 2.3.1T (inorder traversal). */
	assert (callback);
	
	/* T1. */
	TreeNode<KEY, VALUE> *an[TREE_MAX_HEIGHT];	/* Stack A: nodes. */
	TreeNode<KEY, VALUE> **ap = an;				/* Stack A: stack pointer. */
	TreeNode<KEY, VALUE> *p = Root.link[0];
	
	for (;;)
	{
		/* T2. */
		while (p != NULL)
		{
			/* T3. */
			*ap++ = p;
			p = p->link[0];
		}
		
		/* T4. */
		if (ap == an)
			return;
		p = *--ap;
		
		/* T5. */
		callback (p->key);
		p = p->link[1];
	}
	
}


template <class KEY, class VALUE> bool CTTree<KEY, VALUE>::Remove( const KEY& key )
{
	/* Uses Ben Pfaff's algorithm D, which can be found at
	http://www.msu.edu/user/pfaffben/avl.  Algorithm D is based on
	Knuth's Algorithm 6.2.2D (Tree deletion) and 6.2.3A (Balanced
	tree search and insertion), as well as the notes on pages 465-466
	of Vol. 3. */
	
	/* D1. */
	TreeNode<KEY, VALUE> *pa[TREE_MAX_HEIGHT];/* Stack P: Nodes. */
	char a[TREE_MAX_HEIGHT];				/* Stack P: Bits. */
	int k = 1;								/* Stack P: Pointer. */
	
	TreeNode<KEY, VALUE> **q;
	TreeNode<KEY, VALUE> *p;
	
	a[0] = 0;
	pa[0] = &Root;
	p = Root.link[0];
	for (;;)
    {
		/* D2. */
		if (p == NULL)
			return false;				// not found
		
		int diff = Compare (key, p->key);
		if (diff == 0)
			break;						// item found
		
		/* D3, D4. */
		pa[k] = p;
		if (diff < 0)
		{
			p = p->link[0];
			a[k] = 0;
		}
		else if (diff > 0)
		{
			p = p->link[1];
			a[k] = 1;
		}
		k++;
    }

	NumItems--;							// we have one item less
	
							// item = pointer to the data
	
	/* D5. */
	q = &pa[k - 1]->link[(int) a[k - 1]];
	if (p->link[1] == NULL)
    {
		*q = p->link[0];
		if (*q)
			(*q)->bal = 0;
    }
	else
    {
		/* D6. */
		TreeNode<KEY, VALUE> *r = p->link[1];
		if (r->link[0] == NULL)
		{
			r->link[0] = p->link[0];
			*q = r;
			r->bal = p->bal;
			a[k] = 1;
			pa[k++] = r;
		}
		else
		{
			/* D7. */
			TreeNode<KEY, VALUE> *s = r->link[0];
			int l = k++;
			
			a[k] = 0;
			pa[k++] = r;
			
			/* D8. */
			while (s->link[0] != NULL)
			{
				r = s;
				s = r->link[0];
				a[k] = 0;
				pa[k++] = r;
			}
			
			/* D9. */
			a[l] = 1;
			pa[l] = s;
			s->link[0] = p->link[0];
			r->link[0] = s->link[1];
			s->link[1] = p->link[1];
			s->bal = p->bal;
			*q = s;
		}
    }

	delete p;			// delete the node (not the item !!!)
	
	//assert (k > 0);
	/* D10. */
	while (--k)
    {
		TreeNode<KEY, VALUE> *s = pa[k], *r;
		
		if (a[k] == 0)
		{
			/* D10. */
			if (s->bal == -1)
			{
				s->bal = 0;
				continue;
			}
			else if (s->bal == 0)
			{
				s->bal = 1;
				break;
			}
			
			//assert (s->bal == +1);
			r = s->link[1];
			
			//assert (r != NULL);
			if (r->bal == 0)
			{
				/* D11. */
				s->link[1] = r->link[0];
				r->link[0] = s;
				r->bal = -1;
				pa[k - 1]->link[(int) a[k - 1]] = r;
				break;
			}
			else if (r->bal == +1)
			{
				/* D12. */
				s->link[1] = r->link[0];
				r->link[0] = s;
				s->bal = r->bal = 0;
				pa[k - 1]->link[(int) a[k - 1]] = r;
			}
			else 
			{
				/* D13. */
				//assert (r->bal == -1);
				p = r->link[0];
				r->link[0] = p->link[1];
				p->link[1] = r;
				s->link[1] = p->link[0];
				p->link[0] = s;
				if (p->bal == +1)
					s->bal = -1, r->bal = 0;
				else if (p->bal == 0)
					s->bal = r->bal = 0;
				else
				{
					//assert (p->bal == -1);
					s->bal = 0, r->bal = +1;
				}
				p->bal = 0;
				pa[k - 1]->link[(int) a[k - 1]] = p;
			}
		}
		else
		{
			//assert (a[k] == 1);
			
			/* D10. */
			if (s->bal == +1)
			{
				s->bal = 0;
				continue;
			}
			else if (s->bal == 0)
			{
				s->bal = -1;
				break;
			}
			
			//assert (s->bal == -1);
			r = s->link[0];
			
			if (r == NULL || r->bal == 0)
			{
				/* D11. */
				s->link[0] = r->link[1];
				r->link[1] = s;
				r->bal = 1;
				pa[k - 1]->link[(int) a[k - 1]] = r;
				break;
			}
			else if (r->bal == -1)
			{
				/* D12. */
				s->link[0] = r->link[1];
				r->link[1] = s;
				s->bal = r->bal = 0;
				pa[k - 1]->link[(int) a[k - 1]] = r;
			}
			else if (r->bal == +1)
			{
				/* D13. */
				p = r->link[1];
				r->link[1] = p->link[0];
				p->link[0] = r;
				s->link[0] = p->link[1];
				p->link[1] = s;
				if (p->bal == -1)
					s->bal = 1, r->bal = 0;
				else if (p->bal == 0)
					s->bal = r->bal = 0;
				else
				{
					//assert (p->bal == 1);
					s->bal = 0, r->bal = -1;
				}
				p->bal = 0;
				pa[k - 1]->link[(int) a[k - 1]] = p;
			}
		}
    }
	
	keyArray.Clear();
	valueArray.Clear();
	return true;
}

// Search the tree for an item close to the value of ITEM, and return it.
// This function will return a null pointer only if the tree is empty. 
/*template <class KEY> KEY * CTTree<KEY>::FindClose (KEY *item)
{
	const TreeNode *p;
	
	p = Root.link[0];
	if (p == NULL)
		return NULL;
	
	for (;;)
    {
		int diff = Compare (item, p->data);
		int t;
		
		if (diff < 0)
			t = 0;				// walk left
		else if (diff > 0)
			t = 1;				// walk right
		else
			return p->data;		// exact match
		
		if (p->link[t])			// if link exists, follow link
			p = p->link[t];
		else
			return p->data;		// there is no next item, 
								// return value at actual node
    }
}*/

template <class KEY, class VALUE> void CTTree<KEY, VALUE>::Clear(TreeCallbackFunc callback, TreeNode<KEY, VALUE> * TheRoot)
{
/*  Uses Knuth's Algorithm 2.3.1T as modified in exercise 13
	(postorder traversal). */
	
	/* T1. */
	TreeNode<KEY, VALUE> *an[TREE_MAX_HEIGHT];	// Stack A: nodes
	char ab[TREE_MAX_HEIGHT];					// Stack A: bits
	int ap = 0;									// Stack A: height
	TreeNode<KEY, VALUE> *p = (TheRoot?*TheRoot:Root).link[0];

	keyArray.Clear();
	valueArray.Clear();
	
	
	for (;;)
	{
		/* T2. */
		while (p != NULL)
		{
			/* T3. */
			ab[ap] = 0;
			an[ap++] = p;
			p = p->link[0];
		}
		
		/* T4. */
		for (;;)
		{
			if (ap == 0) return;
			
			p = an[--ap];
			if (ab[ap] == 0)
			{
				ab[ap++] = 1;
				p = p->link[1];
				break;
			}

			if(callback)			// if there is a callback function:
				callback(p->key);	//		call it
			//else					// if there is none
			//	delete p->data;		//		delete the data
			
			delete p;				// delete the node
		}
	}

	
}

/*template <class KEY> void CTTree<KEY>::Resort(TreeCompareFunc Func)
{
	TempRoot = Root;
	Root.link[0] = NULL;
	Root.link[1] = NULL; 
	NumItems=0;

	assert(Func);
	Compare=Func;

	//  Uses Knuth's Algorithm 2.3.1T as modified in exercise 13 (postorder traversal).
	
	// T1. 
	TreeNode<KEY> *an[TREE_MAX_HEIGHT];	// Stack A: nodes
	char ab[TREE_MAX_HEIGHT];					// Stack A: bits
	int ap = 0;									// Stack A: height
	TreeNode<KEY> *p = TempRoot.link[0];	

	for (;;)
	{
		// T2. 
		while (p != NULL)
		{
			// T3. 
			ab[ap] = 0;
			an[ap++] = p;
			p = p->link[0];
		}
		
		// T4. 
		for (;;)
		{
			if (ap == 0) return;
			
			p = an[--ap];
			if (ab[ap] == 0)
			{
				ab[ap++] = 1;
				p = p->link[1];
				break;
			}

			TryInsert(p->data);		// insert the data			
			delete p;				// delete the node
		}
	}
}*/

#endif // #ifndef _CTTREE_H

//---------------------------------- end of CTTree.h -----------------------------
