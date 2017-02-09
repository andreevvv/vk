Tasks applied for a job position in vk.com.

Source code:
Task 1: 
  bsearch.c - binary search
Testcases:

Task 2:
  strsearch_avl.c - search the same string line using AVL tree.
  strsearch_trie.c - search the same string line using Trie tree.
Testcases:
  Test0: base64 /dev/urandom  | tr -dc 'a-zA-Z0-9' |  head -c 134217729 > test0.txt // more than 128 mb
  Test1: ? invalid chars
  Test2: base64 /dev/urandom  | tr -dc 'a-zA-Z0-9' |  head -c 134217728 > test2.txt  // 128 mb one string line
  Test3: base64 /dev/urandom  | tr -dc 'a-zA-Z0-9\n' |  head -c 134217728 > test3.txt   // 128 mb string lines
