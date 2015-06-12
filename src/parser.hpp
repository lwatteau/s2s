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
#ifndef MD_S2S_PARSER_HPP_
#define MD_S2S_PARSER_HPP_

//S2S
#include "config.hpp" //[TODO]: A pre-declaration should be enough

//C++ STANDARD
#include <iostream>
#include <map>
#include <memory>
#include <regex>
#include <string>
#include <vector>

class block;


class parser
{
public:

    static void init(std::unique_ptr<config> conf);
    static auto process_istream(std::istream& is) -> bool;


private:
    static auto get_context() -> block*; 
    static auto open(const std::string& payload) -> bool;
    static auto close(const std::string& dummy) -> bool;
    static auto write(const std::string& payload) -> bool;
    static auto write_open_bracket(const std::string& payload) -> bool;
    static auto write_close_bracket(const std::string& payload) -> bool;
    static auto require(const std::string& id) -> bool;
    static auto comment(const std::string& payload) -> bool;

private:

    using parser_func = bool(*)(const std::string&);
    static std::map<std::string, parser_func> parser_map;

};
#endif //#ifndef MD_S2S_PARSER_HPP_
