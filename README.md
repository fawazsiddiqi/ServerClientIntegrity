# ServerClientIntegrity
A demonstration of SHA256 on a Server and Client to compare hashes and check the authenticity and integrity of a file.

# SHA256
SHA256 is one of the successors to hash functions SHA1, SHA is known as a cryptographic hash also known as a ‘digest’ and it is a signature for a text or data file to verify its authenticity with the SHA256 hash generated on the server side and on the client side. 

![alt text](https://www.danielefavi.com/wp-content/uploads/2017/12/blockchain_to_calculate_2.png)

It is not an encryption but it is a hash so it is a one-way function, and in our scenario, we are transmitting a video file (download) from a server and the hash is generated within the server and as the client is downloading it, the hash generated on the client side is compared to the server side, and thus authenticity and integrity of the file is assured as the hashes from the server and the client are same when compared to each other.

The server starts from the last block of the file and its cash and thus the computed hash will be attached to the next block and the combination will be hashed, this process keeps repeating as it reaches the final block which is considered to be the first block of the file, the hash of that block plus the previous block results in the creation of H0

# Reading and Preparing the file on Server Side
At this point we are going to read the file and store the blocks into a vector so that its easier to traverse the blocks.
 
We are going to open the file and then we have a while loop with a condition that it will keep running as there are more characters we would append a character from the file to a string and then if the size is equal to 2048 we are going to add the string to the vector and then clear the string and then in the end if we have more characters in the string we would create a block with the contents and then pad the block.
 
To prepare the hash file we would use two vectors one to hold the bitset size of 1024*8 and the other to hold the already computed hashes. and then we will create a new file to store the hashes. We will start with traversing the vector from the end and if the current block is the last block then we are going to compute the hash of the current block and then push it into the vector where we are storing hashes. Else we would computer the hash of the current block and the previous block and then push it to the vector of the hashes (used in the case of HASH 0). And then we are going to flip the elements in the hash vector and then store them in a hash file
 
# Receiving the blocks on the Server or Client Side 
 
We are going to use a pair vector in this case vector<pair<string, bitset<256>>> hashStore, first value hold the hash and the second value stores the binary representation of the hash.
 
In this process we reverse the process of creating the file in the server side in order to check the authenticity of the blocks received. The server computes the number of blocks, sends the number of blocks to the client, and then get the value HASH 0 from the file and compute its binary value and upload the binary value to the buffer, while the number of blocks are received by the client is less than the number of blocks received earlier, if the current block is not the last block the server gets the hash value, binary value and upload the file block and the computed binary value  and on the client side it received data sent by the server, computes the hash of the file block and hash, and if the compared value is equal to the previous hash value it accepts or rejects the block accordingly. Else if the current block is the last block the server will upload the file block to buffer, and then the client will computer the hash of the block and if the compared value is equal to the previous hash value it accepts or rejects the block accordingly.
