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
#include "context.hpp"
#include "parser.hpp"
#include "config.hpp"
#include "magic.hpp"

//C++ STANDARD

//BOOST
#include <boost/algorithm/string.hpp>

using namespace std;

//VARIABLES
unsigned int parser::tabspace { magic::default_tabspace } ;
//------------------------------------------------------------------------------
unsigned int parser::stack_state { 1u };
context* parser::root_context = nullptr;
vector<decltype(parser::root_context)> parser::m_stack;

map<string, parser::parser_func> parser::parser_map {
    {"{",           parser::open},
    {"}",           parser::close},
    {"?",           parser::require},
    {"|",           parser::write},
    {"[",           parser::write_open_bracket},
    {"]",           parser::write_close_bracket},
    {"#",           parser::comment}
};

//------------------------------------------------------------------------------
auto parser::get_context() -> context* 
{
    auto max = m_stack.size();
    decltype(max) pos {0};
    
    while (pos < max)
    {
        if (!(stack_state & (1<<pos)))
            return m_stack[max-pos-1];
        ++pos;
    }
    
    return root_context;
}

//------------------------------------------------------------------------------
auto parser::open(const string& payload) -> bool
{
    auto id = boost::trim_left_copy(payload);

    stack_state <<= 1;

    context* new_context = new context;
    new_context->m_id=id;
    m_stack.push_back(new_context);

    return true;
}

//------------------------------------------------------------------------------
auto parser::close(const string& dummy) -> bool
{
    if (root_context == get_context())
        return false; //We are in the root, so stop the program
    
    ++stack_state;
    while ((stack_state % 2) == 0 )
    {
        stack_state >>= 1;
        if (stack_state > 1)
            m_stack.pop_back();
    }
    
    return true;
}

//------------------------------------------------------------------------------
auto parser::write(const string& payload) -> bool
{
    auto cont = get_context();
    const auto space = string(tabspace, ' ');
    auto line  = ""s;
    for (int i=0; i < cont->m_indent_level; ++i)
        line += space;
    line += payload;

    if (root_context == cont)            //We are in the root context
        cout << line << endl;    //We write into the context board
    else                        
        cont->m_board.push_back(line);

    return true;
}

//------------------------------------------------------------------------------
auto parser::write_open_bracket(const string& payload) -> bool
{
    auto res = write(payload);
    ++(get_context()->m_indent_level);
    return res;
}

//------------------------------------------------------------------------------
auto parser::write_close_bracket(const string& payload) -> bool
{
    auto cont = get_context();
    if (--cont->m_indent_level < 0)
        cont->m_indent_level = 0;
    return write(payload);
}

//------------------------------------------------------------------------------
auto parser::require(const string& id) -> bool
{
    //First, find if in the stack...
    auto max = m_stack.size();
    decltype(max) index {0};

    while (index < max)
    {
        if (0 == id.compare(m_stack[index]->m_id))
            break;
        ++index;
    }

    if (max == index)
    {
        cerr << id << " is not defined" << endl;
        return false;
    }
    
    //Then, look if it's opened/closed
    if (stack_state & (1 << (max - index - 1)))
    {
        for (const auto& elem : m_stack[index]->m_board)
            write(elem);
        return true;
    }
    cerr << "bad context for " << m_stack[index]->m_id << endl;
    return false;
}

//------------------------------------------------------------------------------
auto parser::comment(const string& payload) -> bool
{
    return true;
}
//------------------------------------------------------------------------------
auto parser::process_istream(std::istream& is) -> bool
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
    
        auto keyword = m.str();

        if (keyword.empty())
        {
            cerr << "Unknown keyword" << endl;
            continue;
        }
        
        auto payload = m.suffix();
        //Récupération du nouveau contexte
        if (!parser_map[keyword](payload))
            return true; //End of program
    }
    return true;
}

void parser::init(unique_ptr<config> conf)
{
//[TODO] refactor the code below

    //This is the @ context 

    root_context= new context;
    root_context->m_id="@";
}


