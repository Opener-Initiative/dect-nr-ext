import networkx as nx
import matplotlib.pyplot as plt

nodes = []
edges = []

nodes.append(('FT_MAC[0]', 'FT', 'NOT_ASSOCIATED', 0, 0))
nodes.append(('PT_MAC[1]', 'PT', 'ASSOCIATED', 36.4294, 0))
edges.append(('FT_MAC[0]', 'PT_MAC[1]', {'power': -83.5213}))
nodes.append(('PT_MAC[2]', 'PT', 'ASSOCIATED', -38.8272, 4.75496e-15))
edges.append(('FT_MAC[0]', 'PT_MAC[2]', {'power': -84.3518}))

graph = nx.Graph()

for name, type, status, x, y in nodes:
    graph.add_node(name, type=type, status=status, position=(x, y))

graph.add_edges_from(edges)

position = {name: (x, y) for name, type, status, x, y in nodes}

plt.figure(figsize=(14, 10))
nx.draw(graph, position, with_labels=True, node_size=2000, edge_color='red', font_size=12, font_weight='bold')

edge_labels = nx.get_edge_attributes(graph, 'power')
edge_labels = {k: f'{v:.1f} dBm' for k, v in edge_labels.items()}

nx.draw_networkx_edge_labels(graph, position, edge_labels, font_size=20)

plt.axis('on')
plt.grid(True)
plt.margins(0.4)
plt.show()
