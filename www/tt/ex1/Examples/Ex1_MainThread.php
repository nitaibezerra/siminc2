<?php
/**
 * This Example Demostrates using Shared Memory Communication with Socket Thread Calling
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
 * @subpackage Example1
 * @author Mohammed Yousef Bassyouni <harrrrpo@gmail.com>
 * @license http://opensource.org/licenses/gpl-license.php GNU Public License
 * @todo Fully Document the example
 */
require_once("../Handlers/Loader.php");
require_once("../Thread/SocThreader.php");
require_once("../Com/ShmopCom.php");
$fcom=new ShmopCom();
$P=new Process($fcom);
$s=new SocThread("http://localhost/Threader_Dev/Examples/Ex1_Echoer.php","",$P);
$e=new SocThread("http://localhost/Threader_Dev/Examples/Ex1_Counter.php","",$P);
$s->SetParams("ID=".$s->GetID()."&xsh=".$fcom->MemKey);
$e->SetParams("ID=".$e->GetID()."&xsh=".$fcom->MemKey);
$s->Go();
$e->Go();
CallerBasic::Join(array($s->GetID(),$e->GetID()),$fcom);
print $s->GetState()." S<br>";
print $e->GetState()." E<br>";
$fcom->Clear();
?>