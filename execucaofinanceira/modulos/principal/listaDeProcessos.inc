<?php
//include APPRAIZ . "execucaofinanceira/classes/processos.class.inc";
$obProcesso = new Processo();

if( $_POST['requisicao'] == 'mostraInformacoesProcesso' ){
	header('content-type: text/html; charset=ISO-8859-1');
	$obProcesso->carregaInformacoesProcesso($_POST['proid']);
	die();
}

if( $_POST['requisicao'] == 'excluir' ){
	
	$sql = "DELETE FROM execucaofinanceira.contacorrente WHERE proid = {$_POST['proid']}";
	$db->executar($sql);
	$sql = "DELETE FROM execucaofinanceira.processos WHERE proid = {$_POST['proid']}";
	$db->executar($sql);
	
	if($db->commit()){
		$obProcesso->sucesso('principal/listaDeProcessos');
	} else {
		echo "<script>
				alert('Falha na operação');
			</script>";
	}
}

$arrPerfil = carregarPerfil();

unset($_SESSION['execucaofinanceira']['proid']);
include APPRAIZ . "includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, $parametros);
monta_titulo( $titulo_modulo, '' );
?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>

<form action="" method="post" name="formularioBusca" id="formularioBusca" >
	<input type="hidden" name="requisicao" id="requisicao" value="<?=$_POST['requisicao']; ?>"> 
	<input type="hidden" name="proid" id="proid" value="">  
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
				<?
				$filtroGeral = $_POST['filtroGeral']; 
				echo campo_texto('filtroGeral', 'N', 'S', '', 37, 25, '', '', 'left', '', 0, 'id="filtroGeral" style="text-align:right;" '); ?>
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
						<td class="SubTituloDireita" >Tipo de Execução:</td>
						<td>
						<?
						$tprid = $_POST['tprid'];
						$sql = "SELECT tprid as codigo, tprdescricao as descricao FROM execucaofinanceira.tipoexecucao WHERE tprstatus = 'A' order by tprdescricao";
						$db->monta_combo( "tprid", $sql, 'S', 'Todos os Tipos de Execução', '', '' );
						?>
						</td> 
					</tr>
					<tr>
						<td class="SubTituloDireita" >Tipo de Processo:</td>
						<td>
						<?
						$tipid = $_POST['tipid'];
						$sql = "SELECT tipid as codigo, tipdescricao as descricao FROM execucaofinanceira.tipoprocesso WHERE tipstatus = 'A' order by tipdescricao";
						$db->monta_combo( "tipid", $sql, 'S', 'Todos os Tipos de Processo', '', '' );
						?>
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
	$obProcesso->listaprocessos($_POST, $arrPerfil);
?>

<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<script src="livro/cufon/cufon-yui.js" type="text/javascript"></script>
<script type="text/javascript" src="http://code.jquery.com/jquery-1.4.4.min.js"></script>
<script src="livro/booklet/jquery.easing.1.3.js" type="text/javascript"></script>
<script src="livro/booklet/jquery.booklet.1.1.0.min.js" type="text/javascript"></script>

<style type="text/css">
	
	.modalDialog_transparentDivs{	
		filter:alpha(opacity=40);	/* Transparency */
		opacity:0.4;	/* Transparency */
		background-color:#AAA;
		z-index:1;
		position:absolute; /* Always needed	*/
	}
	.modalDialog_contentDiv{
		border:3px solid #000;	
		padding:2px;
		z-index:100;/* Always needed	*/
		position:absolute;	/* Always needed	*/
		background-color:#FFF;	/* White background color for the message */
	}
	.modalDialog_contentDiv_shadow{
		z-index:90;/* Always needed	- to make it appear below the message */
		position:absolute;	/* Always needed	*/
		background-color:#555;
		filter:alpha(opacity=30);	/* Transparency */
		opacity:0.3;	/* Transparency */	
	}

</style>
		
<script type="text/javascript">
	messageObj = new DHTML_modalMessage();	// We only create one object of this class
	messageObj.setShadowOffset(5);	// Large shadow
	
	function displayMessage(url){
		messageObj.setSource(url);
		messageObj.setCssClassMessageBox(false);
		messageObj.setSize(1000,650);
		messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
		messageObj.display();
	}
	
	function closeMessage(){
		messageObj.close();
		window.location.href = "execucaofinanceira.php?modulo=principal/listaDeProcessos&acao=A";
	}
	
	$(document).ready(function() {
		if( $('#requisicao').val() == 'pesquisar' && $('#filtroGeral').val() == '' ){
			$("#buscaavancada").show();
		} else {
			$("#buscaavancada").hide();
		}
		$("#setasubacaavancadabaixo").hide();
	});
	
	function carregarDadosProcesso(idImg, proid){
		var img 	 = $('#'+idImg );
		var tr_nome = 'listaDadosProcesso_'+ proid;
		var td_nome  = 'trV_'+ proid;
		
		if($('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() == ''){
			$('#'+td_nome).html('Carregando...');
			img.attr('src', '../imagens/menos.gif');
			mostraInformacoesProcesso(proid, td_nome);
		}
		if($('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() != ""){
			$('#'+tr_nome).css('display', '');
			img.attr('src', '../imagens/menos.gif');
		} else {
			$('#'+tr_nome).css('display', 'none');
			img.attr('src', '/imagens/mais.gif');
		}
	}
	
	function mostraInformacoesProcesso(proid, td_nome){
		$.ajax({
	   		type: "POST",
	   		url: "execucaofinanceira.php?modulo=principal/listaDeProcessos&acao=A",
	   		data: "requisicao=mostraInformacoesProcesso&proid="+proid,
	   		async: false,
	   		success: function(msg){
	   			$('#'+td_nome).html(msg);
	   			//$('[class="listagem"]').attr('align', 'left');
	   		}
		});
	}
	
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
			$("[name='formularioBusca']").submit();
		}
	});
	
	$("#listarTodos").click(function() {
		$("#formularioBusca input, #formularioBusca select").each(function(){
			$(this).attr('value', '');
		});
		$("[name='formularioBusca']").submit();
		
	});
	
	function editarProcesso(numeroProcesso){
		window.location.href = 'execucaofinanceira.php?modulo=principal/administrarProcesso&acao=A&processo='+numeroProcesso;
		//window.open("par.php?modulo=principal/administrarProcesso&acao=A&processo="+numeroProcesso,"Administração de Processos",'scrollbars=yes,height=500,width=900,status=no,toolbar=no,menubar=no,location=no');
	}
	
	function visualizarProcesso(numeroProcesso){
		//window.location.href = 'execucaofinanceira.php?modulo=principal/visualizaProcessoCompleto&acao=A&processo='+numeroProcesso;
		window.open("execucaofinanceira.php?modulo=principal/visualizaProcessoCompleto&acao=A&processo="+numeroProcesso+"&popup=true","Administração de Processos",'height=640,width=980,scrollbars=no,fullscreen=no,status=no,toolbar=no,menubar=no,location=no');
		
		//displayMessage('execucaofinanceira.php?modulo=principal/visualizaProcessoCompleto&acao=A&processo='+numeroProcesso);
	}
	
	function excluirProcesso(proid){
		$('#proid').val(proid);
		$('#requisicao').val('excluir');
		$("[name='formularioBusca']").submit();
	}
	
</script>