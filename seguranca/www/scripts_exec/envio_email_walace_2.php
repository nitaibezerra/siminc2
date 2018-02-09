<?php
	/* configurações */
	ini_set("memory_limit", "3000M");
	set_time_limit(0);
	
	require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
	require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
		
	global $db;
	
	$sql = "
		select 	distinct pcu.pcunome as usunome,
				pcu.pcuemail as usuemail
		
		from territorios.municipio mun	
		
		left join par.instrumentounidade inu on inu.muncod = mun.muncod	
		left join par.pfadesaoprograma adp on adp.inuid = inu.inuid and adp.tapid in (13,14)
		left join workflow.documento doc on doc.docid = adp.docid
		left join workflow.estadodocumento esd on esd.esdid = doc.esdid
		left join par.pftermoadesaoprograma tap on adp.tapid = tap.tapid and tap.prgid in (157)
		left join par.pfcurso pfc on pfc.prgid = tap.prgid and pfcstatus = 'A'
		left join par.pfcursista pcu on pcu.adpid = adp.adpid and pcu.pfcid = pfc.pfcid
		left join public.tipoformacao tfo on tfo.tfoid = pcu.tfoid
		left join public.tipovinculoprofissional tvp on tvp.tvpid = pcu.tvpid
		left join par.pffuncao pff on pff.pffid = pcu.pffid           
		
		where pcu.pcunome <> '' or pcu.pcuemail <> ''
	";
	
	$us = $db->carregar($sql);
	
	$us[] = array("usuemail"=>"luciano.fr.ribeiro@gmail.com","usunome"=>"luciano");
	$us[] = array("usuemail"=>"wallcp@gmail.com","usunome"=>"Wallace");
	
	$path_0 = '/var/www/simec/simec_dev/simec/www/anexo_email_wallace/SisPacto_Manual_Orientacoes.pdf';	

	if($us[0]){
		foreach($us as $u){
			
			$mensagem = new PHPMailer();
			$mensagem->persistencia = $db;
			$mensagem->Host         = "localhost";
			$mensagem->Mailer       = "smtp";
			$mensagem->FromName		= "Pacto Nacional pela Alfabetização na Idade Certa - cadastramento dos Orientadores de Estudo";
			$mensagem->From 		= $_SESSION['email_sistema'];
			$mensagem->AddAddress( $u['usuemail'], $u['usunome'] ); 
			
			echo $i.' - '.$u['usuemail'].' - '.$u['usunome'].'<br>';
			
			$mensagem->AddAttachment($path_0);
			
			$mensagem->Subject = "Pacto Nacional pela Alfabetização na Idade Certa - cadastramento dos Orientadores de Estudo";
			$mensagem->Body = "
					<p><b>Pacto Nacional pela Alfabetização na Idade Certa - cadastramento dos Orientadores de Estudo</b></p>
					<p>Prezado(a) Coordenador(a) do Pacto Nacional pela Alfabetização na Idade Certa</p>  
					<p>Encaminhamos em anexo o Manual do SisPACTO, módulo do SIMEC que será utilizado para a gestão das Ações do Pacto e que estará aberto a partir de 19/10/2012</p> 
					<p>Neste momento estamos disponibilizando algumas funções, principalmente aquela referente ao cadastramento dos Orientadores de Estudo selecionados. 
					Este cadastramento deverá ser feito por você até 16/11/2012. Contamos com seu empenho no sentido do cumprimento desta etapa do Pacto Nacional pela Alfabetização na Idade Certa.</p>
					<p>
					Atenciosamente,<br>
					MEC/Secretaria de Educação Básica
					</p>
			";
			
			$mensagem->IsHTML( true );
			$mensagem->Send();
			/*
			$i = $i+1;
			
			if($i <= 5000){
				$sql = "UPDATE pdeinterativo.listapdeinterativo SET email_enviado='S' WHERE lower(trim(usuemail)) = lower(trim('".$u['usuemail']."'));";
				$db->executar( $sql );
				$db->commit();
			}	*/		
		}
	}

	//$sql = "Select count(lstid) as qtd From pdeinterativo.listapdeinterativo Where email_enviado='S';";
	//$qtd = $db->pegaUm($sql);
	
	//if($qtd == 107765 ){
		echo 'foi enviado!';
	//}else{
	//	echo "<script>window.location='pdeinterativo.php?modulo=sistema/geral/envia_email_sis&acao=A';</script>";
	//}


	if($mensagem){
		echo 'foi enviado!';
	}		
		
