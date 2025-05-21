import matplotlib.pyplot as plt # type: ignore

# --- Part 1: Insertion & Search Times (in milliseconds) ---

operations = ['Insertion', 'Search']
trie_times = [1141, 771]
hashmap_times = [489, 301]

x = range(len(operations))
bar_width = 0.35

plt.figure(figsize=(10, 6))
plt.bar(x, trie_times, width=bar_width, label='Trie', color='skyblue')
plt.bar([i + bar_width for i in x], hashmap_times, width=bar_width, label='HashMap', color='lightgreen')

plt.xlabel('Operation')
plt.ylabel('Time (ms)')
plt.title('Trie vs HashMap - Insertion & Search Time')
plt.xticks([i + bar_width / 2 for i in x], operations)
plt.legend()
plt.grid(axis='y', linestyle='--', alpha=0.7)

plt.tight_layout()
plt.show()


# --- Part 2: LPM Search Time (in microseconds) ---

lpm_labels = ['Trie', 'HashMap']
lpm_times = [37, 115]

plt.figure(figsize=(6, 4))
bar_positions = range(len(lpm_labels))
plt.bar(bar_positions, lpm_times, width=0.5, color=['deepskyblue', 'limegreen'])

plt.xticks(bar_positions, lpm_labels)
plt.ylabel('Time (µs)')
plt.title('Trie vs HashMap - LPM Search Time')
plt.grid(axis='y', linestyle='--', alpha=0.7)

plt.tight_layout()
plt.show()
