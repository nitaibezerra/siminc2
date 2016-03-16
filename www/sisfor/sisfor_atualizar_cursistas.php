<?
$_REQUEST['baselogin'] = "simec_espelho_producao";

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(0);
/* FIM configurações */


// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/workflow.php";

include "_constantes.php";
include "_funcoes.php";

$_SESSION['sisid'] = SIS_SISFOR;

// CPF do administrador de sistemas
if(!$_SESSION['usucpf']) {
	$_SESSION['usucpforigem'] = '';
	$_SESSION['usucpf'] = '';
}

?>
<html>
<head>
	<title>SIMEC- Sistema Integrado de Monitoramento do Ministério da Educação</title>
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
</head>
<body topmargin="0" leftmargin="0">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
<tr>
	<td width="25%"><img src="/includes/layout/azul/img/logo.png" border="0" /></td>
	<td valign="middle" style="font-size:15px;"><b>Atualização dos dados complementares dos cursistas - SISFOR</b></td>
</tr>
</table>
<?

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

if($_REQUEST['requisicao']=='gravarInformacoesCursistas') {
	gravarInformacoesCursistas($_REQUEST);
	?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
<tr>
	<td valign="middle" style="font-size:15px;"><h1>Obrigado pela atualização, essas informações serão muito importantes para a melhoria dos próximos cursos</h1></td>
</tr>
</table>
	<?
	exit;
}


$curid = base64_decode( $_REQUEST['curid']);
$sifid = base64_decode( $_REQUEST['sifid']);

if(is_numeric($curid) && is_numeric($sifid)) {
	$_SESSION['sisfor']['sifid'] = $sifid;
	exibirInformacoesCursistas(array('curid'=>$curid,'noredirect' => true));
} else {
	echo '[ PROBLEMAS COM AS INFORMAÇÕES ]';
}

?>
</body>
</html>