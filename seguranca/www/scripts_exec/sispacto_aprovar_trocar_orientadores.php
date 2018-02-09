<?php
header( 'Content-Type: text/html; charset=ISO-8859-1' );

define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );


error_reporting( E_ALL ^ E_NOTICE );

ini_set("memory_limit", "1024M");
set_time_limit(0);

$_REQUEST['baselogin']  = "simec_espelho_producao";//simec_desenvolvimento

// carrega as funções gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";
require_once APPRAIZ . "www/sispacto/_constantes.php";
require_once APPRAIZ . "www/sispacto/_funcoes.php";
require_once APPRAIZ . "www/sispacto/_funcoes_coordenadorlocal.php";
require_once APPRAIZ . "includes/workflow.php";

require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';


// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';
    
   
// abre conexção com o servidor de banco de dados
$db = new cls_banco();


$sql = "SELECT p.picid, 
			   p.docid,
			   COALESCE((SELECT iusnome FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON i.iusd=t.iusd WHERE i.picid=p.picid AND t.pflcod=".PFL_COORDENADORLOCAL."),'Coordenador Local não cadastrado') as coordenadorlocal,
			   COALESCE((SELECT iusemailprincipal FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON i.iusd=t.iusd WHERE i.picid=p.picid AND t.pflcod=".PFL_COORDENADORLOCAL."),'Coordenador Local não cadastrado') as coordenadorlocalemail,
			   CASE WHEN m.muncod IS NOT NULL THEN m.estuf||' - '||m.mundescricao ELSE e.estuf||' - '||e.estdescricao END as descricao  
		FROM sispacto.pactoidadecerta p 
		LEFT JOIN territorios.municipio m ON m.muncod = p.muncod 
		LEFT JOIN territorios.estado e ON e.estuf = p.estuf 
		INNER JOIN workflow.documento d ON d.docid = p.docid 
		WHERE d.esdid='".ESD_ANALISE_TROCANDO_ORIENTADORES_COORDENADOR_LOCAL."'";

$pactoidadecerta = $db->carregar($sql);

if($pactoidadecerta) {
	foreach($pactoidadecerta as $pi) {
		wf_alterarEstado( $pi['docid'], AED_AUTORIZAR_TROCA_ORIENTADORES, $cmddsc = 'Autorização automática. Os documentos estão sendo analisados e caso tenha alguma irregularidade, a autorização poderá ser cancelada.', array('picid' => $pi['picid']) );
		
		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;
		
		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISPACTO - Substituição dos Orientadores de Estudo autorizada";
		
		$mensagem->AddAddress( $pi['coordenadorlocalemail'], $pi['coordenadorlocal'] );
		
			
		$mensagem->Body = "<p>Prezado(a) ".$pi['coordenadorlocal']." (Coordenador(a) do Pacto),</p>
				 		   <p>Você solicitou a substituição de um ou mais Orientadores de Estudo do seu município/ estado. Por favor, acesse novamente o SisPacto para dar continuidade à troca. Para saber como proceder, acesse o Manual de Orientações para substituição dos Orientadores de Estudo, disponível no site do Pacto (http://pacto.mec.gov.br). Lembre-se de que a data limite para concluir a troca termina, impreterivelmente, no próximo dia 15 de fevereiro de 2013.</p> 
				 		   <p>Secretaria de Educação Básica<br/>Ministério da Educação</p>";
		
		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		echo $pi['descricao']." : ".$resp."<br>";
		
	}
}

?>