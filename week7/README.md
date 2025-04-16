# week7
## How to run the code
To run the code use the following prompts in the command line:

    python3 HPQC/week7/quantum_com1.py
or

    python3 HPQC/week7/improved_quantum_com1.py
## Part 1: Push a qubit into a stack
When a new qubit is pushe the size of the stack doubles. Each new qubit is represented by a weight vectror (a,b) the weight satisfy this equations  ∣α∣ <sup>2</sup>  + ∣β∣ <sup>2</sup> =1, this makes sure that qubit only has a value of 0 or 1 once the wavefunction collapses.
To implement this the qubit workspace has to be reshaped into (1 x N <sup>2</sup>) a row vector and subsequently using the kron function in numpy to get the new weight vector.
## Part 2: Performin a gate operation
Gate operation are performed using matrix multiplication, they don't add or remove qubit but they change weight vectors. The gate opertion matrix shape (2<sup>M</sup> x 2<sup>M</sup>) where M is the nuber of qubits, and it can be shown all gates with M>2 can be represent by a sequence of gates with M≤2. To implemenet the gate function one has to reshape the qubit workspace to be (2<sup>N-M</sup> x 2<sup>M</sup>) and multiply it by the gate matrix of shape (2<sup>M</sup> x 2<sup>M</sup>). 

Examples of the gate operations are the Pauli x-gate or NOT gate which inverts the weight vector of a qubit or the Hadamard gate which can create an superpostion state with equal weght for 0 or 1 when a 0 or 1 qubit are inputted.
## Part 3: Moving a qubit to the top of the stack
The gate operations are only performed on the first qubit so a function is require to move a specific qubit to the top of the stack and move the rest down. To acheve this the swapaxes function from numpy can be used, but the first the workspace of the qubits has to be reshaped so that qubit that the qubit that has to be at the top of the stack is at the second last index of the array.
## Part 4: measuring a qubit
To measure a qubit one has to collapse the wavefunction. The qubits are represented as a weight vector each element, the square of the norm of each elemnt represnt the probablity of the qubit to be in that state. So to collapse the wavefunction and get an output of either 0 or 1 the random comand from numpy can be used. This command pick a random state (0 or 1) based on the probability of each state derived from the weights vector of that qubit. After a measurement is performed the qubit weight array size halves and the weights of the remainig qubits have to be renormalized.

An simulation of the AND gate can be done by using qubits by appliyng the Hadamrd gate to two 0 qubits so that the probaility of the 0 or 1 outcomes are equal and subsequently use a Toffoli gate on athird 0 qubit which performs the AND gate operation between qubit 1 and qubit 2. An AND gate truth table can be shown by printing all 3 measuremnts of the qubits.
The full truth table can take many iterations to be shown as the qubits are in a random state and the program could output the same measuremnt twice. This is one of the main problems with quantum computers they can most of the time get the result that is looked for but it might take multiple iterations. So it can output incorrect results due to it's probabilistic nature making it less accurate.
## Source 
Code and text taken from: https://enccs.se/tutorials/2024/02/quantum-computing-for-beginners-part-1/
