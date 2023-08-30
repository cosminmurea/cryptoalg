<div style="text-align: justify">

# Data Encryption Standard - DES

The Data Encryption Standard - DES is a block cipher designed to process (encipher and decipher) data in blocks of 64 bits and return processed data blocks of the same size. It uses a 64-bit key, however, only 56 bits are used (resulting in a key space of 2^56). 

# Structure of DES

<ol style="list-style: none">
    <!-- Stage 1 -->
    <li>
        <u style="font-size: 1.5em">1. Generating the 16 Subkeys</u>
        <p>The DES algorithm uses a 64-bit key however, only 56 bits are actually used. This happens because every 8th bit of the key is used for assuring that every byte is of odd parity. From this initial key K we must generate 16 subkeys K<sub>n</sub> where 1 <span>&#8804;</span> n <span>&#8804;</span> 16.</p>
        <ol style="list-style: none">
            <!-- Stage 1 Step 1 -->
            <li>
                <u style="font-size: 1.17em">1.1. Permuted Choice 1</u>
                <p>The initial key K is first permuted according to the following table.</p>
                <img src="images/permutation_choice1.png" alt="Permutation Choice 1" width="400" height="200"/><br>
                <p>The permutation works as follows: the first bit of the permuted key PK is the 57th bit of the initial key K, the second bit of PK is the 49th bit of K and eventually the 56th (last) bit of PK is the 4th bit of K. Notice how the initial key K was 64 bits long but the permuted key is only 56 bits long? This is due to the fact that PC-1 ignores every 8th bit of K (the parity bit). The permuted key PK is then split up into two equal 28-bit halves referred to as C<sub>0</sub> and D<sub>0</sub>.</p>
            </li>
            <!-- Stage 1 Step 2 -->
            <li>
                <u style="font-size: 1.17em">1.2. The Key Schedule</u>
                <p>The two halves of the permuted key are used to generate another 16 pairs of halves. Each pair (C<sub>n</sub>, D<sub>n</sub>) is obtained from the previous pair (C<sub>n-1</sub>, D<sub>n-1</sub>) by applying a specific number of left shifts. This number is defined in the key schedule.</p>
                <img src="images/key_schedule.png" alt="Key Schedule" width="400" height="400"/><br>
                <p>The pair (C<sub>1</sub>, D<sub>1</sub>) is generated from the previous pair (C<sub>0</sub>, D<sub>0</sub>) such that (C<sub>1</sub> = C<sub>0</sub> << 1, D<sub>1</sub> = D<sub>0</sub> << 1). The pair (C<sub>4</sub>, D<sub>4</sub>) is obtained from the pair (C<sub>3</sub>, D<sub>3</sub>) by applying 2 left shifts (C<sub>4</sub> = C<sub>3</sub> << 2, D<sub>4</sub> = D<sub>3</sub> << 2). This way, we can generate all pairs if we know the key schedule and the permuted key PK.</p>
            </li>
            <!-- Stage 1 Step 3 -->
            <li>
                <u style="font-size: 1.17em">1.3. Permuted Choice 2</u>
                <p>Each pair of halves is concatenated to obtain 16 56-bit keys C<sub>n</sub>D<sub>n</sub>. These keys are all permuted according to the following table.</p>
                <img src="images/permutation_choice2.png" alt="Permutation Choice 2" width="400" height="200"/><br>
                <p>This permutation has the following effect: the first bit of K<sub>n</sub> is the 14th bit of the pair C<sub>n</sub>D<sub>n</sub>, the second bit of K<sub>n</sub> is the 17th bit of the pair C<sub>n</sub>D<sub>n</sub> and eventually the 48th (last) bit of K<sub>n</sub> is the 32nd bit of the pair C<sub>n</sub>D<sub>n</sub>. Notice how the initial concatenated pairs are 56 bits long but the permuted keys are only 48 bits long? PC-2 ignores another 8 bits when generating the 16 subkeys. This step concludes the first stage of the DES algorithm, the generation of subkeys.</p>
            </li>
        </ol>
    </li>
    <li>
        <u style="font-size: 1.5em">2. Encoding the Data Blocks</u>
    </li>
</ol>

</div>