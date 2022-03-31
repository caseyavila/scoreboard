import pygame
from pygame.locals import (
    K_SPACE,
    K_ESCAPE,
    K_RETURN,
    KEYDOWN,
    QUIT,
    MOUSEBUTTONDOWN
)
import time
import colorsys
import renderer
import frameio
import elements

def main():
    # initialize
    screen = renderer.init()
    # create leds
    leds = elements.generate_leds()
    # draw all leds
    renderer.draw_leds(screen, leds)
    # create buttons
    buttons = elements.generate_buttons()
    # draw all buttons
    renderer.draw_buttons(screen, buttons)

    drawing = False
    frames_created = 0
    loop(screen, leds, buttons, drawing, frames_created)

def loop(screen, leds, buttons, drawing, frames_created):
    led_width = elements.get_led_size()[0]
    led_height = elements.get_led_size()[1]
    x_min = y_min = 15 
    x_max = 64 * led_width + 15
    y_max = 32 * led_height + 15
    color = (255, 255, 255)

    running = True
    while running:
        # key events
        for event in pygame.event.get():
            if event.type == KEYDOWN:
                if event.key == K_ESCAPE:
                    running = False
                elif event.key == K_RETURN:
                    frameio.output(leds, frames_created)
                    frames_created += 1 
            elif event.type == QUIT:
                running = False
            # mouse button
            elif event.type == MOUSEBUTTONDOWN:
                pos = pygame.mouse.get_pos()
                # loop over all buttons
                for b in buttons:
                    # check if mouse over button
                    if b.x_min() < pos[0] < b.x_max() and b.y_min() < pos[1] < b.y_max():
                        # drawing
                        if b.name == "drawing":
                            if drawing:
                                b.set_color((200, 0, 0))
                            else:
                                b.set_color((0, 200, 0))
                            drawing = not drawing    
                            # new color
                            renderer.draw_surf(screen, b.surf, b.pos)
                        # clear button
                        elif b.name == "clear":
                            for led in leds.values():
                                led.Disable()
                            renderer.draw_leds(screen, leds)
                        # color button
                        elif b.name == "color":
                            color_hex = input("Color hex: ").lstrip("#")
                            color = tuple(int(color_hex[i:i+2], 16) for i in (0, 2, 4))    
                            b.set_color(color)
                            renderer.draw_surf(screen, b.surf, b.pos)
                            
        # mouse input
        if pygame.mouse.get_pressed()[0]:
            pos = pygame.mouse.get_pos()
            # check if mouse is out of bounds
            if x_min < pos[0] < x_max and y_min < pos[1] < y_max:
                pos_to_grid = ((pos[0] // led_width) * led_width, (pos[1] // led_height) * led_height)
                c_led = leds[pos_to_grid]
                if drawing:
                    c_led.Enable(color)
                else:
                    c_led.Disable()

                # redraw
                renderer.draw_surf(screen, c_led.surf, c_led.pos)


if __name__ == "__main__":
    main()

