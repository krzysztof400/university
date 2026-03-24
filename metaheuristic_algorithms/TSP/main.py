from graph import Graph

western_sahara = Graph("data/wi29.tsp")


western_sahara.compute_mst()
western_sahara.compute_tsp_greedy()
western_sahara.visualize()
