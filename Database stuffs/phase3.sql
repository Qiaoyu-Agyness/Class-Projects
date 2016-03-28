/* Project1 Phase3
Qiaoyu Liao qliao@wpi.edu
*/

/* Initial setup 
*/
drop table bookedtour purge;
drop table travelingwith purge;
drop table traveling purge;
drop table tour purge;
drop table location purge;
drop table guide purge;
drop table customer purge;
drop table vehicle purge;

CREATE TABLE Guide( DriverLicense varchar2(12) Primary Key, FirstName varchar2(20), LastName varchar2(20), Phone number(10), VehicleType varchar2(12), Title varchar2(20), Salary number, HireDate date, CONSTRAINT vehicleValg check( VehicleType in('car', 'amphibious', 'bus')), CONSTRAINT titleVal check(Title in('junior guide', 'guide', 'senior guide')));
CREATE TABLE Vehicle( LicensePlate varchar2(7) Primary Key, VehicleType varchar2(12), Make varchar2(15), Model varchar2(15), Year number(4), MaxPassenger number(3), CONSTRAINT vehicleVal check(VehicleType in('car', 'amphibious', 'bus')));
CREATE TABLE Tour(TourID number(4) Primary Key, TourName varchar2(20), Description varchar2(200), City varchar2(20), State varchar2(20), Duration number(3), VehicleType varchar2(12), AdultCost number, ChildCost number, CONSTRAINT vehicleValt check( VehicleType in('car', 'amphibious', 'bus')));
CREATE TABLE Location(LocationID number(4) Primary Key, LocationName varchar2(40), Type varchar2(10), Address varchar2(100), Longitude number(7,4), Latitude number(7,4), CONSTRAINT loctypeVal check(Type in('historic', 'museum', 'restaurant')));
CREATE TABLE Traveling(TourID number(4), LocationID number(4), Constraint fktourIDtravel FOREIGN KEY(TourID) References Tour(TourID), CONSTRAINT fkLocationIDtravel FOREIGN KEY (LocationID) References Location(LocationID), CONSTRAINT pktraveling Primary Key(TourID, LocationID));
CREATE TABLE Customer( CustomerID number(4) Primary Key, FirstName varchar2(20), LastName varchar2(20), address varchar2(100), Phone number(10), Age number(3));
CREATE TABLE TravelingWith(TravelingWithID number(4), CustomerID number(4), FirstName varchar2(20), LastName varchar2(20), Age number(3), CONSTRAINT fk_CustomerID_t FOREIGN KEY (CustomerID) References Customer(CustomerID), CONSTRAINT pk_ravelingwith PRIMARY KEY (TravelingWithID, CustomerID));
CREATE TABLE BookedTour( BookedTourID number(4) PRIMARY KEY, PurchaseDate date, TravelDate date, TotalPrice number(6), TourID number(4), DriverLicense varchar2(12), LicensePlate varchar2(7), CustomerID number(6), CONSTRAINT fktourIDbt FOREIGN KEY(TourID) References Tour(TourID), CONSTRAINT fkdriverlicensebt FOREIGN KEY( DriverLicense)  References Guide(DriverLicense), CONSTRAINT fklicenseplatebt FOREIGN KEY(LicensePlate)  References Vehicle(LicensePlate), CONSTRAINT fkcustomerIDbt  FOREIGN KEY(CustomerID)  References Customer(CustomerID));

/* part2
*/

INSERT INTO Customer VALUES(1,'Faith','Watson','3682 Route 2, Pasadena, MD 21122',8442340421,38);
INSERT INTO Customer VALUES(2,'Michelle','Hill','1716 Pearl Streat, Adrian, MI 49221', 8110913605,53);
INSERT INTO Customer VALUES(3,'Victor','Murray','6711 Jame Street, Ontario, CA 91762',8222077392,20);
INSERT INTO Customer VALUES(4,'Robert','Robertson','9272 Valley Drive, Clarksville, TN 37040',8334142990,72);
INSERT INTO Customer VALUES(5,'John','Lee','593 Laurel Street, Hyde Park, MA 02136',8999135294,41);
Insert INTO Customer Values(6, 'Andrew','August', '1 Dean Street',1234567890,30);
Insert INTO Customer Values(7, 'Betty','Brown', '7 Main Street',1234567891,40);
Insert INTO Customer Values(8, 'Chuck','Cheese', '14 Salsa Ave',1234567892,50);
Insert INTO Customer Values(9, 'Dillan','Doore', '4 Dean Street',1234567893,60);
Insert INTO Customer Values(10, 'Eliza','Evern', '5 Dean Street',1234567894,70);


INSERT INTO Travelingwith VALUES(11,(SELECT CustomerID FROM Customer Where CustomerID=1),'Kathy','Clark',20);
INSERT INTO Travelingwith VALUES(12,(SELECT CustomerID FROM Customer Where CustomerID=1),'Joshua','Lewis',10);
INSERT INTO Travelingwith VALUES(13,(SELECT CustomerID FROM Customer Where CustomerID=1),'Jimmy','Kelly',35);
INSERT INTO Travelingwith VALUES(14,(SELECT CustomerID FROM Customer Where CustomerID=1),'John','Reed',8);
INSERT INTO Travelingwith VALUES(15,(SELECT CustomerID FROM Customer Where CustomerID=1),'Frank','Bell',34);
INSERT INTO Travelingwith VALUES(21,(SELECT CustomerID FROM Customer Where CustomerID=2),'Betty','Collins',12);
INSERT INTO Travelingwith VALUES(22,(SELECT CustomerID FROM Customer Where CustomerID=2),'Chris','Cook',24);
INSERT INTO Travelingwith VALUES(23,(SELECT CustomerID FROM Customer Where CustomerID=2),'Brenda','Baker',50);
INSERT INTO Travelingwith VALUES(31,(SELECT CustomerID FROM Customer Where CustomerID=3),'Wanda','Bailey',19);
INSERT INTO Travelingwith VALUES(32,(SELECT CustomerID FROM Customer Where CustomerID=3),'Juan','Young',17);
INSERT INTO Travelingwith VALUES(33,(SELECT CustomerID FROM Customer Where CustomerID=3),'Jack','Martin',20);
INSERT INTO Travelingwith VALUES(41,(SELECT CustomerID FROM Customer Where CustomerID=4),'Ann','Torres',69);
INSERT INTO Travelingwith VALUES(42,(SELECT CustomerID FROM Customer Where CustomerID=4),'Donna','Lewis',15);
INSERT INTO Travelingwith VALUES(51,(SELECT CustomerID FROM Customer Where CustomerID=5),'Tina','Powell',45);
INSERT INTO Travelingwith VALUES(52,(SELECT CustomerID FROM Customer Where CustomerID=5),'Alice','Perry',18);
INSERT INTO Travelingwith VALUES(61,(SELECT CustomerID FROM Customer Where CustomerID=6),'AA','A',20);
INSERT INTO Travelingwith VALUES(62,(SELECT CustomerID FROM Customer Where CustomerID=6),'AB','A',10);
INSERT INTO Travelingwith VALUES(63,(SELECT CustomerID FROM Customer Where CustomerID=6),'AC','A',35);
INSERT INTO Travelingwith VALUES(64,(SELECT CustomerID FROM Customer Where CustomerID=6),'AD','A',8);
INSERT INTO Travelingwith VALUES(65,(SELECT CustomerID FROM Customer Where CustomerID=6),'AE','A',34);
INSERT INTO Travelingwith VALUES(66,(SELECT CustomerID FROM Customer Where CustomerID=6),'AF','A',20);
INSERT INTO Travelingwith VALUES(67,(SELECT CustomerID FROM Customer Where CustomerID=6),'AG','A',10);
INSERT INTO Travelingwith VALUES(68,(SELECT CustomerID FROM Customer Where CustomerID=6),'AH','A',35);
INSERT INTO Travelingwith VALUES(69,(SELECT CustomerID FROM Customer Where CustomerID=6),'AI','A',8);
INSERT INTO Travelingwith VALUES(610,(SELECT CustomerID FROM Customer Where CustomerID=6),'AJ','A',34);
INSERT INTO Travelingwith VALUES(71,(SELECT CustomerID FROM Customer Where CustomerID=7),'BA','B',12);
INSERT INTO Travelingwith VALUES(72,(SELECT CustomerID FROM Customer Where CustomerID=7),'BB','B',24);
INSERT INTO Travelingwith VALUES(73,(SELECT CustomerID FROM Customer Where CustomerID=7),'BC','B',50);
INSERT INTO Travelingwith VALUES(81,(SELECT CustomerID FROM Customer Where CustomerID=8),'CA','C',19);
INSERT INTO Travelingwith VALUES(82,(SELECT CustomerID FROM Customer Where CustomerID=8),'CB','C',17);
INSERT INTO Travelingwith VALUES(83,(SELECT CustomerID FROM Customer Where CustomerID=8),'CC','C',20);
INSERT INTO Travelingwith VALUES(91,(SELECT CustomerID FROM Customer Where CustomerID=9),'DA','D',69);
INSERT INTO Travelingwith VALUES(92,(SELECT CustomerID FROM Customer Where CustomerID=9),'DB','D',15);
INSERT INTO Travelingwith VALUES(101,(SELECT CustomerID FROM Customer Where CustomerID=10),'EA','E',45);
INSERT INTO Travelingwith VALUES(102,(SELECT CustomerID FROM Customer Where CustomerID=10),'EB','E',18);


INSERT INTO Guide VALUES ('019624722','Justin','Bieber',8992243779,'car','junior guide', 35000,'04-MAR-15');
INSERT INTO Guide VALUES('6875262','Tyler','Pratt',8227521008,'bus','senior guide', 70000, '10-DEC-06');
INSERT INTO Guide VALUES('024464525','Amy','Stewart',8553023358,'amphibious','guide',50000,'23-JUN-23');
INSERT INTO Guide VALUES('022348277','Owen','Hudson',8448976043,'bus','guide',53000,'12-APR-13');
INSERT INTO Guide VALUES('SMITH796ME','John','Smith',8559731327,'car','senior guide',68000,'01-JAN-09');
INSERT INTO Guide VALUES('023686313','Sasha','Smith',96323780,'amphibious', 'guide', 34000,'18-MAR-15');
INSERT INTO Guide VALUES('7459727','Matt','Wilson',894721653,'car','senior guide', 60000, '11-DEC-06');
INSERT INTO Guide VALUES('895326895','Lily','Wilkins',8567026853,'amphibious','guide',50000,'23-JUN-12');
INSERT INTO Guide VALUES('465864379','Fiona','Rees',8757566043,'bus','guide',51000,'13-APR-13');
INSERT INTO Guide VALUES('358684324','Theresa','Langdon',8683491327,'car','senior guide',69000,'25-JAN-09');


INSERT INTO Vehicle VALUES('73ZL46','bus','Ford','7024',2010,50);
INSERT INTO Vehicle VALUES('14AD51','amphibious','Jeep','qa24',2003,7);
INSERT INTO Vehicle VALUES('61DL26','car','Honda','xs53',2015,5);
INSERT INTO Vehicle VALUES('93EM22','bus','Toyota','24sa',2012,43);
INSERT INTO Vehicle VALUES('22DE66','car','Cadillac','oc05',2008,7);
INSERT INTO Vehicle VALUES('93RL29','bus','Kia','29io',2009,50);
INSERT INTO Vehicle VALUES('89Y2V1','amphibious','Jeep','le37',2010,27);
INSERT INTO Vehicle VALUES('68HN3L','car','Tesla','bn21',2015,4);
INSERT INTO Vehicle VALUES('93TOY2','bus','Toyota','23rx',2011,43);
INSERT INTO Vehicle VALUES('2DACI23','car','Cadillac','78vk',2006,7);


INSERT INTO Tour VALUES(1,'tour1','East Coast Tour','Boston','MA',12,'bus',350,150);
INSERT INTO Tour VALUES(2,'tour2','West Coast Tour','Los ANgeles','CA',5,'amphibious',500,200);
INSERT INTO Tour VALUES(3,'tour3','Florida Tour','Miami','FL',10,'car',400,175);
INSERT INTO Tour VALUES(4,'tour4','Canada Tour','New York','NY',7,'car',200,100);
Insert INTO Tour Values(5,'tour5','California Food Tour','Los Angeles','CA',30,'bus',500,200);
Insert INTO Tour Values(6,'tour6','Boston Historic tour','Boston','NC',30,'car',700,300);
Insert INTO Tour Values(7,'tour7','Virginia Tour','Richmond','VA',30,'amphibious',500,250);
Insert INTO Tour Values(8,'tour8','Washington DC Tour','DC','MY',30,'bus',400,250);


INSERT INTO Location VALUES(11,'Museum of Fine Arts','museum','465 Huntington Ave, Boston, MA 02115',42.3392,-71.0942);
INSERT INTO Location VALUES(12,'Symphony Hall','historic','301 Massachusetts Ave, Boston, MA 02115',42.3429,-71.0856);
INSERT INTO Location VALUES(21,'Museum of Contemporary Art','museum','250 S Grand Ave, Los Angeles, CA 90012',34.0534,-118.2504);
INSERT INTO Location VALUES(22,'Mr.Chow','restaurant','344 N Camden Dr, Beverly Hills, CA 90210',34.0681,-118.4031);
INSERT INTO Location VALUES(23,'Los Angeles County Museum of Art','museum','5905 Wilshire Blvd, Los Angeles, CA 90036',34.0637,-118.3588);
INSERT INTO Location VALUES(24,'Hilltown Ferry State Historic Landmark','historic','1416 9th St, Sacramento, CA 95814',38.5755,-121.4976);
INSERT INTO Location VALUES(25,'N/Naka','restaurant','3455 Overland Ave, Los Angeles, CA 90034',34.0251,-118.4122);
INSERT INTO Location VALUES(31,'Miami Museum of Science','museum','3280 S Miami Ave, Miami, FL 33133',25.7475,-80.2120);
INSERT INTO Location VALUES(32,'Planet Sushi','restaurant','860 Washiongton Ave, Miami Beach, FL 33139', 25.7792,-80.1333);
INSERT INTO Location VALUES(33,'Coconut Grove','historic','2985 S Bayshore Dr, Miami, FL 33133',25.7269,-80.1333);
INSERT INTO Location VALUES(41,'Royal Ontario Museum','museum','100 Queens Park, Toronto, ON M5S 2C6, Canada',43.6676,-79.3946);
INSERT INTO Location VALUES(42,'touqe','restaurant','900 Place Jean-Paul-Propelle, Montreal, QC H2Z 2B2, Canada',45.5032,-73.5614);
INSERT INTO Location VALUES(43,'Golden Gate Bridge','museum','45 Oak Ave, Boston, MA 02115',42.2315,-70.0913);
INSERT INTO Location VALUES(44,'Boston History Museum','historic','231 Concord Ave, Boston, MA 02115',40.3500,-71.0856);
INSERT INTO Location VALUES(45,'Museum of Modern Art','museum','52 N Grande Ave, Los Angeles, CA 90012',34.0537,-120.2304);
INSERT INTO Location VALUES(46,'Dragon Dynasty','restaurant','324 N Elm Dr, Beverly Hills, CA 90210',32.0928,-117.4736);
INSERT INTO Location VALUES(47,'Los Angeles  Museum of Science','museum','975 ViewBlvd, Los Angeles, CA 90036',33.9035,-119.7293);
INSERT INTO Location VALUES(48,'Coal Miners Landmark','historic','411 7th St, Richmond, VA 23219',40.5235,-121.4974);
INSERT INTO Location VALUES(49,'SushiRUs','restaurant','5543 Underland Ave, Richmond, VA 23220',35.1250,-117.4125);
INSERT INTO Location VALUES(50,'Miami Museum of Palm Trees','museum','321 Washington Cedar Ave, Washington DC, DC 20001',38.9575,78.2020);
INSERT INTO Location VALUES(51,'Cheesecake Factory','restaurant','68 Lake Ave, Miami Beach, Washington DC, DC 20001', 37.6792,80.1350);
INSERT INTO Location VALUES(52,'Coconut Avenue','historic','2985 S Eastside Dr, Washington DC, DC 20003',38.7200,77.1300);
INSERT INTO Location VALUES(53,'History Of Toronto  Museum','museum','100 Kings Park, Toronto, ON M5S 2C6, Canada',43.6506,-79.3900);
INSERT INTO Location VALUES(54,'The Rouge','restaurant','900 Main Palace, Richmond, VA 23220',40.5035,-121.5684);

INSERT INTO Traveling VALUES((SELECT TourID FROM Tour Where TourID=1),(SELECT LocationID FROM Location WHERE LocationID=11));
INSERT INTO Traveling VALUES((SELECT TourID FROM Tour Where TourID=1),(SELECT LocationID FROM Location WHERE LocationID=12));
INSERT INTO Traveling VALUES((SELECT TourID FROM Tour Where TourID=2),(SELECT LocationID FROM Location WHERE LocationID=21));
INSERT INTO Traveling VALUES((SELECT TourID FROM Tour Where TourID=2),(SELECT LocationID FROM Location WHERE LocationID=22));
INSERT INTO Traveling VALUES((SELECT TourID FROM Tour Where TourID=2),(SELECT LocationID FROM Location WHERE LocationID=23));
INSERT INTO Traveling VALUES((SELECT TourID FROM Tour Where TourID=2),(SELECT LocationID FROM Location WHERE LocationID=24));
INSERT INTO Traveling VALUES((SELECT TourID FROM Tour Where TourID=2),(SELECT LocationID FROM Location WHERE LocationID=25));
INSERT INTO Traveling VALUES((SELECT TourID FROM Tour Where TourID=3),(SELECT LocationID FROM Location WHERE LocationID=31));
INSERT INTO Traveling VALUES((SELECT TourID FROM Tour Where TourID=3),(SELECT LocationID FROM Location WHERE LocationID=32));
INSERT INTO Traveling VALUES((SELECT TourID FROM Tour Where TourID=3),(SELECT LocationID FROM Location WHERE LocationID=33));
INSERT INTO Traveling VALUES((SELECT TourID FROM Tour Where TourID=4),(SELECT LocationID FROM Location WHERE LocationID=41));
INSERT INTO Traveling VALUES((SELECT TourID FROM Tour Where TourID=4),(SELECT LocationID FROM Location WHERE LocationID=42));
INSERT INTO Traveling VALUES((SELECT TourID FROM Tour Where TourID=5),(SELECT LocationID FROM Location WHERE LocationID=46));
INSERT INTO Traveling VALUES((SELECT TourID FROM Tour Where TourID=5),(SELECT LocationID FROM Location WHERE LocationID=22));
INSERT INTO Traveling VALUES((SELECT TourID FROM Tour Where TourID=5),(SELECT LocationID FROM Location WHERE LocationID=25));
INSERT INTO Traveling VALUES((SELECT TourID FROM Tour Where TourID=6),(SELECT LocationID FROM Location WHERE LocationID=12));
INSERT INTO Traveling VALUES((SELECT TourID FROM Tour Where TourID=6),(SELECT LocationID FROM Location WHERE LocationID=43));
INSERT INTO Traveling VALUES((SELECT TourID FROM Tour Where TourID=6),(SELECT LocationID FROM Location WHERE LocationID=11));
INSERT INTO Traveling VALUES((SELECT TourID FROM Tour Where TourID=7),(SELECT LocationID FROM Location WHERE LocationID=48));
INSERT INTO Traveling VALUES((SELECT TourID FROM Tour Where TourID=7),(SELECT LocationID FROM Location WHERE LocationID=49));
INSERT INTO Traveling VALUES((SELECT TourID FROM Tour Where TourID=7),(SELECT LocationID FROM Location WHERE LocationID=54));
INSERT INTO Traveling VALUES((SELECT TourID FROM Tour Where TourID=8),(SELECT LocationID FROM Location WHERE LocationID=50));
INSERT INTO Traveling VALUES((SELECT TourID FROM Tour Where TourID=8),(SELECT LocationID FROM Location WHERE LocationID=51));
INSERT INTO Traveling VALUES((SELECT TourID FROM Tour Where TourID=8),(SELECT LocationID FROM Location WHERE LocationID=52));

INSERT INTO BookedTour VALUES(1,'3-FEB-15','6-MAY-15',1700,(SELECT TourID FROM Tour Where TourID=1),(SELECT Driverlicense FROM Guide WHERE Driverlicense='6875262'),(SELECT Licenseplate FROM Vehicle WHERE LicensePlate='73ZL46'),(SELECT CustomerID FROM Customer WHERE CustomerID=1));
INSERT INTO BookedTour VALUES(2,'2-JAN-13','4-MAR-13',1700,(SELECT TourID FROM Tour Where TourID=2),(SELECT Driverlicense FROM Guide Where Driverlicense='024464525'),(SELECT Licenseplate FROM Vehicle WHERE LicensePlate='14AD51'),(SELECT CustomerID FROM Customer WHERE CustomerID=2));
INSERT INTO BookedTour VALUES(3,'5-APR-14','6-MAY-14',1600,(SELECT TourID FROM Tour Where TourID=3),(SELECT Driverlicense FROM Guide Where Driverlicense='SMITH796ME'),(SELECT Licenseplate FROM Vehicle WHERE LicensePlate='61DL26'),(SELECT CustomerID FROM Customer WHERE CustomerID=3));
INSERT INTO BookedTour VALUES(4,'2-JAN-12','4-MAR-12',500,(SELECT TourID FROM Tour Where TourID=4),(SELECT Driverlicense FROM Guide Where Driverlicense='019624722'),(SELECT Licenseplate FROM Vehicle WHERE LicensePlate='22DE66'),(SELECT CustomerID FROM Customer WHERE CustomerID=4));
INSERT INTO BookedTour VALUES(5,'2-JAN-13','4-MAR-13',1500,(SELECT TourID FROM Tour Where TourID=2),(SELECT Driverlicense FROM Guide Where Driverlicense='024464525'),(SELECT Licenseplate FROM Vehicle WHERE LicensePlate='14AD51'),(SELECT CustomerID FROM Customer WHERE CustomerID=5));
INSERT INTO BookedTour VALUES(6,'2-JAN-12','4-MAR-12',600,(SELECT TourID FROM Tour Where TourID=4),(SELECT Driverlicense FROM Guide Where Driverlicense='019624722'),(SELECT Licenseplate FROM Vehicle WHERE LicensePlate='22DE66'),(SELECT CustomerID FROM Customer WHERE CustomerID=5));
INSERT INTO BookedTour VALUES(7,'3-MAY-16','6-MAY-16',3000,(SELECT TourID FROM Tour Where TourID=4),(SELECT Driverlicense FROM Guide WHERE Driverlicense='023686313'),(SELECT Licenseplate FROM Vehicle WHERE LicensePlate='14AD51'),(SELECT CustomerID FROM Customer WHERE CustomerID=6));
INSERT INTO BookedTour VALUES(8,'2-JAN-13','4-MAR-13',2500,(SELECT TourID FROM Tour Where TourID=5),(SELECT Driverlicense FROM Guide Where Driverlicense='7459727'),(SELECT Licenseplate FROM Vehicle WHERE LicensePlate='2DACI123'),(SELECT CustomerID FROM Customer WHERE CustomerID=7));
INSERT INTO BookedTour VALUES(9,'5-APR-14','6-MAY-14',1900,(SELECT TourID FROM Tour Where TourID=6),(SELECT Driverlicense FROM Guide Where Driverlicense='895326895'),(SELECT Licenseplate FROM Vehicle WHERE LicensePlate='89Y2V1'),(SELECT CustomerID FROM Customer WHERE CustomerID=8));
INSERT INTO BookedTour VALUES(10,'2-JAN-12','4-MAR-12',2000,(SELECT TourID FROM Tour Where TourID=7),(SELECT Driverlicense FROM Guide Where Driverlicense='465864379'),(SELECT Licenseplate FROM Vehicle WHERE LicensePlate='93EM22'),(SELECT CustomerID FROM Customer WHERE CustomerID=8));
INSERT INTO BookedTour VALUES(11,'2-JAN-13','4-FEB-13',700,(SELECT TourID FROM Tour Where TourID=8),(SELECT Driverlicense FROM Guide Where Driverlicense='358684324'),(SELECT Licenseplate FROM Vehicle WHERE LicensePlate='68HN3L'),(SELECT CustomerID FROM Customer WHERE CustomerID=9));
INSERT INTO BookedTour VALUES(12,'2-JAN-12','4-FEB-12',900,(SELECT TourID FROM Tour Where TourID=7),(SELECT Driverlicense FROM Guide Where Driverlicense='023686313'),(SELECT Licenseplate FROM Vehicle WHERE LicensePlate='14AD51'),(SELECT CustomerID FROM Customer WHERE CustomerID=10));

/* The view 
*/
CREATE or REPLACE VIEW LargeGroup AS
(SELECT B.BookedTourID, B.TravelDate, 'Regular' AS GroupSize
FROM BookedTour B, 
(SELECT CustomerID, COUNT(*)+1 AS GSize
 FROM TravelingWith 
 GROUP BY CustomerID) A
WHERE B.CustomerID = A.CustomerID AND A.GSize < 10)
UNION
(SELECT B.BookedTourID, B.TravelDate, 'Large' AS GroupSize
FROM BookedTour B, 
(SELECT CustomerID, COUNT(*)+1 AS GSize
 FROM TravelingWith 
 GROUP BY CustomerID) A
WHERE B.CustomerID = A.CustomerID AND A.GSize >= 10);

/* show the view
*/
SELECT * FROM LargeGroup;

/* Report BookedTour Consist of large group with date after march 1 2016 */
SELECT * 
FROM LargeGroup L
WHERE L.TravelDate >= to_date('01-03-16','DD-MM-YY') 
AND L.GroupSize = 'Large';










/* part2
*/

/*1 
Senior Guide must have a salary of at least 50000
*/
CREATE or Replace Trigger seniorsa 
Before Insert or Update on Guide
For Each Row

When(new.Title = 'senior guide')
BEGIN
	if(:new.Salary < 50000) Then
	RAISE_APPLICATION_ERROR(-20000,'senior guide dont have the enough salary');
	END IF;
END;
/

/*2
Buses must be made in 2010 or newer
*/
CREATE or Replace Trigger busyear
Before Insert or Update on Vehicle
For Each Row

When(new.VehicleType = 'bus')
BEGIN
	if(:new.Year < 2010) Then
	RAISE_APPLICATION_ERROR(-20000,'bus is too old');
	END IF;
END;
/

/*3
Tours in the city of Boston requires amphibious
*/
CREATE or Replace Trigger bosvehicle
Before Insert or Update on Tour
For Each Row

when(new.City = 'Boston')
BEGIN
	if(:new.VehicleType != 'amphibious') Then
	RAISE_APPLICATION_ERROR(-20000,'Boston trip need amphibious vehicle');
	END IF;
END;
/

/*4
Create bookedtour the purchase date of 3 month after the creation date
*/
CREATE or Replace Trigger purchasedate
Before Insert on BookedTour
For Each Row
Declare
	temp date;
BEGIN
	Select add_months(sysdate,3) into temp from dual;
	:new.PurchaseDate := temp;
END;
/

/*5
A tour guide cannot be assigned to more than one booked tour in the same day
*/
CREATE or Replace Trigger tourguide
After Insert or Update on BookedTour
For Each Row
Declare
	PRAGMA AUTONOMOUS_TRANSACTION;
	num NUMBER;

BEGIN
	if(Inserting) Then
	SELECT Count(*) INTO num from BookedTour Where DriverLicense = :new.DriverLicense AND TravelDate = :new.TravelDate;
	if num > 0 Then
	RAISE_APPLICATION_ERROR(-20000,'Tour guide has more than one tour in the same day');
	END IF;
	END IF;

	if(Updating AND :new.DriverLicense != :old.DriverLicense) Then
	SELECT Count(*) INTO num from BookedTour Where DriverLicense = :new.DriverLicense AND TravelDate = :new.TravelDate;
	if num > 0 Then
	RAISE_APPLICATION_ERROR(-20000,'Tour guide has more than one tour in the same day');
	END IF;
	END IF;
	COMMIT;
END;
/


