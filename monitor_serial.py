import serial
import matplotlib.pyplot as plt
from drawnow import *
from threading import Thread, Lock
from datetime import datetime
import sys

luminosidade = []
conexao = serial.Serial('com10', 115200)
plt.ion()
cont = 0
data_lock = Lock()
thread_running = True 

def makeFig():
    plt.ylim(0, 255)
    plt.title('Sensores')
    plt.grid(True)
    plt.ylabel('Luminosidade')
    plt.plot(luminosidade, 'ro-', label='Luminosidade')

def read_from_arduino():
    global cont, thread_running
    fileName = f'luminosidade_{datetime.today().strftime("%Y-%m-%d_%H-%M-%S")}.txt'
    while thread_running:
        try:
            if conexao.in_waiting > 0:
                arduinoString = conexao.readline().decode('utf-8')
                print(arduinoString)
                ldr = float(arduinoString)
                
                with data_lock:
                    luminosidade.append(ldr)
                
                with open(fileName, "a") as file:
                    file.write(f"{ldr}\n")
                
                with data_lock:
                    if len(luminosidade) > 50:
                        luminosidade.pop(0)
        except Exception as e:
            print(f"Erro na leitura: {e}")

def on_close(event):
    global thread_running
    print("Encerrando...")
    thread_running = False 
    thread.join()
    conexao.close()
    plt.close('all')
    sys.exit()

thread = Thread(target=read_from_arduino, daemon=True)
thread.start()

plt.gcf().canvas.mpl_connect("close_event", on_close)

while True:
    drawnow(makeFig)
