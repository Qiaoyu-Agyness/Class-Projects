package qliao;


import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

import ks.common.model.BuildablePile;
import ks.common.model.Card;
import ks.common.model.Column;
import ks.common.model.Move;
//import ks.common.view.BuildablePileView;
import ks.common.view.CardView;
import ks.common.view.ColumnView;
import ks.common.view.Container;
import ks.common.view.Widget;
/**
 * Controls all actions to do with mouse events over BuildablePile
 * 
 * 
 * 
 * 
 * @author Qiaoyu Liao (qliao@wpi.edu)
 *
 */
public class NestorBuildablePileController extends MouseAdapter {
	
	protected Nestor theGame;
	
	protected BuildablePileView bpv;
	
	public NestorBuildablePileController(Nestor theGame, BuildablePileView bpv){
		super();
		this.theGame = theGame;
		this.bpv = bpv;
	}
	
	/**
	 * Coordinate reaction to the beginning of a Drag Event.
	 *
	 * Note: There is no way to differentiate between a press that
	 *       will become part of a double click vs. a click that will
	 *       be held and dragged. Only mouseReleased will be able to 
	 *       help us out with that one.
	 *
	 * Creation date: (10/4/01 6:05:55 PM)
	 * @param me java.awt.event.MouseEvent
	 */
	public void mousePressed(MouseEvent me) {

		// The container manages several critical pieces of information; namely, it
		// is responsible for the draggingObject; in our case, this would be a CardView
		// Widget managing the card we are trying to drag between two piles.
		Container c = theGame.getContainer();

		/** Return if there is no card to be chosen. */
		BuildablePile bp = (BuildablePile) bpv.getModelElement();
		
		if (bp.count() == 0) {
			c.releaseDraggingObject();
			return;
		}
		
		/** if there's no card face up */
		if (bp.getNumFaceUp() == 0) {
			System.err.println ("BuildablePileController::mousePressed(). Unexpected failure in get the card.");
			c.releaseDraggingObject();
			return;
		}
		
		// Get a column of cards to move from the BuildablePileView
		// Note that this method will alter the model for BuildablePileView if the condition is met.
		ColumnView colView = bpv.getColumnView (me);
		
		// an invalid selection (either all facedown, or not in faceup region)
		if (colView == null) {
			c.releaseDraggingObject();
			return;
		}
		
		// Check conditions
		Column col = (Column) colView.getModelElement();
		if (col == null) {
			System.err.println ("BuildablePileController::mousePressed(): Unexpectedly encountered a ColumnView with no Column.");
			c.releaseDraggingObject();
			return; // sanity check, but should never happen.
		}
	
		// Get a card to move from PileView. Note: this returns a CardView.
		// Note that this method will alter the model for BuildablePileView if the condition is met.
		CardView cardView = colView.getCardViewForTopCard (me);
				
		// an invalid selection of some sort.
		if (cardView == null) {
			c.releaseDraggingObject();
			return;
		}

		// If we get here, then the user has indeed clicked on the top card in the PileView and
		// we are able to now move it on the screen at will. For smooth action, the bounds for the
		// cardView widget reflect the original card location on the screen.
		Widget w = c.getActiveDraggingObject();
		if (w != Container.getNothingBeingDragged()) {
			System.err.println ("BuildablePileController::mousePressed(): Unexpectedly encountered a Dragging Object during a Mouse press.");
			return;
		}

		// Tell container which object is being dragged, and where in that widget the user clicked.
		c.setActiveDraggingObject (cardView, me);

		// Tell container which BuildablePileView is the source for this drag event.
		c.setDragSource (bpv);

		// we simply redraw our source pile to avoid flicker,
		// rather than refreshing all widgets...
		bpv.redraw();
	}
	
	/**
	 * Coordinate reaction to the completion of a Drag Event.
	 * 
	 * @param me java.awt.event.MouseEvent
	 */
	public void mouseReleased(MouseEvent me) {
		Container c = theGame.getContainer();
		
		/** Return if there is no card being dragged chosen. */
		Widget draggingWidget = c.getActiveDraggingObject();
		if (draggingWidget == Container.getNothingBeingDragged()) {
			System.err.println ("BuildablePileController::mouseReleased() unexpectedly found nothing being dragged.");
			c.releaseDraggingObject();		
			return;
		}

		/** Recover the from BuildablePile OR waste Pile */
		Widget fromWidget = c.getDragSource();
		if (fromWidget == null) {
			System.err.println ("BuildablePileController::mouseReleased(): somehow no dragSource in container.");
			c.releaseDraggingObject();
			return;
		}
		Column col = new Column("col");
		ColumnView cv = new ColumnView(col);
		
		/** if the card is from a column view */
		if(fromWidget.getClass().equals(cv.getClass())){
			Column from = (Column) fromWidget.getModelElement();
			BuildablePile to = (BuildablePile) bpv.getModelElement();
			
			CardView cardView = (CardView) draggingWidget;
			Card theCard = (Card) cardView.getModelElement();
			
			Move move = new MoveCardReserve(theGame.deck,from, theCard,to);
			if (move.doMove(theGame)) {
				theGame.pushMove (move);     // Successful Move has been Move
			} else {
				fromWidget.returnWidget (draggingWidget);
			}
			
			// release the dragging object, (this will reset dragSource)
			c.releaseDraggingObject();
			
			// finally repaint
			c.repaint();
		}	
		
		// if it is not dragged from a column
		else{
			// Simply return the widget to where it came from.
			fromWidget.returnWidget (draggingWidget);

			// release the dragging object, (container will reset dragSource) and repaint
			c.releaseDraggingObject();		
			c.repaint();
		}
				
	}
	
	/**
	 * Coordinate reaction to the Click Event.
	 * 
	 * @param me java.awt.event.MouseEvent
	 */
	public void mouseClicked(MouseEvent me){
		Container c = theGame.getContainer();
		//check the click count
		if(me.getClickCount() >= 1){
			
			BuildablePile bp = (BuildablePile) bpv.getModelElement();
			// if the buildable pile has no card in it
			if(bp.count() == 0){
				c.releaseDraggingObject();	
				return;
			}
						
			// if the buildable pile has no card face up,flip the top card in the buildable pile
			
			
			if(bp.getNumFaceUp() == 0){
				Move move = new FlipCard(bp);
				if(move.doMove(theGame)){
					theGame.pushMove(move);
					theGame.refreshWidgets();
				} else {
					System.err.println ("BuildablePileController::mousePressed(). Unexpected failure in flip card.");
				}
				c.releaseDraggingObject();	
				return;
			}
			
			// if there's card face up in the buildable pile
			
				c.releaseDraggingObject();	
				return;
			
			
		}
	}

}
