#include <iostream>
#include <sstream>
#include <string>
#include <regex>
#include <iterator>
#include <map>
#include <fstream>
#include<bits/stdc++.h> 

using namespace std;

struct var_struct
{
    string name;
    string size;
    string type;
    string funcType;
};

struct rst_struct
{
    string name;
    bool activeHigh;
};

/********** FUNCTIONS *************/

void readConfig(string config_name);

//display
void getClk();
void getRst();
void displayMenu(struct var_struct *var);
void selectForIterations();
void timescale();
void printHelp();

string generateMainSequence();
string generateInputTb();
string generateOutputTb();
string variableInit();

string getTBStr();

// Regex
regex re_module("module\\s+([_a-zA-Z]\\w*)");
regex re_inout("(input|output|inout)(\\s+(reg|logic))?(\\s*\\[[^\\]]+\\]\\s*|\\s+)((?!input|output|inout|reg|logic)[_a-zA-Z]\\w*(,\\s*(?!input|output|inout|reg|logic)[_a-zA-Z]\\w*)*)");
regex re_parameters("((parameter)\\s+(\\w*)\\s*\\=\\s*((\\d+\'(b|h|d))?\\w+))");
smatch m;

// Global variables
int forIt = 10;
bool hasClk = false;
bool hasRst = false;
string moduleName = "";
string paramStr = "";
string initIn;
string initOut;
string mainSequence;
string varInit;
string clk = "clk";
struct rst_struct rst = { .name = "rst", .activeHigh = true };
string scale = "1ns/1ps";

// Global Dictonaries
map<string, var_struct> input_map;
map<string, var_struct> output_map;
map<string, var_struct> inout_map;

// Override definition
string funcOverride;
bool fOverride = false;
bool scaleOverride = false;
bool clkOverride = false;
bool rstOverride = false;
bool forOverride = false;
bool hasConfig = false;

int main(int argc, char *argv[])
{
    string filename;
    string config = "";

    if (argc <= 1)
    {
        cout << "Missing input arguments!" << endl;
        return 0;
    }
    else if(argc == 2)
    {
        if(strcmp(argv[1], "--help") == 0)
        {
            printHelp();
            return 0;
        }
        filename = argv[1];
    }
    else
    {
        filename = argv[1];
        for(uint16_t i = 2; i < argc; i++)
        {
            if(argv[i][0] == '-')
            {
                if(strcmp(argv[i], "--help") == 0)
                {
                    printHelp();
                    return 0;
                }
                
                uint16_t j = 0;
                while(argv[i][++j]!= 0)
                {
                    switch (argv[i][j])
                    {
                    case 'r':
                        funcOverride = "random";
                        fOverride = true;
                        break;
                    case 'a':
                        funcOverride = "up";
                        fOverride = true;
                        break;
                    case 'd':
                        funcOverride = "down";
                        fOverride = true;
                        break;
                    case 't':
                        scaleOverride = true;
                        break;
                    case 'c':
                        clkOverride = true;
                        break;
                    case 's':
                        rstOverride = true;
                        break;
                    case 'f':
                        forOverride = true;
                        break;
                    default:
                        break;
                    }
                }
            }
            else
            {
               config = argv[i];
               hasConfig = true;
            }
            
        }
    }

    // Read File
    fstream file;
    string word;
    string text = "";

    // opening file
    file.open(filename.c_str());
    // extracting words from the file
    while (getline(file, word)) //(file >> word)
    {
        // displaying content
        //cout << word << endl;
        text += word + "\n";
    }
    
    file.close();

    
    if(hasConfig)
        readConfig(config);

    // Welcome message
    cout << "Welcome to the testbench generator!\n" << endl;

    if(!hasConfig)
    {
        //Clk name
        if (clkOverride)
            getClk();

        if (rstOverride)
            getRst();
    }
    

    
    // Get the module name
    if (regex_search(text, m, re_module))
        moduleName = m[1];

    // Get the parameters and format them
    string par = text;
    
    while (regex_search(par, m, re_parameters))
    {
        paramStr += "\n\t" + (string)m[0] + ";";
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
                struct var_struct x = {.name = varName, .size = m[4], .type = m[3]};

                if (!hasClk)
                    hasClk = varName == clk;
                if (!hasRst)
                    hasRst = varName == rst.name;

                if (m[1] == "input")
                {
                    if (varName != clk && varName != rst.name)
                        
                        if(fOverride)
                            x.funcType = funcOverride;
                        else
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

    if(!hasConfig)
    {
        // User type in iterator for simulation values
        if (forOverride)
            selectForIterations();

        // User type in the timescale for the testbench
        if (scaleOverride)
            timescale();
    }
    


    initIn = generateInputTb();
    initOut = generateOutputTb();
    mainSequence = generateMainSequence();
    varInit = variableInit();

    //Write file
    string testname;
    testname = filename;

    int size = testname.length();
    testname[size - 3] = '_';
    testname[size - 2] = 't';
    testname[size - 1] = 'b';
    testname +=".sv";
    
    ofstream test(testname.c_str());
    test << getTBStr() << endl;
    test.close();
    cout << "Done" << endl;
    return 0;
}


/*********** FUNCTIONS *********************/

//Config

void readConfig(string config_name)
{
    fstream file;
    string line;
    // opening file
    file.open(config_name.c_str());
    // extracting words from the file

    string FLAG = "";
    string value = "";
    bool flag_ready;
    while (getline(file, line)) //(file >> word)
    {   
        FLAG = "";
        value = "";
        flag_ready = false;
        for(char c: line)
        {
            if(!flag_ready)
            {
                if(c!='=')
                    FLAG+=c;
                else
                    flag_ready = true;
            }
            else{
                if(c != 0)
                    value += c;
            }
        }

        if(FLAG == "RST")
            rst.name = value;
        else if(FLAG=="ACTIVE_HIGH")
            rst.activeHigh = value == "1";
        else if(FLAG=="CLK")
            clk = value;
        else if(FLAG=="FOR_IT")
            forIt = stoi(value);
        else if(FLAG == "SCALE")
            scale = value;
        else if(FLAG == "FUNC")
        {
            fOverride = true;
            if(value == "r")
                funcOverride = "random";
            else if(value =="a")
                funcOverride = "up";
            else
                funcOverride = "down";
        }
    
    }
    
    file.close();
}

//Get clock signal function

void getClk()
{
    printf("\nDefine new name of your clock: ");
    cin >> clk;
}

//Get reset signal function
void getRst()
{
    string active="";

    printf("\nDefine name of your reset signal: ");
    cin >> rst.name;
    printf("\nIs it active HIGH? [y/n] ");
    cin >> active;
    transform(active.begin(), active.end(), active.begin(), ::tolower); 
    rst.activeHigh = active == "y";
}

//Get user defined timescale function
void timescale()
{
    printf("\nSet time unit and time precision for timescale: ");
    cin >> scale;
}

//Display menu function
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

//Iterator function
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

//Help menu function
void printHelp()
{
    cout << "\n\n\t./tb-generator [FILENAME] ([OPTIONS] | [CONFIG]) " << endl
        <<"\nOptions:\n" << endl
        << "-r --> All variables are assigned a random number $urandom() in every iteration" << endl
        << "-a --> Variables are assigned a number that increases by 1 with each iteration"<<endl
        << "-d --> Variables are assigned a number that starts at the for loop limit and decreases by 1 every iteration" << endl
        << "-t --> Override default timescale of 1ns/1ps" << endl
        <<"-s --> Override default reset name (rst) and active high"<<endl
        <<"-c --> Override default clock name (clk)"<<endl
        <<"-f --> Override default number of iterations (10)"<<endl
        <<"CONFIG is the path to a configuration fila"<<endl
        << "With no option, the user will be prompted to select the value to assign for each variable, the loop iterations"
        <<"\nwill be set to 10, the clock signal is expected to be named \"clk\" and the reset signal is expected to be \"rst\""
        << "\nand active high, and the timescale is set to 1ns/1ps.\n"<<endl;
    return;
}


string getTBStr()
{
    string s = "// Project Name: "+moduleName;
    
    s += "\n\n`timescale " + scale + "\n\nmodule " + moduleName + "_tb;";
    if (paramStr != "")
        s += "\n" + paramStr;
    if (initIn != "")
        s += "\n" + initIn;
    if (initOut != "")
        s += "\n" + initOut;

    s += "\n\t" + moduleName + " UUT(.*);\n\t";
    s += "initial begin\n\t\t$dumpfile(\"" + moduleName + "_tb.vcd\");";
    s += "\n\t\t$dumpvars (1, " + moduleName + "_tb);";
    s += "\n\t" + varInit;
    if (hasRst)
        s += "\n\t\t#3\n\t\t"+rst.name + " = ";
        s += rst.activeHigh ? "0;":"1;";
    s += "\n" + mainSequence;
    s += "\n\t\t#4\n\t\t$finish;\n\tend";
    if (hasClk)
        s += "\n\talways forever #0.5 " + clk + " = ~" + clk + ";";

    s += "\nendmodule";

    return s;
}

string generateMainSequence()
{
    bool exists = false;
    string s = "\t\tfor(integer i = 0; i < " + to_string(forIt) + "; i++) begin \n\t\t\t#2";
    for (auto const &varTupple : input_map)
    {
        if ((varTupple.first != clk) && (varTupple.first != rst.name))
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
        if (varTuple.first == rst.name)
            if(rst.activeHigh)
                s += " = 1;";
            else
                s += " = 0;";
            
        else
            s += " = 0;";
    }
    return s;
}