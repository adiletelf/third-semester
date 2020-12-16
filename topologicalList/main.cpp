#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

class Leader;

class Trailer
{
public:
    Leader *out;
    Trailer *next;

    Trailer(Leader *out_v=nullptr, Trailer *next_v=nullptr)
    {
        out = out_v;
        next = next_v;
    }
};


class Leader
{
public:
    int key;
    int inTotal;
    Leader *next;
    Trailer *outs;

    Leader(int key_v, Leader *next_v=nullptr, Trailer *outs_v=nullptr)
    {
        key = key_v;
        inTotal = 0;
        next = next_v;
        outs = outs_v;
    }

    ~Leader()
    {
        void deleteOuts();
    }


    void addOut(Leader *out)
    {
        outs = new Trailer(out, outs);
    }

    void deleteOuts()
    {
        Trailer *tr = this->outs;
        while (tr)
        {
            Trailer *next = tr->next;
            delete tr;

            tr = next;
        }
    }
};



class Graph
{
private:
    Leader *graph;

public:
    Graph(const char filename[])
    {
        graph = nullptr;
        ifstream inf(filename);
        if (!inf)
            graph = nullptr;
        else
        {
            int a, b;
            while (!inf.eof())
            {
                inf >> a >> b;
                addEdge(a, b);
            }
        }
    }

    ~Graph()
    {
        Leader *node = graph;
        while (node)
        {
            Leader *next = node->next;
            delete node;

            node = next;
        }
    }


    int getSize()
    {
        int size = 0;
        Leader *node = graph;
        while (node)
        {
            size++;
            node = node->next;
        }
        return size;
    }

    void addEdge(int a, int b)
    {
        if (!a)
            return;

        Leader *pa = nullptr;
        Leader *pb = nullptr;
        Leader *node = graph;
        while (node)
        {
            if (node->key == a)
                pa = node;

            if (node->key == b)
                pb = node;

            node = node->next;
        }
        if (!pa)
        {
            pa = new Leader(a, graph);
            graph = pa;

        }

        if (!pb)
        {
            pb = new Leader(b, graph);
            graph = pb;

        }

        pb->inTotal++;
        pa->addOut(pb);
    }


    vector<int> topologicalSort()
    {
        vector<int> vertices;
        Leader *givers; // contains nodes that only gives edges, but don't receive
        Leader *node = graph;
        while (node)
        {
            if (node->inTotal == 0)
            {
                Leader *giver = new Leader(node->key, givers, node->outs);
                givers = giver;
            }
            node = node->next;
        }

        node = givers;
        while (node)
        {
            givers = givers->next;
            vertices.push_back(node->key);

            Trailer *tr = node->outs;
            while (tr)
            {
                // tr->out->inTotal -= 1;
                if (--tr->out->inTotal == 0)
                {
                    // push new vertice got the top of givers
                    Leader *head = new Leader(tr->out->key, givers, tr->out->outs);
                    head->inTotal = tr->out->inTotal;
                    givers = head;
                }
                tr = tr->next;
            }
            
            // now we need to delete references to trailer from graph
            // so references only exits in givers at this moment
            
            node->outs = nullptr;
            
            delete node;

            node = givers;
        }

        return vertices;
    }

    friend ostream& operator<< (ostream& out, const Graph& graph)
    {
        Leader *node = graph.graph;
        while (node)
        {
            out << "Key: " << node->key << '\n';
            out << "Total amount of incoming arcs: " << node->inTotal << '\n';
            out << "Trailer: ";
            Trailer *tr = node->outs;
            while (tr)
            {
                out << tr->out->key << ' ';
                tr = tr->next;
            }
            out << "\n\n";

            node = node->next;
        }

        return out;
    }


};



int main()
{
    const char filename[] = "graph.txt";
    Graph graph(filename);

    cout << graph;

    cout << "Topological sort solution:\n";
    vector<int> vertices = graph.topologicalSort();


    int size = graph.getSize();

    if (size != vertices.size()) {
        std::cout << "Graph has a cycle" << std::endl;
    } else {
        for (auto &elem : vertices)
        {
            cout << elem << ' ';
        }
        cout << endl;
    }

    return 0;
}
