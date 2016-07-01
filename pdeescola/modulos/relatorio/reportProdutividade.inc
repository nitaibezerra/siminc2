<?php
ini_set("memory_limit","1024M");

$docid = pegarDocid( $entid );
if ( $_REQUEST['formulario'] ) {
	$codigo  = $_REQUEST['codigo'];
} else {
	$codigo = $_SESSION['sisid'];
}
	
if ($_POST || $_GET['entid']){
	include 'resultRelProdutividade.php';
	exit;
}
include APPRAIZ. '/includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( 'Relatório de Produtividade', 'Relatório de Produtividade' );

?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Relatório</title>
<script src="/includes/calendario.js"></script>

<script type="text/javascript">
function gerarRelatorio(){
	var formulario = document.formulario;
	var  htddataini = document.formulario.htddataini.value;
	var  htddataini = document.formulario.htddataini.value;
	
	
	selectAllOptions( formulario.agrupador );	
	selectAllOptions( formulario.f_estuf );	
	selectAllOptions( formulario.f_municipio );	
	
	var janela = window.open( '', 'relatorio', 'width=900,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
	formulario.target = 'relatorio';	
	formulario.submit();
	janela.focus();
}

function onOffCampo( campo ){
	var div_on = document.getElementById( campo + '_campo_on' );
	var div_off = document.getElementById( campo + '_campo_off' );
	var input = document.getElementById( campo + '_campo_flag' );
	if ( div_on.style.display == 'none' ){
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '1';
	}
	else{
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '0';
	}
}
</script>
</head>
<body>
<form name="formulario" id="formulario" action="pdeescola.php?modulo=relatorio/resultReportProdutividade&acao=A" method="post" enctype="multipart/form-data">	
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
		<tr>	
	        <td class="SubTituloDireita" valign="top">Situação</td>
	        <td>  
	        <?php
	          $sql = "SELECT
											 esdid AS codigo,
											 esddsc AS descricao
											FROM
											 workflow.estadodocumento et
											 INNER JOIN workflow.tipodocumento tp ON tp.tpdid = et.tpdid AND
											 	sisid = ".$_SESSION['sisid']."
											 WHERE et.tpdid = ".TPDID_PDE_ESCOLA."
											ORDER BY
											 esdordem;";
									 
									$db->monta_combo( "esdid", $sql, 'S', 'Selecione...', '', '' );
			 ?>
	        </td>
    	</tr>
    	<tr>
	        <td class="SubTituloDireita" valign="top">Período da Situação</td>
	        <td>  
	           <?= campo_data( 'htddataini', 'S','S','','S','' ); ?> (dd/mm/aaaa) &nbsp;&nbsp;&nbsp;até &nbsp; 
	           <?= campo_data( 'htddatafim', 'S','S','','S','' ); ?> (dd/mm/aaaa)
	        </td>
    	</tr>
    	<tr>
			<td class="SubTituloDireita" valign="top">Agrupadores</td>
			<td>
				<?
				$matriz = agrupador();
				$campoAgrupador = new Agrupador( 'formulario' );
				$campoAgrupador->setOrigem( 'agrupadorOrigem', null, $matriz );
				$campoAgrupador->setDestino( 'agrupador');
				$campoAgrupador->exibir();
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'estuf' );">
				Estado
				<input
					type="hidden"
					id="estuf_campo_flag"
					name="estuf_campo_flag"
					value="<?php echo $possuiSelecionado ? "1" : "0" ?>"
				/>
			</td>
			<td>
				<div
					id="estuf_campo_off"
					style="color:#a0a0a0;display:<?php echo !$possuiSelecionado ? "block" : "none" ?>"
					onclick="javascript:onOffCampo( 'estuf' );"
				><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="estuf_campo_on" style="display:<?php echo $possuiSelecionado ? "block" : "none" ?>;">
					<?php
						$sql_combo ="SELECT 
										estuf AS codigo,
										estuf || ' - ' || estdescricao AS descricao
									FROM
									    territorios.regiao r   
										INNER JOIN territorios.estado e ON e.regcod = r.regcod	 									
									ORDER BY
										estuf";
						
						combo_popup(
							'f_estuf',						 
							$sql_combo,						 
							'Selecione o(s) Estado(s)', 
							'400x400',						 
							0,								 
							array(),						 
							'',								 
							'S',							 
							true,							 
							true  			
						);
					?>
				</div>
			</td>
		</tr>	
		<tr>
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'municipio' );">
				Município
				<input
					type="hidden"
					id="municipio_campo_flag"
					name="municipio_campo_flag"
					value="<?php echo $possuiSelecionado ? "1" : "0" ?>"
				/>
			</td>
			<td>
				<div
					id="municipio_campo_off"
					style="color:#a0a0a0;display:<?php echo !$possuiSelecionado ? "block" : "none" ?>"
					onclick="javascript:onOffCampo( 'municipio' );">
					<img src="../imagens/combo-todos.gif" border="0" align="middle">
				</div>
				<div id="municipio_campo_on" style="display:<?php echo $possuiSelecionado ? "block" : "none" ?>;">
					<?php
						$sql_combo ="SELECT
										--DISTINCT
										m.muncod AS codigo,
										m.estuf || ' - ' || mundescricao AS descricao
									FROM
										territorios.municipio m
									ORDER BY
										m.estuf";
						
						combo_popup(
							'f_municipio',						 
							$sql_combo,					 
							'Selecione o(s) município(s)',	 
							'400x400',						 
							0,								 
							array(),						 
							'',								 
							'S',							 
							true,							 
							true 		 
						);
					?>
				</div>
			</td>
		</tr>
		<tr>
			<td align="center" colspan="2">
				<input type="button" name="Gerar Relatório" value="Gerar Relatório" onclick="javascript:gerarRelatorio();"/>
			</td>
		</tr>
</table>		
</form>
</body>
</html>
<?php
function agrupador(){
	return array( 	
				array('codigo' => 'estado',
					  'descricao' => 'Estado'),
				array('codigo' => 'municipio',
					  'descricao' => 'Município'),
				array('codigo' => 'tipoesfera',
					  'descricao' => 'Esfera')		
			);
}
?>