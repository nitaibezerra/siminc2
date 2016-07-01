<?php
// inicializa sistema
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
$db = new cls_banco();

if (!$_SESSION['usucpf']){
	die('<script>
			alert(\'Acesso negado!\');
			window.close();
		 </script>');
}

if( $_POST['requisicao'] == 'excluir' ){
	$sql = "UPDATE seguranca.regra SET rgastatus = 'I' WHERE rgaid = ".$_POST['rgaid'];
	$db->executar( $sql );
	if( $db->commit() ){
		echo "<script>
				alert('Registro excluido com sucesso!');
				window.location.href = window.location;
			</script>";
		exit();
	} else {
		echo "<script>
				alert('Falha na operação!');
				window.location.href = window.location;
			</script>";
		exit();
	}
}

$sql= "SELECT s.sisdsc, * 
	   FROM seguranca.menu m 
	   		inner join seguranca.sistema s on s.sisid = m.sisid 
	   WHERE mnuid=".$_GET['mnuid'];
$saida = $db->pegaLinha($sql);
extract($saida);

$boRegra = $db->pegaUm("SELECT count(rgaid) FROM seguranca.regra WHERE mnuid = ".$_GET['mnuid']);
?>
<head>
<META http-equiv="Pragma" content="no-cache" />
<title>SIMEC - Regras da Tela</title>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel="stylesheet" type="text/css" href="../includes/listagem.css" />
<script type="text/javascript">
	window.focus();	
</script>
</head>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0"	marginheight="0" bgcolor="#ffffff">
<form method="post"  name="formulario" id="formulario">	
	<input type="hidden" name="requisicao" id="requisicao" value="">
	<input type="hidden" name="rgaid" id="rgaid" value="">
	<table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="notprint">
    	<tbody>
    	<tr bgcolor="#DCDCDC">
    		<td align="center" width="100%"><label class="TituloTela" style="color: rgb(0, 0, 0);">Regras da Tela</label></td>
    	</tr>
    	<tr>
    		<td style="" align="center" bgcolor="#e9e9e9"><?=$sisdsc; ?></td>
    	</tr>
    	</tbody>
    </table>	
    <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="notprint">
    	<tr bgcolor="#E9E9E9"> 
		    <td height="25" valign="middle"><font face="Verdana, Arial, Helvetica, sans-serif" size="2"><b>Menu:</b> <?=$mnudsc?></font></td>
			<td align="right" height="25"><b>Objetivo:</b> <?=$mnutransacao;?></td>
		</tr>
    </table>
    <table id="tblform" class="notprint" width="100%" cellspacing="0" cellpadding="0" align="center">
		<tr>
			<td><br><input type="button" name="btnIncluir" id="btnIncluir" value="Incluir Nova Regra" onclick="incluirRegra(<?=$_GET['mnuid'] ?>);"><br></td>
		</tr>
	</table>
	<table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="notprint">
    	<tbody>
    	<tr bgcolor="#DCDCDC">
    		<td align="center" width="100%"><label class="TituloTela" style="color: rgb(0, 0, 0);">Lista de Regras</label></td>
    	</tr>
    	</tbody>
    </table>
    <?
    	/*$sql = "SELECT 
    			'<center>
    				<img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"alterarRegra('|| r.mnuid ||',' || r.rgaid || ');\">&nbsp;
    				<img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"excluirRegra(' || r.rgaid || ');\">
    			</center>' as acao,
		  	r.rgadesc,
		  	u1.usunome as gestor,
		  	u2.usunome as analista,
		  	u3.usunome as desenvolvedor,
		  	to_char(r.rgadata, 'DD/MM/YYYY') as data
		FROM 
		  	seguranca.regra r
		  	inner join seguranca.usuario u1 on u1.usucpf = r.rgademandante
		  	inner join seguranca.usuario u2 on u2.usucpf = r.rgasolicitante
		  	inner join seguranca.usuario u3 on u3.usucpf = r.rgadesenvolvedor
		WHERE mnuid = ".$_GET['mnuid']." and rgaidpai is null";
	
	$cabecalho = array( "&nbsp;&nbsp;Ação&nbsp;&nbsp;", "Descrição", "Gestor", "Analista", "Programador", "Data");
*/
    
    	$sql = "SELECT 
	    			'<center>
	    				<img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"alterarRegra('|| r.mnuid ||','|| r.rgaid ||');\">&nbsp;
	    				<img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"excluirRegra('|| r.rgaid ||');\">&nbsp;
	    				<img src=\"/imagens/consultar.gif\" style=\"cursor: pointer\" onclick=\"visualizarRegra('|| r.mnuid ||','|| r.rgacod ||');\" border=0 alt=\"Ir\" title=\"Visulizar Regras\">
	    				'||case when arqid is not null then '<img src=\"/imagens/anexo.gif\" style=\"cursor: pointer\" border=0 alt=\"Ir\" title=\"Existe anexos\">' else '' end||'
	    			</center>' as acao,
	    			r.rgadesc,
			        r.rgacampo,
				  	to_char(r.rgadata, 'DD/MM/YYYY') as data
				FROM 
				  	seguranca.regra r
				WHERE r.mnuid = ".$_GET['mnuid']."
					and rgaid = (select max(rgaid) from seguranca.regra where rgacod = r.rgacod and rgastatus = 'A')";
    	
    	$cabecalho = array( "&nbsp;&nbsp;Ação&nbsp;&nbsp;", "Descrição", "Campo", "Data");
    	$db->monta_lista_simples( $sql, $cabecalho, 100000, 1, 'N', '100%');
    ?>
</form>
<script type="text/javascript">
function incluirRegra(mnuid){
	window.open(
		'../geral/popupIncluirRegras.php?mnuid='+mnuid,
		'regras',
		'height=600,width=800,scrollbars=yes,top=50,left=200'
	);
}
function excluirRegra(rgaid){
	if( confirm('Tem certeza que deseja excluir esta regra?') ){
		window.document.getElementById('requisicao').value = 'excluir';
		window.document.getElementById('rgaid').value = rgaid;
		window.document.getElementById('formulario').submit();
	}
}
function alterarRegra(mnuid, rgaid){
	window.open(
		'../geral/popupIncluirRegras.php?mnuid='+mnuid+'&rgaid='+rgaid,
		'regras',
		'height=600,width=800,scrollbars=yes,top=50,left=200'
	);
}
function visualizarRegra(mnuid, rgacod){
	window.open(
		'../geral/popupVisualizaRegra.php?rgacod='+rgacod+'&mnuid='+mnuid,
		'regras',
		'height=600,width=800,scrollbars=yes,top=50,left=200'
	);
}
</script>
</body>
