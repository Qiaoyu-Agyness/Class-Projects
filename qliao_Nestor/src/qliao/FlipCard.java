package qliao;

import ks.common.games.Solitaire;
import ks.common.model.BuildablePile;
import ks.common.model.Move;

/**
 * Flip Card move Flip the top card of reserves pile from faceDown to faceUp
 * @author Qiaoyu Liao (qliao@wpi.edu)
 */

public class FlipCard extends Move{
	
	protected BuildablePile bp;
	
	public FlipCard(BuildablePile bp) {
		super ();		
		this.bp = bp;
	}

	@Override
	/*
	 * (non-Javadoc)
	 * @see ks.common.model.Move#doMove(ks.common.games.Solitaire)
	 * 
	 */
	public boolean doMove(Solitaire game) {
		// VALIDATE
		if (valid (game) == false){
			return false;
			}

		// EXECUTE
		bp.flipCard();
		return true;
	}

	@Override
	public boolean undo(Solitaire game) {
		// VALIDATE
		if (bp.empty()){ return false;}

		// UNDO
		bp.flipCard();
		
		return true;
	}

	@Override
	public boolean valid(Solitaire game) {
		// VALIDATION:
		boolean validation = false;

		//    flipCard(pile) : not pile.empty() and not pile.faceUp()
		if (!bp.empty() && !bp.faceUp()){
			validation = true;
		}

		return validation;
	}

}
