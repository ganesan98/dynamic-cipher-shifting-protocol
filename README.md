# Dynamic Cipher-Shifting Protocol

A UDP-based client-server system that dynamically selects encryption strategies based on real-time network latency.

---

## 🚀 What it does

- Monitors network latency (ping)
- Chooses an optimal cipher dynamically
- Embeds cipher ID into packet header
- Server decodes and processes accordingly

---

## 🧠 How it works

1. Client takes network latency as input  
2. Python script selects the best cipher  
3. Cipher ID is added to the packet header  
4. Server reads the header and processes the message  

---

## ⚙️ Tech Stack

- C++
- Python
- Winsock (UDP Networking)

---

## 🛠️ How to run

### Compile
```bash
g++ server.cpp CipherEngine.cpp -o server.exe -lws2_32
g++ client.cpp -o client.exe -lws2_32

RUN
# Terminal 1
server.exe

# Terminal 2
client.exe
