//
//  polymorphic_graph.h
//  DeSalvo
//
//  Created by Stephen DeSalvo on 01/29/19.
//  Copyright (c) 2019 Stephen DeSalvo. All rights reserved.
//



#ifndef DeSalvoPolymorphicGraph_h
#define DeSalvoPolymorphicGraph_h


#include <vector>
#include <string>
#include <numeric>
#include <random>

#include "std_cin.h"
#include "std_cout.h"
#include "file.h"



namespace desalvo_standard_library {

    // Forward reference to class PolymorphicGraph for the Node and Edge classes.
    class PolymorphicGraph;
    class Node;

    /** @class Edge
        @brief Abstract base class to store edge objects.
        @details

        This class stores a PolymorphicGraph* object, as well as a vector of Node* 
        in case it needs to be used as a base class for a hyperedge, which 
        can connect more than simply two nodes.

        The two member variables are protected so that any edge object derived
        from this class can treat the members as their own.

    */
    class Edge {
    protected:
        PolymorphicGraph* graph;
        std::vector<const Node*> nodes;
    public:

        // No default or copy construction allowed, must always refer to a PolymorphicGraph and never be copied.
        Edge() = delete;
        Edge(Edge& e) = delete;
        Edge(PolymorphicGraph* initial_graph);

        /** No resources itself, just the standard virtual destructor. */
        virtual ~Edge() = default;

        // Utility functions
        virtual void print(std::ostream& out=std::cout) const = 0;
        virtual std::vector<const Node*> get_nodes() const; 

        /** @returns 0 since needs a default definition. */
        virtual double get_label() const { return 0; };
    };


    /** @class UndirectedEdge
        @brief An undirected edge connecting two nodes.

        This is a standard undirected edge in a simple graph.
    */
    class UndirectedEdge : public Edge {
    public:

        // Must initialize Undirected Edge with two nodes, no default or copy contruction allowed.
        UndirectedEdge() = delete;
        UndirectedEdge(UndirectedEdge& e) = delete;
        UndirectedEdge(PolymorphicGraph* initial_graph, Node* initial_first_node, Node* initial_second_node);

        /** No resources itself, just the standard virtual destructor. */
        virtual ~UndirectedEdge() = default;

        virtual void print(std::ostream& out=std::cout) const;
    };

    class DirectedEdge : public Edge {
    public:

        // Must initialize Directed Edge with two nodes, from one to the other, no default or copy contruction allowed.
        DirectedEdge() = delete;
        DirectedEdge(DirectedEdge& e) = delete;
        DirectedEdge(PolymorphicGraph* initial_graph, Node* initial_start, Node* initial_stop);

        /** No resources itself, just the standard virtual destructor. */
        virtual ~DirectedEdge() = default;

        /** @returns 0 since needs a default definition. */
        virtual double get_label() const { return 0; };

        virtual void print(std::ostream& out=std::cout) const;

    };


    class WeightedDirectedEdge : public DirectedEdge {
    public:
        // Must initialize Weighted Directed Edge with two nodes, from one to the other with a weight, no default or copy contruction allowed.
        WeightedDirectedEdge() = delete;
        WeightedDirectedEdge(DirectedEdge& e) = delete;
        WeightedDirectedEdge(PolymorphicGraph* initial_graph, Node* initial_start, Node* initial_stop, double initial_weight);

        /** No resources itself, just the standard virtual destructor. */
        virtual ~WeightedDirectedEdge() = default;

        /** @returns the weight of the edge */
        virtual double get_label() const { return weight; } ;

        virtual void print(std::ostream& out=std::cout) const;

    private:
        double weight;
    };


    class Node {
    public:
        Node() = delete;
        Node(Node&) = delete;
        Node(PolymorphicGraph* initial_graph); // : graph(initial_graph) { }
        Node(PolymorphicGraph* initial_graph, Edge* initial_edge); // : graph(initial_graph) { incident_edges.push_back(initial_edge); }
        Node(PolymorphicGraph* initial_graph, std::vector<Edge*>& initial_incident_edges); // : graph(initial_graph), incident_edges(initial_incident_edges) { }

        /** No resources itself, just the standard virtual destructor. */
        virtual ~Node() = default;

        virtual void print(std::ostream& out=std::cout) const = 0;
    private:
        // Which graph the node currently belongs to.
        PolymorphicGraph* graph;
        std::vector<Edge*> incident_edges;
    };


    class LabelledNode : public Node {
    public:
        LabelledNode() = delete;
        LabelledNode(LabelledNode&) = delete;
        LabelledNode(PolymorphicGraph* initial_graph, std::string initial_label); // : Node(initial_graph), label(initial_label) { initial_graph->add_node(this); };
        LabelledNode(PolymorphicGraph* initial_graph, Edge* initial_edge, const std::string& initial_label); // : Node(initial_graph, initial_edge), label(initial_label) { initial_graph->add_node(this); };
        LabelledNode(PolymorphicGraph* initial_graph, std::vector<Edge*>& initial_incident_edges, const std::string& initial_label); // : Node(initial_graph, initial_incident_edges), label(initial_label) { initial_graph->add_node(this); };

        /** No resources itself, just the standard virtual destructor. */
        virtual ~LabelledNode() = default;

        virtual void print(std::ostream& out=std::cout) const;

    private:
        std::string label;
    };

    /** @class PolymorphicGraph
        @brief A network of nodes and edges
    */
    class PolymorphicGraph {
    public:
        PolymorphicGraph() = default;
        // Do NOT allow copy construction for memory management reasons.
        PolymorphicGraph(PolymorphicGraph& g) = delete;

        /** Constructs a PolymorphicGraph using the input nodes and edges.
            It assumes everything is in order, but if you want to be safe
            then you would default-construct and then call the add_* members functions.
            @param initial_nodes is the vector of node pointers.
            @param initial_edges is the vector of edge pointers.
        */
        PolymorphicGraph(std::vector<Node*>& initial_nodes, std::vector<Edge*>& initial_edges) : nodes(initial_nodes), edges(initial_edges) { }


        /** Move constructor simply moves the nodes and edges along and clear out the vectors in g.
            @param g is the existing PolymorphicGraph object.
        */
        PolymorphicGraph(PolymorphicGraph&& g) : nodes(g.nodes), edges(g.edges) { 
            g.nodes.clear();
            g.edges.clear();
        }

        /** Resource management, calls delete on all the objects */
        virtual ~PolymorphicGraph() {
            //std::cout << "Calling the PolymorphicGraph Destructor" << std::endl;
            for (auto& node : nodes) delete node;
            for (auto& edge : edges) delete edge;
        }

        /** prints the nodes first and then the edges in the graph,
            not the prettiest or non-redundant but gets the job done.
            @param out is a stream object, by default std::cout
        */
        void print(std::ostream& out=std::cout) const {
            //out << "    Nodes: " << std::endl;
            for (auto& node : nodes)
                node->print(out);
            
            out << std::endl;
            //out << "    Edges: " << std::endl;
            for (auto& edge : edges)
                edge->print(out);

        };

        /** Adds in a node to the graph, AND ASSUMES OWNERSHIP OF ITS MEMORY!
            @param node to add to graph
        */
        void add_node(Node* node) { nodes.push_back(node); };

        /** WARNING!  Adds in an edge to the graph.  ASSUMES its member Node* are already part of graph.
            @param edge to add to graph
        */
        void add_edge(Edge* edge) { edges.push_back(edge); };

        /** Safe way to add undirected edge by linking to existing nodes by index in the vector of node*s.
            @param i is the first index in the vector of node*s.
            @param j is the second index in the vector of node*s.
        */
        void add_undirected_edge_by_indices(size_t i, size_t j) { UndirectedEdge* edge = new UndirectedEdge(this, nodes[i], nodes[j]); }

        /** Safe way to add directed edge by linking to existing nodes by index in the vector of node*s.
            @param i is the first index in the vector of node*s.
            @param j is the second index in the vector of node*s.
        */
        void add_directed_edge_by_indices(size_t i, size_t j) { DirectedEdge* edge = new DirectedEdge(this, nodes[i], nodes[j]); }

        /** Safe way to add directed weighted edge by linking to existing nodes by index in the vector of node*s.
            @param i is the first index in the vector of node*s.
            @param j is the second index in the vector of node*s.
        */
        void add_weighted_directed_edge_by_indices(size_t i, size_t j, double weight) { WeightedDirectedEdge* edge = new WeightedDirectedEdge(this, nodes[i], nodes[j], weight); }

        /** ESSENTIAL FUNCTION FOR Particle class, needs both the type signature and the initial places to position the particles. 
            This function designates which node in the graph is a state where all particles can be placed, say e.g., for ASEP.
        */
        const Node* initial_state() const { return nodes[0]; }

        std::vector<const Node*> neighbors(const Node* state, bool directed) const;
        std::pair<std::vector<const Node*>, std::vector<double>> neighbors_and_weights(const Node* state, bool directed) const;



    private:
        std::vector<Node*> nodes;
        std::vector<Edge*> edges;
    };


    /** Provides access to the nodes associated with the edge.
        @returns a vector of Node* representing the connection. */
    std::vector<const Node*> Edge::get_nodes() const { 
        return nodes;
    }


    /** prints the nodes and undirected edge connecting them.
        @param out is a stream object, by default std::cout
    */
    void UndirectedEdge::print(std::ostream& out) const {
        nodes[0]->print(out);
        out << " ---- ";
        nodes[1]->print(out);
        out << std::endl;
    };

    /** prints the nodes and directed edge connecting them.
        @param out is a stream object, by default std::cout
    */
    void DirectedEdge::print(std::ostream& out) const {
        nodes[0]->print(out);
        out << " ---> ";
        nodes[1]->print(out);
        out << std::endl;
    };

    /** prints the nodes and weighted directed edge connecting them.
        @param out is a stream object, by default std::cout
    */
    void WeightedDirectedEdge::print(std::ostream& out) const {
        nodes[0]->print(out);
        out << " -- " << weight << " --> ";
        nodes[1]->print(out);
        out << std::endl;
    };

    /** prints the label of the node. 
        @param out is a stream object, by default std::cout
    */
    void LabelledNode::print(std::ostream& out) const {
        out << label;
    }


    /** Node constructor.  Importantly, it instructs the graph to add the node,
        so that none of the derived classes needs to worry about it.  
        @param initial_graph is the PolymorphicGraph object the Node belongs to.
    */
    Node::Node(PolymorphicGraph* initial_graph) : graph(initial_graph) { 
        graph->add_node(this);
    }

    /** Node constructor.  Importantly, it instructs the graph to add the node,
        so that none of the derived classes needs to worry about it.  
        @param initial_graph is a PolymorphicGraph pointer in which the Node belongs to.
        @param initial_edge is an edge which is incident to the node.
    */
    Node::Node(PolymorphicGraph* initial_graph, Edge* initial_edge) : graph(initial_graph) { 
        incident_edges.push_back(initial_edge);
        graph->add_node(this);
    }

    /** Node constructor.  Importantly, it instructs the graph to add the node,
        so that none of the derived classes needs to worry about it.  
        @param initial_graph is a PolymorphicGraph pointer in which the Node belongs to.
        @param initial_incident_edges is a vector of edges which are incident to the node.
    */
    Node::Node(PolymorphicGraph* initial_graph, std::vector<Edge*>& initial_incident_edges) : graph(initial_graph), incident_edges(initial_incident_edges) { 
        graph->add_node(this);
    }


    /** LabelledNode constructor.  Importantly, it uses the base Node class implmentation to connect
        the graph to the node, so that this derived class does not need to worry about it.
        @param initial_graph is a PolymorphicGraph pointer in which the Node belongs to.
        @param initial_label is the initial label for the node.
    */
    LabelledNode::LabelledNode(PolymorphicGraph* initial_graph, std::string initial_label) : Node(initial_graph), label(initial_label) { 
        // Node base class and field initializer list takes care of everything
    };

    /** LabelledNode constructor.  Importantly, it uses the base Node class implmentation to connect
        the graph to the node, so that this derived class does not need to worry about it.
        @param initial_graph is a PolymorphicGraph pointer in which the Node belongs to.
        @param initial_edge is an edge which is incident to the node.
        @param initial_label is the initial label for the node.        
    */
    LabelledNode::LabelledNode(PolymorphicGraph* initial_graph, Edge* initial_edge, const std::string& initial_label) : Node(initial_graph, initial_edge), label(initial_label) { 
        // Node base class and field initializer list takes care of everything
    };

    /** LabelledNode constructor.  Importantly, it uses the base Node class implmentation to connect
        the graph to the node, so that this derived class does not need to worry about it.
        @param initial_graph is a PolymorphicGraph pointer in which the Node belongs to.
        @param initial_incident_edges is a vector of edges which are incident to the node.
        @param initial_label is the initial label for the node.
    */
    LabelledNode::LabelledNode(PolymorphicGraph* initial_graph, std::vector<Edge*>& initial_incident_edges, const std::string& initial_label) : Node(initial_graph, initial_incident_edges), label(initial_label) {
        // Node base class and field initializer list takes care of everything
    };

    /** Edge constructor.  Importantly, it instructs the graph to add the edge,
        so that none of the derived classes needs to worry about it.
        @param initial_graph is a PolymorphicGraph pointer in which the Edge belongs to.
    */
    Edge::Edge(PolymorphicGraph* initial_graph) : graph(initial_graph) { 
        initial_graph->add_edge(this);
    }

    /** Edge constructor.  Importantly, it uses the base Edge class implementation to connect
        the graph to the edge, so that this derived class does not need to worry about it. 
        @param initial_graph is a PolymorphicGraph pointer in which the Edge belongs to.
        @param initial_first_node is a Node pointer to one of the two nodes.
        @param initial_second_node is a Node pointer to the other of the two nodes.
    */
    UndirectedEdge::UndirectedEdge(PolymorphicGraph* initial_graph, Node* initial_first_node, Node* initial_second_node) : Edge(initial_graph) { 
        nodes.push_back(initial_first_node);
        nodes.push_back(initial_second_node);
    }

    /** Edge constructor.  Importantly, it uses the base Edge class implementation to connect
        the graph to the edge, so that this derived class does not need to worry about it. 
        @param initial_graph is a PolymorphicGraph pointer in which the Edge belongs to.
        @param initial_start is a Node pointer to one of the two nodes.
        @param initial_stop is a Node pointer to the other of the two nodes.
    */
    DirectedEdge::DirectedEdge(PolymorphicGraph* initial_graph, Node* initial_start, Node* initial_stop) : Edge(initial_graph) {
        nodes.push_back(initial_start);
        nodes.push_back(initial_stop);
    }

    /** WeightedDirectedEdge constructor.  Importantly, it uses the base DirectedEdge class implementation 
        which itself uses the Edge class implementation to connect
        the graph to the edge, so that this derived class does not need to worry about it. 
        @param initial_graph is a PolymorphicGraph pointer in which the Edge belongs to.
        @param initial_start is a Node pointer to one of the two nodes.
        @param initial_stop is a Node pointer to the other of the two nodes.
        @param initial_weight is the initial weight for the node.
    */
    WeightedDirectedEdge::WeightedDirectedEdge(PolymorphicGraph* initial_graph, Node* initial_start, Node* initial_stop, double initial_weight) : DirectedEdge(initial_graph, initial_start, initial_stop), weight(initial_weight) { 
        // (There is no step 3.)  See field initializer list all the way on the right in the line above.
    }

    /** Returns a vector of neighbors incident to a given node.
        @param state is the location to find neighbors of.
        @param directed indicates whether the edge relations are directed.
        @returns a vector of nodes which are neighbors to state.
    */
    std::vector<const Node*> PolymorphicGraph::neighbors(const Node* state, bool directed) const {
        // Only works for graphs, not yet hyper graphs where a hyperedge can connect 3 or more nodes.

        // Find all edges which have that node as its starting point.
        std::vector<const Node*> neighbors;
        std::vector<const Node*> local_neighbors;
        for (auto& edge : edges) {
            local_neighbors = edge->get_nodes();
            if (local_neighbors[0] == state)
                neighbors.push_back(local_neighbors[1]);

            if (!directed)
                if (local_neighbors[1] == state)
                    neighbors.push_back(local_neighbors[0]);
        }
        return neighbors;
    }


    /** Returns a vector of neighbors incident to a given node.
        @param state is the location to find neighbors of.
        @param directed indicates whether the edge relations are directed.
        @returns a vector of nodes which are neighbors to state.
    */
    std::pair<std::vector<const Node*>, std::vector<double>> PolymorphicGraph::neighbors_and_weights(const Node* state, bool directed) const {
        // Only works for graphs, not yet hyper graphs where a hyperedge can connect 3 or more nodes.

        std::vector<const Node*> neighbors;
        std::vector<double> weights;

        // Used inside the for-loop below to store the nodes of a given edge.
        std::vector<const Node*> local_neighbors;

        // Find all edges which have that node as its starting point.
        for (auto& edge : edges) {
            local_neighbors = edge->get_nodes();
            if (local_neighbors[0] == state) {
                neighbors.push_back(local_neighbors[1]);
                weights.push_back(edge->get_label());
            }

            if (!directed)
                if (local_neighbors[1] == state) {
                    neighbors.push_back(local_neighbors[0]);
                    weights.push_back(edge->get_label());
                }
        }

        std::pair<std::vector<const Node*>, std::vector<double>> result;
        result.first = neighbors;
        result.second = weights;

        return result;
    }


    /** Creates a simple complete graph on k nodes where all nodes are
        connected by undirected edges.
        @param k is the number of nodes.
        @returns a complete graph.
    */
    PolymorphicGraph complete_graph(unsigned int k) {
        PolymorphicGraph graph; // = new PolymorphicGraph;

        // First initialize nodes so that the edges can be initialized to them.
        for(unsigned int i=0;i<k;++i){
            LabelledNode* node = new LabelledNode(&graph, std::to_string(i));
        }

        // Iterate through all possible k choose 2 edges
        for (unsigned int i=0;i<k;++i){
            for (unsigned int j=i+1; j<k; ++j){
                graph.add_undirected_edge_by_indices(i, j);
            }
        }
        return graph;
    }


    /** Creates an Erdos-Renyi random simple graph where each edge occurs
        with probability p.
        @param k is the number of nodes.
        @param p is the probability that any edge appears in the graph.
        @param generator is the random engine generator.
        @tparam URNG is the standard uniform random number generator type
        @returns An Erdos-Renyi random simple graph.
    */
    template<typename URNG=std::default_random_engine>
    PolymorphicGraph random_graph(unsigned int k, double p, URNG& generator) {
        PolymorphicGraph graph; // = new PolymorphicGraph;

        // First initialize nodes so that the edges can be initialized to them.
        for(unsigned int i=0;i<k;++i){
            LabelledNode* node = new LabelledNode(&graph, std::to_string(i));
        }

        //std::default_random_engine generator(seed);
        std::uniform_real_distribution<double> unif(0.0,1.0);

        // Iterate through all possible k choose 2 edges
        for (unsigned int i=0;i<k;++i){
            for (unsigned int j=i+1; j<k; ++j){
                if (unif(generator) <= p)
                    graph.add_undirected_edge_by_indices(i, j);
            }
        }
        return graph;
    }


    /** Creates a complete graph on k nodes with Uniform(0,1) weights on each edge.
        @param k is the number of nodes.
        @param generator is the random engine generator.
        @tparam URNG is the standard uniform random number generator type
        @returns a complete graph with random weights.
    */
    template<typename URNG=std::default_random_engine>
    PolymorphicGraph random_weighted_complete_directed_graph(unsigned int k, URNG& generator) {
        PolymorphicGraph graph; // = new PolymorphicGraph;

        // First initialize nodes so that the edges can be initialized to them.
        for(unsigned int i=0;i<k;++i){
            LabelledNode* node = new LabelledNode(&graph, std::to_string(i));
        }

        // Initialize the random distribution
        //std::default_random_engine generator(seed);
        std::uniform_real_distribution<double> unif(0.0,1.0);

        // All possible edges including self edges representing holding probability.
        for (unsigned int i=0;i<k;++i){
            for (unsigned int j=0; j<k; ++j){
                graph.add_weighted_directed_edge_by_indices(i, j, unif(generator));
            }
        }
        return graph;
    }

    /** Creates a source + complete graph on k-2 nodes + sink with Uniform(0,1) weights on each edge.
        @param k is the number of nodes.
        @param generator is the random engine generator.
        @tparam URNG is the standard uniform random number generator type
        @returns a source + complete graph + sink.
    */
    template<typename URNG=std::default_random_engine>
    PolymorphicGraph random_weighted_complete_directed_graph_with_source_sink(unsigned int k, URNG& generator) {
        PolymorphicGraph graph; // = new PolymorphicGraph;

        // First initialize nodes so that the edges can be initialized to them.
        for(unsigned int i=0;i<k;++i){
            LabelledNode* node = new LabelledNode(&graph, std::to_string(i));
        }

        // Initialize the random generator and distribution
        //std::default_random_engine generator(seed);
        std::uniform_real_distribution<double> unif(0.0,1.0);

        // All possible edges including self edges representing holding probability.
        for (unsigned int i=0;i<k-1;++i){
            for (unsigned int j=1; j<k; ++j){
                graph.add_weighted_directed_edge_by_indices(i, j, unif(generator));
            }
        }
        graph.add_weighted_directed_edge_by_indices(k-1, k-1, unif(generator));

        // Uses either return-value optimization or move semantics.
        return graph;
    }


    /** Creates a directed chain with Uniform(0,1) weights on each edge.
        @param k is the number of nodes.
        @param generator is the random engine generator.
        @tparam URNG is the standard uniform random number generator type
        @returns a complete graph.
    */
    template<typename URNG=std::default_random_engine>
    PolymorphicGraph random_weighted_line_with_source_sink(unsigned int k, URNG& generator) {
        
        PolymorphicGraph graph; //= new PolymorphicGraph;

        // First initialize nodes so that the edges can be initialized to them.
        for(unsigned int i=0;i<k;++i){
            LabelledNode* node = new LabelledNode(&graph, std::to_string(i));
        }

        // Initialize the random generator and distribution
        //std::default_random_engine generator(seed);
        std::uniform_real_distribution<double> unif(0.0,1.0);

        // All possible edges including self edges representing holding probability.
        for (unsigned int i=0;i<k-1;++i){
                graph.add_weighted_directed_edge_by_indices(i, i, unif(generator));
                graph.add_weighted_directed_edge_by_indices(i, i+1, unif(generator));
        }
        graph.add_weighted_directed_edge_by_indices(k-1, k-1, unif(generator));
        return graph;
    }


    /** Creates a directed graph from an incidence matrix.
        @param filename is the name of the file
        @tparam URNG is the standard uniform random number generator type
        @returns a directed graph.
    */
    PolymorphicGraph graph_from_file(std::string filename) {
        file< file_type::input > graph_file(filename); //"input/" + "graph.txt");

        std::vector<double> weights;
        graph_file.read(weights); 

        size_t k = weights.size();

        PolymorphicGraph graph; // = new PolymorphicGraph;
        // First initialize nodes so that the edges can be initialized to them.
        for(size_t i=0;i<k;++i){
            LabelledNode* node = new LabelledNode(&graph, std::to_string(i));
        }

        for (size_t i=0;i<k; ++i) {
            if (weights[i] > 1e-10)
                graph.add_weighted_directed_edge_by_indices(0, i, weights[i]);
        }

        weights.clear();
        size_t index = 0;
        graph_file.read(weights);
        //std::cout << weights << std::endl;
        while (weights.size() > 0) {
            index += 1;
            for (size_t i=0;i<k; ++i) {
                if (weights[i] > 1e-10)
                    graph.add_weighted_directed_edge_by_indices(index, i, weights[i]);
            }
            weights.clear();
            graph_file.read(weights);
            //std::cout << weights << std::endl;
        }

        return graph;


    }





}


#endif
