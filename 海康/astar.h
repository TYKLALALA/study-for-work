#ifndef __ASTAR_H__
#define __ASTAR_H__
#include <vector>
#include <memory>
#include <cstdint>
#include <functional>
class BlockAllocator;
class AStar
{
public:
    struct Vec2
    {
        uint16_t x;
        uint16_t y;

        Vec2() : x(0) , y(0)
        {
        }

        Vec2(uint16_t x1, uint16_t y1) : x(x1), y(y1)
        {
        }

        void reset(uint16_t x1, uint16_t y1)
        {
            x = x1;
            y = y1;
        }

        int distance(const Vec2 &other) const
        {
            return abs(other.x - x) + abs(other.y - y);
        }

        bool operator== (const Vec2 &other) const
        {
            return x == other.x && y == other.y;
        }
    };

    typedef std::function<bool(const Vec2&)> Callback;
    struct Params
    {
        bool        corner;    
        uint16_t    height;   
        uint16_t    width;    
        Vec2        start; 
        Vec2        end;  
        Callback    can_pass;  

        Params() : height(0), width(0), corner(false)
        {
        }
    };

private:
    enum NodeState
    {
        NOTEXIST,  
        IN_OPENLIST,    
        IN_CLOSEDLIST  
    };
    struct Node
    {
        uint16_t    g;         
        uint16_t    h;        
        Vec2        pos; 
        NodeState   state;
        Node*       parent;
        int f() const
        {
            return g + h;
        }

        inline Node(const Vec2 &pos)
            : g(0), h(0), pos(pos), parent(nullptr), state(NOTEXIST)
        {
        }
    };

public:
    AStar(BlockAllocator *allocator);

    ~AStar();

public:
  
    int get_step_value() const;

    int get_oblique_value() const;

   
    void set_step_value(int value);


    void set_oblique_value(int value);

  
    std::vector<Vec2> find(const Params &param);

private:
  
    void clear();

    void init(const Params &param);

  
    bool is_vlid_params(const Params &param);

private:
   
    void percolate_up(size_t hole);

   
    bool get_node_index(Node *node, size_t *index);

    uint16_t calcul_g_value(Node *parent, const Vec2 &current);
    uint16_t calcul_h_value(const Vec2 &current, const Vec2 &end);
    bool in_open_list(const Vec2 &pos, Node *&out_node);
    bool in_closed_list(const Vec2 &pos);

    bool can_pass(const Vec2 &pos);

    
    bool can_pass(const Vec2 &current, const Vec2 &destination, bool allow_corner);

   
    void find_can_pass_nodes(const Vec2 &current, bool allow_corner, std::vector<Vec2> *out_lists);

    
    void handle_found_node(Node *current, Node *destination);

   
    void handle_not_found_node(Node *current, Node *destination, const Vec2 &end);

private:
    int                     step_val_;
    int                     oblique_val_;
    std::vector<Node*>      mapping_;
    uint16_t                height_;
    uint16_t                width_;
    Callback                can_pass_;
    std::vector<Node*>      open_list_;
    BlockAllocator*         allocator_;
};

#endif
