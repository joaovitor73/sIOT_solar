import serial
import matplotlib.pyplot as plt
from drawnow import *
from threading import Thread, Lock
from datetime import datetime

luminosidade = []
conexao = serial.Serial('com10', 115200)
plt.ion()
cont = 0

data_lock = Lock()

def makeFig():
    plt.ylim(0, 255)
    plt.title('Sensores')
    plt.grid(True)
    plt.ylabel('Luminosidade')
    plt.plot(luminosidade, 'ro-', label='Luminosidade')

def read_from_arduino():
    global cont
    fileName = f'luminosidade_{datetime.today().strftime("%Y-%m-%d_%H-%M-%S")}.txt'
    while True:
            try:
                if conexao.in_waiting > 0:
                    arduinoString = conexao.readline().decode('utf-8').strip()
                    dataArray = arduinoString.split(': ')
                    print(dataArray)
                    if 'connecting' not in arduinoString:
                        ldr = float(dataArray[0])
                        
                        with data_lock:
                            luminosidade.append(ldr)
                            # Salva os dados no arquivo
                            with open(fileName, "a") as file:
                                file.write(f"{ldr}\n")
                                file.close()
                            if len(luminosidade) > 50:
                                luminosidade.pop(0)
                            
            except Exception as e:
                print(f"Erro na leitura: {e}")

thread = Thread(target=read_from_arduino, daemon=True)
thread.start()

while True:
    with data_lock:
        drawnow(makeFig)