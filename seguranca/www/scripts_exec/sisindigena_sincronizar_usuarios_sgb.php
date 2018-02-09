<?php
header( 'Content-Type: text/html; charset=ISO-8859-1' );
//header( 'Content-Type: text/html; charset=UTF-8' );

define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );


error_reporting( E_ALL ^ E_NOTICE );

ini_set("memory_limit", "1024M");
set_time_limit(0);

ini_set( 'soap.wsdl_cache_enabled', '0' );
ini_set( 'soap.wsdl_cache_ttl', 0 );


$_REQUEST['baselogin']  = "simec_espelho_producao";//simec_desenvolvimento

// carrega as funções gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";
require_once APPRAIZ . "includes/workflow.php";
require_once APPRAIZ . "www/sisindigena/_constantes.php";
require_once APPRAIZ . "www/sisindigena/_funcoes.php";


if(!$_SESSION['usucpf']) {
	// CPF do administrador de sistemas
	$_SESSION['usucpforigem'] = '00000000191';
	$_SESSION['usucpf'] = '00000000191';
}
    
   
// abre conexção com o servidor de banco de dados
$db = new cls_banco();

if(!$_REQUEST['somentesincronizar']) {

	$sql = "update sisindigena.identificacaousuario x set muncodatuacao=foo.muncod from (
	select i.iusd, ie.muncod from sisindigena.identificacaousuario i inner join sisindigena.identificaoendereco ie on i.iusd = ie.iusd where muncodatuacao is null
	) foo where x.iusd = foo.iusd";
	
	$db->executar($sql);
	$db->commit();
	
	$sql = "update sisindigena.identificacaousuario x set muncodatuacao=foo.muncod from (
	select i.iusd, fk_cod_municipio_dend as muncod from sisindigena.identificacaousuario i inner join educacenso_2013.tab_docente d on d.num_cpf=i.iuscpf where muncodatuacao is null
	) foo where x.iusd = foo.iusd";
	
	$db->executar($sql);
	$db->commit();
	
	$sql = "update sisindigena.identificacaousuario x set muncodatuacao=foo.muncod from (
	select i.iusd, muncod from sisindigena.identificacaousuario i where muncodatuacao is null and muncod is not null
	) foo where x.iusd = foo.iusd";
	
	$db->executar($sql);
	$db->commit();
	
	$sql = "update sisindigena.identificacaousuario x set muncodatuacao=foo.muncod from (
	select i.iusd, u.muncod from sisindigena.identificacaousuario i
	inner join sisindigena.nucleouniversidade n on n.picid = i.picid
	inner join sisindigena.universidade u on u.uniid = n.uniid
	where muncodatuacao is null
	) foo where x.iusd = foo.iusd";
	
	$db->executar($sql);
	$db->commit();
	
	
	$sql = "update sisindigena.identificacaousuario x set iustipoprofessor='censo' from (
	select i.iusd, c.cpf, i.iustipoprofessor from sisindigena.identificacaousuario i
	inner join sisindigena.tipoperfil t on t.iusd = i.iusd and t.pflcod=1027
	left join sismedio.professoresalfabetizadores c on c.cpf = i.iuscpf
	where c.cpf is not null
	) foo where x.iusd = foo.iusd";
	
	$db->executar($sql);
	$db->commit();
	
	
	$sql = "update sisindigena.identificacaousuario x set uncid=foo.uncid from (
	select i.iusd, n.uncid from sisindigena.identificacaousuario i
	inner join sisindigena.tipoperfil t on t.iusd = i.iusd
	inner join sisindigena.nucleouniversidade n on n.picid = i.picid
	where i.uncid is null and i.picid is not null) foo
	where x.iusd = foo.iusd";

	$db->executar($sql);
	$db->commit();
	
	$sql = "update sisindigena.identificacaousuario x set uncid=foo.uncid_certo, picid=foo.picid_certo from (
			select i.uncid as uncid_certo, i2.uncid, i.picid as picid_certo, i2.picid, i2.iusd from sisindigena.identificacaousuario i 
			inner join sisindigena.tipoperfil t on t.iusd = i.iusd and t.pflcod=1029
			inner join sisindigena.turmas tu on tu.iusd = t.iusd 
			inner join sisindigena.orientadorturma ot on ot.turid = tu.turid 
			inner join sisindigena.identificacaousuario i2 on i2.iusd = ot.iusd 
			inner join sisindigena.tipoperfil t2 on t2.iusd = i2.iusd and t2.pflcod=1027
			where i.uncid!=i2.uncid or i.picid!=i2.picid
			) foo where foo.iusd=x.iusd";
	
	$db->executar($sql);
	$db->commit();
	
	$sql = "UPDATE sisindigena.identificacaousuario SET iusemailprincipal=replace(iusemailprincipal,'@com','@meudominio.com') WHERE iusemailprincipal ilike '%@com%';";
	$db->executar($sql);
	$db->commit();
	
	$sql = "UPDATE sisindigena.identificacaousuario SET iusemailprincipal=replace(iusemailprincipal,'@.','@') WHERE iusemailprincipal ILIKE '%@.%';";
	$db->executar($sql);
	$db->commit();
	
	$sql = "select distinct i.iusd, s.logresponse from sisindigena.identificacaousuario i 
	inner join sisindigena.tipoperfil t on t.iusd = i.iusd 
	inner join log_historico.logsgb_sisindigena s on s.logcpf = i.iuscpf and s.logservico='gravarDadosBolsista' and s.logerro=true
	where cadastradosgb=false and iustermocompromisso=true and logresponse ilike '%Erro: 00026:%';";
	
	$arr = $db->carregar($sql);
	
	if($arr[0]) {
		foreach($arr as $ar) {
			$sl = explode("(",$ar['logresponse']);
			$sl = explode(")",$sl[1]);
			
			$iusnome_antigo = $db->pegaUm("SELECT iusnome FROM sisindigena.identificacaousuario WHERE iusd='".$ar['iusd']."'");
			
			// somente atualizar se os 9 primeiros digitos forem semelhantes
			if(substr(strtoupper($iusnome_antigo),0,9)==substr(strtoupper(trim($sl[0])),0,9)) {
				$sql = "UPDATE sisindigena.identificacaousuario SET iusnome='".trim($sl[0])."' WHERE iusd='".$ar['iusd']."'";
				$db->executar($sql);
			}
		}
		$db->commit();
	}

}

// black list
$pularcpf = $db->carregarColuna("SELECT lnscpf FROM sisindigena.listanegrasgb");


$sql = "SELECT DISTINCT i.iusd, l.logcpf FROM sisindigena.identificacaousuario i 
		LEFT JOIN log_historico.logsgb_sisindigena l ON l.logcpf = i.iuscpf
		WHERE iustermocompromisso=true AND cadastradosgb=false".(($pularcpf)?" AND iuscpf NOT IN('".implode("','",$pularcpf)."')":"")." ORDER BY l.logcpf DESC";
$iusds = $db->carregarColuna($sql);

libxml_use_internal_errors( true );

if($iusds) {
	foreach($iusds as $iusd) {
		
		$lnsid = $db->pegaUm("INSERT INTO sisindigena.listanegrasgb(lnscpf) VALUES ((SELECT iuscpf FROM sisindigena.identificacaousuario WHERE iusd='".$iusd."')) RETURNING lnsid;");
		$db->commit();
		
		sincronizarDadosUsuarioSGB(array("iusd" => $iusd, "sincronizacao" => true));
		
		$db->executar("DELETE FROM sisindigena.listanegrasgb WHERE lnsid='".$lnsid."'");
		$db->commit();
		
	}
}


echo "Sincronizar USUARIOS DO PACTO NO SGB - OK";


$sql = "SELECT uncid FROM sisindigena.universidadecadastro WHERE cadastrosgb=false";
$uncids = $db->carregarColuna($sql);

libxml_use_internal_errors( true );

if($uncids) {
	foreach($uncids as $uncid) {
		sincronizarDadosEntidadeSGB(array("uncid" => $uncid));
	}
}


echo "Sincronizar ENTIDADES DO PACTO NO SGB - OK";


/*
 * ENVIANDO EMAIL CONFIRMANDO O PROCESSAMENTO
 */
require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
$mensagem = new PHPMailer();
$mensagem->persistencia = $db;
$mensagem->Host         = "localhost";
$mensagem->Mailer       = "smtp";
$mensagem->FromName		= "SISINdígena - Sincronizar Usuários SGB";
$mensagem->From 		= $_SESSION['email_sistema'];
$mensagem->AddAddress($_SESSION['email_sistema'], SIGLA_SISTEMA);
$mensagem->Subject = "Sincronizar Usuários SGB";
$mensagem->Body = "Sincronização realizada com sucesso";
$mensagem->IsHTML( true );
$mensagem->Send();
/*
 * FIM
 * ENVIANDO EMAIL CONFIRMANDO O PROCESSAMENTO
 */

if($_SESSION['usucpf'] == '00000000191') {
	
	unset($_SESSION['usucpf']);
	unset($_SESSION['usucpforigem']);
	
}

$db->close();

?>