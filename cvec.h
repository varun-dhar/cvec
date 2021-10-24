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

#define _CVEC_PHI ((long double)((1+sqrt(5))/2))

typedef struct{
	size_t length,capacity;
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

#define cvec_new(type,name) type* name = (type*)((_cvec_hdr*)CVEC_CALLOC(1,sizeof(_cvec_hdr))+1)

#define cvec_new_from_data(type,name,data,len)\
	type* name = (type*)((_cvec_hdr*)CVEC_MALLOC(sizeof(_cvec_hdr)+(len)*sizeof(type))+1);\
	memcpy(name,data,(len)*sizeof(type));\
	_cvec_get_header(name)->capacity = len;\
	_cvec_get_header(name)->length = len

#define cvec_new_len(type,name,len)\
	type* name = (type*)((_cvec_hdr*)CVEC_MALLOC(sizeof(_cvec_hdr)+(len)*sizeof(type))+1);\
	_cvec_get_header(name)->capacity = len;\
	_cvec_get_header(name)->length = 0

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
	_cvec_get_header(name)->length = _cvec_get_header(name)->capacity = len;\

#define cvec_copy(name,vec) cvec_new_from_data(typeof(*(vec)),name,vec,_cvec_get_header(vec)->length)

#define cvec_reserve(vecp,new_size)\
	*(vecp) = (typeof(*(vecp)))((_cvec_hdr*)CVEC_REALLOC(_cvec_get_header(*(vecp)), (new_size) * sizeof(**(vecp)) + sizeof(_cvec_hdr)) + 1);\
	_cvec_get_header(*(vecp))->capacity = new_size

#define cvec_shrink(vecp) cvec_reserve(vecp,_cvec_get_header(*(vecp))->length)

#define _cvec_check_grow(vecp)do{\
	if(_cvec_get_header(*(vecp))->length+1 > _cvec_get_header(*(vecp))->capacity){\
		cvec_reserve(vecp,(size_t)(_cvec_get_header(*(vecp))->length+1) * _CVEC_PHI);\
	}}while(0)

#define _cvec_check_shrink(vecp)do{\
	if((long double)_cvec_get_header(*(vecp))->length/_cvec_get_header(*(vecp))->capacity < 1 / _CVEC_PHI){\
		cvec_shrink(vecp);\
	}}while(0)

#define cvec_insert_element(vecp,pos,element)\
	_cvec_check_grow(vecp);\
	memmove(*(vecp)+((pos)+1),*(vecp)+(pos),(_cvec_get_header(*(vecp))->length-(pos))*sizeof(**(vecp)));\
	(*(vecp))[pos] = element;\
	_cvec_get_header(*(vecp))->length++

#define cvec_insert_data(vecp,pos,data,size)\
	_cvec_check_grow(vecp);\
	memmove(*(vecp)+(pos)+(size),*(vecp)+(pos),(_cvec_get_header(*(vecp))->length-(pos))*sizeof(**(vecp)));\
	memcpy(*(vecp)+(pos),data,(size)*sizeof(**(vecp)));\
	_cvec_get_header(*(vecp))->length+=size

#define cvec_insert_vec(vecp,pos,insert) cvec_insert_data(vecp, pos, insert, _cvec_get_header(insert)->length)

#define cvec_push_back(vecp,element)\
	_cvec_check_grow(vecp);\
	(*(vecp))[_cvec_get_header(*(vecp))->length] = element;\
	_cvec_get_header(*(vecp))->length++

#define cvec_pop_back(vecp)\
	_cvec_get_header(*(vecp))->length--;\
	_cvec_check_shrink(vecp)

#define cvec_erase(vecp,pos)\
	memmove(*(vecp)+(pos),*(vecp)+(pos)+1,(_cvec_get_header(*(vecp))->length-(pos)-1)*sizeof(**(vecp)));\
	_cvec_get_header(*(vecp))->length--;\
	_cvec_check_shrink(vecp)

#define cvec_erase_range(vecp,start,end)\
	memmove(*(vecp)+(start),*(vecp)+(end),(_cvec_get_header(*(vecp))->length-(end))*sizeof(**(vecp)));\
	_cvec_get_header(*(vecp))->length-=(end)-(start);\
	_cvec_check_shrink(vecp)

#define cvec_at(vec,i) (assert((i) < _cvec_get_header(vec)->length),(vec)[i])

#define cvec_empty(vec) (_cvec_get_header(vec)->length == 0)

#define cvec_len(vec) (_cvec_get_header(vec)->length)

#define cvec_capacity(vec) (_cvec_get_header(vec)->capacity)

#define cvec_clear(vec)	(_cvec_get_header(vec)->length = 0)

#define cvec_destroy(vec) free(_cvec_get_header(vec))

#endif
