import os
import sys

header_template = """
#ifndef {{CLASSNAME_UPPER}}_H
#define {{CLASSNAME_UPPER}}_H

class {{CLASSNAME}}
{
private:
    //
public:
    {{CLASSNAME}}();

};

#endif //{{CLASSNAME_UPPER}}_H
"""

source_template = """
#include "{{CLASSNAME_LOWER}}.h"

{{CLASSNAME}}::{{CLASSNAME}}()
{

}
"""

def gen_file_content(template, classname):
    replacements = {
        "{{CLASSNAME_UPPER}}": classname.upper(),
        "{{CLASSNAME_LOWER}}": classname.lower(),
        "{{CLASSNAME}}": classname}
    output = template

    for r in replacements:
        output = output.replace(r, replacements[r])

    return output

def gen_files(folder_path, classname):
    header = gen_file_content(header_template, classname)
    source = gen_file_content(source_template, classname)

    os.makedirs(folder_path, exist_ok=True)
    
    header_file_path = os.path.join(folder_path, classname.lower() + ".h")
    source_file_path = os.path.join(folder_path, classname.lower() + ".cpp")

    f = open(header_file_path, "w")
    f.write(header)
    f.close()

    f = open(source_file_path, "w")
    f.write(source)
    f.close()

def main():
    #input arguments:
    args = sys.argv[1:]
    if len(args) != 2:
        print("You must provide two arguments: mkclass.py output_folder class_name")
        return
    
    

    folder = args[0]
    classname = args[1]

    cwd  = os.getcwd()
    output_folder = os.path.realpath(os.path.join(cwd, folder))
    gen_files(output_folder, classname)
    print(output_folder)

if __name__ == "__main__":
    main()