<?php
$width = 'width="600px"';
//$bgColor = 'bgcolor="#f5f5f5"';
$bgColor = 'bgcolor="silver"';

function espacoPlano(){
	return '<table align="center" border="0" cellspacing="0" cellpadding="0" style="border-color: black;">
				<tr>
					<td style="height: 50px;">&nbsp;</td>
				</tr>
			</table>';
}
?>
<html>
<head>
<style type="">
	.fot{
		text-transform: uppercase; 
		font-family: arial black; 
		font-size: 20px;
		text-align: center;
		}
	.lista{
		font-size: 11px;
		padding: 3px;
		border-top: 2px solid #000;
		border-collapse: collapse;
	}
	.lista1{
		font-size: 11px;
		padding: 3px;
		border-top: 2px solid #000;
		border-collapse: collapse;
	}
	table.lista td{
		border-style: solid;
		border-width: 1px;
		border-color: #000;
		border-collapse: collapse;
	}
	.folha {
    	page-break-after: always;
	}
	@media print {.notprint { display: none } .div_rolagem{display: none} }	
	@media screen {.notscreen { display: none; }
	
	.div_rolagem{ overflow-x: auto; overflow-y: auto; height: 50px;}
	
</style>
</head>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<body>
<?
	$ptrid = $_REQUEST['ptrid'];
		
	$sql = "SELECT DISTINCT
				pt.ptrid,
			    ent.entnumcpfcnpj,
			    to_char(to_number(ent.entnumcpfcnpj,'000'), '00\".\"000\".\"000\"/\"0000\"-\"00') as entnumcpfcnpj,
			    ent.entnome,
			    mun.mundescricao,
			    est.estuf,
			    est.estdescricao,
			    pt.ptrtipodirigente,
			    pt.ptrjustificativa,
			    (SELECT entnome FROM entidade.entidade WHERE entid = pt.entiddirigente) as dirigente
			FROM
			    entidade.entidade ent INNER JOIN entidade.endereco ende 
			    ON (ende.entid = ent.entid) INNER JOIN territorios.municipio mun 
			    ON (mun.muncod = ende.muncod) INNER JOIN territorios.estado est 
			    ON (est.estuf = mun.estuf) INNER JOIN emenda.planotrabalho pt
			    ON (ent.entid = pt.entid)
			WHERE
			    ent.entstatus = 'A' 
			    AND	ende.endstatus = 'A' 
			    AND pt.ptrstatus = 'A'
			    AND pt.ptrid = $ptrid";
	
	$arDados = $db->pegaLinha($sql);
	$arDados = $arDados ? $arDados : array();

	if(empty($arDados) ) {
		echo "<script>
				alert('Não há registros para serem exibidos!');
				window.close();
	  		  </script>";
		exit();	
	}
?>
<div class="folha">
<? echo relatDeclaracaoAdimplenciaPlanoTrabalho($width, $arDados, $bgColor); ?>
</div>

<? echo espacoPlano(); ?>

<div class="folha">
<? echo relatDescricaoProjetoPlanoTrabalho($width, $arDados, $bgColor); ?>
 </div>
 
<? echo espacoPlano(); ?>

<div class="folha">
<? echo relatDescricaoProjetoPlanoTrabalhoContinuacao($width, $arDados, $bgColor); ?>
</div>

<? echo espacoPlano(); ?>

<div class="folha">
<? echo relatDetalhamentoAcaoPlanoTrabalho($width, $arDados, $bgColor); ?>
</div>

<? echo espacoPlano(); ?>

<div class="folha">
<? echo relatDetalhamentoAcaoContinuacaoPlanoTrabalho($width, $arDados, $bgColor); ?>
</div>

<? echo espacoPlano(); ?>

<div class="folha">
<? echo relatEspecificacaoAcaoPlanoTrabalho('width="1000px"', $arDados, $bgColor); ?>
</div>

<? echo espacoPlano(); ?>

<div class="folha">
<? echo relatCronogramaExecucaoDesembolsoPlanoTrabalho('width="1000px"', $arDados, $bgColor); ?>
</div>

<? echo espacoPlano(); ?>

<div class="folha">
<? echo relatEscolaBeneficiadaAcaoPlanoTrabalho($width, $arDados, $bgColor); ?>
</div>

<? echo espacoPlano(); ?>

<div class="folha">
<? echo relatDetalhamentoItensEspecificacaoAcaoPlanoTrabalho($width, $arDados, $bgColor); ?>
</div>
<??>
<table  align="center" cellspacing="1" cellpadding="4">
	<tr>
		<td style="height: 20px;"></td>
	</tr>
	<tr>
		<td style="text-align: center;" class="div_rolagem">
			<input type="button" name="imprimir" value="Imprimir" onclick="imprimirExtrato();">&nbsp;
			<input type="button" name="fechar" value="Fechar" onclick="javascript: window.close();">
		</td>
	</tr>
</table>

</body>
<script>
// carregando a lista de preenchimento de itens
function imprimirExtrato(){
	window.print();
}
</script>
</html>