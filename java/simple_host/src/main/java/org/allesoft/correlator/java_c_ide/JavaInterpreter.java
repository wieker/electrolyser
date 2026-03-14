package org.allesoft.correlator.java_c_ide;

import java.io.*;
import java.util.*;

public class JavaInterpreter {

    public static void main(String[] args) {
        TestSuite tests = new TestSuite();

        // ---------- Existing Tests ----------
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

        tests.test("IntegerTokenizer", () -> {
            TokenInput input = new TokenInput("   123 456");
            TokenReader intReader = new IntegerTokenizer();
            Token t1 = intReader.tryGet(input);
            tests.check(t1 != null && t1.getText().equals("123") && t1.getType() == TokenType.INTEGER, "first integer");
            tests.check(input.getPos() == 6, "position after 123 should be 6");

            Token t2 = intReader.tryGet(input);
            //tests.check(t2 == null, "second attempt should fail");
            //tests.check(input.getPos() == 6, "position unchanged");
        });

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

        tests.test("StringTokenizer", () -> {
            TokenInput input = new TokenInput("  \"hello\\nworld\\\"escaped\\\\\"  ");
            TokenReader strReader = new StringTokenizer();
            Token t = strReader.tryGet(input);
            tests.check(t != null && t.getType() == TokenType.STRING, "string token");
            tests.check(t.getText().equals("hello\nworld\"escaped\\"), "unescaped content");
            tests.check(input.getPos() == 27, "position after string should be 27");
        });

        tests.test("IdentifierTokenizer", () -> {
            TokenInput input = new TokenInput("  _myVar123  while");
            TokenReader idReader = new IdentifierTokenizer();
            Token t1 = idReader.tryGet(input);
            tests.check(t1 != null && t1.getText().equals("_myVar123") && t1.getType() == TokenType.IDENTIFIER, "first identifier");
            tests.check(input.getPos() == 11, "position after _myVar123");
            Token t2 = idReader.tryGet(input);
            tests.check(t2 != null && t2.getText().equals("while") && t2.getType() == TokenType.IDENTIFIER, "second identifier");
        });

        tests.test("MoreParser", () -> {
            ParserContext ctx = new ParserContext();
            for (int i = 0; i <= 9; i++) {
                ctx.define("digit" + i, new KeywordTokenizer(String.valueOf(i)));
            }
            String[] digitNames = new String[10];
            for (int i = 0; i <= 9; i++) digitNames[i] = "digit" + i;
            ctx.define("digit", new SelectParser(ctx, digitNames));
            ctx.define("moreDigits", new MoreParser(ctx, "digit"));

            TokenReader moreDigits = ctx.get("moreDigits");
            TokenInput input = new TokenInput("  123abc");
            Token t1 = moreDigits.tryGet(input);
            tests.check(t1 != null && t1.getType() == TokenType.COMPOSITE && t1.getText().equals("123"), "more digits");
            tests.check(input.getPos() == 5, "position after digits");
            Token t2 = moreDigits.tryGet(input);
            tests.check(t2 != null && t2.getText().equals(""), "zero repetitions yields empty token");
            tests.check(input.getPos() == 5, "position unchanged");
        });

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
                    + "integer num;\n"
                    + "keyword plus +;\n"
                    + "linear sum expr plus expr;\n"
                    + "select expr num sum;\n";

            List<ParserDef> defs = GrammarParser.parse(grammar);
            tests.check(defs.size() == 15, "14 definitions parsed");

            ParserContext context = new ParserContext();
            GrammarParser.buildParsers(defs, context);

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

            TokenReader expr = context.get("expr");
            TokenInput recTest = new TokenInput("1+2+3");
            Token tRec = expr.tryGet(recTest);
            tests.check(tRec != null, "recursive expr should parse");
            tests.check(tRec.getText().length() > 0, "expr produced non-empty text");
        });

        // ---------- New Calculator Tests ----------
        calculatorTests(tests);

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

    // ---------- Calculator Grammar and Evaluator ----------
    private static void calculatorTests(TestSuite tests) {
        String calcGrammar = ""
                + "float number;\n"
                + "keyword op_add +;\n"
                + "keyword op_sub -;\n"
                + "keyword op_mul *;\n"
                + "keyword op_div /;\n"
                + "keyword lparen (;\n"
                + "keyword rparen );\n"
                + "empty empty;\n"
                + "select factor number lparen_expr;\n"
                + "linear lparen_expr lparen expr rparen;\n"
                + "linear term factor term_tail;\n"
                + "select term_tail empty term_tail_mul;\n"
                + "linear term_tail_mul op_mul factor term_tail;\n"
                + "linear expr term expr_tail;\n"
                + "select expr_tail empty expr_tail_add;\n"
                + "linear expr_tail_add op_add term expr_tail;\n"
                + "linear expr_tail_sub op_sub term expr_tail;\n";

        tests.test("Calculator grammar parsing and evaluation", () -> {
            List<ParserDef> defs = GrammarParser.parse(calcGrammar);
            // We expect 15 definitions (count manually: number, op_add, op_sub, op_mul, op_div, lparen, rparen, empty, factor, lparen_expr, term, term_tail, term_tail_mul, expr, expr_tail, expr_tail_add, expr_tail_sub) – let's check later.
            ParserContext ctx = new ParserContext();
            GrammarParser.buildParsers(defs, ctx);

            TokenReader exprParser = ctx.get("expr");

            String[] expressions = {
                    "3 + 5", "10 - 4", "6 * 7", "15 / 3",
                    "2 + 3 * 4", "(2 + 3) * 4", "2 * (3 + 4)",
                    "3.5 * 2", "1.2 + 2.3", "10 / 4", "2.5e-1 * 4",
                    " 1+2  ", "  (  3 + 4  )  * 5 "
            };
            double[] expected = {
                    8, 6, 42, 5,
                    14, 20, 14,
                    7.0, 3.5, 2.5, 1.0,
                    3, 35
            };

            for (int i = 0; i < expressions.length; i++) {
                TokenInput input = new TokenInput(expressions[i]);
                Token token = exprParser.tryGet(input);
                tests.check(token != null, "Parsing failed: " + expressions[i]);
                // Ensure full consumption
                while (!input.isEOF() && Character.isWhitespace(input.peek())) input.read();
                tests.check(input.isEOF(), "Leftover chars: " + expressions[i]);
                double result = CalculatorEvaluator.evaluate(token);
                tests.check(Math.abs(result - expected[i]) < 1e-9,
                        expressions[i] + " → " + result + " (expected " + expected[i] + ")");
            }
        });

        tests.test("Calculator division by zero", () -> {
            List<ParserDef> defs = GrammarParser.parse(calcGrammar);
            ParserContext ctx = new ParserContext();
            GrammarParser.buildParsers(defs, ctx);
            TokenReader exprParser = ctx.get("expr");
            TokenInput input = new TokenInput("1/0");
            Token token = exprParser.tryGet(input);
            tests.check(token != null, "Parsing 1/0 failed");
            try {
                CalculatorEvaluator.evaluate(token);
                tests.check(false, "Should throw ArithmeticException");
            } catch (ArithmeticException e) {
                // expected
            }
        });

        tests.test("Calculator syntax errors", () -> {
            List<ParserDef> defs = GrammarParser.parse(calcGrammar);
            ParserContext ctx = new ParserContext();
            GrammarParser.buildParsers(defs, ctx);
            TokenReader exprParser = ctx.get("expr");
            String[] badExprs = {"2+", "(2+3", "abc"};
            for (String bad : badExprs) {
                TokenInput input = new TokenInput(bad);
                Token token = exprParser.tryGet(input);
                tests.check(token == null, "Should not parse: " + bad);
            }
        });
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

// ---------- CompositeToken ----------
class CompositeToken implements Token {
    private final int pos;
    private final List<Token> children;
    private final String text;

    CompositeToken(int pos, List<Token> children) {
        this.pos = pos;
        this.children = children;
        StringBuilder sb = new StringBuilder();
        for (Token t : children) sb.append(t.getText());
        this.text = sb.toString();
    }

    @Override public int getPos() { return pos; }
    @Override public String getText() { return text; }
    @Override public TokenType getType() { return TokenType.COMPOSITE; }
    public List<Token> getChildren() { return children; }
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

// ---------- FloatTokenizer (with optional leading sign) ----------
class FloatTokenizer implements TokenReader {
    @Override
    public Token tryGet(TokenInput input) {
        int originalPos = input.getPos();
        while (!input.isEOF() && Character.isWhitespace(input.peek())) {
            input.read();
        }
        int startPos = input.getPos();

        // Optional leading sign
        boolean hasSign = false;
        if (!input.isEOF() && (input.peek() == '+' || input.peek() == '-')) {
            input.read(); // consume sign
            hasSign = true;
        }

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

        // Must have at least integer or fraction part
        if (!hasInteger && !hasFraction) {
            input.setPos(originalPos);
            return null;
        }

        // Exponent part
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

// ---------- SymbolTokenizer (for grammar arguments) ----------
class SymbolTokenizer implements TokenReader {
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
        if (first == ';') { // don't consume semicolon
            input.setPos(originalPos);
            return null;
        }
        StringBuilder sb = new StringBuilder();
        while (!input.isEOF()) {
            int ch = input.peek();
            if (ch == ';' || Character.isWhitespace(ch)) {
                break;
            }
            sb.append((char) input.read());
        }
        if (sb.length() == 0) {
            input.setPos(originalPos);
            return null;
        }
        return new Token() {
            private final int pos = startPos;
            private final String text = sb.toString();
            @Override public int getPos() { return pos; }
            @Override public String getText() { return text; }
            @Override public TokenType getType() { return TokenType.IDENTIFIER; } // reuse type
        };
    }
}

// ---------- LinearParser ----------
class LinearParser implements TokenReader {
    private final ParserContext context;
    private final String[] subParserNames;
    private final TokenReader[] subParsers;

    public LinearParser(TokenReader... subParsers) {
        this.context = null;
        this.subParserNames = null;
        this.subParsers = subParsers;
    }

    public LinearParser(ParserContext context, String... subParserNames) {
        this.context = context;
        this.subParserNames = subParserNames;
        this.subParsers = null;
    }

    @Override
    public Token tryGet(TokenInput input) {
        int originalPos = input.getPos();
        List<Token> tokens = new ArrayList<>();
        if (context != null) {
            for (String name : subParserNames) {
                TokenReader parser = context.get(name);
                Token t = parser.tryGet(input);
                if (t == null) {
                    input.setPos(originalPos);
                    return null;
                }
                tokens.add(t);
            }
        } else {
            for (TokenReader parser : subParsers) {
                Token t = parser.tryGet(input);
                if (t == null) {
                    input.setPos(originalPos);
                    return null;
                }
                tokens.add(t);
            }
        }
        int startPos = tokens.get(0).getPos();
        return new CompositeToken(startPos, tokens);
    }
}

// ---------- SelectParser ----------
class SelectParser implements TokenReader {
    private final ParserContext context;
    private final String[] subParserNames;
    private final TokenReader[] subParsers;

    public SelectParser(TokenReader... subParsers) {
        this.context = null;
        this.subParserNames = null;
        this.subParsers = subParsers;
    }

    public SelectParser(ParserContext context, String... subParserNames) {
        this.context = context;
        this.subParserNames = subParserNames;
        this.subParsers = null;
    }

    @Override
    public Token tryGet(TokenInput input) {
        int originalPos = input.getPos();
        if (context != null) {
            for (String name : subParserNames) {
                TokenReader parser = context.get(name);
                Token t = parser.tryGet(input);
                if (t != null) {
                    return t;
                }
                input.setPos(originalPos);
            }
        } else {
            for (TokenReader parser : subParsers) {
                Token t = parser.tryGet(input);
                if (t != null) {
                    return t;
                }
                input.setPos(originalPos);
            }
        }
        return null;
    }
}

// ---------- MoreParser ----------
class MoreParser implements TokenReader {
    private final ParserContext context;
    private final String subParserName;
    private final TokenReader subParser;

    public MoreParser(TokenReader subParser) {
        this.context = null;
        this.subParserName = null;
        this.subParser = subParser;
    }

    public MoreParser(ParserContext context, String subParserName) {
        this.context = context;
        this.subParserName = subParserName;
        this.subParser = null;
    }

    @Override
    public Token tryGet(TokenInput input) {
        int originalPos = input.getPos();
        List<Token> tokens = new ArrayList<>();
        while (true) {
            int beforeAttempt = input.getPos();
            Token t;
            if (context != null) {
                TokenReader parser = context.get(subParserName);
                t = parser.tryGet(input);
            } else {
                t = subParser.tryGet(input);
            }
            if (t == null) {
                input.setPos(beforeAttempt);
                break;
            }
            tokens.add(t);
        }
        if (tokens.isEmpty()) {
            // Return empty composite token
            return new CompositeToken(originalPos, Collections.emptyList());
        }
        int start = tokens.get(0).getPos();
        return new CompositeToken(start, tokens);
    }
}

// ---------- EmptyParser ----------
class EmptyParser implements TokenReader {
    @Override public Token tryGet(TokenInput input) {
        return new CompositeToken(input.getPos(), Collections.emptyList());
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

        TokenReader typeParser = new SelectParser(
                new KeywordTokenizer("integer"),
                new KeywordTokenizer("float"),
                new KeywordTokenizer("string"),
                new KeywordTokenizer("identifier"),
                new KeywordTokenizer("keyword"),
                new KeywordTokenizer("linear"),
                new KeywordTokenizer("select"),
                new KeywordTokenizer("more"),
                new KeywordTokenizer("empty")
        );
        TokenReader argTokenizer = new SymbolTokenizer(); // for arguments like "+"
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

            Token nameToken = argTokenizer.tryGet(input); // parser names are also symbols
            if (nameToken == null) {
                throw new RuntimeException("Expected parser name after type at position " + input.getPos());
            }
            String name = nameToken.getText();

            List<String> args = new ArrayList<>();
            while (true) {
                // Skip whitespace before argument
                while (!input.isEOF() && Character.isWhitespace(input.peek())) {
                    input.read();
                }
                if (input.isEOF() || input.peek() == ';') break;
                Token argToken = argTokenizer.tryGet(input);
                if (argToken == null) break; // shouldn't happen
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
                case "empty":
                    parser = new EmptyParser();
                    break;
                default:
                    throw new RuntimeException("Unknown parser type: " + def.type);
            }
            context.define(def.name, parser);
        }
    }
}

// ---------- Calculator Evaluator ----------
class CalculatorEvaluator {
    static double evaluate(Token token) {
        if (token.getType() == TokenType.FLOAT || token.getType() == TokenType.INTEGER) {
            return Double.parseDouble(token.getText());
        }
        if (token.getType() != TokenType.COMPOSITE) {
            throw new RuntimeException("Unexpected token: " + token.getText());
        }
        CompositeToken ct = (CompositeToken) token;
        List<Token> children = ct.getChildren();
        if (children.isEmpty()) return Double.NaN;

        // Determine production by looking at first child
        Token first = children.get(0);
        if (first.getType() == TokenType.KEYWORD) {
            String kw = first.getText();
            switch (kw) {
                case "+":
                    return evaluate(children.get(1)) + evaluate(children.get(2));
                case "-":
                    return evaluate(children.get(1)) - evaluate(children.get(2));
                case "*":
                    return evaluate(children.get(1)) * evaluate(children.get(2));
                case "/":
                    double right = evaluate(children.get(2));
                    if (right == 0) throw new ArithmeticException("division by zero");
                    return evaluate(children.get(1)) / right;
                case "(":
                    return evaluate(children.get(1)); // ( expr ) → child[1] is expr
                default:
                    throw new RuntimeException("Unknown operator: " + kw);
            }
        } else {
            // factor → number or lparen_expr
            if (children.size() == 1) {
                return evaluate(children.get(0));
            }
            // term: factor term_tail
            double val = evaluate(children.get(0));
            if (children.size() > 1) {
                val = applyTail(val, children.get(1));
            }
            return val;
        }
    }

    private static double applyTail(double left, Token tail) {
        if (tail.getType() != TokenType.COMPOSITE) return left;
        CompositeToken ct = (CompositeToken) tail;
        List<Token> children = ct.getChildren();
        if (children.isEmpty()) return left;
        Token first = children.get(0);
        if (first.getText().equals("*")) {
            double right = evaluate(children.get(1));
            double newLeft = left * right;
            if (children.size() > 2) {
                return applyTail(newLeft, children.get(2));
            }
            return newLeft;
        } else if (first.getText().equals("/")) {
            double right = evaluate(children.get(1));
            if (right == 0) throw new ArithmeticException("division by zero");
            double newLeft = left / right;
            if (children.size() > 2) {
                return applyTail(newLeft, children.get(2));
            }
            return newLeft;
        }
        return left;
    }
}