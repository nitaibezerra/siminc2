<?php

	if($_REQUEST['relatorio'] == 'relatorio'){

		$_REQUEST['esdid'] = is_array( $_REQUEST['esdid'] ) ? $_REQUEST['esdid'] : array();
		$list_esdid = '';
		foreach ($_REQUEST['esdid'] as $esdid => $status){
			$list_esdid .= $status[0] . ',';
		}
		$list_esdid = substr($list_esdid, 0, -1);
		if($list_esdid != '')
			$where .= ' and wd.esdid in (' . $list_esdid . ') ';
						
		$sql = "
			-- Número de municípios em que pelo menos 01 escola enviou Plano para a Secretaria
			select distinct tm.estuf as uf,
			       count(distinct tm.muncod) as municipio,       
			       ed.esddsc as status
			from pdeinterativo.pdinterativo pi
			inner join territorios.municipio tm ON tm.muncod = pi.muncod														
			inner join workflow.documento wd ON wd.docid = formacaodocid														
			inner join workflow.estadodocumento ed ON ed.esdid = wd.esdid														
			where   pi.formacaodocid is not null and pi.pdistatus = 'A' $where														
			group by tm.estuf , ed.esddsc														
			order by tm.estuf
		";
		//die($sql);
		$cabecalho = array("UF", "N. de Município", "Status");
			
		if($_REQUEST['visualizar'] == 'html'){
			echo "
				<link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/Estilo.css\"/>
				<link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/listagem.css\"/>		
				<table class=tabela bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center>
					<tr>						
						<td class=SubTituloCentro>Relatório Consolidado: Número de municípios que enviaram planos - Por Estado</td>
					</tr>
					<tr>
						<td>
			";
			$db->monta_lista_simples($sql, $cabecalho, 1000000, 10, 'S', '100%', $par2);
			echo "
						</td>
					</tr>
				</table>
			";
		}
		else{
			ob_clean();
			header('content-type: text/html; charset=ISO-8859-1');
			$db->sql_to_excel($sql, 'Numeros_de_Municipios_que_Enviaram_Planos', $cabecalho);
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
	$titulo_modulo = "Relatório Consolidado: Número de municípios que enviaram planos - Por Estado";
	monta_titulo( $titulo_modulo, 'Clique no botão abaixo para visualizar o relatório');
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
				var janela = window.open('', 'numeroescola', 'width=650,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
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
			<td class="SubTituloDireita">Status do documento:</td>
			<td>
			<?php
				$sql = "
					Select	esdid as codigo,
							esddsc as descricao
					From workflow.estadodocumento 
					where esdid in (416, 417)
				";
				combo_popup( 'esdid['. $esdid .']', $sql, 'Selecione o(s) status(s)', '360x460' );
			?>
			</td>
		</tr>	
		<tr>		
		<td bgcolor="#CCCCCC"></td>
			<td bgcolor="#CCCCCC">
				<input type="button" name="filtrar" value="Visualizar HTML" style="cursor: pointer;" onclick="exibirRelatorio(0);"/> 
				<input type="button" name="filtrar" value="Visualizar XLS"  style="cursor: pointer;" onclick="exibirRelatorio(1);"/>			
			</td>
		</tr>		
	</table>
</form>
