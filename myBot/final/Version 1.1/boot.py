from config import wifi_config
import network

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