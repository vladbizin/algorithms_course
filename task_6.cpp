#include <algorithm>
#include <random>
#include <string>
#include <string.h>
#include <fstream>
#include <sstream>

#include "C:\Users\vladb\source\repos\7smstr\matplotlib-cpp-master\matplotlibcpp.h"
namespace plt = matplotlibcpp;

#include<chrono>
using Time = std::chrono::high_resolution_clock;
using mks = std::chrono::microseconds;
auto start = Time::now();
auto stop = Time::now();





template<typename T>
class unbal_Node
{
public:
    T key;
    int count;
    unbal_Node* left;
    unbal_Node* right;
    int height;
};

template<typename T>
int height(unbal_Node<T>* N)
{
    if (!N) return 0;
    return N->height;
}

template<typename T>
unbal_Node<T>* new_unbal_Node(T key)
{
    unbal_Node<T>* node = new unbal_Node<T>();
    node->key = key;
    node->left = nullptr;
    node->right = nullptr;
    node->height = 1;
    node->count = 1;
    return (node);
}

template<typename T>
unbal_Node<T>* insert(unbal_Node<T>* node, T key)
{
    if (!node)
    {
        node = (new_unbal_Node(key));
        return node;
    }
    else if (key < node->key) node->left = insert(node->left, key);
    else if (key > node->key) node->right = insert(node->right, key);
    
    node->height = 1 + std::max(height(node->left), height(node->right));
    
    return node;
}

template<typename T>
void free_mem(unbal_Node<T>* node)
{
    if (node == nullptr) return;
    else
    {
        free_mem(node->left);
        free_mem(node->right);
        delete node;
    }
}

template<typename T>
int count_nodes(unbal_Node<T>* node)
{
    if (node == nullptr) return 0;
    return 1 + count_nodes(node->left) + count_nodes(node->right);
}



template<typename T>
class Node
{
public:
    T key;
    int count;
    Node* left;
    Node* right;
    int height;
};

template<typename T>
int height(Node<T>* N)
{
    if (!N) return 0;
    return N->height;
}

template<typename T>
Node<T>* new_Node(T key)
{
    Node<T>* node = new Node<T>();
    node->key = key;
    node->left = nullptr;
    node->right = nullptr;
    node->height = 1;
    node->count=1;
    return (node);
}

template<typename T>
Node<T>* right_rotate(Node<T>* y) {
    Node<T>* x = y->left;
    Node<T>* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = std::max(height(y->left), height(y->right)) + 1;
    x->height = std::max(height(x->left), height(x->right)) + 1;
    return x;
}

template<typename T>
Node<T>* left_rotate(Node<T>* x)
{
    Node<T>* y = x->right;
    Node<T>* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = std::max(height(x->left), height(x->right)) + 1;
    y->height = std::max(height(y->left), height(y->right)) + 1;
    return y;
}

template<typename T>
int bal(Node<T>* N)
{
    if (!N) return 0;
    return height(N->left) - height(N->right);
}

template<typename T>
Node<T>* insert(Node<T>* node, T key)
{
    if (!node)
    {
        node = (new_Node(key));
        return node;
    }
    else if (key < node->key) node->left = insert(node->left, key);
    else if (key > node->key) node->right = insert(node->right, key);
    else
    {
        node->count++;
        return node;
    }

    node->height = 1 + std::max(height(node->left), height(node->right));
    int balance = bal(node);
    if (balance > 1)
    {
        if (key < node->left->key) return right_rotate(node);
        else if (key > node->left->key)
        {
            node->left = left_rotate(node->left);
            return right_rotate(node);
        }
    }
    if (balance < -1)
    {
        if (key > node->right->key) return left_rotate(node);
        else if (key < node->right->key)
        {
            node->right = right_rotate(node->right);
            return left_rotate(node);
        }
    }
    return node;
}

template<typename T>
void free_mem(Node<T>* node)
{
    if (node == nullptr) return;
    else
    {
        free_mem(node->left);
        free_mem(node->right);
        delete node;
    }
}

template<typename T>
int count_nodes(Node<T>* node)
{
    if (node == nullptr) return 0;
    return 1 + count_nodes(node->left) + count_nodes(node->right);
}






//void LL(Node*& p, Node*& p1)
//{
//    p->left=p1->right;
//    p1->right = p;
//    p->bal = 0;
//    p = p1;
//}
//
//void RR(Node*& p, Node*& p1)
//{
//    p->right = p1->left;
//    p1->left = p;
//    p = p1;
//}
//
//void LR(Node*& p, Node*& p1)
//{
//    auto p2=p1->right;
//    p1->right = p2->left;
//    p2->left = p1;
//    p->left = p2->right;
//    p2->right = p;
//    if (p2->bal == -1) p->bal = 1;
//    else p->bal = 0;
//    if (p2->bal == 1) p1->bal = -1;
//    else p1->bal = 0;
//    p = p2;
//}
//
//void RL(Node*& p, Node*& p1)
//{
//    auto p2 = p1->left;
//    p1->left = p2->right;
//    p2->right = p1;
//    p->right = p2->left;
//    p2->left = p;
//    if (p2->bal == 1) p->bal = -1;
//    else p->bal = 0;
//    if (p2->bal == -1) p1->bal = 1;
//    else p1->bal = 0;
//    p = p2;
//}
//
//void(bal_L)(Node*& p, bool& h)
//{
//    switch (p->bal)
//    {
//    case(1):
//        p->bal = 0;
//        h = false;
//        break;
//    case(0):
//        p->bal = -1;
//        break;
//    case(-1):
//        auto p1 = p->left;
//        if (p1->bal == -1) LL(p, p1);
//        else LR(p, p1);
//        p->bal = 0;
//        h = false;
//        break;
//    }
//}
//
//void(bal_R)(Node*& p, bool& h)
//{
//    switch (p->bal)
//    {
//    case(-1):
//        p->bal = 0;
//        h = false;
//        break;
//    case(0):
//        p->bal = 1;
//        break;
//    case(1):
//        auto p1 = p->right;
//        if (p1->bal == 1) RR(p, p1);
//        else RL(p, p1);
//        p->bal = 0;
//        h = false;
//        break;
//    }
//}
//
//Node* insert(Node* p, int key,bool& h)
//{
//    if (!p)
//    {
//        Node* p = new_Node(key);
//        h = true;
//        return p;
//    }
//    else if (key < p->key)
//    {
//        insert(p->left, key, h);
//        if (h) bal_L(p, h);
//    }
//    else if (key > p->key)
//    {
//        insert(p->right, key, h);
//        if (h) bal_R(p, h);
//    }
//    else p->count++;
//    return p;
//}
//
//Node* insert(Node* p, int key)
//{
//    bool h = false;
//    return insert(p, key, h);
//}





struct Trunk
{
    Trunk* prev;
    std::string str;

    Trunk(Trunk* prev, std::string str)
    {
        this->prev = prev;
        this->str = str;
    }
};

void showTrunks(Trunk* p)
{
    if (p == nullptr) {
        return;
    }

    showTrunks(p->prev);
    std::cout << p->str;
}

template<class T>
void printTree(T* root, Trunk* prev=nullptr, bool isLeft=false)
{
    if (root == nullptr) {
        return;
    }

    std::string prev_str = "    ";
    Trunk* trunk = new Trunk(prev, prev_str);

    printTree(root->right, trunk, true);

    if (!prev) {
        trunk->str = "---";
    }
    else if (isLeft)
    {
        trunk->str = ".---";
        prev_str = "   |";
    }
    else {
        trunk->str = "'---";
        prev->str = prev_str;
    }

    showTrunks(trunk);
    std::cout << " " << root->key << ' ' << root->count << std::endl;

    if (prev) {
        prev->str = prev_str;
    }
    trunk->str = "   |";

    printTree(root->left, trunk, false);
}

template<typename T>
void balance_check(Node<T>* N, bool& ok)
{
    if (!N) return;

    balance_check(N->left, ok);

    if (std::abs(bal(N)) > 1)
    {
        std::cout << N->key << ' ';
        ok = false;
    }

    balance_check(N->right, ok);
}



template<typename T>
T* read_to_tree(T* root, std::string file_name)
{
    std::ifstream input_file;
    input_file.open(file_name);
    std::string line;
    std::string element;
    while (getline(input_file, line))
    {
        std::istringstream ss_line(line);
        while (ss_line)
        {
            ss_line >> element;
            root = insert(root, element);
        }
    }
    input_file.close();
    return root;
}

std::vector<std::string> read_to_vector(std::string file_name)
{
    std::vector<std::string> txt;
    std::ifstream input_file;
    input_file.open(file_name);
    std::string line;
    std::string element;
    while (getline(input_file, line))
    {
        std::istringstream ss_line(line);
        while (ss_line)
        {
            ss_line >> element;
            txt.push_back(element);
        }
    }
    input_file.close();
    return txt;
}


int main()
{
    /*Node<int>* root = nullptr;
    unbal_Node<int>* unbal_root = nullptr;

    std::random_device rnd_device;
    std::mt19937 mersenne_engine{ rnd_device() };

    int size = std::pow(2, 20)-1;

    std::normal_distribution<> normal_dist{ double(size), double(size)/4 };
    auto normal_gen = [&normal_dist, &mersenne_engine]() {
        return normal_dist(mersenne_engine);
    };


    std::vector<int> v(size);
    generate(v.begin(), v.end(), normal_gen);

    for (auto i : v)
    {
        root = insert(root, i);
        unbal_root = insert(unbal_root, i);
    }
    
    printTree(root);
    std::cout << '\n' << height(root) << '\n';
    bool ok = true;
    balance_check(root, ok);
    std::cout << '\n' << int(ok);

    std::cout << "\n\n\n\n\n";
    printTree(unbal_root);
    std::cout << '\n' << height(unbal_root) << '\n';*/
    




    /*Node<std::string>* root = nullptr;
    unbal_Node<std::string>* unbal_root = nullptr;

    root = read_to_tree(root, "War_and_Peace.txt");
    unbal_root = read_to_tree(unbal_root, "War_and_Peace.txt");


    printTree(root);
    bool ok = true;
    balance_check(root, ok);
    std::string balance = (ok == true) ? ", balanced" : ", unbalanced";
    std::cout << '\n' << height(root) << balance << '\n';

    printTree(unbal_root);
    std::cout << '\n' << height(unbal_root) << '\n';*/
    

    /*Node<std::string>* root = nullptr;
    unbal_Node<std::string>* unbal_root = nullptr;

    auto txt = read_to_vector("War_and_Peace.txt");
    const int txt_size = txt.size();
    const int step = 1E2;
    const int size = txt_size / step;
    const int iterations = 5E0;
    std::vector<double> balanced(size), unbalanced(size);
    std::vector<int> N(size);

    for (int k = 0; k < iterations; k++)
    {
        root = nullptr;
        unbal_root = nullptr;
        int count = 0;
        int tmp;
        for (int i = 0; i < size; i++)
        {
            std::cout << double(i) / size * 100 << '\n';


            tmp = count;
            balanced[i] = balanced[i - 1];
            while (count < (i + 1) * step)
            {
                start = Time::now();
                root = insert(root, txt[count++]);
                stop = Time::now();
                balanced[i] += (std::chrono::duration_cast<mks>(stop - start).count());
            }


            unbalanced[i] = unbalanced[i - 1];
            while (tmp < (i + 1) * step)
            {
                start = Time::now();
                unbal_root = insert(unbal_root, txt[tmp++]);
                stop = Time::now();
                unbalanced[i] += (std::chrono::duration_cast<mks>(stop - start).count());
            }

            N[i] = (i + 1) * step;
        }
        free_mem(root);
        free_mem(unbal_root);
    }
    for (int j = 0; j < size; j++)
    {
        balanced[j] /= iterations;
        unbalanced[j] /= iterations;
    }


    plt::figure_size(1200, 780);
    plt::title("Basic Tree vs AVL Tree");
    plt::named_plot("AVL", N, balanced, "-");
    plt::named_plot("Basic", N, unbalanced, "-");
    plt::xlabel("Text size");
    plt::ylabel("Insertion Time, mks");
    plt::legend();

    plt::show();*/






    Node<std::string>* root = nullptr;
    unbal_Node<std::string>* unbal_root = nullptr;

    auto txt = read_to_vector("War_and_Peace.txt");
    const int txt_size = txt.size();
    
    root = read_to_tree(root, "War_and_Peace.txt");
    int nodes = count_nodes(root);
    std::cout << nodes;
    free_mem(root);
    root = nullptr;

    const int step = 1E3;
    const int size = nodes / step;
    const int iterations = 1E0;
    std::vector<double> balanced(size), unbalanced(size);
    std::vector<int> N(size);
    int count=0;
    int tmp;
    for (int k = 0; k < iterations; k++)
    {
        root = nullptr;
        unbal_root = nullptr;
        for (int i = 0; i < size; i++)
        {
            std::cout << double(i) / size * 100 << '\n';

            tmp = count;
            balanced[i] = balanced[i - 1];
            while (count_nodes(root) < (i + 1) * step)
            {
                start = Time::now();
                root = insert(root, txt[count++]);
                stop = Time::now();
                balanced[i] += (std::chrono::duration_cast<mks>(stop - start).count());
            }

            unbalanced[i] = unbalanced[i - 1];
            while (count_nodes(unbal_root) < (i + 1) * step)
            {
                start = Time::now();
                unbal_root = insert(unbal_root, txt[tmp++]);
                stop = Time::now();
                unbalanced[i] += (std::chrono::duration_cast<mks>(stop - start).count());
            }

            N[i] = (i + 1) * step;
        }
        free_mem(root);
        free_mem(unbal_root);
    }

    plt::figure_size(1200, 780);
    plt::title("Basic Tree vs AVL Tree");
    plt::named_plot("AVL", N, balanced, "-");
    plt::named_plot("Basic", N, unbalanced, "-");
    plt::xlabel("Dictionary size");
    plt::ylabel("Insertion Time, mks");
    plt::legend();

    plt::show();



}