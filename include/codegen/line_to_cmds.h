if (strncmp ("PUSH", line, PUSH_LEN) == 0)
{
    ParseCmd (AsmInfo, line + PUSH_LEN + 1, PUSH);
}
else if (strncmp ("POP", line, POP_LEN) == 0)
{
    ParseCmd (AsmInfo, line + POP_LEN + 1, POP);
}
else if (strncmp ("JMP", line, JMP_LEN) == 0)
{
    ParseJmp (AsmInfo, line + JMP_LEN + 1, JMP);
}
else if (strncmp ("J", line, J_LEN) == 0)
{
    IsJmp (AsmInfo, line);
}
else