# write to frame file
def output(leds, frame_number):
    fout = open("output/frame" + str(frame_number), "w")
    idx = 0
    for led in leds.values():
        if led.enabled:
            x = idx % 64
            y = idx // 64
            r = led.enabled_color[0]
            g = led.enabled_color[1]
            b = led.enabled_color[2]
            fout.write(str(x) + " " + str(y) + " " + str(r) + " " + str(g) + " " + str(b) + "\n")
        idx += 1
    fout.close()
    
