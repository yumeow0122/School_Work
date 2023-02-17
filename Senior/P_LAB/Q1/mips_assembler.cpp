#include <bits/stdc++.h>

using namespace std;

class Assembler
{
    const int R = 0;
    const int I = 1;
    const int J = 2;

    map<string, int> type;
    map<string, string> opc;
    map<string, string> reg;
    map<string, string> shamt;
    map<string, string> funct;

public:
    Assembler()
    {
        _buildType();
        _buildOpc();
        _buildReg();
        _buildShamt();
        _buildFunct();
    }
    
    string assemble(vector<string> args)
    {
        string res;
        string ope = args[0];
        if (type[ope] == R)
        {
            string rd = args[1], rs = args[2], rt = args[3];
            res += opc[ope];
            res += reg[rs];
            res += reg[rt];
            res += reg[rd];
            res += shamt[ope];
            res += funct[ope];
        }
        else if (type[ope] == I)
        {
            string rs = args[1], rt = args[2], imm = _toBinary(atoi(args[3].c_str()));
            res += opc[ope];
            res += reg[rs];
            res += reg[rt];
            res += imm;
        }

        return res;
    }

    vector<string> parse(string src)
    {
        vector<string> args;
        string tmp;
        bool isOpe = true;
        for (int cur = 0; cur < src.size(); cur++)
        {
            if (src[cur] == ' ')
            {
                if (isOpe)
                {
                    isOpe = false;
                    args.push_back(tmp);
                }
                else
                {
                    tmp = tmp.substr(0, 3);
                    args.push_back(tmp);
                }
                tmp.clear();
                cur++;
            }
            tmp += src[cur];
        }
        args.push_back(tmp);
        return args;
    }

private:
    string _toBinary(int src)
    {
        string res;
        while (src != 0)
        {
            res += (src % 2 == 0 ? "0" : "1");
            src /= 2;
        }

        while (res.size() < 16)
        {
            res += '0';
        }
        reverse(res.begin(), res.end());
        return res;
    }

    void _buildOpc()
    {
        // R-Type
        opc.insert({"add", "000000"});
        opc.insert({"sub", "000000"});
        opc.insert({"or", "000000"});
        opc.insert({"and", "000000"});

        // I-Type
        opc.insert({"addi", "001000"});
    }

    void _buildReg()
    {
        reg.insert({"$t0", "01000"});
        reg.insert({"$t1", "01001"});
        reg.insert({"$t2", "01010"});
        reg.insert({"$t3", "01011"});
        reg.insert({"$t4", "01100"});
        reg.insert({"$t5", "01101"});
        reg.insert({"$t6", "01110"});
        reg.insert({"$t7", "01111"});
    }

    void _buildFunct()
    {
        funct.insert({"add", "100000"});
        funct.insert({"sub", "100010"});
        funct.insert({"or", "100101"});
        funct.insert({"and", "100100"});
    }

    void _buildShamt()
    {
        shamt.insert({"add", "00000"});
        shamt.insert({"sub", "00000"});
        shamt.insert({"or", "00000"});
        shamt.insert({"and", "00000"});
    }
    void _buildType()
    {
        // R-Type
        type.insert({"add", 0});
        type.insert({"sub", 0});
        type.insert({"or", 0});
        type.insert({"and", 0});

        // I-Type
        type.insert({"addi", 1});
    }
};

// add $t0, $t1, $t2
// addi $t0, $t1, 3
int main()
{
    Assembler assembler;

    char code[33];
    while (cin.getline(code, 33))
    {
        // assembler.assemble(code);
        vector<string> args = assembler.parse(code);
        string obj = assembler.assemble(args);
        cout << obj << endl;
    }

    return 0;
}