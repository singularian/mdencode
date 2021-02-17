// fnv.h
// compute the FNV1 and FNV1a hash
// originally developed by Fowler, Noll and Vo
// http://isthe.com/chongo/tech/comp/fnv/
// https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function#FNV_prime

// const uint32_t Prime     = 0x01000193; //   16777619
// const uint32_t Offset64  = 0x811C9DC5; // 2166136261
const uint32_t Prime32   = 16777619;
const uint32_t Offset32  = 2166136261;

// const uint64_t Prime64   = 0x00000100000001B3
// const uint64_t Offset64  = 0xcbf29ce484222325
const uint64_t Prime64   = 1099511628211;
const uint64_t Offset64  = 14695981039346656037u;

const uint32_t FNV1_32_INIT = 0x811c9dc5;
const uint64_t FNV1_64_INIT = 0xcbf29ce484222325ULL;
const uint32_t FNV1_32A_INIT = 0x811c9dc5;
const uint64_t FNV1A_64_INIT = 0xcbf29ce484222325ULL;

// fnv32_1 hash of a block of memory
inline uint32_t fnv32_1(const void* data, size_t numBytes, uint32_t hash)
{
    const unsigned char* ptr = (const unsigned char*)data;
    while (numBytes--)
	hash = (hash * Prime32) ^ (*ptr++);
    return hash;
}

// fnv64_1 hash of a block of memory
inline uint64_t fnv64_1(const void* data, size_t numBytes, uint64_t hash)
{
    const unsigned char* ptr = (const unsigned char*)data;
    while (numBytes--)
      hash = (hash * Prime64) ^ (*ptr++);
    return hash;
}



// fnv32_1a hash a block of memory
inline uint32_t fnv32_1a(const void* data, size_t numBytes, uint32_t hash)
{
    const unsigned char* ptr = (const unsigned char*)data;
    while (numBytes--)
      hash = (*ptr++ ^ hash) * Prime32;
    return hash;
}

// fnv64_1a hash a block of memory
inline uint64_t fnv64_1a(const void* data, size_t numBytes, uint64_t hash)
{
    const unsigned char* ptr = (const unsigned char*)data;
    while (numBytes--)
      hash = (*ptr++ ^ hash) * Prime64;
    return hash;
}

