# tabu-search-graph-coloring
In graph coloring, graph must be undirected. 

This console application takes file as an input. First line of file should contain number of nodes. The following lines should contain pairs of two nodes separated with space - representation of edge from one node to another. An example of an input is presented in files 'queen6.txt', 'miles250.txt', 'gc_1000_300013.txt', 'gc500.txt', 'le450_5a.txt' and 'zeroin.i.2.txt'.

After choosing data, program returns graph coloring using greedy algorithm. Then user has to type number to check if it is possible to color a graph with that amount of colors (which is checked using tabu-search metaheuristic). If it is possible, program returns representation node: color for each node. If it is not possible in specified time or specified number of iterations, program returns that information.
