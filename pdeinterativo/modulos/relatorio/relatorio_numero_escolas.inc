<?php
	if($_REQUEST['relatorio'] == 'relatorio'){ 
		
		if($_REQUEST['estuf']) {
			$estado = $_REQUEST['estuf'];
			$where .= "and tm.estuf = '$estado'";
		}
		
		if($_REQUEST['muncod']) {
			$municipio = $_REQUEST['muncod'];
			$where .= "and tm.muncod = '$municipio'";
		}
		
		if($_REQUEST['esfera'] != 'T') {
			$esfera = $_REQUEST['esfera'];
			$where .= "and substr(pi.pdiesfera,1,1) = '$esfera'";
		}
		
		$_REQUEST['esdid'] = is_array( $_REQUEST['esdid'] ) ? $_REQUEST['esdid'] : array();
		$list_esdid = '';
		foreach ($_REQUEST['esdid'] as $esdid => $status){
			$list_esdid .= $status[0] . ',';
		}
		$list_esdid = substr($list_esdid, 0, -1);
		if($list_esdid != '')
			$where .= ' and wd.esdid in (' . $list_esdid . ') ';
		
		$sql = "
			select distinct count (distinct pdicodinep) as inep,
			       tm.estuf        as uf,
			       tm.mundescricao as municipio,
			       pi.pdiesfera    as esfera,
			       ed.esddsc       as status
			from pdeinterativo.pdinterativo pi
				inner join territorios.municipio tm on tm.muncod = pi.muncod
				inner join workflow.documento wd on wd.docid = formacaodocid
				inner join workflow.estadodocumento ed on ed.esdid = wd.esdid
			where pi.formacaodocid is not null and pi.pdistatus = 'A'
			$where
			group by tm.estuf, tm.mundescricao, pi.pdiesfera, ed.esddsc
			order by 1,2,3,4
		";
		$cabecalho = array("Escolas", "UF", "Município", "Esfera", "Status");
			
		if($_REQUEST['visualizar'] == 'html'){
			echo '
				<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
				<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
				<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					<tr>
						<td colspan="5" class="SubTituloCentro">Relatório Quantitativo: Escolas do plano de formação</td>
					</tr>
					<tr>
						<td>
			';
					
			
			$db->monta_lista_simples($sql, $cabecalho, 1000000, 10, 'S', '100%', $par2);
			echo '
						</td>
					</tr>
				</table>
			';
		}
		elseif($_REQUEST['visualizar'] == 'xls'){
			$dadosExcel = $db->carregar($sql);
			if(empty($dadosExcel)){
				echo '
					<script>
						alert("Nenhum registro encontrado!");
						window.close(); 
					</script>
				';
				exit;
			}
			else{
				ob_clean();
				header('content-type: text/html; charset=ISO-8859-1');
				$db->sql_to_excel($dadosExcel, 'Escolas_do_Plano_de_Formacao', $cabecalho);
			}
		}
		exit;
	}
?>

<?php
	if(isset($_POST['carregar_municipios'])){
		$sql = "
			Select 	muncod AS codigo, 
		 			mundescricao AS descricao 
			From territorios.municipio
			Where estuf = '".$_POST['carregar_municipios']."'
			Order by mundescricao asc
		";
		$db->monta_combo( "muncod", $sql, 'S', 'Selecione...', '', '' );
		exit;
	}	
	include APPRAIZ . 'includes/cabecalho.inc';
	echo "<br>";
	$titulo_modulo = "Relatório Quantitativo: Escolas do plano de formação";
	monta_titulo( $titulo_modulo, 'Selecione os filtros desejados' );
?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
	jQuery.noConflict();

	jQuery(function(){
		// Popula municipios
		jQuery('select[name=estuf]').change(function(){	
			jQuery('#td_municipio').show();
			jQuery('#combo_municipio').html('Carregando...');
			
			jQuery.ajax({
				url: 'pdeinterativo.php?modulo=relatorio/relatorio_numero_escolas&acao=A',
				type: 'post',
				data: 'carregar_municipios='+this.value,
				success: function(res){		
					jQuery('#combo_municipio').html(res);				
				}
			});	
		});
	});

	function exibirRelatorio(p) {
		if(document.getElementById('relatorio').value!='') {
			var formulario = document.formulario;
			prepara_formulario();
			if(p=='0') {
				document.getElementById('visualizar').value='html';
				// submete formulario
				formulario.target = 'numeroescola';
				var janela = window.open('', 'numeroescola', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
				formulario.submit();
				janela.focus();
			} else {
				document.getElementById('visualizar').value='xls';
				formulario.submit();
			}

		} else {
			alert("Selecione um relatório");
			return false;
		}
	}
</script>

<form action="" method="post" name="formulario" id="filtro">
	<input type="hidden" id="relatorio" name="relatorio" value="relatorio"/>
	<input type="hidden" name="visualizar" id="visualizar" value=""/>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">	
		<tr>	
			<td class="SubTituloDireita">Estado: </td>
				<td>									
					<?php
						$ativo = 'S';
						if($_SESSION['estuf']){
							$estuf = $_SESSION['estuf'];
							$ativo = 'N';
						}
						$sql = "
							Select 	e.estuf as codigo, 
							 		e.estdescricao as descricao 
							From territorios.estado e 
							Order by e.estdescricao ASC
						";
						$db->monta_combo( "estuf", $sql, $ativo, 'Selecione...', '', '');
					?>
				</td>
			</tr>		
			<tr>
				<td class="SubTituloDireita" id="td_municipio" style="display:yes;">Município<td>
					<div id="combo_municipio">
					<?php 
						$sql = "
							Select 	muncod AS codigo, 
						 			mundescricao AS descricao 
							From territorios.municipio
							limit 1
						";
						$db->monta_combo( "muncod", $sql, $ativo, 'Selecione...', '', '');
					?>
					</div>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Esfera:</td>
					<td>
					<?
						$arExp = array(
								array("codigo" => 'T',
									  "descricao" => 'Todos'),
								array("codigo" => 'M',
									  "descricao" => 'Municipal'),
								array("codigo" => 'E',
									  "descricao" => 'Estadual'),
								array("codigo" => 'F',
									  "descricao" => 'Federal')								
							  );
						$db->monta_combo("esfera", $arExp, 'S','','', '', '','','N','esfera', '', '', 'Esfera'); 
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Status do documento:</td>
				<td>
				<?php
					$sql = "
						Select	esdid as codigo,
								esddsc as descricao
						From workflow.estadodocumento 
						where esdid in (416, 417)
					";
					//$db->monta_combo("esdid", $sql, $ativo, 'Selecione...', '', '','','','N','esdid','','', 'Workflow');
					combo_popup( 'esdid['. $esdid .']', $sql, 'Selecione o(s) status(s)', '360x460' );
				?>
				<td>
			<tr>
			<tr>
				<td bgcolor="#CCCCCC"></td>
				<td bgcolor="#CCCCCC">
					<input type="button" name="filtrar" value="Visualizar HTML" style="cursor: pointer;" onclick="exibirRelatorio(0);"/> 
					<input type="button" name="filtrar" value="Visualizar XLS"  style="cursor: pointer;" onclick="exibirRelatorio(1);"/>			
				</td>
			</tr>		
	</table>
</form>
