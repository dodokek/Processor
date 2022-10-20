DEF_LINE("PUSH", PUSH_LEN, 
    HandlePushPop (AsmInfo, line + PUSH_LEN + 1, PUSH)
)

DEF_LINE("POP", POP_LEN, 
    HandlePushPop (AsmInfo, line + POP_LEN + 1, POP)
)

DEF_LINE("JMP", JMP_LEN, 
    ParseJmp (AsmInfo, line + JMP_LEN, JMP)
)

DEF_LINE("J", J_LEN, 
    IsJmp (AsmInfo, line)
)

DEF_LINE("CALL", 4, 
    IsJmp (AsmInfo, line)
)