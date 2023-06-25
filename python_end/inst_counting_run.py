from processing import inst_counting_process, print_scope_table
from load_results import print_highlighted_source
from globals import c_source_loc

if __name__ == '__main__':
    # Define source code
    source_name = 'sample.c'

    # Preprocess source code
    scope_table = inst_counting_process(source_name)

    # Load source code
    with open(c_source_loc+source_name, 'r') as src_file:
        source = src_file.read()

    print_highlighted_source(source, scope_table)

    print_scope_table(scope_table)
