<?
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
include_once APPRAIZ . "includes/workflow.php";
include_once APPRAIZ . "www/sispacto/_constantes.php";
include_once APPRAIZ . "www/sispacto/_funcoes.php";
include_once APPRAIZ . "www/sispacto/_funcoes_coordenadorlocal.php";

// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();
?>
<br>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<link rel="stylesheet" type="text/css" href="../includes/listagem2.css">
<?php
//header("Cache-Control: no-store, no-cache, must-revalidate");// HTTP/1.1
//header("Cache-Control: post-check=0, pre-check=0", false);
//header("Pragma: no-cache");// HTTP/1.0 Canhe Livre
//header("Expires: Mon, 26 Jul 1997 05:00:00 GMT"); // Date in the past
//

//include APPRAIZ . 'includes/cabecalho.inc';
#include APPRAIZ . 'includes/Agrupador.php';
print '<br/>';
monta_titulo('Usuários Online', '');


$sql = "SELECT
			sisdsc AS sisdsc,
			qtdonline
		FROM
			seguranca.qtdusuariosonline u 
			INNER JOIN seguranca.sistema s ON s.sisid = u.sisid
		ORDER BY
			qtdonline DESC";


include APPRAIZ. 'includes/classes/relatorio.class.inc';

$dados = $db->carregar($sql);
$agrup = array(
				"agrupador" => array(
									 array(
											"campo" => "sisdsc",
									  		"label" => "Sistema"										
							   			   )
									),
				"agrupadoColuna" => array(
									   		"qtdonline", 
										  )	  
				);				
$coluna = array(
				array(
					  "campo" 	 => "qtdonline",
				   	  "label" 	 => "Usuários Online",
					  "type"     => "numeric"
					 )				
				);
//dbg($sql,1);
$r = new montaRelatorio();
$r->setAgrupador($agrup, $dados); 
$r->setColuna($coluna);
$r->setTotalizador(false);
echo $r->getRelatorio();

//$cabecalho = array("Sistema", "Usuários Online");
//$db->monta_lista($sql, $cabecalho, 50, 10, '','','');
?>
<script>
function abreChat(sisid){
	var j = window.open('/geral/usuarios_online.php?sisid=' + sisid, 'chat', 'location=0,status=1,scrollbars=0,width=600,height=500');
	j.focus();
	j.moveTo(150,100);;
}
</script>