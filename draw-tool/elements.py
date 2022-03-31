import pygame

# leds
class Led:
    def __init__(self, pos: (int, int)):
        self.pos = pos
        # create surf
        surf = pygame.Surface((10, 10))
        surf.fill((100, 100, 100))
        self.surf = surf
        self.enabled = False
        self.enabled_color = (255, 255, 255)

    def Enable(self, color):
        self.surf.fill(color)
        self.enabled_color = color
        self.enabled = True

    def Disable(self):
        self.surf.fill((100, 100, 100))
        self.enabled = False


def get_led_size():
    return 15, 15

# creates dictionary of leds
def generate_leds():
    leds = {}
    for j in range(32):
        for i in range(64):
            new_led = Led((i * get_led_size()[0] + 15, j * get_led_size()[1] + 15))
            leds[new_led.pos] = new_led
    return leds

# other
class Button:
    def __init__(self, name, size, pos, color):
        self.name = name
        self.size = size
        self.pos = pos
        self.color = color
        self.surf = pygame.Surface(size)
        self.surf.fill(color)

    def set_color(self, color):
        self.surf.fill(color)

    def x_min(self):
        return self.pos[0]
    
    def y_min(self):
        return self.pos[1]

    def x_max(self):
        return self.pos[0] + self.size[0]

    def y_max(self):
        return self.pos[1] + self.size[1]

def generate_buttons():
    buttons = []
    drawing_button = Button("drawing", (80, 40), (1000, 15), (200, 0, 0))
    clear_button = Button("clear", (40, 40), (1100, 15), (255, 127, 80))
    color_button = Button("color", (80, 40), (1000, 80), (255, 255, 255))
    
    buttons.append(drawing_button)
    buttons.append(clear_button)
    buttons.append(color_button)
    return buttons
    

