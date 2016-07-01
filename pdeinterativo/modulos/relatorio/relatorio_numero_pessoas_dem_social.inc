<?php
set_time_limit(60);
ini_set("memory_limit", "1024M");

	if($_REQUEST['relatorio'] == 'relatorio'){ 
		
		if($_REQUEST['estuf']) {
			$estado = $_REQUEST['estuf'];
			$where .= "and pde.estuf = '$estado'";
		}
		
		if($_REQUEST['muncod']) {
			$municipio = $_REQUEST['muncod'];
			$where .= "and tm.muncod = '$municipio'";
		}
		
		$_REQUEST['curid'] = is_array( $_REQUEST['curid'] ) ? $_REQUEST['curid'] : array();
		$list_curid = '';
		foreach ($_REQUEST['curid'] as $curid => $cursos){
			$list_curid .= $cursos[0] . ',';
		}
		$list_curid = substr($list_curid, 0, -1);
		if($list_curid != '')
			$where .= ' and cur.curid in (' . $list_curid . ') ';
		
		if($_REQUEST['pcfano']) {
			$pcfano = $_REQUEST['pcfano'];
			$where .= "and pcf.pcfano = $pcfano";
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
			-- Número de pessoas da demanda social

			select distinct 
	              count (distinct dms.mdscpf)     as qtddem, 
	              tm.estuf                        as uf, 
	              tm.mundescricao                 as municipio,     
	              '('||cur.curid||') - '||curdesc as curso , 
	              pcf.pcfdesc                     as periodo
	        from    
	                           catalogocurso.membrosdemandasocial dms 
	                inner join pdeinterativo.pdinterativo pde         on dms.pdeid = pde.pdeid and pde.pdistatus = 'A' 
	                inner join catalogocurso.curso cur                on cur.curid = dms.curid and cur.curstatus = 'A' 
	                inner join workflow.documento wd                  on wd.docid = pde.formacaodocid 
	                inner join workflow.estadodocumento ed            on ed.esdid = wd.esdid 
	                inner join pdeinterativo.planoformacaodocente pfd on pfd.pdeid = pde.pdeid  and pfd.pfdstatus = 'A' 
	                inner join pdeinterativo.periodocursoformacao pcf on pcf.pcfid = pfd.pcfid 
	                inner join territorios.municipio tm               on tm.muncod = pde.muncod 
	        where wd.esdid = 417 
	        and dms.cpdstatus = 'A' 
			$where
			group by tm.estuf, tm.mundescricao, '('||cur.curid||') - '||curdesc, pcf.pcfdesc
			order by uf
		";
		//die($sql);
		$cabecalho = array("Qt. demandas", "UF", "Município", "Curso", "Período");
			
		if($_REQUEST['visualizar'] == 'html'){
			echo "
				<link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/Estilo.css\"/>
				<link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/listagem.css\"/>		
				<table class=tabela bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center>
					<tr>
						<td class=SubTituloCentro>Relatório Quantitativo: Número de pessoas da demanda social</td>
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
			$db->sql_to_excel($sql, 'Numero_de_Pessoas_da_Demanda_Social', $cabecalho);
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
	$titulo_modulo = "Relatório Quantitativo: Número de pessoas da demanda social";
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
				url: 'pdeinterativo.php?modulo=relatorio/relatorio_numero_pessoas_dem_social&acao=A',
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
				var janela = window.open('', 'numeroescola', 'width=880,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
				formulario.submit();
				janela.focus();
			} else {
				document.getElementById('visualizar').value='xls';
				formulario.submit();
			}
		}else{
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
			<td class="SubTituloDireita">Estado:</td>
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
					$db->monta_combo("estuf", $sql, $ativo,'Selecione...','', '', '','','N','estuf', '', '', 'UF');
				?>
			</td>
		</tr>		
		<tr>
			<td class="SubTituloDireita" id="td_municipio" style="display:yes;">Município</td>
			<td>
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
			<td class="SubTituloDireita" id="td_municipio" style="display:yes;">Periodo</td>
			<td>
				<div id="combo_municipio">
				<?php 
					$sql = "
						Select	pcfano as codigo,
								pcfdesc as descricao
						From pdeinterativo.periodocursoformacao
					";
					$db->monta_combo( "pcfano", $sql, $ativo, '', '', '','','','N','pcfano','','','Periodo');
				?>
				</div>
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
				combo_popup( 'esdid['. $esdid .']', $sql, 'Selecione o(s) status(s)', '360x460' );
			?>
			</td>
		</tr>				
		<tr>
			<td class="SubTituloDireita">Curso:</td>
			<td>
				<?php 
					$sql = "
						select	curid as codigo, 
								curdesc as descricao
						from catalogocurso.curso 
						where curstatus = 'A'
						order by curdesc
					";
					combo_popup( 'curid['. $curid .']', $sql, 'Selecione o(s) curso(s)', '360x460' );
				?>
			</td>
		<tr>
		<td bgcolor="#CCCCCC"></td>
			<td bgcolor="#CCCCCC">
				<input type="button" name="filtrar" value="Visualizar HTML" style="cursor: pointer;" onclick="exibirRelatorio(0);"/> 
				<input type="button" name="filtrar" value="Visualizar XLS"  style="cursor: pointer;" onclick="exibirRelatorio(1);"/>			
			</td>
		</tr>		
	</table>
</form>