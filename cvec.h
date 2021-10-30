/*
   Copyright 2021 Varun Dhar

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#ifndef _CVEC_H
#define _CVEC_H
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>

#define _CVEC_PHI ((long double)((1+sqrt(5))/2))

typedef struct{
	size_t length,capacity;
	max_align_t _a[];
}_cvec_hdr;

void* _cvec_xcalloc(size_t nmemb,size_t size){
	void* p = calloc(nmemb,size);
	if(!p){puts("_cvec_xcalloc: allocation failed, exiting");exit(1);}
	return p;
}

void* _cvec_xmalloc(size_t size){
	void* p = malloc(size);
	if(!p){puts("_cvec_xmalloc: allocation failed, exiting");exit(1);}
	return p;
}

void* _cvec_xrealloc(void* ptr,size_t size){
	void* p = realloc(ptr,size);
	if(!p){puts("_cvec_xrealloc: allocation failed, exiting");exit(1);}
	return p;
}

#ifndef CVEC_CALLOC
#define CVEC_CALLOC _cvec_xcalloc
#endif
#ifndef CVEC_MALLOC
#define CVEC_MALLOC _cvec_xmalloc
#endif
#ifndef CVEC_REALLOC
#define CVEC_REALLOC _cvec_xrealloc
#endif

#define _cvec_get_header(vec) ((_cvec_hdr *)(vec) - 1)

#define cvec_len(vec) (_cvec_get_header(vec)->length)

#define cvec_capacity(vec) (_cvec_get_header(vec)->capacity)

#define cvec_new(type,name) type* name = (type*)((_cvec_hdr*)CVEC_CALLOC(1,sizeof(_cvec_hdr))+1)

#define cvec_new_from_data(type,name,data,len)\
	type* name = (type*)((_cvec_hdr*)CVEC_MALLOC(sizeof(_cvec_hdr)+(len)*sizeof(type))+1);\
	memcpy(name,data,(len)*sizeof(type));\
	cvec_capacity(name) = len;\
	cvec_len(name) = len

#define cvec_new_len(type,name,len)\
	type* name = (type*)((_cvec_hdr*)CVEC_MALLOC(sizeof(_cvec_hdr)+(len)*sizeof(type))+1);\
	cvec_capacity(name) = len;\
	cvec_len(name) = 0

#define cvec_new_filled(type,name,value,len)\
	type* name;\
	if(!(value)) {\
		(name) = (type*)((_cvec_hdr*)CVEC_CALLOC(1,sizeof(_cvec_hdr)+(len)*sizeof(type))+1);\
	}else{\
		(name) = cvec_new_len(len);\
		for(size_t i = 0;i<(len);i++){\
			(name)[i] = value;\
		}\
	}\
	cvec_len(name) = cvec_capacity(name) = len

#define cvec_copy(name,vec) cvec_new_from_data(typeof(*(vec)),name,vec,cvec_len(vec))

#define cvec_empty(vec) (cvec_len(vec) == 0)

#define cvec_clear(vec)	(cvec_len(vec) = 0)

#define cvec_destroy(vec) free(_cvec_get_header(vec))

#define cvec_reserve(vecp,new_size)do{\
	*(vecp) = (void*)((_cvec_hdr*)CVEC_REALLOC(_cvec_get_header(*(vecp)), (new_size) * sizeof(**(vecp)) + sizeof(_cvec_hdr)) + 1);\
	cvec_capacity(*(vecp)) = new_size;\
	}while(0)

#define cvec_shrink(vecp) cvec_reserve(vecp,cvec_len(*(vecp)))

#define _cvec_check_grow(vecp,size)do{\
	if(cvec_len(*(vecp))+(size) > cvec_capacity(*(vecp))){\
		cvec_reserve(vecp,(size_t)(cvec_len(*(vecp))+(size)) * _CVEC_PHI);\
	}}while(0)

#define _cvec_check_shrink(vecp)do{\
	if((long double)cvec_len(*(vecp))/cvec_capacity(*(vecp)) < 1 / _CVEC_PHI){\
		cvec_shrink(vecp);\
	}}while(0)

#define cvec_insert_element(vecp,pos,element)do{\
	_cvec_check_grow(vecp,1);\
	memmove(*(vecp)+((pos)+1),*(vecp)+(pos),(cvec_len(*(vecp))-(pos))*sizeof(**(vecp)));\
	(*(vecp))[pos] = element;\
	cvec_len(*(vecp))++;\
	}while(0)

#define cvec_insert_data(vecp,pos,data,size)do{\
	_cvec_check_grow(vecp,size);\
	memmove(*(vecp)+(pos)+(size),*(vecp)+(pos),(cvec_len(*(vecp))-(pos))*sizeof(**(vecp)));\
	memcpy(*(vecp)+(pos),data,(size)*sizeof(**(vecp)));\
	cvec_len(*(vecp))+=size;\
	}while(0)

#define cvec_insert_vec(vecp,pos,insert) cvec_insert_data(vecp, pos, insert, cvec_len(*(vecp)))

#define cvec_push_data_back(vecp,data,size) cvec_insert_data(vecp,cvec_len(*(vecp)),data,size)

#define cvec_push_vec_back(vecp,insert) cvec_insert_vec(vecp,cvec_len(*(vecp)),insert)

#define cvec_push_back(vecp,element)do{\
	_cvec_check_grow(vecp,1);\
	(*(vecp))[cvec_len(*(vecp))] = element;\
	cvec_len(*(vecp))++;\
	}while(0)

#define cvec_pop_back(vecp)do{\
	cvec_len(*(vecp))--;\
	_cvec_check_shrink(vecp);\
	}while(0)

#define cvec_erase(vecp,pos)do{\
	memmove(*(vecp)+(pos),*(vecp)+(pos)+1,(cvec_len(*(vecp))-(pos)-1)*sizeof(**(vecp)));\
	cvec_len(*(vecp))--;\
	_cvec_check_shrink(vecp);\
	}while(0)

#define cvec_erase_range(vecp,start,end)do{\
	memmove(*(vecp)+(start),*(vecp)+(end),(cvec_len(*(vecp))-(end))*sizeof(**(vecp)));\
	cvec_len(*(vecp))-=(end)-(start);\
	_cvec_check_shrink(vecp);\
	}while(0)

#define cvec_at(vec,i) (assert((i) < cvec_len(vec)),(vec)[i])

#endif
