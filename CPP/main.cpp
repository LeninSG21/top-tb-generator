#include <iostream>
#include <sstream>
#include <string>
#include <regex>
#include <iterator>
#include <map>
#include <fstream>

using namespace std;

void generateMainSequence();

struct var_struct
{
    string name;
    string size;
    string type;
    string funcType;
};

regex re_module("module\\s+([_a-zA-Z]\\w*)");
regex re_inout("(input|output|inout)(\\s+(reg|logic))?(\\s*\\[[^\\]]+\\]\\s*|\\s+)((?!input|output|inout|reg|logic)[_a-zA-Z]\\w*(,\\s*(?!input|output|inout|reg|logic)[_a-zA-Z]\\w*)*)");
regex re_parameters("((parameter)\\s+(\\w*)\\s*\\=\\s*((\\d+\'(b|h|d))?\\w+))");
smatch m;

string moduleName = "";

map<string, var_struct> input_map;
map<string, var_struct> output_map;
map<string, var_struct> inout_map;

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        cout << "Missing input arguments!" << endl;
        return 0;
    }

    fstream file;
    string word, filename;

    // filename of the file
    filename = argv[1];
    // opening file
    file.open(filename.c_str());
    // extracting words from the file
    while (getline(file, word)) //(file >> word)
    {
        // displaying content
        cout << word << endl;
    }

    // Get the module name
    if (regex_search(text, m, re_module))
        moduleName = m[1];

    // Get the parameters
    string par = text;
    string paramStr = "";
    while (regex_search(par, m, re_parameters))
    {
        paramStr += "\\n\\t" + (string)m[0] + ";";
        par = m.suffix().str();
    }

    /* Get the inputs, outputs and inouts
        1st group --> input|ouput|inout
        3rd group --> logic | reg
        4th group --> bus size
        5th group --> variables separated by coma
    */
    string ioi = text;
    while (regex_search(ioi, m, re_inout))
    {

        string vars = m[5];
        string varName = "";
        for (char c : vars)
        {
            if (c == ',')
            {
                struct var_struct x;
                x.name = varName;
                x.size = m[4];
                x.type = m[3];

                if (m[1] == "input")
                    input_map[varName] = x;
                else if (m[1] == "output")
                    output_map[varName] = x;
                else
                    inout_map[varName] = x;

                varName = "";
            }
            else if (c != ' ')
                varName += c;
        }
        struct var_struct x;
        x.name = varName;
        x.size = m[4];
        x.type = m[3];
        x.funcType = "random";

        if (m[1] == "input")
            input_map[varName] = x;
        else if (m[1] == "output")
            output_map[varName] = x;
        else
            inout_map[varName] = x;
        ioi = m.suffix().str();
    }

    return 0;
}

string generateMainSequence()
{
    bool exists = false;
    string s = "\t\tfor(integer i = 0; i < " + to_string(forIt) + "; i++) begin \n\t\t\t#2";
    for (auto const &varTupple : input_map)
    {
        if ((varTupple.first != "clk") && (varTupple.first != "rst"))
        {
            exists = true;
            if (varTupple.second.funcType == "random")
                s += "\n\t\t\t" + varTupple.first + " = $urandom();";
            else if (varTupple.second.funcType == "up")
                s += "\n\t\t\t" + varTupple.first + " = i;";
            else if (varTupple.second.funcType == "down")
                s += "\n\t\t\t" + varTupple.first + " = " + to_string(forIt - 1) + " - i;";
        }
    }
    s += "\n\t\tend";
    return exists ? s : "";
}

/*
{
    string original = "module ALU(input [7:0] A, B, output out);";
    string s = "module ALU(input [7:0] A, B, output out);";
    smatch m;
    regex re_module("module\\s+([a-zA-Z]\\w*)");
    string module_name;
    if (regex_search (s,m,re_module)) {
        module_name = m[1];
        s = m.suffix().str();
    }
    regex re_inout("\\W*((input|output|inout)\\s*(\\[\\d+:\\d+\\]\\s*|\\s+)([_a-zA-Z]\\w*(,\\s*[_a-zA-Z]\\w*)*))");
    string type, bus, vars;
    if (regex_search (s,m,re_inout)) {
        type = m[2];
        bus = m[3];
        vars = m[4];
        s = m.suffix().str();
    }
    
    cout << original << endl;
    cout << "Module Name: "<< module_name << endl;
    cout << "Type: " << type << endl;
    cout << "Bus: " << bus << endl;
    cout << "Vars: " << vars << endl;
    cout << "Restante: " << s << endl;
    
}
*/