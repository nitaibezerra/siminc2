<?php 

function organizaArrayMarcaModelo($array = array()) {
	
	$marcaModelo = array();
	$i = 0;
	$marcatmp = '';
	
	if(is_array($array)){
		
		foreach ($array as $valores){
			
			if($valores['marca'] != $marcatmp){
				$i = 0;
			}
			
			$marcaModelo[$valores['marca']][$i]['modelo'] = $valores['modelo'];
			$marcaModelo[$valores['marca']][$i]['modeloid'] = $valores['modeloid'];
			
			$marcatmp = $valores['marca'];
			$i++; 
		}// fim do foreach
		
	}// fim do if
	
	return $marcaModelo;
	
}

function carregaMarcaModelo($valor = 0) {
	global $db;
	
	$sql = "SELECT 
				dm.mrcdsc as marca,
				dmo.mdldsc as modelo,
				dmo.mdlid as modeloid
			FROM 
				demandas.marca dm
			INNER JOIN
				demandas.modelo dmo ON dmo.mrcid = dm.mrcid
			WHERE
				dm.mrcstatus = 'A'
				AND dmo.mdlstatus = 'A'
			ORDER BY dm.mrcdsc, dmo.mdldsc";
	
	$valores = $db->carregar($sql);
	
	$marcaModelo = organizaArrayMarcaModelo($valores);
	
	$html = "<select name='modelo' id='modelo' class='CampoEstilo' style='width: 200px;'>
				<option value=''>Selecione...</option>";
	
	foreach ($marcaModelo as $marca => $modelos) {
		$html .= "<optgroup label='{$marca}'>";

		foreach ($modelos as $modelo) {
			$html .= "<option ".($valor == $modelo['modeloid'] ? "selected='selected' " : "")."value='{$modelo['modeloid']}'>{$modelo['modelo']}</option>";
		}
		
		$html .= "</optgroup>";
		
	}
	
	return $html."</select>";
	
}

function ajaxImpressora($mdlid, $iprid = 0) {
	global $db;
	
	$sql = "SELECT 
				iprid as codigo,
				iprnome ||' - '|| iprnumserie as descricao
			FROM 
				demandas.impressora
			WHERE 
				iprstatus = 'A'
				AND mdlid = {$mdlid}
			ORDER BY iprnome";
	
	$impressoras = $db->carregar($sql);
	
	$html = "<option value = ''>Selecione...</option>";
	
	if (is_array($impressoras)) {
		foreach ($impressoras as $impressora) {
			$html .= "<option ".($iprid == $impressora['codigo'] ? "selected='selected'" : "")." value = '{$impressora['codigo']}'>{$impressora['descricao']}</option>";
		}// fim do foreach
	}// fim do if
	
	return $html;
	
}

function ajaxSala($iprid) {
	global $db;
	
	$iprid = (int)$iprid;
	
	$sql = "SELECT 
				di.iprsala as sala,
				pug.ungdsc as unidade,
				dvi.vlivalor as valor
			FROM 
				demandas.impressora di
			INNER JOIN
				public.unidadegestora pug ON pug.ungcod = di.ungcod
			
			INNER JOIN
				demandas.valorimpressao dvi ON dvi.mdlid = di.mdlid -- pq não utilizar o vliid da tabela valorimpressao?
				
			WHERE
				di.iprid = {$iprid}
				AND di.iprstatus = 'A'
				AND pug.ungstatus = 'A'";
	
	$dados = $db->pegaLinha($sql);
	
	$string = implode("|", $dados);
	
	return $string;
	
}

function salvarImpressao($array = array()) {
	global $db;
	
	$qtd_inpressoes = $array['fim'] - $array['inicio'];
	
	// pegando o valor da impressão
	$sql = "SELECT 
				dvi.vlivalor as valor
			FROM 
				demandas.impressora di
			INNER JOIN
				public.unidadegestora pug ON pug.ungcod = di.ungcod
			
			INNER JOIN
				demandas.valorimpressao dvi ON dvi.mdlid = di.mdlid
				
			WHERE
				di.iprid = {$array['impressora']}
				AND di.iprstatus = 'A'
				AND pug.ungstatus = 'A'";
	
	$valor = $db->pegaUm($sql);
	
	// salvando no banco de dados
	$sql = "INSERT INTO 
				demandas.historicoimpressao
			(iprid, hticontadorinicio, hticontadorfim, htiqtd, htivlrunidade, htimes, htiano)
    			VALUES 
    		({$array['impressora']}, {$array['inicio']}, {$array['fim']}, {$qtd_inpressoes}, {$valor}, '".date("m")."', '".date("Y")."')";
	
	$db->carregar($sql);
	$db->commit();
	
	echo '<script type="text/javascript">
			alert("Histórico Salvo com Sucesso.");
			location.href="demandas.php?modulo=principal/histImpressao&acao=A";
		  </script>';
	
}

function pesquisarHistImpressao($array = array()) {
	
	$arCondicao = array();
	
	if ($array['modelo']) {
		$arCondicao[] = "dm.mdlid = {$array['modelo']}";
	}
	
	if ($array['impressora']) {
		$arCondicao[] = "di.iprid = {$array['impressora']}";
	}
	
	return $arCondicao;
	
}

function cadastrarMultiplos($array = array()) {
	global $db;
	
	// substituindo o Enter "\n" por tab "	"
	$array['dados'] = str_replace("\n", "	", $array['dados']);
	// criando o array
	$dados = explode('	', $array['dados']);
	$tamanho = count($dados);
	
	for($i = 0; $i < $tamanho; $i += 3){
		
		if($dados[$i] != ""){
			
			$qtd_inpressoes = $dados[$i+2] - $dados[$i+1];
			
			// pegando o valor da impressão
			$sql = "SELECT 
						di.iprid as impressora,
						dvi.vlivalor as valor
					FROM 
						demandas.impressora di
					INNER JOIN
						public.unidadegestora pug ON pug.ungcod = di.ungcod
					
					INNER JOIN
						demandas.valorimpressao dvi ON dvi.mdlid = di.mdlid
						
					WHERE
						di.iprnumserie = '{$dados[$i]}'
						AND di.iprstatus = 'A'
						AND pug.ungstatus = 'A'";

			$valor = $db->pegaLinha($sql);
			
			if ($valor) {
				// salvando no banco de dados
				$sql = "INSERT INTO 
							demandas.historicoimpressao
						(iprid, hticontadorinicio, hticontadorfim, htiqtd, htivlrunidade, htimes, htiano)
			    			VALUES 
			    		({$valor['impressora']}, {$dados[$i+1]}, {$dados[$i+2]}, {$qtd_inpressoes}, {$valor['valor']}, '".date("m")."', '".date("Y")."')";

				$db->carregar($sql);
				
			}else{
				echo '<script type="text/javascript">
						alert("O valor de impressão da série '.$dados[$i].' ainda não foi cadastrada no banco de dados.");
						//location.href="demandas.php?modulo=principal/histImpressao&acao=A";
					  </script>';
				exit();
			}
			
			
		}// fim do if
		
	}// fim do for
	
	$db->commit();
	
	echo '<script type="text/javascript">
			alert("Impressoras Salvas com Sucesso.");
			location.href="demandas.php?modulo=principal/histImpressao&acao=A";
		  </script>';
	
}

if($_REQUEST['requisicao']){
	switch ($_REQUEST['requisicao']) {
		
		case 'ajaximpressora':
			// carregando o select das impressoras
			echo ajaxImpressora($_REQUEST['modelo']);
			exit();
			
		break;
		
		case 'ajaxsala':
			// carregando a sala da impressora
			echo ajaxSala($_REQUEST['impressora']);
			exit();
			
		break;
		
		case 'salvarImpressao':
			// salvando o histórico da impressão
			salvarImpressao($_REQUEST);
			exit();
			
		break;
		
		case 'pesquisarHistImpressao':
			// fazendo pesquisa
			$arCondicao = pesquisarHistImpressao($_POST);
		break;
		
		case 'ajaxdados':
			// cadastrando várias impressoras
			cadastrarMultiplos($_POST);
			exit();
			
		break;
		
	}// fim do switchcase
	
}// fim do if

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo '<br>';
$db->cria_aba( $abacod_tela, $url, '' );

$titulo_modulo = "Histórico de Impressão";
monta_titulo( $titulo_modulo, "&nbsp;");

?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
$(document).ready(function() {

	// ajax da impressora
	$("#modelo").change(function () {

		$('#impressora').html('<option>Aguarde...</option>');

		var modelo = $("#modelo").val();

		//se modelo não possuir valor então eu saio da função
		if(!modelo){
			$('#impressora').attr('disabled', true);
			$('#impressora').html('');
			return false;
		}
		
		//enviando o post
		$.post('demandas.php?modulo=principal/histImpressao&acao=A&requisicao=ajaximpressora', { modelo : modelo },
			function(data){
//				$('#debug').html(data);
				$('#impressora').html(data);
				$('#impressora').attr('disabled', false);
				$('#sala').val('');
				$('#unidade').html('');
				$('#valor').val('')
			});
		
	})
	
	// ajax para exibir a sala
	$("#impressora").change(function () {

		var impressora = $("#impressora").val();

		//se impressora não possuir valor então eu saio da função
		if(!impressora){
			$('#sala').attr('disabled', true);
			$('#sala').html('');
			return false;
		}

		//enviando o post
		$.post('demandas.php?modulo=principal/histImpressao&acao=A&requisicao=ajaxsala', { impressora : impressora },
			function(data){
//				$('#debug').html(data);

				var array_dados = new Array();
				var i;
				
				array_dados = data.split("|");

				$('#sala').val(array_dados[0]);
				$('#unidade').html("<option value=''>"+array_dados[1]+"</option>");
				$('#valor').val(array_dados[2]);

				$('#debug').html('<input type="hidden" name="sala_pesquisa" value="'+array_dados[0]+'"><input type="hidden" name="unidade_pesquisa" value="'+array_dados[1]+'"><input type="hidden" name="valor_pesquisa" value="'+array_dados[2]+'">');
				
			});
		
	})

	// botão salvar
	$("#salvar").click(function () {

		if( !$('select:#modelo option:selected').val() ){
			alert('Selecione um Modelo.');
			$('#modelo').focus();
			return false;
			
		}else if( !$('select:#impressora option:selected').val() ){
			alert('Selecione uma Impressora.');
			$('#impressora').focus();
			return false;
			
		}else if( !$('#inicio').val() ){
			alert('Preencha o Valor Inícial do Contador corretamente.');
			$('#inicio').focus();
			return false;
			
		}else if( !$('#fim').val() ){
			alert('Preencha o Valor Final do Contador corretamente.');
			$('#fim').focus();
			return false;
			
		}else if( $('#fim').val() < $('#inicio').val() ){
			alert('O valor final não pode ser menor que o inicial.');
			$('#inicio').val('');
			$('#fim').val('');
			$('#inicio').focus();
			return false;
			
		}else{
			$('#debug').html('<input type="hidden" name="requisicao" value="salvarImpressao">');
			document.formulario.submit();
			return true;
		}
			
	})

	// botão pesquisar
	$("#pesquisar").click(function () {
		var html = $('#debug').html(); 
		$('#debug').html(html+'<input type="hidden" name="requisicao" value="pesquisarHistImpressao">');
		document.formulario.submit();
	})

	// botão exportar múltiplos
	$("#exportar").click(function () {
		$("#tabeladados_1").css('display','table-row');
		$("#tabeladados_2").css('display','table-row');
		$("#tabeladados_3").css('display','table-row');
		$("#tabeladados_4").css('display','none');

		$("[id^='impressora_']").each(function() {
			$("#"+this.id).css('display','none');
		})
		
	})

	// botão cancelar
	$("#cancelar").click(function () {
		$("#tabeladados_1").css('display','none');
		$("#tabeladados_2").css('display','none');
		$("#tabeladados_3").css('display','none');
		$("#tabeladados_4").css('display','table-row');

		$("[id^='impressora_']").each(function() {
			$("#"+this.id).css('display','table-row');
		})
	})

	// botão salvar múltiplas impressoras
	$("#salvardados").click(function () {

		var dados = $('#dados').val();
		
		//enviando o post
		$.post('demandas.php?modulo=principal/histImpressao&acao=A&requisicao=ajaxdados', { dados : dados },
			function(data){
				$('#debug').html(data);
				
			});
		
	})
});
</script>

<form name="formulario" method="post">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" border="0">
		<tr id="tabeladados_1" style="display:none">
			<td></td>
			<td>
				Colunas:&nbsp;&nbsp;
				Série&nbsp;&nbsp;|
				Cont Anterior&nbsp;&nbsp;|&nbsp;&nbsp;
				Cont Atual
				<br/>
				Um registro por linha separados por tabulação
			</td>
		</tr>
		<tr id="tabeladados_2" style="display:none">
			<td align='right' class="SubTituloDireita">Dados:</td>
			<td align='left' width="90%" class="SubTituloDireita">
				<p style="text-align: left;">
					<? $dados = $_REQUEST['dados']; ?>
					<?= campo_textarea( 'dados', 'S', 'S', 'Dados', 125, 20, null ) ?>
				</p>
			</td>
		</tr>
		<tr id="tabeladados_3" style="display:none">
			<td bgcolor="#d0d0d0"></td>
			<td bgcolor="#d0d0d0">
				<input type="button" value="Salvar" id="salvardados">
				&nbsp;
				<input type="button" value="Cancelar" id="cancelar">
			</td>
		</tr>
		<tr id="tabeladados_4">
			<td bgcolor="#d0d0d0"></td>
			<td bgcolor="#d0d0d0">
				<input type="button" value="Exportar Múltiplos" id="exportar">
			</td>
		</tr>
		<tr id="impressora_1">
			<td class="SubTituloEsquerda">Marca/Modelo:</td>
			<td>
			<?php
				echo carregaMarcaModelo($_REQUEST['modelo']); 
			?>
			<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
		</tr>
		<tr id="impressora_2">
			<td class="SubTituloEsquerda">Nome da Impressora/Série:</td>
			<td>
			<?php 
				if ($_REQUEST['impressora'] && $_REQUEST['modelo']) {
					echo '<select id="impressora" style="width: auto;" class="CampoEstilo" name="impressora">';
					echo ajaxImpressora($_REQUEST['modelo'], $_REQUEST['impressora']);
					echo '</select>';
				}else{
			?>
				<select id="impressora" style="width: auto;" class="CampoEstilo" name="impressora" disabled="disabled">
				</select>
			<?php } ?>
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
		</tr>
		<tr id="impressora_3">
			<td class="SubTituloEsquerda">Sala:</td>
			<td><input type="text" name="sala" id="sala" disabled="disabled" value="<?php echo $_REQUEST['sala_pesquisa']; ?>"></td>
		</tr>
		<tr id="impressora_4">
			<td class="SubTituloEsquerda">Unidade Gestora:</td>
			<td>
				<select id="unidade" style="width: 250px;" class="CampoEstilo obrigatorio" name="unidade" disabled="disabled">
					<?php if ($_REQUEST['unidade_pesquisa']) { ?>
						<option><?php echo $_REQUEST['unidade_pesquisa']; ?></option>
					<?php }?>
				</select>
			</td>
		</tr>
		<tr id="impressora_5">
			<td class="SubTituloEsquerda">Valor por Impressão:</td>
			<td><input type="text" name="valor" id="valor" disabled="disabled" value="<?php echo $_REQUEST['valor_pesquisa']; ?>"></td>
		</tr>
		<tr id="impressora_6">
			<td class="SubTituloEsquerda">Contador Início:</td>
			<td>
				<input type="text" name="inicio" id="inicio" value="<?php echo $_REQUEST['inicio']; ?>">
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
		</tr>
		<tr id="impressora_7">
			<td class="SubTituloEsquerda">Contador Fim:</td>
			<td>
				<input type="text" name="fim" id="fim" value="<?php echo $_REQUEST['fim']; ?>">
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
		</tr>
		<tr id="impressora_8">
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
		       dma.mrcdsc ||' - '|| dm.mdldsc as marca_modelo,
		       di.iprnome ||' - '|| di.iprnumserie as nome_serie,
		       di.iprsala as sala,
		       pug.ungdsc as unidade_gestora,
		       dvi.vlivalor as valor
		FROM 
			demandas.historicoimpressao dhi
		INNER JOIN
			demandas.impressora di ON di.iprid = dhi.iprid
		INNER JOIN
			demandas.modelo dm ON dm.mdlid = di.mdlid
		INNER JOIN	
			demandas.marca dma ON dma.mrcid = dm.mrcid
		INNER JOIN
			public.unidadegestora pug ON pug.ungcod = di.ungcod
		INNER JOIN
			demandas.valorimpressao dvi ON dvi.mdlid = dm.mdlid
		WHERE
			dma.mrcstatus = 'A'
			AND dvi.vlistatus = 'A'
			AND dm.mdlstatus = 'A'
			AND di.iprstatus = 'A'
			AND pug.ungstatus = 'A'" . ($arCondicao ? ' AND '.implode(' AND ', $arCondicao) : '');

$cabecalho = array( "Marca/Modelo", "Nomde da Impressora/Série", "Sala", "Unidade Gestora", "Valor por Impressão" );
$db->monta_lista($sql, $cabecalho, 50, 20, '', 'center', '');

?>
