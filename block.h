#ifndef BLOCK_H
#define BLOCK_H

#include <vector>
using namespace std;

class Block {
public:
    vector<vector<vector<int>>> mino;
    Block() : mino(4, vector<vector<int>>(4, vector<int>(4, 0))) {};
    virtual vector<vector<vector<int>>> create_block();
    virtual ~Block() = default;
private:
    
};

class I_Mino : public Block {
public:
    vector<vector<vector<int>>> create_block() override;
};

class O_Mino : public Block {
public:
    vector<vector<vector<int>>> create_block() override;
};

class T_Mino : public Block {
public:
    vector<vector<vector<int>>> create_block() override;
};

class L_Mino : public Block {
public:
    vector<vector<vector<int>>> create_block() override;
};

class J_Mino : public Block {
public:
    vector<vector<vector<int>>> create_block() override;
};

class S_Mino : public Block {
public:
    vector<vector<vector<int>>> create_block() override;
};

class Z_Mino : public Block {
public:
    vector<vector<vector<int>>> create_block() override;
};


#endif
