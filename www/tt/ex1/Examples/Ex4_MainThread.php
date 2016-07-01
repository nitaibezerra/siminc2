<?php
/**
 * This Example Demostrates using MySQL Communication with Ajax Thread Calling
 * 
 * it does a simple task one thread is counting and the other is printing to file
 * 
 * it Consists of the Files:-
 * 
 * 1)Ex1_MainThread.php : The Main thread that watches two threads and prints to screen when they finish
 * 2)Ex1_Counter.php    : The Thread that Counts
 * 3)Ex1_Echoer.php     : The Thread That Prints to file
 * 
 * @package Examples
 * @subpackage Example4
 * @author Mohammed Yousef Bassyouni <harrrrpo@gmail.com>
 * @license http://opensource.org/licenses/gpl-license.php GNU Public License
 * @todo Fully Document the example
 */
require_once("../Handlers/Loader.php");
require_once("../Thread/AjThreader.php");
require_once("../Com/MyCom.php");
$fcom=new MyCom("","root","","webct");
$P=new Process($fcom);
$e=new AjThread("Ex4_Counter.php","",$P);
$s=new AjThread("Ex4_Echoer.php","",$P);

$s->SetParams("ID=".$s->GetID()."&tb=".$fcom->Table);
$e->SetParams("ID=".$e->GetID()."&tb=".$fcom->Table);
$s->Go();
$e->Go();
CallerBasic::Join(array($s->GetID(),$e->GetID()),$fcom);
//print $s->GetState()." S<br>";
//print $e->GetState()." E";
$fcom->Clear();
?>