from lora.crypto import loramac_decrypt

payload = 'EB89F757EF113EFC6586A7BCD40815C9'
sequence_counter = 2575
key = '2B7E151628AED2A6ABF7158809CF4F3C'
dev_addr = '01202093'
print(loramac_decrypt(payload, sequence_counter, key, dev_addr))
