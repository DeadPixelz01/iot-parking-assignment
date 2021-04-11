# imports
import serial
import mysql.connector
from Carspot import Carspot
from datetime import datetime
import os

# reading mysql connection settings from file
mysql_conf = open("./mysql_account_info.txt", "r")
lines = mysql_conf.readlines()
mysql_host = lines[0]
mysql_user = lines[1]
mysql_passwd = lines[2].rstrip("\n")
mysql_db = lines[3]

# establish a connection the database
db = mysql.connector.connect(
  host = mysql_host,
  user = mysql_user,
  password = mysql_passwd,
  database = mysql_db
)
mycursor = db.cursor()

# serial stuff - have no idea if this stuff works just yet! complete proof of concept
ser = serial.Serial('/dev/ttyACM0',9600)
ser.flushInput()
while True:
  # read the serial and split the string into the correct attributes
  read_serial = ser.readline()
  split_ser = read_serial.split(",")

  # assign values
  status = split_ser[0]
  distance = int(split_ser[1])
  correctness = split_ser[2]
  now = datetime.now()
  time = now.strftime("%H:%M:%S")

  # convert strings to boolean values
  if status == "Taken":
    status = True
    ser.write("There is a car parked here!".encode("utf-8"))
  else:
    status == False

  if correctness == "Correct":
    correctness = True
  else:
    correctness = False
    ser.write("The car is parked incorrectly, please straighten up!".encode("utf-8"))

  # create a new carspot obj
  park = Carspot("Top Level", 1, status, distance, correctness, time)

# sql query
sql_insert = "INSERT INTO tbl_parking_lot (floor, bay_number, status, distance, correctness, time) VALUES (%s, %d, %r, %d, %r, %s)"
# sql values to fill
val = (park.floor, park.bay_number, park.status, park.distance, park.correctness, park.time)
# execute query and commit changes to the database
mycursor.execute(sql_insert, val)
db.commit()
# print the console for confirmation
print(mycursor.rowcount, "record inserted.")
