from scipy.constants import c

distance_m = 100
time_ms = (distance_m / c) * 1e3

# mu = 1, beta = 1
slot_ms = 0.41667
subslot_ms = slot_ms / 2

print(f"Propagation delay: {time_ms} ms")
print(f"Subslot duration: {subslot_ms} ms")