
from config import utelegram_config
from config import wifi_config

import utelegram
import network

debug = True

sta_if = network.WLAN(network.STA_IF)
if not sta_if.isconnected():
    sta_if.active(True)
    sta_if.scan()
    sta_if.connect(wifi_config['ssid'], wifi_config['password'])
    print('WAITING FOR NETWORK')
    while not sta_if.isconnected():
        pass
    if sta_if.isconnected():
        print(f'conectado a red con IP {sta_if.ipconfig("addr4")}')
        bot = utelegram.ubot(utelegram_config['token'])
        
        bot.saluda(utelegram_config['chat_id_default'])
        print('Bot Listening')
        bot.listen()
    else:
        print('No conectado a red') 