<?php
	$inner = '';
	if( $_REQUEST['modid'] )
	{
		$inner = 'inner join seguranca.moduloperfil mp ON mp.pflcod = p.pflcod and mp.modid = '.$_REQUEST['modid'].' ';
	}
	
	$restricao_par = "";
	
	if($sisid == 23)
	{
		$restricao_par = "and pflcod not in (1374, 1416)";
	}
	
	$sql = "select
				p.pflcod as codigo,
				p.pfldsc as descricao
			from
				seguranca.perfil p
			{$inner}
			where
				p.pflstatus='A'
				{$restricao_par}
				and p.pflnivel > 1 
				and p.sisid=". $sisid ."
			order by
				descricao";
			
	$db->monta_combo( "pflcod", $sql,  'S', "Selecione o perfil desejado", 'selecionar_perfil', '', '', '', 'S', 'pflcod', '', '', '', '', 'chosen-select' );
?>