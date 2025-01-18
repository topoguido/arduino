
from config import utelegram_config
#from config import wifi_config

import utelegram
import time
import gc
import hardware


bot = utelegram.ubot(utelegram_config['token'])
bot.saluda(utelegram_config['chat_id_default'])
sensor = hardware.sensor()

if debug: print('Bot Listening')

while True:
    #bot.listen()
    if bot.read_once():
        # Analiza el comando recibido
        if bot.command == '/ping':
            bot.reply_ping(bot.chat_id)
        elif bot.command == '/temp':
            # Obtiene los valores de temperatura y humedad requeridos para responder
            sensor.update_values()
            time.sleep(1)
            bot.send(bot.chat_id, f'Temperatura: {sensor.get_temp()} - Humedad: {sensor.get_hum()}')

    else: 
        time.sleep(bot.sleep_btw_updates)
        gc.collect()
