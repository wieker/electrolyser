package org.allesoft.correlator.java_c_ide;

import java.io.*;
import java.util.*;

public class JavaInterpreter {

    public static void main(String[] args) {
        // ---------- Test MoreParser (zero-or-more repetition) ----------
        // Build a parser for one or more digits (like IntegerTokenizer but as repetition of single digit)
        TokenReader digitParser = new SelectParser(
                new KeywordTokenizer("0"), new KeywordTokenizer("1"), new KeywordTokenizer("2"),
                new KeywordTokenizer("3"), new KeywordTokenizer("4"), new KeywordTokenizer("5"),
                new KeywordTokenizer("6"), new KeywordTokenizer("7"), new KeywordTokenizer("8"),
                new KeywordTokenizer("9")
        );
        TokenReader moreDigits = new MoreParser(digitParser);
        TokenInput moreInput = new TokenInput("  123abc");
        Token m1 = moreDigits.tryGet(moreInput);
        assert m1 != null && m1.getType() == TokenType.COMPOSITE;
        assert m1.getText().equals("123"); // digits concatenated
        assert moreInput.getPos() == 5; // "  123"
        // Now try at position where no digits exist
        Token m2 = moreDigits.tryGet(moreInput); // at 'a'
        assert m2 != null && m2.getText().equals(""); // zero repetitions yields empty token
        // Position should not advance
        assert moreInput.getPos() == 5;

        // ---------- Test Grammar Definition Parser ----------
        String grammar = ""
                + "integer intParser;\n"
                + "float floatParser;\n"
                + "string stringParser;\n"
                + "identifier idParser;\n"
                + "keyword kwIf if;\n"
                + "linear helloWorld kwHello kwWorld;\n"
                + "select choice kwIf kwHello;\n"
                + "more stars kwStar;\n"; // Note: kwStar must be defined; we'll add it
        // We need to define kwHello, kwWorld, kwStar as keywords
        // Let's extend grammar to include those definitions:
        grammar += "keyword kwHello hello;\n";
        grammar += "keyword kwWorld world;\n";
        grammar += "keyword kwStar *;\n"; // '*' is not an identifier, but we can handle it as a keyword literal
        // However our keyword parser expects the literal as an identifier token; '*' would be a separate token.
        // To keep it simple, we'll use letters for keywords: "star" instead of "*".
        grammar = "integer intParser;\n"
                + "float floatParser;\n"
                + "string stringParser;\n"
                + "identifier idParser;\n"
                + "keyword kwIf if;\n"
                + "keyword kwHello hello;\n"
                + "keyword kwWorld world;\n"
                + "keyword kwStar star;\n"
                + "linear helloWorld kwHello kwWorld;\n"
                + "select choice kwIf kwHello;\n"
                + "more stars kwStar;\n";

        // Parse the grammar
        List<ParserDef> defs = GrammarParser.parse(grammar);
        assert defs.size() == 11;

        // Build parsers
        ParserContext context = new ParserContext();
        GrammarParser.buildParsers(defs, context);

        // Test each created parser
        // intParser
        TokenReader intParser = context.get("intParser");
        TokenInput intTest = new TokenInput("  456");
        Token tInt = intParser.tryGet(intTest);
        assert tInt != null && tInt.getType() == TokenType.INTEGER && tInt.getText().equals("456");

        // floatParser
        TokenReader floatParser = context.get("floatParser");
        TokenInput floatTest = new TokenInput("  3.14");
        Token tFloat = floatParser.tryGet(floatTest);
        assert tFloat != null && tFloat.getType() == TokenType.FLOAT && tFloat.getText().equals("3.14");

        // stringParser
        TokenReader stringParser = context.get("stringParser");
        TokenInput stringTest = new TokenInput("  \"hello\"");
        Token tString = stringParser.tryGet(stringTest);
        assert tString != null && tString.getType() == TokenType.STRING && tString.getText().equals("hello");

        // idParser
        TokenReader idParser = context.get("idParser");
        TokenInput idTest = new TokenInput("  _var123");
        Token tId = idParser.tryGet(idTest);
        assert tId != null && tId.getType() == TokenType.IDENTIFIER && tId.getText().equals("_var123");

        // kwIf
        TokenReader kwIf = context.get("kwIf");
        TokenInput ifTest = new TokenInput("  if");
        Token tIf = kwIf.tryGet(ifTest);
        assert tIf != null && tIf.getType() == TokenType.KEYWORD && tIf.getText().equals("if");

        // helloWorld (linear)
        TokenReader helloWorld = context.get("helloWorld");
        TokenInput hwTest = new TokenInput("  hello world");
        Token tHw = helloWorld.tryGet(hwTest);
        assert tHw != null && tHw.getType() == TokenType.COMPOSITE && tHw.getText().equals("helloworld");

        // choice (select)
        TokenReader choice = context.get("choice");
        TokenInput choiceTest = new TokenInput("  if something");
        Token tChoice = choice.tryGet(choiceTest);
        assert tChoice != null && tChoice.getType() == TokenType.KEYWORD && tChoice.getText().equals("if");
        TokenInput choiceTest2 = new TokenInput("  hello");
        Token tChoice2 = choice.tryGet(choiceTest2);
        assert tChoice2 != null && tChoice2.getType() == TokenType.KEYWORD && tChoice2.getText().equals("hello");

        // stars (more)
        TokenReader stars = context.get("stars");
        TokenInput starsTest = new TokenInput("  star star star");
        Token tStars = stars.tryGet(starsTest);
        assert tStars != null && tStars.getType() == TokenType.COMPOSITE;
        assert tStars.getText().equals("starstarstar"); // repeated "star"

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

// ---------- KeywordTokenizer ----------
class KeywordTokenizer implements TokenReader {
    private final String keyword;

    public KeywordTokenizer(String keyword) {
        this.keyword = keyword;
    }

    @Override
    public Token tryGet(TokenInput input) {
        int originalPos = input.getPos();
        while (!input.isEOF() && Character.isWhitespace(input.peek())) {
            input.read();
        }
        int startPos = input.getPos();
        for (int i = 0; i < keyword.length(); i++) {
            int ch = input.read();
            if (ch == -1 || ch != keyword.charAt(i)) {
                input.setPos(originalPos);
                return null;
            }
        }
        return new Token() {
            private final int pos = startPos;
            private final String text = keyword;
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

// ---------- FloatTokenizer ----------
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

// ---------- StringTokenizer ----------
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

// ---------- IdentifierTokenizer ----------
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

// ---------- LinearParser ----------
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

// ---------- SelectParser ----------
class SelectParser implements TokenReader {
    private final TokenReader[] parsers;

    public SelectParser(TokenReader... parsers) {
        this.parsers = parsers;
    }

    @Override
    public Token tryGet(TokenInput input) {
        int originalPos = input.getPos();
        for (TokenReader parser : parsers) {
            Token t = parser.tryGet(input);
            if (t != null) {
                return t;
            }
            input.setPos(originalPos);
        }
        return null;
    }
}

// ---------- MoreParser (zero-or-more) ----------
class MoreParser implements TokenReader {
    private final TokenReader parser;

    public MoreParser(TokenReader parser) {
        this.parser = parser;
    }

    @Override
    public Token tryGet(TokenInput input) {
        int startPos = input.getPos(); // we'll need to track start after whitespace? Actually we want to include leading whitespace? The subparser already skips whitespace, so startPos should be after any whitespace that the subparser consumes on its first successful call. But if zero repetitions, we return an empty token at current position? Let's define: we return a composite token that starts at the position after any whitespace that was skipped before the first successful parse. However, if zero repetitions, we return an empty token at the current position (no whitespace skipped). To keep it simple, we'll record the position after any initial whitespace that the subparser would skip on its first attempt, but if it fails, we don't advance. We'll implement by accumulating tokens.
        int originalPos = input.getPos();
        List<Token> tokens = new ArrayList<>();
        while (true) {
            // We need to mark position before each attempt to backtrack if fails
            int beforeAttempt = input.getPos();
            Token t = parser.tryGet(input);
            if (t == null) {
                input.setPos(beforeAttempt); // restore to before attempt (no progress)
                break;
            }
            tokens.add(t);
        }
        if (tokens.isEmpty()) {
            // Return an empty token at original position
            return new Token() {
                private final int pos = originalPos;
                private final String text = "";
                @Override public int getPos() { return pos; }
                @Override public String getText() { return text; }
                @Override public TokenType getType() { return TokenType.COMPOSITE; }
            };
        }
        int start = tokens.get(0).getPos();
        StringBuilder sb = new StringBuilder();
        for (Token t : tokens) {
            sb.append(t.getText());
        }
        return new Token() {
            private final int pos = start;
            private final String text = sb.toString();
            @Override public int getPos() { return pos; }
            @Override public String getText() { return text; }
            @Override public TokenType getType() { return TokenType.COMPOSITE; }
        };
    }
}

// ---------- Parser Definition and Context ----------
class ParserDef {
    String type;
    String name;
    List<String> args;
    ParserDef(String type, String name, List<String> args) {
        this.type = type;
        this.name = name;
        this.args = args;
    }
}

class ParserContext {
    private Map<String, TokenReader> parsers = new HashMap<>();

    public void define(String name, TokenReader parser) {
        parsers.put(name, parser);
    }

    public TokenReader get(String name) {
        TokenReader pr = parsers.get(name);
        if (pr == null) throw new IllegalArgumentException("Undefined parser: " + name);
        return pr;
    }
}

// ---------- Grammar Parser ----------
class GrammarParser {
    // Parse a grammar string into a list of ParserDef
    public static List<ParserDef> parse(String grammar) {
        TokenInput input = new TokenInput(grammar);
        List<ParserDef> defs = new ArrayList<>();

        // Build tokenizers for grammar tokens
        TokenReader typeParser = new SelectParser(
                new KeywordTokenizer("integer"),
                new KeywordTokenizer("float"),
                new KeywordTokenizer("string"),
                new KeywordTokenizer("identifier"),
                new KeywordTokenizer("keyword"),
                new KeywordTokenizer("linear"),
                new KeywordTokenizer("select"),
                new KeywordTokenizer("more")
        );
        TokenReader idParser = new IdentifierTokenizer();
        TokenReader semiParser = new KeywordTokenizer(";");

        while (!input.isEOF()) {
            // Skip possible empty lines
            int startPos = input.getPos();
            // Try to parse a definition line
            Token typeToken = typeParser.tryGet(input);
            if (typeToken == null) {
                // No type found; maybe blank line or whitespace only
                // Advance to next line? We'll just break if we can't find anything
                // But to be robust, we should skip whitespace and newlines
                // Simpler: if we're at EOF after skipping whitespace, break
                while (!input.isEOF() && Character.isWhitespace(input.peek())) {
                    input.read();
                }
                if (input.isEOF()) break;
                // Otherwise, it's an error
                throw new RuntimeException("Expected type at position " + input.getPos());
            }
            String type = typeToken.getText();

            Token nameToken = idParser.tryGet(input);
            if (nameToken == null) {
                throw new RuntimeException("Expected parser name after type at position " + input.getPos());
            }
            String name = nameToken.getText();

            List<String> args = new ArrayList<>();
            while (true) {
                // Try to get an identifier argument
                Token argToken = idParser.tryGet(input);
                if (argToken == null) {
                    break;
                }
                args.add(argToken.getText());
            }

            Token semi = semiParser.tryGet(input);
            if (semi == null) {
                throw new RuntimeException("Expected ';' at end of definition at position " + input.getPos());
            }

            defs.add(new ParserDef(type, name, args));
        }
        return defs;
    }

    // Build actual parsers from definitions (second pass)
    public static void buildParsers(List<ParserDef> defs, ParserContext context) {
        // First, create all built-in parsers (those that don't depend on others)
        for (ParserDef def : defs) {
            TokenReader parser = null;
            switch (def.type) {
                case "integer":
                    parser = new IntegerTokenizer();
                    break;
                case "float":
                    parser = new FloatTokenizer();
                    break;
                case "string":
                    parser = new StringTokenizer();
                    break;
                case "identifier":
                    parser = new IdentifierTokenizer();
                    break;
                case "keyword":
                    // keyword expects one argument: the literal
                    if (def.args.size() != 1) {
                        throw new RuntimeException("Keyword parser '" + def.name + "' requires exactly one argument (the literal)");
                    }
                    parser = new KeywordTokenizer(def.args.get(0));
                    break;
                // combinators are handled later
                default:
                    // skip for now
            }
            if (parser != null) {
                context.define(def.name, parser);
            }
        }

        // Now build combinators (which may refer to other parsers)
        for (ParserDef def : defs) {
            if (def.type.equals("linear") || def.type.equals("select") || def.type.equals("more")) {
                // Resolve arguments to actual parsers
                TokenReader[] subParsers = new TokenReader[def.args.size()];
                for (int i = 0; i < def.args.size(); i++) {
                    subParsers[i] = context.get(def.args.get(i));
                }
                TokenReader parser;
                switch (def.type) {
                    case "linear":
                        parser = new LinearParser(subParsers);
                        break;
                    case "select":
                        parser = new SelectParser(subParsers);
                        break;
                    case "more":
                        if (subParsers.length != 1) {
                            throw new RuntimeException("More parser '" + def.name + "' requires exactly one subparser");
                        }
                        parser = new MoreParser(subParsers[0]);
                        break;
                    default:
                        throw new RuntimeException("Unknown combinator type: " + def.type);
                }
                context.define(def.name, parser);
            }
        }
    }
}