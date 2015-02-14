abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ
x if else struct void inline using namespace int bool case new for return switch break while and do delete
#include <iostream>

using namespace std;

struct node
{
	int value;
	node *next;
}

void add(int number, node *&where)
{
	if ( where ) {
		where->next=new node;
		where=where->next;
		where->value=number;
		where->next=NULL;
	} else {
		where=new node;
		where->value=number;
		where->next=NULL;
	}
}

void input(node *&in)
{
	int number;
	cin >> number;
	in=new node;
	in->value=number;
	in->next=NULL;
	if ( number!=0 ) {
		cin >> number;
		while ( number!=0 ) {
			node *iter;
			if ( number<in->value ) {
				iter=new node;
				iter->value=number;
				iter->next=in;
				in=iter;
			} else if ( number>in->value ) {
				node *prev=in;
				iter=in->next;
				while ( iter and iter->value<number ) {
					prev=iter;
					iter=iter->next;
				}
				if ( !iter or iter->value!=number ) {
					prev->next=new node;
					prev->next->value=number;
					prev->next->next=iter;
				}
			}
			cin >> number;
		}
	}
}

inline void output(node *out)
{
	do {
		cout << out->value << " ";
		out=out->next;
	} while ( out );
	cout << endl;
}

inline void clear(node *target)
{
	node *tmp;
	do {
		tmp=target->next;
		delete(target);
		target=tmp;
	} while ( target );
}

void m_union(node *a, node *b, node *&result)
{
	if ( a->value==0 ) {
		add(b->value, result);
		node *iter=result;
		b=b->next;
		while ( b ) {
			add(b->value, iter);
			b=b->next;
		}
	} else if ( b->value==0 ) {
		add(a->value, result);
		node *iter=result;
		a=a->next;
		while ( a ) {
			add(a->value, iter);
			a=a->next;
		}
	} else {
		if ( a->value<b->value ) {
			add(a->value, result);
			a=a->next;
		} else if ( a->value>b->value ) {
			add(b->value, result);
			b=b->next;
		} else {
			add(a->value, result);
			a=a->next;
			b=b->next;
		}
		node *iter=result;
		while ( a and b ) {
			while ( a and a->value<b->value ) {
				add(a->value, iter);
				a=a->next;
			}
			if ( a ) {
				while ( b and b->value<a->value) {
					add(b->value, iter);
					b=b->next;
				}
			}
			if ( a and b and a->value==b->value ) {
				add(a->value, iter);
				a=a->next;
				b=b->next;
			}
		}
		if ( a ) {
			do {
				add(a->value, iter);
				a=a->next;
			} while ( a );
		} else if ( b ) {
			do {
				add(b->value, iter);
				b=b->next;
			} while ( b );
		}
	}
}

void m_complement(node *a, node *b, node *&result)
{
	if ( a->value==0 ) {
		add(0, result);
	} else if ( b->value==0 ) {
		add(a->value, result);
		node *iter=result;
		a=a->next;
		while ( a ) {
			add(a->value, iter);
			a=a->next;
		}
	} else {
		if ( a->value<b->value ) {
			add(a->value, result);
			a=a->next;
		} else {
			while ( a and b and b->value<=a->value ) {
				while ( b and b->value<a->value )
					b=b->next;
				while ( a and b and b->value==a->value ) {
					a=a->next;
					b=b->next;
				}
			}
			if ( a ) {
				add(a->value, result);
				a=a->next;
			} else {
				add(0, result);
			}
		}
		node *iter=result;
		while ( a and b ) {
			while ( a and a->value<b->value ) {
				add(a->value, iter);
				a=a->next;
			}
			if ( a ) {
				while ( b and b->value<a->value)
					b=b->next;
			}
			if ( a and b and a->value==b->value ) {
				a=a->next;
				b=b->next;
			}
		}
		if ( a ) {
			do {
				add(a->value, iter);
				a=a->next;
			} while ( a );
		}
	}
}

void m_intersect(node *a, node *b, node *&result)
{
	if ( a->value==0 or b->value==0 ) {
		add(0, result);
	} else {
		if ( a->value==b->value ) {
			add(a->value, result);
			a=a->next;
			b=b->next;
		} else {
			while ( a and b and a->value!=b->value ) {
				while ( a and a->value<b->value )
					a=a->next;
				if ( a ) {
					while ( b and b->value<a->value )
						b=b->next;
				}
				if ( a and b ) {
					add(a->value, result);
					a=a->next;
					b=b->next;
				} else {
					add(0, result);
				}
			}
		}
		node *iter=result;
		while ( a and b ) {
			while ( a and a->value<b->value )
				a=a->next;
			if ( a ) {
				while ( b and b->value<a->value )
					b=b->next;
			}
			while ( a and b and a->value==b->value ) {
				add(a->value, iter);
				a=a->next;
				b=b->next;
			}
		}
	}
}

void m_symdif(node *a, node *b, node *&result)
{
	if ( a->value==0 ) {
		add(b->value, result);
		node *iter=result;
		b=b->next;
		while ( b ) {
			add(b->value, iter);
			b=b->next;
		}
	} else if ( b->value==0 ) {
		add(a->value, result);
		node *iter=result;
		a=a->next;
		while ( a ) {
			add(a->value, iter);
			a=a->next;
		}
	} else {
		if ( a->value<b->value ) {
			add(a->value, result);
			a=a->next;
		} else if ( a->value>b->value ) {
			add(b->value, result);
			b=b->next;
		} else {
			while ( a and b and a->value==b->value ) {
				a=a->next;
				b=b->next;
			}
			if ( a ) {
				if ( b ) {
					if ( a->value<b->value ) {
						add(a->value, result);
						a=a->next;
					} else {
						add(b->value, result);
						b=b->next;
					}
				} else {
					add(a->value, result);
					a=a->next;
				}
			} else if ( b ) {
				add(b->value, result);
				b=b->next;
			} else {
				add(0, result);
			}
		}
		node *iter=result;
		while ( a and b ) {
			while ( a and a->value<b->value ) {
				add(a->value, iter);
				a=a->next;
			}
			if ( a ) {
				while ( b and b->value<a->value) {
					add(b->value, iter);
					b=b->next;
				}
			}
			if ( a and b and a->value==b->value ) {
				a=a->next;
				b=b->next;
			}
		}
		if ( a ) {
			do {
				add(a->value, iter);
				a=a->next;
			} while ( a );
		} else if ( b ) {
			do {
				add(b->value, iter);
				b=b->next;
			} while ( b );
		}
	}
}

int main()
{
	ios_base::sync_with_stdio(0);
	int il_zad;
	cin >> il_zad;
	for (int i=0; i<il_zad;i++ ) {
		node *zb1=NULL, *zb2=NULL, *zb3=NULL;
		input(zb1);
		input(zb2);
		input(zb3);
		int select;
		cin >> select;
		while ( select!=0 ) {
			node *op1, *op2, *result=NULL;
			switch ( select ) {
				case 1: op1=zb1; break;
				case 2: op1=zb2; break;
				case 3: op1=zb3; break;
			}
			char action;
			cin >> action >> select;
			switch ( select ) {
				case 1: op2=zb1; break;
				case 2: op2=zb2; break;
				case 3: op2=zb3; break;
			}
			switch ( action ) {
				case 'U': m_union(op1, op2, result); break;
				case 'C': m_complement(op1, op2, result); break;
				case 'I': m_intersect(op1, op2, result); break;
				case 'S': m_symdif(op1, op2, result); break;
			}
			cin >> action >> select;
			op1=result;
			result=NULL;
			switch ( select ) {
				case 1: op2=zb1; break;
				case 2: op2=zb2; break;
				case 3: op2=zb3; break;
			}
			switch ( action ) {
				case 'U': m_union(op1, op2, result); break;
				case 'C': m_complement(op1, op2, result); break;
				case 'I': m_intersect(op1, op2, result); break;
				case 'S': m_symdif(op1, op2, result); break;
			}
			output(result);
			clear(op1);
			clear(result);
			cin >> select;
		}
		for (int i=0;i<4;i++) {
			int x;
			cin >> x;
		}
		clear(zb1);
		clear(zb2);
		clear(zb3);
	}
	return 0;
}
