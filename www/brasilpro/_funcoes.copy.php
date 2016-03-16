<?php

//function criarcopia ( $inuid )
//{
	global $db;
	
	
	$pontuacao = $db->carregar("select * from cte.pontuacao where inuid = 27 ");
	
	$i = 0;
	foreach ( $pontuacao as $pontuacao1 )
	{
		echo $pontuacao1[$i]['ptoid'].'<br>';
	}
	
//}

?>