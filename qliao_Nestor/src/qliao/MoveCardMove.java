package qliao;

import ks.common.games.Solitaire;
import ks.common.model.Card;
import ks.common.model.Column;
import ks.common.model.Deck;
import ks.common.model.Move;

/**
 * MoveCardMove moves the top card of one column to another to match and remove the cards
 * 
 * @author Qiaoyu Liao (qliao@wpi.edu)
 *
 */
public class MoveCardMove extends Move{
	
	protected Deck deck;
	protected Column from;
	protected Card movingCard;
	protected Column to;
	
	public MoveCardMove(Deck deck, Column from, Card movingCard, Column to){
		this.deck = deck;
		this.from = from;
		this.movingCard = movingCard;
		this.to = to;
	}


	@Override
	public boolean doMove(Solitaire game) {
		if(!valid(game)){ return false;}
		
		/*
		 * remove the movingCard and top card of the column to the deck
		 */
		deck.add(movingCard);	
		deck.add(to.get());
		/*
		 * update stats
		 */
		game.updateScore(2);
		game.updateNumberCardsLeft(-2);
		
		return true;
	}

	@Override
	public boolean undo(Solitaire game) {
		if(deck.count() < 2){ return false;}
		
		/*
		 * move the top card of column and moving card back to the original pile
		 */
		to.add(deck.get());
		from.add(deck.get());
		/*
		 * update stats
		 */
		game.updateScore(-2);
		game.updateNumberCardsLeft(2);
		
		return true;
	}

	@Override
	public boolean valid(Solitaire game) {
		//if anything is null
				if( movingCard == null || from == null || to == null){
					return false;
				}
				
				//if any of the pile is empty but not null
				if(to.empty()){
					return false;
				}
				
				//only if the top card of to pile is the same rank with movingCard
				if(to.peek() != null){
					return to.peek().sameRank(movingCard);
				}
				
				else{ return false;}
	}

}
