#import ujson
import urequests
import gc


class ubot:
    def __init__(self, token, offset=0):
        self.url = 'https://api.telegram.org/bot' + token + '/getMe'
        print(self.url)
        self.commands = {}
        self.default_handler = None
        self.message_offset = offset
        self.sleep_btw_updates = 3

    def test(self):
        headers = {'Content-type': 'application/json', 'Accept': 'text/plain'}
        gc.collect()
        result = []
        try:
            response = urequests.post(url=self.url, headers=headers)
            try:
                update_messages = response.json()
            except ValueError:
                print("Error: La respuesta no es un JSON válido")
                update_messages = {}
            
            response.close()
            if 'result' in update_messages:
                for item in update_messages['result']:
                    result.append(item)
            response.close()
            return result
        except OSError as e:
            print(f'Error de red (OSError): {e}')
        except Exception as e:
            print(f'Error inesperado: {e}')
        finally:
            if 'response' in locals() and response:  # Asegura cerrar la respuesta si existe
                response.close()
        return result