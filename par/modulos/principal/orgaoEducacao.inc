<?php

if( !$_SESSION['par']['inuid'] ){
	echo "
		<script>
			alert('Erro de sessão');
			window.location.href = 'par.php?modulo=inicio&acao=C';
		</script>";
	die();
}

/* TRECHO DO POPUP DO HISTORICO DE MODIFICACAO */

if($_REQUEST['popupentid']) {
	/*
	$arrDados = $db->pegaLinha("SELECT hdpdados, usu.usunome, to_char(hpddatainsercao,'dd/mm/YYYY HH24:MI') as hpddatainsercao 
								FROM par.historicodadospar hdp 
								INNER JOIN seguranca.usuario usu ON usu.usucpf = hdp.usucpf 
								WHERE hdpid='".$_REQUEST['hdpid']."'");
	
	$dados = unserialize($arrDados['hdpdados']);
	*/

	$dados = $db->pegaLinha("SELECT ent.*, CASE WHEN usu.usunome IS NOT NULL THEN usu.usunome ELSE usu2.usunome END as usunome, to_char(ent.entdatainclusao, 'dd/mm/YYYY') as datainclusao
								FROM par.entidade ent
								LEFT JOIN seguranca.usuario usu ON usu.usucpf = ent.entcpfalteracao 
								LEFT JOIN seguranca.usuario usu2 ON usu2.usucpf = ent.usucpf
								WHERE ent.entid=".$_REQUEST['popupentid']);
	

	if(strlen($dados['entnumcpfcnpj'])==14) $htmltable .= "<tr><td class=SubTituloDireita>CNPJ:</td><td>".formatar_cnpj($dados['entnumcpfcnpj'])."</td></tr>";
	if(strlen($dados['entnumcpfcnpj'])==11) $htmltable .= "<tr><td class=SubTituloDireita>CPF:</td><td>".formatar_cpf($dados['entnumcpfcnpj'])."</td></tr>";
	if($dados['entnuninsest']) $htmltable .= "<tr><td class=SubTituloDireita>Inscrição Estadual :</td><td>".$dados['entnuninsest']."</td></tr>";
	if($dados['entnome']) $htmltable .= "<tr><td class=SubTituloDireita>Nome :</td><td>".$dados['entnome']."</td></tr>";
	if($dados['entrazaosocial']) $htmltable .= "<tr><td class=SubTituloDireita>Razão Social :</td><td>".$dados['entrazaosocial']."</td></tr>";
	if($dados['entemail']) $htmltable .= "<tr><td class=SubTituloDireita>Email :</td><td>".$dados['entemail']."</td></tr>";
	if($dados['entnumrg']) $htmltable .= "<tr><td class=SubTituloDireita>Registro Geral (RG) :</td><td>".$dados['entnumrg']."</td></tr>";
	if($dados['entorgaoexpedidor']) $htmltable .= "<tr><td class=SubTituloDireita>Orgão Expedidor :</td><td>".$dados['entorgaoexpedidor']."</td></tr>";
	if($dados['entsexo']) $htmltable .= "<tr><td class=SubTituloDireita>Sexo :</td><td>".$dados['entsexo']."</td></tr>";
	if($dados['entdatanasc']) $htmltable .= "<tr><td class=SubTituloDireita>Data de Nascimento :</td><td>".formata_data($dados['entdatanasc'])."</td></tr>";
	
	if($dados['entsig']) $htmltable .= "<tr><td class=SubTituloDireita>Sigla :</td><td>".$dados['entsig']."</td></tr>";
	if($dados['entnumresidencial']) $htmltable .= "<tr><td class=SubTituloDireita>Telefone residencial :</td><td>(".trim($dados['entnumdddresidencial']).") ".$dados['entnumresidencial']."</td></tr>";
	if($dados['entnumcomercial']) $htmltable .= "<tr><td class=SubTituloDireita>Telefone comercial :</td><td>(".trim($dados['entnumdddcomercial']).") ".$dados['entnumcomercial']." ".$dados['entnumramalcomercial']."</td></tr>";
	if($dados['entnumfax']) $htmltable .= "<tr><td class=SubTituloDireita>Telefone fax :</td><td>(".trim($dados['entnumdddfax']).") ".$dados['entnumfax']." ".$dados['entnumramalfax']."</td></tr>";
	if($dados['entnumcelular']) $htmltable .= "<tr><td class=SubTituloDireita>Telefone celular :</td><td>(".trim($dados['entnumdddcelular']).") ".$dados['entnumcelular']."</td></tr>";
	
	if($dados['endcep']) $htmltable .= "<tr><td class=SubTituloDireita>CEP :</td><td>".formata_cep($dados['endcep'])."</td></tr>";
	if($dados['endlog']) $htmltable .= "<tr><td class=SubTituloDireita>Logradouro :</td><td>".$dados['endlog']."</td></tr>";
	if($dados['endcom']) $htmltable .= "<tr><td class=SubTituloDireita>Complemento :</td><td>".$dados['endcom']."</td></tr>";
	if($dados['endnum']) $htmltable .= "<tr><td class=SubTituloDireita>Número :</td><td>".$dados['endnum']."</td></tr>";
	if($dados['endbai']) $htmltable .= "<tr><td class=SubTituloDireita>Bairro :</td><td>".$dados['endbai']."</td></tr>";
	
	if($dados['estuf']) $htmltable .= "<tr><td class=SubTituloDireita>Estado :</td><td>".$dados['estuf']."</td></tr>";
	if($dados['muncod']) $htmltable .= "<tr><td class=SubTituloDireita>Munícipio :</td><td>".$db->pegaUm("SELECT mundescricao FROM territorios.municipio WHERE muncod='".$dados['muncod']."'")."</td></tr>";
	
	?>
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr><td class="SubTituloCentro" colspan="2">DADOS MODIFICADOS POR <?=$dados['usunome'] ?>, <?=$dados['datainclusao'] ?></td></tr>
	<?=$htmltable ?>
	<tr><td class="SubTituloCentro" colspan="2"><input type="button" name="fechar" value="Fechar" onclick="window.close();"></td></tr>
	</table>	
	<?php
	exit;
}

/* FIM - TRECHO DO POPUP DO HISTORICO DE MODIFICACAO */


require_once APPRAIZ . 'includes/cabecalho.inc';
require_once APPRAIZ . "includes/classes/entidades.class.inc";
require APPRAIZ . "www/includes/webservice/pj.php";
require APPRAIZ . "www/includes/webservice/cpf.php";
$entidade = new Entidades();
$oDadosUnidade = new DadosUnidade();

$oDadosUnidade->recuperarHistoricoEntidadeDadosUnidade($_SESSION['par']['muncod'], $_SESSION['par']['estuf'], $_SESSION['par']['itrid'], true);



if ($_REQUEST['opt'] == 'salvarRegistro') {
	
	if($_POST['funcoes']['funid'] == FUNID_PREFEITURA 
		|| $_POST['funcoes']['funid'] == FUNID_SECRETARIA_MUNICIPAL_EDUCACAO )
	{
		if($_REQUEST['endereco']['muncod'] == NULL){
	//		if( $_SESSION['par']['muncod'] ){
				$muncodEntidade = $_SESSION['par']['muncod'];
				$_REQUEST['endereco'][1]['muncod'] = $muncodEntidade;
	//		}else{
	//			$muncodEntidade = $_REQUEST['endereco'][1]['muncod'];
	//		}
		}else{
			$muncodEntidade = $_REQUEST['endereco']['muncod'];
		}
		
		if($muncodEntidade != $_SESSION['par']['muncod']) {
			die("<script>
					alert('Município inválido.');
					window.location.href = '?modulo=principal/orgaoEducacao&acao=A&funid={$_REQUEST['funid']}';
				 </script>");
		}
	}
	
	
	/*
	//ver($_REQUEST, d);
	$entidade->carregarEntidade($_REQUEST);
	if($_REQUEST['funcoes']['funid']){
		$entidade->adicionarFuncoesEntidade($_REQUEST['funcoes']);
		$entidade->salvar();
		*/
		/**/
	
	if( $_REQUEST['funid'] == FUNID_PREFEITURA ){
		
		$dutid = DUTID_PREFEITURA;
		$latitude  = trim($_POST['latitude'][0].".".$_POST['latitude'][1].".".$_POST['latitude'][2].".".$_POST['latitude'][3]);
		$longitude = trim($_POST['longitude'][0].".".$_POST['longitude'][1].".".$_POST['longitude'][2].".".$_POST['longitude'][3]);
		$cep = str_replace("-","",$_POST['endereco'][1]['endcep']);

		$sql = "UPDATE par.entidade SET entstatus = 'I', entdataalteracao = 'NOW()', entcpfalteracao = '".$_SESSION['usucpf']."' WHERE entstatus = 'A' AND inuid = ".$_SESSION['par']['inuid']." AND dutid = ".$dutid;
		$db->executar($sql);
		
		$sql = "INSERT INTO par.entidade(
			            inuid, dutid, entnumcpfcnpj, entnome, entemail, entnuninsest, 
			            entstatus, entnumdddcomercial, entnumramalcomercial, entnumcomercial, 
			            entnumdddfax, entnumramalfax, entnumfax, entsig, entdatainclusao, 
			            entrazaosocial, endcep, endlog, endcom, endbai, muncod, estuf, endnum, 
			            medlatitude, medlongitude, usucpf)
			    VALUES (".$_SESSION['par']['inuid'].", ".$dutid.", '".str_replace(array(".", "/", "-"), "", $_POST['entnumcpfcnpj'])."', '".$_POST['entnome']."', '".$_POST['entemail']."', '".$_POST['entnuninsest']."',
			    		'A', '".$_POST['entnumdddcomercial']."', '".$_POST['entnumramalcomercial']."', '".$_POST['entnumcomercial']."',
			    		'".$_POST['entnumdddfax']."', '".$_POST['entnumramalfax']."', '".$_POST['entnumfax']."','".$_POST['entsig']."','NOW()',
			    		'".trim($_POST['entrazaosocial'])."', '".$cep."', '".$_POST['endereco'][1]['endlog']."','".$_POST['endereco'][1]['endcom']."',
			    		'".$_POST['endereco'][1]['endbai']."', '{$_SESSION['par']['muncod']}', '{$_SESSION['par']['estuf']}','".$_POST['endereco'][1]['endnum']."',
			    		'".$latitude."', '".$longitude."', '".$_SESSION['usucpf']."')";
		
	} elseif( $_REQUEST['funid'] == FUNID_PREFEITO ){	
		
		$dutid = DUTID_PREFEITO;
		
		$sql = "UPDATE par.entidade SET entstatus = 'I', entdataalteracao = 'NOW()', entcpfalteracao = '".$_SESSION['usucpf']."' WHERE entstatus = 'A' AND inuid = ".$_SESSION['par']['inuid']." AND dutid = ".$dutid;
		$db->executar($sql);
		
		$sql = "INSERT INTO par.entidade(inuid, dutid, entnumcpfcnpj, entnome, entemail, entstatus, entnumrg, entorgaoexpedidor, entsexo, entdatanasc, 
					entnumdddresidencial, entnumresidencial, entnumdddcomercial, entnumramalcomercial, entnumcomercial, entnumdddfax, entnumramalfax, entnumfax,
					entnumdddcelular, entnumcelular, entdatainclusao, usucpf, muncod, estuf)
			    VALUES (".$_SESSION['par']['inuid'].", ".$dutid.", '".str_replace(array(".", "/", "-"), "", $_POST['entnumcpfcnpj'])."', '".$_POST['entnome']."', '".$_POST['entemail']."',
			    	'A', '".$_POST['entnumrg']."', '".$_POST['entorgaoexpedidor']."', '".$_POST['entsexo']."', ".($_POST['entdatanasc'] ? "'".$_POST['entdatanasc']."'" : 'null').", '".$_POST['entnumdddresidencial']."', '".$_POST['entnumresidencial']."',
			    	'".$_POST['entnumdddcomercial']."', '".$_POST['entnumramalcomercial']."', '".$_POST['entnumcomercial']."', '".$_POST['entnumdddfax']."', '".$_POST['entnumramalfax']."', '".$_POST['entnumfax']."', 
			    	'".$_POST['entnumdddcelular']."', '".$_POST['entnumcelular']."', 'NOW()', '".$_SESSION['usucpf']."', '{$_SESSION['par']['muncod']}', '{$_SESSION['par']['estuf']}')";
		
	/*
		$sql = "INSERT INTO par.entidade(
			            inuid, dutid, entnumcpfcnpj, entnome, entemail, entnuninsest, 
			            entstatus, entnumdddcomercial, entnumramalcomercial, entnumcomercial, 
			            entnumdddfax, entnumramalfax, entnumfax, entsig, entdatainclusao, 
			            entnumdddcelular, entnumcelular, entrazaosocial, endcep, endlog, endcom, 
			            endbai, muncod, estuf, endnum, medlatitude, medlongitude)
			    VALUES (".$_SESSION['par']['inuid'].", ".$dutid.", '".str_replace(array(".", "/", "-"), "", $_POST['entnumcpfcnpj'])."', '".$_POST['entnome']."', '".$_POST['entemail']."', '".$_POST['entnuninsest']."',
			    		'A', '".$_POST['entnome']."')";
	*/
	} elseif( $_REQUEST['funid'] == FUNID_SECRETARIA_MUNICIPAL_EDUCACAO ){
		$dutid = DUTID_SECRETARIA_MUNICIPAL;
		
		$latitude  = trim($_POST['latitude'][0].".".$_POST['latitude'][1].".".$_POST['latitude'][2].".".$_POST['latitude'][3]);
		$longitude = trim($_POST['longitude'][0].".".$_POST['longitude'][1].".".$_POST['longitude'][2].".".$_POST['longitude'][3]);
		$cep = str_replace("-","",$_POST['endereco'][1]['endcep']);

		$sql = "UPDATE par.entidade SET entstatus = 'I', entdataalteracao = 'NOW()', entcpfalteracao = '".$_SESSION['usucpf']."' WHERE entstatus = 'A' AND inuid = ".$_SESSION['par']['inuid']." AND dutid = ".$dutid;
		$db->executar($sql);
		
		$sql = "INSERT INTO par.entidade(
			            inuid, dutid, entnome, entemail, 
			            entstatus, entnumdddcomercial, entnumramalcomercial, entnumcomercial, 
			            entnumdddfax, entnumramalfax, entnumfax, entsig, entdatainclusao, 
			            endcep, endlog, endcom, endbai, muncod, estuf, endnum, 
			            medlatitude, medlongitude, usucpf)
			    VALUES (".$_SESSION['par']['inuid'].", ".$dutid.", '".$_POST['entnome']."', '".$_POST['entemail']."',
			    		'A', '".$_POST['entnumdddcomercial']."', '".$_POST['entnumramalcomercial']."', '".$_POST['entnumcomercial']."',
			    		'".$_POST['entnumdddfax']."', '".$_POST['entnumramalfax']."', '".$_POST['entnumfax']."','".$_POST['entsig']."','NOW()',
			    		'".$cep."', '".$_POST['endereco'][1]['endlog']."','".$_POST['endereco'][1]['endcom']."',
			    		'".$_POST['endereco'][1]['endbai']."', '".$_POST['endereco'][1]['muncod']."','".$_POST['endereco'][1]['estuf']."','".$_POST['endereco'][1]['endnum']."',
			    		'".$latitude."', '".$longitude."', '".$_SESSION['usucpf']."')";
		
	} elseif( $_REQUEST['funid'] == FUNID_DIRIGENTE_MUNICIPAL_EDUCACAO ){
		$dutid = DUTID_DIRIGENTE;
		
		$sql = "UPDATE par.entidade SET entstatus = 'I', entdataalteracao = 'NOW()', entcpfalteracao = '".$_SESSION['usucpf']."' WHERE entstatus = 'A' AND inuid = ".$_SESSION['par']['inuid']." AND dutid = ".$dutid;
		$db->executar($sql);
		
		$sql = "INSERT INTO par.entidade(
			            inuid, dutid, entnumcpfcnpj, entnome, entemail, 
			            entstatus, entnumrg, entorgaoexpedidor, entsexo, entdatanasc, entnumdddresidencial, entnumresidencial, 
			            entnumdddcomercial, entnumramalcomercial, entnumcomercial, entnumdddfax, entnumramalfax, entnumfax, 
			            entnumdddcelular, entnumcelular, entdatainclusao, usucpf, muncod, estuf)
			    VALUES (".$_SESSION['par']['inuid'].", ".$dutid.", '".str_replace(array(".", "/", "-"), "", $_POST['entnumcpfcnpj'])."', '".$_POST['entnome']."', '".$_POST['entemail']."',
			    		'A', '".$_POST['entnumrg']."', '".$_POST['entorgaoexpedidor']."', '".$_POST['entsexo']."', ".($_POST['entdatanasc'] ? "'".$_POST['entdatanasc']."'" : 'null').", '".$_POST['entnumdddresidencial']."', '".$_POST['entnumresidencial']."',
			    		'".$_POST['entnumdddcomercial']."', '".$_POST['entnumramalcomercial']."', '".$_POST['entnumcomercial']."', '".$_POST['entnumdddfax']."', '".$_POST['entnumramalfax']."', '".$_POST['entnumfax']."', 
			    		'".$_POST['entnumdddcelular']."', '".$_POST['entnumcelular']."', 'NOW()', '".$_SESSION['usucpf']."', '{$_SESSION['par']['muncod']}', '{$_SESSION['par']['estuf']}')";

	} elseif( $_REQUEST['funid'] == FUNID_SECRETARIA_ESTADUAL_EDUCACAO ){	
		
		$dutid = DUTID_SECRETARIA_ESTADUAL;
		$latitude  = trim($_POST['latitude'][0].".".$_POST['latitude'][1].".".$_POST['latitude'][2].".".$_POST['latitude'][3]);
		$longitude = trim($_POST['longitude'][0].".".$_POST['longitude'][1].".".$_POST['longitude'][2].".".$_POST['longitude'][3]);
		$cep = str_replace("-","",$_POST['endereco'][1]['endcep']);

		$sql = "UPDATE par.entidade SET entstatus = 'I', entdataalteracao = 'NOW()', entcpfalteracao = '".$_SESSION['usucpf']."' WHERE entstatus = 'A' AND inuid = ".$_SESSION['par']['inuid']." AND dutid = ".$dutid;
		$db->executar($sql);
		
		$sql = "INSERT INTO par.entidade(
			            inuid, dutid, entnumcpfcnpj, entnome, entemail, entnuninsest, 
			            entstatus, entnumdddcomercial, entnumramalcomercial, entnumcomercial, 
			            entnumdddfax, entnumramalfax, entnumfax, entsig, entdatainclusao, 
			            entrazaosocial, endcep, endlog, endcom, endbai, muncod, estuf, endnum, 
			            medlatitude, medlongitude, usucpf)
			    VALUES (".$_SESSION['par']['inuid'].", ".$dutid.", '".str_replace(array(".", "/", "-"), "", $_POST['entnumcpfcnpj'])."', '".$_POST['entnome']."', '".$_POST['entemail']."', '".$_POST['entnuninsest']."',
			    		'A', '".$_POST['entnumdddcomercial']."', '".$_POST['entnumramalcomercial']."', '".$_POST['entnumcomercial']."',
			    		'".$_POST['entnumdddfax']."', '".$_POST['entnumramalfax']."', '".$_POST['entnumfax']."','".$_POST['entsig']."','NOW()',
			    		'".trim($_POST['entrazaosocial'])."', '".$cep."', '".$_POST['endereco'][1]['endlog']."','".$_POST['endereco'][1]['endcom']."',
			    		'".$_POST['endereco'][1]['endbai']."', '".$_POST['endereco'][1]['muncod']."','".$_POST['endereco'][1]['estuf']."','".$_POST['endereco'][1]['endnum']."',
			    		'".$latitude."', '".$longitude."', '".$_SESSION['usucpf']."')";
		
	} elseif( $_REQUEST['funid'] == FUNID_SECRETARIO_ESTADUAL_EDUCACAO ){	
		$dutid = DUTID_SECRETARIO_ESTADUAL;	
		$latitude  = trim($_POST['latitude'][0].".".$_POST['latitude'][1].".".$_POST['latitude'][2].".".$_POST['latitude'][3]);
		$longitude = trim($_POST['longitude'][0].".".$_POST['longitude'][1].".".$_POST['longitude'][2].".".$_POST['longitude'][3]);
		$cep = str_replace("-","",$_POST['endereco'][1]['endcep']);
		
		$sql = "UPDATE par.entidade SET entstatus = 'I', entdataalteracao = 'NOW()', entcpfalteracao = '".$_SESSION['usucpf']."' WHERE entstatus = 'A' AND inuid = ".$_SESSION['par']['inuid']." AND dutid = ".$dutid;
		$db->executar($sql);
		
		$sql = "INSERT INTO par.entidade(
			            inuid, dutid, entnumcpfcnpj, entnome, entemail, 
			            entstatus, entnumrg, entorgaoexpedidor, entsexo, entdatanasc, entnumdddresidencial, entnumresidencial, 
			            entnumdddcomercial, entnumramalcomercial, entnumcomercial, entnumdddfax, entnumramalfax, entnumfax, 
			            entnumdddcelular, entnumcelular, entdatainclusao, 
			            endcep, endlog, endcom, endbai, muncod, estuf, endnum, 
			            medlatitude, medlongitude, usucpf)
			    VALUES (".$_SESSION['par']['inuid'].", ".$dutid.", '".str_replace(array(".", "/", "-"), "", $_POST['entnumcpfcnpj'])."', '".$_POST['entnome']."', '".$_POST['entemail']."',
			    		'A', '".$_POST['entnumrg']."', '".$_POST['entorgaoexpedidor']."', '".$_POST['entsexo']."', '".$_POST['entdatanasc']."', '".$_POST['entnumdddresidencial']."', '".$_POST['entnumresidencial']."',
			    		'".$_POST['entnumdddcomercial']."', '".$_POST['entnumramalcomercial']."', '".$_POST['entnumcomercial']."', '".$_POST['entnumdddfax']."', '".$_POST['entnumramalfax']."', '".$_POST['entnumfax']."', 
			    		'".$_POST['entnumdddcelular']."', '".$_POST['entnumcelular']."', 'NOW()', 
			    		'".$cep."', '".$_POST['endereco'][1]['endlog']."','".$_POST['endereco'][1]['endcom']."',
			    		'".$_POST['endereco'][1]['endbai']."', '".$_POST['endereco'][1]['muncod']."','".$_POST['endereco'][1]['estuf']."','".$_POST['endereco'][1]['endnum']."',
			    		'".$latitude."', '".$longitude."', '".$_SESSION['usucpf']."')";
		
	}
	$db->executar($sql);
	$db->commit();
	

	$sql = "INSERT INTO par.historicodadospar(muncod, estuf, hdplink, hdpdados, usucpf, hpddatainsercao)
    		VALUES (".(($_SESSION['par']['muncod'])?"'".$_SESSION['par']['muncod']."'":"NULL").",
    				".(($_SESSION['par']['estuf'])?"'".$_SESSION['par']['estuf']."'":"NULL").",
    				'".$_SERVER['REQUEST_URI']."', '".serialize($_REQUEST)."', '".$_SESSION['usucpf']."',
    				NOW());";
	
	$db->executar($sql);
	$db->commit();
	
	echo "<script>
		alert('Dados gravados com sucesso.');
		document.location.href = '?modulo=principal/orgaoEducacao&acao=A&funid={$_REQUEST['funid']}';
	  </script>";
	/*
	}else{
		echo "<script>
			alert('Não foi possível gravar os dados.');
			document.location.href = '?modulo=principal/orgaoEducacao&acao=A&funid={$_REQUEST['funid']}';
		  </script>";
	}
	*/
}

if(!$_REQUEST['funid']){
	$funid = $_SESSION['par']['itrid'] == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL ? FUNID_PREFEITURA : FUNID_SECRETARIA_ESTADUAL_EDUCACAO;
}

$funid = $funid ? $funid : $_REQUEST['funid'];

$stFuncaoEntidade = $oDadosUnidade->recuperarFuncaoEntidadePorFunid($funid);

print "<br />";

if($_SESSION['par']['atualizacao']){
	
}

print carregaAbaDadosUnidade("/par/par.php?modulo=principal/orgaoEducacao&acao=A&funid=".$funid, $_SESSION['par']['itrid']);

monta_titulo( "Dados da unidade", "{$stFuncaoEntidade}" );

$entidadePar = ($_SESSION['par']['itrid'] == 1) ? $_SESSION['par']['estuf'] : $_SESSION['par']['muncod'];

//Pega os perfis do usuário
$arrPerfil = pegaPerfilGeral();
$arrPerfil = $arrPerfil ? $arrPerfil : array();
//Desabilita as opções de salvar para os perfis de consulta

#Estrutura condicional mudada em 04/06/2012, para que seja habilitando o questionário somente para os perfis que tem essa permissão, permissão para edição.
#Habilitando ou desabilitando os botões "Salvar anterior, Salvar e Salvar próximo".  
#O valor "false" habilita o quetionário.
#O valor "true" desabilita o questionário.
if(	in_array(PAR_PERFIL_SUPER_USUARIO, $arrPerfil) ||
	in_array(PAR_PERFIL_ADMINISTRADOR, $arrPerfil) || 
	in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $arrPerfil) ||
	in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $arrPerfil) ||
	in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $arrPerfil) ||
	in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $arrPerfil) ||
	in_array(PAR_PERFIL_PREFEITO, $arrPerfil)	
){
	$boFormDesabilitado = false;
}else{
	$boFormDesabilitado = true;
}

$formaction = "?modulo=principal/orgaoEducacao&acao=A&funid={$funid}&opt=salvarRegistro";

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript" src="/includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="/includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="/includes/ModalDialogBox/ajax.js"></script>
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<link rel="stylesheet" href="/includes/entidadesn.css" type="text/css" media="screen" />
<script type="text/javascript" src="../includes/funcoes.js" ></script>
<script type="text/javascript" src="../includes/entidadesn.js"></script>
<script type="text/javascript">
<!--

	messageObj = new DHTML_modalMessage();	// We only create one object of this class
	messageObj.setShadowOffset(5);	// Large shadow
	function displayMessage(url) {
		messageObj.setSource(url);
		messageObj.setCssClassMessageBox(false);
		messageObj.setSize(500,300);
		messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
		messageObj.display();
	}
	function displayStaticMessage(messageContent,cssClass) {
		messageObj.setHtmlContent(messageContent);
		messageObj.setSize(650,300);
		messageObj.setCssClassMessageBox(cssClass);
		messageObj.setSource(false);	// no html source since we want to use a static message here.
		messageObj.setShadowDivVisible(false);	// Disable shadow for these boxes	
		messageObj.display();
	}
	function closeMessage() {
		messageObj.close();	
	}	

	var funidEspecifico = '<?php echo $funid ?>';
	var funidFiltroEspecifico = '<?php echo $funid ?>';
	
jQuery.noConflict();
jQuery(document).ready(function(){

	
	var funid = <?php echo $funid ?>;

	switch(funid){
		case 15:
			nrAnterior = 7;
			nrProximo = 0;
			break;
		case 7:
			nrProximo = 15;
			nrAnterior = 2;
			break;
		case 2:
			nrProximo = 7;
			nrAnterior = 1;
			break;
		case 1:
			nrAnterior = 0;
			nrProximo = 2;
			break;
		case 6:
			nrAnterior = 0;
			nrProximo = 25;
			break;
		case 25:
			nrAnterior = 6;
			nrProximo = 0;
			break;
		default:
			nrAnterior = 0;
			nrProximo = 0;
			break;
	}

	btAnterior = '<input type="button" class="botao" name="anterior" value=\'Anterior\' onclick="javascript:void(0)\'" disabled>';
	btProximo = '<input type="button" class="botao" name="proximo" value=\'Próximo\' onclick="javascript:void(0)\'" disabled>';

	if(nrAnterior != 0){
		btAnterior = '<input type="button" class="botao" name="anterior" value=\'Anterior\' onclick="window.location=\'par.php?modulo=principal/orgaoEducacao&acao=A&funid='+nrAnterior+'\'" >';
	}	
	
	if(funid == 15 || funid == 25){
		btProximo = '<input type="button" class="botao" name="proximo" value=\'Próximo\' onclick="window.location=\'par.php?modulo=principal/equipeLocal&acao=A\'>';
	}else{
		if(nrProximo != 0){
			btProximo = '<input type="button" class="botao" name="proximo" value=\'Próximo\' onclick="window.location=\'par.php?modulo=principal/orgaoEducacao&acao=A&funid='+nrProximo+'\'" >';
		}
	}
	
	if(funid == 1){
		btAnterior = '<input type="button" class="botao" name="anterior" value=\'Anterior\' onclick="javascript:void(0)\'" disabled>';
		if(nrProximo != 0){
			btProximo = '<input type="button" class="botao" name="proximo" value=\'Próximo\' onclick="window.location=\'par.php?modulo=principal/orgaoEducacao&acao=A&funid='+nrProximo+'\'" >';
		}
	}
	var acoes = jQuery('#tr_acoes').find('td');
	acoesHtml = btAnterior+acoes.html()+btProximo;
	acoes.html(acoesHtml);

	if('<?php echo $_SESSION['par']['boPerfilConsulta'];?>'){
		jQuery('#btngravar').remove();
	}

	
	jQuery('#btncancelar').val('Limpar');

	jQuery('#btncancelar').click(function(){
		jQuery('#frmEntidade').each(function(){
			for(x=0;x<this.length;x++){
				if(this[x].type != 'button' && 
				   this[x].type != 'submit' &&
				   this[x].id   != 'funid' &&
				   this[x].id   != 'entidassociado'){
					   
					this[x].value = '';
				}
			}
		});
	});

	<?php if($funid == 2 || $funid == 15): ?>
		
		jQuery('#tr_endereco').hide();
		jQuery('#tr_endcep').hide();
		jQuery('#tr_endlog').hide();
		jQuery('#tr_endcom').hide();
		jQuery('#tr_endnum').hide();
		jQuery('#tr_endbai').hide();
		jQuery('#tr_estuf').hide();
		jQuery('#tr_mundescricao').hide();
		jQuery('#tr_latitude').hide();
		jQuery('#tr_longitude').hide();
		jQuery('#tr_endmapa').hide();
		
	<?php else: ?>

		jQuery('#entnome').attr('disabled', 						false);
		jQuery('#entnome').attr('readonly', 						false);
		jQuery('#entnome').attr('class', 							'normal');
	
		jQuery('#endcep1').attr('disabled', 						false);
		jQuery('#endcep1').attr('readonly', 						false);
		jQuery('#endcep1').attr('class', 							'normal');
	
		jQuery('#endlog1').attr('disabled', 						false);
		jQuery('#endlog1').attr('readonly', 						false);
		jQuery('#endlog1').attr('class', 							'normal');
	
		jQuery('#endcom1').attr('disabled', 						false);
		jQuery('#endcom1').attr('readonly', 						false);
		jQuery('#endcom1').attr('class', 							'normal');
	
		jQuery('#endnum1').attr('disabled', 						false);
		jQuery('#endnum1').attr('readonly', 						false);
		jQuery('#endnum1').attr('class', 							'normal');
	
		jQuery('#endbai1').attr('disabled', 						false);
		jQuery('#endbai1').attr('readonly', 						false);
		jQuery('#endbai1').attr('class', 							'normal');
	
		jQuery('#estuf1').attr('disabled', 							false);
		jQuery('#estuf1').attr('readonly', 							false);
		jQuery('#estuf1').attr('class', 							'normal');
		
		jQuery('#mundescricao1').attr('disabled', 					false);
		jQuery('#mundescricao1').attr('readonly', 					false);
		jQuery('#mundescricao1').attr('class', 						'normal');
		
	<?php endif; ?>

	<?php if($funid == 7): ?>
	
		jQuery('#tr_entnumcpfcnpj').hide();
		jQuery('#tr_entnuninsest').hide();
		jQuery('#tr_entrazaosocial').hide();
				
	<?php endif; ?>
	
	jQuery('#tr_funid').hide();
	jQuery('#tr_entobs').hide();
	jQuery('#tr_funcoescadastradas').hide();
	jQuery('#tr_entcodent').hide();
	jQuery('#tr_entunicod').hide();
	jQuery('#tr_entungcod').hide();
	jQuery('#tr_njuid').hide();
	jQuery('#tr_tpctgid').hide();
	jQuery('#tr_tpcid').hide();
	jQuery('#tr_tplid').hide();
	jQuery('#tr_tpsid').hide();

//	jQuery('input[name=entnome]').attr('disabled', 				false);
//	jQuery('input[name=entnome]').attr('readonly', 				false);
//	jQuery('input[name=entnome]').attr('class', 				'normal');
//	
//	jQuery('input[name=entrazaosocial]').attr('disabled', 		false);
//	jQuery('input[name=entrazaosocial]').attr('readonly', 		false);
//	jQuery('input[name=entrazaosocial]').attr('class', 			'normal');
	
	jQuery('input[name=entemail]').attr('disabled', 			false);
	jQuery('input[name=entemail]').attr('readonly', 			false);
	jQuery('input[name=entemail]').attr('class', 				'normal');
	
	jQuery('input[name=entnumdddcomercial]').attr('disabled', 	false);
	jQuery('input[name=entnumdddcomercial]').attr('readonly', 	false);
	jQuery('input[name=entnumdddcomercial]').attr('class', 		'normal');

	jQuery('input[name=entnumcomercial]').attr('disabled', 		false);
	jQuery('input[name=entnumcomercial]').attr('readonly', 		false);
	jQuery('input[name=entnumcomercial]').attr('class', 		'normal');

	jQuery('input[name=entnumramalcomercial]').attr('disabled', false);
	jQuery('input[name=entnumramalcomercial]').attr('readonly', false);
	jQuery('input[name=entnumramalcomercial]').attr('class', 	'normal');
	

	jQuery('input[name=entnumdddfax]').attr('disabled', 		false);
	jQuery('input[name=entnumdddfax]').attr('readonly', 		false);
	jQuery('input[name=entnumdddfax]').attr('class', 			'normal');

	jQuery('input[name=entnumfax]').attr('disabled', 			false);
	jQuery('input[name=entnumfax]').attr('readonly', 			false);
	jQuery('input[name=entnumfax]').attr('class', 				'normal');

	jQuery('input[name=entnumramalfax]').attr('disabled', 		false);
	jQuery('input[name=entnumramalfax]').attr('readonly', 		false);
	jQuery('input[name=entnumramalfax]').attr('class', 			'normal');

	jQuery('#entnumcpfcnpj').after('&nbsp;<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">');
	//jQuery('#entnuninsest').after('&nbsp;<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">');
	jQuery('#entnome').after('&nbsp;<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">');
	jQuery('#entrazaosocial').after('&nbsp;<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">');	
	jQuery('#entemail').after('&nbsp;<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">');
	jQuery('#entsig').after('&nbsp;<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">');
	jQuery('#entnumrg').after('&nbsp;<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">');
	jQuery('#entsexo').after('&nbsp;<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">');
	jQuery('#entdatanasc').after('&nbsp;<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">');
	jQuery('#entnumcomercial').after('&nbsp;<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">');
	jQuery('#endcep1').after('&nbsp;<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">');
	jQuery('#endlog1').after('&nbsp;<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">');
	jQuery('#endnum1').after('&nbsp;<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">');
	jQuery('#endbai1').after('&nbsp;<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">');
	jQuery('#estuf1').after('&nbsp;<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">');
	jQuery('#mundescricao1').after('&nbsp;<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">');
	jQuery('#pololatitude1').after('&nbsp;<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">');
	jQuery('#pololongitude1').after('&nbsp;<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">');
	jQuery('#entorgaoexpedidor').after('&nbsp;<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">');

	jQuery('#frmEntidade').submit(function(){

		var cpfcnpjtxt;
		var cpfcnpj = jQuery('#tr_entnumcpfcnpj').find('td:first').html();
		
		if(cpfcnpj == 'CPF :'){
			cpfcnpjtxt = 'CPF';
		}else{
			cpfcnpjtxt = 'CNPJ';
		}		
		
		if(jQuery('#tr_entnumcpfcnpj').css('display') != 'none'){
			if(jQuery('#entnumcpfcnpj').val()  == ''){
				alert('O campo '+cpfcnpjtxt+' é obrigatório.');
				jQuery('#entnumcpfcnpj').focus();
				return false;
			}
		}

		/*if(jQuery('#tr_entnuninsest').css('display') != 'none'){
			if(jQuery('#entnuninsest').val()  == ''){
				alert('O campo Inscrição estadual é obrigatório.');
				jQuery('#entnuninsest').focus();
				return false;
			}
		}*/

		if(jQuery('#tr_entnome').css('display') != 'none'){
			if(jQuery('#entnome').val()  == ''){
				alert('O campo Nome é obrigatório.');
				jQuery('#entnome').focus();
				return false;
			}
		}

//		if(jQuery('#tr_entrazaosocial').css('display') != 'none'){
//			if(jQuery('#entrazaosocial').val()  == ''){
//				alert('O campo Razão social é obrigatório.');
//				jQuery('#entrazaosocial').focus();
//				return false;
//			}
//		}

		if(jQuery('#tr_entemail').css('display') != 'none'){
			if(jQuery('#entemail').val()  == ''){
				alert('O campo E-mail é obrigatório.');
				jQuery('#entemail').focus();
				return false;
			}
		}

		if(jQuery('#tr_entsig').css('display') != 'none'){
			if(jQuery('#entsig').val()  == ''){
				alert('O campo Sigla é obrigatório.');
				jQuery('#entsig').focus();
				return false;
			}
		}

		if(jQuery('#tr_entnumrg').css('display') != 'none'){
			if(jQuery('#entnumrg').val()  == ''){
				alert('O campo Registro geral (RG) é obrigatório.');
				jQuery('#entnumrg').focus();
				return false;
			}
		}

		if(jQuery('#tr_entorgaoexpedidor').css('display') != 'none'){
			if(jQuery('#entorgaoexpedidor').val()  == ''){
				alert('O campo Orgão expedidor é obrigatório.');
				jQuery('#entorgaoexpedidor').focus();
				return false;
			}
		}

		

		if(jQuery('#tr_entsexo').css('display') != 'none'){
			if(jQuery('#entsexo').val()  == ''){
				alert('O campo Sexo é obrigatório.');
				jQuery('#entsexo').focus();
				return false;
			}
		}

		if(jQuery('#tr_entdatanasc').css('display') != 'none'){
			if(jQuery('#entdatanasc').val()  == ''){
				alert('O campo Data de nascimento é obrigatório.');
				jQuery('#entdatanasc').focus();
				return false;
			}
			if(!validaData(document.getElementById('entdatanasc'))){
				alert('O campo Data de nascimento deve ser no formato dd/mm/YYYY.');
				jQuery('#entdatanasc').focus();
				return false;
			}
		}
		
		if(jQuery('#tr_entnumdddcomercial').css('display') != 'none'){
			if(jQuery('#entnumdddcomercial').val()  == ''){
				alert('O campo DDD do telefone comercial é obrigatório.');
				jQuery('#entnumdddcomercial').focus();
				return false;
			}
		}
		
		if(jQuery('#tr_entnumcomercial').css('display') != 'none'){
			if(jQuery('#entnumcomercial').val()  == ''){
				alert('O campo Telefone comercial é obrigatório.');
				jQuery('#entnumcomercial').focus();
				return false;
			}
		}

		if(jQuery('#tr_entnumdddfax').css('display') != 'none'){
			if(trim(jQuery('#entnumfax').val())  != '' && trim(jQuery('#entnumdddfax').val())  == ''){
				alert('O campo DDD é obrigatório.');
				jQuery('#entnumdddfax').focus();
				return false;
			}
		}

		if(jQuery('#tr_entnumdddresidencial').css('display') != 'none'){
			if(trim(jQuery('#entnumresidencial').val())  != '' && trim(jQuery('#entnumdddresidencial').val())  == ''){
				alert('O campo DDD é obrigatório.');
				jQuery('#entnumdddresidencial').focus();
				return false;
			}
		}

		if(jQuery('#tr_entnumdddcelular').css('display') != 'none'){			
			if(trim(jQuery('#entnumcelular').val())  != '' && trim(jQuery('#entnumdddcelular').val())  == ''){
				alert('O campo DDD é obrigatório.');
				jQuery('#entnumdddcelular').focus();
				return false;
			}
		}
		
		if(jQuery('#tr_endcep').css('display') != 'none'){
			if(jQuery('#endcep1').val()  == ''){
				alert('O campo CEP é obrigatório.');
				jQuery('#endcep1').focus();
				return false;
			}
		}

		if(jQuery('#tr_endlog').css('display') != 'none'){
			if(jQuery('#endlog1').val()  == ''){
				alert('O campo Logradouro é obrigatório.');
				jQuery('#endlog1').focus();
				return false;
			}
		}

//		if(jQuery('#tr_endcom').css('display') != 'none'){
//			if(jQuery('#endcom').val()  == ''){
//				alert('O campo Complemento é obrigatório.');
//				jQuery('#endcom').focus();
//				return false;
//			}
//		}

		if(jQuery('#tr_endnum').css('display') != 'none'){
			if(jQuery('#endnum1').val()  == ''){
				alert('O campo Número é obrigatório.');
				jQuery('#endnum1').focus();
				return false;
			}
		}

		if(jQuery('#tr_endbai').css('display') != 'none'){
			if(jQuery('#endbai1').val()  == ''){
				alert('O campo Bairro é obrigatório.');
				jQuery('#endbai1').focus();
				return false;
			}
		}

		if(jQuery('#tr_latitude').css('display') != 'none'){
			if(jQuery('#graulatitude1').val()  == '' &&
			jQuery('#minlatitude1').val()  == '' &&
			jQuery('#seglatitude1').val()  == '' &&
			jQuery('#pololatitude1').val()  == '' &&
			jQuery('#graulongitude1').val()  == '' &&
			jQuery('#minlongitude1').val()  == '' &&
			jQuery('#seglongitude1').val()  == '' &&
			jQuery('#graulatitude1').val()  == '' &&
			jQuery('#pololongitude1').val()  == ''){
				
				alert('Os campos Latitude e Longitude são obrigatórios.');
				jQuery('#endbai1').focus();
				return false;
			}
		}			

	});

	if(jQuery('#tr_entnumcomercial').css('display') != 'none'){
		jQuery('#entnumcomercial').blur(function(){
			numero = this.value.replace('-','');
			if(numero.length < 8){
				alert('Telefone inválido.');
				jQuery('#entnumcomercial').val('');
				jQuery('#entnumcomercial').focus();
				return false;
			}
		});
	}
	
	if(jQuery('#tr_entnumdddresidencial').css('display') != 'none'){
		jQuery('#entnumresidencial').blur(function(){
			numero = this.value.replace('-','');
			if(numero.length < 8){
				alert('Telefone inválido.');
				jQuery('#entnumresidencial').val('');
				jQuery('#entnumresidencial').focus();
				return false;
			}
		});
	}

	if(jQuery('#tr_entnumdddfax').css('display') != 'none'){
		jQuery('#entnumfax').blur(function(){
			numero = this.value.replace('-','');
			if(numero.length < 8){
				alert('Telefone inválido.');
				jQuery('#entnumfax').val('');
				jQuery('#entnumfax').focus();
				return false;
			}
		});
	}

	if(jQuery('#tr_entnumdddcelular').css('display') != 'none'){
		jQuery('#entnumcelular').blur(function(){
			numero = this.value.replace('-','');
			if(numero.length < 8){
				alert('Telefone inválido.');
				jQuery('#entnumcelular').val('');
				jQuery('#entnumcelular').focus();
				return false;
			}
		});
	}
	
	<?php if($boFormDesabilitado): ?>
		jQuery('#entnumcpfcnpj').attr("disabled", true);
		jQuery('#btngravar').remove();
		jQuery('#btncancelar').remove();
		jQuery('input').attr("readonly", true);
		jQuery('select').attr("disabled", true);
		jQuery('textarea').attr("readonly", true);
		jQuery('#frmEntidade').attr("onsubmit","return false");
		jQuery('#frmEntidade').submit(function(){
			alert('Ação Indisponível!');
			return false;
		});
	<?php endif; ?>

});

//-->

function verModificacao(entid){
	window.open('par.php?modulo=principal/orgaoEducacao&acao=A&popupentid='+entid, 'blank', 'height=600,width=750,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes');
}

function enviaForm(){
	document.frmEntidade.submit();
}

</script>
<form name="frmEntidade" id="frmEntidade" method="post" action="<?= $formaction ?>" onsubmit="">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-bottom:0px;">
	<input type="hidden" name="entid" id="entid" value="" />
	<?php if( $_SESSION['par']['atualizacao'] ){ ?>
	<tr>
		<td colspan="2">
			<table cellspacing="1" cellpadding="3" bgcolor="#f5f5f5" align="center" width="60%" class="tabela">
				<tbody>
					<tr bgcolor="#cccccc">
						<td>
							<div style="float: left; width:100%; text-align: center;">
								<b>ATUALIZAÇÃO DO PAR</b><BR>
								<input value="Retornar para a Atualização do PAR" type="button" id="atualizacaopar" onclick="javascript:window.location='par.php?modulo=principal/dadosUnidadeAtualizacaoPAR&acao=A'">
							</div>
						</td>
					</tr>
				</tbody>
			</table>
		</td>
	</tr>	
	<? } ?>
	<tr>
		<td style="color: blue; font-size: 22px">
			<a href="par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=arvore"> 
				<?php echo EntidadeParControle::recuperaDescricaoEntidadePar($entidadePar, $_SESSION['par']['itrid']); ?>
			</a>
		</td>
		<td style="color: blue; font-size: 12px; text-align:right">
			<a href="par.php?modulo=principal/planoTrabalho&acao=A">Voltar à Árvore |</a>
			<label style="color:black; text-align:center"><b>Dados da Unidade</b></label>
			<a href="par.php?modulo=principal/listaObrasParUnidade&acao=A">| Lista de Obras </a>
			<a href="par.php?modulo=principal/questoesPontuais&acao=A">| Questões Pontuais</a>
		</td>
	</tr>
</table>
<?php
/*
	1  Prefeitura 
	2  Prefeito(a) 
	7  Secretaria Municipal de Educação 
	15 Secretário(a) Municipal de Educação 	
	6  Secretaria Estadual de Educação 
	25 Secretário(a) Estadual de Educação 
 */
if($funid == FUNID_PREFEITURA || $funid == FUNID_PREFEITO){
	$funid1 = FUNID_PREFEITO;
	$funid2 = FUNID_PREFEITURA;
}elseif($funid == FUNID_SECRETARIA_MUNICIPAL_EDUCACAO || $funid == FUNID_DIRIGENTE_MUNICIPAL_EDUCACAO){
	$funid1 = FUNID_DIRIGENTE_MUNICIPAL_EDUCACAO;
	$funid2 = FUNID_SECRETARIA_MUNICIPAL_EDUCACAO;
}elseif($funid == FUNID_SECRETARIA_ESTADUAL_EDUCACAO || $funid == FUNID_SECRETARIO_ESTADUAL_EDUCACAO){
	$funid1 = FUNID_SECRETARIO_ESTADUAL_EDUCACAO;
	$funid2 = FUNID_SECRETARIA_ESTADUAL_EDUCACAO;
}

$stWhere .= $_SESSION['par']['muncod'] ? "and eed2.muncod = '{$_SESSION['par']['muncod']}'" : "";
$stWhere .= $_SESSION['par']['estuf'] ? "and eed2.estuf = '{$_SESSION['par']['estuf']}'" : "";
/*
if($funid == FUNID_PREFEITURA || $funid == FUNID_SECRETARIA_ESTADUAL_EDUCACAO || $funid == FUNID_SECRETARIA_MUNICIPAL_EDUCACAO){
	
	$sql = "SELECT
				max(ent.entid) as entid1				
			FROM entidade.entidade ent
				INNER JOIN entidade.endereco 		eed2 ON eed2.entid = ent.entid
				INNER JOIN entidade.funcaoentidade 	fue ON fue.entid = ent.entid AND fue.funid = {$funid2} AND fue.fuestatus = 'A'
				INNER JOIN entidade.funcao 			fun ON fun.funid = fue.funid				
			WHERE (ent.entstatus = 'A' OR ent.entstatus IS NULL)
			{$stWhere}";
	$entid = $db->pegaUm($sql);
// ver($entid);
}else{
	
//	coalesce da data feito por não existir um default now() para esse campo
	$sql = "SELECT
				--max(ent.entid) as entid1
				max(coalesce(hdp.hpddatainsercao,'01/01/1901')) as data,
				ent.entid as entid1								
			FROM entidade.entidade ent
			INNER JOIN entidade.funcaoentidade 	fue  ON fue.entid  = ent.entid AND fue.funid = $funid1 AND fue.fuestatus = 'A'
			INNER JOIN entidade.funcao 			fun  ON fun.funid  = fue.funid
			LEFT  JOIN seguranca.usuario 		usu  ON usu.usucpf = ent.entnumcpfcnpj 				
			LEFT  JOIN par.historicodadospar 	hdp  ON hdp.usucpf = usu.usucpf
			LEFT  JOIN entidade.funentassoc 	fea  ON fea.fueid  = fue.fueid
			LEFT  JOIN entidade.entidade        ent2 ON ent2.entid = fea.entid 
			LEFT  JOIN entidade.endereco        eed2 ON eed2.entid = ent2.entid 
			LEFT  JOIN entidade.funcaoentidade 	fue2 ON fue2.entid = ent2.entid AND fue2.funid = $funid2 AND fue2.fuestatus = 'A'
			LEFT  JOIN entidade.funcao 			fun2 ON fun2.funid = fue2.funid
			WHERE 
				(ent.entstatus = 'A' OR ent.entstatus IS NULL)
				AND fun2.funstatus IS NOT NULL
				{$stWhere}
			GROUP BY ent.entid, hdp.hpddatainsercao
			ORDER BY 1 DESC";
// 			ver($sql,d);
	$rsEntid = $db->pegaLinha($sql);	
	$entid = $rsEntid['entid1']; 
// ver($sql);
	$sql = "SELECT
				max(ent.entid) as entid1				
			FROM entidade.entidade ent
				INNER JOIN entidade.endereco 		eed2 ON eed2.entid = ent.entid
				INNER JOIN entidade.funcaoentidade 	fue ON fue.entid = ent.entid AND fue.funid = {$funid2} AND fue.fuestatus = 'A'
				INNER JOIN entidade.funcao 			fun ON fun.funid = fue.funid				
			WHERE (ent.entstatus = 'A' OR ent.entstatus IS NULL)
			{$stWhere}";
	 $funentassoc = $db->pegaUm($sql);
	 
	 if(!$funentassoc){
	 	if($_SESSION['par']['itrid'] == 2){
		 	if($funid == FUNID_PREFEITO){
		 		$funidTemp = FUNID_PREFEITURA;
		 		$msg = 'Prefeitura não cadastrada.';
		 	} elseif($funid == FUNID_DIRIGENTE_MUNICIPAL_EDUCACAO){
		 		$funidTemp = FUNID_SECRETARIA_MUNICIPAL_EDUCACAO;
		 		$msg = 'Secretaria não cadastrada.';
		 	}	 		
	 	} else {
	 		if($funid == FUNID_SECRETARIO_ESTADUAL_EDUCACAO){
		 		$funidTemp = FUNID_SECRETARIA_ESTADUAL_EDUCACAO;
		 		$msg = 'Secretaria não cadastrada.';
		 	}
	 	}
	 	echo "<script>
				alert('$msg');
				window.location.href = 'par.php?modulo=principal/orgaoEducacao&acao=A&funid=$funidTemp'
			  </script>";
		die;	 	
	 }
}

if($entid){
	$entidade->carregarPorEntid($entid);
}*/

$funentassoc = $funentassoc ? $funentassoc : null;
/*
echo $entidade->formEntidade("?modulo=principal/orgaoEducacao&acao=A&funid={$funid}&opt=salvarRegistro",
							 array("funid" => $funid, "entidassociado" => $funentassoc),
							 array( "enderecos"=>array(1) ) );
*/
if( $funid == FUNID_PREFEITURA ){
	$dutid = DUTID_PREFEITURA;
	$texto1 = "PREFEITURA";
} elseif( $funid == FUNID_PREFEITO ){	
	$dutid = DUTID_PREFEITO;
	$texto2 = "PREFEITO(A)";
} elseif( $funid == FUNID_SECRETARIA_MUNICIPAL_EDUCACAO ){	
	$dutid = DUTID_SECRETARIA_MUNICIPAL;
	$texto1 = "SECRETARIA MUNICIPAL DE EDUCAÇÃO";
} elseif( $funid == FUNID_DIRIGENTE_MUNICIPAL_EDUCACAO ){	
	$dutid = DUTID_DIRIGENTE;
	$texto2 = "DIRIGENTE MUNICIPAL DE EDUCAÇÃO";
} elseif( $funid == FUNID_SECRETARIA_ESTADUAL_EDUCACAO ){	
	$dutid = DUTID_SECRETARIA_ESTADUAL;
	$texto1 = "SECRETARIA ESTADUAL DE EDUCAÇÃO";
} elseif( $funid == FUNID_SECRETARIO_ESTADUAL_EDUCACAO ){	
	$dutid = DUTID_SECRETARIO_ESTADUAL;
	$texto2 = "SECRETÁRIO(A)";
}

$sql = "SELECT * FROM par.entidade ent LEFT JOIN territorios.municipio mun ON mun.muncod = ent.muncod WHERE ent.entstatus = 'A' AND ent.inuid = ".$_SESSION['par']['inuid']." AND ent.dutid = ".$dutid;
$dados = $db->pegaLinha($sql);

if( $dados ){
	extract( $dados );
}

if( $medlatitude ){
	$latitude = explode('.',$medlatitude);
}
if( $medlongitude ){
	$longitude = explode('.',$medlongitude);
}
?>

<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center" id="tblentidade">
	<tbody>
		<tr id="msg_buscacnpj_receita" style="display: none"><td colspan="2"  style="background: rgb(252, 253, 219) none repeat scroll 0% 0%; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;"><img src="../imagens/restricao.png"> <b>Essa não é a entidade que procura?</b> <a href="#" onclick="confirmaBuscaReceitaPeloCNPJ($('entnumcpfcnpj').value)">Clique aqui</a> para consultar a base da Receita Federal. </td></tr>
		<tr id="msg_buscacnpj_receita" style="display: none"><td colspan="2" style="background: rgb(252, 253, 219) none repeat scroll 0% 0%; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;"><img src="../imagens/restricao.png"><b>Essa não é a entidade que procura?</b> <a href="#" onclick="confirmaBuscaReceitaPeloCNPJ(jQuery('entnumcpfcnpj').value)">Clique aqui</a> para consultar a base da Receita Federal. </td></tr>
		

<?php if( $funid == FUNID_PREFEITURA || $funid == FUNID_SECRETARIA_ESTADUAL_EDUCACAO ){ ?>

		<tr id="tr_titulo"><td class="SubTituloCentro" colspan="2"><?=$texto1 ?></td></tr>
		<tr id="tr_entnumcpfcnpj"><td class="SubTituloDireita">CNPJ :</td><td><input type="text" name="entnumcpfcnpj" id="entnumcpfcnpj" value="<?=formatar_cnpj($entnumcpfcnpj) ?>" size="22" maxlength="18" class="normal" onkeyup="this.value=mascaraglobal('##.###.###/####-##',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);getEntidadePeloCNPJ(this.value);"></td></tr>
		<tr id="tr_entnuninsest"><td class="SubTituloDireita">Inscrição Estadual :</td><td><input type="text" name="entnuninsest" id="entnuninsest" value="<?=$entnuninsest ?>" size="16" maxlength="14" class="normal" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"></td></tr>
		<tr id="tr_entunicod" style="display: none;"><td class="SubTituloDireita">Código da unidade :</td><td><input type="text" name="entunicod" id="entunicod" value="<?=$entunicod ?>" size="6" maxlength="5" class="normal" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);getEntidadePeloUnicod(this.value);"></td></tr>
		<tr id="tr_entungcod" style="display: none;"><td class="SubTituloDireita">Código da unidade gestora :</td><td><input type="text" name="entungcod" id="entungcod" value="<?=$entungcod ?>" size="16" maxlength="14" class="normal" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"></td></tr>
		<tr id="tr_entnome"><td class="SubTituloDireita">Nome :</td><td><input type="text" name="entnome" id="entnome" value="<?=$entnome ?>" size="90" maxlength="255" class="normal" onfocus="" onmouseout="" onblur=""></td></tr>
		<tr id="tr_entrazaosocial"><td class="SubTituloDireita">Razão Social :</td><td><input type="text" name="entrazaosocial" id="entrazaosocial" value="<?=$entrazaosocial ?>" size="90" maxlength="255" class="disabled" onfocus="" onmouseout="" onblur="" readonly="readonly"></td></tr>
		<tr id="tr_entemail"><td class="SubTituloDireita">Email :</td><td><input type="text" name="entemail" id="entemail" value="<?=$entemail ?>" size="45" maxlength="100" class="normal" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"></td></tr>
		<tr id="tr_entsig"><td class="SubTituloDireita">Sigla :</td><td><input type="text" name="entsig" id="entsig" value="<?=$entsig ?>" size="45" maxlength="50" class="normal" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"></td></tr>
		<tr id="tr_entnumdddcomercial"><td class="SubTituloDireita">Telefone comercial :</td><td>( <input type="text" name="entnumdddcomercial" id="entnumdddcomercial" value="<?=$entnumdddcomercial ?>" size="3" maxlength="2" class="normal" onkeyup="this.value=mascaraglobal('##',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"> ) <input type="text" name="entnumcomercial" id="entnumcomercial" size="11" maxlength="9" value="<?=$entnumcomercial ?>" class="normal" onkeyup="this.value=mascaraglobal('####-####',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"> <strong>Ramal :</strong> <input type="text" name="entnumramalcomercial" id="entnumramalcomercial" value="<?=$entnumramalcomercial ?>" size="5" maxlength="4" class="normal" onkeyup="this.value=mascaraglobal('##########',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"></td></tr>
		<tr id="tr_entnumdddfax"><td class="SubTituloDireita">Fax :</td><td>( <input type="text" name="entnumdddfax" id="entnumdddfax" value="<?=$entnumdddfax ?>" size="3" maxlength="2" class="normal" onkeyup="this.value=mascaraglobal('##',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"> ) <input type="text" name="entnumfax" id="entnumfax" value="<?=$entnumfax ?>" size="11" maxlength="9" class="normal" onkeyup="this.value=mascaraglobal('####-####',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"> <strong>Ramal :</strong> <input type="text" name="entnumramalfax" id="entnumramalfax" value="<?=$entnumramalfax ?>" size="5" maxlength="4" class="normal" onkeyup="this.value=mascaraglobal('####',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"></td></tr>
		<tr id="tr_njuid" style="display: 'none'"><td class="SubTituloDireita">Natureza Jurídica :</td><td><select name="njuid" class="CampoEstilo" style="width: auto" id="njuid"><option value="">Selecione</option></select></td></tr>
		<tr id="tr_funcoescadastradas" style="display: none;"><td class="SubTituloDireita">Funções cadastradas:</td><td><select size="5" style="width:450px" id="funcoescadastradas"><option>Prefeitura</option></select></td></tr>
		<tr id="tr_endereco"><td class="SubTituloCentro" colspan="2">ENDEREÇO<input type="hidden" name="endereco[1][endid]" id="endid1" value="9983"></td></tr>
		<tr id="tr_endcep"><td class="SubTituloDireita">CEP :</td><td><input type="text" name="endereco[1][endcep]" id="endcep1" value="<?=formata_cep($endcep) ?>" size="11" maxlength="9" class="normal" onkeyup="this.value=mascaraglobal('#####-###',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);getEnderecoPeloCEP(this.value,'1');"></td></tr>
		<tr id="tr_endlog"><td class="SubTituloDireita">Logradouro :</td><td><input type="text" name="endereco[1][endlog]" id="endlog1" value="<?=$endlog ?>" size="80" maxlength="200" class="normal" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"></td></tr>
		<tr id="tr_endcom"><td class="SubTituloDireita">Complemento :</td><td><input type="text" name="endereco[1][endcom]" id="endcom1" value="<?=$endcom ?>" size="80" maxlength="200" class="normal" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"></td></tr>
		<tr id="tr_endnum"><td class="SubTituloDireita">Número :</td><td><input type="text" name="endereco[1][endnum]" id="endnum1" value="<?=$endnum ?>" size="11" maxlength="10" class="normal" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"></td></tr>
		<tr id="tr_endbai"><td class="SubTituloDireita">Bairro :</td><td><input type="text" name="endereco[1][endbai]" id="endbai1" value="<?=$endbai ?>" size="80" maxlength="100" class="normal" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"></td></tr>
		<tr id="tr_estuf"><td class="SubTituloDireita">UF :</td><td><input type="text" name="endereco[1][estuf]" id="estuf1" value="<?=$estuf ?>" size="3" maxlength="2" class="normal"></td></tr>
		<tr id="tr_mundescricao"><td class="SubTituloDireita">Munícipio :</td><td><input type="text" id="mundescricao1" name="mundescricao1" value="<?=$mundescricao ?>" size="30" class="normal"><input type="hidden" name="endereco[1][muncod]" id="muncod1" value="<?=$muncod ?>"></td></tr>
		<tr id="tr_latitude"><td class="SubTituloDireita">Latitude :</td>
			<td>
				<input name="latitude[]" id="graulatitude1" maxlength="2" size="3" value="<? echo $latitude[0]; ?>" class="normal" type="hidden"> 
					<span id="_graulatitude1"><?php echo ($latitude[0]) ? $latitude[0] : 'XX'; ?></span> º 
				<input name="latitude[]" id="minlatitude1" size="3" maxlength="2" value="<? echo $latitude[1]; ?>" class="normal" type="hidden"> 
					<span id="_minlatitude1"><?php echo ($latitude[1]) ? $latitude[1] : 'XX'; ?></span>  ' 
				<input name="latitude[]" id="seglatitude1" size="3" maxlength="2" value="<? echo $latitude[2]; ?>" class="normal" type="hidden"> 
					<span id="_seglatitude1"><?php echo ($latitude[2]) ? $latitude[2] : 'XX'; ?></span> " 
				<input name="latitude[]" id="pololatitude1" value="<? echo $latitude[3]; ?>" type="hidden">
					<span id="_pololatitude1"><?php echo ($latitude[3]) ? $latitude[3] : 'X'; ?></span>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Longitude :</td>
			<td>
				<input name="longitude[]" id="graulongitude1" maxlength="2" size="3" value="<? echo $longitude[0]; ?>" type="hidden"> 
					<span id="_graulongitude1"><?php echo ($longitude[0]) ? $longitude[0] : 'XX'; ?></span>	º 
				<input name="longitude[]" id="minlongitude1" size="3" maxlength="2" value="<? echo $longitude[1]; ?>" type="hidden"> 
					<span id="_minlongitude1"><?php echo ($longitude[1]) ? $longitude[1] : 'XX'; ?></span>  ' 
				<input name="longitude[]" id="seglongitude1" size="3" maxlength="2" value="<? echo $longitude[2]; ?>" type="hidden"> 
					<span id="_seglongitude1"><?php echo ($longitude[2]) ? $longitude[2] : 'XX'; ?></span> " 
				<input name="longitude[]" id="pololongitude1" value="<? echo $longitude[3]; ?>" type="hidden"> 
					<span id="_pololongitude1"><?php echo ($longitude[3]) ? $longitude[3] : 'X'; ?></span>
				<input type="hidden" name="endzoom" id="endzoom" value="<? echo $obCoendereCoentrega->endzoom; ?>" />
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">&nbsp;</td>
			<td>
				<a href="#" onclick="abreMapaEntidade('1');">Visualizar / Buscar No Mapa</a> 
				<input style="display: none;" name="endereco[1][endzoom]" id="endzoom1" value="" type="text">
			</td>
		</tr>
		
<?php } elseif( $funid == FUNID_PREFEITO || $funid == FUNID_SECRETARIO_ESTADUAL_EDUCACAO ){ 

		$selectedM = "";
		$selectedF = "";
		if( $entsexo == "M" ){
			$selectedM = "selected";
		}
		if( $entsexo == "F" ){
			$selectedF = "selected";
		}
		?>

		<tr id="msg_buscacnpj_receita" style="display: none"><td colspan="2"  style="background: rgb(252, 253, 219) none repeat scroll 0% 0%; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;"><img src="../imagens/restricao.png"> <b>Essa não é a entidade que procura?</b> <a href="#" onclick="confirmaBuscaReceitaPeloCNPJ($('entnumcpfcnpj').value)">Clique aqui</a> para consultar a base da Receita Federal. </td></tr>
		<tr id="msg_buscacnpj_receita" style="display: none"><td colspan="2" style="background: rgb(252, 253, 219) none repeat scroll 0% 0%; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;"><img src="../imagens/restricao.png"><b>Essa não é a entidade que procura?</b> <a href="#" onclick="confirmaBuscaReceitaPeloCNPJ(jQuery('entnumcpfcnpj').value)">Clique aqui</a> para consultar a base da Receita Federal. </td></tr>
		
		<tr id="tr_titulo"><td class="SubTituloCentro" colspan="2"><?=$texto2 ?></td></tr>
		<tr id="tr_entnumcpfcnpj"><td class="SubTituloDireita">CPF :</td><td><input type="text" name="entnumcpfcnpj" id="entnumcpfcnpj" size="17" maxlength="14" value="<?=formatar_cpf($entnumcpfcnpj) ?>" class="normal" onkeyup="this.value=mascaraglobal('###.###.###-##',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);getEntidadePeloCPF(this.value);"></td></tr>
		<tr id="tr_entnome"><td class="SubTituloDireita">Nome :</td><td><input readonly="" type="text" name="entnome" id="entnome" size="45" maxlength="255" value="<?=$entnome ?>" class="normal" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"></td></tr>
		<tr id="tr_entemail"><td class="SubTituloDireita">Email :</td><td><input type="text" name="entemail" id="entemail" value="<?=$entemail ?>" size="45" maxlength="100" class="normal" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"></td></tr>
		<tr id="tr_entnumrg"><td class="SubTituloDireita">Registro Geral (RG) :</td><td><input type="text" name="entnumrg" id="entnumrg" size="15" maxlength="15" value="<?=$entnumrg ?>" class="normal" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"></td></tr>
		<tr id="tr_entorgaoexpedidor"><td class="SubTituloDireita">Orgão Expedidor :</td><td><input type="text" name="entorgaoexpedidor" id="entorgaoexpedidor" size="15" maxlength="10" value="<?=$entorgaoexpedidor ?>" class="normal" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"></td></tr>
		<tr id="tr_entsexo"><td class="SubTituloDireita">Sexo :</td>
			<td>
				<select name="entsexo" class="CampoEstilo" id="entsexo">
					<option value="">Selecione</option>
					<option value="M" <?=$selectedM ?>>Masculino</option>
					<option value="F" <?=$selectedF ?>>Feminino</option>
				</select>
			</td></tr>
		<tr id="tr_entdatanasc"><td class="SubTituloDireita">Data de Nascimento :</td><td><input type="text" name="entdatanasc" id="entdatanasc" size="12" maxlength="10" value="<?=formata_data($entdatanasc) ?>" class="normal" onkeyup="this.value=mascaraglobal('##/##/####',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"></td></tr>
		<tr id="tr_entnumdddresidencial"><td class="SubTituloDireita">Telefone residencial :</td><td>( <input type="text" name="entnumdddresidencial" id="entnumdddresidencial" size="3" maxlength="2" value="<?=$entnumdddresidencial ?>" class="normal" onkeyup="this.value=mascaraglobal('##',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"> ) <input type="text" name="entnumresidencial" id="entnumresidencial" size="11" maxlength="9" value="<?=$entnumresidencial ?>" class="normal" onkeyup="this.value=mascaraglobal('####-####',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"></td></tr>
		<tr id="tr_entnumdddcomercial"><td class="SubTituloDireita">Telefone comercial :</td><td>( <input type="text" name="entnumdddcomercial" id="entnumdddcomercial" value="<?=$entnumdddcomercial ?>" size="3" maxlength="2" class="normal" onkeyup="this.value=mascaraglobal('##',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"> ) <input type="text" name="entnumcomercial" id="entnumcomercial" size="11" maxlength="9" value="<?=$entnumcomercial ?>" class="normal" onkeyup="this.value=mascaraglobal('####-####',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"> <strong>Ramal :</strong> <input type="text" name="entnumramalcomercial" id="entnumramalcomercial" value="<?=$entnumramalcomercial ?>" size="5" maxlength="4" class="normal" onkeyup="this.value=mascaraglobal('##########',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"></td></tr>
		<tr id="tr_entnumdddfax"><td class="SubTituloDireita">Fax :</td><td>( <input type="text" name="entnumdddfax" id="entnumdddfax" value="<?=$entnumdddfax ?>" size="3" maxlength="2" class="normal" onkeyup="this.value=mascaraglobal('##',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"> ) <input type="text" name="entnumfax" id="entnumfax" value="<?=$entnumfax ?>" size="11" maxlength="9" class="normal" onkeyup="this.value=mascaraglobal('####-####',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"> <strong>Ramal :</strong> <input type="text" name="entnumramalfax" id="entnumramalfax" value="<?=$entnumramalfax ?>" size="5" maxlength="4" class="normal" onkeyup="this.value=mascaraglobal('####',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"></td></tr>
		<tr id="tr_entnumdddcelular"><td class="SubTituloDireita">Celular :</td><td>( <input type="text" name="entnumdddcelular" id="entnumdddcelular" size="3" maxlength="2" value="<?=$entnumdddcelular ?>" class="normal" onkeyup="this.value=mascaraglobal('##',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"> ) <input type="text" name="entnumcelular" id="entnumcelular" size="12" maxlength="10" value="<?=$entnumcelular ?>" class="normal" onkeyup="this.value=mascaraglobal('#####-####',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"></td></tr>
		<tr id="tr_entobs"><td class="SubTituloDireita">Observação :</td><td><textarea  id="entobs" name="entobs" cols="45" rows="5" onmouseover="MouseOver(this);" onfocus="MouseClick(this);" onmouseout="MouseOut(this);" onblur="MouseBlur(this);textCounter(this.form.entobs,this.form.no_entobs,500);"  style="width:45 ex;"  onkeydown="textCounter(this.form.entobs,this.form.no_entobs,500);" onkeyup="textCounter(this.form.entobs,this.form.no_entobs,500);"></textarea><br><input ".$bloq." readonly style="text-align:right;border-left:#888888 3px solid;color:#808080;" type="text" name="no_entobs" size="6" maxlength="6" value="500" class="CampoEstilo"><font color="red" size="1" face="Verdana"> máximo de caracteres</font></div><div id="print_entobs" class="notscreen" style="text-align: left;"></div></td></tr>
		<tr id="tr_funcoescadastradas" style="display: none;"><td class="SubTituloDireita">Funções cadastradas:</td><td><select size="5" style="width:450px" id="funcoescadastradas"><option>Prefeitura</option></select></td></tr>
		<?php if( $funid == FUNID_SECRETARIO_ESTADUAL_EDUCACAO ){ ?>
			<tr id="tr_endereco"><td class="SubTituloCentro" colspan="2">ENDEREÇO<input type="hidden" name="endereco[1][endid]" id="endid1" value="9983"></td></tr>
			<tr id="tr_endcep"><td class="SubTituloDireita">CEP :</td><td><input type="text" name="endereco[1][endcep]" id="endcep1" value="<?=formata_cep($endcep) ?>" size="11" maxlength="9" class="normal" onkeyup="this.value=mascaraglobal('#####-###',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);getEnderecoPeloCEP(this.value,'1');"></td></tr>
			<tr id="tr_endlog"><td class="SubTituloDireita">Logradouro :</td><td><input type="text" name="endereco[1][endlog]" id="endlog1" value="<?=$endlog ?>" size="80" maxlength="200" class="normal" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"></td></tr>
			<tr id="tr_endcom"><td class="SubTituloDireita">Complemento :</td><td><input type="text" name="endereco[1][endcom]" id="endcom1" value="<?=$endcom ?>" size="80" maxlength="200" class="normal" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"></td></tr>
			<tr id="tr_endnum"><td class="SubTituloDireita">Número :</td><td><input type="text" name="endereco[1][endnum]" id="endnum1" value="<?=$endnum ?>" size="11" maxlength="10" class="normal" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"></td></tr>
			<tr id="tr_endbai"><td class="SubTituloDireita">Bairro :</td><td><input type="text" name="endereco[1][endbai]" id="endbai1" value="<?=$endbai ?>" size="80" maxlength="100" class="normal" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"></td></tr>
			<tr id="tr_estuf"><td class="SubTituloDireita">UF :</td><td><input type="text" name="endereco[1][estuf]" id="estuf1" value="<?=$estuf ?>" size="3" maxlength="2" class="normal"></td></tr>
			<tr id="tr_mundescricao"><td class="SubTituloDireita">Munícipio :</td><td><input type="text" id="mundescricao1" name="mundescricao1" value="<?=$mundescricao ?>" size="30" class="normal"><input type="hidden" name="endereco[1][muncod]" id="muncod1" value="<?=$muncod ?>"></td></tr>
			<tr id="tr_latitude"><td class="SubTituloDireita">Latitude :</td>
				<td>
					<input name="latitude[]" id="graulatitude1" maxlength="2" size="3" value="<? echo $latitude[0]; ?>" class="normal" type="hidden"> 
						<span id="_graulatitude1"><?php echo ($latitude[0]) ? $latitude[0] : 'XX'; ?></span> º 
					<input name="latitude[]" id="minlatitude1" size="3" maxlength="2" value="<? echo $latitude[1]; ?>" class="normal" type="hidden"> 
						<span id="_minlatitude1"><?php echo ($latitude[1]) ? $latitude[1] : 'XX'; ?></span>  ' 
					<input name="latitude[]" id="seglatitude1" size="3" maxlength="2" value="<? echo $latitude[2]; ?>" class="normal" type="hidden"> 
						<span id="_seglatitude1"><?php echo ($latitude[2]) ? $latitude[2] : 'XX'; ?></span> " 
					<input name="latitude[]" id="pololatitude1" value="<? echo $latitude[3]; ?>" type="hidden">
						<span id="_pololatitude1"><?php echo ($latitude[3]) ? $latitude[3] : 'X'; ?></span>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Longitude :</td>
				<td>
					<input name="longitude[]" id="graulongitude1" maxlength="2" size="3" value="<? echo $longitude[0]; ?>" type="hidden"> 
						<span id="_graulongitude1"><?php echo ($longitude[0]) ? $longitude[0] : 'XX'; ?></span>	º 
					<input name="longitude[]" id="minlongitude1" size="3" maxlength="2" value="<? echo $longitude[1]; ?>" type="hidden"> 
						<span id="_minlongitude1"><?php echo ($longitude[1]) ? $longitude[1] : 'XX'; ?></span>  ' 
					<input name="longitude[]" id="seglongitude1" size="3" maxlength="2" value="<? echo $longitude[2]; ?>" type="hidden"> 
						<span id="_seglongitude1"><?php echo ($longitude[2]) ? $longitude[2] : 'XX'; ?></span> " 
					<input name="longitude[]" id="pololongitude1" value="<? echo $longitude[3]; ?>" type="hidden"> 
						<span id="_pololongitude1"><?php echo ($longitude[3]) ? $longitude[3] : 'X'; ?></span>
					<input type="hidden" name="endzoom" id="endzoom1" value="<? echo $obCoendereCoentrega->endzoom; ?>" />
				</td>
			</tr>
		<?php } ?>
<?php } elseif( $funid == FUNID_SECRETARIA_MUNICIPAL_EDUCACAO ){ ?>

		<tr id="tr_titulo"><td class="SubTituloCentro" colspan="2"><?=$texto1 ?></td></tr>
		<tr id="tr_entnumcpfcnpj"><td class="SubTituloDireita">CNPJ :</td><td><input type="text" name="entnumcpfcnpj" id="entnumcpfcnpj" value="<?=formatar_cnpj($entnumcpfcnpj) ?>" size="22" maxlength="18" class="normal" onkeyup="this.value=mascaraglobal('##.###.###/####-##',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);getEntidadePeloCNPJ(this.value);"></td></tr>
		<tr id="tr_entnuninsest"><td class="SubTituloDireita">Inscrição Estadual :</td><td><input type="text" name="entnuninsest" id="entnuninsest" value="<?=$entnuninsest ?>" size="16" maxlength="14" class="normal" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"></td></tr>
		<tr id="tr_entunicod" style="display: none;"><td class="SubTituloDireita">Código da unidade :</td><td><input type="text" name="entunicod" id="entunicod" value="<?=$entunicod ?>" size="6" maxlength="5" class="normal" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);getEntidadePeloUnicod(this.value);"></td></tr>
		<tr id="tr_entungcod" style="display: none;"><td class="SubTituloDireita">Código da unidade gestora :</td><td><input type="text" name="entungcod" id="entungcod" value="<?=$entungcod ?>" size="16" maxlength="14" class="normal" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"></td></tr>
		<tr id="tr_entnome"><td class="SubTituloDireita">Nome :</td><td><input type="text" name="entnome" id="entnome" value="<?=$entnome ?>" size="90" maxlength="255" class="normal" onfocus="" onmouseout="" onblur=""></td></tr>
		<tr id="tr_entrazaosocial"><td class="SubTituloDireita">Razão Social :</td><td><input type="text" name="entrazaosocial" id="entrazaosocial" value="<?=$entrazaosocial ?>" size="90" maxlength="255" class="disabled" onfocus="" onmouseout="" onblur="" readonly="readonly"></td></tr>
		<tr id="tr_entemail"><td class="SubTituloDireita">Email :</td><td><input type="text" name="entemail" id="entemail" value="<?=$entemail ?>" size="45" maxlength="100" class="normal" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"></td></tr>
		<tr id="tr_entsig"><td class="SubTituloDireita">Sigla :</td><td><input type="text" name="entsig" id="entsig" value="<?=$entsig ?>" size="45" maxlength="50" class="normal" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"></td></tr>
		<tr id="tr_entnumdddcomercial"><td class="SubTituloDireita">Telefone comercial :</td><td>( <input type="text" name="entnumdddcomercial" id="entnumdddcomercial" value="<?=$entnumdddcomercial ?>" size="3" maxlength="2" class="normal" onkeyup="this.value=mascaraglobal('##',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"> ) <input type="text" name="entnumcomercial" id="entnumcomercial" size="11" maxlength="9" value="<?=$entnumcomercial ?>" class="normal" onkeyup="this.value=mascaraglobal('####-####',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"> <strong>Ramal :</strong> <input type="text" name="entnumramalcomercial" id="entnumramalcomercial" value="<?=$entnumramalcomercial ?>" size="5" maxlength="4" class="normal" onkeyup="this.value=mascaraglobal('##########',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"></td></tr>
		<tr id="tr_entnumdddfax"><td class="SubTituloDireita">Fax :</td><td>( <input type="text" name="entnumdddfax" id="entnumdddfax" value="<?=$entnumdddfax ?>" size="3" maxlength="2" class="normal" onkeyup="this.value=mascaraglobal('##',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"> ) <input type="text" name="entnumfax" id="entnumfax" value="<?=$entnumfax ?>" size="11" maxlength="9" class="normal" onkeyup="this.value=mascaraglobal('####-####',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"> <strong>Ramal :</strong> <input type="text" name="entnumramalfax" id="entnumramalfax" value="<?=$entnumramalfax ?>" size="5" maxlength="4" class="normal" onkeyup="this.value=mascaraglobal('####',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"></td></tr>
		<tr id="tr_funcoescadastradas" style="display: none;"><td class="SubTituloDireita">Funções cadastradas:</td><td><select size="5" style="width:450px" id="funcoescadastradas"><option>Prefeitura</option></select></td></tr>
		<tr id="tr_endereco"><td class="SubTituloCentro" colspan="2">ENDEREÇO<input type="hidden" name="endereco[1][endid]" id="endid1" value="9983"></td></tr>
		<tr id="tr_endcep"><td class="SubTituloDireita">CEP :</td><td><input type="text" name="endereco[1][endcep]" id="endcep1" value="<?=formata_cep($endcep) ?>" size="11" maxlength="9" class="normal" onkeyup="this.value=mascaraglobal('#####-###',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);getEnderecoPeloCEP(this.value,'1');"></td></tr>
		<tr id="tr_endlog"><td class="SubTituloDireita">Logradouro :</td><td><input type="text" name="endereco[1][endlog]" id="endlog1" value="<?=$endlog ?>" size="80" maxlength="200" class="normal" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"></td></tr>
		<tr id="tr_endcom"><td class="SubTituloDireita">Complemento :</td><td><input type="text" name="endereco[1][endcom]" id="endcom1" value="<?=$endcom ?>" size="80" maxlength="200" class="normal" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"></td></tr>
		<tr id="tr_endnum"><td class="SubTituloDireita">Número :</td><td><input type="text" name="endereco[1][endnum]" id="endnum1" value="<?=$endnum ?>" size="11" maxlength="10" class="normal" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"></td></tr>
		<tr id="tr_endbai"><td class="SubTituloDireita">Bairro :</td><td><input type="text" name="endereco[1][endbai]" id="endbai1" value="<?=$endbai ?>" size="80" maxlength="100" class="normal" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"></td></tr>
		<tr id="tr_estuf"><td class="SubTituloDireita">UF :</td><td><input type="text" name="endereco[1][estuf]" id="estuf1" value="<?=$estuf ?>" size="3" maxlength="2" class="normal"></td></tr>
		<tr id="tr_mundescricao"><td class="SubTituloDireita">Munícipio :</td><td><input type="text" id="mundescricao1" name="mundescricao1" value="<?=$mundescricao ?>" size="30" class="normal"><input type="hidden" name="endereco[1][muncod]" id="muncod1" value="<?=$muncod ?>"></td></tr>
		<tr id="tr_latitude"><td class="SubTituloDireita">Latitude :</td>
			<td>
				<input name="latitude[]" id="graulatitude1" maxlength="2" size="3" value="<? echo $latitude[0]; ?>" class="normal" type="hidden"> 
					<span id="_graulatitude1"><?php echo ($latitude[0]) ? $latitude[0] : 'XX'; ?></span> º 
				<input name="latitude[]" id="minlatitude1" size="3" maxlength="2" value="<? echo $latitude[1]; ?>" class="normal" type="hidden"> 
					<span id="_minlatitude1"><?php echo ($latitude[1]) ? $latitude[1] : 'XX'; ?></span>  ' 
				<input name="latitude[]" id="seglatitude1" size="3" maxlength="2" value="<? echo $latitude[2]; ?>" class="normal" type="hidden"> 
					<span id="_seglatitude1"><?php echo ($latitude[2]) ? $latitude[2] : 'XX'; ?></span> " 
				<input name="latitude[]" id="pololatitude1" value="<? echo $latitude[3]; ?>" type="hidden">
					<span id="_pololatitude1"><?php echo ($latitude[3]) ? $latitude[3] : 'X'; ?></span>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Longitude :</td>
			<td>
				<input name="longitude[]" id="graulongitude1" maxlength="2" size="3" value="<? echo $longitude[0]; ?>" type="hidden"> 
					<span id="_graulongitude1"><?php echo ($longitude[0]) ? $longitude[0] : 'XX'; ?></span>	º 
				<input name="longitude[]" id="minlongitude1" size="3" maxlength="2" value="<? echo $longitude[1]; ?>" type="hidden"> 
					<span id="_minlongitude1"><?php echo ($longitude[1]) ? $longitude[1] : 'XX'; ?></span>  ' 
				<input name="longitude[]" id="seglongitude1" size="3" maxlength="2" value="<? echo $longitude[2]; ?>" type="hidden"> 
					<span id="_seglongitude1"><?php echo ($longitude[2]) ? $longitude[2] : 'XX'; ?></span> " 
				<input name="longitude[]" id="pololongitude1" value="<? echo $longitude[3]; ?>" type="hidden"> 
					<span id="_pololongitude1"><?php echo ($longitude[3]) ? $longitude[3] : 'X'; ?></span>
				<input type="hidden" name="endzoom" id="endzoom" value="<? echo $obCoendereCoentrega->endzoom; ?>" />
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">&nbsp;</td>
			<td>
				<a href="#" onclick="abreMapaEntidade('1');">Visualizar / Buscar No Mapa</a> 
				<input style="display: none;" name="endereco[1][endzoom]" id="endzoom1" value="" type="text">
			</td>
		</tr>
		
<?php } elseif( $funid == FUNID_DIRIGENTE_MUNICIPAL_EDUCACAO ){ 

		$selectedM = "";
		$selectedF = "";
		if( $entsexo == "M" ){
			$selectedM = "selected";
		}
		if( $entsexo == "F" ){
			$selectedF = "selected";
		}
		?>

		<textarea style="display: none;" id="entobs" name="entobs" cols="45" rows="5" onmouseover="MouseOver(this);" onfocus="MouseClick(this);" onmouseout="MouseOut(this);" onblur="MouseBlur(this);textCounter(this.form.entobs,this.form.no_entobs,500);"  style="width:45 ex;"  onkeydown="textCounter(this.form.entobs,this.form.no_entobs,500);" onkeyup="textCounter(this.form.entobs,this.form.no_entobs,500);"></textarea>
		<tr id="msg_buscacnpj_receita" style="display: none"><td colspan="2"  style="background: rgb(252, 253, 219) none repeat scroll 0% 0%; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;"><img src="../imagens/restricao.png"> <b>Essa não é a entidade que procura?</b> <a href="#" onclick="confirmaBuscaReceitaPeloCNPJ($('entnumcpfcnpj').value)">Clique aqui</a> para consultar a base da Receita Federal. </td></tr>
		<tr id="msg_buscacnpj_receita" style="display: none"><td colspan="2" style="background: rgb(252, 253, 219) none repeat scroll 0% 0%; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;"><img src="../imagens/restricao.png"><b>Essa não é a entidade que procura?</b> <a href="#" onclick="confirmaBuscaReceitaPeloCNPJ(jQuery('entnumcpfcnpj').value)">Clique aqui</a> para consultar a base da Receita Federal. </td></tr>
		
		<tr id="tr_titulo"><td class="SubTituloCentro" colspan="2"><?=$texto2 ?></td></tr>
		<tr id="tr_entnumcpfcnpj"><td class="SubTituloDireita">CPF :</td><td><input type="text" name="entnumcpfcnpj" id="entnumcpfcnpj" size="17" maxlength="14" value="<?=formatar_cpf($entnumcpfcnpj) ?>" class="normal" onkeyup="this.value=mascaraglobal('###.###.###-##',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);getEntidadePeloCPF(this.value);"></td></tr>
		<tr id="tr_entnome"><td class="SubTituloDireita">Nome :</td><td><input readonly="" type="text" name="entnome" id="entnome" size="45" maxlength="255" value="<?=$entnome ?>" class="normal" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"></td></tr>
		<tr id="tr_entemail"><td class="SubTituloDireita">Email :</td><td><input type="text" name="entemail" id="entemail" value="<?=$entemail ?>" size="45" maxlength="100" class="normal" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"></td></tr>
		<tr id="tr_entnumrg"><td class="SubTituloDireita">Registro Geral (RG) :</td><td><input type="text" name="entnumrg" id="entnumrg" size="15" maxlength="15" value="<?=$entnumrg ?>" class="normal" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"></td></tr>
		<tr id="tr_entorgaoexpedidor"><td class="SubTituloDireita">Orgão Expedidor :</td><td><input type="text" name="entorgaoexpedidor" id="entorgaoexpedidor" size="15" maxlength="10" value="<?=$entorgaoexpedidor ?>" class="normal" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"></td></tr>
		<tr id="tr_entsexo"><td class="SubTituloDireita">Sexo :</td>
			<td>
				<select name="entsexo" class="CampoEstilo" id="entsexo">
					<option value="">Selecione</option>
					<option value="M" <?=$selectedM ?>>Masculino</option>
					<option value="F" <?=$selectedF ?>>Feminino</option>
				</select>
			</td></tr>
		<tr id="tr_entdatanasc"><td class="SubTituloDireita">Data de Nascimento :</td><td><input type="text" name="entdatanasc" id="entdatanasc" size="12" maxlength="10" value="<?=formata_data($entdatanasc) ?>" class="normal" onkeyup="this.value=mascaraglobal('##/##/####',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"></td></tr>
		<tr id="tr_entnumdddresidencial"><td class="SubTituloDireita">Telefone residencial :</td><td>( <input type="text" name="entnumdddresidencial" id="entnumdddresidencial" size="3" maxlength="2" value="<?=$entnumdddresidencial ?>" class="normal" onkeyup="this.value=mascaraglobal('##',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"> ) <input type="text" name="entnumresidencial" id="entnumresidencial" size="11" maxlength="9" value="<?=$entnumresidencial ?>" class="normal" onkeyup="this.value=mascaraglobal('####-####',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"></td></tr>
		<tr id="tr_entnumdddcomercial"><td class="SubTituloDireita">Telefone comercial :</td><td>( <input type="text" name="entnumdddcomercial" id="entnumdddcomercial" value="<?=$entnumdddcomercial ?>" size="3" maxlength="2" class="normal" onkeyup="this.value=mascaraglobal('##',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"> ) <input type="text" name="entnumcomercial" id="entnumcomercial" size="11" maxlength="9" value="<?=$entnumcomercial ?>" class="normal" onkeyup="this.value=mascaraglobal('####-####',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"> <strong>Ramal :</strong> <input type="text" name="entnumramalcomercial" id="entnumramalcomercial" value="<?=$entnumramalcomercial ?>" size="5" maxlength="4" class="normal" onkeyup="this.value=mascaraglobal('##########',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"></td></tr>
		<tr id="tr_entnumdddfax"><td class="SubTituloDireita">Fax :</td><td>( <input type="text" name="entnumdddfax" id="entnumdddfax" value="<?=$entnumdddfax ?>" size="3" maxlength="2" class="normal" onkeyup="this.value=mascaraglobal('##',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"> ) <input type="text" name="entnumfax" id="entnumfax" value="<?=$entnumfax ?>" size="11" maxlength="9" class="normal" onkeyup="this.value=mascaraglobal('####-####',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"> <strong>Ramal :</strong> <input type="text" name="entnumramalfax" id="entnumramalfax" value="<?=$entnumramalfax ?>" size="5" maxlength="4" class="normal" onkeyup="this.value=mascaraglobal('####',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"></td></tr>
		<tr id="tr_entnumdddcelular"><td class="SubTituloDireita">Celular :</td><td>( <input type="text" name="entnumdddcelular" id="entnumdddcelular" size="3" maxlength="2" value="<?=$entnumdddcelular ?>" class="normal" onkeyup="this.value=mascaraglobal('##',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"> ) <input type="text" name="entnumcelular" id="entnumcelular" size="12" maxlength="10" value="<?=$entnumcelular ?>" class="normal" onkeyup="this.value=mascaraglobal('#####-####',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"></td></tr>
		<tr id="tr_funcoescadastradas" style="display: none;"><td class="SubTituloDireita">Funções cadastradas:</td><td><select size="5" style="width:450px" id="funcoescadastradas"><option>Prefeitura</option></select></td></tr>

<?php } ?>
		<tr id="tr_acoes">
			<td class="SubTituloCentro" colspan="2">
				<input type="button" class="botao" name="anterior" value="Anterior" onclick="javascript:void(0)'" disabled="">
				<input type="button" value="Gravar" id="btngravar" onclick="enviaForm()">
				<input type="button" value="Limpar" id="btncancelar" onclick="window.close();">
				<input type="button" class="botao" name="proximo" value="Próximo" onclick="window.location='par.php?modulo=principal/orgaoEducacao&amp;acao=A&amp;funid=2'">
			</td>
		</tr>
	</tbody>
</table>
<?php
				

monta_titulo( "Histórico de modificações", "{$stFuncaoEntidade}" );

/*
$sql = "SELECT 
			'<center><img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" onclick=\"verModificacao(\''||hdp.hdpid||'\');\"></center>' as acao, 
			usu.usunome, to_char(hdp.hpddatainsercao, 'dd/mm/YYYY HH24:MI') as hpddatainsercao 
		FROM par.historicodadospar hdp
		LEFT JOIN seguranca.usuario usu ON usu.usucpf = hdp.usucpf 
		WHERE 
			hdplink='/par/par.php?modulo=principal/orgaoEducacao&acao=A&funid={$funid}&opt=salvarRegistro' ".(($_SESSION['par']['itrid']==2)?"AND hdp.muncod='".$_SESSION['par']['muncod']."' ":"").(($_SESSION['par']['itrid']==1)?"AND hdp.estuf='".$_SESSION['par']['estuf']."' AND hdp.muncod IS NULL ":"").
		"ORDER BY
			hdp.hdpid DESC";
*/

$sql = "SELECT 
			'<center><img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" onclick=\"verModificacao(\''||foo.entid||'\');\"></center>' as acao, 
			usunome, 
			entdataalteracao  
		FROM
		(SELECT
			ent.entid,
			usu.usunome, 
			to_char(ent.entdatainclusao, 'dd/mm/YYYY') as entdataalteracao 
		FROM par.entidade ent
		LEFT JOIN seguranca.usuario usu ON usu.usucpf = ent.usucpf
		WHERE 
			ent.entstatus = 'A' AND ent.inuid = ".$_SESSION['par']['inuid']." AND ent.dutid = ".$dutid."
		UNION ALL
		SELECT
			ent.entid,
			usu.usunome, 
			to_char(ent.entdataalteracao, 'dd/mm/YYYY') as entdataalteracao 
		FROM par.entidade ent
		LEFT JOIN seguranca.usuario usu ON usu.usucpf = ent.entcpfalteracao 
		WHERE 
			ent.entstatus = 'I' AND ent.inuid = ".$_SESSION['par']['inuid']." AND ent.dutid = ".$dutid." ) foo
		ORDER BY
			foo.entid DESC";
/*
$sql = "SELECT
			'<center><img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" onclick=\"verModificacao(\''||ent.entid||'\');\"></center>' as acao, 
			usu.usunome, to_char(ent.entdataalteracao, 'dd/mm/YYYY') as entdataalteracao 
		FROM par.entidade ent
		LEFT JOIN seguranca.usuario usu ON usu.usucpf = ent.entcpfalteracao 
		WHERE 
			ent.entstatus = 'I' AND ent.inuid = ".$_SESSION['par']['inuid']." AND ent.dutid = ".$dutid."
		ORDER BY
			ent.entid DESC";
*/
$cabecalho = array("&nbsp;","Nome","Data alteração");
$db->monta_lista_simples($sql,$cabecalho,50,5,'N','95%',$par2);

?>