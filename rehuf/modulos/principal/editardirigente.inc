<html>
<head>
<meta http-equiv="Cache-Control" content="no-cache">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="Connection" content="Keep-Alive">
<meta http-equiv="Expires" content="Mon, 26 Jul 1997 05:00:00 GMT">
<title>Dados do Dirigente - REHUF</title>
<script language="JavaScript" src="../includes/calendario.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script language="JavaScript" src="./js/rehuf.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script type="text/javascript">
this._closeWindows = false;
</script>
</head>
<body style="margin:10px; padding:0; background-color: #fff; background-image: url(../imagens/fundo.gif); background-repeat: repeat-y;">
<div>
<?
require_once APPRAIZ . "includes/classes/entidades.class.inc";

if(validaVariaveisSistema()) {
	echo "<script>
			alert('Problemas nas variáveis do sistema.');
			window.opener.location='?modulo=inicio&acao=A';
			window.close();
		  </script>";
	exit;
}
switch($_REQUEST['acao']) {
	case 'A';
		$_FUNCOESAUTORIZADAS = array('salvarRegistroDirigente'=>true,
									 'salvarFormacaoDirigentes'=>true);
	break;
}
if($_REQUEST['requisicao']) {
	if($_FUNCOESAUTORIZADAS[$_REQUEST['requisicao']]) {
		$_REQUEST['requisicao']($_REQUEST);
	}
}

if($_REQUEST['funid']) {
	$_SESSION['rehuf_var']['funid'] = $_REQUEST['funid'];
}

/* Verifica se o dirigente esta vinculado a uma função, caso não, imprimir erro */
if($_SESSION['rehuf_var']['funid']) {
$entidade = new Entidades();
$entidade->carregarPorFuncaoEntAssociado($_SESSION['rehuf_var']['funid'],$_SESSION['rehuf_var']['entid']);
echo $entidade->formEntidade("rehuf.php?modulo=principal/editardirigente&acao=A&requisicao=salvarRegistroDirigente",
							 array("funid" => $_SESSION['rehuf_var']['funid'], "entidassociado" => $_SESSION['rehuf_var']['entid']),
							 array("enderecos"=>array(1))
							 );

?>
<script type="text/javascript"><!--
$('frmEntidade').onsubmit  = function(e) {
	if (trim($F('entnumcpfcnpj')) == '') {
		alert('CPF é obrigatório.');
    	return false;
	}
	if (trim($F('entnome')) == '') {
		alert('O nome da entidade é obrigatório.');
		return false;
	}
	if (trim($F('entdatanasc')) != '') {
		if(!validaData(document.getElementById('entdatanasc'))) {
			alert("Data de nascimento é inválida.");return false;
		}
	}
	return true;
}
</script>
<?
	echo "<br />";
	if($entidade->getEntId()) {
		echo "<form name='formulario' id='formulario' method='post' action='rehuf.php?modulo=principal/editardirigente&acao=A&requisicao=salvarFormacaoDirigentes' onsubmit='return validarFormacaoDirigentes(this);'>";
		if($_REQUEST['edtid']) {
			$entidadedetalhe = $db->carregar("SELECT * FROM rehuf.entidadedetalhe WHERE edtid='".$_REQUEST['edtid']."'");
			if($entidadedetalhe[0]) {
				$entidadedetalhe=current($entidadedetalhe);
				$edtcurso = $entidadedetalhe['edtcurso'];
				$edtlocalcurso = $entidadedetalhe['edtlocalcurso'];
				$edtdtconclusaocurso = $entidadedetalhe['edtdtconclusaocurso'];
				$edtnrhorascurso = $entidadedetalhe['edtnrhorascurso'];
				echo "<input type='hidden' name='edtid' value='".$entidadedetalhe['edtid']."'>";
			}
		}
		echo "<input type='hidden' name='funid' value='". $_SESSION['rehuf_var']['funid'] ."'>";
		echo "<input type='hidden' name='entid' value='". $entidade->getEntId() ."'>";
		echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">";
		echo "<tr><td colspan='2'><strong>Formação dos dirigentes</strong></td></tr>";
		echo "<tr><td class=\"SubTituloDireita\">Nome do curso :</td><td>". campo_texto('edtcurso', "S", "S", "Nome do curso", 40, 100, "", "", '', '', 0, '' ) ."</td>";
		echo "<tr><td class=\"SubTituloDireita\">Local de realização :</td><td>". campo_texto('edtlocalcurso', "S", "S", "Local do curso", 40, 100, "", "", '', '', 0, '' ) ."</td>";
		echo "<tr><td class=\"SubTituloDireita\">Data de conclusão :</td><td>".campo_data( 'edtdtconclusaocurso', 'N', 'S', '', 'S' )."</td>";
		echo "<tr><td class=\"SubTituloDireita\">Número de horas :</td><td>". campo_texto('edtnrhorascurso', "S", "S", "Descrição do item", 20, 40, "######", "", '', '', 0, '' ) ."</td>";
		echo "<tr><td class=\"SubTituloCentro\" colspan='2'><input type='submit' value='Gravar' ><input type='button' value='Novo' onclick=\"window.location='?modulo=principal/editardirigente&acao=A&funid=".$_SESSION['rehuf_var']['funid']."';\"></td>";
		echo "</table>";
		echo "</form>";
		echo "<br />";
		$sql = "SELECT '<center><img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"window.location=\'?modulo=principal/editardirigente&acao=A&funid=".$_SESSION['rehuf_var']['funid']."&edtid='|| entd.edtid ||'\'\"><img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"Excluir(\'?modulo=principal/editardirigente&acao=A&alterabd=salvarFormacaoDirigentes&funid=".$_SESSION['rehuf_var']['funid']."&rmedtid='|| entd.edtid ||'\',\'Deseja realmente excluir esta formação?\');\"></center>', edtcurso, edtlocalcurso, to_char(edtdtconclusaocurso,'DD/MM/YYYY'), edtnrhorascurso FROM rehuf.entidadedetalhe entd
				WHERE entd.entid='".$entidade->getEntId()."'";
		
		$cabecalho = array("","Nome do curso","Local de realização","Data de conclusão","Número de horas");
		echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">";
		echo "<tr><td class=\"SubTituloCentro\" colspan='4'><strong>Formação do dirigente</strong></td></tr>";
		echo "<tr><td>";
		$db->monta_lista_simples( $sql, $cabecalho, 50, 10, 'N', '100%','N');
		echo "</td></tr>";
		echo "</table>";
	
	}
		
} else {
	echo "<script>
			alert('Erro com paramêtros. Entre em contato com a equipe técnica');
			window.close();
		  </script>";
	exit;
}
?>
</div>

  </body>
</html>

