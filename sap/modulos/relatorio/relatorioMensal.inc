<?php
$ano 		= $_REQUEST['var_ano'];
$mes 		= $_REQUEST['var_mes'];
$validacao	= $_REQUEST['validacao'];
$varExcel	= $_REQUEST['excel'];

$sql="SELECT 
		codcontacontabil as conta_contabil, 
		ccbdsc as descricao, 
		rmivlrsaldoanterior as saldo_anterior,
		rmivlrorcamentario as valor_orcamentario,
		rmivlrextraorcamentario as extra_orcamentario,
		rmivlrsaida as valor_saida,
		rmivlrsaldoatual as saldo_atual,
		situacao as situacao
	  FROM sap.fn_lista_rmb($ano, $mes)";

$listaMovimentacaoBens= $db->carregar($sql);

if ( !empty($validacao) ){
	if( !$listaMovimentacaoBens ){ 
		$passou = array( 'status' => false, 'msg' => 'Nenhum registro encontrado.' );		
	}else{
		$passou = array( 'status' => true );
	}
	echo simec_json_encode( $passou );
	exit;
}elseif (!empty( $varExcel )){

	// definimos o tipo de arquivo
	header("Content-type: application/msexcel");
	
	// Como será gravado o arquivo
	header("Content-Disposition: attachment; filename=relatorio.xls");
}

$somaSaldoAnterior=0;
$somaOrcamentario=0;
$somaExtraOrcamentario=0;
$somaSaida=0;
$somaSaldoAtual=0;

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
        	<td valign="top" width="50" rowspan="2" style=""><img src="../imagens/brasao.gif" width="45" height="45" colspan="1"></td>                    
        	<td nowrap align="left" valign="middle" height="1" style="padding:5px 0 0 0;" colspan="2">                 
                SIMEC- Sistema Integrado do Ministério da Educação<br/>                              
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
		    <h2 align="center">RELATÓRIO DE MOVIMENTAÇÃO DE BENS MÓVEIS - RMB</h2>
		    <h3 align="center"><?php echo $mes; ?> / <?php echo $ano; ?></h3>
		    </td>
		</tr>
		<tr>
			<th><b>Gestão Tesouro</b></th>
			<th colspan="6" align="left"><b>Unidade Gestora - 150002</b></th>
		</tr>
		<tr>
			<th width="10%"><b>Conta</b></th>
			<th width="30%"><b>Denominação</b></th>
			<th width="10%"><b>Saldo Anterior</b></th>
			<th width="10%"><b>Orçamentário</b></th>
			<th width="15%"><b>Extra Orçamentário</b></th>
			<th width="10%"><b>Saída</b></th>
			<th width="10%"><b>Saldo Atual</b></th>
		</tr>

		<?php
		foreach($listaMovimentacaoBens as $ben) {
			$somaSaldoAnterior +=$ben['saldo_anterior'];
			$somaOrcamentario +=$ben['valor_orcamentario'];
			$somaExtraOrcamentario +=$ben['extra_orcamentario'];
			$somaSaida +=$ben['valor_saida'];
			$somaSaldoAtual +=$ben['saldo_atual'];
			
			echo "<tr>";
			echo "<td align=\"center\" title=\"Conta\">" 				. $ben['conta_contabil'] . "</td>";
			echo "<td align=\"left\"   title=\"Denominação\">" 			. $ben['descricao'] . "</td>";
			echo "<td align=\"left\" title=\"Saldo Anterior\">R$ " 		. number_format($ben['saldo_anterior'],2,',','.') . "</td>";
			echo "<td align=\"left\" title=\"Orçamentário\">R$ " 		. number_format($ben['valor_orcamentario'],2,',','.') . "</td>";
			echo "<td align=\"left\" title=\"Extra Orçamentário\">R$ " 	. number_format($ben['extra_orcamentario'],2,',','.') . "</td>";
			echo "<td align=\"left\" title=\"Saída\">R$ " 				. number_format($ben['valor_saida'],2,',','.') . "</td>";
			echo "<td align=\"left\" title=\"Saldo Atual\">R$ " 		. number_format($ben['saldo_atual'],2,',','.') . "</td>";
			echo "</tr>";
		}
		?>
		<tr>
			<td></td>
		</tr>
		<tr style="background-color:#EAEAEA;">
			<td colspan="2"><b>Totais:</b></td>
			<td width="10%" align="center"><b>R$ <?php echo number_format($somaSaldoAnterior,2,',','.') ?></b></td>
			<td width="10%" align="center"><b>R$ <?php echo number_format($somaOrcamentario,2,',','.') ?></b></td>
			<td width="10%" align="center"><b>R$ <?php echo number_format($somaExtraOrcamentario,2,',','.') ?></b></td>
			<td width="10%" align="center"><b>R$ <?php echo number_format($somaSaida,2,',','.') ?></b></td>
			<td width="10%" align="center"><b>R$ <?php echo number_format($somaSaldoAtual,2,',','.') ?></b></td>
		</tr>
		<tr>
			<td></td>
		</tr>
		<tr>
			<th colspan="7"><b>Total de Contas Registradas: <?php echo count($listaMovimentacaoBens) ?></b></th>
		</tr>
	</tbody>
</table>
