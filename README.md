==========================================================================
PROJECT ANTIGRAVITY: ADAPTIVE CIPHER-SHIFTING PROTOCOL (ACSP)
==========================================================================

[1.0] OVERVIEW
--------------------------------------------------------------------------
Project Antigravity is a high-performance, bare-metal communication 
protocol built in C++. It features a "shape-shifting" architecture that 
dynamically swaps encryption ciphers mid-stream based on real-time 
network latency.

[2.0] THE PROBLEM
--------------------------------------------------------------------------
Standard secure sockets (like TLS/SSL) negotiate a single cipher for an 
entire session. In high-speed environments (gaming, VoIP), a sudden 
latency spike combined with heavy encryption (AES-256) causes packet 
loss and "rubber-banding." 

[3.0] THE SOLUTION
--------------------------------------------------------------------------
This protocol uses a custom 2-bit header injected into the UDP datagram. 
By monitoring live ping, the system uses a mathematical decision engine 
to downshift to lighter cryptography (like Speck) during lag spikes to 
keep the connection alive, and snaps back to heavy security (AES) when 
the network recovers.

[4.0] THE OPTIMIZATION MATH
--------------------------------------------------------------------------
The Decision Engine uses the following Linear Algebra Cost Function:

J(c) = alpha * L(c) + beta * (1 / S(c))

Where:
- J(c)   : Total Cost (System selects the cipher with the lowest J)
- L(c)   : Total Latency (Current Ping * Cipher Multiplier)
- S(c)   : Security Score of the cipher
- alpha  : Weight for Speed (Latency priority)
- beta   : Weight for Security (Protection priority)

[5.0] REPOSITORY STRUCTURE
--------------------------------------------------------------------------
- server.cpp       : The UDP listener and bit-stripping node.
- client.cpp       : The interactive sender with IPC bridge.
- CipherEngine.cpp : The cryptographic routing hardware.
- optimizer.py     : The mathematical brain (Decision Engine).

[6.0] EXECUTION COMMANDS
--------------------------------------------------------------------------
To Compile:
g++ server.cpp CipherEngine.cpp -o server.exe -lws2_32
g++ client.cpp -o client.exe -lws2_32

To Run:
1. Start Server: cmd /c server.exe
2. Start Client: .\client.exe

==========================================================================
BUILD COMPLETE | STATUS: OPTIMIZED
==========================================================================
