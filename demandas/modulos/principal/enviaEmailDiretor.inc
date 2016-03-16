<?php

	$emailCopia = '';

	// Seta remetente
	$remetente = array("nome"=>"SIMEC - PROGRAMA ATLETA NA ESCOLA", "email"=>"simec-atletanaescola@mec.gov.br");

	// Pega email TÉCNICO e ANO
	$sql = "Select ateid, usucpf, usunome, lower(usuemail) as usuemail from carga.atletaemail where usuemail <> ''";
	$dados = (array) $db->carregar($sql);

	$i = 0;
	foreach ($dados as $d){
		
		$email = $d['usuemail'];
		$usucpf = $d['usucpf'];

		// Seta assunto
		$assunto  = "PROGRAMA ATLETA NA ESCOLA";
		
		// Seta Conteúdo
		$conteudo = "Prezado (a) Diretor (a) de Escola,<br><br>
					A sua escola já pode aderir ao Programa Atleta na Escola.<br><br>  
					Para sua escola aderir ao programa, acesse: <a href='http://pdeinterativo.mec.gov.br' target='_blank'>http://pdeinterativo.mec.gov.br</a><br><br> 
					Login: {$usucpf}<br>
					Senha: simecdti<br><br>  
					Para mais informações sobre o programa, acesse: <a href='http://atletanaescola.mec.gov.br/index.html' target='_blank'>http://atletanaescola.mec.gov.br/index.html</a>";
		
		
	  	enviar_email( $remetente, $email, $assunto, $conteudo, $emailCopia );

	  	//atualiza retorno
	  	$sql = "UPDATE carga.atletaemail SET retorno='t' WHERE ateid=".$d['ateid'];
	  	$db->executar($sql);
  	
	  	echo "$i - $usucpf - $email<br>";
	  	
	  	$i++;
		
	}
	
	$db->commit();
	
	echo '<br><br>ACABOUUUUUUUUUUUUUUUUUU';
	
	 

?>
	
