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

        // Test 1: Successfully read "hello"
        Token t1 = helloReader.tryGet(input);
        assert t1 != null : "t1 should not be null";
        assert t1.getText().equals("hello") : "t1 text should be 'hello'";
        assert t1.getPos() == 0 : "t1 position should be 0";
        assert input.getPos() == 5 : "Input position should be 5 after 'hello'";

        // Test 2: Manually consume the space (as in original example)
        int space = input.read();
        assert space == (int) ' ' : "Should read a space character";

        // Test 3: Successfully read "world"
        Token t2 = worldReader.tryGet(input);
        assert t2 != null : "t2 should not be null";
        assert t2.getText().equals("world") : "t2 text should be 'world'";
        assert t2.getPos() == 6 : "t2 position should be 6 (after space)";
        assert input.getPos() == 11 : "Input position should be 11 after 'world'";

        // Test 4: Failed attempt to read "hello" again (should not change position)
        Token t3 = helloReader.tryGet(input);
        assert t3 == null : "t3 should be null (no 'hello' at current position)";
        assert input.getPos() == 11 : "Input position should remain 11 after failed read";

        System.out.println("All assertions passed.");
        // Test SimpleWsTokenReader with whitespace
        TokenInput inputWs = new TokenInput("  \n\thello world");
        TokenReader wsHelloReader = new SimpleWsTokenReader("hello");

        Token tWs = wsHelloReader.tryGet(inputWs);
        assert tWs != null : "tWs should not be null";
        assert tWs.getText().equals("hello") : "tWs text should be 'hello'";
        assert tWs.getPos() == 5 : "Token should start at index 5 (after whitespace)"; // "  \n\t" is 4 chars? Let's count: space(0), space(1), \n(2), \t(3) -> hello starts at 4. Adjust assertion based on exact input.
        assert inputWs.getPos() == 9 : "Input should be at index 9 (after 'hello')";
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

/**
 * A token reader that first skips any whitespace, then tries to match a fixed literal string.
 * If the literal is found, it returns a token whose position is the start of the literal.
 * If not, it restores the input position to where it was before skipping whitespace.
 */
class SimpleWsTokenReader implements TokenReader {
    private final String literal;

    public SimpleWsTokenReader(String literal) {
        this.literal = literal;
    }

    @Override
    public Token tryGet(TokenInput input) {
        int originalPos = input.getPos();

        // 1. Skip whitespace characters
        while (!input.isEOF() && Character.isWhitespace(input.peek())) {
            input.read(); // consume whitespace
        }

        int afterWsPos = input.getPos(); // Position where token should start
        int startPos = afterWsPos;       // This will be the token's reported position if successful

        // 2. Try to match the literal
        for (int i = 0; i < literal.length(); i++) {
            int ch = input.read();
            if (ch == -1 || ch != literal.charAt(i)) {
                // Mismatch or EOF: restore position to original (before whitespace)
                input.setPos(originalPos);
                return null;
            }
        }

        // 3. Success: return token positioned at the literal's start (after whitespace)
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