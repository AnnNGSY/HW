package Player;{
	use strict;
	use warnings;
	sub new{	 # constructor
		my $class = shift;
		my ($symbol) = @_;
		my $self = bless {
			symbol => $symbol,
		}, $class;
		return $self;
	}
	sub nextMove{ # to be implemented in sub classes;
		return (-1, -1);
	}
}
package Human;{
	use strict;
	use warnings;
	use base qw ( Player );
	sub nextMove{ # assumes input always 2 integers with space in middle
		my $self = shift;
		print "Player $self->{symbol}, make a move (row col): ";
		chomp(my @position = split(/ /, <STDIN>)); 		# get input form standin split with space and remove return key
		return @position;
	}
}
package Computer;{# version 2
	use strict;
	use warnings;
	use base qw ( Player );
	use Clone 'clone';
	sub nextMove{
		my $self = shift;
		my ($reversiPtr) = @_;
		my $retMove;
		my @avalibleMoves = ();
		my $minOpMoba = 100;		# save the real game setting when entering....
		my $temdiscs = clone(\@{$reversiPtr->{discs}});
		my $temboard = clone(\@{$reversiPtr->{board}});
		my $temturn = $reversiPtr->{turn};
		for my $i (0..7) { 
			for my $j (0..7) {	
				my @temMove = ($i, $j);
				if($reversiPtr->putDisc($i, $j , 1)){ # place the disc if it is valid
					$reversiPtr->{turn} ^= 1; # imaging oppent's move
					my $opMobility = 0;				
					for my $m (0..7) { 	# count the Mobility for each valid move
						for my $n (0..7) {							
							if($reversiPtr->putDisc($m, $n , 0)) { # place the disc if it is valid							
								$opMobility++;
							}		
						}
					}
					if($opMobility==$minOpMoba){
						push(@avalibleMoves, \@temMove); # put all avalible moves in an array
					} elsif($opMobility<$minOpMoba){ # if new minimum mobility is found
						@avalibleMoves = (); # clear other validmoves
						$minOpMoba = $opMobility;
						push(@avalibleMoves, \@temMove); # put all avalible moves in an array
					}
				}
				$reversiPtr->{discs} = clone(\@{$temdiscs});
				$reversiPtr->{board} = clone(\@{$temboard});
				$reversiPtr->{turn} = $temturn;
			}
		}
		$reversiPtr->{discs} = clone(\@{$temdiscs}); # reset the discs counters 
		$reversiPtr->{board} = clone(\@{$temboard}); # reset the board 
		$reversiPtr->{turn} = $temturn; # reset turn
		if(my $range = (scalar @avalibleMoves)){# if array size of valid moves is not 0
			$retMove = $avalibleMoves[int(rand($range))];# get a random number
			return @$retMove; # return the (random)th element for move
		}else{
			return (-1, -1); 
		}	
	}
}
package Reversi;{ # models the Reversi game
	use strict;
	use warnings;
	use Clone 'clone';
	our $BOARDSIZE = 8;
	sub new { # initializing all the instance variables	
		my $class = shift;
		my @board;# 8*8 2d array representing game board		
		for my $i (0..$BOARDSIZE-1) {
			for my $j (0..$BOARDSIZE-1) {
				if (($i==3&&$j==3)||($i==4&&$j==4)) {
					$board[$i][$j] = 'O';
				} elsif (($i==3&&$j==4)||($i==4&&$j==3)) {
					$board[$i][$j] = 'X';
				} else {
					$board[$i][$j] = '.';
				}
			}
		}
		my @discs = (2, 2, 0); # counter of discs and passed round	
		my $self = bless {
			board => \@board,
			discs => \@discs,
			playerList => [my $black, my $white], # put players in array for easy management
			turn => 0, # player in the current turn: start form black
		}, $class;
		return $self;
	}
	sub startGame{		
		my $self = shift;		
		my $orders = ['First', 'Second'];
		my $sym = ['X', 'O'];
		my $player = {'1' => 'Computer', '2' => 'Human'}; 
		for (my $i = 0; $i < 2; $i++) { # let users choose players' type
			print "$$orders[$i] player is (1) Computer or (2) Human? ";
			chomp(my $choice = <STDIN>);
			$self->{playerList}[$i] = ($choice eq '1') ? Computer->new($$sym[$i]) : Human->new($$sym[$i]);
			print "Player $$sym[$i] is $$player{$choice}\n";
		}
		while (1) {	# Perl : non-zero => false
			my @move = (-1, -1);
			$self->printBoard();
			last if ( ($self->{discs}[0] + $self->{discs}[1]) >= 64||$self->{discs}[2] >= 2);	# Repeat until ( board full || 2 consecutive passes )
			if(($self->putDisc(@move = ($self->{playerList}[$self->{turn}]->nextMove($self)), 1))){			# Obtain a position, put disc and filp if vaild
				print("Player $self->{playerList}[$self->{turn}]->{symbol} places to row $move[0], col $move[1]\n");
				$self->{discs}[2] = 0;
			}else{
				print("Row $move[0], col $move[1] is invalid! Player $self->{playerList}[$self->{turn}]->{symbol} passed!\n");
				$self->{discs}[2]++;
			}
			$self->{turn} ^= 1; # Alternate Players X and O in each round
		}		
		$self->checkWin(); # display the message when game finished
	}
	sub putDisc{
		my $self = shift;
		my (@move) = @_; # move = (row, col, flag)
		my $ret = 0;
		my $temboard = clone(\@{$self->{board}});
		for (my $i = 0; $i < 2; $i++) {
			if($move[$i]<0||$move[$i]>=$BOARDSIZE){
				return 0; # out-of-board
			}
		}
		if($$temboard[$move[0]][$move[1]] ne '.'){
			return 0; # required position occupied
		}
		my @vector=(0, 0);
		for (my $i = -1; $i <= 1; $i++){		# for each direction...
			for (my $j = -1; $j <= 1; $j++){
				if($i!=0 || $j!=0){
					@vector = ($i, $j);
					my $row = $move[0] + $vector[0];
					my $col = $move[1] + $vector[1];
					my $loop = 1;
					my $filp = 0;
					my $count = 0;
					while($row >= 0 && $row < $BOARDSIZE && $col >= 0 && $col < $BOARDSIZE && $loop){
						if($$temboard[$row][$col] ne ($self->{playerList}[1-$self->{turn}]->{symbol})){
							$loop = 0;									
						}else{
							$count++;
						}
						if($loop){
							$row = $row + $vector[0];
							$col = $col + $vector[1];
						}else{
							if($count && ($$temboard[$row][$col] eq ($self->{playerList}[$self->{turn}]->{symbol}))){			
								$ret = 1;
								$filp = 1;
							}
						}
					}
					if($filp&&$move[2]){
						while($row!=$move[0] || $col!= $move[1]){
							if(($$temboard[$row][$col])ne($self->{playerList}[$self->{turn}]->{symbol})){
								$$temboard[$row][$col] = $self->{playerList}[$self->{turn}]->{symbol};	
								$self->{discs}[$self->{turn}]++;
								$self->{discs}[1-$self->{turn}]--;							
							}
							$row = $row - $vector[0];
							$col = $col - $vector[1];							
						}	
					}							
				}				
			}			
		}
		if($move[2]&&$ret){
			$$temboard[$move[0]][$move[1]] = $self->{playerList}[$self->{turn}]->{symbol};
			$self->{discs}[$self->{turn}]++;
			$self->{board} = $temboard;				
		}					
		return $ret;
	}
	sub checkWin{
		my $self = shift;
		if($self->{discs}[0] == $self->{discs}[1]){
			print("Draw game!\n");
		} else{
			print("Player $self->{playerList}[$self->{discs}[0] < $self->{discs}[1]]->{symbol} wins!\n");
		}
	}
	sub printBoard{
		my $self = shift;
		print " ";
		for my $i (0..$BOARDSIZE-1){
			print " $i";
		}
		print "\n";
		for my $i (0..$BOARDSIZE-1){
			print "$i";
			for my $j (0..$BOARDSIZE-1){
				print " $self->{board}[$i][$j]";
			}
			print " \n";
		}
		for my $i (0..1) {
			print("Player $self->{playerList}[$i]->{symbol}: $self->{discs}[$i]\n");
		}		
	}
}
my $game = Reversi->new(); # Create object; set up human/computer players 
$game->startGame(); # Start playing game