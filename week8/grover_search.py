# -*- coding: utf-8 -*-
"""grover_search.ipynb

Automatically generated by Colab.

Original file is located at
    https://colab.research.google.com/drive/1ZWIbFvkiF_Xqeo-BdPNGT_Htcv48ufpq
"""

import numpy as np

def pushQubit(name,weights):
    global workspace
    global namestack
    if workspace.shape == (1,1):                  # if workspace empty
        namestack = []                            # then reset
    namestack.append(name)                        # push name
    weights = weights/np.linalg.norm(weights)     # normalize
    weights = np.array(weights,dtype=workspace[0,0].dtype)
    workspace = np.reshape(workspace,(1,-1))      # to row vector
    workspace = np.kron(workspace,weights)

def tosQubit(name):
  global workspace
  global namestack
  k = len(namestack)-namestack.index(name)    # qubit pos
  if k > 1:                                   # if non-trivial
    namestack.append(namestack.pop(-k))         # rotate name stack
    workspace = np.reshape(workspace,(-1,2,2**(k-1)))
    workspace = np.swapaxes(workspace,-2,-1)

def applyGate(gate,*names):
    global workspace
    if list(names) != namestack[-len(names):]: # reorder stack
        for name in names: # if necessary
            tosQubit(name)
    workspace = np.reshape(workspace,(-1,2**(len(names))))
    subworkspace = workspace[:,-gate.shape[0]:]
    np.matmul(subworkspace,gate.T,out=subworkspace)

X_gate = np.array([[0, 1],                      # Pauli X gate
                   [1, 0]])                     # = NOT gate

Y_gate = np.array([[ 0,-1j],                    # Pauli Y gate
                   [1j,  0]])                   # = SHZHZS

Z_gate = np.array([[1, 0],                      # Pauli Z gate
                   [0,-1]])                     # = P(pi) = S^2
                                                # = HXH

H_gate = np.array([[1, 1],                      # Hadamard gate
                   [1,-1]]) * np.sqrt(1/2)

S_gate = np.array([[1, 0],                      # Phase gate
                   [0,1j]])                     # = P(pi/2) = T^2

T_gate = np.array([[1,                0],       # = P(pi/4)
                   [0,np.exp(np.pi/-4j)]])

Tinv_gate = np.array([[1, 0],                   # = P(-pi/4)
                      [0,np.exp(np.pi/4j)]])    # = T^-1

def P_gate(phi):                                # Phase shift gate
    return np.array([[1,             0],
                     [0,np.exp(phi*1j)]])

def Rx_gate(theta):                             # Y rotation gate
    return np.array([[np.cos(theta/2),-1j*np.sin(theta/2)],
                     [-1j*np.sin(theta/2),np.cos(theta/2)]])

def Ry_gate(theta):                             # Y rotation gate return
    np.array([[np.cos(theta/2),-np.sin(theta/2)],
              [np.sin(theta/2), np.cos(theta/2)]])

def Rz_gate(theta):                             # Z rotation gate
    return np.array([[np.exp(-1j*theta/2),                0],
                     [                  0,np.exp(1j*theta/2)]])

CNOT_gate = np.array([[1, 0, 0, 0],             # Ctled NOT gate
                      [0, 1, 0, 0],             #=XORgate
                      [0, 0, 0, 1],
                      [0, 0, 1, 0]])

CZ_gate = np.array([[1, 0, 0, 0],               # Ctled Z gate
                    [0, 1, 0, 0],
                    [0, 0, 1, 0],
                    [0, 0, 0,-1]])

SWAP_gate = np.array([[1, 0, 0, 0],             # Swap gate
                      [0, 0, 1, 0],
                      [0, 1, 0, 0],
                      [0, 0, 0, 1]])

TOFF_gate = np.array([[1, 0, 0, 0, 0, 0, 0, 0], # Toffoli gate
                     [0, 1, 0, 0, 0, 0, 0, 0],
                     [0, 0, 1, 0, 0, 0, 0, 0],
                     [0, 0, 0, 1, 0, 0, 0, 0],
                     [0, 0, 0, 0, 1, 0, 0, 0],
                     [0, 0, 0, 0, 0, 1, 0, 0],
                     [0, 0, 0, 0, 0, 0, 0, 1],
                     [0, 0, 0, 0, 0, 0, 1, 0]])

def probQubit(name):
    global workspace
    tosQubit(name)
    workspace = np.reshape(workspace,(-1,2))
    prob = np.linalg.norm(workspace,axis=0)**2
    return prob/prob.sum()                 # make sure sum is one

def measureQubit(name):
    global workspace
    global namestack
    prob = probQubit(name)
    measurement = np.random.choice(2,p=prob)
    workspace = (workspace[:,[measurement]]/
                 np.sqrt(prob[measurement]))
    namestack.pop()
    return str(measurement)

def TOFF3_gate(q1,q2,q3,q4):
    applyGate(X_gate,q1,q2,q3,q4)

def TOFFn_gate(ctl,result):
    applyGate(X_gate,*ctl,result)

def zero_phaseOracle(qubits):            # all qubits zero?
    # if all qubits==0 return -weight else return weight
    for qubit in qubits:                 # negate all inputs
        applyGate(X_gate,qubit)
    applyGate(Z_gate,*namestack)         # controlled Z gate
    for qubit in qubits:                 # restore inputs
        applyGate(X_gate,qubit)

def sample_phaseOracle(qubits):          # sample function
        # if all f(x)==1 return -weight else return weight
    applyGate(X_gate,qubits[1])          # negate qubit 1
    applyGate(Z_gate,*namestack)         # controlled Z gate
    applyGate(X_gate,qubits[1])          # restore qubit 1

def groverSearch(n, printProb=True):
    optimalTurns = int(np.pi/4*np.sqrt(2**n)-1/2)   # iterations
    qubits = list(range(n))                         # generate qubit names
    for qubit in qubits:                            # initialize qubits
        pushQubit(qubit,[1,1])
    for k in range(optimalTurns):                   # Grover iterations:
        sample_phaseOracle(qubits)                  # apply phase oracle
        for qubit in qubits:                        # H-gate all qubits
            applyGate(H_gate,qubit)
        zero_phaseOracle(qubits)                    # apply 0 phase oracle
        for qubit in qubits:                        # H-gate all qubits
            applyGate(H_gate,qubit)
        if printProb:                               # peek probabilities
            print(probQubit(qubits[0]))             # to show convergence
    for qubit in reversed(qubits):                  # print result
        print(measureQubit(qubit),end="")

workspace = np.array([[1.]])              # initialize workspace
groverSearch(4)