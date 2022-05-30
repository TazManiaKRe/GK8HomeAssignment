#include <iostream>
#include <vector>
#include <limits>
#include <iterator>
#include <numeric>

using namespace std;
using Satoshi = uint64_t;
using Address = uint64_t;

//The amount of money created every block and given to the block's miner
#define MONEY_CREATED_FOR_THE_MINER_EACH_BLOCK 1000
#define MAX_ENTRIES_IN_BLOCK 10

#define SIG_LENGTH 32

struct Transaction
{
    //creator of transaction and sender of funds
    Address from;

    //transaction destination
    Address to;

    //amount of money to send to the destination
    Satoshi amount;

    //amount of fee is offered by the creator to the miner to add this transaction to the blockchain
    Satoshi fee;

    //creation time, written by the transaction creator
    time_t creationTime;

    //signing {from,amount,to,fee,creation time}
    uint8_t signature[SIG_LENGTH];
};

struct Block
{
    //holds up to 10 transactions
    std::vector<Transaction> trans;

    //the miner that added the block to the blockchain
    Address miner;

    // the next block at the blockchain (is nullptr for the last block)
    Block* next;
};

class Blockchain
{
    //not null
    Block* m_firstBlock;
public:
    //assumption firstBlock is not null
    Blockchain(Block* firstBlock) :m_firstBlock(firstBlock) {}

    //Assumption: all the transaction in the blockchain ("firstBlock linked-list") are valid
    //return whether or not the new transaction is valid, given this blockchain
    //No need to validate the crypto signature
    bool isValid(const Transaction& newTransaction) const
    {
        if (!newTransaction.from || !newTransaction.to || !newTransaction.fee || !newTransaction.creationTime)
            return false;

        if (newTransaction.from == newTransaction.to)
            return false;

        //find last block
        Block* lastBlock = m_firstBlock;
        while (lastBlock->next)
            lastBlock = lastBlock->next;

        //full block transaction can't be added
        if (lastBlock->trans.size() == MAX_ENTRIES_IN_BLOCK)
            return false;

        //check transaction signature uniqueness
        Block* curBlock = m_firstBlock;
        do
        {
            for (const auto& trans : curBlock->trans)
                if (!memcmp(trans.signature, newTransaction.signature, SIG_LENGTH))
                    return false;
        } while (curBlock = curBlock->next);

        //calculate transaction amount validity from the genesis block
        curBlock = m_firstBlock;
        uint64_t wallet = 0;

        do
        {
            if (newTransaction.from == curBlock->miner)
                wallet += MONEY_CREATED_FOR_THE_MINER_EACH_BLOCK;

            for (const auto& trans : curBlock->trans)
            {
                //collect fees if creator's wallet is the miner
                if (newTransaction.from == curBlock->miner)
                    wallet += trans.fee;

                //add or deduct amount from creator's wallet
                if (newTransaction.from == trans.to)
                    wallet += trans.amount;
                else if (newTransaction.from == trans.from)
                    wallet -= trans.amount;
            }
        } while (curBlock = curBlock->next);

        if (newTransaction.amount + newTransaction.fee > wallet)
            return false;

        return true;
    }
};






//sum numbers
int sumWithOutNumeric(vector<int> numList) {
    int sum = 0;
    for (int i = 0; i < numList.size(); i++)
        sum += numList.at(i);


    return sum;
}
//sum numbers
int sumWithNumeric(vector<int> numList) {
    int sum = std::accumulate(numList.begin(), numList.end(), 0);
    return sum;
}


//help func
int calcTwoNumbers(int numOne, int NumTwo) {
    return (numOne + NumTwo);
}

//template for sum even numbers with vectors
template<typename T, typename A>
vector<int> sumEvenNumbers(vector<T,A> const& numList) {
    vector<int> temp;
    int temp1 = 0;
    for (int i = 0; i < numList.size(); i = i+2) {
        temp1 = calcTwoNumbers(numList.at(i), numList.at(i + 1));
        temp.push_back(temp1);
    }
    return temp;
}



int main() {

    //1)
    vector<int> list = { 1, 2,100,150,200,700 }; // 3, 250, 900
    std::cout << "all sum are without using numeric: " << sumWithOutNumeric(list) << std::endl;
    std::cout << "all sum are within using numeric: " << sumWithNumeric(list) << std::endl;

    //2)
    vector<int> temp = sumEvenNumbers(list);
    for (int i = 0; i < temp.size(); i++) {
        std::cout << temp.at(i) << ",";
    }
    cout << endl;
    


    std::cout << "Hello Blockchain! Very nice to meet you! My name is main()" << std::endl;
    std::cout << "Nice to meet you to !" << std::endl;
    return 0;
}
