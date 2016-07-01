<?php

function carregarModelo($marca) {
	global $db;
	$sql = "SELECT 
				mdlid as codigo,
				mdldsc as descricao
			FROM 
				demandas.modelo
			WHERE
				mrcid = {$marca}
				AND mdlstatus = 'A'
			ORDER BY mdldsc";
	
	$tipos = $db->carregar($sql);
	
	$html = "<option value=''>Selecione...</option>";
	
	if( is_array($tipos) ){
		foreach ($tipos as $tipo) {
			$html .= "<option value='{$tipo['codigo']}'>{$tipo['descricao']}</option>";
		}
		
	}
	return $html;
}

function formularioModelo() {
	global $db;
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Inserir Modelo</title>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel="stylesheet" type="text/css" href="../includes/listagem.css" />
<script type="text/javascript">
$(document).ready(function() {

	// função excluir
	$("[id^='excluir_']").click(function () {

		if( confirm('Deseja realmente excluir este Modelo?') ){

			var id = this.id.replace("excluir_","");
	
			//enviando o post
			$.post('demandas.php?modulo=principal/cadImpressao&acao=A&requisicao=excluirModelo', { id : id },
				function(data){
					alert('Modelo excluido com sucesso.');
//					$('#debug').html(data);
					$('#marca', window.opener.document).html(data);
					$('#modelo', window.opener.document).html('');
					$('#modelo', window.opener.document).attr('disabled', true);
					location.href = 'demandas.php?modulo=principal/cadImpressao&acao=A&requisicao=InserirModelo';
				});
			
		}
		
	})
	
	$("#salvar").click(function () {

		if ( !$('#marca').val() ){
			alert('Selecione uma Marca.');
			$('#marca').focus();
			return false;
			
		}else if( !$('#mdldsc').val() ){
			alert('Preencha o Modelo corretamente.');
			$('#mdldsc').focus();
			return false;
			
		}else{
			//enviando o post
			$.post('demandas.php?modulo=principal/cadImpressao&acao=A', { marca : $('#marca').val(), mdldsc : $('#mdldsc').val(), requisicao : 'salvarNovoModelo' },
				function(data){
//					$("#debug").html(data);
					// alterando o select da marca para a mesma opção que o usuário selecionou anteriormente
					$("#marca option[value='"+$('#marca').val()+"']", window.opener.document).attr('selected', 'selected');
					// atualizando o select do modelo
					$('#modelo', window.opener.document).html(data);
					$('#modelo', window.opener.document).attr('disabled', false);
				});
			alert('Modelo salvo com sucesso.');
			location.href = 'demandas.php?modulo=principal/cadImpressao&acao=A&requisicao=InserirModelo';
			return true;
		}
		
	})
});
</script>
</head>
<body>
<form action="" method="post" name="frm">
	<table cellspacing="0" cellpadding="2" border="0" align="center" width="95%" class="listagem">
		<tr>
			<td bgcolor="#DCDCDC" colspan="2" align="center" width="100%">
				<label class="TituloTela">Novo Modelo</label>
			</td>
		</tr>
		<tr>
			<td class="SubTituloEsquerda">Marca</td>
			<td>
			<?php
					$sql = "SELECT 
								mrcid as codigo, 
								mrcdsc as descricao
							FROM 
								demandas.marca
							WHERE
								mrcstatus = 'A'
							ORDER BY mrcdsc";
					
					echo $db->monta_combo("marca",$sql, 'S','Selecione...', '', '', '', 250, 'S','marca','',$_REQUEST['marca']);
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloEsquerda">Modelo</td>
			<td><input type="text" id="mdldsc" name="mdldsc" maxlength="250">&nbsp;<img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório."></td>
		</tr>
		<tr>
			<td bgcolor="#d0d0d0"></td>
			<td bgcolor="#d0d0d0"><input type="button" value="Salvar" id="salvar"></td>
		</tr>
	</table>
</form>
<div id="debug"></div>

<?php 

$sql = "SELECT 
			'<center>
				<img style=\"cursor: pointer;\" src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir\" id=\"excluir_'|| dm.mdlid ||'\" title=\"excluir\" alt=\"excluir\" \">
			 </center>' as acao,
			 dma.mrcdsc as marca,
			 dm.mdldsc as modelo
		FROM 
			demandas.modelo dm
		INNER JOIN
			demandas.marca dma ON dma.mrcid=dm.mrcid
		WHERE
			mdlstatus = 'A'
		ORDER BY mdldsc";

$cabecalho = array( "Excluir", "Marca", "Modelo" );
$db->monta_lista($sql, $cabecalho, 50, 20, '', 'center', '');

?>

</body>
</html>
<?php
}

function salvarNovoModelo($marcaId, $modeloDsc){
	global $db;
	
	$sql = "INSERT INTO 
				demandas.modelo
			(mrcid, mdldsc, mdlstatus)
    		VALUES 
    		({$marcaId}, '{$modeloDsc}', 'A')";
	
	$db->carregar(iconv("UTF-8", "ISO-8859-1", $sql));
	$db->commit();
	
	$sql = "SELECT 
				mdlid as codigo,
				mdldsc as descricao
			FROM 
				demandas.modelo
			WHERE
				mrcid = {$marcaId}
				AND mdlstatus = 'A'
			ORDER BY mdldsc";
	
	$tipos = $db->carregar($sql);
	
	$html = "<option value=''>Selecione...</option>";
	
	if( is_array($tipos) ){
		foreach ($tipos as $tipo) {
			$html .= "<option value='{$tipo['codigo']}'>{$tipo['descricao']}</option>";
		}
		
	}
	return $html;
	
}

function formularioMarca() {
	global $db;
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Inserir Modelo</title>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel="stylesheet" type="text/css" href="../includes/listagem.css" />
<script type="text/javascript">
$(document).ready(function() {

	// função excluir
	$("[id^='excluir_']").click(function () {

		if( confirm('Deseja realmente excluir esta marca?') ){

			var id = this.id.replace("excluir_","");
	
			//enviando o post
			$.post('demandas.php?modulo=principal/cadImpressao&acao=A&requisicao=excluirMarca', { id : id },
				function(data){
					alert('Marca excluida com sucesso.');
					$('#marca', window.opener.document).html(data);
					location.href = 'demandas.php?modulo=principal/cadImpressao&acao=A&requisicao=InserirMarca';
				});
			
		}
		
	})
	
	$("#salvar").click(function () {

		if ( !$('#marca').val() ){
			alert('Preencha a Marca corretamente.');
			$('#marca').focus();
			return false;
			
		}else{
			//enviando o post
			$.post('demandas.php?modulo=principal/cadImpressao&acao=A', { marca : $('#marca').val(), requisicao : 'salvarNovaMarca' },
				function(data){
//					$("#debug").html(data);
					// alterando o select da marca para a mesma opção que o usuário selecionou anteriormente
//					$("#marca option[value='"+$('#marca').val()+"']", window.opener.document).attr('selected', 'selected');
					// atualizando o select da marca
					$('#marca', window.opener.document).html(data);
				});
			alert('Marca salva com sucesso.');
			location.href = 'demandas.php?modulo=principal/cadImpressao&acao=A&requisicao=InserirMarca';
			return true;
		}
		
	})
	
});
</script>
</head>
<body>
<form action="" method="post" name="frm">
	<table cellspacing="0" cellpadding="2" border="0" align="center" width="95%" class="listagem">
		<tr>
			<td bgcolor="#DCDCDC" colspan="2" align="center" width="100%">
				<label class="TituloTela">Nova Marca</label>
			</td>
		</tr>
		<tr>
			<td class="SubTituloEsquerda">Marca</td>
			<td><input type="text" name="marca" id="marca" maxlength="250"></td>
		</tr>
		<tr>
			<td bgcolor="#d0d0d0"></td>
			<td bgcolor="#d0d0d0"><input type="button" value="Salvar" id="salvar"></td>
		</tr>
	</table>
</form>
<div id="debug"></div>

<?php 

$sql = "SELECT 
			'<center>
				<img style=\"cursor: pointer;\" src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir\" id=\"excluir_'|| mrcid ||'\" title=\"excluir\" alt=\"excluir\" \">
			 </center>' as acao,
			mrcdsc as marca
		FROM 
			demandas.marca
		WHERE
			mrcstatus = 'A'
		ORDER BY mrcdsc";

$cabecalho = array( "Excluir", "Marca" );
$db->monta_lista($sql, $cabecalho, 50, 20, '', 'center', '');

?>

</body>
</html>
<?php	
}

function salvarNovaMarca($marca) {
	global $db;
	
	$sql = "INSERT INTO 
				demandas.marca
			(mrcdsc, mrcstatus)
    		VALUES 
    		('{$marca}', 'A')";
	
	$db->carregar($sql);
	$db->commit();
	
	$sql = "SELECT 
				mrcid as codigo, 
				mrcdsc as descricao
			FROM 
				demandas.marca
			WHERE
				mrcstatus = 'A'";
	
	$tipos = $db->carregar($sql);
	
	$html = "<option value=''>Selecione...</option>";
	
	if( is_array($tipos) ){
		foreach ($tipos as $tipo) {
			$html .= "<option value='{$tipo['codigo']}'>{$tipo['descricao']}</option>";
		}
		
	}
	return $html;
	
}

function salvarImpressao($modeloId, $dataInicio, $dataFim, $valor) {
	global $db;
	
	$modeloId 	= (int)$modeloId;	
	$dataInicio = formata_data_sql($dataInicio);
	$dataFim 	= formata_data_sql($dataFim);
	$valor		= str_replace(",",".", str_replace(".","",$valor ) );
	
	$sql = "INSERT INTO 
				demandas.valorimpressao
				(mdlid, vlivalor, vlidtinicio, vlidtfim, vlistatus)
    		VALUES 
    			({$modeloId}, {$valor}, '{$dataInicio}', '{$dataFim}', 'A')";
	
	$db->carregar($sql);
	$db->commit();
	
	echo '<script type="text/javascript">
			alert("Impressão salva com sucesso.");
			location.href="demandas.php?modulo=principal/cadImpressao&acao=A";
		  </script>';
	
}

function excluirImpressao($vliid) {
	global $db;
	
	$vliid = (int)$vliid;
	
	$sql = "UPDATE 
				demandas.valorimpressao
			SET 
				vlistatus = 'I'
			WHERE 
				vliid = {$vliid}";
	
	$db->carregar($sql);
	$db->commit();
	
	
}

function pesquisarImpressao($array = array()){
	
	$arCondicao = array();
	
	if( $array['marca'] ){
		$arCondicao[] = "dma.mrcid = {$array['marca']}";
	}
	
	if( $array['modelo'] ){
		$arCondicao[] = "dm.mdlid = {$array['modelo']}";
	}
	
	if( $array['datainicio'] ){
		$arCondicao[] = "dvi.vlidtinicio = '".formata_data_sql($array['datainicio'])."'";
	}
	
	if( $array['datafim'] ){
		$arCondicao[] = "dvi.vlidtfim = '".formata_data_sql($array['datafim'])."'";
	}
	
	if( $array['valor'] ){
		$arCondicao[] = "dvi.vlivalor = ".str_replace(",",".", str_replace(".","", $array['valor'] ) );
	}
	
	return $arCondicao;
	
}

function excluirMarca($mrcid) {
	global $db;
	
	$sql = "UPDATE 
				demandas.marca
			SET 
				mrcstatus = 'I'
			WHERE 
				mrcid = {$mrcid}";
	
	$db->carregar($sql);
	$db->commit();
	
	$sql = "SELECT 
				mrcid as codigo, 
				mrcdsc as descricao
			FROM 
				demandas.marca
			WHERE
				mrcstatus = 'A'";
	
	$marcas = $db->carregar($sql);
	
	$html = "<option value=''>Selecione...</option>";
	
	foreach ($marcas as $marca) {
		$html .= "<option value='{$marca['codigo']}'>{$marca['descricao']}</option>";
	}
	
	return $html;
	
}

function excluirModelo($mdlid) {
	global $db;
	
	$sql = "UPDATE 
				demandas.modelo
   			SET 
   				mdlstatus = 'I'
 			WHERE 
 				mdlid = {$mdlid}";
	
	$db->carregar($sql);
	$db->commit();
	
	$sql = "SELECT 
				mrcid as codigo, 
				mrcdsc as descricao
			FROM 
				demandas.marca
			WHERE
				mrcstatus = 'A'";
	
	$marcas = $db->carregar($sql);
	
	$html = "<option value=''>Selecione...</option>";
	
	foreach ($marcas as $marca) {
		$html .= "<option value='{$marca['codigo']}'>{$marca['descricao']}</option>";
	}
	
	return $html;
	
}

if ($_REQUEST['requisicao']) {
	switch ($_REQUEST['requisicao'] ) {
		
		case 'ajaxmarca':
			// fazendo o ajax para procurar o modelo de acordo com a marca
			header("Content-Type: text/html; charset=ISO-8859-1");
			echo carregarModelo($_REQUEST['marca']);
			exit();
			
		break;
		
		case 'InserirModelo':
			// exibindo o formulário para cadastrar um novo modelo de impressora
			formularioModelo();
			exit();
			
		break;
		
		case 'salvarNovoModelo':
			// salvando um novo modelo
			echo salvarNovoModelo($_REQUEST['marca'], $_REQUEST['mdldsc']);
			exit();
			
		break;
		
		case 'InserirMarca':
			//exibindo o formulário para cadastrar uma nova marca
			formularioMarca();
			exit();
		break;
		
		case 'salvarNovaMarca':
			// salvando uma nova marca
			echo salvarNovaMarca($_REQUEST['marca']);
			exit();
			
		break;
		
		case 'salvarImpressao':
			// salvando uma impressão
			echo salvarImpressao($_REQUEST['modelo'], $_REQUEST['datainicio'], $_REQUEST['datafim'], $_REQUEST['valor']);
			exit();
			
		break;
		
		case 'excluirImpressao':
			// excluindo impressão
			echo excluirImpressao($_REQUEST['id']);
			exit();
			
		break;
		
		case 'excluirMarca':
			// excluindo a marca
			echo excluirMarca($_REQUEST['id']);
			exit();
			
		break;
		
		case 'excluirModelo':
			// excluindo o modelo
			echo excluirModelo($_REQUEST['id']);
			exit();
			
		break;
		
		case 'pesquisarImpressao';
			// pesquisando as impressões cadastradas
			$arCondicao = array();
			$arCondicao = pesquisarImpressao($_POST);
		break;
		
		
	}// fim do switch
}// fim do if

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo '<br>';
$db->cria_aba( $abacod_tela, $url, '' );

$titulo_modulo = "Cadastro de Impressão";
monta_titulo( $titulo_modulo, "&nbsp;");

?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
$(document).ready(function() {

	// ajax da marca
	$("#marca").change(function () {

		$('#modelo').html('<option>Aguarde...</option>');

		var marca = $("#marca").val();

		//se marca não possuir valor então eu saio da função
		if(!marca){
			$('#modelo').attr('disabled', true);
			$('#modelo').html('');
			return false;
		}
		
		//enviando o post
		$.post('demandas.php?modulo=principal/cadImpressao&acao=A&requisicao=ajaxmarca', { marca : marca },
			function(data){
//				$('#debug').html(data);
				$('#modelo').html(data);
				$('#modelo').attr('disabled', false);
			});
		
	})
	
	// botão inserir
	$("[id^='Inserir']").click(function () {
		window.open('demandas.php?modulo=principal/cadImpressao&acao=A&requisicao='+this.id,this.id,'width=530,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
	})

	// botão salvar
	$("#salvar").click(function () {

		var obData = new Data();

		if( !$('select:#marca option:selected').val() ){
			alert('Selecione uma Marca.');
			$('#marca').focus();
			return false;
			
		}else if( !$('select:#modelo option:selected').val() ){
			alert('Selecione um Modelo.');
			$('#modelo').focus();
			return false;
			
		}else if( !$('#datainicio').val() ){
			alert('Preencha a Data de Início corretamente.');
			$('#datainicio').focus();
			return false;
			
		}else if( !$('#datafim').val() ){
			alert('Preencha a Data Fim corretamente.');
			$('#datafim').focus();
			return false;
			
		}else if( obData.comparaData($('#datafim').val() ,$('#datainicio').val(), '<') ){
			alert('Preencha Data Início e Data Fim corretamente.');
			$('#datainicio').val('');
			$('#datafim').val('');
			$('#datainicio').focus();
			return false;
			
		}else if( !$('#valor').val() ){
			alert('Preencha o Valor da Impressão corretamente.');
			$('#valor').focus();
			return false;
			
		}else{
			$('#debug').html('<input type="hidden" name="requisicao" value="salvarImpressao">');
			document.formulario.submit();
			return true;
		}
			
	})

	// função excluir
	$("[id^='excluir_']").click(function () {

		if( confirm('Deseja realmente excluir este registro?') ){

			var id = this.id.replace("excluir_","");
	
			//enviando o post
			$.post('demandas.php?modulo=principal/cadImpressao&acao=A&requisicao=excluirImpressao', { id : id },
				function(){
					alert('Registro excluido com sucesso.');
					location.href='demandas.php?modulo=principal/cadImpressao&acao=A';
				});
			
		}
		
	})

	// botão pesquisar
	$("#pesquisar").click(function () {
		$('#debug').html('<input type="hidden" name="requisicao" value="pesquisarImpressao">');
		document.formulario.submit();
	})
	
});
</script>

<form name="formulario" method="post">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" border="0">
		<tr>
			<td class="SubTituloEsquerda">Marca:</td>
			<td>
				<?php
					$sql = "SELECT 
								mrcid as codigo, 
								mrcdsc as descricao
							FROM 
								demandas.marca
							WHERE
								mrcstatus = 'A'";
					
					echo $db->monta_combo("marca",$sql, 'S','Selecione...', '', '', '', 250, 'S','marca','',$_REQUEST['marca']);
				?>
				&nbsp;
				<a href="#" id="InserirMarca">
					<img border="0" title="Inserir Marca" alt="Inserir Marca" style="cursor: pointer;" src="/imagens/gif_inclui.gif">
					Inserir Marca
				</a>
			</td>
		</tr>
		<tr>
			<td class="SubTituloEsquerda">Modelo:</td>
			<td>
			
			<?php
				if( $_REQUEST['marca'] && $_REQUEST['modelo'] ){
					
					$sql = "SELECT 
								mdlid as codigo,
								mdldsc as descricao
							FROM 
								demandas.modelo
							WHERE
								mrcid = {$_REQUEST['marca']}
								AND mdlstatus = 'A'
							ORDER BY mdldsc";

					echo $db->monta_combo("modelo",$sql, 'S','Selecione...', '', '', '', 250, 'S','modelo','',$_REQUEST['modelo']);
					
				}else{
			?>
				<select id="modelo" style="width: auto;" class="CampoEstilo" name="modelo" disabled="disabled">
				</select>
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
				<?php } ?>
				&nbsp;
				<a href="#" id="InserirModelo">
					<img border="0" title="Inserir Modelo" alt="Inserir Modelo" style="cursor: pointer;" src="/imagens/gif_inclui.gif">
					Inserir Modelo
				</a>
			</td>
		</tr>
		<tr>
			<td class="SubTituloEsquerda">Data Início:</td>
			<td><?php echo campo_data2( 'datainicio', 'S', 'S', '', 'S', '', '', formata_data_sql($_REQUEST['datainicio']) ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloEsquerda">Data Fim:</td>
			<td><?php echo campo_data2( 'datafim', 'S', 'S', '', 'S', '', '', formata_data_sql($_REQUEST['datafim']) ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloEsquerda">Valor da Impressão:</td>
			<td>
				<?php echo campo_texto( 'valor', 'S', 'S', 'Valor da Impressão', 18, 18, '[###.]###,##', '','','','','id="valor"','',$_REQUEST['valor'],"calculaTotal(this.value); this.value=mascaraglobal('[###.]###,##',this.value)"); ?>
			</td>
		</tr>
		<tr>
			<td bgcolor="#d0d0d0"></td>
			<td bgcolor="#d0d0d0">
				<input type="button" value="Salvar" id="salvar">
				&nbsp;
				<input type="button" value="Pesquisar" id="pesquisar">
			</td>
		</tr>
	</table>
	<div id="debug"></div>
</form>

<?php 

$sql = "SELECT 
			'<center>
				<img style=\"cursor: pointer;\" src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir\" id=\"excluir_'|| dvi.vliid ||'\" title=\"excluir\" alt=\"excluir\" \">
			 </center>' as acao,
			 dma.mrcdsc as marca,
			 dm.mdldsc as modelo,
			 to_char(dvi.vlidtinicio, 'DD/MM/YYYY') as data_inicio,
			 to_char(dvi.vlidtfim, 'DD/MM/YYYY') as data_fim,
			 'R$ ' || dvi.vlivalor as valor
		FROM 
			demandas.valorimpressao dvi
		INNER JOIN
			demandas.modelo dm ON dm.mdlid = dvi.mdlid
		INNER JOIN
			demandas.marca dma ON dma.mrcid = dm.mrcid
		WHERE
			dvi.vlistatus = 'A'
			AND dm.mdlstatus = 'A'
			AND dma.mrcstatus = 'A'" . ($arCondicao ? ' AND '.implode(' AND ', $arCondicao) : '');

$cabecalho = array( "Excluir", "Marca", "Modelo", "Data Início", "Data Fim", "Valor da Impressão" );
$db->monta_lista($sql, $cabecalho, 50, 20, '', 'center', '');

?>