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

function salvarImpressora($array = array()) {
	global $db;
	
	$array['unidade'] 	= (int)$array['unidade'];
	$array['modelo']	= (int)$array['modelo'];
	$array['sala']		= (int)$array['sala'];
	
	$sql = "INSERT INTO 
				demandas.impressora
			(ungcod, mdlid, iprnome, iprnumserie, iprsala, iprstatus, iprdatainclusao)
			    VALUES 
			({$array['unidade']}, {$array['modelo']}, '{$array['nome']}', '{$array['numserie']}', {$array['sala']}, 'A', now())";
	
	$db->carregar($sql);
	$db->commit();
	
	echo '<script type="text/javascript">
			alert("Impressora salva com sucesso.");
			location.href="demandas.php?modulo=principal/cadImpressora&acao=A";
		  </script>';	

}

function excluirImpressora($iprid) {
	global $db;
	
	$iprid = (int)$iprid;
	
	$sql = "UPDATE 
				demandas.impressora
   			SET 
       			iprstatus='I'
 			WHERE 
 				iprid={$iprid}";
	
	$db->carregar($sql);
	$db->commit();
	
}

function pesquisarImpressora($array = array()) {
	
	$arCondicao = array();
	
	if ($array['unidade']) {
		$arCondicao[] = "pug.ungcod = '{$array['unidade']}'";
	}
	
	if ($array['marca']) {
		$arCondicao[] = "dma.mrcid = {$array['marca']}";
	}
	
	if ($array['modelo']) {
		$arCondicao[] = "dm.mdlid = {$array['modelo']}";
	}
	
	if ($array['nome']) {
		$arCondicao[] = "di.iprnome ILIKE '%{$array['nome']}%'";
	}
	
	if ($array['numserie']) {
		$arCondicao[] = "di.iprnumserie ILIKE '%{$array['numserie']}%'";
	}
	
	if ($array['sala']) {
		$arCondicao[] = "di.iprsala = {$array['sala']}";
	}
	
	return $arCondicao;
	
}

if ($_REQUEST['requisicao']) {
	switch ($_REQUEST['requisicao'] ) {
		
		case 'ajaxmarca':
			// fazendo o ajax para procurar o modelo de acordo com a marca
			header("Content-Type: text/html; charset=ISO-8859-1");
			echo carregarModelo($_REQUEST['marca']);
			exit();
			
		break;
		
		case 'salvarImpressora':
			// salvando uma impressora
			echo salvarImpressora($_REQUEST);
			exit();
			
		break;
		
		case 'excluirImpressora':
			// excluindo impressora
			echo excluirImpressora($_REQUEST['id']);
			exit();
			
		break;
		
		case 'pesquisarImpressora':
			// pesquisando a impressora
			$arCondicao = pesquisarImpressora($_REQUEST);
			
		break;
		
		
	}
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo '<br>';
$db->cria_aba( $abacod_tela, $url, '' );

$titulo_modulo = "Cadastro de Impressora";
monta_titulo( $titulo_modulo, "&nbsp;");

?>

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
		$.post('demandas.php?modulo=principal/cadImpressora&acao=A&requisicao=ajaxmarca', { marca : marca },
			function(data){
//				$('#debug').html(data);
				$('#modelo').html(data);
				$('#modelo').attr('disabled', false);
			});
		
	})

	// botão salvar
	$("#salvar").click(function () {

		if( !$('select:#unidade option:selected').val() ){
			alert('Selecione uma Unidade Gestora.');
			$('#unidade').focus();
			return false;
			
		}else if( !$('select:#marca option:selected').val() ){
			alert('Selecione uma Marca.');
			$('#marca').focus();
			return false;
			
		}else if( !$('select:#modelo option:selected').val() ){
			alert('Selecione um Modelo.');
			$('#modelo').focus();
			return false;
			
		}else if( !$('#nome').val() ){
			alert('Preencha o Nome da Impressora corretamente.');
			$('#nome').focus();
			return false;
			
		}else if( !$('#numserie').val() ){
			alert('Preencha o Número de Série corretamente.');
			$('#numserie').focus();
			return false;
			
		}else if( !$('#sala').val() ){
			alert('Preencha o Número da Sala corretamente.');
			$('#sala').focus();
			return false;
			
		}else{
			$('#debug').html('<input type="hidden" name="requisicao" value="salvarImpressora">');
			document.formulario.submit();
			return true;
		}
			
	})

	// função excluir
	$("[id^='excluir_']").click(function () {

		if( confirm('Deseja realmente excluir este registro?') ){

			var id = this.id.replace("excluir_","");
	
			//enviando o post
			$.post('demandas.php?modulo=principal/cadImpressora&acao=A&requisicao=excluirImpressora', { id : id },
				function(){
					alert('Registro excluido com sucesso.');
					location.href='demandas.php?modulo=principal/cadImpressora&acao=A';
				});
			
		}
		
	})

	// botão pesquisar
	$("#pesquisar").click(function () {
		$('#debug').html('<input type="hidden" name="requisicao" value="pesquisarImpressora">');
		document.formulario.submit();
	})

	// botão inserir
	$("[id^='Inserir']").click(function () {
		window.open('demandas.php?modulo=principal/cadImpressao&acao=A&requisicao='+this.id,this.id,'width=530,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
	})
	
});
</script>

<form name="formulario" method="post">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" border="0">
		<tr>
			<td class="SubTituloEsquerda">Undidade Gestora:</td>
			<td>
				<?php
					$sql = "SELECT 
								ungcod as codigo,
								ungdsc as descricao
							FROM 
								public.unidadegestora
							WHERE
								ungstatus = 'A'
							ORDER BY ungdsc";
					
					echo $db->monta_combo("unidade",$sql, 'S','Selecione...', '', '', '', 250, 'S','unidade','',$_REQUEST['unidade']);
				?>
			</td>
		</tr>
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
			<td class="SubTituloEsquerda">Nome da Impressora:</td>
			<td>
				<input type="text" name="nome" id="nome" value="<?php echo $_REQUEST['nome'] ?>" maxlength="250">
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
		</tr>
		<tr>
			<td class="SubTituloEsquerda">Número de Série da Impressora:</td>
			<td>
				<input type="text" name="numserie" id="numserie" value="<?php echo $_REQUEST['numserie'] ?>" maxlength="250">
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
		</tr>
		<tr>
			<td class="SubTituloEsquerda">Sala:</td>
			<td>
				<input type="text" name="sala" id="sala" value="<?php echo $_REQUEST['sala'] ?>" maxlength="250">
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
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
				<img style=\"cursor: pointer;\" src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir\" id=\"excluir_'|| di.iprid ||'\" title=\"excluir\" alt=\"excluir\" \">
			 </center>' as acao,
			pug.ungdsc as unidade_gestora,
			dma.mrcdsc as marca,
			dm.mdldsc as modelo,
			di.iprnome as compartilhamento,
			di.iprnumserie as numero_de_serie,
			di.iprsala as sala
		FROM 
			demandas.impressora di
		INNER JOIN
			demandas.modelo dm ON dm.mdlid = di.mdlid
		INNER JOIN
			demandas.marca dma ON dma.mrcid = dm.mrcid
		INNER JOIN
			public.unidadegestora pug ON pug.ungcod = di.ungcod
		WHERE
			di.iprstatus = 'A'
			AND dm.mdlstatus = 'A'
			AND dma.mrcstatus = 'A'
			AND pug.ungstatus = 'A'" . ($arCondicao ? ' AND '.implode(' AND ', $arCondicao) : '');

$cabecalho = array( "Excluir", "Unidade Gestora", "Marca", "Modelo", "Nome de Compartilhamento", "Número de Série", "Sala" );
$db->monta_lista($sql, $cabecalho, 50, 20, '', 'center', '');

?>
