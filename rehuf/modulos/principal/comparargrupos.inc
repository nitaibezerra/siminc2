<html>
<head>
<meta http-equiv="Cache-Control" content="no-cache">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="Connection" content="Keep-Alive">
<meta http-equiv="Expires" content="Mon, 26 Jul 1997 05:00:00 GMT">
<title>Comparar grupos</title>
<script language="JavaScript" src="../includes/calendario.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script language="JavaScript" src="./js/rehuf.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script type="text/javascript">
this._closeWindows = false;

function compararGrupos(valor) {
	var dados = valor.split("##");
	window.location='?modulo=principal/comparargrupos&acao=A&gitid='+dados[0]+'&entidcombo='+dados[1]+'&ano='+dados[2];
}
</script>

</head>
<body style="margin:10px; padding:0; background-color: #fff; background-image: url(../imagens/fundo.gif); background-repeat: repeat-y;">
<?



$sqlHospitais = "SELECT '".$_REQUEST['gitid']."##'||ent.entid||'##'||'".$_REQUEST['ano']."' as codigo, ent.entnome||' - '||coalesce(ena.entsig, '') as descricao FROM entidade.entidade ent 
 				 LEFT JOIN rehuf.estruturaunidade esu ON esu.entid = ent.entid 
				 LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
				 LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid 
				 LEFT JOIN entidade.entidade ena ON ena.entid = fea.entid 
				 WHERE fen.funid = '". HOSPITALUNIV ."' AND (esu.esuindexibicao IS NULL OR esu.esuindexibicao = true) ORDER BY ena.entsig";

?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
<td class="SubTituloDireita">
<? 
$db->monta_combo('entid', $sqlHospitais, 'S', 'Selecione', 'compararGrupos', '', '', '200', 'S', 'entid');
?>
</td>
</tr>
</table>
<?
if($_REQUEST['entidcombo'] && $_REQUEST['gitid']) {
	
	/*
	 * AJUSTANDO PARAMETROS PARA CHAMAR O RELATORIO
	 */
	$anos  = $db->pegaLinha("SELECT t.tabtid, t.tabanoini, t.tabanofim FROM rehuf.tabela t 
							 INNER JOIN rehuf.grupoitem g ON g.tabtid=t.tabtid 
							 WHERE g.gitid='".$_REQUEST['gitid']."'");
	
	if($_REQUEST['ano']) {
		$_POST['filano'][] = $_REQUEST['ano'];	
		$_REQUEST['filano'][] = $_REQUEST['ano'];
	} else {
		for($i=$anos['tabanoini'];$i<=$anos['tabanofim'];$i++) {
			$_POST['filano'][] = $i;	
			$_REQUEST['filano'][] = $i;
		}
	}

	$_REQUEST['agrupador'][] = $anos['tabtid'];
	$_REQUEST['entid'][] = $_REQUEST['entidcombo'];
	
	require_once APPRAIZ . "rehuf/modulos/relatorio/relatoriogeral_resultado.inc";
}
?>
</body>
</html>

