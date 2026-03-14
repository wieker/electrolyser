package org.allesoft.correlator.java_c_ide;

import java.io.*;
import java.util.*;

public class JavaInterpreter {

    public static void main(String[] args) {
        TestSuite tests = new TestSuite();

        // ---------- Test KeywordTokenizer (with built‑in whitespace skip) ----------
        tests.test("KeywordTokenizer", () -> {
            TokenInput input = new TokenInput("  hello world");
            TokenReader helloReader = new KeywordTokenizer("hello");
            TokenReader worldReader = new KeywordTokenizer("world");

            Token t1 = helloReader.tryGet(input);
            tests.check(t1 != null, "t1 should not be null");
            tests.check(t1.getText().equals("hello"), "t1 text should be 'hello'");
            tests.check(t1.getType() == TokenType.KEYWORD, "t1 type should be KEYWORD");
            tests.check(input.getPos() == 7, "position after 'hello' should be 7");

            Token t2 = worldReader.tryGet(input);
            tests.check(t2 != null, "t2 should not be null");
            tests.check(t2.getText().equals("world"), "t2 text should be 'world'");
            tests.check(t2.getType() == TokenType.KEYWORD, "t2 type should be KEYWORD");
            tests.check(input.getPos() == 13, "position after 'world' should be 13");

            Token t3 = helloReader.tryGet(input);
            tests.check(t3 == null, "t3 should be null");
            tests.check(input.getPos() == 13, "position should remain 13");
        });

        // ---------- Test IntegerTokenizer ----------
        tests.test("IntegerTokenizer", () -> {
            TokenInput input = new TokenInput("   123 456");
            TokenReader intReader = new IntegerTokenizer();
            Token t1 = intReader.tryGet(input);
            tests.check(t1 != null && t1.getText().equals("123") && t1.getType() == TokenType.INTEGER, "first integer");
            tests.check(input.getPos() == 7, "position after 123");

            Token t2 = intReader.tryGet(input);
            tests.check(t2 == null, "second attempt should fail");
            tests.check(input.getPos() == 7, "position unchanged");
        });

        // ---------- Test FloatTokenizer ----------
        tests.test("FloatTokenizer", () -> {
            TokenReader floatReader = new FloatTokenizer();
            String[] cases = {"  3.14", "  .5", "  2e10", "  -1.2e-3"};
            String[] expected = {"3.14", ".5", "2e10", "-1.2e-3"};
            for (int i = 0; i < cases.length; i++) {
                TokenInput input = new TokenInput(cases[i]);
                Token t = floatReader.tryGet(input);
                tests.check(t != null && t.getText().equals(expected[i]) && t.getType() == TokenType.FLOAT,
                        "float case " + i + ": " + expected[i]);
            }
        });

        // ---------- Test StringTokenizer ----------
        tests.test("StringTokenizer", () -> {
            TokenInput input = new TokenInput("  \"hello\\nworld\\\"escaped\\\\\"  ");
            TokenReader strReader = new StringTokenizer();
            Token t = strReader.tryGet(input);
            tests.check(t != null && t.getType() == TokenType.STRING, "string token");
            tests.check(t.getText().equals("hello\nworld\"escaped\\"), "unescaped content");
            tests.check(input.getPos() == 31, "position after string");
        });

        // ---------- Test IdentifierTokenizer ----------
        tests.test("IdentifierTokenizer", () -> {
            TokenInput input = new TokenInput("  _myVar123  while");
            TokenReader idReader = new IdentifierTokenizer();
            Token t1 = idReader.tryGet(input);
            tests.check(t1 != null && t1.getText().equals("_myVar123") && t1.getType() == TokenType.IDENTIFIER, "first identifier");
            tests.check(input.getPos() == 11, "position after _myVar123");
            Token t2 = idReader.tryGet(input);
            tests.check(t2 != null && t2.getText().equals("while") && t2.getType() == TokenType.IDENTIFIER, "second identifier");
        });

        // ---------- Test MoreParser ----------
        tests.test("MoreParser", () -> {
            // Build a parser for digits (select of single digits)
            TokenReader digitParser = new SelectParser(null, // context not needed for these basic parsers
                    new KeywordTokenizer("0"), new KeywordTokenizer("1"), new KeywordTokenizer("2"),
                    new KeywordTokenizer("3"), new KeywordTokenizer("4"), new KeywordTokenizer("5"),
                    new KeywordTokenizer("6"), new KeywordTokenizer("7"), new KeywordTokenizer("8"),
                    new KeywordTokenizer("9")
            );
            // Wrap in MoreParser – we need a context; create a dummy context that contains digitParser
            ParserContext ctx = new ParserContext();
            ctx.define("digit", digitParser);
            TokenReader moreDigits = new MoreParser(ctx, "digit");
            TokenInput input = new TokenInput("  123abc");
            Token t1 = moreDigits.tryGet(input);
            tests.check(t1 != null && t1.getType() == TokenType.COMPOSITE && t1.getText().equals("123"), "more digits");
            tests.check(input.getPos() == 5, "position after digits");
            Token t2 = moreDigits.tryGet(input);
            tests.check(t2 != null && t2.getText().equals(""), "zero repetitions yields empty token");
            tests.check(input.getPos() == 5, "position unchanged");
        });

        // ---------- Test Grammar Definition Parser (including recursion) ----------
        tests.test("GrammarParser", () -> {
            String grammar = ""
                    + "integer intParser;\n"
                    + "float floatParser;\n"
                    + "string stringParser;\n"
                    + "identifier idParser;\n"
                    + "keyword kwIf if;\n"
                    + "keyword kwHello hello;\n"
                    + "keyword kwWorld world;\n"
                    + "keyword kwStar star;\n"
                    + "linear helloWorld kwHello kwWorld;\n"
                    + "select choice kwIf kwHello;\n"
                    + "more stars kwStar;\n"
                    // Recursive grammar: expr = num | sum ; sum = expr plus expr
                    + "integer num;\n"
                    + "keyword plus +;\n"
                    + "linear sum expr plus expr;\n"
                    + "select expr num sum;\n";

            List<ParserDef> defs = GrammarParser.parse(grammar);
            tests.check(defs.size() == 14, "14 definitions parsed");

            ParserContext context = new ParserContext();
            GrammarParser.buildParsers(defs, context);

            // Test each built parser
            TokenReader intParser = context.get("intParser");
            TokenInput intTest = new TokenInput("  456");
            Token tInt = intParser.tryGet(intTest);
            tests.check(tInt != null && tInt.getType() == TokenType.INTEGER && tInt.getText().equals("456"), "intParser");

            TokenReader floatParser = context.get("floatParser");
            TokenInput floatTest = new TokenInput("  3.14");
            Token tFloat = floatParser.tryGet(floatTest);
            tests.check(tFloat != null && tFloat.getType() == TokenType.FLOAT && tFloat.getText().equals("3.14"), "floatParser");

            TokenReader stringParser = context.get("stringParser");
            TokenInput stringTest = new TokenInput("  \"hello\"");
            Token tString = stringParser.tryGet(stringTest);
            tests.check(tString != null && tString.getType() == TokenType.STRING && tString.getText().equals("hello"), "stringParser");

            TokenReader idParser = context.get("idParser");
            TokenInput idTest = new TokenInput("  _var123");
            Token tId = idParser.tryGet(idTest);
            tests.check(tId != null && tId.getType() == TokenType.IDENTIFIER && tId.getText().equals("_var123"), "idParser");

            TokenReader kwIf = context.get("kwIf");
            TokenInput ifTest = new TokenInput("  if");
            Token tIf = kwIf.tryGet(ifTest);
            tests.check(tIf != null && tIf.getType() == TokenType.KEYWORD && tIf.getText().equals("if"), "kwIf");

            TokenReader helloWorld = context.get("helloWorld");
            TokenInput hwTest = new TokenInput("  hello world");
            Token tHw = helloWorld.tryGet(hwTest);
            tests.check(tHw != null && tHw.getType() == TokenType.COMPOSITE && tHw.getText().equals("helloworld"), "helloWorld");

            TokenReader choice = context.get("choice");
            TokenInput choiceTest = new TokenInput("  if something");
            Token tChoice = choice.tryGet(choiceTest);
            tests.check(tChoice != null && tChoice.getType() == TokenType.KEYWORD && tChoice.getText().equals("if"), "choice if");
            TokenInput choiceTest2 = new TokenInput("  hello");
            Token tChoice2 = choice.tryGet(choiceTest2);
            tests.check(tChoice2 != null && tChoice2.getType() == TokenType.KEYWORD && tChoice2.getText().equals("hello"), "choice hello");

            TokenReader stars = context.get("stars");
            TokenInput starsTest = new TokenInput("  star star star");
            Token tStars = stars.tryGet(starsTest);
            tests.check(tStars != null && tStars.getType() == TokenType.COMPOSITE && tStars.getText().equals("starstarstar"), "stars");

            // Test recursive expr parser
            TokenReader expr = context.get("expr");
            TokenInput recTest = new TokenInput("1+2+3");
            Token tRec = expr.tryGet(recTest);
            tests.check(tRec != null, "recursive expr should parse");
            // The exact parse tree is ambiguous, but we just check that something was parsed
            tests.check(tRec.getText().length() > 0, "expr produced non-empty text");
        });

        tests.summarize();
    }

    // Simple test harness
    static class TestSuite {
        private int passed = 0;
        private int failed = 0;
        private List<String> failures = new ArrayList<>();

        void test(String name, Runnable testBlock) {
            try {
                testBlock.run();
                passed++;
                System.out.println("✅ " + name);
            } catch (Exception e) {
                failed++;
                failures.add(name + ": " + e.getMessage());
                System.out.println("❌ " + name + " - " + e.getMessage());
            }
        }

        void check(boolean condition, String message) {
            if (!condition) {
                throw new RuntimeException("Assertion failed: " + message);
            }
        }

        void summarize() {
            System.out.println("\nTests passed: " + passed + ", failed: " + failed);
            if (!failures.isEmpty()) {
                System.out.println("Failures:");
                for (String f : failures) {
                    System.out.println("  " + f);
                }
                throw new RuntimeException("Some tests failed");
            } else {
                System.out.println("All tests passed!");
            }
        }
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

// ---------- TokenInput ----------
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

// ---------- LinearParser (with lazy resolution) ----------
class LinearParser implements TokenReader {
    private final ParserContext context;
    private final String[] subParserNames;

    public LinearParser(ParserContext context, String... subParserNames) {
        this.context = context;
        this.subParserNames = subParserNames;
    }

    @Override
    public Token tryGet(TokenInput input) {
        int originalPos = input.getPos();
        List<Token> tokens = new ArrayList<>();
        for (String name : subParserNames) {
            TokenReader parser = context.get(name);
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

// ---------- SelectParser (with lazy resolution) ----------
class SelectParser implements TokenReader {
    private final ParserContext context;
    private final String[] subParserNames;

    public SelectParser(ParserContext context, String... subParserNames) {
        this.context = context;
        this.subParserNames = subParserNames;
    }

    @Override
    public Token tryGet(TokenInput input) {
        int originalPos = input.getPos();
        for (String name : subParserNames) {
            TokenReader parser = context.get(name);
            Token t = parser.tryGet(input);
            if (t != null) {
                return t;
            }
            input.setPos(originalPos);
        }
        return null;
    }
}

// ---------- MoreParser (with lazy resolution) ----------
class MoreParser implements TokenReader {
    private final ParserContext context;
    private final String subParserName;

    public MoreParser(ParserContext context, String subParserName) {
        this.context = context;
        this.subParserName = subParserName;
    }

    @Override
    public Token tryGet(TokenInput input) {
        int originalPos = input.getPos();
        List<Token> tokens = new ArrayList<>();
        while (true) {
            int beforeAttempt = input.getPos();
            TokenReader parser = context.get(subParserName);
            Token t = parser.tryGet(input);
            if (t == null) {
                input.setPos(beforeAttempt);
                break;
            }
            tokens.add(t);
        }
        if (tokens.isEmpty()) {
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

// ---------- ParserContext ----------
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

// ---------- Parser Definition ----------
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

// ---------- Grammar Parser ----------
class GrammarParser {
    public static List<ParserDef> parse(String grammar) {
        TokenInput input = new TokenInput(grammar);
        List<ParserDef> defs = new ArrayList<>();

        TokenReader typeParser = new SelectParser(null,
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
            // Skip whitespace and newlines
            while (!input.isEOF() && Character.isWhitespace(input.peek())) {
                input.read();
            }
            if (input.isEOF()) break;

            Token typeToken = typeParser.tryGet(input);
            if (typeToken == null) {
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
                Token argToken = idParser.tryGet(input);
                if (argToken == null) break;
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

    public static void buildParsers(List<ParserDef> defs, ParserContext context) {
        // First pass: create all parsers and register them (names only)
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
                    if (def.args.size() != 1) {
                        throw new RuntimeException("Keyword parser '" + def.name + "' requires exactly one argument (the literal)");
                    }
                    parser = new KeywordTokenizer(def.args.get(0));
                    break;
                case "linear":
                    parser = new LinearParser(context, def.args.toArray(new String[0]));
                    break;
                case "select":
                    parser = new SelectParser(context, def.args.toArray(new String[0]));
                    break;
                case "more":
                    if (def.args.size() != 1) {
                        throw new RuntimeException("More parser '" + def.name + "' requires exactly one subparser");
                    }
                    parser = new MoreParser(context, def.args.get(0));
                    break;
                default:
                    throw new RuntimeException("Unknown parser type: " + def.type);
            }
            context.define(def.name, parser);
        }
    }
}