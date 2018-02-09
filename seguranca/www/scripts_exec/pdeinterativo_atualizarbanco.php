<?php

function getmicrotime()
{list($usec, $sec) = explode(" ", microtime());
 return ((float)$usec + (float)$sec);} 

date_default_timezone_set ('America/Sao_Paulo');

$_REQUEST['baselogin'] = "simec_espelho_producao";

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(0);
/* FIM configurações */

// carrega as funções gerais
//include_once "/var/www/simec/global/config.inc";
include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";

// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '00000000191';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

// atribuindo perfil aos cpfs repetidos
$sql = "select min(pesid) as pesid_atual, usucpf, count(pesid) from pdeinterativo.pessoa where pesstatus='A' OR pesstatus='P' OR pesstatus='B' group by usucpf having count(pesid)>1";
$dados = $db->carregar($sql);

if($dados[0]) {
	foreach($dados as $us) {
		$sql = "INSERT INTO pdeinterativo.pessoatipoperfil (pesid, tpeid) SELECT '".$us['pesid_atual']."' as pesid, tpeid from pdeinterativo.pessoa where usucpf='".$us['usucpf']."' and tpeid is not null group by tpeid";
		$db->executar($sql);
	}
	$db->commit();
}


if($dados[0]) {
	foreach($dados as $us) {
		
		$pesids_antigo = $db->carregarColuna("select pesid from pdeinterativo.pessoa where pesid!='".$us['pesid_atual']."' and usucpf='".$us['usucpf']."'");
		
		$sql = "update pdeinterativo.direcao set pesid='".$us['pesid_atual']."' where pesid in('".implode("','",$pesids_antigo)."');";
		$db->executar($sql);
		$sql = "update pdeinterativo.detalhepessoa set pesid='".$us['pesid_atual']."' where pesid in('".implode("','",$pesids_antigo)."');";
		$db->executar($sql);
		$sql = "update pdeinterativo.pessoagruptrab set pesid='".$us['pesid_atual']."' where pesid in('".implode("','",$pesids_antigo)."');";
		$db->executar($sql);
		$sql = "update pdeinterativo.demaisprofissionais set pesid='".$us['pesid_atual']."' where pesid in('".implode("','",$pesids_antigo)."');";
		$db->executar($sql);
		$sql = "update pdeinterativo.pessoaareaatuacao set pesid='".$us['pesid_atual']."' where pesid in('".implode("','",$pesids_antigo)."');";
		$db->executar($sql);
		$sql = "update pdeinterativo.membroconselho set pesid='".$us['pesid_atual']."' where pesid in('".implode("','",$pesids_antigo)."');";
		$db->executar($sql);
		$sql = "update pdeinterativo.pessoa set pesstatus='I' where pesid in('".implode("','",$pesids_antigo)."')";
		$db->executar($sql);
		$db->commit();
	}
}

$sql = "select count(dpeid), pesid, max(dpeid) as dpeid_atual from pdeinterativo.detalhepessoa where dpestatus='A' group by pesid having count(dpeid)>1";
$detalhespessoa = $db->carregar($sql);

if($detalhespessoa[0]) {
	foreach($detalhespessoa as $dt) {
		
		$dpeids_antigo = $db->carregarColuna("select pesid from pdeinterativo.detalhepessoa where dpeid!='".$dt['dpeid_atual']."' and pesid='".$dt['pesid']."'");
		
		$db->executar("delete from pdeinterativo.detalhepessoa where pesid in('".implode("','",$dpeids_antigo)."')");
		$db->commit();
	}
}


$sql = "delete from pdeinterativo.direcao where pesid in( select pesid from pdeinterativo.pessoa  where pesstatus = 'I' );";
$db->executar($sql);
$sql = "delete from pdeinterativo.detalhepessoa where pesid in( select pesid from pdeinterativo.pessoa  where pesstatus = 'I' );";
$db->executar($sql);
$sql = "delete from pdeinterativo.pessoagruptrab where pesid in( select pesid from pdeinterativo.pessoa  where pesstatus = 'I' );";
$db->executar($sql);
$sql = "delete from pdeinterativo.demaisprofissionais where pesid in( select pesid from pdeinterativo.pessoa  where pesstatus = 'I' );";
$db->executar($sql);
$sql = "delete from pdeinterativo.pessoaareaatuacao where pesid in( select pesid from pdeinterativo.pessoa  where pesstatus = 'I' );";
$db->executar($sql);
$sql = "delete from pdeinterativo.membroconselho where pesid in( select pesid from pdeinterativo.pessoa  where pesstatus = 'I' );";
$db->executar($sql);
$sql = "delete from pdeinterativo.pessoatipoperfil where pesid in(select pesid from pdeinterativo.pessoa  where pesstatus = 'I' )";
$db->executar($sql);
$sql = "delete from pdeinterativo.pessoa  where pesstatus = 'I';";
$db->executar($sql);

$sql = "insert into pdeinterativo.pessoatipoperfil
	(pesid,tpeid)
select pesid,2 from pdeinterativo.pessoa where pflcod = '544' and pesid not in (select pesid from pdeinterativo.pessoatipoperfil where tpeid = 2) ;
";
$db->executar($sql);

$db->commit();
/*
$mensagem = new PHPMailer();
$mensagem->persistencia = $db;
$mensagem->Host         = "localhost";
$mensagem->Mailer       = "smtp";
$mensagem->FromName		= SIGLA_SISTEMA. " - PDEInterativo";
$mensagem->From 		= "noreply@mec.gov.br";
$mensagem->AddAddress( $_SESSION['email_sistema'], SIGLA_SISTEMA );
$mensagem->Subject = SIGLA_SISTEMA. " - PDEInterativo";
$mensagem->Body = "Correção efetuada com sucesso";
$mensagem->IsHTML( true );
$mensagem->Send();
*/

echo "fim";
?>