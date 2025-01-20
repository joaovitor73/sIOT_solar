import serial
import numpy
import matplotlib.pyplot as plt
from drawnow import *
luminosidade = []
conexao = serial.Serial('com3', 115200)
plt.ion()
cont = 0
def makeFig(): 
    plt.ylim(0,255) 
    plt.title('Sensores')
    plt.grid(True)
    plt.ylabel('Luminosidade')
    plt.plot(luminosidade,'ro-', label='Luminosidade')
       
while True:
    while (conexao.inWaiting()==0):
        pass 
    arduinoString = conexao.readline().decode('utf-8')

    dataArray = arduinoString.split(' , ')

    ldr = float(dataArray[0]) 
    luminosidade.append(ldr)
    drawnow(makeFig)
    cont = cont +1

    if(cont > 50):
        luminosidade.pop(0)

