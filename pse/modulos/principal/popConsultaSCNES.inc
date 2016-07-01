<?php
global $db;

monta_titulo( 'Códigos SCNES', '' );

?>
<style>
<!--
#loader-container,
#LOADER-CONTAINER{
    background: transparent;
    position: absolute;
    width: 97%;
    text-align: center;
    z-index: 8000;
    height: 90%;
}


#loader {
    background-color: #fff;
    color: #000033;
    width: 250px;
    border: 2px solid #cccccc;
    font-size: 12px;
    padding: 25px;
    font-weight: bold;
    margin: 150px auto;
}
-->
</style>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<body>
<form method="post" name="formulario" id="formulario">
<div id="loader-container" style="display: none">
   	<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
</div>

<script type="text/javascript" src="/includes/prototype.js"></script>

<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
<tr>
	<td class="SubTituloEsquerda">Pesquisar Código SCNES: </td><td class="SubTituloEsquerda"><input type="text" id="codigo" onkeypress="return somenteNumeros(event);" name="codigo" onkeyup="this.value=mascaraglobal('##########',this.value);" ></td>
</tr>
<tr>
	<td class="SubTituloEsquerda">Pesquisar Nome da ESF: </td><td class="SubTituloEsquerda"><input type="text" id="nome" name="nome"></td>
</tr>
<tr>
	<td colspan="2" class="SubTituloEsquerda">
		<center><input type="submit" value="Pesquisar" ></center>
	</td>
</tr>
</table>
</form>
<? 	

$where = '';

if( $_POST['codigo'] ){
	$codigo = $_POST['codigo'];
	$where.= " WHERE s.cnecodigocnes = ".$codigo;
}

if( $_POST['nome'] ){
	if( $codigo ){
		$where.= " AND ";
	} else {
		$where.= " WHERE ";
	}
	$nome = removeAcentos( $_POST['nome'] );
	$where.= " public.removeacento(s.cnenomefantasia) ilike '%".$nome."%'";
}


	$sql = "
		SELECT 
			'<center>
				<a style=\"cursor:pointer;\" onclick=\"selecionaSCNES('||s.cnecodigocnes||')\">
					<img src=\"/imagens/alterar.gif \" border=0 title=\"Seleciona Escola\">
				</a>
			</center>' as acao,	s.cnecodigocnes, s.cnenomefantasia,count(*)
		FROM
			pse.scnes s
		$where
		GROUP BY s.cnecodigocnes, s.cnenomefantasia
		ORDER BY s.cnenomefantasia";

	$cabecalho 	= array("Ação", "Código", "Nome", "Qd Equipes");
	$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', 'center', '', '', '','');

?>

<script type="text/javascript" src="/includes/prototype.js"></script>	
<script type="text/javascript">

function selecionaSCNES(scnes,id){
	window.opener.document.getElementById('codSCNES').value = scnes;
	window.opener.verEquipes(scnes);
	window.close();
}

</script>
</body>