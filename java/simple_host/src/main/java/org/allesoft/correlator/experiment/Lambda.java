package org.allesoft.correlator.experiment;

public class Lambda {
    class Element {
        String name;
    }

    class InputSeq {
        String str;
        int pos;
    }

    public boolean parseLiteral(InputSeq input) {
        int letterStart = -1;
        for (int i = input.pos; i < input.str.length(); i ++) {
            if (input.str.charAt(i) == ' ' && letterStart == -1) {
                input.pos ++;
                continue;
            }
            if (!Character.isLetter(input.str.charAt(i)) && letterStart != -1) {
                return true;
            }
            if (Character.isLetter(input.str.charAt(i)) && letterStart == -1) {
                letterStart = input.pos;
                continue;
            }
            if (Character.isLetter(input.str.charAt(i))) {
                continue;
            }
            return false;
        }
        return letterStart != -1;
    }

    public boolean parseLambda(InputSeq input) {
        return false;
    }

    public boolean parseApplication(InputSeq input) {
        return false;
    }

    public void parse(InputSeq input) {
        if (!parseLiteral(input)) {
            if (!parseLambda(input)) {
                if (!parseApplication(input)) {
                    throw new RuntimeException("syntax error");
                }
            }
        }
    }

    public static void main(String[] args) {
        String program = "";
    }
}
