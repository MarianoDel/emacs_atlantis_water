import serial
import threading
import time


def ReadBytes(self, cb):
    t = threading.currentThread()

    while getattr(t, "do_run", True):
        datar = self.ser.readline()
        ## timeout or readed bytes
        if datar != b'':
            try:
                print(datar)
                cb((datar).decode())
            except:
                pass        


class SerialComm:
    """
    Clase para manejo de puerto serie

    recibe:
    port: nombre o ruta de dispositivo, ej /dev/ttyACM0
    velocidad: ej 9600
    callback: funcion a la que enviar los datos recibidos

    Variables miembro:
    port_open, True o False
    """

    port_open = False

    def __init__(self, callback, port, velocidad=9600):
        """
        Abrir puerto seleccionado
        """
        # print ("piden " + port)
        # print (velocidad)
        # callback()
        try:
            self.ser = serial.Serial(port, velocidad, timeout=0.5)
            if (self.ser != None):
                print ("Port Open\n")
                self.port_open = True

            #comienzo el thread de lectura
            self.hilo1 = threading.Thread(target=ReadBytes, args=(self, callback))
            self.hilo1.start()
        except:
            print ("Port Not Open\n")

    def Write(self, data):
        self.ser.write(data.encode('utf-8'))

    def Read(self):
        bytes_to_read = self.ser.inWaiting()
        if (bytes_to_read > 0):
            datar = self.ser.read(bytes_to_read)
            return datar

    def Close(self):
        if (self.port_open == True):
            print ("stop read thread")
            self.hilo1.do_run = False
            self.hilo1.join()
            
            print ("close serial port")
            self.port_open = False
            self.ser.close()



