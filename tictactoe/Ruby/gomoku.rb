class Gomoku
	def initialize	# A constructor for initializing all the instance variables.
		@board = Array.new(15) {Array.new(15,'.')}	# @board A two dimensional array of size 15x15 representing the game board.
		@playerList = [@player1, @player2]			# @player1, @player2 Player O and Player X respectively.
		@turn = 1									# @turn The player in the current turn. The first turn is Player O
		@win = 0
	end
	def startGame	# Starts a new Gomoku game and play until the end of game. The flow of this method is as follows:
		orders = ['First', 'Second']
		symbols = ['O', 'X']
		player = {'1' => 'Computer', '2' => 'Human'}
		2.times do |i|    # 1.Prompt the user to choose the player type
			print "#{orders[i]} player is (1) Computer or (2) Human? "
			type = gets.chomp
			@playerList[i] = type == '1' ? Computer.new(symbols[i]) : Human.new(symbols[i])
			puts "Player #{@playerList[i].getSymbol} is #{player[type]}."
		end
		count = 0
		loop do	# 6. Repeat until finished.
			count += 1 
			printBoard
			move = @playerList[@turn-1].nextMove(self)		# 3. Obtain a position 
			@board[move[0]][move[1]] = symbols[@turn-1]		# 4. Update the board
			puts "Player #{symbols[@turn-1]} places to row #{move[0]}, col #{move[1]}"
			@win = checkWin	# 5. Determine if the current player has connected five or above.	
			break if @win == 1 || @win == 2 || count == 15*15		
			@turn =@turn%2+1
		end
		printResult	# 7. Once the game is finished, display the message “Player O wins!”, “Player X wins!”, or “Draw game!” accordingly.
	end
	def printBoard	# Prints out the game board in the console.
		print "  "
		15.times do |i|
			if i>9 then
				print " #{i/10}"
			else
				print "  "
			end
		end	   
		print "\n  "
		15.times do |i|
			print " #{i%10}"
		end
		puts
		15.times do |i|
			printf( "%2d",  "#{i}")
			15.times do |j|
				print " #{@board[i][j]}"
			end
			puts
		end 
	end	
	def checkMove(moveArr)
		2.times do |i|
			if moveArr[i]<0|| moveArr[i]>14  then
				return 0
			end
		end
		if @board[moveArr[0]][moveArr[1]]!= '.' then
			return 0
		end
		return 1
	end
	def checkWin(sym = '.')	# loop for all possible "connect Five", assuming the checking block as sym if it is '.'
		symlist = ['.', 'O', 'X']
		count = [0,0,0,0] # counting for "'.', 'O' and 'X'" and totalcount respectively
		temresult = [-1,-1]
		15.times do |i| # check horizontal
			15.times do |j|
				count[3]+=1
				3.times do |k|
					if @board[i][j]==symlist[k] then
						count[k]+=1
					end					
				end	
				if @board[i][j]=='.' then
					temresult = [i, j]
				end
				if count[3]>=5 then
			#	puts "-#{i} #{count[0]} #{count[1]} #{count[2]} #{count[3]}"
					for k in (1..2) do	
						if count[k] == 5  then
							return k #return winner
						end		
						if sym == symlist[k] && count[k] == 4 && count[0] == 1 then # pattern 4 disc and one blank
							return temresult #return winning move
						end				
					end	
					3.times do |k|
						if @board[i][j-4]==symlist[k] then
							count[k]-=1
						end					
					end	
				end		
			end
			count = [0,0,0,0] # End of line, recount
		end	
		15.times do |j|# check vertical
			15.times do |i| 
				count[3]+=1
				3.times do |k|
					if @board[i][j]==symlist[k] then
						count[k]+=1
					end					
				end	
				if @board[i][j]=='.' then
					temresult = [i, j]
				end
				if count[3]>=5 then
			#	puts "|#{j} #{count[0]} #{count[1]} #{count[2]} #{count[3]}"
					for k in (1..2) do	
						if count[k] == 5  then
							return k #return winner
						end		
						if sym == symlist[k] && count[k] == 4 && count[0] == 1 then # pattern 4 disc and one blank
							return temresult #return winning move
						end				
					end	
					3.times do |k|
						if @board[i-4][j]==symlist[k] then
							count[k]-=1
						end					
					end	
				end		
			end
			count = [0,0,0,0] # End of line, recount
		end			
		11.times do |i| # check \ direction 1st half
			loop do			
				3.times do |k|
					if @board[i+count[3]][count[3]]==symlist[k] then
						count[k]+=1
					end					
				end	
				if @board[i+count[3]][count[3]]=='.' then
					temresult = [i+count[3], count[3]]
				end	
				count[3]+=1
				if count[3]>=5 then
					for k in (1..2) do	
						if count[k] == 5  then
							return k #return winner
						end		
						if sym == symlist[k] && count[k] == 4 && count[0] == 1 then # pattern 4 disc and one blank
							return temresult #return winning move
						end				
					end	
					3.times do |k|
						if @board[i+count[3]-5][count[3]-5]==symlist[k] then
							count[k]-=1
						end					
					end	
				end	
				break if (count[3]+i) == 15
			end
			count = [0,0,0,0] # End of line, recount
		end			
		for j in (1..10) do	# check \ direction 2nd half
			loop do			
				3.times do |k|
					if @board[count[3]][j+count[3]]==symlist[k] then
						count[k]+=1
					end					
				end	
				if @board[count[3]][j+count[3]]=='.' then
					temresult = [count[3], j+count[3]]
				end		
				count[3]+=1
				if count[3]>=5 then
					for k in (1..2) do	
						if count[k] == 5  then
							return k #return winner
						end		
						if sym == symlist[k] && count[k] == 4 && count[0] == 1 then # pattern 4 disc and one blank
							return temresult #return winning move
						end				
					end	
					3.times do |k|
						if @board[count[3]-5][j+count[3]-5]==symlist[k] then
							count[k]-=1
						end					
					end	
				end	
				break if (count[3]+j) == 15
			end
			count = [0,0,0,0] # End of line, recount
		end			
		for j in (4..14) do	# check / direction 1st half
			loop do			
				3.times do |k|
					if @board[count[3]][j-count[3]]==symlist[k] then
						count[k]+=1
					end					
				end	
				if @board[count[3]][j-count[3]]=='.' then
					temresult = [count[3], j-count[3]]
				end		
				count[3]+=1
				if count[3]>=5 then
			#		puts "/#{j} #{count[0]} #{count[1]} #{count[2]} #{count[3]}"
					for k in (1..2) do	
						if count[k] == 5  then
							return k #return winner
						end		
						if sym == symlist[k] && count[k] == 4 && count[0] == 1 then # pattern 4 disc and one blank
							return temresult #return winning move
						end				
					end	
					3.times do |k|
						if @board[count[3]-5][j-count[3]+5]==symlist[k] then
							count[k]-=1
						end					
					end	
				end	
				break if count[3] == j + 1
			end
			count = [0,0,0,0] # End of line, recount
		end	
		for j in (1..10) do	# check / direction 2nd half
			loop do			
				3.times do |k|
					if @board[j+count[3]][14-count[3]]==symlist[k] then
						count[k]+=1
					end					
				end	
				if @board[j+count[3]][14-count[3]]=='.' then
					temresult = [j+count[3], 14-count[3]]
				end		
				count[3]+=1
				if count[3]>=5 then
				#	puts "/#{j} #{count[0]} #{count[1]} #{count[2]} #{count[3]}"
					for k in (1..2) do	
						if count[k] == 5  then
							return k #return winner
						end		
						if sym == symlist[k] && count[k] == 4 && count[0] == 1 then # pattern 4 disc and one blank
							return temresult #return winning move
						end				
					end	
					3.times do |k|
						if @board[j+count[3]-5][14-count[3]+5]==symlist[k] then
							count[k]-=1
						end					
					end	
				end	
				break if count[3] + j == 15
			end
			count = [0,0,0,0] # End of line, recount
		end		
		if sym=='.' then
			return 0 # no winner, not finished or draw
		else
			return [-1, -1] # no winning move
		end
	end
	def printResult
		printBoard
		if @win==0 then
		puts "Draw game!"
		else
		puts "Player #{@playerList[@win-1].getSymbol} wins!"
		end
	end
end

class Player
	def initialize(symbol)	# A constructor for initializing the player symbol as the parameter symbol. 
		@symbol = symbol	# @symbol The symbol of the player. It is either “O” or “X”.
	end
	def nextMove(game)		# An abstract method to be implemented in subclasses. 
		raise NotImplementedError	# return a list of length two, [row, column] that is a valid move(within 0–14 and square is unoccupied)
	end
	def getSymbol
		return @symbol
	end
end

class Human < Player
	def nextMove(game)	# This method obtains the next move of a human player from user input. The flow of the method is as follows:
		temMove = [-1, -1]
		loop do
	# 1. Prompt the user to enter a row number and a column number for placing a disc. [R C]
			print "Player #{@symbol}, make a move (row col): "
			move = gets.split(' ')
			2.times do |i| 
				if move[i] != nil then
					temMove[i] = move[i].to_i
				end
			end
			if game.checkMove(temMove)==1 then
	# 3. Return the input position as an array of length two (e.g., [6, 8] for row 6, column 8).
				return temMove
			else
	# 2. In case the player makes an invalid input, display a warning message and go back to Step 1.
				puts "Invalid input. Try again!"
			end
		end	
	end	
end

class Computer < Player
	def nextMove(game)	# determines the next move of a computer player with simple AI.
	# 1. return a position for winning
		temMove = game.checkWin(@symbol)
		if temMove == [-1,-1] then
	# 2. failed to find winning position then random generate a valid position
			loop do
				2.times do |i|
					temMove[i] = rand(15)
				end
				break if (game.checkMove(temMove)==1)
			end
		end
		return temMove
	end	
end

# main program
Gomoku.new.startGame