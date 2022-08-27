# speck_cipher

## Resources: ##
https://en.wikipedia.org/wiki/Speck_(cipher)

Speck is a family of lightweight block ciphers publicly released by the National Security Agency. I based my code off of source code found on wikipedia. From the information given I was able to create a decryption function to read encrypted messages. 

## How to use: ##
First run make to create an executable named speck. The program takes one input argument which is the message you wish to encrypt and decrypt. For example to run the the executable type the following command ```./speck "message"``` where message is the desired plain text you wish to encrypt.

## Future Goals: ##
<br >-Add this cipher algorithm to the jpeg_secret_message repo to allow users to encode secret messages using the speck cipher.<br />
<br >-Implement the algorithm in a generic fashion so that blocksize and message size can be dynamically changed. <br />

![image](https://user-images.githubusercontent.com/30327564/187025531-f2747c19-eb6d-49b2-bfa5-5d7c318894d7.png)
