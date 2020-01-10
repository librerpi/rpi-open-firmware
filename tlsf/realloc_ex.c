#include <string.h>
#include "tlsf.h"

/******************************************************************/
void *realloc_ex(void *ptr, size_t new_size, void *mem_pool)
{
/******************************************************************/
    tlsf_t *tlsf = (tlsf_t *) mem_pool;
    void *ptr_aux;
    unsigned int cpsize;
    bhdr_t *b, *tmp_b, *next_b;
    int fl, sl;
    size_t tmp_size;

    if (!ptr) {
        if (new_size)
            return (void *) malloc_ex(new_size, mem_pool);
        if (!new_size)
            return NULL;
    } else if (!new_size) {
        free_ex(ptr, mem_pool);
        return NULL;
    }

    b = (bhdr_t *) ((char *) ptr - BHDR_OVERHEAD);
    next_b = GET_NEXT_BLOCK(b->ptr.buffer, b->size & BLOCK_SIZE);
    new_size = (new_size < MIN_BLOCK_SIZE) ? MIN_BLOCK_SIZE : ROUNDUP_SIZE(new_size);
    tmp_size = (b->size & BLOCK_SIZE);
    if (new_size <= tmp_size) {
	TLSF_REMOVE_SIZE(tlsf, b);
        if (next_b->size & FREE_BLOCK) {
            MAPPING_INSERT(next_b->size & BLOCK_SIZE, &fl, &sl);
            EXTRACT_BLOCK(next_b, tlsf, fl, sl);
            tmp_size += (next_b->size & BLOCK_SIZE) + BHDR_OVERHEAD;
            next_b = GET_NEXT_BLOCK(next_b->ptr.buffer, next_b->size & BLOCK_SIZE);
            /* We allways reenter this free block because tmp_size will
               be greater then sizeof (bhdr_t) */
        }
        tmp_size -= new_size;
        if (tmp_size >= sizeof(bhdr_t)) {
            tmp_size -= BHDR_OVERHEAD;
            tmp_b = GET_NEXT_BLOCK(b->ptr.buffer, new_size);
            tmp_b->size = tmp_size | FREE_BLOCK | PREV_USED;
            next_b->prev_hdr = tmp_b;
            next_b->size |= PREV_FREE;
            MAPPING_INSERT(tmp_size, &fl, &sl);
            INSERT_BLOCK(tmp_b, tlsf, fl, sl);
            b->size = new_size | (b->size & PREV_STATE);
        }
	TLSF_ADD_SIZE(tlsf, b);
        return (void *) b->ptr.buffer;
    }
    if ((next_b->size & FREE_BLOCK)) {
        if (new_size <= (tmp_size + (next_b->size & BLOCK_SIZE))) {
			TLSF_REMOVE_SIZE(tlsf, b);
            MAPPING_INSERT(next_b->size & BLOCK_SIZE, &fl, &sl);
            EXTRACT_BLOCK(next_b, tlsf, fl, sl);
            b->size += (next_b->size & BLOCK_SIZE) + BHDR_OVERHEAD;
            next_b = GET_NEXT_BLOCK(b->ptr.buffer, b->size & BLOCK_SIZE);
            next_b->prev_hdr = b;
            next_b->size &= ~PREV_FREE;
            tmp_size = (b->size & BLOCK_SIZE) - new_size;
            if (tmp_size >= sizeof(bhdr_t)) {
                tmp_size -= BHDR_OVERHEAD;
                tmp_b = GET_NEXT_BLOCK(b->ptr.buffer, new_size);
                tmp_b->size = tmp_size | FREE_BLOCK | PREV_USED;
                next_b->prev_hdr = tmp_b;
                next_b->size |= PREV_FREE;
                MAPPING_INSERT(tmp_size, &fl, &sl);
                INSERT_BLOCK(tmp_b, tlsf, fl, sl);
                b->size = new_size | (b->size & PREV_STATE);
            }
			TLSF_ADD_SIZE(tlsf, b);
            return (void *) b->ptr.buffer;
        }
    }

    if (!(ptr_aux = malloc_ex(new_size, mem_pool))){
        return NULL;
    }

    cpsize = ((b->size & BLOCK_SIZE) > new_size) ? new_size : (b->size & BLOCK_SIZE);

    memcpy(ptr_aux, ptr, cpsize);

    free_ex(ptr, mem_pool);
    return ptr_aux;
}

/******************************************************************/
void *tlsf_realloc(void *ptr, size_t size)
{
/******************************************************************/
    void *ret;

#if USE_MMAP || USE_SBRK
	if (!mp) {
		return tlsf_malloc(size);
	}
#endif

    TLSF_ACQUIRE_LOCK(&((tlsf_t *)mp)->lock);

    ret = realloc_ex(ptr, size, mp);

    TLSF_RELEASE_LOCK(&((tlsf_t *)mp)->lock);

    return ret;
}
