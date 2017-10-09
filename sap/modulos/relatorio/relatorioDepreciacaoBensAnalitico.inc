<?php
//Variáveis
$dtentrada  = formata_data_sql($_REQUEST['dtentrada']);
$tipo 		= $_REQUEST['tipo'];
$validacao	= $_REQUEST['validacao'];
$varExcel	= $_REQUEST['excel'];

//$data="2012-03-31";

$sql="SELECT
		codcontacontabil AS \"conta\",
		ccbdsc AS \"descricao\",
		to_char(bendtentrada, 'DD/MM/YYYY') AS \"data_entrada\",
		matdsc AS \"material\",
		bmtvlrunitario AS \"valor_entrada\",
		vdepreciavel AS \"valor_depreciavel\",
		vdepreciacaoanual AS \"depreciacao_anual\",
		vdepreciacaomes AS \"depreciacao_corrente\",
		vresidual AS \"valor_residual\",
		vdepreciacaoacumulada AS \"depreciacao_acumulada\",
		vliquido AS \"valor_liquido\",
		vmesesutilizacao as \"meses_utilizacao\",
		vanosutilizacao as \"anos_utilizacao\",
		vdepreciacaoacumulada as \"depreciacao_acumulada\",
		vdepreciacaomes as \"depreciacao_mensal\",
		rgpnum as \"rgp\",
		ccbvidautil as \"vida_util\",
		ccbpercresidual as \"percentual_residual\",
		vmesesutilizacao as \"limite_meses\"
	  FROM 
	  	sap.fn_relatorio_depreciacao_mensal( DATE('".$dtentrada."'))
  ORDER BY
		codcontacontabil";

$relAnaliticoDepBens= $db->carregar($sql);

if( !empty($validacao) ){

	if ( is_bool($relAnaliticoDepBens) ){
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

<style type="text/css">
    table.bordasimples {border-collapse: collapse; width:95%;}
    table.cabecalho tr {background-color:#EAEAEA;}
    table.bordasimples td {border:1px solid #cccccc;}
    
    thead {display: table-header-group;}
</style>

<table align="center" class="tabela" width="95%" border="0">
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
		    <td colspan="11">
		    <br />   
		    <h2 align="center">RELATÓRIO ANALÍTICO DE DEPRECIAÇÃO DE BENS</h2>
			<h3 align="center">Bens com entrada até: <?php echo $_REQUEST['dtentrada']; ?></h3>
		    </td>
		</tr>
		<tr>
			<td></td>
		</tr>

		<?php 
			foreach ($relAnaliticoDepBens as $depBens){
		
				echo "<tr>";
				echo "<th colspan='2'><b>Conta</b>:	" 		.$depBens['conta']			. "</th>";
				echo "<th colspan='2'><b>Descrição:</b> " 	.$depBens['descricao']		. "</th>";
				echo "<th colspan='2'><b>RGP:</b> " 		.$depBens['rgp']			. "</th>";
				echo "<th colspan='3'><b>Material:</b> " 	.$depBens['material']		. "</th>";
				echo "<th colspan='2'><b>Dt. Entrada:</b> " .$depBens['data_entrada']	. "</th>";
				echo "</tr>";
		
		echo "<tr>";
		echo "<td align='center'><b>Valor de Entrada</b></td>";
		echo "<td align='center'><b>Vida Útil (anos)</b></td>";
		echo "<td align='center'><b>Percentual Residual</b></td>";
		echo "<td align='center'><b>Valor Depreciável</b></td>";
		echo "<td align='center'><b>Depreciação Anual</b></td>";
		echo "<td align='center'><b>Depreciação Corrente</b></td>";
		echo "<td align='center'><b>Valor Residual</b></td>";
		echo "<td align='center'><b>Depreciação Acumulada</b></td>";
		echo "<td align='center'><b>Limite Meses</b></td>";
		echo "<td align='center'><b>Anos/Meses</b></td>";
		echo "<td align='center'><b>Valor Líquido</b></td>";
		echo "</tr>";
		
		echo "<tr>";
		echo "<td align='center'>R$ " . number_format($depBens['valor_entrada'],2,',','.')					. "</td>";
		echo "<td align='center'>   " . $depBens['vida_util']												. "</td>";
		echo "<td align='center'>   " . $depBens['percentual_residual']										. "</td>";
		echo "<td align='center'>R$ " . number_format($depBens['valor_depreciavel'],2,',','.') 				. "</td>";
		echo "<td align='center'>R$ " . number_format($depBens['depreciacao_anual'],2,',','.') 				. "</td>";
		echo "<td align='center'>R$ " . number_format($depBens['depreciacao_mensal'],2,',','.') 			. "</td>";
		echo "<td align='center'>R$ " . number_format($depBens['valor_residual'],2,',','.') 				. "</td>";
		echo "<td align='center'>R$ " . number_format($depBens['depreciacao_acumulada'],2,',','.') 			. "</td>";
		echo "<td align='center'>   " . $depBens['meses_utilizacao']										. "</td>";
		echo "<td align='center'>   " . $depBens['anos_utilizacao'] . " / " . $depBens['meses_utilizacao']  . "</td>";
		echo "<td align='center'>R$ " . number_format($depBens['valor_liquido'],2,',','.') 					. "</td>";
		echo "</tr>";
		echo "<tr>";
		echo "<td></td>";
		echo "</tr>";
		echo "</tr>";
		echo "<tr>";
		echo "<td></td>";
		echo "</tr>";
	}
	?>
		<tr>
			<td></td>
		</tr>
		<tr>
			<td></td>
		</tr>
	</tbody>
</table>