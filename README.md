# cvec
Fast header-only vector implementation in C

# Example

```c
#include <assert.h>
#include "cvec.h"

int main(void){
  cvec_new(int,vec);
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
cvec_new(type,name)
```

Creates a new, empty, vector of type `type` named `name`.

```
cvec_new_from_data(type,name,data,len)
```

Creates a new vector of type `type` named `name` with data copied from `data`.

```
cvec_new_len(type,name,len)
```

Creates a new, empty, vector of type `type` named `name` of size `len`.

```
cvec_new_filled(type,name,value,len)
```

Creates a new vector of type `type` named `name` filled with `len` copies of `value`.

```
cvec_copy(name,vec)
```

Creates a copy of `vec` named `name`.

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
