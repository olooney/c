
TODO
====

Rabin-Karp
----------

We can search about 40 MB of text in 1 second.
That's roughly equivalent to the regex for one pattern.
However, with multiple patterns, regex is slower.
If we can get the Rabin-Karp to handle multiple patterns,
then in theory it will be faster because it's O(N+M)

1. Ignore Case Option
2. Word Boundary Option
3. Multiple Patterns
4. Streaming

List
----

1. skip value == NULL in get / set / length
2. separate `list_init()`?

Dict
----

1. grow table dynamically
