/* from https://github.com/DLTcollab/sse2neon/issues/472#issuecomment-894836735 */
static __inline__ uint64_t __rdtsc(void)
{
    uint64_t val;

    /* According to ARM DDI 0487F.c, from Armv8.0 to Armv8.5 inclusive, the
     * system counter is at least 56 bits wide; from Armv8.6, the counter
     * must be 64 bits wide.  So the system counter could be less than 64
     * bits wide and it is attributed with the flag 'cap_user_time_short'
     * is true.
     */
    asm volatile("mrs %0, cntvct_el0" : "=r" (val));

    return val;
}

// COPIED FROM SSE2NEON

// Free aligned memory that was allocated with _mm_malloc.
// https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_mm_free
static __inline__ void _mm_free(void *addr)
{
    free(addr);
}

// Allocate aligned blocks of memory.
// https://software.intel.com/en-us/
//         cpp-compiler-developer-guide-and-reference-allocating-and-freeing-aligned-memory-blocks
static __inline__ void *_mm_malloc(size_t size, size_t align)
{
    void *ptr;
    if (align == 1)
        return malloc(size);
    if (align == 2 || (sizeof(void *) == 8 && align == 4))
        align = sizeof(void *);
    if (!posix_memalign(&ptr, align, size))
        return ptr;
    return NULL;
}

/* Constants for use with _mm_prefetch.  */
enum _mm_hint {
    _MM_HINT_NTA = 0,  /* load data to L1 and L2 cache, mark it as NTA */
    _MM_HINT_T0 = 1,   /* load data to L1 and L2 cache */
    _MM_HINT_T1 = 2,   /* load data to L2 cache only */
    _MM_HINT_T2 = 3,   /* load data to L2 cache only, mark it as NTA */
    _MM_HINT_ENTA = 4, /* exclusive version of _MM_HINT_NTA */
    _MM_HINT_ET0 = 5,  /* exclusive version of _MM_HINT_T0 */
    _MM_HINT_ET1 = 6,  /* exclusive version of _MM_HINT_T1 */
    _MM_HINT_ET2 = 7   /* exclusive version of _MM_HINT_T2 */
};

// Loads one cache line of data from address p to a location closer to the
// processor. https://msdn.microsoft.com/en-us/library/84szxsww(v=vs.100).aspx
static __inline__ void _mm_prefetch(const void *p, int i)
{
    (void) i;
    __builtin_prefetch(p);
}

