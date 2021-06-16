module top(
    output LED1, LED2,
    input btn1, btn2,
);



    assign LED1 = btn1;
    assign LED2 = btn2;

endmodule
