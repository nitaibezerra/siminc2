<?php
$dtinicio	= formata_data_sql( $_REQUEST['dtinicio'] );
$dtfim		= formata_data_sql( $_REQUEST['dtfim'] );
$validacao	= $_REQUEST['validacao'];
$varExcel	= $_REQUEST['excel'];

$sqlContaContabil="SELECT 
					codcontacontabil as conta_contabil
					, ccbdsc as descricao
					, SUM ( bmtvlrunitario ) AS soma_valor_saida
					, count(rgpnum)as qtdItens
				FROM sap.fn_relatorio_analitico_rmb( DATE('".$dtinicio."') , DATE('".$dtfim."') ) 
				GROUP BY codcontacontabil, ccbdsc";

$dadosContasContabeis = $db->carregar( $sqlContaContabil );

if( !empty($validacao) ){
	
	if ( is_bool($dadosContasContabeis) ){
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
                <?php echo NOME_SISTEMA; ?><br/>                              
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
			<td colspan="7">
			<br />
			<h2 align="center">RELATÓRIO ANALÍTICO - RMB</h2>
			<h3 align="center">SAÍDA - PERÍODO DE <?php echo formata_data($dtinicio); ?> À <?php echo formata_data($dtfim); ?></h3>
			</td>
		</tr>
		
		<?php
		$qtdGeral=0;
		$totalGeral=0;
		$contadorGeral=0;
			
		foreach ($dadosContasContabeis as $conta){
			$contadorGeral++;
	
			echo "<tr>";
			echo "	<th colspan='6'> <b>CONTA CONTÁBIL:</b> " . $conta['conta_contabil'] . " " . $conta['descricao'] . "</th>";
			echo "</tr>";
			echo "<tr>";
			echo "	<th><b>RGP</b></th>";
			echo "	<th><b>Descrição</b></th>";
			echo "	<th><b>Nº Processo</b></th>";
			echo "	<th><b>Data Docum.</b></th>";
			echo "	<th><b>Valor Saída</b></th>";
			echo "</tr>";
			
			$sql="SELECT
			codcontacontabil as conta_contabil,
			ccbdsc as descricao,
			rgpnum as rgp,
			matdsc as material_descricao,
			bebnumprocesso as num_processo,
			to_char(bebdataprocesso,'DD/MM/YYYY') as data_processo,
			bmtvlrunitario as valor_unitario
			FROM
			sap.fn_relatorio_analitico_rmb( DATE('".$dtinicio."'), DATE('".$dtfim."') )
			WHERE codcontacontabil ='{$conta['conta_contabil']}'";
		
			  $relatorioAnalitico= $db->carregar($sql);
			
			  $qtdItens=0;
			  $totalParcial=0;
			
				foreach($relatorioAnalitico as $rmba) {
					echo "<tr>";
					echo "<td align=\"center\" title=\"RGP\">" 				. $rmba['rgp'] . "</td>";
					echo "<td align=\"left\"   title=\"Descrição\">" 		. $rmba['material_descricao'] . "</td>";
					echo "<td align=\"center\" title=\"Nº do Processo\">" 	. $rmba['num_processo'] . "</td>";
					echo "<td align=\"center\" title=\"Data Docum.\">" 		. $rmba['data_processo'] . "</td>";
					echo "<td align=\"left\" title=\"Valor Saída\"> R$ "	. number_format($rmba['valor_unitario'],2,',','.') . "</td>";
					echo "</tr>";
		
					$totalParcial=$totalParcial + $rmba['valor_unitario'];
					
					$qtdItens++;
					}
					
					$qtdGeral=$qtdGeral+$qtdItens;
					$totalGeral=$totalGeral+$totalParcial;
					
					echo "<tr style='background-color:#EAEAEA;'>";
					echo "<td colspan='2'><b>Quantidade de itens:</b> " . $qtdItens . "</td>";
					echo "<td> </td>";
					echo "<td>  </td>";
					echo "<td colspan='2' align='center'>" . "<b>Total Parcial: R$ "  .  number_format($totalParcial,2,',','.') . "</b>" . "</td>";
					echo "</tr>";
					echo "<tr>";
					echo "<td colspan='6'> </td>";
					echo "</tr>";
					echo "<tr>";
					echo "<td colspan='6'> </td>";
					echo "</tr>";
		
				}
				if (count($dadosContasContabeis)==$contadorGeral){
					echo "<tr>";
					echo "<th colspan='4'><b>Quantidade Geral:</b> " . $qtdGeral . "</th>";
		
					echo "<th colspan='2' align='center'>" . "<b>Total Geral: R$ " . number_format($totalGeral,2,',','.') . "</th>";
					echo "</tr>";
					echo "</tbody>";
					echo "</table>";
				}
		?>	