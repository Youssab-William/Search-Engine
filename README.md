# Search Engine
This project is an implementation of a search engine using C++. It utilizes Google's PageRank algorithm to rank web pages for a given query. The project aims to provide a simple yet functional search engine to retrieve relevant web pages based on user queries.

# Algorithm Overview
The core of this search engine is Google's PageRank algorithm, which ranks web pages based on their importance and relevance. The algorithm works by analyzing the structure of the web, specifically the links between pages.
<br><br>
Here's an overview of the PageRank algorithm:<br>
<ul>
  <li>Each web page is treated as a node in a graph.</li>
  <li>Links between web pages are represented as edges in the graph.</li>
  <li>The algorithm assigns an initial rank to each web page.</li>
  <li>Iteratively, the algorithm redistributes the ranks among pages based on incoming links and their importance.</li>
  <li>The process continues until the ranks converge to stable values.</li>
</ul>

# Installation

1. Clone the GitHub repository to your local machine:
```
git clone https://github.com/Youssab-William/Search-Engine

```
2. Navigate to the project directory:
```
cd Search-Engine

```
3. Run the main cpp file:
```
g++ SearchEngine.cpp && ./a.out

```
