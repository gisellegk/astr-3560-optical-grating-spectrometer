import serial, time
from tkinter import *
import threading

# open serial port
ser = serial.Serial()
ser.port = 'COM8'
ser.baudrate = 9600
ser.open()

line = ser.readline()
print(line)

time.sleep(1)
# ser.write(str.encode("600"))

def getData():
  line = ser.readline()
  print(line[:-2].decode('utf-8'))

#def send():
#  ser.write(str.encode(ENTRY1.get()))

class App(threading.Thread):
  def __init__(self):
    threading.Thread.__init__(self)
    self.start()

  def callback(self):
    self.root.quit()

  def send(self):
    ser.write(str.encode(self.ENTRY1.get()))
  
  def run(self):
    self.root = Tk()
    #self.root.bind("<Return>", send)
    self.root.geometry("400x400")
    self.root.title("F-EOS Controller")
    self.root.resizable(width=False, height=False)
    self.ENTRY1 = Entry(self.root,bd=8, width=10)
    self.ENTRY1.place(x=100, y=100)
    BUTTON = Button(bg="#000000", fg="#ffffff", bd=12, text="SEND", padx=33,pady=10, command=self.send)
    BUTTON.grid(row=4, column=0, sticky=W)
    BUTTON.place(x=100, y=150)
    #TOP.after(1,getData)
    self.root.mainloop()
  
app = App()

while(True):
  getData()