#include <bits/stdc++.h>

using namespace std;


struct Instruction{
  string address;
  string label;
  string operation;
  string target;
};


class Assembler{
  Instruction instruction[1000];
  map <string, string> opcode;
  map <string, string> labels;
  vector <string> object_code;
  int index;
public:
  Assembler(){
    index = 0;
    _init_opcode();
  }

  void compile(string fileName){
    fstream file;
    file.open(fileName, ios::in);

    for(string str=""; getline(file,str);){
      if(str[0] == '.') continue; // comment

      string temp;
      bool foundOpration = false;
      for(int i=0; i<str.size(); i++){
        while(i<str.size() && str[i] != ' ' && str[i] != '\n'){
          temp.push_back(str[i++]);
        }

        // cout << temp << endl;
        if(getOpcode(temp)== "-1" && // is label or target
            temp != "BYTE" && temp != "WORD" &&
            temp != "RESW" && temp != "RESB"&&
            temp != "START" && temp != "END"){
          if(!foundOpration) instruction[index].label = temp;
          else instruction[index].target = temp;
        }
        else{
          instruction[index].operation = temp;
          foundOpration = true;
        }
        temp.clear();
      }
      index++;
    }

    //_INSTRUCTION_DEBUG();

    file.close();

    addressing();
    generate_object_code();
    generate_machine_code();
  }

  string getOpcode(string ins){
    if(opcode.find(ins) == opcode.end())    //�S���
      return "-1";

    return opcode[ins];
  }

  void addressing(){
    instruction[0].address =  instruction[0].target;
    instruction[1].address =  instruction[0].target;
    if(instruction[0].label.size() > 0) labels[instruction[0].operation] = instruction[0].address;

    for(int i=2; i<index; i++){
      string mnemonic = instruction[i-1].operation;
      string lastAddress = instruction[i-1].address;
      if(mnemonic == "BYTE"){
        int bytes;
        string lastTarget =instruction[i-1].target;
        char judgeChar = lastTarget[0];
        if(judgeChar == 'C'){
          bytes = lastTarget.size() - 3;
        }
        else{
					if((lastTarget.size() - 3) % 2 == 0)
						bytes = (lastTarget.size() -3) / 2;
					else
						bytes = ((lastTarget.size() - 3) / 2) + 1;
        }
        instruction[i].address = _hex_add_hex(lastAddress, to_string(bytes));
      }
      else if(mnemonic == "RESB"){
        int num = stoi(instruction[i-1].target);
        instruction[i].address = _dec_add_hex(num, lastAddress);
      }
      else if(mnemonic == "RESW"){
        int num = 3 * stoi(instruction[i-1].target);
        instruction[i].address = _dec_add_hex(num, lastAddress);
      }
      else{
        instruction[i].address = _dec_add_hex(3, lastAddress);
      }
      if(instruction[i].label.size() > 0) labels[instruction[i].label] = instruction[i].address;
    }
    // _INSTRUCTION_DEBUG();
  }

  // first = address
  // second.first = label
  // second.second.first = operation
  // second.second.second = target
  // 1 -> hex add hex
  // 0 -> dec add hex
  void generate_object_code(){
    string objectCode = "", mnemonic, operand, label_address;
    for(int i=0; i<index; i++){
      bool isBuffer = false;
      objectCode.clear();
      mnemonic = instruction[i].operation;
      if(mnemonic == "RESW" || mnemonic == "RESB" || mnemonic == "END"){
        object_code.push_back("");
        continue;
      }

      operand.clear();

      // check target
      for(int l = 0; l < instruction[i].target.size(); l++){
        if(instruction[i].target[l] == ','){
          isBuffer = true;
          break;
        }
        operand += instruction[i].target[l];
      }

      if(mnemonic == "BYTE"){
        if(operand[0] == 'C'){
          for(int i = 2; i < operand.size()-1; i++){
            int ascii = operand[i];
            objectCode += (_dec_to_hex(ascii));
          }
        }
        else{ // 'X'
          for(int i = 2; i < operand.size()-1; i++){
            objectCode += (operand[i]);
          }
        }


        object_code.push_back(objectCode);
        continue;
      }
      else if(mnemonic == "WORD"){
        objectCode += _dec_to_hex(atoi(operand.c_str()));
        if(objectCode.size() < 6){
          string zero;
          for(int i = 0; i < 6 - objectCode.size(); i++){
            zero += "0";
          }
          objectCode.insert(0, zero);
        }
        object_code.push_back(objectCode);
        continue;
      }

      objectCode += getOpcode(mnemonic);
      if(operand == ""){
        objectCode += "0000";
        object_code.push_back(objectCode);
        continue;
      }
      label_address = labels[operand];
      if(label_address[0] > '7'){
        if(label_address[0] >= 'A')
          label_address[0] -= 15;
        else
          label_address[0] -= 8;
      }
      objectCode += label_address;

      if(isBuffer) objectCode = _hex_add_hex("8000", objectCode);


      if(objectCode.size() < 6){
        string zero;
        for(int i = 0; i < 6 - objectCode.size(); i++){
          zero += "0";
        }
        objectCode.insert(0, zero);
      }
      object_code.push_back(objectCode);
    }
    //_OBJECT_DEBUG();
  }

  // first = address
  // second.first = label
  // second.second.first = operation
  // second.second.second = target
  // 1 -> hex add hex
  // 0 -> dec add hex
  void generate_machine_code(){
    ofstream out("output.txt");
    string _size = _dec_to_hex(_hex_to_dec(instruction[index-1].address) - _hex_to_dec(instruction[0].address));
    out << "H" << instruction[0].label << "  " << instruction[0].address << "" << _size << endl;
    int i = 0, save = 1;
    while(1){
      int length = 0, codeLen = 0;
      bool printT = false;
      i = save;
      while(i<index-1 && object_code[i] != "" && codeLen < 30){
        length++;
        if(!printT && getOpcode(instruction[i].operation) != "-1"){
          out << "T";
          printT = true;
          out << "00" <<instruction[i].address << "";
        }

        if(object_code[i] != ""){
          codeLen += (object_code[i].size() / 2);
          if(codeLen > 30){
            codeLen -= (object_code[i].size() / 2);
            break;
          }
        }
        i++;
      }

      if(!printT){
        save++;
        continue;
      }
      out << _dec_to_hex(codeLen) << "";
      i = save;
      codeLen = 0;
      for(; codeLen < 30; i++){
        if(i >= index - 1) break;
        if(object_code[i] != ""){
          codeLen += (object_code[i].size() / 2);
        if(codeLen > 30) break;
        out << object_code[i] << "";
        }
        else{
          i++;
          break;
        }
      }
      save = i;
      if(i >= index-1) break;
      out << endl;
    }

    out << endl << "E" << "00" << instruction[1].address << endl;
    out.close();
    cout << "###" << "CONVERT SUCCESS" << "###" << endl << endl;
    ifstream readFile("output.txt");
    string line;
    while(readFile)
    {
        line = readFile.get();
        cout << line;
    }
    readFile.close();
  }

private:
  void _INSTRUCTION_DEBUG(){
    for(int i=0; i<index; i++){
      cout << "instruction " << i << " : ";

      cout << instruction[i].address << " " << instruction[i].label << " "
              << instruction[i].operation << " " << instruction[i].target << endl;
    }
  }

  void _OBJECT_DEBUG(){
    int i=0;
    for(auto it=object_code.begin(); it<object_code.end(); it++){
      cout << "instruction " << i << " : ";

      cout << instruction[i].address << " " << instruction[i].label << " "
              << instruction[i].operation << " " << instruction[i].target << " " << *it << endl;
      i++;
    }
  }

  void _init_opcode(){
    opcode.insert(pair <string , string> ("ADD", "18"));
    opcode.insert(pair <string , string> ("AND", "40"));
    opcode.insert(pair <string , string> ("COMP", "28"));
    opcode.insert(pair <string , string> ("DIV", "24"));
    opcode.insert(pair <string , string> ("J", "3C"));
    opcode.insert(pair <string , string> ("JEQ", "30"));

    opcode.insert(pair <string , string> ("JGT", "34"));
    opcode.insert(pair <string , string> ("JLT", "38"));
    opcode.insert(pair <string , string> ("JSUB", "48"));
    opcode.insert(pair <string , string> ("LDA", "00"));
    opcode.insert(pair <string , string> ("LDCH", "50"));
    opcode.insert(pair <string , string> ("LDL", "08"));

    opcode.insert(pair <string , string> ("LDX", "04"));
    opcode.insert(pair <string , string> ("MUL", "20"));
    opcode.insert(pair <string , string> ("OR", "44"));
    opcode.insert(pair <string , string> ("RD", "D8"));
    opcode.insert(pair <string , string> ("RSUB", "4C"));
    opcode.insert(pair <string , string> ("STA", "0C"));

    opcode.insert(pair <string , string> ("STCH", "54"));
    opcode.insert(pair <string , string> ("STL", "14"));
    opcode.insert(pair <string , string> ("STSW", "E8"));
    opcode.insert(pair <string , string> ("STX", "10"));
    opcode.insert(pair <string , string> ("SUB", "1C"));
    opcode.insert(pair <string , string> ("TD", "E0"));

    opcode.insert(pair <string , string> ("TIX", "2C"));
    opcode.insert(pair <string , string> ("WD", "DC"));
  }

  int _hex_to_dec(string str){
      int y;
      stringstream stream;
      stream << str;
      stream >> hex >> y;
      return y;
  }

  string _dec_to_hex(int num){
      stringstream stream;
      stream << hex << num;
      string out;
      stream >> out;
      transform(out.begin(), out.end(), out.begin(), ::toupper);
      if(out.length()<2) out.insert(0,1,'0');
      return out;
  }

  string _hex_add_hex(string hex1, string hex2){
    return _dec_to_hex(_hex_to_dec(hex1) + _hex_to_dec(hex2));
  }

  string _dec_add_hex(int dec, string hex){
    return _dec_to_hex(dec + _hex_to_dec(hex));
  }

};


int main(){
  Assembler *assembler = new Assembler();
  assembler->compile("input.txt");
  return 0;
}
