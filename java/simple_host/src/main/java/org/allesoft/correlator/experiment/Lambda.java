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
        return false;
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
