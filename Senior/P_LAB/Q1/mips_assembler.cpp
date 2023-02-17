#include <bits/stdc++.h>

using namespace std;

class Assembler
{
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

    void assemble(string code)
    {
        string tmp;
        string ope;
        int format;
        bool isOpe = true;
        vector<string> inputReg;
        for (int cur = 0; cur < code.size(); cur++)
        {
            if (code[cur] == ' ')
            {
                if (isOpe)
                {
                    isOpe = false;
                    ope = tmp;
                    format = type[ope];
                    cout << opc[ope];
                }
                else
                {
                    tmp = tmp.substr(0, 3);
                    inputReg.push_back(tmp);
                }
                tmp.clear();
                cur++;
            }
            tmp += code[cur];
        }

        if (format == 0)
        {
            inputReg.push_back(tmp);

            cout << reg[inputReg[1]];
            cout << reg[inputReg[2]];
            cout << reg[inputReg[0]];
            cout << shamt[ope];
            cout << funct[ope];
        }
        else if (format == 1)
        {
            string imm = _toBinary(atoi(tmp.c_str()));
            cout << reg[inputReg[1]];
            cout << reg[inputReg[0]];
            cout << setw(16) << setfill('0') << imm;
        }
        cout << endl;
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
        assembler.assemble(code);
    }

    return 0;
}