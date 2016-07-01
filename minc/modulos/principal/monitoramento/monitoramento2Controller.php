<?php

if ($_REQUEST['requisicao'] == 'downloadArquivo') {
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$file = new FilesSimec();
	$file->getDownloadArquivo($_REQUEST['arqid']);
	die();
}


if ($_REQUEST['requisicao'] == 'deletarArquivo') {
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$file = new FilesSimec();
	$sqlDel .= " DELETE FROM minc.mceanexo WHERE arqid = {$_REQUEST['arquivo']}; ";
	$sqlDel .= " DELETE FROM public.arquivo WHERE arqid = {$_REQUEST['arquivo']}; ";
	$db->executar($sqlDel);
	$db->commit();
	$file->excluiArquivoFisico($_REQUEST['arqid']);

	$db->sucesso('principal/monitoramento2');
	die();
}

if($_REQUEST['requisicao']=='salvarmonitoramento_2'){
	
	if (!empty($_FILES['arquivo1']['name'])||!empty($_FILES['arquivo2']['name'])||!empty($_FILES['arquivo3']['name'])) {
		$arquivos = array();
		$_FILES['arquivo1']['autor'] = $_REQUEST['arquivo']['autor'][1];
		$_FILES['arquivo2']['autor'] = $_REQUEST['arquivo']['autor'][2];
		$_FILES['arquivo3']['autor'] = $_REQUEST['arquivo']['autor'][3];
		if(!empty($_FILES['arquivo1']['name'])){
			$arquivos[1] = $_FILES['arquivo1'];
		}
		if(!empty($_FILES['arquivo2']['name'])){
			$arquivos[2] = $_FILES['arquivo2'];
		}
		if(!empty($_FILES['arquivo3']['name'])){
			$arquivos[3] = $_FILES['arquivo3'];
		}
		$x = 0;
		foreach($arquivos as $arqui){
			unset($_FILES);
			$_FILES['arquivo'] = $arqui;
			if ($arquivo['error'] == 0) {
				include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
				
				$campos = array("mceid" => "'".$_SESSION['minc']['mceid']."'", "anetipo" => "1", "anedatahora" => "NOW()", "anestatus" => "'A'" , "autor" => "'".$_FILES['arquivo']['autor']."'");
				
				$file = new FilesSimec("mceanexo", $campos, "minc");
				$file->setUpload('Foto',NULL,true, 'aneid');
			}
		}
	}
	$sql = "DELETE FROM minc.resposta
 			WHERE 
				mceid = {$_SESSION['minc']['mceid']}";
	$db->executar($sql);
	
	$sqlquestoes = '';
	$sqltestequestao9 = '';
	
	foreach($_REQUEST['alunosatendidos'] as $leaid => $alunosatendidos){
		if($alunosatendidos!=''){
			$sqltestequestao9 .= "UPDATE minc.listaestudantesatendidos
								   SET 
									alunosatendidos = $alunosatendidos
								 WHERE 
									mceid= {$_SESSION['minc']['mceid']}
								 AND leaid = {$leaid};";
		}
	}
	
	$_REQUEST['qtddocentes'] = $_REQUEST['qtddocentes']?$_REQUEST['qtddocentes']:'NULL';
	if($_REQUEST['envolvimentodocentes_2']==1){
		
		$sqlquestoes .="INSERT INTO minc.resposta(
					            queid, respostageral, respostabooleana, mceid)
					    VALUES ( 3,{$_REQUEST['qtddocentes']}, 't', {$_SESSION['minc']['mceid']});
						";
		$sqlquestoes .="INSERT INTO minc.resposta(
								queid, respostabooleana, mceid)
						VALUES ( 4, 't', {$_SESSION['minc']['mceid']});
						
						";
	}elseif($_REQUEST['envolvimentodocentes_2']==2){
		
		$sqlquestoes .="INSERT INTO minc.resposta(
								queid, respostabooleana, mceid)
						VALUES ( 2, 'f', {$_SESSION['minc']['mceid']});
						";
		
	}
	
	if($_REQUEST['colaboraintegracao_7']==1){
		
		$sqlquestoes .="INSERT INTO minc.resposta(
							queid, respostabooleana, mceid)
						VALUES ( 10, 't', {$_SESSION['minc']['mceid']});
						";
		
	}elseif($_REQUEST['colaboraintegracao_7']==2){
		
		$sqlquestoes .="INSERT INTO minc.resposta(
							queid, respostabooleana, mceid)
						VALUES ( 9, 'f', {$_SESSION['minc']['mceid']});
						";
		
	}
	
	if($_REQUEST['praticasculturais_8']==1){
		
		$sqlquestoes .="INSERT INTO minc.resposta(
							queid, respostabooleana, mceid)
						VALUES ( 12, 't', {$_SESSION['minc']['mceid']});
						";
		
	}elseif($_REQUEST['praticasculturais_8']==2){
		
		$sqlquestoes .="INSERT INTO minc.resposta(
							queid, respostabooleana, mceid)
						VALUES ( 11, 'f', {$_SESSION['minc']['mceid']});
						";
		
	}
	
	if($_REQUEST['selecaopublico_13']==1){
		
		$sqlquestoes .="INSERT INTO minc.resposta(
								queid, respostabooleana, mceid)
						VALUES ( 17, 't', {$_SESSION['minc']['mceid']});
		";
		
	}elseif($_REQUEST['selecaopublico_13']==2){
		
		$sqlquestoes .="INSERT INTO minc.resposta(
								queid, respostabooleana, mceid)
						VALUES ( 16, 'f', {$_SESSION['minc']['mceid']});
						";
		
	}
	
	if($_REQUEST['articuladooutrosprojetos_14']==1){
		
		$sqlquestoes .="INSERT INTO minc.resposta(
								queid, respostabooleana, mceid)
						VALUES (19, 't', {$_SESSION['minc']['mceid']});
		";
	}elseif($_REQUEST['articuladooutrosprojetos_14']==2){
		
		$sqlquestoes .="INSERT INTO minc.resposta(
								queid, respostabooleana, mceid)
						VALUES (18, 'f', {$_SESSION['minc']['mceid']});
						";
	}
	
	$_REQUEST['familiaresdosestudantes'] = $_REQUEST['familiaresdosestudantes']?$_REQUEST['familiaresdosestudantes']:'NULL';
	$_REQUEST['docentesdaescola'] = $_REQUEST['docentesdaescola']?$_REQUEST['docentesdaescola']:'NULL';
	$_REQUEST['pessoasdacomunidade'] = $_REQUEST['pessoasdacomunidade']?$_REQUEST['pessoasdacomunidade']:'NULL';
	$_REQUEST['qtdoutros'] = $_REQUEST['qtdoutros']?$_REQUEST['qtdoutros']:'NULL';
	$_REQUEST['link1'] = $_REQUEST['link1']?$_REQUEST['link1']:'';
	$_REQUEST['link2'] = $_REQUEST['link2']?$_REQUEST['link2']:'';
	$_REQUEST['link3'] = $_REQUEST['link3']?$_REQUEST['link3']:'';
	
	if($_REQUEST['utilizouespacovirtual_16']==1 || $_REQUEST['familiaresdosestudantes'] !='' || $_REQUEST['docentesdaescola']!='' || $_REQUEST['pessoasdacomunidade'] !='' ||$_REQUEST['qtdoutros']!=''){
		
		if($_REQUEST['utilizouespacovirtual_16']==1 ){
			$sqlquestoes .="INSERT INTO minc.resposta(
									queid, respostabooleana, mceid)
							VALUES (21, 't', {$_SESSION['minc']['mceid']});
			";
		}
		$sql = "SELECT
					coqid
				FROM
					minc.complementoquestionario
				WHERE
					mceid = {$_SESSION['minc']['mceid']}";
		$testecomplemento = $db->pegaUm($sql);
		
		if($testecomplemento == ''){
			
			$sqlcomplemento ="INSERT INTO minc.complementoquestionario(
							            familiaresdosestudantes, docentesdaescola, pessoasdacomunidade, 
							            qtdoutros, link1, link2, link3, mceid)
							    VALUES ({$_REQUEST['familiaresdosestudantes']},{$_REQUEST['docentesdaescola']}, {$_REQUEST['pessoasdacomunidade']}, 
							            {$_REQUEST['qtdoutros']}, '{$_REQUEST['link1']}', '{$_REQUEST['link2']}', '{$_REQUEST['link3']}', {$_SESSION['minc']['mceid']});";
		}else{
			
			$sqlcomplemento ="UPDATE minc.complementoquestionario
								   SET 
								   		familiaresdosestudantes={$_REQUEST['familiaresdosestudantes']}, 
								   		docentesdaescola={$_REQUEST['docentesdaescola']}, 
								   		pessoasdacomunidade={$_REQUEST['pessoasdacomunidade']}, 
								       qtdoutros={$_REQUEST['qtdoutros']}, 
								       link1='{$_REQUEST['link1']}', 
								       link2='{$_REQUEST['link2']}', 
								       link3='{$_REQUEST['link3']}'				       
								 WHERE 
										mceid = {$_SESSION['minc']['mceid']};";
		}
		
	}else{
		if($_REQUEST['utilizouespacovirtual_16']==2){
			$sqlquestoes .="INSERT INTO minc.resposta(
									queid, respostabooleana, mceid)
							VALUES (21, 'f', {$_SESSION['minc']['mceid']});
							";
		}
		$sql = "SELECT
					coqid
				FROM
					minc.complementoquestionario
				WHERE
					mceid = {$_SESSION['minc']['mceid']}";
		$testecomplemento = $db->pegaUm($sql);
		
		if($testecomplemento != ''){
			
			$sqlcomplemento ="UPDATE minc.complementoquestionario
									SET
										familiaresdosestudantes='',
										docentesdaescola='',
										pessoasdacomunidade='',
										qtdoutros='',
										link1='',
										link2='',
										link3=''
									WHERE 
										mceid = {$_SESSION['minc']['mceid']};";
			
		}
	}
	
	$sqlrespostas='';
	
	if($_REQUEST['publico']!=''){
		$_REQUEST['oprid'][$_REQUEST['publico']] = 't';
	}
	
	if($_REQUEST['oprid']){
		foreach ($_REQUEST['oprid'] as $oprid => $valor){
				$sqlrespostas .="INSERT INTO minc.resposta(
							            oprid, respostabooleana, mceid)
							    VALUES ( {$oprid},'{$valor}', {$_SESSION['minc']['mceid']});
									";
		}
	}
	if($sqlrespostas!=''){
		$db->executar($sqlrespostas);
	}
	if($sqlquestoes!=''){
		$db->executar($sqlquestoes);
	}
	if($sqltestequestao9!=''){
		$db->executar($sqltestequestao9);
	}
	if($sqlcomplemento!=''){
		$db->executar($sqlcomplemento);
	}
	$db->commit();
	
	die("<script>alert('Dados gravados com sucesso!.');window.location='minc.php?modulo=principal/monitoramento2&acao=A';</script>");
}
if ($_REQUEST['requisicao']) {
	unset($_REQUEST['requisicao']);
	exit;
}

$sql = "SELECT
			arq.arqid,
			autor,
			arqnome,
			arqextensao
		FROM
			minc.mceanexo ane
		INNER JOIN public.arquivo arq ON arq.arqid = ane.arqid
		WHERE
			mceid = {$_SESSION['minc']['mceid']}";
$arquivo = $db->carregar($sql);

$sql = "SELECT
			*
		FROM
			minc.complementoquestionario
		WHERE
			mceid = {$_SESSION['minc']['mceid']}";
$complementoquestionario = $db->pegaLinha($sql);
if($complementoquestionario){
	extract($complementoquestionario);
}

$sql = "SELECT 
			* 
		FROM
			minc.resposta
		WHERE
			mceid ={$_SESSION['minc']['mceid']} ";
$monitoramento2 = $db->carregar($sql);
if($monitoramento2){
	$oprid = array();
	$queid = array();
	foreach($monitoramento2 as $resposta){
		if($resposta['oprid']){
			if(in_array($resposta['oprid'], array(124,125,126,127,128,129,130,131,132))){
				$publico = $resposta['oprid'];
			}else{
				$oprid[$resposta['oprid']] = $resposta['respostabooleana'];
			}
		}elseif($resposta['queid']){
			if($resposta['queid']=='3'){
				$queid[$resposta['queid']]['bol']= $resposta['respostabooleana'];
				$queid[$resposta['queid']]['respostageral']= $resposta['respostageral'];
			}else{
				$queid[$resposta['queid']]= $resposta['respostabooleana'];
			}
		}
	}
}
