kissdb
======

(Keep It) Simple Stupid Database

KISSDB is about the simplest key/value store you'll ever see, anywhere.
It's written in plain vanilla C using only the standard string and FILE
I/O functions, and should port to just about anything with a disk or
something that acts like one.

It stores keys and values of fixed length in a stupid-simple file format
based on fixed-size hash tables. If a hash collision occurrs, a new "page"
of hash table is appended to the database. The format is append-only.
There is no delete. Puts that replace an existing value, however, will not
grow the file as they will overwrite the existing entry.

Hash table size is a space/speed trade-off parameter. Larger hash tables
will reduce collisions and speed things up a bit, at the expense of memory
and disk space. A good size is usually about 1/2 the average number of
entries you expect.

Features:

 * Tiny, compiles to ~4k on an x86_64 Linux system
 * Small memory footprint (only caches hash tables)
 * Very space-efficient (on disk) if small hash tables are used
 * Makes a decent effort to be robust on power loss
 * Pretty respectably fast, especially given its simplicity
 * 64-bit, file size limit is 2^64 bytes
 * Ports to anything with a C compiler and stdlib/stdio
 * Public domain

Limitations:

 * Fixed-size keys and values, must recreate and copy to change any init size parameter
 * Add/update only, no delete
 * Iteration is supported but key order is undefined
 * No search for subsets of keys/values
 * No indexes
 * No transactions
 * No built-in thread-safety (guard it with a mutex in MT code)
 * No built-in caching of data (only hash tables are cached for lookup speed)
 * So minimal it doesn't store meta-data about the database, so init parameters must be the same for a given DB file
 * No endian-awareness (currently), so big-endian DBs won't read on little-endian machines

Alternative key/value stores and embedded databases:

 * SQLite (embedded SQL, public domain, much larger but very full-featured)
 * Kyoto Cabinet
 * GNU DBM (GDBM)
 * Berkeley DB
 * Memcached / MemcacheDB
 * NDBM (old-school but still usable)
 * (probably others, use the Google)

KISSDB is good if you want space-efficient relatively fast write-once/read-many storage
of keys mapped to values. Its not a good choice if you need searches, indexes, delete,
structured storage, or widely varying key/value sizes.

KISSDB is in the public domain. One reason it was written was the
poverty of simple key/value databases with wide open licensing. Even old
ones like GDBM have GPL, not LGPL, licenses.

See comments in kissdb.h for documentation. Makefile can be used to build
a test program on systems with gcc.

Author: Adam Ierymenko / ZeroTier Networks LLC
