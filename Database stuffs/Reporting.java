/* Database Project1 Phase3
 * Qiaoyu Liao
 * qliao@wpi.edu
 */
import java.sql.*;
import java.util.Scanner;

public class Reporting {
	
	public static void main(String[] args) throws SQLException{

		
		DriverManager.registerDriver(new oracle.jdbc.driver.OracleDriver());
		
		System.out.println("Oracle JDBC Driver Registered!");
		Connection conn = null;
		//System.out.println(args[0]);
		//System.out.println(args[1]);
		try{
			conn = DriverManager.getConnection(
					"jdbc:oracle:thin:@oracle.wpi.edu:1521:WPI11grxx",args[0],args[1]);
		} catch(SQLException e){
			System.out.println("Connection Failed! Check the output console");
			e.printStackTrace();
			return;
		}
		
		System.out.println("Oracle JDBC Driver Connected!");
		
		
		// if no option inputed
		if(args.length < 3){
			System.out.println("1 – Report Customer Information" + "\n" + "2 – Report Tour Guide Information" + "\n" + "3 – Report Booked Tour Information" + "\n" + "4 – Update Booked Tour Vehicle");
			conn.close();
			return;
		}
		
		
		//meet condition 1, use a user's id to get the information
		else if(args[2].equals("1")){
			Scanner reader = new Scanner(System.in);
			System.out.println("Enter Customer ID:");
			String s = reader.nextLine();
			
			Statement stmt = conn.createStatement();
			String str1 = "SELECT * FROM CUSTOMER WHERE CustomerID = " + s;
			ResultSet rset1 = stmt.executeQuery(str1);
			
			reader.close();
			
			int custID = 0,age = 0;
			long phone;
			String firstn = "", lastn = "", address = "";
			
			while(rset1.next()){
				custID = rset1.getInt("CustomerID");
				firstn = rset1.getString("FirstName");
				lastn = rset1.getString("LastName");
				address = rset1.getString("Address");
				phone = rset1.getLong("Phone");
				age = rset1.getInt("Age");
				System.out.println("Customer Information");
				System.out.println("ID: " + custID + "\n" + "FirstName: " + firstn + "\n" + "LastName: "  + lastn + "\n" + "Address: " + address + "\n" + "Phone: " + phone + "\n" + "Age: " + age);
			}
			
			if (age == 0){
				System.out.println("Wrong Input, cannot get the information");
			}
			
			rset1.close();
			stmt.close();
			conn.close();
			return;
		}
		
		
		// if meet the condition 2
		else if(args[2].equals("2")){
			Scanner reader = new Scanner(System.in);
			System.out.println("Enter Tour Guide's Driver License:");
			String s = reader.nextLine();
			
			//System.out.println(s);
			
			Statement stmt = conn.createStatement();
			String str2 = "SELECT * FROM GUIDE WHERE DriverLicense = '" + s + "'";
			ResultSet rset2 = stmt.executeQuery(str2);
			
			String lsID = "",firstn = "", lastn = "", title = "", vt = "";
			
			while(rset2.next()){
				lsID = rset2.getString("DriverLicense");
				firstn = rset2.getString("FirstName");
				lastn = rset2.getString("LastName");
				title = rset2.getString("Title");
				vt = rset2.getString("VehicleType");
				System.out.println("TourGuide Information");
				System.out.println("DriverLicense: " + lsID + "\n" + "FirstName: " + firstn + "\n" + "LastName: "  + lastn + "\n" + "Title: " + title + "\n" + "VehicleType: " + vt);
			}
			
			if (lsID.equals("")){
				System.out.println("Wrong Input, cannot get the information");
			}
			
			rset2.close();
			stmt.close();
			conn.close();
			return;
		}
		
		
		
		
		//if meet the condition 3
		else if(args[2].equals("3")){
			Scanner reader = new Scanner(System.in);
			System.out.println("Enter Booked Tour ID:");
			String s = reader.nextLine();
			
			Statement stmt = conn.createStatement();
			String str = "SELECT * FROM BookedTour WHERE BookedTourID = " + s;
			ResultSet rset = stmt.executeQuery(str);
			
			int btID = 0, ctID = 0, tID = 0, price = 0;
			String dlID = "", ls = "";
			Date pd = null, td = null;
			
			while(rset.next()){
				btID = rset.getInt("BookedTourID");
				ctID = rset.getInt("CustomerID");
				tID = rset.getInt("TourID");
				dlID = rset.getString("DriverLicense");
				//travel date and purchase date
				price = rset.getInt("TotalPrice");
				ls = rset.getString("LicensePlate");
				pd = rset.getDate("PurchaseDate");
				td = rset.getDate("Traveldate");				
			}
			
			if (btID == 0){
				System.out.println("Wrong Input, cannot get the information");
				return;
			}
	
			//get the cusomer info
			//Statement stmtc = conn.createStatement();
			String strc = "SELECT * FROM Customer WHERE CustomerID = " + ctID;
			ResultSet rsetc = stmt.executeQuery(strc);
			
			String cfname = "", clname = "";
			int cage = 0;
			while(rsetc.next()){
				cfname = rsetc.getString("FirstName");
				clname = rsetc.getString("LastName");
				cage = rsetc.getInt("Age");
			}			
			//get the travelingwith info
			//Statement stmttw = conn.createStatement();
			String strtw = "SELECT * FROM TravelingWith WHERE CustomerID = " + ctID;
			ResultSet rsettw = stmt.executeQuery(strtw);
			int count = 0;
			String[] twfname, twlname;
			int[] twage;
            //assume no more than 15 travelingwith
			twfname = new String[20];
			twlname = new String[20];
			twage = new int[20];
			while(rsettw.next()){
				twfname[count] = rsettw.getString("FirstName");
				twlname[count] = rsettw.getString("LastName");
				twage[count] = rsettw.getInt("Age");
				count++;
			}
			
			//get the tour info
			String strt = "SELECT * FROM Tour WHERE TourID = " + tID;
			ResultSet rsett = stmt.executeQuery(strt);
			String tname = "";
			while(rsett.next()){
				tname = rsett.getString("TourName");
			}
			
			//get the guide info
			String strtg = "SELECT * FROM Guide WHERE DriverLicense = '" + dlID + "'";
			ResultSet rsettg = stmt.executeQuery(strtg);
			String tgfname = "", tglname = "";
			while(rsettg.next()){
				tgfname = rsettg.getString("FirstName");
				tglname = rsettg.getString("LastName");
			}
			
			//all the printout for part3
			System.out.println("Booked Tour Information");
			System.out.println("Booked Tour ID: " + btID);
			System.out.println("Customer Name: " + cfname + " "+ clname);
			System.out.println("Customer Age: " + cage);
			System.out.println("People Traveling With");
			for(int i = 0; i < count; i++){
				System.out.println("	" + twfname[i] + " " + twlname[i] + "  " + twage[i]);
			}
			System.out.println("Tour Name: " + tname);
			System.out.println("Purchase Date: " + pd.toString());
			System.out.println("Travel Date: " + td.toString());
			System.out.println("Tour Guide Name: " + tgfname + tglname);
			System.out.println("License Plate: " + ls);
			System.out.println("Total Price: " + price);			
			rset.close();
			rsetc.close();
			rsettw.close();
			rsett.close();
			rsettg.close();
			stmt.close();
			conn.close();
		}
		
		else if(args[2].equals("4")){
			Scanner reader = new Scanner(System.in);
			System.out.println("Enter Booked Tour ID: ");
			String s1 = reader.nextLine();
			
			System.out.println("Enter the new vehicle License Plate: ");
			String s2 = reader.nextLine();
			
			
			Statement stmt = conn.createStatement();
			System.out.println("Connection Established");
			String str1 = "DISABLE CONSTRAINT FKLICENSEPLATEBT";
			String str = "UPDATE BookedTour SET LicensePlate = '" + s2 + "' WHERE BookedTourID = " + s1;
			//System.out.println(str);
			int rset = stmt.executeUpdate(str);
			if(rset != 0){
				System.out.println("Sucessful Update the License Plate");
			}
			if(rset == 0){
				System.out.println("Update not success");
			}
			stmt.close();
			conn.close();
			return;
		}
		
		else{
			System.out.println("Run use java Reporting <username> <password> -options" + "\n" + "1 – Report Customer Information" + "\n" + "2 – Report Tour Guide Information" + "\n" + "3 – Report Booked Tour Information" + "\n" + "4 – Update Booked Tour Vehicle");
			return;
		}
	}
	
}
