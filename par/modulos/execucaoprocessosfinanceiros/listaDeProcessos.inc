<?php
include APPRAIZ . "par/modulos/execucaoprocessosfinanceiros/classes/processos.class.inc";
include APPRAIZ . "includes/cabecalho.inc";
echo'<br>';
monta_titulo( $titulo_modulo, '' );
?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>

<form action="" method="post" name="formularioBusca" id="formularioBusca" >
	<input type="hidden" name="requisicao" id="requisicao" value="<?=$_POST['requisicao']; ?>"> 
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
	<thead>
		<tr>
			<td style="width: 300px;"></td>
			<td></td>
		</tr>
	</thead>
	<tbody>
		<tr>
			<td colspan="2" class="subtituloEsquerda">Filtros:</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Busca Geral:</td>
			<td>
				<? echo campo_texto('filtroGeral', 'N', 'S', '', 37, 25, '', '', 'left', '', 0, 'id="filtroGeral" style="text-align:right;" '); ?>
				(O campo "busca geral" filtra sua pesquisa por qualquer campos apresentados na lista de processos.)
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" width="20%"></td>
			<td>
				<img src="../imagens/seta_ordemDESC.gif" id="setasubacaavancadacima" style="cursor: pointer;"> 
				<img src="../imagens/seta_ordemASC.gif" id="setasubacaavancadabaixo" style="cursor: pointer;"> 
				<img src="../imagens/busca.gif" id="btnbuscaavancada" style="cursor: pointer;"> 
				<span id="textobuscaavancada" style="cursor: pointer;" >Busca Avançada </span>
			</td>
		</tr>
		<tr id="buscaavancada">
			<td colspan="2">
				<table class="tabela" align="center" style="width: 100%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
					<tr>
						<td class="SubTituloDireita">Número de Processo:</td>
						<td colspan="3">
						<?php
							$filtro = simec_htmlentities( $_POST['numeroprocesso'] );
							$numeroprocesso = $filtro;
							echo campo_texto( 'numeroprocesso', 'N', 'S', '', 50, 200, '', ''); 
						?>
						</td>
						
					</tr>
					<tr>
						<td class="SubTituloDireita" >Município:</td>
						<td colspan="3">
						<?php 
							$filtro = simec_htmlentities( $_POST['municipio'] );
							$municipio = $filtro;
							echo campo_texto( 'municipio', 'N', 'S', '', 50, 200, '', ''); 
						?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita" >Estado:</td>
						<td colspan="3">
						<?php
							$estuf = $_POST['estuf'];
							$sql = "select e.estuf as codigo, e.estdescricao as descricao from territorios.estado e order by e.estdescricao asc";
							$db->monta_combo( "estuf", $sql, 'S', 'Todas as Unidades Federais', '', '' );
						?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita" >Empenho Solicitado:</td>
						<td colspan="3">
							Sim: <input <?php echo $_REQUEST['empenhado'] == "1" ? "checked='checked'" : "" ?> type="radio" name="empenhado" value="1" >
							Não: <input <?php echo $_REQUEST['empenhado'] == "2" ? "checked='checked'" : "" ?> type="radio" name="empenhado" value="2" >
						</td> 
					</tr>
					<tr>
						<td class="SubTituloDireita" >Tipo Execução:</td>
						<td colspan="3">
							Convênio: <input <?php echo $_REQUEST['tipoExecucao'] == "C" ? "checked='checked'" : "" ?> type="radio" name="tipoExecucao" value="C" >
							Termo: <input <?php echo $_REQUEST['tipoExecucao'] == "T" ? "checked='checked'" : "" ?> type="radio" name="tipoExecucao" value="T" >
							Todos: <input <?php echo $_REQUEST['tipoExecucao'] == "TTE" ? "checked='checked'" : "" ?> type="radio" name="tipoExecucao" value="TTE" >
						</td> 
					</tr>
					<tr>
						<td class="subtitulodireita">Esfera:</td>
						<td>
							Estadual: <input <?php echo $_REQUEST['esfera'] == "e" ? "checked='checked'" : "" ?> type="radio" value="e" id="esfera" name="esfera" />
							Municipal: <input <?php echo $_REQUEST['esfera'] == "m" ? "checked='checked'" : "" ?> type="radio" value="m" id="esfera" name="esfera"  />
							Todos: <input <?php echo $_REQUEST['esfera'] == "t" ? "checked='checked'" : "" ?> type="radio" value="t" id="esfera" name="esfera"  />
						</td>
					</tr>
					<tr>
						<td class="subtitulodireita">Ano do Processo:</td>
						<td>
							<?php
							$anoprocesso = $_POST['anoprocesso'];
							$sql = "select proano as codigo, proano as descricao from execucaofinanceira.processos
									where proano is not null and prostatus = 'A'
									group by proano";
							$db->monta_combo( "anoprocesso", $sql, 'S', 'Todos', '', '' );
							?>
						</td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita"></td>
			<td>
				<input type="button" name="filtrar" id="filtrar" value="Filtrar">
				<input type="button" name="listarTodos" id="listarTodos" value="Listar Todos">
			</td>
		</tr>
	</tbody>
</table>
</form>
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
	<tr>
		<td class="subtituloCentro">Lista de Processos</td>
	</tr>
</table>
<?php	
	$processo = new Processos();
	$processo->listaprocessos($_POST);
?>

<script type="text/javascript">
	$(document).ready(function() {
		if( $('#requisicao').val() == 'pesquisar' ){
			$("#buscaavancada").show();
		} else {
			$("#buscaavancada").hide();
		}
		$("#setasubacaavancadabaixo").hide();
	})
	
	$("#btnbuscaavancada,#textobuscaavancada").click(function() {
		if($("#buscaavancada").css("display") == 'none'){
			$("#buscaavancada").show();
			$("#setasubacaavancadabaixo").show();
			$("#setasubacaavancadacima").hide();
		}else{
			$("#buscaavancada").hide();
			$("#setasubacaavancadabaixo").hide();
			$("#setasubacaavancadacima").show();
		}	
	});

	$("#filtrar").click(function() {
		if($("#filtroGeral")){
			$('#requisicao').val('pesquisar');
			$("[name='formularioBusca']").submit()
		}
	});
	
	$("#listarTodos").click(function() {
		$("#formularioBusca input, #formularioBusca select").each(function(){
			$(this).attr('value', '');
		});
		$("[name='formularioBusca']").submit()
		
	});
	
	function editarProcesso(numeroProcesso){
		window.open("par.php?modulo=execucaoprocessosfinanceiros/administrarProcesso&acao=A&processo="+numeroProcesso,"Administração de Processos",'scrollbars=yes,height=500,width=900,status=no,toolbar=no,menubar=no,location=no');
	}
	
</script>