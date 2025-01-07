
from config import utelegram_config
from config import wifi_config

import uutelegram
import network
import utime

#debug = True

sta_if = network.WLAN(network.STA_IF)
sta_if.active(True)
sta_if.scan()
sta_if.connect(wifi_config['ssid'], wifi_config['password'])

print('WAITING FOR NETWORK - sleep 10')
utime.sleep(10)
if sta_if.isconnected():
    print(f'conectado a red con IP {sta_if.ipconfig("addr4")}')
    bot = utelegram.ubot(utelegram_config['token'])
    print('Ejecutando bot')
    print(bot.test())
else:
    print('No conectado a red')