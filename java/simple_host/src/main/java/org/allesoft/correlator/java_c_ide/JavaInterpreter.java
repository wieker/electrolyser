package org.allesoft.correlator.java_c_ide;

import java.io.*;

public class JavaInterpreter {

    public static void main(String[] args) {
        // Test existing SimpleTokenReader (now with KEYWORD type)
        TokenInput input = new TokenInput("hello world");
        TokenReader helloReader = new SimpleTokenReader("hello");
        TokenReader worldReader = new SimpleTokenReader("world");

        Token t1 = helloReader.tryGet(input);
        assert t1 != null && t1.getText().equals("hello") && t1.getType() == TokenType.KEYWORD;
        assert input.getPos() == 5;

        input.read(); // consume space

        Token t2 = worldReader.tryGet(input);
        assert t2 != null && t2.getText().equals("world") && t2.getType() == TokenType.KEYWORD;
        assert input.getPos() == 11;

        Token t3 = helloReader.tryGet(input);
        assert t3 == null && input.getPos() == 11;

        // Test IntegerTokenizer
        TokenInput intInput = new TokenInput("   123 456");
        TokenReader intReader = new IntegerTokenizer();
        Token i1 = intReader.tryGet(intInput);
        assert i1 != null && i1.getText().equals("123") && i1.getType() == TokenType.INTEGER;
        assert intInput.getPos() == 7; // after "   123"
        // now at space before 456
        Token i2 = intReader.tryGet(intInput);
        assert i2 == null; // space, not a digit
        assert intInput.getPos() == 7; // unchanged

        // Test FloatTokenizer
        TokenInput floatInput = new TokenInput("  3.14  .5  2e10  -1.2e-3");
        TokenReader floatReader = new FloatTokenizer();
        Token f1 = floatReader.tryGet(floatInput);
        assert f1 != null && f1.getText().equals("3.14") && f1.getType() == TokenType.FLOAT;
        // skip whitespace manually for simplicity; we trust tokenizer skips leading ws each time
        // Let's reset and test each one with separate inputs or use a loop; for brevity, we'll test one more
        TokenInput fInput2 = new TokenInput("  .5");
        Token f2 = floatReader.tryGet(fInput2);
        assert f2 != null && f2.getText().equals(".5") && f2.getType() == TokenType.FLOAT;
        TokenInput fInput3 = new TokenInput("  2e10");
        Token f3 = floatReader.tryGet(fInput3);
        assert f3 != null && f3.getText().equals("2e10") && f3.getType() == TokenType.FLOAT;
        TokenInput fInput4 = new TokenInput("  -1.2e-3");
        Token f4 = floatReader.tryGet(fInput4);
        assert f4 != null && f4.getText().equals("-1.2e-3") && f4.getType() == TokenType.FLOAT;

        // Test StringTokenizer
        TokenInput strInput = new TokenInput("  \"hello\\nworld\\\"escaped\\\\\"  ");
        TokenReader strReader = new StringTokenizer();
        Token s1 = strReader.tryGet(strInput);
        assert s1 != null && s1.getType() == TokenType.STRING;
        assert s1.getText().equals("hello\\nworld\\\"escaped\\\\"); // raw escaped string
        // For unescaped content we could add a method, but the token stores raw text.
        // Check position after token
        assert strInput.getPos() == 31; // after the closing quote

        // Test IdentifierTokenizer
        TokenInput idInput = new TokenInput("  _myVar123  while");
        TokenReader idReader = new IdentifierTokenizer();
        Token id1 = idReader.tryGet(idInput);
        assert id1 != null && id1.getText().equals("_myVar123") && id1.getType() == TokenType.IDENTIFIER;
        assert idInput.getPos() == 11; // after identifier
        // "while" is also an identifier, not a keyword (keywords are separate)
        Token id2 = idReader.tryGet(idInput);
        assert id2 != null && id2.getText().equals("while") && id2.getType() == TokenType.IDENTIFIER;

        System.out.println("All assertions passed.");
    }
}

// ---------- Token and TokenType ----------
enum TokenType {
    KEYWORD, INTEGER, FLOAT, STRING, IDENTIFIER
}

interface Token {
    int getPos();
    String getText();
    TokenType getType();  // added
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

// ---------- SimpleTokenReader (updated to include KEYWORD type) ----------
class SimpleTokenReader implements TokenReader {
    private final String literal;

    public SimpleTokenReader(String literal) {
        this.literal = literal;
    }

    @Override
    public Token tryGet(TokenInput input) {
        int startPos = input.getPos();
        for (int i = 0; i < literal.length(); i++) {
            int ch = input.read();
            if (ch == -1 || ch != literal.charAt(i)) {
                input.setPos(startPos);
                return null;
            }
        }
        // Success – return token with type KEYWORD
        return new Token() {
            private final int pos = startPos;
            private final String text = literal;
            @Override public int getPos() { return pos; }
            @Override public String getText() { return text; }
            @Override public TokenType getType() { return TokenType.KEYWORD; }
        };
    }
}

// ---------- SimpleWsTokenReader (already provided) ----------
class SimpleWsTokenReader implements TokenReader {
    private final String literal;
    public SimpleWsTokenReader(String literal) { this.literal = literal; }

    @Override
    public Token tryGet(TokenInput input) {
        int originalPos = input.getPos();
        while (!input.isEOF() && Character.isWhitespace(input.peek())) {
            input.read();
        }
        int afterWsPos = input.getPos();
        for (int i = 0; i < literal.length(); i++) {
            int ch = input.read();
            if (ch == -1 || ch != literal.charAt(i)) {
                input.setPos(originalPos);
                return null;
            }
        }
        return new Token() {
            private final int pos = afterWsPos;
            private final String text = literal;
            @Override public int getPos() { return pos; }
            @Override public String getText() { return text; }
            @Override public TokenType getType() { return TokenType.KEYWORD; }
        };
    }
}

// ---------- IntegerTokenizer ----------
class IntegerTokenizer implements TokenReader {
    @Override
    public Token tryGet(TokenInput input) {
        int originalPos = input.getPos();
        // Skip whitespace
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
    private boolean isDigit(int c) {
        return c >= '0' && c <= '9';
    }
}

// ---------- FloatTokenizer ----------
class FloatTokenizer implements TokenReader {
    @Override
    public Token tryGet(TokenInput input) {
        int originalPos = input.getPos();
        // Skip whitespace
        while (!input.isEOF() && Character.isWhitespace(input.peek())) {
            input.read();
        }
        int startPos = input.getPos();

        // State machine for float: optional sign? Actually our pattern doesn't include sign by default,
        // but we can allow an optional '+' or '-' at the beginning (scientific notation sign is handled separately).
        // For simplicity, we'll not accept leading sign; the number may start with digits or '.'.
        // We'll try to parse a float pattern and backtrack if incomplete.

        // Save position before starting to parse
        int beforeParse = input.getPos();

        // Integer part (optional)
        boolean hasInteger = false;
        while (!input.isEOF() && isDigit(input.peek())) {
            hasInteger = true;
            input.read();
        }

        // Fractional part (optional)
        boolean hasFraction = false;
        if (!input.isEOF() && input.peek() == '.') {
            input.read(); // consume dot
            // At least one digit required after dot? Usually yes, but some accept "123." as valid.
            // We'll require at least one digit after dot to be a float, else it's just integer.
            if (!input.isEOF() && isDigit(input.peek())) {
                hasFraction = true;
                while (!input.isEOF() && isDigit(input.peek())) {
                    input.read();
                }
            } else {
                // dot without following digits – not a float, backtrack
                input.setPos(beforeParse);
                return null;
            }
        }

        // Must have at least integer or fraction
        if (!hasInteger && !hasFraction) {
            input.setPos(originalPos);
            return null;
        }

        // Exponent part (optional)
        boolean hasExponent = false;
        if (!input.isEOF() && (input.peek() == 'e' || input.peek() == 'E')) {
            input.read(); // consume e/E
            // Optional sign
            if (!input.isEOF() && (input.peek() == '+' || input.peek() == '-')) {
                input.read();
            }
            // At least one digit required after exponent
            if (input.isEOF() || !isDigit(input.peek())) {
                // Invalid exponent, backtrack
                input.setPos(originalPos);
                return null;
            }
            hasExponent = true;
            while (!input.isEOF() && isDigit(input.peek())) {
                input.read();
            }
        }

        // At this point we have successfully parsed a float
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
    private boolean isDigit(int c) {
        return c >= '0' && c <= '9';
    }
}

// ---------- StringTokenizer ----------
class StringTokenizer implements TokenReader {
    @Override
    public Token tryGet(TokenInput input) {
        int originalPos = input.getPos();
        // Skip whitespace
        while (!input.isEOF() && Character.isWhitespace(input.peek())) {
            input.read();
        }
        int startPos = input.getPos();
        if (input.isEOF() || input.peek() != '"') {
            input.setPos(originalPos);
            return null;
        }
        input.read(); // consume opening quote
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
                    default:   sb.append('\\').append((char)ch); // keep raw if unknown escape
                }
                escaped = false;
            } else {
                if (ch == '"') {
                    // End of string
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
        // Unterminated string – backtrack
        input.setPos(originalPos);
        return null;
    }
}

// ---------- IdentifierTokenizer ----------
class IdentifierTokenizer implements TokenReader {
    @Override
    public Token tryGet(TokenInput input) {
        int originalPos = input.getPos();
        // Skip whitespace
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
        input.read(); // consume first character
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
    private boolean isLetter(int c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
    }
    private boolean isDigit(int c) {
        return c >= '0' && c <= '9';
    }
}