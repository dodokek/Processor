DEF_LINE("PUSH", PUSH_LEN, 
    ParseCmd (AsmInfo, line + PUSH_LEN + 1, PUSH)
)

DEF_LINE("POP", POP_LEN, 
    ParseCmd (AsmInfo, line + POP_LEN + 1, POP)
)

DEF_LINE("JMP", JMP_LEN, 
    ParseCmd (AsmInfo, line + JMP_LEN + 1, JMP)
)

DEF_LINE("J", J_LEN, 
    IsJmp (AsmInfo, line)
)