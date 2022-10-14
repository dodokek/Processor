if (strncmp ("PUSH", line, PUSH_LEN) == 0)
{
    return ParseCmd (AsmInfo, line + PUSH_LEN + 1, PUSH);
}
else if (strncmp ("POP", line, POP_LEN) == 0)
{
    return ParseCmd (AsmInfo, line + POP_LEN + 1, POP);
}
else if (strncmp ("JMP", line, JMP_LEN) == 0)
{
    return ParseJmp (AsmInfo, line + JMP_LEN + 1, JMP);
}
else if (strncmp ("J", line, J_LEN) == 0)
{
    return IsJmp (AsmInfo, line);
}
else