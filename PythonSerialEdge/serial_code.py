# imports
import serial
import mysql.connector

# reading mysql connection settings from file
mysql_conf = open("./mysql_account_info.text", "r")
lines = mysql_conf.readlines()
mysql_host = lines[0]
mysql_user = lines[1]
mysql_passwd = lines[2]
mysql_db = lines[3]

# establish a connection the database
mydb = mysql.connector.connect(
  host = mysql_host,
  user = mysql_user,
  password = mysql_passwd,
  database = mysql_db
)
mycursor = mydb.cursor()

# serial stuff - not working rn
ser = serial.Serial('/dev/ttyACM0',9600)
while True:
	read_serial=ser.readline()
	print(read_serial)

# sql query
sql = "INSERT INTO tbl_name (name, address) VALUES (%s, %s)"
# sql values to fill
val = ("Alex", "My house address")
# execute query and commit changes to the database
mycursor.execute(sql, val)
mydb.commit()
# print the console for confirmation
print(mycursor.rowcount, "record inserted.")