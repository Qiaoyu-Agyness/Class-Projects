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
 * Controls all actions to do with mouse events over Column
 * 
 * @author Qiaoyu Liao (qliao@wpi.edu)
 *
 */

public class NestorColumnController extends MouseAdapter {
	
	protected Nestor theGame;
	
	protected ColumnView cv;
	

	//constructing the Column Controller
	public NestorColumnController(Nestor theGame, ColumnView cv){
		super();
		this.theGame = theGame;
		this.cv = cv;			
	}

	/**
	 * Coordinate reaction to the beginning of a Drag Event.
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
		Column col = (Column) cv.getModelElement();
		if (col.count() == 0) {
			c.releaseDraggingObject();
			return;
		}
	
		// Get a card to move from PileView. Note: this returns a CardView.
		// Note that this method will alter the model for BuildablePileView if the condition is met.
		CardView cardView = cv.getCardViewForTopCard (me);
				
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
			System.err.println ("ColumnController::mousePressed(): Unexpectedly encountered a Dragging Object during a Mouse press.");
			return;
		}

		// Tell container which object is being dragged, and where in that widget the user clicked.
		c.setActiveDraggingObject (cardView, me);

		// Tell container which BuildablePileView is the source for this drag event.
		c.setDragSource (cv);

		// we simply redraw our source pile to avoid flicker,
		// rather than refreshing all widgets...
		cv.redraw();
	}
	
	/**
	 * Coordinate reaction to the completion of a Drag Event.
	 * <p>
	 * A bit of a challenge to construct the appropriate move, because cards
	 * can be dragged both from the WastePile (as a CardView object) and the 
	 * BuildablePileView (as a ColumnView).
	 * @param me java.awt.event.MouseEvent
	 */
	public void mouseReleased(MouseEvent me) {
		Container c = theGame.getContainer();
		
		/** Return if there is no card being dragged chosen. */
		Widget draggingWidget = c.getActiveDraggingObject();
		if (draggingWidget == Container.getNothingBeingDragged()) {
			System.err.println ("ColumnController::mouseReleased() unexpectedly found nothing being dragged.");
			c.releaseDraggingObject();		
			return;
		}

		/** Recover the from BuildablePile OR waste Pile */
		Widget fromWidget = c.getDragSource();
		if (fromWidget == null) {
			System.err.println ("ColumnController::mouseReleased(): somehow no dragSource in container.");
			c.releaseDraggingObject();
			return;
		}
		BuildablePile bp = new BuildablePile("bp");
		BuildablePileView bpv = new BuildablePileView(bp);
		
		/** if the card is from a column view */
		if(fromWidget.getClass().equals(cv.getClass())){
			Column from = (Column) fromWidget.getModelElement();
			Column to = (Column) cv.getModelElement();
			
			CardView cardView = (CardView) draggingWidget;
			Card theCard = (Card) cardView.getModelElement();
			
			Move move = new MoveCardMove(theGame.deck, from, theCard, to);
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
		
		/** if the card is from a buildablePile view */
		else if(fromWidget.getClass().equals(bpv.getClass())){
			BuildablePile from = (BuildablePile) fromWidget.getModelElement();
			Column to = (Column) cv.getModelElement();
			
			CardView cardView = (CardView) draggingWidget;
			Card theCard = (Card) cardView.getModelElement();
			
			Move move = new MoveCardColumn(theGame.deck, from, theCard, to);
			if(move.doMove(theGame)){
				theGame.pushMove(move);      // Successful Move has been Move
			} else {
				System.out.println("send card back to where it is");
				fromWidget.returnWidget(draggingWidget);
			}
			
			// release the dragging object, (this will reset dragSource)
			c.releaseDraggingObject();
						
			// finally repaint
			c.repaint();
			
		}
		
		/** if the card is not from anything */
		else{
			// Simply return the widget to where it came from.
			fromWidget.returnWidget (draggingWidget);

			// release the dragging object, (container will reset dragSource) and repaint
			c.releaseDraggingObject();		
			c.repaint();
		}
				
	}


}
