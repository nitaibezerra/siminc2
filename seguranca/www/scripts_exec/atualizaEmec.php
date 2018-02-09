<?php
// carrega as funções gerais
include_once "/var/www/simec/global/config.inc";
//include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

/* configurações do relatorio - Memoria limite de 3000 Mbytes */
ini_set("memory_limit", "3000M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 3000 Mbytes */

// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '00000000191';

$db = new cls_banco();

//Parâmetros
$database = dbemec;
$host = "10.1.3.143";
$user = sysdbemec_consulta;
$password = sysdbemec_consulta;
$porta = "6545";

//ATUALIZA IES
$sql = "SELECT co_ies, no_ies, sg_ies, tp_organizacao_gn, co_mantenedora
FROM dblink
	('dbname=$database
	hostaddr=$host
	user=$user
	password=$password
	port=$porta',
	'SELECT distinct co_ies, no_ies, sg_ies, tp_organizacao_gn, co_mantenedora FROM emec_cadastro.vw_cdst_ies'
	) AS RS
(
co_ies integer,
no_ies character varying(200),
sg_ies character varying(20),
tp_organizacao_gn integer,
co_mantenedora integer
)";
$arDadosIES = $db->carregar($sql);

foreach ($arDadosIES as $ies) {
	$co_ies = $ies['co_ies'];
	$no_ies = $ies['no_ies'];
	$sg_ies = $ies['sg_ies'];
	$tp_organizacao_gn = $ies['tp_organizacao_gn'];
	$co_mantenedora = $ies['co_mantenedora'];
	$existeIES = $db->pegaUm("SELECT COUNT(0) FROM emec.ies WHERE co_ies = ".$co_ies);
	if( (int) $existeIES == 0 ){
		$sql = "INSERT INTO emec.ies(co_ies, no_ies, sg_ies, tp_organizacao_gn, co_mantenedora)
			VALUES ($co_ies, '$no_ies', '$sg_ies', $tp_organizacao_gn, $co_mantenedora)";
		$db->executar( $sql );
	}else{
		$sql = "UPDATE emec.ies SET
					tp_organizacao_gn = $tp_organizacao_gn,
					co_mantenedora = $co_mantenedora
				WHERE co_ies = $co_ies";
        $db->executar( $sql );
	}
}
$db->commit();

//ATUALIZA CURSOS
$sql = "SELECT co_curso, co_ies, no_curso
FROM dblink
	('dbname=$database
	hostaddr=$host
	user=$user
	password=$password
	port=$porta',
	'SELECT distinct co_curso, co_ies, no_curso FROM emec_cadastro.vw_cdst_ies_curso WHERE co_curso IS NOT NULL AND co_habilitacao IS NULL'
	) AS RS
(
co_curso integer,
co_ies integer,
no_curso varchar(300)
)";
$arDadosCursos = $db->carregar($sql);

foreach ($arDadosCursos as $curso) {
	$co_curso = $curso['co_curso'];
	$co_ies = $curso['co_ies'];
	$no_curso = $curso['no_curso'];
	$existeCursos = $db->pegaUm("SELECT COUNT(0) FROM emec.cursos WHERE co_curso = ".$co_curso." AND co_ies = ".$co_ies);
	if( (int) $existeCursos == 0 ){
		$sql = "INSERT INTO emec.cursos (co_curso, co_ies, no_curso)
			VALUES ($co_curso, $co_ies, '$no_curso')";
		$db->executar( $sql );
	}
}
$db->commit();

//ATUALIZA ENDEREÇOS
$sql = "SELECT co_ies_endereco, co_ies, co_municipio, no_endereco, no_bairro, no_campus
FROM dblink
	('dbname=$database
	hostaddr=$host
	user=$user
	password=$password
	port=$porta',
	'SELECT DISTINCT co_ies_endereco, co_ies, co_municipio, no_endereco, no_bairro, no_campus FROM emec_cadastro.vw_cdst_ies_endereco'
	) AS RS
(
co_ies_endereco integer,
co_ies integer,
co_municipio character varying(15),
no_endereco character varying(100),
no_bairro character varying(50),
no_campus character varying(500)
)";
$arDadosEnderecos = $db->carregar($sql);

foreach ($arDadosEnderecos as $endereco) {
	$co_ies_endereco = $endereco['co_ies_endereco'];
	$co_ies = $endereco['co_ies'];
	$co_municipio = $endereco['co_municipio'];
	$no_endereco = addslashes($endereco['no_endereco']);
	$no_bairro = addslashes($endereco['no_bairro']);
	$no_campus = addslashes($endereco['no_campus']);
	$existeEnderecos = $db->pegaUm("SELECT COUNT(0) FROM emec.ies_endereco WHERE co_ies_endereco = ".$co_ies_endereco);
	if( (int) $existeEnderecos == 0 ){
		$sql = "INSERT INTO emec.ies_endereco (co_ies_endereco, co_ies, co_municipio, no_endereco, no_bairro, no_campus)
			VALUES ($co_ies_endereco, $co_ies, '$co_municipio', '$no_endereco', '$no_bairro', '$no_campus')";
		$db->executar( $sql );
	}else{
		$sql = "UPDATE emec.ies_endereco SET
					co_municipio = '$co_municipio',
					no_bairro = '$no_bairro',
					no_campus = '$no_campus'
				WHERE co_ies_endereco = $co_ies_endereco";
        	$db->executar( $sql );
	}
}
$db->commit();

/*
 * ENVIANDO EMAIL CONFIRMANDO O PROCESSAMENTO
 */
require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
$mensagem = new PHPMailer();
$mensagem->persistencia = $db;
$mensagem->Host         = "localhost";
$mensagem->Mailer       = "smtp";
$mensagem->FromName		= "WS Atualizar EMEC";
$mensagem->From 		= $_SESSION['email_sistema'];
$mensagem->AddAddress($_SESSION['email_sistema']);
$mensagem->Subject = "WS Atualizar EMEC";

$mensagem->Body = $corpoemail;
$mensagem->IsHTML( true );
$mensagem->Send();
/*
 * FIM
 * ENVIANDO EMAIL CONFIRMANDO O PROCESSAMENTO
 */

$db->close();
?>