package org.allesoft.correlator.java_c_ide;

import java.io.*;
import java.util.*;

public class JavaInterpreter {

    public static void main(String[] args) {
        // ---------- Test KeywordTokenizer (with built‑in whitespace skip) ----------
        TokenInput input = new TokenInput("  hello world");
        TokenReader helloReader = new KeywordTokenizer("hello");
        TokenReader worldReader = new KeywordTokenizer("world");

        Token t1 = helloReader.tryGet(input);
        assert t1 != null && t1.getText().equals("hello") && t1.getType() == TokenType.KEYWORD;
        assert input.getPos() == 7; // "  hello" → 2 spaces + 5 chars = 7

        // There is still a space before "world", but the next reader skips it automatically
        Token t2 = worldReader.tryGet(input);
        assert t2 != null && t2.getText().equals("world") && t2.getType() == TokenType.KEYWORD;
        assert input.getPos() == 13; // 7 + space + 5 = 13

        Token t3 = helloReader.tryGet(input);
        assert t3 == null && input.getPos() == 13; // unchanged

        // ---------- Test IntegerTokenizer ----------
        TokenInput intInput = new TokenInput("   123 456");
        TokenReader intReader = new IntegerTokenizer();
        Token i1 = intReader.tryGet(intInput);
        assert i1 != null && i1.getText().equals("123") && i1.getType() == TokenType.INTEGER;
        assert intInput.getPos() == 7; // "   123"
        // next token attempt fails (stops at space)
        Token i2 = intReader.tryGet(intInput);
        assert i2 == null && intInput.getPos() == 7;

        // ---------- Test FloatTokenizer ----------
        TokenInput floatInput = new TokenInput("  3.14  .5  2e10  -1.2e-3");
        TokenReader floatReader = new FloatTokenizer();
        Token f1 = floatReader.tryGet(floatInput);
        assert f1 != null && f1.getText().equals("3.14") && f1.getType() == TokenType.FLOAT;
        // We'll test others separately for clarity
        TokenInput f2in = new TokenInput("  .5");
        Token f2 = floatReader.tryGet(f2in);
        assert f2 != null && f2.getText().equals(".5") && f2.getType() == TokenType.FLOAT;
        TokenInput f3in = new TokenInput("  2e10");
        Token f3 = floatReader.tryGet(f3in);
        assert f3 != null && f3.getText().equals("2e10") && f3.getType() == TokenType.FLOAT;
        TokenInput f4in = new TokenInput("  -1.2e-3");
        Token f4 = floatReader.tryGet(f4in);
        assert f4 != null && f4.getText().equals("-1.2e-3") && f4.getType() == TokenType.FLOAT;

        // ---------- Test StringTokenizer ----------
        TokenInput strInput = new TokenInput("  \"hello\\nworld\\\"escaped\\\\\"  ");
        TokenReader strReader = new StringTokenizer();
        Token s1 = strReader.tryGet(strInput);
        assert s1 != null && s1.getType() == TokenType.STRING;
        assert s1.getText().equals("hello\nworld\"escaped\\"); // unescaped content
        assert strInput.getPos() == 31; // after closing quote

        // ---------- Test IdentifierTokenizer ----------
        TokenInput idInput = new TokenInput("  _myVar123  while");
        TokenReader idReader = new IdentifierTokenizer();
        Token id1 = idReader.tryGet(idInput);
        assert id1 != null && id1.getText().equals("_myVar123") && id1.getType() == TokenType.IDENTIFIER;
        assert idInput.getPos() == 11;
        Token id2 = idReader.tryGet(idInput);
        assert id2 != null && id2.getText().equals("while") && id2.getType() == TokenType.IDENTIFIER;

        // ---------- Test LinearParser (sequence) ----------
        // Build a parser for "hello world" as a single composite token
        TokenReader linear = new LinearParser(
                new KeywordTokenizer("hello"),
                new KeywordTokenizer("world")
        );
        TokenInput linearInput = new TokenInput("  hello world  ");
        Token comp = linear.tryGet(linearInput);
        assert comp != null && comp.getType() == TokenType.COMPOSITE;
        assert comp.getText().equals("helloworld"); // concatenated texts
        assert linearInput.getPos() == 13; // "  hello world" length
        // Should fail on incomplete input
        TokenInput failInput = new TokenInput("  hello");
        Token fail = linear.tryGet(failInput);
        assert fail == null && failInput.getPos() == 0; // unchanged

        // ---------- Test SelectParser (choice) ----------
        TokenReader select = new SelectParser(
                new KeywordTokenizer("if"),
                new KeywordTokenizer("while"),
                new KeywordTokenizer("for")
        );
        TokenInput selectInput = new TokenInput("  while x");
        Token chosen = select.tryGet(selectInput);
        assert chosen != null && chosen.getText().equals("while") && chosen.getType() == TokenType.KEYWORD;
        assert selectInput.getPos() == 7; // "  while"
        // Should fail on unknown keyword
        TokenInput selectFail = new TokenInput("  switch");
        Token noMatch = select.tryGet(selectFail);
        assert noMatch == null && selectFail.getPos() == 0;

        System.out.println("All assertions passed.");
    }
}

// ---------- TokenType and Token ----------
enum TokenType {
    KEYWORD, INTEGER, FLOAT, STRING, IDENTIFIER, COMPOSITE
}

interface Token {
    int getPos();
    String getText();
    TokenType getType();
}

// ---------- TokenInput (unchanged) ----------
class TokenInput {
    private final String text;
    private int pos;

    public TokenInput(String text) {
        this.text = text;
        this.pos = 0;
    }

    public int getPos() { return pos; }
    public void setPos(int pos) { this.pos = pos; }
    public String getText() { return text; }
    public int peek() { return pos < text.length() ? text.charAt(pos) : -1; }
    public int read() { return pos < text.length() ? text.charAt(pos++) : -1; }
    public boolean isEOF() { return pos >= text.length(); }
}

// ---------- TokenReader ----------
interface TokenReader {
    Token tryGet(TokenInput input);
}

// ---------- KeywordTokenizer (merges whitespace skipping) ----------
class KeywordTokenizer implements TokenReader {
    private final String keyword;

    public KeywordTokenizer(String keyword) {
        this.keyword = keyword;
    }

    @Override
    public Token tryGet(TokenInput input) {
        int originalPos = input.getPos();
        // Skip leading whitespace
        while (!input.isEOF() && Character.isWhitespace(input.peek())) {
            input.read();
        }
        int startPos = input.getPos();

        // Try to match keyword
        for (int i = 0; i < keyword.length(); i++) {
            int ch = input.read();
            if (ch == -1 || ch != keyword.charAt(i)) {
                input.setPos(originalPos);
                return null;
            }
        }
        // Success
        return new Token() {
            private final int pos = startPos;
            private final String text = keyword;
            @Override public int getPos() { return pos; }
            @Override public String getText() { return text; }
            @Override public TokenType getType() { return TokenType.KEYWORD; }
        };
    }
}

// ---------- IntegerTokenizer (unchanged except it already skips whitespace) ----------
class IntegerTokenizer implements TokenReader {
    @Override
    public Token tryGet(TokenInput input) {
        int originalPos = input.getPos();
        while (!input.isEOF() && Character.isWhitespace(input.peek())) {
            input.read();
        }
        int startPos = input.getPos();
        if (input.isEOF() || !isDigit(input.peek())) {
            input.setPos(originalPos);
            return null;
        }
        StringBuilder sb = new StringBuilder();
        while (!input.isEOF() && isDigit(input.peek())) {
            sb.append((char) input.read());
        }
        return new Token() {
            private final int pos = startPos;
            private final String text = sb.toString();
            @Override public int getPos() { return pos; }
            @Override public String getText() { return text; }
            @Override public TokenType getType() { return TokenType.INTEGER; }
        };
    }
    private boolean isDigit(int c) { return c >= '0' && c <= '9'; }
}

// ---------- FloatTokenizer (unchanged) ----------
class FloatTokenizer implements TokenReader {
    @Override
    public Token tryGet(TokenInput input) {
        int originalPos = input.getPos();
        while (!input.isEOF() && Character.isWhitespace(input.peek())) {
            input.read();
        }
        int startPos = input.getPos();
        int beforeParse = input.getPos();

        boolean hasInteger = false;
        while (!input.isEOF() && isDigit(input.peek())) {
            hasInteger = true;
            input.read();
        }

        boolean hasFraction = false;
        if (!input.isEOF() && input.peek() == '.') {
            input.read();
            if (!input.isEOF() && isDigit(input.peek())) {
                hasFraction = true;
                while (!input.isEOF() && isDigit(input.peek())) {
                    input.read();
                }
            } else {
                input.setPos(originalPos);
                return null;
            }
        }

        if (!hasInteger && !hasFraction) {
            input.setPos(originalPos);
            return null;
        }

        if (!input.isEOF() && (input.peek() == 'e' || input.peek() == 'E')) {
            input.read();
            if (!input.isEOF() && (input.peek() == '+' || input.peek() == '-')) {
                input.read();
            }
            if (input.isEOF() || !isDigit(input.peek())) {
                input.setPos(originalPos);
                return null;
            }
            while (!input.isEOF() && isDigit(input.peek())) {
                input.read();
            }
        }

        int endPos = input.getPos();
        String floatText = input.getText().substring(startPos, endPos);
        return new Token() {
            private final int pos = startPos;
            private final String text = floatText;
            @Override public int getPos() { return pos; }
            @Override public String getText() { return text; }
            @Override public TokenType getType() { return TokenType.FLOAT; }
        };
    }
    private boolean isDigit(int c) { return c >= '0' && c <= '9'; }
}

// ---------- StringTokenizer (unchanged) ----------
class StringTokenizer implements TokenReader {
    @Override
    public Token tryGet(TokenInput input) {
        int originalPos = input.getPos();
        while (!input.isEOF() && Character.isWhitespace(input.peek())) {
            input.read();
        }
        int startPos = input.getPos();
        if (input.isEOF() || input.peek() != '"') {
            input.setPos(originalPos);
            return null;
        }
        input.read(); // consume "
        StringBuilder sb = new StringBuilder();
        boolean escaped = false;
        while (!input.isEOF()) {
            int ch = input.read();
            if (escaped) {
                switch (ch) {
                    case '"':  sb.append('"'); break;
                    case '\\': sb.append('\\'); break;
                    case 'n':  sb.append('\n'); break;
                    case 'r':  sb.append('\r'); break;
                    case 't':  sb.append('\t'); break;
                    default:   sb.append('\\').append((char)ch);
                }
                escaped = false;
            } else {
                if (ch == '"') {
                    return new Token() {
                        private final int pos = startPos;
                        private final String text = sb.toString();
                        @Override public int getPos() { return pos; }
                        @Override public String getText() { return text; }
                        @Override public TokenType getType() { return TokenType.STRING; }
                    };
                } else if (ch == '\\') {
                    escaped = true;
                } else {
                    sb.append((char) ch);
                }
            }
        }
        input.setPos(originalPos);
        return null;
    }
}

// ---------- IdentifierTokenizer (unchanged) ----------
class IdentifierTokenizer implements TokenReader {
    @Override
    public Token tryGet(TokenInput input) {
        int originalPos = input.getPos();
        while (!input.isEOF() && Character.isWhitespace(input.peek())) {
            input.read();
        }
        int startPos = input.getPos();
        if (input.isEOF()) {
            input.setPos(originalPos);
            return null;
        }
        int first = input.peek();
        if (!isLetter(first) && first != '_') {
            input.setPos(originalPos);
            return null;
        }
        input.read();
        StringBuilder sb = new StringBuilder();
        sb.append((char) first);
        while (!input.isEOF() && (isLetter(input.peek()) || isDigit(input.peek()) || input.peek() == '_')) {
            sb.append((char) input.read());
        }
        return new Token() {
            private final int pos = startPos;
            private final String text = sb.toString();
            @Override public int getPos() { return pos; }
            @Override public String getText() { return text; }
            @Override public TokenType getType() { return TokenType.IDENTIFIER; }
        };
    }
    private boolean isLetter(int c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
    private boolean isDigit(int c) { return c >= '0' && c <= '9'; }
}

// ---------- LinearParser (sequence) ----------
class LinearParser implements TokenReader {
    private final TokenReader[] parsers;

    public LinearParser(TokenReader... parsers) {
        this.parsers = parsers;
    }

    @Override
    public Token tryGet(TokenInput input) {
        int originalPos = input.getPos();
        List<Token> tokens = new ArrayList<>();
        for (TokenReader parser : parsers) {
            Token t = parser.tryGet(input);
            if (t == null) {
                input.setPos(originalPos);
                return null;
            }
            tokens.add(t);
        }
        // Build a composite token
        int startPos = tokens.get(0).getPos();
        StringBuilder combinedText = new StringBuilder();
        for (Token t : tokens) {
            combinedText.append(t.getText());
        }
        return new Token() {
            private final int pos = startPos;
            private final String text = combinedText.toString();
            @Override public int getPos() { return pos; }
            @Override public String getText() { return text; }
            @Override public TokenType getType() { return TokenType.COMPOSITE; }
        };
    }
}

// ---------- SelectParser (choice) ----------
class SelectParser implements TokenReader {
    private final TokenReader[] parsers;

    public SelectParser(TokenReader... parsers) {
        this.parsers = parsers;
    }

    @Override
    public Token tryGet(TokenInput input) {
        int originalPos = input.getPos();
        for (TokenReader parser : parsers) {
            // Each parser is tried from the original position
            Token t = parser.tryGet(input);
            if (t != null) {
                return t; // success, position already advanced
            }
            // Reset position for next attempt
            input.setPos(originalPos);
        }
        return null;
    }
}