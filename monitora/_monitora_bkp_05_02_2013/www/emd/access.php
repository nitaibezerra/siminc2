<?php
//
//				$query = "SELECT * FROM orzare_gnd";
//				$rs = odbc_exec($myDB, $query);
//				while($row = odbc_fetch_row($rs)) {
//					dbg($row);
//				}
//				dbg($rs);

				
// - This is a complete working dynamic example of using:
//    odbc_connect, odbc_exec, getting col Names,
//    odbc_fetch_row and no of rows. hope it helps
// - your driver should point to your MS access file


$conn = odbc_connect('emdcamara','admin','');


while($row = @odbc_data_source($conn, SQL_FETCH_NEXT)) var_dump($row);

$nrows=0;

if ($conn)
{
$sql =  "select cod_gnd from orzare_gnd";
//this function will execute the sql satament
$result=odbc_exec($conn, $sql);

echo "<table  align=\"center\" border=\"1\" borderColor=\"\" cellpadding=\"0\" cellspacing=\"0\">\n";
echo "<tr> ";
// -- print field name
$colName = odbc_num_fields($result);
for ($j=1; $j<= $colName; $j++)
{
echo "<th  align=\"left\" bgcolor=\"#CCCCCC\" > <font color=\"#990000\"> ";
echo odbc_field_name ($result, $j );
echo "</font> </th>";
}
$j=$j-1;
$c=0;
// end of field names
while(odbc_fetch_row($result)) // getting data
{
 $c=$c+1;
 if ( $c%2 == 0 )
 echo "<tr bgcolor=\"#d0d0d0\" >\n";
 else
 echo "<tr bgcolor=\"#eeeeee\">\n";
   for($i=1;$i<=odbc_num_fields($result);$i++)
     {       
       echo "<td>";
       echo odbc_result($result,$i);
       echo "</td>";       
       if ( $i%$j == 0 ) 
           {
           $nrows+=1; // counting no of rows   
         } 
     }
   echo "</tr>";
}

echo "</td> </tr>\n";
echo "</table >\n";
// --end of table 
if ($nrows==0) echo "<br/><center> Nothing for $month yet! Try back later</center>  <br/>";
else echo "<br/><center> Total Records:  $nrows </center>  <br/>";
odbc_close ($conn);

}
else echo "odbc not connected <br>";				
				
?>