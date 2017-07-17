# Copyright (C) 2008-2015, Marvell International Ltd.
# All Rights Reserved.

exec-y += freertos_blinkled
freertos_blinkled-objs-y :=  src/main.c
# Applications could also define custom board files if required using following:
#freertos_blinkled-board-y := /path/to/boardfile
