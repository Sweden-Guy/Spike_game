# Spike_game
This is a small portable game console that you can take with you wherever you go. It easily fits in your pocket or bag.

The foundation of the game was created by ultramegabombasticfuze, but I have fixed bugs and other issues. 

 All the pictures and files you need can be found further down. I hope everyone likes the new version I have made, and if you do please show me some respect by giving me a star so that more people can find this project. =) 

 In the STL file, I have two of each part. One with holes and one without, so in case you want to fix your own without it already having holes, there is one without holes. 

 # How to Set Up (Be careful to connect everything correctly so that you don't accidentally break anything!)

## Arduino UNO R3 Wiring

### OLED to Arduino UNO R3
| OLED Pin | Arduino UNO Pin |
|----------|-----------------|
| GND      | GND             |
| VCC      | 3.3V            |
| SCL      | A5              |
| SDA      | A4              |

### Button to Arduino UNO R3
| Button Pin | Arduino UNO Pin |
|------------|-----------------|
| One side   | Digital 3       |
| Other side | GND             |

### Buzzer to Arduino UNO R3
| Buzzer Pin | Arduino UNO Pin |
|------------|-----------------|
| One side   | Digital 6       |
| Other side | GND             |

---

## ESP32 C3 Mini Wiring

### OLED to ESP32 C3 Mini
| OLED Pin | ESP32 C3 Mini Pin |
|----------|-------------------|
| GND      | GND               |
| VCC      | 3.3V              |
| SCL      | 9                 |
| SDA      | 8                 |

### Button to ESP32 C3 Mini
| Button Pin | ESP32 C3 Mini Pin |
|------------|-------------------|
| One side   | Digital 3         |
| Other side | GND               |

### Buzzer to ESP32 C3 Mini
| Buzzer Pin | ESP32 C3 Mini Pin |
|------------|-------------------|
| One side   | Digital 1         |
| Other side | GND               |


 ### Things I have fixed: 

- Game speed 

- Sprites

- Collision 

- High score 

- See score after you have died 

- Works on an Esp32 C3 Mini

- Smoother visuals 

- Better spike placement 

- Delay so you can see your score in case you accidentally skip it
  
- Sweep Effect

Link to ultramegabombasticfuze:


Arduino: https://projecthub.arduino.cc/ultramegabombasticfuze 

GitHub: https://github.com/Coool-one

