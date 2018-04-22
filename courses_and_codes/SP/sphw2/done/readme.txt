1. Execution
	$ make
	$ bidding_system [host_num] [player_num]

2. Description

	Using FIFOs between bidding_system and host, instead of pipes.

	Bonus strategy:
		bid = (money I have) * (3/4) + rand()%10
		bid = (all money I have) 		//in the final round
	Explanation:
		According to the theory I learned in economics, given x people in an auction, 
		bidding at (personal evaluation) * ((x-1)/x) is the optimal strategy.
		(If one keeps bidding at all of his money, he won't get extra value for the next round.)
		Adding rand()%10 is to avoid failure when encountering same strategy.

3. Self-Examination
	All tasks are finished, so I should gain 7 points.
