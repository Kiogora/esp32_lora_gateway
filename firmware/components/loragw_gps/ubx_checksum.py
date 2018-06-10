#Dont include UBX header or UBX checksum
#Calculate Fletcher-16 checksum https://en.wikipedia.org/wiki/Fletcher%27s_checksum
#(Fletcher,1982)
packet = [0x06, 0x01, 0x08, 0x00, 0xf0, 0x04, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00]

CK_A,CK_B = 0, 0
for i in range(len(packet)):
  CK_A = CK_A + packet[i]
  CK_B = CK_B + CK_A

# ensure unsigned byte range
CK_A = CK_A & 0xFF
CK_B = CK_B & 0xFF

print "UBX packet checksum:", ("0x%02X,0x%02X" % (CK_A,CK_B))
