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
#include "cmdline_parser.hpp"
#include "parser.hpp"

//C++ STANDARD
#include <cstdlib>
#include <iostream>

//------------------------------------------------------------------------------
auto main(int argc, char* argv[]) -> int
{
    parser::init(cmdline_parser(argc, argv));

     return parser::process_istream(std::cin) ? EXIT_SUCCESS: EXIT_FAILURE;
}

