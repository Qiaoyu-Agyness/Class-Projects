/* Database Project1 Phase2
	Qiaoyu Liao qliao@wpi.edu
	Trivani Shahi tjshahi@wpi.edu
/* part 1
*/
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

INSERT INTO Guide VALUES ('019624722','Justin','Bieber',8992243779,'car','junior guide', 35000,'04-MAR-15');
INSERT INTO Guide VALUES('6875262','Tyler','Pratt',8227521008,'bus','senior guide', 70000, '10-DEC-06');
INSERT INTO Guide VALUES('024464525','Amy','Stewart',8553023358,'amphibious','guide',50000,'23-JUN-23');
INSERT INTO Guide VALUES('022348277','Owen','Hudson',8448976043,'bus','guide',53000,'12-APR-13');
INSERT INTO Guide VALUES('SMITH796ME','John','Smith',8559731327,'car','senior guide',68000,'01-JAN-09');

INSERT INTO Vehicle VALUES('73ZL46','bus','Ford','7024',2010,50);
INSERT INTO Vehicle VALUES('14AD51','amphibious','Jeep','qa24',2003,7);
INSERT INTO Vehicle VALUES('61DL26','car','Honda','xs53',2015,5);
INSERT INTO Vehicle VALUES('93EM22','bus','Toyota','24sa',2012,43);
INSERT INTO Vehicle VALUES('22DE66','car','Cadillac','oc05',2008,7);

INSERT INTO Tour VALUES(1,'tour1','East Coast Tour','Boston','MA',12,'bus',350,150);
INSERT INTO Tour VALUES(2,'tour2','West Coast Tour','Los ANgeles','CA',5,'amphibious',500,200);
INSERT INTO Tour VALUES(3,'tour3','Florida Tour','Miami','FL',10,'car',400,175);
INSERT INTO Tour VALUES(4,'tour4','Canada Tour','New York','NY',7,'car',200,100);

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

INSERT INTO BookedTour VALUES(1,'3-FEB-15','6-MAY-15',1700,(SELECT TourID FROM Tour Where TourID=1),(SELECT Driverlicense FROM Guide WHERE Driverlicense='6875262'),(SELECT Licenseplate FROM Vehicle WHERE LicensePlate='73ZL46'),(SELECT CustomerID FROM Customer WHERE CustomerID=1));
INSERT INTO BookedTour VALUES(2,'2-JAN-13','4-MAR-13',1700,(SELECT TourID FROM Tour Where TourID=2),(SELECT Driverlicense FROM Guide Where Driverlicense='024464525'),(SELECT Licenseplate FROM Vehicle WHERE LicensePlate='14AD51'),(SELECT CustomerID FROM Customer WHERE CustomerID=2));
INSERT INTO BookedTour VALUES(3,'5-APR-14','6-MAY-14',1600,(SELECT TourID FROM Tour Where TourID=3),(SELECT Driverlicense FROM Guide Where Driverlicense='SMITH796ME'),(SELECT Licenseplate FROM Vehicle WHERE LicensePlate='61DL26'),(SELECT CustomerID FROM Customer WHERE CustomerID=3));
INSERT INTO BookedTour VALUES(4,'2-JAN-12','4-MAR-12',500,(SELECT TourID FROM Tour Where TourID=4),(SELECT Driverlicense FROM Guide Where Driverlicense='019624722'),(SELECT Licenseplate FROM Vehicle WHERE LicensePlate='22DE66'),(SELECT CustomerID FROM Customer WHERE CustomerID=4));
INSERT INTO BookedTour VALUES(5,'2-JAN-13','4-MAR-13',1500,(SELECT TourID FROM Tour Where TourID=2),(SELECT Driverlicense FROM Guide Where Driverlicense='024464525'),(SELECT Licenseplate FROM Vehicle WHERE LicensePlate='14AD51'),(SELECT CustomerID FROM Customer WHERE CustomerID=5));
INSERT INTO BookedTour VALUES(6,'2-JAN-12','4-MAR-12',600,(SELECT TourID FROM Tour Where TourID=4),(SELECT Driverlicense FROM Guide Where Driverlicense='019624722'),(SELECT Licenseplate FROM Vehicle WHERE LicensePlate='22DE66'),(SELECT CustomerID FROM Customer WHERE CustomerID=5));


/* part3
*/

/* Q1 Report the vehicle that can carry more than 10 passengers
*/
SELECT * 
FROM Vehicle 
WHERE MaxPassenger >= 10;

/* Q2 For a given tour guide (in this case DriverLicense = 019624722), report which of the 5 company vehicles that the tour guide can drive
*/
SELECT *
FROM  Guide G, Vehicle V
WHERE  G.DriverLicense = '019624722'                                     	
AND G.Vehicletype = V.Vehicletype;

/* Q3 Report the number of booked tours each customer has. Also include the FirstName, LastName, and Age
*/
SELECT FirstName, LastName, Age, COUNT(*) AS BookedTourCOUNT
FROM Customer C, BookedTour B
WHERE C.CustomerID = B.CustomerID
GROUP BY FirstName, LastName, Age;


/* Q4 Report the number of booked tours to which each tour guide has been assigned. Also include the FirstName, LastName, and Title of the tour guide.
*/
SELECT FirstName, LastName, Title, COUNT(*) AS BookedTourCOUNT
FROM Guide G, BookedTour B: Report the number of each location type (historic, museum, restaurant) along with the tour name that each tour containsReport the number of booked tours each customer has. Also include the FirstName, LastName, and Age
*/
SELECT TourName, Type, COUNT(*) AS COUNT
FROM Tour T, Location L, Traveling TR
WHERE T.TourID = TR.TourID AND L.LocationID = TR.LocationID
GROUP BY Type, TourName
ORDER BY TourName, Type;

/* Q6 Report the number of people taking each of the different tours. Include the TourName, City, State and VehicleType
*/
SELECT TourName, City, State, VehicleType, SUM(CT.COUNTC) AS COUNT
FROM Tour, BookedTour,
    (SELECT C.CustomerID, COUNT(*)+1 AS COUNTC
    FROM Customer C, TravelingWith T
    WHERE C.CustomerID = T.CustomerID
    GROUP BY C.CustomerID) CT
WHERE Tour.TourID = BookedTour.TourID
AND BookedTour.CustomerID = CT.CustomerID
GROUP BY TourName, City, State, VehicleType
ORDER BY TourName;


/* Q7 Report primary customers who have booked tours in 2015 and 2016
*/
SELECT C.CustomerID, C.FirstName, C.LastName, B.PurchaseDate
FROM Customer C, BookedTour B
WHERE C.CustomerID = B.CustomerID 
AND B.PurchaseDate >= to_date('01-01-15','DD-MM-YY') 
AND B.PurchaseDate <= to_date('30-12-16','DD-MM-YY');

/* Q8 Report which is the most popular tour, i.e. the tour that has been booked the most often
*/
SELECT T.TourName,  Count(*)
FROM Tour T, BookedTour B
WHERE T.TourID = B.TourID
GROUP BY T.TourName
HAVING COUNT(*)=(SELECT MAX(COUNT(*)) FROM BookedTour T1 Group BY TourID);


/* Q9 Report the total number of tours taken by people 
   this one count how many people are in each booked tour and sum the number of all the bookedtour together
*/
SELECT SUM(CT.COUNTC) AS COUNT
FROM BookedTour B,
   (SELECT C.CustomerID, COUNT(*)+1 AS COUNTC
   FROM Customer C, TravelingWith T
   WHERE C.CustomerID = T.CustomerID
   GROUP BY C.CustomerID) CT
WHERE B.CustomerID = CT.CustomerID ;

/* Q10 For a specific booked tour, insert the TotalPrice by calculating $100 per adult ( >= 18 yrs) and $50 per child (<18 yrs).
   I chose the booketour1 in this case
*/
DECLARE
a NUMBER;
b NUMBER;
CURSOR c1 IS
SELECT COUNT(Age)*100 AS APrice 
FROM
((SELECT Age
 FROM BookedTour, Customer 
 Where BookedTour.BookedTourID = 1 
 AND BookedTour.CustomerID = Customer.CustomerID) 
Union 
(SELECT T.Age 
FROM BookedTour, Customer, TravelingWith T 
WHERE BookedTour.BookedTourID = 1 
AND BookedTour.CustomerID = Customer.CustomerID 
AND BookedTour.CustomerID = T.CustomerID))G
WHERE Age >= 18;

CURSOR c2 IS
SELECT COUNT(Age)*50 AS APrice FROM
((SELECT Age  
FROM BookedTour, Customer  
Where BookedTour.BookedTourID = 1 
AND BookedTour.CustomerID = Customer.CustomerID) 
Union 
(SELECT T.Age 
FROM BookedTour, Customer, TravelingWith T 
WHERE BookedTour.BookedTourID = 1 
AND BookedTour.CustomerID = Customer.CustomerID 
AND BookedTour.CustomerID = T.CustomerID))F
WHERE Age < 17;

BEGIN
open c1;
FETCH c1 into a;
open c2;
FETCH c2 into b;

UPDATE BookedTour SET TotalPrice = (a + b) WHERE bookedTourID = 1;
close c1;
close c2;
END;
/



/* Q10
*/