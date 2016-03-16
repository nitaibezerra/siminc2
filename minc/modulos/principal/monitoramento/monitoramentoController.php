<?php
require APPRAIZ . 'www/includes/webservice/PessoaJuridicaClient.php';

if (!$_SESSION['minc']['mceid'] || !$_SESSION['minc']['entid']) {
	echo '<script>
	        alert("Sua sessão expirou. Por favor, entre novamente!");
	        location.href = "minc.php?modulo=inicio&acao=A";
	    </script>';
	exit;
}

$sql = "SELECT * FROM minc.monitoramento WHERE mceid='{$_SESSION['minc']['mceid']}'";
// ver($sql);
$monitoramento = $db->pegaLinha($sql);
if ($monitoramento) {
	extract($monitoramento);
}

$sql = "SELECT entnome as nome, entnumcpfcnpj as cpf, entnumresidencial as telefone, entemail as email, e.entnumdddresidencial as ddd
		FROM entidade.entidade e  
		INNER JOIN entidade.funcaoentidade fe on e.entid = fe.entid 
		INNER JOIN entidade.funentassoc fea on fea.fueid = fe.fueid 
		WHERE fea.entid = '{$_SESSION['minc']['entid']}' AND fe.funid = '109'";
$dadoscoordenador = $db->pegaLinha($sql);

if ($_REQUEST['requisicao'] == 'downloadArquivo') {
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$file = new FilesSimec();
	$file->getDownloadArquivo($_REQUEST['arqid']);
	die();
}

if ($_REQUEST['requisicao'] == 'deletaArquivo') {
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$file = new FilesSimec();
	$sqlDel .= " DELETE FROM minc.mceanexo WHERE arqid = {$_REQUEST['arqid']}; ";
	$sqlDel .= " DELETE FROM public.arquivo WHERE arqid = {$_REQUEST['arqid']}; ";
	$db->executar($sqlDel);
	$db->commit();
	$file->excluiArquivoFisico($_REQUEST['arqid']);

	$db->sucesso('principal/monitoramento');
	die();
}

if ($_REQUEST['requisicao3'] == 'salvaresporadica') {

	$sql = "SELECT monid FROM minc.monitoramento WHERE mceid = '{$_SESSION['minc']['mceid']}'";
	$monid = $db->pegaUm($sql);

	if ($atiid) {
		$sql = "UPDATE minc.atividadesmonitoramento
					SET atidatainicio='" . formata_data_sql($_REQUEST['atidatainicio2']) . "', atidatafim='" . formata_data_sql($_REQUEST['atidatafim2']) . "',atidescricao='{$_REQUEST['atidescricao2']}'
			 	WHERE monid = '{$monid}'";
	} else {
		$sql = "INSERT INTO minc.atividadesmonitoramento(
					atidatainicio, atidatafim, atidescricao, atitipo, monid)
				VALUES ('" . formata_data_sql($_REQUEST['atidatainicio2']) . "','" . formata_data_sql($_REQUEST['atidatafim2']) . "','{$_REQUEST['atidescricao2']}','E','{$monid}');";
	}
	$db->executar($sql);
	$db->commit();

	echo "<script>
			alert('Dados Gravados com sucesso');
			window.location='minc.php?modulo=principal/monitoramento&acao=A';
		  </script>";
	exit();
}

if ($_REQUEST['requisicao2'] == 'inserirorcamento') {
	$sql = "SELECT monid FROM minc.monitoramento WHERE mceid = '{$_SESSION['minc']['mceid']}'";
	$monid = $db->pegaUm($sql);

	/*
	$sql = "SELECT atiid FROM minc.atividadesmonitoramento WHERE atitipo='F' and monid ='{$monid}'";
	$atiid = $db->pegaUm($sql);
	*/

	//monsta string diasdasemana
	$atidiasdasemana = "";
	if (!$_REQUEST['atidiasdasemana_seg']) $atidiasdasemana .= "0"; else $atidiasdasemana .= $_REQUEST['atidiasdasemana_seg'];

	if (!$_REQUEST['atidiasdasemana_ter']) $atidiasdasemana .= "0"; else $atidiasdasemana .= $_REQUEST['atidiasdasemana_ter'];

	if (!$_REQUEST['atidiasdasemana_qua']) $atidiasdasemana .= "0"; else $atidiasdasemana .= $_REQUEST['atidiasdasemana_qua'];

	if (!$_REQUEST['atidiasdasemana_qui']) $atidiasdasemana .= "0"; else $atidiasdasemana .= $_REQUEST['atidiasdasemana_qui'];

	if (!$_REQUEST['atidiasdasemana_sex']) $atidiasdasemana .= "0"; else $atidiasdasemana .= $_REQUEST['atidiasdasemana_sex'];

	if (!$_REQUEST['atidiasdasemana_sab']) $atidiasdasemana .= "0"; else $atidiasdasemana .= $_REQUEST['atidiasdasemana_sab'];

	if (!$_REQUEST['atidiasdasemana_dom']) $atidiasdasemana .= "0"; else $atidiasdasemana .= $_REQUEST['atidiasdasemana_dom'];

	if ($atiid) {
		$sql = "UPDATE minc.atividadesmonitoramento
			    SET atidatainicio='" . formata_data_sql($_REQUEST['atidatainicio']) . "', atidatafim='" . formata_data_sql($_REQUEST['atidatafim']) . "', atidiasdasemana='" . $atidiasdasemana . "',
			    	atiturno='{$_REQUEST['atiturno']}',atidescricao='{$_REQUEST['atidescricao']}'
			 	WHERE monid = '{$monid}'";
	} else {
		$sql = "INSERT INTO minc.atividadesmonitoramento(
	            	atidatainicio, atidatafim, atidiasdasemana, atiturno, 
	            	atidescricao, atitipo, monid)
			    VALUES ('" . formata_data_sql($_REQUEST['atidatainicio']) . "','" . formata_data_sql($_REQUEST['atidatafim']) . "','" . $atidiasdasemana . "','{$_REQUEST['atiturno']}',
			    		'{$_REQUEST['atidescricao']}','F','{$monid}');";
	}
	$db->executar($sql);
	$db->commit();

	echo "<script>
			alert('Dados Gravados com sucesso');
			window.location='minc.php?modulo=principal/monitoramento&acao=A';
		  </script>";
	exit();
}

if ($_REQUEST['requisicao'] == 'inserirparceiro') {
	
 	if($moninicioupreenchimento !='t'){
 		$campo = "moninicioupreenchimento = 't',";
 	}
	if ($_REQUEST['moncpfcoord']) $_REQUEST['moncpfcoord'] = str_replace(Array('.', '-'), '', $_REQUEST['moncpfcoord']);

	if ($_REQUEST['montipoiniciativa'] == '1') {
		$moncpfcnpj = $_REQUEST['moncpfcnpj1'];
	} elseif ($_REQUEST['montipoiniciativa'] == '2') {
		$moncpfcnpj = $_REQUEST['moncpfcnpj2'];
	}

	if ($moncpfcnpj) $moncpfcnpj = str_replace(Array('.', '-', '/'), '', $moncpfcnpj);
	$montelefoneicp = str_replace(Array('.', '-', '/'), '', $_REQUEST['montelefoneicp']);


	$monmesmocorrdenadormaiseducacao = ( empty($_REQUEST['monmesmocorrdenadormaiseducacao'])  ? 'f': $_REQUEST['monmesmocorrdenadormaiseducacao']);
	$moncoordeiniciativacultparceira = ( empty($_REQUEST['moncoordeiniciativacultparceira'])  ? 'f': $_REQUEST['moncoordeiniciativacultparceira']);
	$moncoordmesmodainscr = ( empty($_REQUEST['moncoordmesmodainscr'])  ? 'f': $_REQUEST['moncoordmesmodainscr']);
	$sql = "
			UPDATE minc.monitoramento
   			SET
   				$campo 
				montipoiniciativa='{$_REQUEST['montipoiniciativa']}', 
				monnomeinicultpar='{$_REQUEST['monnomeinicultpar']}',
				moncpfcnpj = '{$moncpfcnpj}', 
				monemailicp='{$_REQUEST['monemailicp']}', 
				montelefoneicp='{$montelefoneicp}',
				monocorreualteracaodaicp='{$_REQUEST['alteracao']}', 
			    tpiid=" . ($_REQUEST['tpiid'] ? $_REQUEST['tpiid'] : 'null') . ",
			    mondescricaotipoidentidadeespecifico='{$_REQUEST['mondescricaotipoidentidadeespecifico']}', 
			    monrepresentagrupo='" . ($_REQUEST['monrepresentagrupo'] ? $_REQUEST['monrepresentagrupo'] : 'f') . "',
				monnomeintegrantes='{$_REQUEST['monnomeintegrantes']}', 
				ttmid=" . ($_REQUEST['ttmid'] ? $_REQUEST['ttmid'] : 'null') . ",
				mondescreveidentidadecultural='{$_REQUEST['mondescreveidentidadecultural']}', 
				tieid=" . ($_REQUEST['tieid'] ? $_REQUEST['tieid'] : 'null') . ",
				monetiniaescola='{$_REQUEST['monetiniaescola']}', 
				monmesmocorrdenadormaiseducacao='{$monmesmocorrdenadormaiseducacao}',
				moncoordeiniciativacultparceira='{$moncoordeiniciativacultparceira}',
				moncoordmesmodainscr='{$moncoordmesmodainscr}',
				monnomecoord='{$_REQUEST['monnomecoord']}', 
				moncpfcoord='{$_REQUEST['moncpfcoord']}', 
				montelcoord='{$_REQUEST['montelcoord']}', 
				monemailcoord='{$_REQUEST['monemailcoord']}',
				monaquisicaomaterialconsumo1=" . (($_REQUEST['monaquisicaomaterialconsumo1']) ? "'" . str_replace(array(".", ","), array("", "."), $_REQUEST['monaquisicaomaterialconsumo1']) . "'" : "NULL") . ",
			    moncontratacaoservicosculturais1=" . (($_REQUEST['moncontratacaoservicosculturais1']) ? "'" . str_replace(array(".", ","), array("", "."), $_REQUEST['moncontratacaoservicosculturais1']) . "'" : "NULL") . ",
		        moncontratacaoservicosdiversos1=" . (($_REQUEST['moncontratacaoservicosdiversos1']) ? "'" . str_replace(array(".", ","), array("", "."), $_REQUEST['moncontratacaoservicosdiversos1']) . "'" : "NULL") . ",
			    monlocacaodeinstrumentos1=" . (($_REQUEST['monlocacaodeinstrumentos1']) ? "'" . str_replace(array(".", ","), array("", "."), $_REQUEST['monlocacaodeinstrumentos1']) . "'" : "NULL") . ",
		        monaquisicaomateriaispermanentes1=" . (($_REQUEST['monaquisicaomateriaispermanentes1']) ? "'" . str_replace(array(".", ","), array("", "."), $_REQUEST['monaquisicaomateriaispermanentes1']) . "'" : "NULL") . ",
			    monsaldoorcamento1=" . (($_REQUEST['monsaldoorcamento1']) ? "'" . str_replace(array(".", ","), array("", "."), $_REQUEST['monsaldoorcamento1']) . "'" : "NULL") . ",
			    monaquisicaomaterialconsumo2=" . (($_REQUEST['monaquisicaomaterialconsumo2']) ? "'" . str_replace(array(".", ","), array("", "."), $_REQUEST['monaquisicaomaterialconsumo2']) . "'" : "NULL") . ",
		        moncontratacaoservicosculturais2=" . (($_REQUEST['moncontratacaoservicosculturais2']) ? "'" . str_replace(array(".", ","), array("", "."), $_REQUEST['moncontratacaoservicosculturais2']) . "'" : "NULL") . ",
			    moncontratacaoservicosdiversos2=" . (($_REQUEST['moncontratacaoservicosdiversos2']) ? "'" . str_replace(array(".", ","), array("", "."), $_REQUEST['moncontratacaoservicosdiversos2']) . "'" : "NULL") . ",
		        monlocacaodeinstrumentos2=" . (($_REQUEST['monlocacaodeinstrumentos2']) ? "'" . str_replace(array(".", ","), array("", "."), $_REQUEST['monlocacaodeinstrumentos2']) . "'" : "NULL") . ",
			    monaquisicaomateriaispermanentes2=" . (($_REQUEST['monaquisicaomateriaispermanentes2']) ? "'" . str_replace(array(".", ","), array("", "."), $_REQUEST['monaquisicaomateriaispermanentes2']) . "'" : "NULL") . ",
		        monsaldoorcamento2=" . (($_REQUEST['monsaldoorcamento2']) ? "'" . str_replace(array(".", ","), array("", "."), $_REQUEST['monsaldoorcamento2']) . "'" : "NULL") . "
			WHERE mceid='{$_SESSION['minc']['mceid']}'
			RETURNING monid";
	$monid = $db->pegaUm($sql);

	//Anexa Arquivo
	if (!empty($_FILES['arquivo']['name'])) {
		if ($_FILES['arquivo']['error'] == 0) {
			include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
			$campos = array("monid" => "'" . $monid . "'", "anetipo" => "1", "anedatahora" => "NOW()", "anestatus" => "'A'");
			$file = new FilesSimec("mceanexo", $campos, "minc");
			$file->setUpload(NULL, $key = "arquivo");
		}
	}

	$db->commit();

	echo "<script>
			alert('Dados Gravados com sucesso');
			window.location='minc.php?modulo=principal/monitoramento&acao=A';
		  </script>";
	exit();
}


if ($_REQUEST['apagarItem']) {
	$sql = "DELETE FROM minc.atividadesmonitoramento WHERE atiid = {$_REQUEST['apagarItem']};";
	$db->executar($sql);
	$db->commit();

	echo "<script>
			alert('Item Apagado com sucesso');
			location.href='minc.php?modulo=principal/monitoramento&acao=A';
		  </script>";
	exit();
}

if ($_REQUEST['requisicao'] == 'getPessoaJuridica') {
	$cnpj = str_replace('.', '', str_replace('-', '', str_replace('/', '', $_POST['cnpj'])));

	$soapClient = new SoapClient('http://ws.mec.gov.br/PessoaJuridica/wsdl');
	$xml = $soapClient->solicitarDadosPessoaJuridicaPorCnpj($cnpj);
	$xmlCorrigido = str_replace(array("& "), array("&amp; "), $xml);
	$objXml =  simplexml_load_string($xmlCorrigido);

	$contatos = (array) $objXml->PESSOA->CONTATOS;
	if (is_array($contatos['CONTATO'])) {
		if (count($contatos['CONTATO']) >= 2) {
			if (strpos((string) $contatos["CONTATO"][0]->ds_contato_pessoa, '-') !== false) {
				list($ddd, $telefone) = explode('-', (string) $contatos["CONTATO"][0]->ds_contato_pessoa);
			}
		} elseif (count($contatos) === 1) {
			if (strpos((string) $contatos["CONTATO"][0]->ds_contato_pessoa, '-') !== false) {
				list($ddd, $telefone) = explode('-', (string) $contatos["CONTATO"][0]->ds_contato_pessoa);
			}
		}
	}
	$arrayEmpresa = array(
		'monnomeinicultpar'=> $objXml->PESSOA->no_empresarial_rf,
//		'monemailicp'=>$dado->ENDERECOS->CONTATO->no_fantasia_rf,
		'montelefoneicp'=> $ddd. ' '. $telefone,
	);
	echo simec_json_encode($arrayEmpresa);
	die();
}

/*****************************************************************/

require_once APPRAIZ . "includes/cabecalho.inc";
if ($_REQUEST['campo_tipo_entidade'] == 2) {
	require_once APPRAIZ . "www/includes/webservice/cpf.php";
}
echo '<br/>';

$sqlDocumento = "SELECT docid FROM minc.mcemaiscultura WHERE mceid = {$_SESSION['minc']['mceid']}";
$rsDocumento = !empty($_SESSION['minc']['mceid']) ? $db->pegaLinha($sqlDocumento) : array();

$docid = $rsDocumento['docid'];

$arMnuid = array();
if ($docid) {
	$sqlDocumento = "select * from workflow.documento where docid = {$docid}";
	$resultDocumento = $db->pegaLinha($sqlDocumento);

	if ($resultDocumento && !in_array($resultDocumento['esdid'], array( /*ESTADO_DOCUMENTO_AVALIACAO, ESTADO_DOCUMENTO_FINALIZADO, ESTADO_DOCUMENTO_AVALIADO, */
			ESTADO_DOCUMENTO_ENVIADO_FNDE /*, ESTADO_DOCUMENTO_CORRECAO_CADASTRADOR, ESTADO_DOCUMENTO_AVALIACAO*/))
	) {
		$arMnuid = array(MNUID_AVALIACAO, MNUID_MONITORAMENTO);
	}
} else {
	$arMnuid = array(MNUID_AVALIACAO, MNUID_MONITORAMENTO);
}