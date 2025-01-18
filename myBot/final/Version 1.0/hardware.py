import machine 
import dht

class sensor:
    
    def __init__(self):
        self.sensor_temp = dht.DHT11(machine.Pin(0, machine.Pin.IN))
    
    def update_values(self):
        self.sensor_temp.measure()
    
    def read_temp(self):
        return self.sensor_temp.temperature()
    
    def read_hum(self):
        return self.sensor_temp.humidity()