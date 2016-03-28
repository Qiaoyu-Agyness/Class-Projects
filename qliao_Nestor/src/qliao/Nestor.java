package qliao;

import java.awt.Dimension;


import ks.common.controller.SolitaireMouseMotionAdapter;
import ks.common.controller.SolitaireReleasedAdapter;
import ks.common.games.Solitaire;
import ks.common.games.SolitaireUndoAdapter;
import ks.common.model.BuildablePile;
import ks.common.model.Card;
import ks.common.model.Column;
import ks.common.model.Deck;
//import ks.common.view.BuildablePileView;
import ks.common.view.CardImages;
import ks.common.view.ColumnView;
import ks.common.view.IntegerView;
import ks.common.view.StringView;
import ks.launcher.Main;

/**
 * 
 * 
 * Nestor variation of Solitaire
 * 
 * @author Qiaoyu Liao (qliao@wpi.edu)
 *
 */
public class Nestor extends Solitaire {
	
	protected Deck deck;
	protected Column columns[] = new Column [9];
	protected BuildablePile reserves;
	protected ColumnView columnViews[] = new ColumnView[9];
	protected BuildablePileView reservesView;
	protected IntegerView scoreView;
	protected IntegerView numLeftView;
	
	public Nestor(){
		super();
	}
	

	@Override
	public String getName() {
		return "qliao-Nestor";
	}

	@Override
	public boolean hasWon() {
		return getScoreValue() == 52;
	}
	
	@Override
	public Dimension getPreferredSize() {
		// default starting dimensions...
		return new Dimension(865, 635);
	}

	@Override
	public void initialize() {
		// initialize model
				initializeModel(getSeed());
				initializeView();
				initializeControllers();

				// prepare game by dealing into 8 columns with 6 cards and a buildable pile with 4 card
				// haven't consider the repetition in one column yet
				for (int pileNum=1; pileNum <= 8; pileNum++) {
					for (int num = 1; num <= 6; num++) {
						Card c = deck.get();
						c.setFaceUp(true);
						//make sure no card at the same pile has the same rank
						   for(int i=0; i < num-1; i++){
							if(columns[pileNum].peek(i).sameRank(c)){
								deck.add(c);
								deck.shuffle(getSeed());
								c = deck.get();
								c.setFaceUp(true);
							}
						} 
						columns[pileNum].add(c);
					}
				}
				// deal into the reserves pile
				for(int num = 1; num <=3; num++){
					Card c = deck.get();
					c.setFaceUp(false);
					reserves.add(c);
				}				
				
				Card c = deck.get();
				c.setFaceUp(true);
				reserves.add(c);
				
				
			

	}

	private void initializeControllers() {
		
				// Set controllers for the Columns
				for (int i = 1; i <= 8; i++) {
					columnViews[i].setMouseAdapter (new NestorColumnController (this, columnViews[i]));
					columnViews[i].setMouseMotionAdapter (new SolitaireMouseMotionAdapter (this));
					columnViews[i].setUndoAdapter (new SolitaireUndoAdapter(this));
				}
				
				// Set controllers for the BuildablePile
				reservesView.setMouseAdapter (new NestorBuildablePileController(this,reservesView));
				reservesView.setMouseMotionAdapter (new SolitaireMouseMotionAdapter (this));
				reservesView.setUndoAdapter (new SolitaireUndoAdapter(this));
				
				// Ensure that any releases (and movement) are handled by the non-interactive widgets
				numLeftView.setMouseMotionAdapter (new SolitaireMouseMotionAdapter(this));
				numLeftView.setMouseAdapter (new SolitaireReleasedAdapter(this));
				numLeftView.setUndoAdapter (new SolitaireUndoAdapter(this));

				// same for scoreView
				scoreView.setMouseMotionAdapter (new SolitaireMouseMotionAdapter(this));
				scoreView.setMouseAdapter (new SolitaireReleasedAdapter(this));
				scoreView.setUndoAdapter (new SolitaireUndoAdapter(this));

				// Finally, cover the Container for any events not handled by a widget:
				getContainer().setMouseMotionAdapter(new SolitaireMouseMotionAdapter(this));
				getContainer().setMouseAdapter (new SolitaireReleasedAdapter(this));
				getContainer().setUndoAdapter (new SolitaireUndoAdapter(this));
	}

	private void initializeView() {
		CardImages ci = getCardImages();

		// create ColumnViews, one after the other (default to 10 full cards -- more than we'll need)
		for (int pileNum = 1; pileNum <=8; pileNum++) {
			columnViews[pileNum] = new ColumnView (columns[pileNum]);
			columnViews[pileNum].setBounds (20*pileNum + (pileNum-1)*ci.getWidth(), 40, ci.getWidth(), 10*ci.getHeight());
			container.addWidget (columnViews[pileNum]);
		}
		
		// create the BuildablePileView for reserves pile
		reservesView = new BuildablePileView(reserves);
		reservesView.setBounds(180+8*ci.getWidth(), 40, ci.getWidth(), 6*ci.getHeight());
		container.addWidget(reservesView);

		// create Score View
		StringView score = new StringView("Score");
		score.setFontSize(20);
		score.setBounds(160,0,50,30);
		container.addWidget(score);
		scoreView = new IntegerView(getScore());
		scoreView.setFontSize(20);
		scoreView.setBounds(215,0,30,30);
		container.addWidget(scoreView);
		
		// create cards left view
		StringView cardsLeft = new StringView("Cards Left");
		cardsLeft.setFontSize(20);
		cardsLeft.setBounds(20, 0, 100, 30);
		container.addWidget(cardsLeft);
		numLeftView = new IntegerView(getNumLeft());
		numLeftView.setFontSize(20);
		numLeftView.setBounds(125,0,30,30);
		container.addWidget(numLeftView);
	}

	private void initializeModel(int seed) {
		// initialize the deck
		deck = new Deck("deck");
		deck.create(seed);
		model.addElement(deck);   // add to our model (as defined within our superclass).
		
		// initialize each of the columns
		for (int i = 1; i<=8; i++) {
			columns[i] = new Column ("column" + i);
			model.addElement (columns[i]);
		} 
		
		// initialize the reserve Pile
		reserves = new BuildablePile("reserves");
		model.addElement(reserves);
		
		
		// initial score is set to ZERO (every Solitaire game by default has a score) and there are 52 cards left.
		this.updateNumberCardsLeft(52);
		this.updateScore(0);
		
	}
	
	/** Code to launch solitaire variation. */
	public static void main (String []args) {
		// Seed is to ensure we get the same initial cards every time.
		// Here the seed is to "order by suit."
		Main.generateWindow(new Nestor(), Deck.OrderBySuit);
	}



}
