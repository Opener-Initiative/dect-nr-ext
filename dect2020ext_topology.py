import networkx as nx
import matplotlib.pyplot as plt

nodes = []
edges = []

nodes.append(('FT_MAC[0]', 'FT', 'NOT_ASSOCIATED', -2.5, -2.5))
nodes.append(('FT_MAC[1]', 'FT', 'NOT_ASSOCIATED', 2.5, 2.5))
nodes.append(('FT_MAC[2]', 'FT', 'NOT_ASSOCIATED', -2.5, 2.5))
nodes.append(('FT_MAC[3]', 'FT', 'NOT_ASSOCIATED', 2.5, -2.5))
nodes.append(('PT_MAC[4]', 'PT', 'ASSOCIATED', 0.593288, 0))
edges.append(('FT_MAC[0]', 'PT_MAC[4]', {'power': -54.6652}))
nodes.append(('PT_MAC[5]', 'PT', 'ASSOCIATED', 0.204782, 0.148783))
edges.append(('FT_MAC[0]', 'PT_MAC[5]', {'power': -54.0223}))
nodes.append(('PT_MAC[6]', 'PT', 'ASSOCIATED', 0.0652053, 0.200681))
edges.append(('FT_MAC[1]', 'PT_MAC[6]', {'power': -52.4247}))
nodes.append(('PT_MAC[7]', 'PT', 'ASSOCIATED', -0.133882, 0.412046))
edges.append(('FT_MAC[1]', 'PT_MAC[7]', {'power': -52.4721}))
nodes.append(('PT_MAC[8]', 'PT', 'ASSOCIATED', -0.60619, 0.440423))
edges.append(('FT_MAC[1]', 'PT_MAC[8]', {'power': -53.8184}))
nodes.append(('PT_MAC[9]', 'PT', 'ASSOCIATED', -0.297441, 3.6426e-17))
edges.append(('FT_MAC[1]', 'PT_MAC[9]', {'power': -53.9047}))
nodes.append(('PT_MAC[10]', 'PT', 'ASSOCIATED', -0.749718, -0.544702))
edges.append(('FT_MAC[1]', 'PT_MAC[10]', {'power': -56.1378}))
nodes.append(('PT_MAC[11]', 'PT', 'ASSOCIATED', -0.0368464, -0.113402))
edges.append(('FT_MAC[1]', 'PT_MAC[11]', {'power': -53.5185}))
nodes.append(('PT_MAC[12]', 'PT', 'ASSOCIATED', 0.256875, -0.79058))
edges.append(('FT_MAC[1]', 'PT_MAC[12]', {'power': -54.6821}))
nodes.append(('PT_MAC[13]', 'PT', 'ASSOCIATED', 0.514287, -0.373651))
edges.append(('FT_MAC[1]', 'PT_MAC[13]', {'power': -52.9736}))

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
