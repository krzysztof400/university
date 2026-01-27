import sys
from collections import deque
import pulp

class FlowNetwork:
    """
    Common flow network class for Edmonds-Karp and future extensions.
    """
    def __init__(self, n):
        self.n = n
        # Each entry in adj: [to_node, capacity, reverse_edge_index, original_capacity]
        self.adj = [[] for _ in range(n)]
        self.paths_found = 0

    def add_edge(self, u, v, capacity):
        # Forward edge
        self.adj[u].append([v, capacity, len(self.adj[v]), capacity])
        # Backward (residual) edge
        self.adj[v].append([u, 0, len(self.adj[u]) - 1, 0])

    def edmonds_karp(self, s, t):
        max_flow = 0
        self.paths_found = 0
        
        while True:
            parent = [-1] * self.n
            edge_from_idx = [-1] * self.n
            queue = deque([s])
            parent[s] = s
            
            # BFS to find the shortest augmenting path
            path_found = False
            while queue:
                u = queue.popleft()
                if u == t:
                    path_found = True
                    break
                for i, (v, cap, rev, _) in enumerate(self.adj[u]):
                    if parent[v] == -1 and cap > 0:
                        parent[v] = u
                        edge_from_idx[v] = i
                        queue.append(v)
                if path_found: break
            
            if not path_found:
                break
            
            self.paths_found += 1
            # Determine path bottleneck capacity
            path_flow = float('inf')
            curr = t
            while curr != s:
                p = parent[curr]
                idx = edge_from_idx[curr]
                path_flow = min(path_flow, self.adj[p][idx][1])
                curr = p
            
            # Update residual capacities
            max_flow += path_flow
            curr = t
            while curr != s:
                p = parent[curr]
                idx = edge_from_idx[curr]
                rev_idx = self.adj[p][idx][2]
                self.adj[p][idx][1] -= path_flow
                self.adj[curr][rev_idx][1] += path_flow
                curr = p
        
        return max_flow

    def get_flow_on_edges(self):
        """Returns flow values for all original edges."""
        edge_flows = []
        for u in range(self.n):
            for v, cap, rev, orig_cap in self.adj[u]:
                if orig_cap > 0: # Only consider original forward edges
                    flow = orig_cap - cap
                    edge_flows.append((u, v, flow))
        return edge_flows

    def export_lp_model(self, filename, s, t):
        """
        Generates an LP model for the max flow problem using PULP and saves it to a file.
        Compatible with GLPK.
        """
        prob = pulp.LpProblem("MaxFlowProblem", pulp.LpMaximize)
        
        # Create variables for flow on each edge
        # We need a unique name for each edge variable: flow_u_v
        # Note: Handling parallel edges might be tricky if not careful, but assuming simple graph for now or unique variable per edge index.
        # FlowNetwork stores edges in adjacency list.
        
        flow_vars = {}
        
        # Iterate over all edges to create variables
        for u in range(self.n):
            for i, (v, cap, rev, orig_cap) in enumerate(self.adj[u]):
                if orig_cap > 0: # Only forward edges defined in problem
                    var_name = f"f_{u}_{v}"
                    # Check if parallel edges exist; if so, append index
                    if var_name in flow_vars:
                         var_name = f"f_{u}_{v}_{i}"
                    
                    flow_vars[(u, v, i)] = pulp.LpVariable(var_name, 0, orig_cap)

        # Objective: Maximize flow out of source
        # Sum of flows exiting s
        source_out_flow = []
        for (u, v, idx), var in flow_vars.items():
            if u == s:
                source_out_flow.append(var)
        
        # We should also subtract flow entering s if graph is cyclic, but usually flow out of s is enough.
        # But strictly: Maximize (Sum(f_sv) - Sum(f_vs))
        
        source_in_flow = []
        for (u, v, idx), var in flow_vars.items():
            if v == s:
                source_in_flow.append(var)
                
        prob += pulp.lpSum(source_out_flow) - pulp.lpSum(source_in_flow), "Max Flow"

        # Constraints: Flow conservation for all nodes except s and t
        for node in range(self.n):
            if node == s or node == t:
                continue
            
            flow_in = []
            flow_out = []
            
            # Find all edges entering node
            for (u, v, idx), var in flow_vars.items():
                if v == node:
                    flow_in.append(var)
                if u == node:
                    flow_out.append(var)
            
            prob += pulp.lpSum(flow_in) == pulp.lpSum(flow_out), f"Conservation_{node}"

        # Write to file
        # The user wants "comments allowing to understand the model".
        # PULP writes standard LP format which uses variable names.
        prob.writeLP(filename)
        return prob

    def shortest_augmenting_path(self, s, t):
        n = self.n
        d = [n] * n
        current_arc = [0] * n
        
        # Backward BFS initialization
        q = deque([t])
        d[t] = 0
        
        # Build reverse graph for BFS
        rev_adj = [[] for _ in range(n)]
        for u in range(n):
            for v, cap, rev, _ in self.adj[u]:
                if cap > 0:
                    rev_adj[v].append(u)
                    
        while q:
            u = q.popleft()
            for v in rev_adj[u]:
                if d[v] == n:
                    d[v] = d[u] + 1
                    q.append(v)
        
        # Gap heuristic count array
        count = [0] * (n + 1)
        for i in range(n):
            if d[i] < n:
                count[d[i]] += 1
        
        pred = [-1] * n
        pred_edge_idx = [-1] * n
        i = s
        max_flow = 0
        self.paths_found = 0
        
        while d[s] < n:
            admissible = False
            # Search for admissible arc using current_arc optimization
            while current_arc[i] < len(self.adj[i]):
                idx = current_arc[i]
                v, cap, rev, _ = self.adj[i][idx]
                if cap > 0 and d[i] == d[v] + 1:
                    admissible = True
                    break
                current_arc[i] += 1
            
            if admissible:
                # Advance
                idx = current_arc[i]
                v = self.adj[i][idx][0]
                pred[v] = i
                pred_edge_idx[v] = idx
                i = v
                
                if i == t:
                    self.paths_found += 1
                    # Augment
                    path_flow = float('inf')
                    curr = t
                    while curr != s:
                        p = pred[curr]
                        edge_idx = pred_edge_idx[curr]
                        path_flow = min(path_flow, self.adj[p][edge_idx][1])
                        curr = p
                    
                    max_flow += path_flow
                    curr = t
                    while curr != s:
                        p = pred[curr]
                        edge_idx = pred_edge_idx[curr]
                        rev_idx = self.adj[p][edge_idx][2]
                        self.adj[p][edge_idx][1] -= path_flow
                        self.adj[curr][rev_idx][1] += path_flow
                        curr = p
                    
                    i = s # Restart from source
            else:
                # Retreat / Relabel
                min_dist = n
                for check_idx, (v, cap, rev, _) in enumerate(self.adj[i]):
                    if cap > 0:
                        min_dist = min(min_dist, d[v])
                
                old_dist = d[i]
                # Gap heuristic
                count[old_dist] -= 1
                if count[old_dist] == 0 and old_dist < n:
                    break 

                d[i] = min_dist + 1
                if d[i] < n:
                    count[d[i]] += 1
                
                current_arc[i] = 0 # Reset current arc
                
                if i != s:
                    i = pred[i]
                    
        return max_flow