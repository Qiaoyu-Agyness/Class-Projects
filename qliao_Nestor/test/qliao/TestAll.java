package qliao;

import java.awt.AWTException;
import java.awt.Robot;
import java.awt.event.InputEvent;
import java.awt.event.MouseEvent;

import junit.framework.TestCase;
import ks.client.gamefactory.GameWindow;
import ks.common.games.Solitaire;
import ks.common.model.BuildablePile;
import ks.common.model.Card;
import ks.common.model.Deck;
import ks.common.view.Container;
import ks.common.view.Widget;
import ks.launcher.Main;
import ks.tests.KSTestCase;

public class TestAll extends KSTestCase {
	Nestor n1;
	GameWindow gw1;
	
	public void setUp(){
		n1 = new Nestor();
		gw1 = Main.generateWindow(n1, Deck.OrderBySuit);
	}
	
	public void tearDown(){
		gw1.dispose();
	}
	
	//test Class MoveCardReserve
	public void testMCR(){
		
		Card topCard1 = n1.columns[1].get();
		
		
		MoveCardReserve mcr1 = new MoveCardReserve(n1.deck,n1.columns[1],topCard1,n1.reserves);
		//the from object is null
		MoveCardReserve mcr2 = new MoveCardReserve(n1.deck,null,topCard1,n1.reserves);		
		//check if both of them are valid
		mcr1.valid(n1);
		mcr2.valid(n1);
		//when two top cards are not the same, do and undo the move
		mcr1.doMove(n1);
		mcr1.undo(n1);

		//set two top cards to be the same, then do and undo the move
		n1.reserves.add(topCard1);
		mcr1.doMove(n1);
		mcr1.undo(n1);		
	}
	
	//test class MoveCardMove
	public void testMCM(){
		Card topCard2 = n1.columns[2].get();
		MoveCardMove mcm1 = new MoveCardMove(n1.deck,n1.columns[2],topCard2,n1.columns[3]);
		//check if the move is valid
		mcm1.valid(n1);
		//do the move when two top cards are not the same
		mcm1.doMove(n1);
		mcm1.undo(n1);
		//set two top cards to be the same,then do and undo the move
		n1.columns[3].add(topCard2);
		mcm1.doMove(n1);
		mcm1.undo(n1);
	}
	
	//test class FlipCard
	public void testFC(){
		FlipCard fc1 = new FlipCard(n1.reserves);	
		n1.reserves.peek().setFaceUp(true);
		//Flip card when the top card is Face Up(which nothing should happen), check valid, do and undo the move
		fc1.valid(n1);
		fc1.doMove(n1);
		fc1.undo(n1);
		//flip card when the top card is Face Down (which do the move), check valid, do and undo the move
		n1.reserves.peek().setFaceUp(false);
		fc1.valid(n1);
		fc1.doMove(n1);
		fc1.undo(n1);
	}
	
	//test class MoveCardColumn
	public void testMCC(){
		n1.reserves.peek().setFaceUp(true);
		Card topCard3 = n1.reserves.get();
		MoveCardColumn mcc1 = new MoveCardColumn(n1.deck,n1.reserves,topCard3,n1.columns[4]);
		//check valid, do and undo the move when two top cards are not the same
		mcc1.valid(n1);
		mcc1.doMove(n1);
		mcc1.undo(n1);
		//check valid, do and undo the move when two top cards are the same
		n1.columns[4].add(topCard3);
		mcc1.valid(n1);
		mcc1.doMove(n1);
		mcc1.undo(n1);
	}
	
	//test FlipCard using the controller
	public void testBPC(){
		//check to flip card
		n1.reserves.peek().setFaceUp(false);
		MouseEvent me2 = createClicked(n1,n1.reservesView,5,20);
		n1.getContainer().dispatchEvent(me2);
	
		//check when no card to flip
		MouseEvent me1 = createClicked(n1,n1.reservesView,5,20);
		n1.getContainer().dispatchEvent(me1);	
	
	}
	
	//test MoveCardColumn using the controller
	public void testMoveCardColumn(){
		//set top cards to be the same
		Card tc = n1.reserves.peek();
		n1.columns[1].add(tc);
		//create mouse event
		MouseEvent me1 = createPressed(n1,n1.reservesView,5,20);	
		MouseEvent me2 = createReleased(n1,n1.columnViews[1],5,0);
		//dispatch both events
		n1.getContainer().dispatchEvent(me1);
		n1.getContainer().dispatchEvent(me2);
	}
	
	//test MoveCardMove using the controller
	public void testMoveCardMove(){
		//set top cards to be the same
		Card tc = n1.columns[5].peek();
		n1.columns[6].add(tc);
		//create mouse event
		MouseEvent me1 = createPressed(n1,n1.columnViews[5],5,150);
		MouseEvent me2 = createReleased(n1,n1.columnViews[6],5,10);
		//dispatch both events
		n1.getContainer().dispatchEvent(me1);
		n1.getContainer().dispatchEvent(me2);
		
	}
	
	//test MOveCardReserve using the controller
	public void testMoveCardReserve(){
		//set top cards to be the same
		Card tc = n1.columns[4].peek();
		tc.setFaceUp(true);
		n1.reserves.add(tc);
		//create mouse event
		MouseEvent me1 = createPressed(n1,n1.columnViews[4],5,150);
		MouseEvent me2 = createReleased(n1,n1.reservesView,5,10);
		//dispatch both events
		n1.getContainer().dispatchEvent(me1);
		n1.getContainer().dispatchEvent(me2);
	}
}
