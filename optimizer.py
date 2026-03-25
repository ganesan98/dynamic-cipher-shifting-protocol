import sys
import time

def calculate_optimal_cipher(baseline_ping_ms, silent=False):
    alpha = 1.0     
    beta = 1000.0   

    ciphers = {
        0: ("Tier 1 (AES-256)", 10.0, 1.5),
        1: ("Tier 2 (ChaCha20)", 7.0, 1.2),
        2: ("Tier 3 (Speck)", 5.0, 1.05)
    }

    best_cipher_id = 0
    lowest_cost = float('inf')

    if not silent:
        print(f"\n[NETWORK MONITOR] Current Ping: {baseline_ping_ms}ms")
    
    for cid, (name, security_score, lat_mult) in ciphers.items():
        total_latency = baseline_ping_ms * lat_mult
        cost = (alpha * total_latency) + (beta * (1.0 / security_score))
        
        if not silent:
            print(f" - Testing {name.ljust(18)}: Total Lag = {total_latency:<5.1f}ms | Math Cost = {cost:.2f}")

        if cost < lowest_cost:
            lowest_cost = cost
            best_cipher_id = cid

    if not silent:
        binary_id = format(best_cipher_id, '02b')
        best_name = ciphers[best_cipher_id][0]
        print(f">>> DECISION: Shifting to {best_name} (Header Flag: {binary_id}) <<<")
    else:
        # IPC MODE: Just print the raw integer ID so C++ can read it cleanly
        print(best_cipher_id)
        
    return best_cipher_id

def main():
    # IPC MODE: If C++ calls this script with a ping argument (e.g., "python optimizer.py 150")
    if len(sys.argv) > 1:
        try:
            live_ping = float(sys.argv[1])
            calculate_optimal_cipher(live_ping, silent=True)
        except ValueError:
            print(1) # Default to ChaCha20 on error
        return

    # SIMULATION MODE: If run normally via terminal
    simulated_network_pings = [20.0, 150.0, 400.0]
    print("=== Dynamic Cipher Optimization Engine ===")
    for ping in simulated_network_pings:
        calculate_optimal_cipher(ping)
        time.sleep(1.5)

if __name__ == "__main__":
    main()