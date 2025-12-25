package org.allesoft.correlator.java_c_ide;

public class JavaCInterpreter {
    /*
        Java C IDE plugin for IntelliJ community:
        - GNU Emacs GUI + Emacs Lisp + lsp-mode/clangd
        - VS Code GUI + JavaScript + C++ LSP/clangd
        - IntelliJ open source GUI + Java + cmake/bear compile_commands.json => Eclipse CDT/LSP parser/interpreter for C => IntelliJ PSI

        so
        1. Run C Interpreter from https://github.com/wieker/java-c-interpreter
        2. Read compile_commands.json
        3. Port to IDEA GUI => PSI

        scope:
        1. C99 parser/interpreter written in Java (based on CDT/Cover)
        2. read cmake/bear compile_commands.json as entry point
        3. integrate into the IntelliJ UI / PSI
        4. add IntelliJ GDB support a-la Borland Turbo Debugger / C++ Builder for this C99 engine
     */
}
