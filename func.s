        setlow .end
        sethigh #end
        copy rs me
        jump
func:
        setlow 1
        add rs r0
        copy rs r0
        copy ra me
        jump
end:
        setlow .func
        sethigh #func
        copy rs me
        setlow 24
        add pc rs
        copy rs ra
        jump
        halt
