import pygame

def get_screen_size():
    return 1200, 510

# returns screen
def init():
    pygame.init()
    pygame.display.set_caption("scoreboard")
    screen = pygame.display.set_mode(get_screen_size())
    screen.fill((0, 0, 0))
    return screen

def draw_surf(screen, surf, pos):
    screen.blit(surf, pos)
    pygame.display.flip()

def draw_leds(screen, leds):
    for led in leds.values():
        screen.blit(led.surf, led.pos)
    pygame.display.flip()

def draw_buttons(screen, buttons):
    for button in buttons:
        screen.blit(button.surf, button.pos)
    pygame.display.flip()

