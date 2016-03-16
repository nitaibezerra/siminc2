<?php
/**
 * This Example Demostrates using Flat Files Communication with Cli Thread Calling
 * 
 * it does a simple task one thread is counting and the other is printing to file
 * 
 * it Consists of the Files:-
 * 
 * 1)Ex2_MainThread.php : The Main thread that watches two threads and prints to screen when they finish
 * 2)Ex2_Counter.php    : The Thread that Counts
 * 3)Ex2_Echoer.php     : The Thread That Prints to file
 * 
 * @package Examples
 * @subpackage Example2
 * @author Mohammed Yousef Bassyouni <harrrrpo@gmail.com>
 * @license http://opensource.org/licenses/gpl-license.php GNU Public License
 * @todo Fully Document the example
 */
require_once("../Handlers/Loader.php");
require_once("../Thread/CliThreader.php");
require_once("../Com/FileCom.php");
$fcom=new FileCom();
$P=new Process($fcom);
$s=new CliThread("Ex2_Echoer.php","",$P);
$e=new CliThread("Ex2_Counter.php","",$P);
$s->SetParams($s->GetID()." ".$fcom->UniqueName);
$e->SetParams($e->GetID()." ".$fcom->UniqueName);
$s->Go();
$e->Go();
CallerBasic::Join(array($s->GetID(),$e->GetID()),$fcom);
print $s->GetState()." S<br>";
print $e->GetState()." E";
$fcom->Clear();
?>