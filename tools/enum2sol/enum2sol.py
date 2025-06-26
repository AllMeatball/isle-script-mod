# zlib License
#
# Copyright (C) 2025 AllMeatball
#
# This software is provided 'as-is', without any express or implied
# warranty.  In no event will the authors be held liable for any damages
# arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:
#
# 1. The origin of this software must not be misrepresented; you must not
#    claim that you wrote the original software. If you use this software
#    in a product, an acknowledgment in the product documentation would be
#    appreciated but is not required.
# 2. Altered source versions must be plainly marked as such, and must not be
#    misrepresented as being the original software.
# 3. This notice may not be removed or altered from any source distribution.

import sys
import argparse
from pycparser import c_ast, c_parser, preprocess_file

class RootVisitor(c_ast.NodeVisitor):
    def __init__(self, output_path):
        self.file_out = open(output_path, 'w')
        # self.file_out = sys.stderr

    def write(self, string):
        self.file_out.write(string)

    def __del__(self):
        self.file_out.close()

    def visit_Enum(self, node):
        print(f"Found enum definition: {node.name}")

        self.write(f'm_lua.new_enum(\n')
        self.write(f'\t"{node.name}",')

        enumlist = node.values
        for i, enum in enumerate(enumlist.enumerators):
            self.write(f'\n\n\t"{enum.name}",')
            self.write(f'\n\t{node.name}::{enum.name}')

            if i < len(enumlist.enumerators) - 1:
                self.write(', ')

        self.write('\n);\n')

arg_parser = argparse.ArgumentParser()
arg_parser.add_argument('input')
arg_parser.add_argument('output')

args = arg_parser.parse_args()

preproc = preprocess_file(args.input)

parser = c_parser.CParser()
ast = parser.parse(preproc, filename=args.input)
# ast.show()

visitor = RootVisitor(args.output)
visitor.visit(ast)

