# cvec
Fast header-only vector implementation in C

# Example

```c
#include <assert.h>
#include "cvec.h"

int main(void){
	int* vec = NULL;
	cvec_init(&vec);
	for(int i = 0;i<20;i++){
		cvec_push_back(&vec,i);
	}
	assert(cvec_len(vec) == 20);
	for(int i = 0;i<10;i++){
		cvec_pop_back(&vec);
	}
	assert(cvec_len(vec) == 10);
	int arr[] = {10,11,12,13,14};
	cvec_insert_data(&vec,cvec_len(vec),arr,sizeof(arr)/sizeof(*arr));
	for(int i = 0;i<15;i++){
		assert(vec[i] == i);
	}
	cvec_destroy(vec);
}
```

# Usage

Functions (macros) taking `vecp` as an argument require the address of the vector. If they take `vec`, pass the vector. 

```
cvec_init(vecp)
```

Creates a new, empty, vector. `vecp` must be a the address of a non-void pointer set to NULL.

```
cvec_init_from_data(vecp,data,len)
```

Creates a new vector with data copied from `data`. `vecp` must be a the address of a non-void pointer set to NULL.

```
cvec_new_len(vecp,len)
```

Creates a new, empty, vector of size `len`. `vecp` must be a the address of a non-void pointer set to NULL.

```
cvec_new_filled(vecp,value,len)
```

Creates a new vector filled with `len` copies of `value`. `vecp` must be a the address of a non-void pointer set to NULL.

```
cvec_copy(vecp,vec)
```

Makes a copy of `vec`. `vecp` must be a the address of a non-void pointer set to NULL.

```
cvec_destroy(vec)
```

Frees `vec`.

```
cvec_push_back(vecp,element)
```

Adds `element` to the back of the vector.

```
cvec_pop_back(vecp)
```

Removes the last element from the vector.

```
cvec_insert_element(vecp,pos,element)
```

Inserts `element` into the vector at `pos`.

```
cvec_insert_data(vecp,pos,data,size)
```

Inserts array `data` of size `size` into the vector at `pos`.

```
cvec_insert_vec(vecp,pos,insert)
```

Inserts vector `insert` into the vector at `pos`.

```
cvec_erase(vecp,pos)
```

Removes element in vector at `pos`.

```
cvec_erase_range(vecp,start,end)
```

Removes elements in vector from \[`start`,`end`).

```
cvec_reserve(vecp,new_size)
```

Resizes vector to `new_size`

```
cvec_shrink(vecp)
```

Removes extra space in vector.

```
cvec_at(vec,i)
```

Bounds-checked access to `vec` at index `i` with `assert()`. Disable checks with `-DNDEBUG`.

```
cvec_empty(vec)
```

Checks if `vec` is empty.

```
cvec_len(vec)
```

Gets length of `vec`.

```
cvec_capacity(vec)
```

Gets capacity of `vec`.

```
cvec_clear(vec)
```

Removes all elements in `vec`.
