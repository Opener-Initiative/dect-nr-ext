import networkx as nx
import matplotlib.pyplot as plt

nodes = []
edges = []

nodes.append(('FT0', 'FT', 'NOT_ASSOCIATED', 0, 0))
nodes.append(('PT0', 'PT', 'ASSOCIATED', 36.4294, 0))
edges.append(('FT0', 'PT0', {'power': 10}))
nodes.append(('PT1', 'PT', 'ASSOCIATED', -38.8272, 4.75496e-15))
edges.append(('FT0', 'PT1', {'power': 10}))

graph = nx.Graph()

for name, type, status, x, y in nodes:
    graph.add_node(name, type=type, status=status, position=(x, y))

graph.add_edges_from(edges)

position = {name: (x, y) for name, type, status, x, y in nodes}

plt.figure(figsize=(10, 6))
nx.draw(graph, position, with_labels=True, node_size=700, edge_color='red')

edge_labels = nx.get_edge_attributes(graph, 'power')
edge_labels = {k: f'{v:.1f} dBm' for k, v in edge_labels.items()}

nx.draw_networkx_edge_labels(graph, position, edge_labels)

plt.axis('on')
plt.grid(True)
plt.show()