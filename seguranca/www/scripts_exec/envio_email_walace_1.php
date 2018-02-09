<?php 
	include  APPRAIZ."includes/cabecalho.inc"; 
	
	if($_POST['enviar_email'] == ''){

?>

<form name="form" id="form" method="post" action="" >
	<input type="hidden" name="enviar_email" id="enviar_email" value="S">
	<input type="submit" name="enviar" id="enviar" value="Enviar e-mail Web Conf">
</form>

<?php 
	}
	
	elseif($_POST['enviar_email'] == 'S'){
		/* configurações */
		ini_set("memory_limit", "3000M");
		set_time_limit(0);
		
		require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
		require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
			
		global $db;
		
		$sql = "
			select	est.estdescricao || ' - ' || pcu.pcunome as usunome,
					pcu.pcuemail as usuemail
			from territorios.estado est
			left join par.instrumentounidade inu on inu.estuf = est.estuf
			left join par.pfadesaoprograma adp on adp.inuid = inu.inuid and adp.tapid in (13,14)
			left join workflow.documento doc on doc.docid = adp.docid
			left join workflow.estadodocumento esd on esd.esdid = doc.esdid
			left join par.pftermoadesaoprograma tap on adp.tapid = tap.tapid and tap.prgid in (157)
			left join par.pfcurso pfc on pfc.prgid = tap.prgid and pfcstatus = 'A'
			left join par.pfcursista pcu on pcu.adpid = adp.adpid and pcu.pfcid = pfc.pfcid
			left join public.tipoformacao tfo on tfo.tfoid = pcu.tfoid
			left join public.tipovinculoprofissional tvp on tvp.tvpid = pcu.tvpid
			left join par.pffuncao pff on pff.pffid = pcu.pffid  
		";
			
			$us = $db->carregar($sql);
			
			$us[] = array("usuemail"=>"luciano.fr.ribeiro@gmail.com","usunome"=>"luciano");
			//$us[] = array("usuemail"=>"missaomip@hotmail.com","usunome"=>"Luciano");
			$us[] = array("usuemail"=>"wallcp@gmail.com","usunome"=>"Wallace");
		
		//$path_0 = '/var/www/simec/simec_dev/simec/www/anexo_email_wallace/SisPacto_Manual_Orientacoes.pdf';	
	
		if($us[0]){
			foreach($us as $u){
				
				$mensagem = new PHPMailer();
				$mensagem->persistencia = $db;
				$mensagem->Host         = "localhost";
				$mensagem->Mailer       = "smtp";
				$mensagem->FromName		= "IV Webconferência sobre o Pacto Nacional pela Alfabetização na Idade Certa";
				$mensagem->From 		= $_SESSION['email_sistema'];
				$mensagem->AddAddress( $u['usuemail'], $u['usunome'] ); 
				
				echo $i.' - '.$u['usuemail'].' - '.$u['usunome'].'<br>';
				
				//$mensagem->AddAttachment($path_0);
				
				$mensagem->Subject = "Pacto Nacional pela Alfabetização na Idade Certa - cadastramento dos Orientadores de Estudo";
				$mensagem->Body = "
						<p><b>Pacto Nacional pela Alfabetização na Idade Certa - cadastramento dos Orientadores de Estudo</b></p>
						<p>Prezado(a) coordenador(a)</p>  
						<p>Convidamos você para a IV Webconferência sobre o Pacto Nacional pela Alfabetização na Idade Certa que se realizará no <b>dia 30 de outubro</b>, a partir das 10:00. Para assisti-la em tempo real, você deve digitar no seu computador http://portal.mec.gov.br/seb/transmissao, no dia e hora indicados.</p> 
						<p>Aproveitamos este e-mail para as seguintes informações:</p>
						<p>- o prazo para indicação dos Orientadores de Estudo é 16 de novembro. Para isso entre no site http://simec.mec.gov.br/, digite seu CPF e sua senha e acesse o SisPACTO, que está aberto desde 19/10;</p>
						<p>- a senha de acesso, para quem nunca teve acesso ao SIMEC é simecdti, em letras minúsculas. Para quem já tinha acesso ao SIMEC, utilize a senha que possui. Caso não lembre desta senha, utilize a função \"Esqueceu a senha?\", que aparece na página de entrada do SIMEC, conforme endereço acima;</p>
						<p>- o coordenador não pode indicar a si mesmo para a função de orientador de estudos. Caso a Secretaria deseje fazer esta alteração, antes é necessário que o Dirigente Municipal de Educação substitua o coordenador. Isso é feito no próprio SisPACTO, conforme o manual já enviado a vocês.</p>
						<p>
						Atenciosamente,<br>
						MEC/Secretaria de Educação Básica
						</p>
				";
				
				$mensagem->IsHTML( true );
				$mensagem->Send();
				
				$i = $i+1;
				
				//if($i <= 5000){
					//$sql = "UPDATE pdeinterativo.listapdeinterativo SET email_enviado='S' WHERE lower(trim(usuemail)) = lower(trim('".$u['usuemail']."'));";
					//$db->executar( $sql );
					//$db->commit();
				//}		
			}
		}
	
		if($i >= 27){
			echo 'Foi enviado!';
		}
	}
		//$sql = "Select count(lstid) as qtd From temporario.email_pde_interativo_est Where email_enviado='S';";
		//$qtd = $db->pegaUm($sql);
		
		//if($qtd == 21384){
			//echo 'Foi enviado!';
		//}else{
			//echo "<script>window.location='pdeinterativo.php?modulo=sistema/geral/envia_email_sis&acao=A&repetir=S';</script>";
		//}
		
