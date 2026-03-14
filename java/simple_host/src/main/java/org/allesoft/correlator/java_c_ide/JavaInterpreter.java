package org.allesoft.correlator.java_c_ide;

import java.io.BufferedInputStream;
import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;

import java.io.*;

public class JavaInterpreter {

    public static void main(String[] args) {
        TokenInput input = new TokenInput("hello world");
        TokenReader helloReader = new SimpleTokenReader("hello");
        TokenReader worldReader = new SimpleTokenReader("world");

        Token t1 = helloReader.tryGet(input);
        System.out.println(t1.getText()); // "hello"
        System.out.println(input.getPos()); // 5 (after "hello")

        // Skip space manually (or you could have a whitespace reader)
        input.read(); // consumes space

        Token t2 = worldReader.tryGet(input);
        System.out.println(t2.getText()); // "world"
        System.out.println(input.getPos()); // 11

        // Attempt to read "hello" again – fails and leaves position unchanged
        Token t3 = helloReader.tryGet(input);
        System.out.println(t3); // null
        System.out.println(input.getPos()); // still 11 (unchanged)
    }
}
/**
 * Holds the source text and a mutable position pointer.
 * Token readers use this to read characters and advance the position.
 */
class TokenInput {
    private final String text;
    private int pos;

    public TokenInput(String text) {
        this.text = text;
        this.pos = 0;
    }

    /** Returns the current position (0‑based index). */
    public int getPos() {
        return pos;
    }

    /** Sets the current position (used for backtracking). */
    public void setPos(int pos) {
        this.pos = pos;
    }

    /** Returns the underlying source text. */
    public String getText() {
        return text;
    }

    /** Peeks at the next character without advancing. Returns -1 at EOF. */
    public int peek() {
        return pos < text.length() ? text.charAt(pos) : -1;
    }

    /** Reads the next character and advances. Returns -1 at EOF. */
    public int read() {
        return pos < text.length() ? text.charAt(pos++) : -1;
    }

    /** Checks if the end of input has been reached. */
    public boolean isEOF() {
        return pos >= text.length();
    }
}

/**
 * Represents a token. Stores its start position and the matched text.
 */
interface Token {
    /** The start position in the source (0‑based index). */
    int getPos();

    /** The exact text that was matched. */
    String getText();
}

/**
 * A reader that attempts to extract a token from the input.
 * If successful, it returns a token and advances the input position.
 * If not, it returns null and leaves the input unchanged.
 */
interface TokenReader {
    Token tryGet(TokenInput input);
}

/**
 * A token reader that tries to match a fixed literal string.
 * For example, new SimpleTokenReader("if") tries to read the keyword "if".
 */
class SimpleTokenReader implements TokenReader {
    private final String literal;

    public SimpleTokenReader(String literal) {
        this.literal = literal;
    }

    @Override
    public Token tryGet(TokenInput input) {
        int startPos = input.getPos();
        // Try to match character by character
        for (int i = 0; i < literal.length(); i++) {
            int ch = input.read();
            if (ch == -1 || ch != literal.charAt(i)) {
                // Mismatch or EOF: restore position and return null
                input.setPos(startPos);
                return null;
            }
        }
        // Success: return a token
        return new Token() {
            private final int pos = startPos;
            private final String text = literal;

            @Override
            public int getPos() {
                return pos;
            }

            @Override
            public String getText() {
                return text;
            }
        };
    }
}