DEF_CMD(PUSH, MULTI_BYTE_OFFSET,
{
    fprintf (DisasFile, "%s %lg %d\n", "PUSH", *(elem_t*)(CpuInfo.cmds + ip + BYTE_OFFSET), *(CpuInfo.cmds + ip + sizeof(elem_t) + BYTE_OFFSET));
})

