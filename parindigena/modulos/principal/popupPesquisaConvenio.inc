<html>
<head>
<meta http-equiv="Cache-Control" content="no-cache">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="Connection" content="Keep-Alive">
<meta http-equiv="Expires" content="-1">

<title><?php echo $titulo ?></title>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script src="../includes/prototype.js"></script>
<script src="../includes/entidades-dev.js"></script>
<script src="../includes/calendario.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
    
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

<style type="text/css" media="screen">
	#loader-container,
	#LOADER-CONTAINER
	{
		background: none;
		position: absolute;
		width: 100%;
		text-align: center;
		z-index: 8000;
		height: 100%;
	}
	
	#loader {
		background-color: #fff;
		color: #000033;
		width: 300px;
		border: 2px solid #cccccc;
		font-size: 12px;
		padding: 25px;
		font-weight: bold;
		margin: 150px auto;
	}
</style>
<script type="text/javascript">
function validaBusca()
{
	var uf = document.formulario.regcod.value.length;
	var ano = document.formulario.codigo.value.length;
	var convenio = document.formulario.covnumero.value.length;
	
	if( uf < 1 && ano < 1 && convenio < 1)
	{
		alert('Refine mais sua busca');
		return false;
	}
	
	if( convenio < 1 && (uf < 1 || ano < 1 ))
	{
		alert('Refine mais sua busca');
		return false;
	}
	
}

function adicionaConvenioParent(covnumero, covcod){	 
    window.opener.document.formulario.covnumero.value = covnumero;
    window.opener.document.formulario.covcod.value = covcod;
    window.close(); 
}
</script>
</head>
<body>
<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;">
	<tr>
		<td width="100%" align="center">
			<label class="TituloTela" style="color:#000000;">Pesquisar Convênios</label>
		</td>
	</tr>
	<tr>
		<td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr='#FFFFFF', endColorStr='#dcdcdc', gradientType='1')" ></td>
	</tr>
</table> 
<form name = "formulario" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario" onsubmit = "return validaBusca();">
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class ="SubTituloDireita" align="right">Número do Convênio:</td>
			<td>                       
			<?= campo_texto('covnumero', 'N', $somenteLeitura, '', 30, 600, '', '', 'left', '',  0, 'id="covnumero" onblur="MouseBlur(this);"', '',$_REQUEST['covnumero']); ?>
			<input type="hidden" name="condicao" value="true">
			</td>
			<td align="left"></td>
		</tr>
		<tr>
			<td class = "SubTituloDireita">UF:</td>
			<td colspan="2">
			<?php $sql = "SELECT regcod as codigo, regcod as descricao from public.uf WHERE idpais = 1 order by regcod"; ?>
			<?=$db->monta_combo('regcod', $sql, 'S', "Selecione...", '', '', '', '100', 'N', 'regcod','',$_REQUEST['regcod']);?>
			</td>
		</tr>
		<tr>
			<td class = "SubTituloDireita">Ano:</td>
			<td colspan="2">
			<?php
			$sql = "select distinct to_char(covdatainicio, 'yyyy') AS codigo,
							to_char(covdatainicio, 'yyyy') AS descricao 
							from financeiro.convenio 
							WHERE to_char(covdatainicio, 'yyyy') IS NOT NULL
							ORDER BY descricao";
			?>
			<?=$db->monta_combo('codigo', $sql, 'S', "Selecione...", '', '', '', '100', 'N', 'codigo','',$_REQUEST['codigo']);?>
			</td>
		</tr>
		<!-- 
		<tr>
		<td class = "SubTituloDireita">
		Processo:
		</td>
		<td colspan="2">            	
		<?= campo_texto('covnumero', 'N', $somenteLeitura, '', 30, 600, '', '', 'left', '',  0, 'id="covnumero" onblur="MouseBlur(this);"' ); ?>
		</td>
		</tr>
		-->
		<tr>
			<td  class ="SubTituloDireita" align="right"></td>
			<td><input type="submit" name="btnBuscar" value="Buscar" id="btnBuscar"></td>
			<td></td>
		</tr>	
		<tr>
			<td  class ="SubTituloDireita" align="right"></td>
			<td>Selecione o convênio desejado a partir da lista abaixo clicando no numero do convenio</td>
			<td></td>
		</tr>	
	</table>
</form>	
</body>	
<?php
if ($_REQUEST['condicao'] == "true")
{			
		$sql = "
		SELECT DISTINCT
			'<a style=\"cursor: pointer;\" onclick=\"adicionaConvenioParent(\''||covnumero||'\', \''||covcod||'\');\"> '||covnumero||' </a>' as num, 
			'<a style=\"cursor: pointer;\" onclick=\"adicionaConvenioParent(\''||covnumero||'\', \''||covcod||'\');\"> '||covprocesso||' </a>' as processo  , 
			'<a style=\"cursor: pointer;\" onclick=\"adicionaConvenioParent(\''||covnumero||'\', \''||covcod||'\');\"> '||covvalor||' </a>' as valor, 
			'<a style=\"cursor: pointer;\" onclick=\"adicionaConvenioParent(\''||covnumero||'\', \''||covcod||'\');\"> '||covobjeto||' </a>' as objeto    
		FROM 
			financeiro.convenio 
		WHERE  
			covstatus = 'A' ";		
		
		if ($_REQUEST['regcod'])
		{
			$sql.= " AND estuf = '{$_REQUEST['regcod']}'";
		}
		if ($_REQUEST['codigo'])
		{
			$sql.= " AND to_char(covdatainicio,'yyyy') = '{$_REQUEST['codigo']}'";
		}	
		if ($_REQUEST['covnumero'])
		{
			$sql.= " AND covnumero LIKE '{$_REQUEST['covnumero']}%' ";			
		}		
		$sql.=" ORDER BY  objeto  ";
		$cabecalho = array("Número", "Processo","Valor do Convênio", "Objeto" );  
		$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '');	
		
}
?>