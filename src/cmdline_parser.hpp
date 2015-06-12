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
#ifndef MD_S2S_CMDLINE_PARSER_HPP_
#define MD_S2S_CMDLINE_PARSER_HPP_

//C++ STANDARD
#include <memory>

//------------------------------------------------------------------------------
//PRE-DECLARATIONS
class config;

//------------------------------------------------------------------------------
auto cmdline_parser(int argc, char* argv[]) -> std::unique_ptr<config>;

#endif //#ifndef MD_S2S_CMDLINE_PARSER_HPP_
