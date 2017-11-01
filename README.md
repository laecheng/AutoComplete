# Efficient Auto-Complete
  - This is a program written in C++. It provides a very efficient algorithm for auto-complete of strings. The program is implemented with a trie and a well-designed heap-based topological sorting algorithm. It speeds up the naïve BFS/DFS searching approach over a factor of around 1000 when tested on a dictionary of size 90MB.

# Usage
  - First create a dictionary trie by using `DictionaryTrie * trie = new DictionaryTrie();`
  - Then insert strings with the relevant frequncy used for ranking into the trie, e.g., `trie->insert("cool", 100);`
  - And execuate an auto-complete with certain number of strings to show up (ranked by the given frequency of these strings), e.g., `trie->predictCompletions("co", 3);`, where "co" here is the specified prefix string.

# Environment
  - An environment to run **C++** on your device.
  - Follow this [guidance](https://gcc.gnu.org/install/) to install **C++** on your operating system.

# Install
```
$ git clone git@github.com:laecheng/AutoComplete .
```

# License
  - AutoComplete is Copyright © 2017-2018 chaoran cheng. It is free software, and may be redistributed under the terms specified in the [LICENSE](https://github.com/laecheng/AutoComplete/blob/master/LICENSE) file.


