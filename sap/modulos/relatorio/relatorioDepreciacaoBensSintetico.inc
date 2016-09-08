<?php
//Variáveis
$dtentrada 	= formata_data_sql($_REQUEST['dtentrada']);
$tipo 		= $_REQUEST['tipo'];
$validacao	= $_REQUEST['validacao'];
$varExcel	= $_REQUEST['excel'];

$sql="SELECT
	codcontacontabil AS conta_contabil,
	ccbdsc AS descricao,
	SUM(bmtvlrunitario) AS valor_entrada,
	SUM(vDepreciavel) AS valor_depreciavel,
	SUM(vDepreciacaoAnual) AS depreciacao_anual,
	SUM(vDepreciacaoMes) AS depreciacao_corrente,
	SUM(COALESCE(vResidual,100)) AS valor_residual,
	SUM(vDepreciacaoAcumulada) AS depreciacao_acumulada,
	SUM(vLiquido) AS valor_liquido
  FROM 
  	sap.fn_relatorio_depreciacao_mensal( DATE('".$dtentrada."'))
GROUP BY
	codcontacontabil,
	ccbdsc
ORDER BY
	codcontacontabil";
	
$depreciacaoBensSintetico = $db->carregar($sql);

if( !empty($validacao) ){

	if ( is_bool($depreciacaoBensSintetico) ){
		$retorno = array('status'=>false, 'msg'=>'Nenhum registro encontrado.');
	}else{
		$retorno = array('status'=>true);
	}

	echo simec_json_encode( $retorno );
	exit;
}elseif (!empty( $varExcel )){
	
	// definimos o tipo de arquivo
	header("Content-type: application/msexcel");
	
	// Como será gravado o arquivo
	header("Content-Disposition: attachment; filename=relatorio.xls");
}

$somaValEntrada=0;
$somaValDepreciavel=0;
$somaDepAnual=0;
$somaValDepCorrente=0;
$somaValResidual=0;
$somaDepAcumulada=0;
$somaValLiquido=0;

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>		 
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

<style type="text/css" media="print">
	.noPrint{ display:none;} 
</style>

<table align="center" class="tabela" width="95%">
	<thead>
		<tr bgcolor="#ffffff" style="">
        	<td valign="top" width="50" rowspan="2" style=""><img src="../imagens/brasao.gif" width="45" height="45" border="0" colspan="1"></td>                    
        	<td nowrap align="left" valign="middle" height="1" style="padding:5px 0 0 0;" colspan="2">                 
                SIMEC- Sistema Integrado de Monitoramento Execução e Controle<br/>                              
                Ministério da Educação <br />
                Secretaria Executiva<br />
                Subsecretaria de Assuntos Administrativos<br />
                Coordenação Geral de Recursos Logísticos
        	</td>
        	<td align="right" valign="middle" height="1" style="padding:5px 0 0 0;" colspan="5">     
				<a class="noPrint" href="javascript:window.print();"><img src="../imagens/print2.gif" border="0" width="20"></a><br/>                           
                Impresso por: <b><?php echo $_SESSION['usunome'] ?></b><br/>                      
                Hora da Impressão: <?php echo date( 'd/m/Y - H:i:s' ) ?><br />                                    
			</td>                             
		</tr>
	</thead>
	
	<tbody>
		<tr width="95%" align="center">
		    <td colspan="9">
		    <br />   
			<h2>RELATÓRIO SINTÉTICO DE DEPRECIAÇÃO DE BENS</h2>
			<h3>Bens com Entrada Até: <?php echo $_REQUEST['dtentrada']; ?></h3>
		    </td>
		</tr>
		<tr>
	    	<th><b>Conta</b></th>
	    	<th><b>Descrição</b></th>
	    	<th><b>Valor Entrada</b></th>
	    	<th><b>Valor Depreciável</b></th>
	    	<th><b>Depreciação Anual</b></th>
	    	<th><b>Depreciação Corrente</b></th>
	    	<th><b>Valor Residual</b></th>
	    	<th><b>Depreciação Acumulada</b></th>
	    	<th><b>Valor Líquido</b></th>
  	</tr>
  <?php 
  foreach ($depreciacaoBensSintetico as $dep){
  	$somaValEntrada 	+=$dep['valor_entrada'];
  	$somaValDepreciavel +=$dep['valor_depreciavel'];
  	$somaDepAnual 		+=$dep['depreciacao_anual'];
  	$somaValDepCorrente +=$dep['depreciacao_corrente'];
  	$somaValResidual 	+=$dep['valor_residual'];
  	$somaDepAcumulada 	+=$dep['depreciacao_acumulada'];
  	$somaValLiquido 	+=$dep['valor_liquido'];
  	
  	echo "<tr>";
  	echo "<td align='center'> ". $dep['conta_contabil'] . "</td>";
  	echo "<td align='left'>   ". $dep['descricao'] . "</td>";
  	echo "<td align='left'>R$ ". number_format($dep['valor_entrada'],2,',','.') . "</td>";
  	echo "<td align='left'>R$ ". number_format($dep['valor_depreciavel'],2,',','.') . "</td>";
  	echo "<td align='left'>R$ ". number_format($dep['depreciacao_anual'],2,',','.') . "</td>";
  	echo "<td align='left'>R$ ". number_format($dep['depreciacao_corrente'],2,',','.') . "</td>";
  	echo "<td align='left'>R$ ". number_format($dep['valor_residual'],2,',','.') . "</td>";
  	echo "<td align='left'>R$ ". number_format($dep['depreciacao_acumulada'],2,',','.') . "</td>";
  	echo "<td align='left'>R$ ". number_format($dep['valor_liquido'],2,',','.') . "</td>";
  	echo "</tr>";
  }
  ?>
 <tr>
 	<td></td>
 </tr>
 <tr style="background-color:#EAEAEA;">
  	<td colspan="2"><b>TOTAL:</b></td>
    <td align="center"><b>R$ <?php echo number_format($somaValEntrada,2,',','.') ?></b></td>
    <td align="center"><b>R$ <?php echo number_format($somaValDepreciavel,2,',','.') ?></b></td>
    <td align="center"><b>R$ <?php echo number_format($somaDepAnual,2,',','.') ?></b></td>
    <td align="center"><b>R$ <?php echo number_format($somaValDepCorrente,2,',','.') ?></b></td>
    <td align="center"><b>R$ <?php echo number_format($somaValResidual,2,',','.') ?></b></td>
    <td align="center"><b>R$ <?php echo number_format($somaDepAcumulada,2,',','.') ?></b></td>
    <td align="center"><b>R$ <?php echo number_format($somaValLiquido,2,',','.') ?></b></td>
  </tr>
</table>
