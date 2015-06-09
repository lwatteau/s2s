//------------------------------------------------------------------------------
// S2S
// Copyright (C) 2015 Laurent Watteau <contact@laurent-watteau.com>
// 557 Rue des Hemmes
// 62215 OYE-PLAGE (FRANCE)
//
// This program is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//------------------------------------------------------------------------------

//S2S

//C++ STANDARD
#include <iostream>
#include <map>
#include <regex>
#include <string>
#include <vector>

//BOOST
#include <boost/algorithm/string.hpp>

using namespace std;

struct block
{
    string m_id;
    vector<string> m_board;
    int m_indent_level = 0;
};

unsigned int stack_state {1};
auto m_stack = vector<block*> {};
block* root_context = nullptr;

using parser_func = bool(*)(const std::string&);

//------------------------------------------------------------------------------
auto get_context() -> block* 
{
    auto max = m_stack.size();
    auto pos = 0;
    while (pos < max)
    {
        if (!(stack_state & (1<<pos)))
            return m_stack[max-pos-1];
        ++pos;
    }
    return root_context;
}

//------------------------------------------------------------------------------
auto open(const string& payload) -> bool
{
    auto id = boost::trim_left_copy(payload);

    stack_state <<= 1;

    block* new_block = new block;
    new_block->m_id=id;
    m_stack.push_back(new_block);

    return true;
}

//------------------------------------------------------------------------------
auto close(const string& dummy) -> bool
{
    if (root_context == get_context())
        return false; //We are in the root, so stop the program
    
    ++stack_state;
    while ((stack_state%2) == 0 )
    {
        stack_state >>= 1;
        if (stack_state > 1)
            m_stack.pop_back();
    }
    return true;
}

//------------------------------------------------------------------------------
auto write(const string& payload) -> bool
{
    auto cont = get_context();

    string space("    ");
    string line;
    for(int i=0; i < cont->m_indent_level; ++i)
        line+=space;
    line+=payload;

    if (root_context == cont)            //We are in the root context
        cout << line << endl;    //We write into the context board
    else                        
        cont->m_board.push_back(line);

    return true;
}

//------------------------------------------------------------------------------
auto write_open_bracket(const string& payload) -> bool
{
    auto cont = get_context();
    auto res = write(payload);
    ++(cont->m_indent_level);
    return res;
}

//------------------------------------------------------------------------------
auto write_close_bracket(const string& payload) -> bool
{
    auto cont = get_context();
    if ((--cont->m_indent_level)<0)
        cont->m_indent_level = 0;
    return write(payload);
}

//------------------------------------------------------------------------------
auto require(const string& id) -> bool
{
    //First, find if in the stack...
    auto max = m_stack.size();
    auto index = 0;
    while (index < max)
    {
        if (id.compare(m_stack[index]->m_id)==0)
            break;
        ++index;
    }

    if (max == index)
    {
        cerr << id << " is not defined" << endl;
        return false;
    }
    //Then, look if it's opened/closed
    if (stack_state & (1 << (max-index-1)))
    {
        //We get the context, and we write the required block into it
        
        auto b = get_context();

        for (const auto& elem : m_stack[index]->m_board)
            write(elem);

        return true;
    }
    cerr << "bad context for " << m_stack[index]->m_id << endl;
    return false;
}

//------------------------------------------------------------------------------
auto comment(const string& payload) -> bool
{
    return true;
}

//------------------------------------------------------------------------------
std::map<std::string, parser_func> parser {
    {"{",           open},
    {"}",           close},
    {"?",           require},
    {"|",           write},
    {"[",           write_open_bracket},
    {"]",           write_close_bracket},
    {"#",           comment}
};

//------------------------------------------------------------------------------
auto process_istream(std::istream& is) -> bool
{
    //In S2S, each line is keyword + payload (set of arguments)
    static const auto keyword_expr = std::regex { 
        R"(^[[:blank:]]*)"      
        R"(\{|)"
        R"(\}|)"
        R"(\?|)"
        R"(\||)"
        R"(\[|)"
        R"(\]|)"
        R"(\#|)"
    };
                         
    auto line = ""s;
    while (std::getline(is, line))
    {
    
        boost::trim(line);

       
        //skips blank line
        if (line.empty()) continue;
            
        auto m = smatch {};
        regex_search(line, m, keyword_expr);
    
        string keyword = m.str();

        if (keyword.empty())
        {
            cerr << "Unknown keyword" << endl;
            continue;
        }
        
        string payload = m.suffix();
        //Récupération du nouveau contexte
        if (!parser[keyword](payload))
            return true; //End of program
    }
    return true;
}

//------------------------------------------------------------------------------
auto main() -> int
{

    //[TODO] refactor the code below
    //This is the @ block

    root_context= new block;
    root_context->m_id="@";

    return process_istream(cin) ? 0: 1;
}
