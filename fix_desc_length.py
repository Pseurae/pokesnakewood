from glob import iglob
import re
import textwrap

def process_content(content, pattern: re.Pattern):
    start = 0

    while (search := pattern.search(content, start)):
        start = search.end(0)
        string = content[search.start(0):search.end(0) - 1]

        if string.startswith('\n    "'):
            string = string.replace('\n    "', "")

        string = string.replace('\n    "', " ")
        string = string.replace('\\n"', "")

        s = ""
        lines = textwrap.wrap(string, 35)
        for i, line in enumerate(lines):
            if i == 0:
                s += '' + line + '\\n"'
            elif i == len(lines):
                s += '\n    "' + line + '\\n"'
            else:
                s += '\n    "' + line + '"'

        content = content[:search.start(0)] + s + content[search.end(0):]

    return content

string_pattern = re.compile(r'("[^)]*")(?=\))')

def perform_decap(path, string_pattern, is_glob=False):
    if is_glob:
        for i in iglob(path, recursive=True):
            perform_decap(i, string_pattern, False)

    else:
        with open(path, "r") as source_file:
            content = source_file.read()

        newcontent = process_content(content, string_pattern)

        with open(path, "w") as dest_file:
            dest_file.write(newcontent)

c_files = [
    # ("src/data/**/*.h", True),
    ("src/data/text/item_descriptions.h", False)
]

if __name__ == "__main__":
    for path, is_glob in c_files:
        perform_decap(path, string_pattern, is_glob)