# week8
## Grover's search
Grover's search is a quantum algorith that find a specific value of a function f(X) that has been implemented with quantum gates and where x can be reprsent by N number of qubits. There is a specic x for which the function retruns 1 while all other values return 0 the Graver's search gives a quadratic speed to finding the solution with resect to a naive search, with the Grover's search requiring a ime of order O(2<sup>N/2</sup>) and the naive search is of oreder O(2<sup>N</sup>)

To implement Grover's search one has to define two new functions a zero_phaseOracle this function applies a phase change if the qubit returns a value of 1 while no phase change is applied to the if it reurns 0.

The second function is a sample_phaseOracle this function negates the weights of the input if it is the desired output, in the case of the grover_search.py program it negates the weghts if the inputted qubit is 111101 in binary fromat.

Grover's search isn't 100% accurate with the accuracy improving after each iteration with a maxim probability found after k iterations with k=π/4 * Sqrt(M) - 1/2 where M = 2<sup>N</sup> and N is the number of qubits.

## Source

Code and text taken from: https://enccs.se/tutorials/2024/02/quantum-computing-for-beginners-part-2-grovers-search/
