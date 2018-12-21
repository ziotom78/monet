#!/usr/bin/env python3
# -*- encoding: utf-8 -*-

# Taken from https://github.com/mkdocs/mkdocs/issues/692#issuecomment-427056366

import html
import os


def declare_variables(variables, macro):
    @macro
    def code_from_file(fn: str, flavor: str = ""):
        """
        Load code from a file and save as a preformatted code block.
        If a flavor is specified, it's passed in as a hint for syntax highlighters.

        Example usage in markdown:

            {{code_from_file("code/myfile.py", "python")}}

        """
        docs_dir = variables.get("docs_dir", "docs")
        fn = os.path.abspath(os.path.join(docs_dir, fn))
        if not os.path.exists(fn):
            return f"""<b>File not found: {fn}</b>"""
        with open(fn, "r") as f:
            return (
                f"""<pre><code class="{flavor}">{html.escape(f.read())}</code></pre>"""
            )

    @macro
    def snippet_from_file(fn: str, flavor: str = ""):
        """Load a snippet from a file and save as a preformatted code
        block. This works like `code_from_file`, except that only some
        of the lines in the file are read.

        To select the lines to read, delimit them with two lines
        containing the strings `@@@START_CODE_SNIPPET@@@` and
        `@@@END_CODE_SNIPPET@@@`, like in the following example:

            def test():
                x = []

                # @@@START_CODE_SNIPPET@@@
                for i in range(10):
                    x.append(i)
                # @@@END_CODE_SNIPPET@@@

                return x

        If a flavor is specified, it's passed in as a hint for syntax
        highlighters.

        Example usage in markdown:

            {{snippet_from_file("code/myfile.py", "python")}}

        """
        docs_dir = variables.get("docs_dir", "docs")
        fn = os.path.abspath(os.path.join(docs_dir, fn))
        if not os.path.exists(fn):
            return f"""<b>File not found: {fn}</b>"""

        lines = []
        with open(fn, "r") as f:
            curline = ''
            while '@@@START_CODE_SNIPPET@@@' not in curline:
                curline = f.readline()

            # How many spaces are there before the first non-space char in curline?
            spaces = curline.find(curline.lstrip()[0])

            while True:
                curline = f.readline()
                if len(curline) > spaces:
                    # Remove indentation spaces
                    curline = curline[spaces:]

                if '@@@END_CODE_SNIPPET@@@' in curline:
                    break

                lines.append(curline)

        return (
            f"""<pre><code class="{flavor}">{html.escape(''.join(lines))}</code></pre>"""
        )

    @macro
    def external_markdown(fn: str):
        """
        Load markdown from files external to the mkdocs root path.
        Example usage in markdown:

            {{external_markdown("../../README.md")}}

        """
        docs_dir = variables.get("docs_dir", "docs")
        fn = os.path.abspath(os.path.join(docs_dir, fn))
        if not os.path.exists(fn):
            return f"""<b>File not found: {fn}</b>"""
        with open(fn, "r") as f:
            return f.read()
