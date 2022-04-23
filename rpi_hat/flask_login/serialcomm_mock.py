

class SerialComm:
    port_open = False

    def __init__(self, callback, port, velocidad=9600):
        print ("Opening Port: {} speed: {}".format(port, velocidad))
        self.port_open = True

    def Write(self, data):
        print ("send -> " + str(data))

    def Read(self):
        return "mocked test data"

    def Close(self):
        if self.port_open == True:
            print ("Closing Port")
            self.port_open = False
        else:
            print ("Port was already closed")



