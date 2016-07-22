        setlow 0
        copy rs r0
        setlow 1
        add rs r0
        copy rs r0
        setlow 3
        compare rs r0
        copy rs r1
        setlow 1
        sub rs r1
        copy rs r1
        setlow 4294967295
        sethigh 4294967295
        copy rs r2
        sethigh 0
        setlow 1
        copy rs sn
        mul r1 r2
        copy rs r1
        setlow 16
        copy rs me
        setlow 0
        copy rs sn
        branch r1
        halt
