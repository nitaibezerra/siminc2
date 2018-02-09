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
require_once APPRAIZ . "www/escolaterra/_constantes.php";
require_once APPRAIZ . "www/escolaterra/_funcoes.php";



function sincronizarDadosEntidadeMunicipiosSGB($dados) {
	global $db;

	set_time_limit( 0 );

	ini_set( 'soap.wsdl_cache_enabled', '0' );
	ini_set( 'soap.wsdl_cache_ttl', 0 );

	$opcoes = Array(
			'exceptions'	=> 0,
			'trace'			=> true,
			//'encoding'		=> 'UTF-8',
			'encoding'		=> 'ISO-8859-1',
			'cache_wsdl'    => WSDL_CACHE_NONE
	);

	$soapClient = new SoapClient( WSDL_CAMINHO_CADASTRO , $opcoes );

	libxml_use_internal_errors( true );

	$sql = "SELECT entnumcpfcnpj, entnome, m.muncod, m.estuf FROM par.entidade e INNER JOIN territorios.municipio m ON m.muncod = e.muncod WHERE e.entid='".$dados['entid']."'";
	$dadosentidade = $db->pegaLinha($sql);

	$xmlRetornoEntidade = $soapClient->lerDadosEntidade( array('sistema'           => SISTEMA_SGB,
			'login'            => USUARIO_SGB,
			'senha'            => SENHA_SGB,
			'nu_cnpj_entidade' => $dadosentidade['entnumcpfcnpj']
	) );

	inserirDadosLog(array('logrequest'=>$soapClient->__getLastRequest(),'logresponse'=>$soapClient->__getLastResponse(),'logcnpj'=>$dadosentidade['entnumcpfcnpj'],'logservico'=>'lerDadosEntidade'));

	preg_match("/<nu_cnpj_entidade>(.*)<\\/nu_cnpj_entidade>/si", $xmlRetornoEntidade, $match);

	$existecnpj = (string) $match[1];

	$dadosEntidade = array( 'sistema'          => SISTEMA_SGB,
			'login'            => USUARIO_SGB,
			'senha'            => SENHA_SGB,
			'nu_cnpj_entidade' => $dadosentidade['entnumcpfcnpj'],
			'co_tipo_entidade' => '1',
			'no_entidade'      => $dadosentidade['entnome'],
			'sg_entidade'      => '',
			'co_municipio'     => $dadosentidade['muncod'],
			'sg_uf'            => $dadosentidade['estuf']
	);

	$xmlRetorno_gravaDadosEntidade   = $soapClient->gravaDadosEntidade( $dadosEntidade );

	$logerro_gravaDadosEntidade = analisaCodXML($xmlRetorno_gravaDadosEntidade,'10001');

	inserirDadosLog(array('logrequest'=>$soapClient->__getLastRequest(),'logresponse'=>$soapClient->__getLastResponse(),'logcnpj'=>$dadosentidade['entnumcpfcnpj'],'logservico'=>'gravaDadosEntidade','logerro' => $logerro_gravaDadosEntidade));

	if($existecnpj) $logerro_gravaDadosEntidade = 'FALSE';
	if(analisaCodXML($xmlRetorno_gravaDadosEntidade,'00036') == 'FALSE') $logerro_gravaDadosEntidade = 'FALSE';

	$sql = "UPDATE escolaterra.entidadecadastro SET cadastradosgb=".(($logerro_gravaDadosEntidade=='TRUE')?'FALSE':'TRUE')." WHERE entid='".$dados['entid']."'";
	$db->executar($sql);
	$db->commit();

}


if(!$_SESSION['usucpf']) {
	// CPF do administrador de sistemas
	$_SESSION['usucpforigem'] = '00000000191';
	$_SESSION['usucpf'] = '00000000191';
}
    
   
// abre conexção com o servidor de banco de dados
$db = new cls_banco();

// black list

$pularcpf = $db->carregarColuna("SELECT lnscpf FROM escolaterra.listanegrasgb");


$sql = "SELECT DISTINCT i.iusid, l.logcpf FROM escolaterra.identificacaousuario i 
		LEFT JOIN log_historico.logsgb_escolaterra l ON l.logcpf = i.iuscpf
		WHERE iustermocompromisso=true AND cadastradosgb=false".(($pularcpf)?" AND iuscpf NOT IN('".implode("','",$pularcpf)."')":"")." ORDER BY l.logcpf DESC";
$iusids = $db->carregarColuna($sql);

libxml_use_internal_errors( true );

if($iusids) {
	foreach($iusids as $iusid) {
		
		$lnsid = $db->pegaUm("INSERT INTO escolaterra.listanegrasgb(lnscpf) VALUES ((SELECT iuscpf FROM escolaterra.identificacaousuario WHERE iusid='".$iusid."')) RETURNING lnsid;");
		$db->commit();
		
		sincronizarDadosUsuarioSGB(array("iusid" => $iusid, "sincronizacao" => true));
		
		$db->executar("DELETE FROM escolaterra.listanegrasgb WHERE lnsid='".$lnsid."'");
		$db->commit();
		
	}
}


echo "Sincronizar USUARIOS DO ESCOLA DA TERRA NO SGB - OK";


$sql = "SELECT ufpid FROM escolaterra.ufparticipantes WHERE (cadastrosgb=false OR cadastrosgb IS NULL) AND ufpcnpj IS NOT NULL";
$ufpids = $db->carregarColuna($sql);

libxml_use_internal_errors( true );

if($ufpids) {
	foreach($ufpids as $ufpid) {
		sincronizarDadosEntidadeSGB(array("ufpid" => $ufpid));
	}
}


echo "Sincronizar ENTIDADES DO ESCOLA DA TERRA NO SGB - OK";

$sql = "SELECT e.entid FROM escolaterra.entidadecadastro e 
		INNER JOIN par.entidade ent ON ent.entid = e.entid 
		WHERE (cadastradosgb=false OR cadastradosgb IS NULL) AND ent.muncod IN(
		select distinct i.muncodatuacao from escolaterra.pagamentobolsista p 
		inner join escolaterra.identificacaousuario i on i.iusid = p.iusid 
		inner join workflow.documento d on d.docid = p.docid 
		where d.esdid in(1348,1345) and i.muncodatuacao is not null
		)";

$entids = $db->carregarColuna($sql);

libxml_use_internal_errors( true );

if($entids) {
	foreach($entids as $entid) {
		sincronizarDadosEntidadeMunicipiosSGB(array("entid" => $entid));
	}
}


echo "Sincronizar ENTIDADES PREF DO ESCOLA DA TERRA NO SGB - OK";


// ATUALIZANDO NOME DO CENSO COM NOME VINDO DA RECEITA FEDERAL (SOMENTE SE OS 9 PRIMEIROS DIGITOS FOREM IGUAIS)

$sql = "select distinct i.iusid, s.logresponse FROM escolaterra.identificacaousuario i
		inner join escolaterra.tipoperfil t on t.iusid = i.iusid
		inner join log_historico.logsgb_escolaterra s on s.logcpf = i.iuscpf and s.logservico='gravarDadosBolsista' and s.logerro=true
		where cadastradosgb=false and iustermocompromisso=true and logresponse ilike '%Erro: 00026:%';";

$arr = $db->carregar($sql);

if($arr[0]) {
	foreach($arr as $ar) {
		$sl = explode("(",$ar['logresponse']);
		$sl = explode(")",$sl[1]);
			
		$iusnome_antigo = $db->pegaUm("select iusnome FROM escolaterra.identificacaousuario where iusid='".$ar['iusid']."'");
			
		// somente atualizar se os 9 primeiros digitos forem semelhantes
		if(substr(strtoupper($iusnome_antigo),0,9)==substr(strtoupper(trim($sl[0])),0,9)) {
			$sql = "UPDATE escolaterra.identificacaousuario set iusnome='".trim($sl[0])."' where iusid='".$ar['iusid']."'";
			$db->executar($sql);
		}
	}
	$db->commit();
}




/*
 * ENVIANDO EMAIL CONFIRMANDO O PROCESSAMENTO
 */
/*
require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
$mensagem = new PHPMailer();
$mensagem->persistencia = $db;
$mensagem->Host         = "localhost";
$mensagem->Mailer       = "smtp";
$mensagem->FromName		= "Escola da terra 2014 - Sincronizar Usuários SGB";
$mensagem->From 		= $_SESSION['email_sistema'];
$mensagem->AddAddress($_SESSION['email_sistema'], SIGLA_SISTEMA);
$mensagem->Subject = "Sincronizar Usuários SGB";
$mensagem->Body = "Sincronização realizada com sucesso";
$mensagem->IsHTML( true );
$mensagem->Send();
*/
/*
 * FIM
 * ENVIANDO EMAIL CONFIRMANDO O PROCESSAMENTO
 */

if($_SESSION['usucpf'] == '00000000191') {
	
	unset($_SESSION['usucpf']);
	unset($_SESSION['usucpforigem']);
	
}

?>