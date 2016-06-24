<?php

ini_set("memory_limit","5024M");
set_time_limit(0);


//cancela demanda
include_once APPRAIZ . 'includes/workflow.php';

$sql = "select me.memid, me.docid, doc.esdid from pdeescola.memaiseducacao me
		inner join workflow.documento doc on doc.docid = me.docid and doc.esdid <> 32 and memanoreferencia = 2012
		where me.docid is not null and me.memclassificacaoescola = 'R' and me.memstatus = 'A'";

$dados = $db->carregar($sql);

$i=1;

if($dados){

	$cmddsc = "As escolas do campo deverão refazer o Plano de Atendimento devido à atualização das regras e atividades.";
	$dadosVerificacao = (array) "a:1:{s:6:\"unicod\";s:0:\"\";}";

	foreach($dados as $d){ 
		
		$memid = $d['memid'];
		$esdid = $d['esdid'];
		$docid = $d['docid'];
	
		switch($esdid) {
			case '35': 
				$aedid = 1221;
				break;
			case '31': 
				$aedid = 1222;
				break;
			case '36': 
				$aedid = 1223;
				break;
			case '33': 
				$aedid = 1224;
				break;
				
			default:
				$aedid = 0;
		}
	
		if($aedid != 0){
			wf_alterarEstado( $docid, $aedid, $cmddsc, $dadosVerificacao );
			
			echo $i .' - '. $memid .'<br>';
		}
		
		
	
		$i++;
		
	}
  	
}
		
unset($dados);

//email escolas rurais 2012
$sql = "--emails escolas rurais 2012
	select distinct e.entemail as email from pdeescola.memaiseducacao me
	inner join workflow.documento doc on doc.docid = me.docid 
	inner join entidade.entidade e on e.entid = me.entid and doc.esdid <> 32 and memanoreferencia = 2012
	where me.docid is not null 
	and me.memclassificacaoescola = 'R' 
	and me.memstatus = 'A'
	and e.entemail is not null";
$dados  = $db->carregar( $sql );


$i=1;
foreach($dados as $d){
	
	//envia email
	$remetente = array("email" => $_SESSION['email_sistema'], "nome" => "SIMEC - Mais Educação");
	$destinatario = $d['email'];
	
  	$assunto = "Atualização do Plano de Atendimento das escolas do campo";

  	$conteudo = "Para as escolas do campo, o Plano de Atendimento deverá ser refeito, devido a atualização das regras e atividades. <br><br> Atenciosamente: <br> Equipe Mais Educação.";

  	if($destinatario){
  		enviar_email( $remetente, $destinatario, $assunto, $conteudo, $emailCopia );
  	}
	
	echo $i .' - '. $d['email'] .'<br>';
	
	$i++;
	
}

unset($dados);

//email coordenadores das escolas 2012
$sql = "--emails coordenadores escolas rurais 2012
		select distinct ent.entemail as email
					from entidade.entidade ent
					inner join entidade.endereco ed on ed.entid = ent.entid
					INNER JOIN entidade.funcaoentidade fe on ed.entid = fe.entid 
					WHERE fe.fuestatus = 'A' and fe.funid in (96) and ent.entemail is not null
					and ed.muncod in (
					
							select distinct ed2.muncod from pdeescola.memaiseducacao me
							inner join workflow.documento doc on doc.docid = me.docid 
							inner join entidade.entidade e on e.entid = me.entid and doc.esdid <> 32 and memanoreferencia = 2012
							inner join entidade.endereco ed2 on ed2.entid = e.entid
							where me.docid is not null 
							and me.memclassificacaoescola = 'R' 
							and me.memstatus = 'A'
							
		)";
$dados  = $db->carregar( $sql );


$i=1;
foreach($dados as $d){
	
	//envia email
	$remetente = array("email" => $_SESSION['email_sistema'], "nome" => "SIMEC - Mais Educação");
	$destinatario = $d['email'];
	
  	$assunto = "Atualização do Plano de Atendimento das escolas do campo";

  	$conteudo = "Para as escolas do campo, o Plano de Atendimento deverá ser refeito, devido a atualização das regras e atividades. <br><br> Atenciosamente: <br> Equipe Mais Educação.";

  	if($destinatario){
  		enviar_email( $remetente, $destinatario, $assunto, $conteudo, $emailCopia );
  	}
	
	echo $i .' - '. $d['email'] .'<br>';
	
	$i++;
	
}
		
/*
//email escolas rurais 2011 e 2012
$sql = "-- pega email das escola rurais de 2011
		select distinct e.entemail as email
			FROM pdeescola.memaiseducacao m
			inner join entidade.entidade e on e.entid = m.entid
			left join entidade.endereco ed on ed.entid = e.entid 
			left join public.municipio mu on mu.muncod = ed.muncod
			where m.memstatus = 'A'
			  and m.memanoreferencia = 2012
			  and m.memclassificacaoescola = 'R'
			  and m.docid is not null
			  and e.entemail is not null
			  and m.entcodent in (
			     select entcodent from pdeescola.memaiseducacao 
			   where memstatus = 'A' 
			   and memanoreferencia = 2012
			   and memclassificacaoescola = 'R'
			   and entcodent in (select entcodent from pdeescola.memaiseducacao where memstatus = 'A'  and memanoreferencia = 2011 and memclassificacaoescola = 'R')
			 )
		
		union 
		-- pega email das escola rurais de 2012
		select distinct e.entemail as email
			FROM pdeescola.memaiseducacao m
			inner join entidade.entidade e on e.entid = m.entid
			left join entidade.endereco ed on ed.entid = e.entid 
			left join public.municipio mu on mu.muncod = ed.muncod
			where m.memstatus = 'A'
			  and m.memanoreferencia = 2012
			  and m.memclassificacaoescola = 'R'
			  and m.docid is not null
			  and e.entemail is not null
			  and m.entcodent not in (
			     select entcodent from pdeescola.memaiseducacao 
			   where memstatus = 'A' 
			   and memanoreferencia = 2012
			   and memclassificacaoescola = 'R'
			   and entcodent in (select entcodent from pdeescola.memaiseducacao where memstatus = 'A'  and memanoreferencia = 2011 and memclassificacaoescola = 'R')
			 )";
$dados  = $db->carregar( $sql );


$i=1;
foreach($dados as $d){
	
	//envia email
	$remetente = array("email" => $_SESSION['email_sistema'], "nome" => "SIMEC - Mais Educação");
	$destinatario = $d['email'];
	
  	$assunto = "Atualização do Plano de Atendimento das escolas do campo";

  	$conteudo = "Para as escolas do campo, o Plano de Atendimento deverá ser refeito, devido a atualização das regras e atividades. <br><br> Atenciosamente: <br> Equipe Mais Educação.";

  	if($destinatario){
  		enviar_email( $remetente, $destinatario, $assunto, $conteudo, $emailCopia );
  	}
	
	echo $i .' - '. $d['email'] .'<br>';
	
	$i++;
	
}


unset($dados);

//coordenadores municipais de escolas rurais 2011 e 2012
$sql = "--pega email dos coordenadores municipais das escolas rurais 2011	 
		select distinct ent.entemail as email
					from entidade.entidade ent
					inner join entidade.endereco ed on ed.entid = ent.entid
					INNER JOIN entidade.funcaoentidade fe on ed.entid = fe.entid 
					WHERE fe.fuestatus = 'A' and fe.funid in (96) and ent.entemail is not null
					and ed.muncod in (
					
							select distinct ed.muncod	
							FROM pdeescola.memaiseducacao m
							inner join entidade.entidade e on e.entid = m.entid
							left join entidade.endereco ed on ed.entid = e.entid 
							where m.memstatus = 'A'
							  and m.memanoreferencia = 2012
							  and m.memclassificacaoescola = 'R'
							  and m.docid is not null
							  and e.entemail is not null
							  and m.entcodent in (
							     select entcodent from pdeescola.memaiseducacao 
							   where memstatus = 'A' 
							   and memanoreferencia = 2012
							   and memclassificacaoescola = 'R'
							   and entcodent in (select entcodent from pdeescola.memaiseducacao where memstatus = 'A'  and memanoreferencia = 2011 and memclassificacaoescola = 'R')
							 )
		)
		
		union 
		
		--pega email dos coordenadores municipais das escolas rurais 2012	 
		select distinct ent.entemail as email
					from entidade.entidade ent
					inner join entidade.endereco ed on ed.entid = ent.entid
					INNER JOIN entidade.funcaoentidade fe on ed.entid = fe.entid 
					WHERE fe.fuestatus = 'A' and fe.funid in (96) and ent.entemail is not null
					and ed.muncod in (
					
							select distinct ed.muncod	
							FROM pdeescola.memaiseducacao m
							inner join entidade.entidade e on e.entid = m.entid
							left join entidade.endereco ed on ed.entid = e.entid 
							where m.memstatus = 'A'
							  and m.memanoreferencia = 2012
							  and m.memclassificacaoescola = 'R'
							  and m.docid is not null
							  and e.entemail is not null
							  and m.entcodent not in (
							     select entcodent from pdeescola.memaiseducacao 
							   where memstatus = 'A' 
							   and memanoreferencia = 2012
							   and memclassificacaoescola = 'R'
							   and entcodent in (select entcodent from pdeescola.memaiseducacao where memstatus = 'A'  and memanoreferencia = 2011 and memclassificacaoescola = 'R')
							 )
		)";
$dados  = $db->carregar( $sql );

$i=1;
foreach($dados as $d){
	
	//envia email
	$remetente = array("email" => $_SESSION['email_sistema'], "nome" => "SIMEC - Mais Educação");
	$destinatario = $d['email'];
	
  	$assunto = "Atualização do Plano de Atendimento das escolas do campo";

  	$conteudo = "Para as escolas do campo, o Plano de Atendimento deverá ser refeito, devido a atualização das regras e atividades. <br><br> Atenciosamente: <br> Equipe Mais Educação.";

  	if($destinatario){
  		enviar_email( $remetente, $destinatario, $assunto, $conteudo, $emailCopia );
  	}
	
	echo $i .' - '. $d['email'] .'<br>';
	
	$i++;
	
}

*/




echo "FIMMMMMMMMMMMMMMM<br>";
echo "ACABOUUUUUUUUUUUUUUUUUUUUUUUUUUU";

?>