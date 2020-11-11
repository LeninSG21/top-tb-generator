#include <iostream>
#include <sstream>
#include <string>
#include <regex>
#include <iterator>
#include <map>
#include <fstream>

using namespace std;

struct var_struct
{
    string name;
    string size;
    string type;
    string funcType;
};

//Functions
string generateMainSequence();
void selectForIterations();
string generateInputTb();
string generateOutputTb();
string variableInit();
void displayMenu(struct var_struct *var);
string getTBStr(string moduleName, string paramStr, string initIn, string initOut, string varInit, string mainSequence);

//Regex
regex re_module("module\\s+([_a-zA-Z]\\w*)");
regex re_inout("(input|output|inout)(\\s+(reg|logic))?(\\s*\\[[^\\]]+\\]\\s*|\\s+)((?!input|output|inout|reg|logic)[_a-zA-Z]\\w*(,\\s*(?!input|output|inout|reg|logic)[_a-zA-Z]\\w*)*)");
regex re_parameters("((parameter)\\s+(\\w*)\\s*\\=\\s*((\\d+\'(b|h|d))?\\w+))");
smatch m;

//Global variables
int forIt = 10;
bool hasClk = false;
bool hasRst = false;


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

    string text = "";


    // filename of the file
    filename = argv[1];
    // opening file
    file.open(filename.c_str());
    // extracting words from the file
    while (getline(file, word)) //(file >> word)
    {
        // displaying content
        //cout << word << endl;
        text += word + "\n";
    }

    //return 0;

    //string text = "module register_file #(parameter W = 32)(input rst, clk, reg_write, input [4:0] rs_addr,rt_addr, rd_addr, input logic  [W-1:0] rd_w_data, output logic [31:0] rs_data, rt_data);";
    // Get the module name
    string moduleName = "";
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
        vars += ",";
        string varName = "";
        for (char c : vars)
        {
            if (c == ',')
            {
                struct var_struct x;
                x.name = varName;
                x.size = m[4];
                x.type = m[3];

                if (!hasClk)
                    hasClk = varName == "clk";
                if (!hasRst)
                    hasRst = varName == "rst";

                if (m[1] == "input")
                {
                    if (varName != "clk" && varName != "rst")
                        displayMenu(&x);
                    input_map[varName] = x;
                }
                else if (m[1] == "output")
                    output_map[varName] = x;
                else
                    inout_map[varName] = x;

                varName = "";
            }
            else if (c != ' ')
                varName += c;
        }

        ioi = m.suffix().str();
    }
    selectForIterations();

    string initIn = generateInputTb();
    string initOut = generateOutputTb();
    string mainSeq = generateMainSequence();
    string varInit = variableInit();

    cout << "\n"
         << endl;
    cout << getTBStr(moduleName, paramStr, initIn, initOut, varInit, mainSeq) << endl;
    return 0;
}

string getTBStr(string moduleName, string paramStr, string initIn, string initOut, string varInit, string mainSequence)
{
    string s = "`timescale 1ns/1ps\n\nmodule " + moduleName + "_tb;";
    if (paramStr != "")
        s += "\n\t" + paramStr;
    if (initIn != "")
        s += "\n\t" + initIn;
    if (initOut != "")
        s += "\n\t" + initOut;

    s += "\n\t" + moduleName + " UUT(.*);\n\t";
    s += "initial begin\n\t\t$dumpfile(\"" + moduleName + "_tb.vcd\");";
    s += "\n\t\t$dumpvars (1, " + moduleName + "_tb);";
    s += "\n\t\t" + varInit;
    if (hasRst)
        s += "\n\t\t#3\n\t\trst = 0;";
    s += "\n\t\t" + mainSequence;
    s += "\n\t\t#4\n\t\t$finish;\n\tend";
    if (hasClk)
        s += "\n\talways forever #0.5 clk = ~clk;";

    s += "\nendmodule";

    return s;
}

string generateMainSequence()
{
    bool exists = false;
    string s = "for(integer i = 0; i < " + to_string(forIt) + "; i++) begin \n\t\t\t#2";
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

void displayMenu(struct var_struct *var)
{
    printf("\n\nFor input %s %s, what do you want to do?\n", var->size.c_str(), var->name.c_str());
    printf("1 -> Random signal generation\n2 -> Ascending counter\n3 -> Descending counter\nOption: ");
    string opt;
    cin >> opt;
    while (opt != "1" && opt != "2" && opt != "3")
    {
        printf("\nInvalid option!\n");
        printf("1 -> Random signal generation\n2 -> Ascending counter\n3 -> Descending counter\nOption: ");
        cin >> opt;
    }
    if (opt == "1")
        var->funcType = "random";
    else if (opt == "2")
        var->funcType = "up";
    else
        var->funcType = "down";
}

void selectForIterations()
{
    printf("\nInput loop for iterations (default 10): ");
    string in;
    cin >> in;
    try
    {
        forIt = stoi(in);
    }
    catch (const invalid_argument &e)
    {
        forIt = 10;
    }
}

string generateInputTb()
{
    string s = "";

    for (auto const &varTupple : input_map)
    {
        s += "\treg " + varTupple.second.size + " " + varTupple.first + ";\n";
    }

    for (auto const &varTupple : inout_map)
    {
        s += "\treg " + varTupple.second.size + " " + varTupple.first + ";\n";
    }

    return s;
}

string generateOutputTb()
{
    string s = "";

    for (auto const &varTupple : output_map)
    {
        s += "\twire " + varTupple.second.size + " " + varTupple.first + ";\n";
    }
    return s;
}

string variableInit()
{
    string s = "";
    for (auto const &varTuple : input_map)
    {
        s += "\n\t\t" + varTuple.first;
        if (varTuple.first == "rst")
            s += " = 1;";
        else
            s += " = 0;";
    }
    return s;
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