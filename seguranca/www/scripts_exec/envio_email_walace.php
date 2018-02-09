<?php 
	include  APPRAIZ."includes/cabecalho.inc"; 
	
	if($_POST['enviar_email'] == ''){

?>

<form name="form" id="form" method="post" action="" >
	<input type="hidden" name="enviar_email" id="enviar_email" value="S">
	<input type="submit" name="enviar" id="enviar" value="Enviar e-mail">
</form>

<?php 
	} elseif($_POST['enviar_email'] == 'S') {
		include  APPRAIZ."includes/cabecalho.inc";
		/* configurações */
		ini_set("memory_limit", "3000M");
		set_time_limit(0);
		
		require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
		require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
			
		global $db;
		
		$sql = "
			Select	lstid, 
					lower(trim(usuemail)) as usuemail, 
					Initcap(trim(usunome)) as usunome, 
					email_enviado
			From temporario.email_pde_interativo
			where lower(trim(usuemail)) ilike '%@%' and email_enviado = 'N' 
			order by 2
			limit 3000
		";
		
		$us = $db->carregar($sql);
		
		$us[] = array("usuemail"=>$_SESSION['email_sistema'],"usunome"=>SIGLA_SISTEMA);
		
		$path_0 = '/var/www/simec/simec_dev/simec/www/anexo_email_wallace/premio_professores_email_marketing.jpg';
		$path_1 = '/var/www/simec/simec_dev/simec/www/anexo_email_wallace/premio_professores.mp3';
		
		$i=0;
	
		if($us[0]){
			foreach($us as $u){
				
				$mensagem = new PHPMailer();
				$mensagem->persistencia = $db;
				$mensagem->Host         = "localhost";
				$mensagem->Mailer       = "smtp";
				$mensagem->FromName		= "PRORROGAÇÃO - Inscrição para Prêmio Professores do Brasil";
				$mensagem->From 		= $_SESSION['email_sistema'];
				$mensagem->AddAddress( $u['usuemail'], $u['usunome'] ); 
				
				echo $i.' - '.$u['usuemail'].' - '.$u['usunome'].'<br>';
				
				$mensagem->AddAttachment($path_0);
				$mensagem->AddAttachment($path_1);
				
				$mensagem->Subject = "Assunto: PRORROGAÇÃO - Inscrição para Prêmio Professores do Brasil";
				$mensagem->Body = "
						<p><b>Inscrições do Prêmio Professores do Brasil prorrogadas até 10 de novembro de 2012.</b></p>
						<p>Professores de todo o país podem se inscrever e concorrer a prêmio por iniciativas de ensino bem-sucedidas</p>  
						<p>Estão prorrogadas, até o dia 10 de novembro de 2012, as inscrições para o 6º Prêmio Professores do Brasil. A iniciativa do Ministério da Educação foi instituída pela Secretaria de Educação Básica (SEB) para valorizar práticas pedagógicas bem-sucedidas, criativas e inovadoras nas redes públicas de ensino.</p> 
						<p>Este ano, foi criada uma segunda categoria, sobre temas específicos, além da já conhecida, de temas livres. Esta é subdividida nas áreas de educação infantil, anos iniciais do ensino fundamental, anos finais e ensino médio. O novo módulo conterá projetos de educação integral ou integrada, ciências para os anos iniciais, alfabetização nos anos iniciais e educação digital articulada ao desenvolvimento do currículo.</p>
						<p>Cada categoria terá até quatro professores premiados em cada uma das subcategorias, um por região do país. Os autores das experiências selecionadas pela comissão julgadora nacional, independentemente de região e da categoria, receberão R$ 7 mil, além de troféu e certificados expedidos pelas instituições parceiras.</p>
						<p>As inscrições para a sexta edição devem ser feitas na página do prêmio na internet - http://www.premioprofessoresdobrasil.mec.gov.br/. Nela, o professor também encontra informações relevantes e o regulamento do 6º Prêmio Professores do Brasil.</p>
						<p>Diretor, convoque os professores de sua escola para participar do 6º Prêmio Professores do Brasil.</p>
						<p>
						Brasília, 29 de novembro de 2012.<br>
						Secretaria de Educação Básica<br>
						MINISTÉRIO DA EDUCAÇÃO
						</p>
				";
			
				$mensagem->IsHTML( true );
				$mensagem->Send();
				
				$i = $i+1;
				
				if($i <= 3000){
					$sql = "UPDATE temporario.email_pde_interativo SET email_enviado = 'S' WHERE lstid = ".$u['lstid'].";";
					$db->executar( $sql );
					$db->commit();
				}
						
			}
		}
	
		$sql = "Select count(lstid) as qtd From temporario.email_pde_interativo Where email_enviado='S';";
		$qtd = $db->pegaUm($sql);
		
		//if($i >= 1 ){
		if($qtd == 107879 ){
			echo 'foi enviado!';
		}else{
			echo "<script>window.location='pdeinterativo.php?modulo=sistema/geral/envia_email_sis&acao=A&enviar_email=S';</script>";
		}
	}