<?php
/************ INCLUDES ***********************/
require_once "config.inc";
include_once APPRAIZ."includes/classes_simec.inc";
include_once APPRAIZ."includes/funcoes.inc";
include_once APPRAIZ.'www/cte/_funcoes.php';
include_once APPRAIZ.'www/cte/_componentes.php';



/************ CARREGA OBJETOS E VARIAVEIS ****************/
$db 	= new cls_banco();
$inuid 	= $_SESSION['inuid'];



/************ LISTA TERMO ****************/
$sqlListaTermo = "	SELECT  '<a style=\"cursor:pointer; margin: 0 -20px 0 20px;\" onclick=\"mostraTermo('|| terid ||');\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a>' as acao, 
							terid,  to_char(terdata, 'DD/MM/YYYY') AS terdata  
					FROM cte.termo 
					WHERE inuid = ".$inuid." 
					ORDER BY terid DESC;";	
$cabecalhoTermo = array("Ação", "N° do Termo","Data de Criação");
	
/************ LISTA PARECER TÉCNICO ****************/
$sqlListaParecer = "SELECT  '<a style=\"cursor:pointer; margin: 0 -20px 0 20px;\" onclick=\"mostraParecer('|| parid ||');\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a>' as acao,
							parid, 
							to_char(pardata, 'DD/MM/YYYY') AS pardata  
					FROM cte.parecer 
					WHERE inuid = ".$inuid." 
					ORDER BY parid DESC;";	
$cabecalhoParecer = array("Ação", "N° do Parecer","Data de Criação");

/************ MONTA TITULO (CABEÇALHO) ****************/
cte_montaTitulo( $titulo_modulo, 'Históricos de Termos de cooperação e Parecer Técnico' ); 
?>
<html>
<head>
	<link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
	<link rel="stylesheet" type="text/css" href="/includes/listagem.css"/>
</head>
<body>
	<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" width="100%">
		<tr>
			<th class="tdDegradde02" colspan="3" >Lista de Histórico de Termos</th>
		</tr>
		<tr>
			<?php $db->monta_lista($sqlListaTermo,$cabecalhoTermo,20, 10, 'N', '', ''); ?>
		</tr>
	</table>
	<br></br>
	<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" width="100%">
		<tr>
			<th class="tdDegradde02" colspan="3" >Lista de Histórico de Pareceres</th>
		</tr>
		<tr>
			<?php $db->monta_lista($sqlListaParecer,$cabecalhoParecer,20, 10, 'N', '', ''); ?>
		</tr>
	</table>
	<script language="javascript">
		function mostraTermo(terid){
			var janela = window.open( 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/cte/visualizatermoouparecer.php?documento=termo&terid='+terid,'blank','height=600,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes');
			janela.focus();
		}
		
		function mostraParecer(parid){
			var janela = window.open( 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/cte/visualizatermoouparecer.php?documento=parecer&parid='+parid,'blank','height=600,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes');
			janela.focus();
		}
	</script>
</body>
</html>