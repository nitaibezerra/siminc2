<?php

//$_REQUEST['baselogin'] = "simec_espelho_producao";

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

$_SESSION['usucpforigem'] = '';

ini_set('max_execution_time', 1800);
ini_set("memory_limit", "2048M");
//set_time_limit(300);

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$dados = file("SEDUC.csv");

if($dados) {
	$HTML .= "<font size='3'><b>INCIANDO CARGA DE SECRETARIAS ESTADUAIS</b></font><BR>";
	// varrendo todas as secretarias que devem ser atualizadas
	foreach($dados as $d) {
		$detalhes = explode(";",$d);
		// buscando se a secrataria ja esta cadastrada no banco de dados do simec
		$entidade = $db->carregar("SELECT * FROM entidade.entidade ent
								   LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
								   LEFT JOIN entidade.endereco ende ON ende.entid = ent.entid   
								   WHERE (ent.entnumcpfcnpj='".$detalhes[0]."' OR ende.muncod='".$detalhes[4]."') AND fen.funid='6'");
		// se existir
		if($entidade[0]) {
			if(count($entidade)==1) {
				if(!$entidade[0]['entnumcpfcnpj'] && $entidade[0]['entid']) {
					$db->executar("UPDATE entidade.entidade SET entnumcpfcnpj='".$detalhes[0]."' WHERE entid='".$entidade[0]['entid']."'");
					$HTML .= "<b>(".$detalhes[1].")</b> TEVE SEU CNPJ ATUALIZADO PARA ".$detalhes[0]."<BR>";
				}
				if(!$entidade[0]['muncod'] && $entidade[0]['endid']) {
					$db->executar("UPDATE entidade.endereco SET muncod='".$detalhes[4]."' WHERE endid='".$entidade[0]['endid']."'");
					$HTML .= "<b>(".$detalhes[1].")</b> TEVE SEU CÓDIGO DO MUNICIPIO ATUALIZADO PARA ".$detalhes[4]."<BR>";
				}
				// validando se existe secretario(a) estadual
				$secretario = $db->carregar("SELECT * FROM entidade.entidade ent 
							   				 LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
							   				 LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid 
							   				 WHERE fea.entid='".$entidade[0]['entid']."' AND fen.funid='25'");
				
				if($secretario[0]) {
					if(count($secretario)==1) {
						if($secretario[0]['entnumcpfcnpj'] != $detalhes[2]) {
							$db->executar("DELETE FROM entidade.funentassoc WHERE feaid='".$secretario[0]['feaid']."'");
							$entid = $db->pegaUm("INSERT INTO entidade.entidade(entnumcpfcnpj, entnome) VALUES ('".$detalhes[2]."','".$detalhes[3]."') RETURNING entid");
							$fueid = $db->pegaUm("INSERT INTO entidade.funcaoentidade(funid, entid, fuedata, fuestatus) VALUES (25, '".$entid."', NOW(), 'A') RETURNING fueid;");
							$db->executar("INSERT INTO entidade.funentassoc(entid, fueid, feadata) VALUES ('".$entidade[0]['entid']."', '".$fueid."', NOW());");
							$HTML .= "FOI SUBSTITUIDO O SECRETARIO <b>(".$secretario[0]['entnumcpfcnpj'].", ".$secretario[0]['entnome'].")</b> PELO <b>(".$detalhes[2].",".$detalhes[3].")</b><BR>";
						}
					} else {
						$HTML .= "PROBLEMAS | <b>(".$detalhes[1].")</b> POSSUI MAIS DE 1 SECRETARIOS NO BANCO DE DADOS<BR>";
					}
				} else {
					$entid = $db->pegaUm("INSERT INTO entidade.entidade(entnumcpfcnpj, entnome) VALUES ('".$detalhes[2]."','".$detalhes[3]."') RETURNING entid");
					$fueid = $db->pegaUm("INSERT INTO entidade.funcaoentidade(funid, entid, fuedata, fuestatus) VALUES (25, '".$entid."', NOW(), 'A') RETURNING fueid;");
					$db->executar("INSERT INTO entidade.funentassoc(entid, fueid, feadata) VALUES ('".$entidade[0]['entid']."', '".$fueid."', NOW());");
					$HTML .= "NOVO SECRETARIO <b>(".$detalhes[2].",".$detalhes[3].")</b> FOI ADICIONADO NA <b>(".$detalhes[1].")</b><BR>";
				}
			} else {
				$HTML .= "PROBLEMAS | <b>(".$detalhes[1].", CNPJ ".$detalhes[0].", MUN ".$detalhes[4].")</b> POSSUI MAIS DE 1 REGISTRO NO BANCO DE DADOS<BR>";
			}
		} else { // caso não exista, inserir nova
			$entidsec = $db->pegaUm("INSERT INTO entidade.entidade(entnumcpfcnpj, entnome) VALUES ('".$detalhes[0]."','".$detalhes[1]."') RETURNING entid");
			$db->executar("INSERT INTO entidade.funcaoentidade(funid, entid, fuedata, fuestatus) VALUES (6, '".$entidsec."', NOW(), 'A');");
			$entid = $db->pegaUm("INSERT INTO entidade.entidade(entnumcpfcnpj, entnome) VALUES ('".$detalhes[2]."','".$detalhes[3]."') RETURNING entid");
			$fueid = $db->pegaUm("INSERT INTO entidade.funcaoentidade(funid, entid, fuedata, fuestatus) VALUES (25, '".$entid."', NOW(), 'A') RETURNING fueid;");
			$db->executar("INSERT INTO entidade.funentassoc(entid, fueid, feadata) VALUES ('".$entidsec."', '".$fueid."', NOW());");
			$HTML .= "NOVA SECRETARIA INSERIDA : <b>(".$detalhes[1].")</b> | NOVO SECRETARIO INSERIDO : <b>(".$detalhes[3].")</b><BR>";
		}
		$HTML .= "<i>".$entidade[0]['entnome']." FOI PROCESSADA</i><br>";
		ob_flush();
		$db->commit();
	}
	$HTML .= "<font size='3'><b>FINALIZANDO CARGA DE SECRETARIAS ESTADUAIS</b></font><BR>";
}

$dados = file("PREFEITURAS.csv");

if($dados) {
	$HTML .= "<font size='3'><b>INCIANDO CARGA DE PREFEITURAS</b></font><BR>";
	// varrendo todas as PREFEITURAS que devem ser atualizadas
	foreach($dados as $key => $d) {
		$detalhes = explode(";",$d);
		// buscando se a prefeitura ja esta cadastrada no banco de dados do simec
		$entidade = $db->carregar("SELECT * FROM entidade.entidade ent
								   LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
								   LEFT JOIN entidade.endereco ende ON ende.entid = ent.entid   
								   WHERE (ent.entnumcpfcnpj='".$detalhes[0]."' OR ende.muncod='".$detalhes[4]."') AND fen.funid='1'");
		// se existir
		if($entidade[0]) {
			if(count($entidade)==1) {
				if(!$entidade[0]['entnumcpfcnpj'] && $entidade[0]['entid']) {
					$db->executar("UPDATE entidade.entidade SET entnumcpfcnpj='".$detalhes[0]."' WHERE entid='".$entidade[0]['entid']."'");
					$HTML .= "<b>(".$detalhes[1].")</b> TEVE SEU CNPJ ATUALIZADO PARA ".$detalhes[0]."<BR>";
				}
				if(!$entidade[0]['muncod'] && $entidade[0]['endid']) {
					$db->executar("UPDATE entidade.endereco SET muncod='".$detalhes[4]."' WHERE endid='".$entidade[0]['endid']."'");
					$HTML .= "<b>(".$detalhes[1].")</b> TEVE SEU CÓDIGO DO MUNICIPIO ATUALIZADO PARA ".$detalhes[4]."<BR>";
				}
				// validando se existe secretario(a) estadual
				$prefeito = $db->carregar("SELECT * FROM entidade.entidade ent 
							   				 LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
							   				 LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid 
							   				 WHERE fea.entid='".$entidade[0]['entid']."' AND fen.funid='2'");
				if($prefeito[0]) {
					if(count($prefeito)==1) {
						if($prefeito[0]['entnumcpfcnpj'] != $detalhes[2]) {
							$db->executar("DELETE FROM entidade.funentassoc WHERE feaid='".$prefeito[0]['feaid']."'");
							$entid = $db->pegaUm("INSERT INTO entidade.entidade(entnumcpfcnpj, entnome) VALUES ('".$detalhes[2]."','".addslashes($detalhes[3])."') RETURNING entid");
							$fueid = $db->pegaUm("INSERT INTO entidade.funcaoentidade(funid, entid, fuedata, fuestatus) VALUES (2, '".$entid."', NOW(), 'A') RETURNING fueid;");
							$db->executar("INSERT INTO entidade.funentassoc(entid, fueid, feadata) VALUES ('".$entidade[0]['entid']."', '".$fueid."', NOW());");
							$HTML .= "FOI SUBSTITUIDO O PREFEITO <b>(".$prefeito[0]['entnumcpfcnpj'].", ".$prefeito[0]['entnome'].")</b> PELO <b>(".$detalhes[2].",".$detalhes[3].")</b><BR>";
						}
					} else {
						$HTML .= "PROBLEMAS | <b>(".$detalhes[1].")</b> POSSUI MAIS DE 1 PREFEITO NO BANCO DE DADOS<BR>";
					}
				} else {
					$entid = $db->pegaUm("INSERT INTO entidade.entidade(entnumcpfcnpj, entnome) VALUES ('".$detalhes[2]."','".$detalhes[3]."') RETURNING entid");
					$fueid = $db->pegaUm("INSERT INTO entidade.funcaoentidade(funid, entid, fuedata, fuestatus) VALUES (2, '".$entid."', NOW(), 'A') RETURNING fueid;");
					$db->executar("INSERT INTO entidade.funentassoc(entid, fueid, feadata) VALUES ('".$entidade[0]['entid']."', '".$fueid."', NOW());");
					$HTML .= "NOVO PREFEITO <b>(".$detalhes[2].",".$detalhes[3].")</b> FOI ADICIONADO NA <b>(".$detalhes[1].")</b>";
				}
			} else {
				$HTML .= "PROBLEMAS | <b>(".$detalhes[1].", CNPJ ".$detalhes[0].", MUN ".$detalhes[4].")</b> POSSUI MAIS DE 1 REGISTRO NO BANCO DE DADOS<BR>";
			}
		} else { // caso não exista, inserir nova
			$entidpref = $db->pegaUm("INSERT INTO entidade.entidade(entnumcpfcnpj, entnome) VALUES ('".$detalhes[0]."','".$detalhes[1]."') RETURNING entid");
			$db->executar("INSERT INTO entidade.funcaoentidade(funid, entid, fuedata, fuestatus) VALUES (1, '".$entidpref."', NOW(), 'A');");
			$entid = $db->pegaUm("INSERT INTO entidade.entidade(entnumcpfcnpj, entnome) VALUES ('".$detalhes[2]."','".$detalhes[3]."') RETURNING entid");
			$fueid = $db->pegaUm("INSERT INTO entidade.funcaoentidade(funid, entid, fuedata, fuestatus) VALUES (2, '".$entid."', NOW(), 'A') RETURNING fueid;");
			$db->executar("INSERT INTO entidade.funentassoc(entid, fueid, feadata) VALUES ('".$entidpref."', '".$fueid."', NOW());");
			$HTML .= "NOVA PREFEITURA INSERIDA : <b>(".$detalhes[1].")</b> | NOVO PREFEITO INSERIDO : <b>(".$detalhes[3].")</b><BR>";
		}
		$HTML .= "<i>".($key+1)." : ".$entidade[0]['entnome']." FOI PROCESSADA</i><br>";
		ob_flush();
		$db->commit();
	}
	$HTML .= "<font size='3'><b>FINALIZANDO CARGA DE PREFEITURAS</b></font><BR>";
}

/*
 * ENVIANDO EMAIL CONFIRMANDO O PROCESSAMENTO
 */
require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
$mensagem = new PHPMailer();
$mensagem->persistencia = $db;
$mensagem->Host         = "localhost";
$mensagem->Mailer       = "smtp";
$mensagem->FromName		= "SISTEMA DE CARGA";
$mensagem->From 		= "simec@mec.gov.br";
$mensagem->AddAddress( "alexandre.dourado@mec.gov.br", "Alexandre Dourado" );
$mensagem->Subject = "Carga do daniel chegou no fim";
$mensagem->Body = $HTML;
$mensagem->IsHTML( true );
return $mensagem->Send();
/*
 * FIM
 * ENVIANDO EMAIL CONFIRMANDO O PROCESSAMENTO
 */

?>