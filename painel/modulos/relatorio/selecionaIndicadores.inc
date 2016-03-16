<?php
if($_REQUEST['requisicaoAjax']){
	header('content-type: text/html; charset=ISO-8859-1');
	$_REQUEST['requisicaoAjax']();
	exit;
}


//função ativada após uma requisicao ajax, esta que filtra os indicadores na lista de DISPONÍVEIS.
function filtrarIndicadores($filtrar = null) {
	global $db;

	$unidadeDesteIndicador = $_REQUEST['comMesmaUnidadeIndicador'];
	$unidadesMedicao = $_REQUEST['unidadesMedicao'];
	$exoID = $_REQUEST['exoid'];
	$secID = $_REQUEST['secid'];
	$acaID = $_REQUEST['acaid'];
	$regID = $_REQUEST['regid'];
	$perID = $_REQUEST['perid'];
	$umeID = $_REQUEST['umeid'];

	$unidadesMedicao = trim($unidadesMedicao,",");
	
	// apenas indicadores publicados
	$sql = "select indid::varchar as codigo, indnome as descricao from painel.indicador ";
	$filtro = "where";

	
	//caso o check de agrupar esteja ativo, são disponibilizados apenas indicadores com o id do primeiro selecionado
	if ($unidadeDesteIndicador != "")
	{
		$filtro .= " and unmid = (select unmid from painel.indicador where indid = ".$unidadeDesteIndicador.") ";	
	} 
	
	if ($unidadesMedicao != "")
	{
		$filtro .= " and unmid in (".$unidadesMedicao.")  ";
	}
	
	if($exoID != "")
	{
		$filtro .= " and exoid =".$exoID;
	}

	if($secID != "")
	{
		$filtro .= " and secid =".$secID;
	}

	if($acaID != "")
	{
		$filtro .= " and acaid =".$acaID;
	}

	if($regID != "")
	{
		$filtro .= " and regid =".$regID;
	}

	if($perID != "")
	{
		$filtro .= " and perid =".$perID;
	}
	
	if($umeID != "")
	{
		$filtro .= " and umeid =".$umeID;
	}

	$escreveObjeto = true;
	if ($filtro != "where")
	{
		
		$filtro = str_replace("where", "where indpublicado and indstatus = 'A' ", $filtro);
		
		$sql.=$filtro;		
				
		$sql.="order by indid, indnome ";
		$rs = @$db->carregar($sql);

		echo "<select onclick=\"adicionarIndicador(this.value, this.options[this.selectedIndex].text,'slIndicadoresSelecionados',true);\"  size='8'  name='slIndicadores'  id='slIndicadores' style='width:700px;'  class='CampoEstilo' >";
		if ($rs)
		{
			foreach($rs as $r):
				echo "<option value=\"".$r['codigo']."\">".$r['codigo']." - ".$r['descricao']."</option>";
			endforeach;
		}
		echo "</select>";
	}
}
?>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Indicadores</title>
<script type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />

<script type="text/javascript"> 


//envia para o form pai os indicadores selecionados pelo usuário
function enviaIndicadores()
{
	var elementoDestino = window.opener.document.getElementsByName('slIndicadores[]')[0];
	var elementoOrigem  = document.getElementsByName('slIndicadoresSelecionados')[0];
	
	var novoOption = null ;
	elementoDestino.options.length = 0;
	for (var i=0; i<elementoOrigem.options.length; i++){

		novoOption = document.createElement('option');
		novoOption.value = elementoOrigem.options[i].value;
		novoOption.text  = elementoOrigem.options[i].text;

		elementoDestino.add(novoOption, null);
	}
	//chama a funçao que gera as colunas de indicadores
	if ($('#ckagruparunidades').attr('checked'))
	{
	 	window.opener.verificarIndicadores(true);
	 	window.opener.document.getElementById('agruparunidades').value='1';
	}
	else
	{
		window.opener.verificarIndicadores(false);
		window.opener.document.getElementById('agruparunidades').value='0';
	}
	window.opener.gerenciarAgrupadores();
	window.close();
}


//apenas reordena alfabeticamente os elementos na lista de indicadores disponíveis
function ordenaElemento() {
	$('#slIndicadores').each(function() {
		$(this).html($("option", $(this)).sort(function(a, b) { 
		    var valora = parseInt($(a).attr('value'), 10);
	    	var valorb = parseInt($(b).attr('value'), 10);
	    	return valora == valorb ? 0 : valora < valorb ? -1 : 1 
		}));
	});
}

//adiciona um indicador à lista de SELECIONADOS. removendo em seguida o item da lista de DISPONÍVEIS 
function adicionarIndicador(indid, inddsc,elementoAlvo,removerApos)
{
	if (indid != undefined)
	{
		//o indice do elemento slIndicadores é 11 e slIndicadoresSelecionados = 12
		
		var elemento = document.getElementById(elementoAlvo);

		for (var i=0; i<elemento.options.length; i++){
			if (elemento.options[i].value==indid){
				alert('Indicador já selecionado');
				return;
			}
		}
		
		var novoOption = document.createElement('option');
		novoOption.text = inddsc;
		novoOption.value = indid;
  
		try {
			elemento.add(novoOption, null); 
		}
		catch(ex) {
			elemento.add(novoOption); // IE
		}
		
		if (removerApos)
		{
			removerIndicador(indid,'slIndicadores',false);
		}

		gerenciarIndicadoresSelecionados();
		
	}
}

//remove um indicador selecionado. 
//como o item é removido da lista de DISPONÍVEIS após selecionado, ele é readicionado nessa lista após remoção.
function removerIndicador(indid,elementoAlvo,readicionar)
{
	
	if (indid != undefined)
	{
	var elemento = document.getElementById(elementoAlvo);
	for (var i=0; i<elemento.options.length; i++){
		if (elemento.options[i].value==indid){
			if (readicionar)
			{
				adicionarIndicador(indid,elemento.options[i].text,'slIndicadores',false);
				ordenaElemento();
			}
			elemento.remove(i);
			return;
		}
	}
	gerenciarIndicadoresSelecionados();
	}
}


//limpa os check de unidades numericas (a regra é numérico OU não numérico)
function gerenciaUnidadeMedicao(valor)
{
	if (valor.trim() != '')
	{
		document.getElementsByName('ckUnidadeMedicao')[0].checked = false;
		document.getElementsByName('ckUnidadeMedicao')[1].checked = false;
	}
	$("#slIndicadoresSelecionados").empty();
	ajax();
}

//limpa o combo de unidades não numericas (a regra é numérico OU não numérico)
function gerenciaUnidadeMedicaoNumerica()
{
	if ($("input[name='ckUnidadeMedicao']:checked").length > 0)
	{
		var elemento = document.getElementsByName('cbunidademedicao')[0];
		elemento.selectedIndex = 0;
	}	
	//$("#slIndicadoresSelecionados").empty();
	ajax();
}



function gerenciarIndicadoresSelecionados()
{
		
	if ($("#ckAgrupar:checked").length > 0)
	{
		
	qtdSelecionados = $("#slIndicadoresSelecionados option").length;

	if ( qtdSelecionados  == 1 )
	{
		valor = $("#slIndicadoresSelecionados option:eq(0)").val();

		//Caso o check de "agrupar" estiver selecionado, Selecione os indicadores tem que ter a mesma unidade de medicao.
		//Logo, é guardado o id do primeiro indicador selecionado, para em seguida trazer apenas indicadores que tenham o mesmo tipo de unidade deste.
		$("#comMesmaUnidadeIndicador").val(valor);

		ajax();
	}
	}
	
}


//função que envia os valores selecionados para filtrar os indicadores disponíveis na lista de DISPONÍVEIS
function ajax(valor){

	unidades = $("#cbUnidadeMedicao").val();

	$("input[name='ckUnidadeMedicao']:checked").each(function(){
		unidades += this.value + ",";
	});

	
	if (unidades == '')
	{
	
		$('#cboprodutoUnidade option:eq(0)').attr('selected','selected'); 
		$('#cboeixo option:eq(0)').attr('selected','selected');
		$('#cbosecretaria option:eq(0)').attr('selected','selected');
		$('#cboacao option:eq(0)').attr('selected','selected');
		$('#cboregionalizacao option:eq(0)').attr('selected','selected');
		$('#cboperiodicidade option:eq(0)').attr('selected','selected');

		alert('Selecione uma unidade de medida');
		return;
	}

	jQuery.ajax({
		type: "POST",
		url: window.location,
		data: "requisicaoAjax=filtrarIndicadores&filtrar=1&exoid="+document.getElementsByName('cboeixo')[0].value+"&secid="+document.getElementsByName('cbosecretaria')[0].value+
		"&acaid="+document.getElementsByName('cboacao')[0].value+"&regid="+document.getElementsByName('cboregionalizacao')[0].value+"&perid="+document.getElementsByName('cboperiodicidade')[0].value +
		"&comMesmaUnidadeIndicador=" + $("#comMesmaUnidadeIndicador").val()+"&unidadesMedicao="+unidades+"&umeid="+$("#cboprodutoUnidade").val(),
		success: function(retorno){
			jQuery('#id_indicador').html(retorno);
	     // a respeito do parametro comMesmaUnidadeIndicador, ler comentario na função gerenciarIndicadoresSelecionados
		}
	});
}


//Caso o check de "agrupar" estiver selecionado, Selecione os indicadores tem que ter a mesma unidade de medição.
//caso haja alguma mudança no estado do check deve se redefinir a lista de selecionados.
//se ja havia indicadores selecionados, envia se uma nova reqisição ajax para voltar a disponibilizar indicadores com todas unidades de medição.
function gerenciarAgrupar()
{
	qtdSelecionados = $("#slIndicadoresSelecionados option").length;

	if ( qtdSelecionados  > 0 )
	{
		$("#slIndicadoresSelecionados").empty();
		$("#comMesmaUnidadeIndicador").val("");
		ajax();
	}
	else
	{
		$("#slIndicadoresSelecionados").empty();
		$("#comMesmaUnidadeIndicador").val("");
	}
	
}

//função de ativação de handlers e inicialização de componentes.
function suporte()
{

	$("input[name='ckAgrupar']").click(gerenciarAgrupar);
	$("input[name='cbUnidadeMedicao']").change(gerenciarAgrupar);
	$("input[name='ckUnidadeMedicao']").click(gerenciaUnidadeMedicaoNumerica);
	
}

</script>
</head>
<body onload = "suporte();" >
	<form name="formulario" id="formulario" action="" method="post">
		
		<table id="tabelaIndicadores" class="tabela" align="center"
			bgcolor="#f5f5f5" cellspacing="1">
			<tr>
				<td class="subtituloesquerda" colspan="2"><strong>Filtros</strong>
				<input type = "button" value="testes"  style="visibility:hidden;" onclick="suporte();" >
				</td>
			</tr>
			<!-- 
			<tr>
				<td class="subtitulodireita">Permitir Agrupamento</td>
				<td>
				<input type = "checkbox"  id = "ckAgrupar" name = "ckAgrupar" value="1" title="Com esta opção selecionada, só serão disponibilizados indicadores com a mesma unidade métrica.">
				</td>
			</tr>
			 -->
			 <input type = "hidden"  id = "ckAgrupar" name = "ckAgrupar" value="1" title="Com esta opção selecionada, só serão disponibilizados indicadores com a mesma unidade métrica.">
			<tr>
				<td class="subtitulodireita">Unidades de medição não numéricas</td>
				<td><?php
				$sql = "SELECT unmid AS codigo, unmdesc AS descricao
						FROM painel.unidademedicao
						where unmid not in (3,5)
						AND unmstatus = 'A'
						order by descricao";
				$db->monta_combo("cbUnidadeMedicao", $sql,'S', 'Selecione', 'gerenciaUnidadeMedicao', null,'Selecione a unidade de medição!', '244',false,"cbUnidadeMedicao");
				?>
				</td>
			</tr>
			<tr>
				<td class="subtitulodireita">Unidades de medição numéricas/monetárias</td>
				<td>
				<?php
				$sql = "SELECT unmid AS codigo, unmdesc AS descricao
							 FROM painel.unidademedicao where unmid in (3,5) AND unmstatus = 'A' order by descricao";
				$db->monta_checkbox("ckUnidadeMedicao", $sql,null, null);
				?>
				</td>
			</tr>
			<tr>
				<td class="subtitulodireita">Produto da unidade</td>
				<td><?php
				$sql = "SELECT umeid AS codigo, umedesc AS descricao
							 FROM painel.unidademeta WHERE umestatus = 'A' order by descricao";
				$db->monta_combo("cboprodutoUnidade", $sql,'S', 'Selecione', 'ajax', '','Selecione o produto da unidade!', '244',false,"cboprodutoUnidade");
				?>
				</td>
			</tr>
			<tr>
				<td class="subtitulodireita">Eixo</td>
				<td><?php
				$sql = "SELECT exoid AS codigo, exodsc AS descricao
							 FROM painel.eixo order by descricao";
				$db->monta_combo("cboEixo", $sql,'S', 'Selecione', 'ajax', '','Selecione o item caso queira filtrar por eixo!', '244', false, "cboEixo");
				?>
				</td>
			</tr>
			<tr>
				<td class="subtitulodireita">Secretaria</td>
				<td><?php
				$sql = "SELECT secid AS codigo, secdsc AS descricao
							 FROM painel.secretaria where secstatus = 'A' order by descricao";
				$db->monta_combo("cboSecretaria", $sql,'S', 'Selecione', 'ajax', '','Selecione o item caso queira filtrar por secretaria!', '244',false, "cboSecretaria");
				?>
				</td>
			</tr>
			<tr>
				<td class="subtitulodireita">Ação</td>
				<td><?php
				$sql = "SELECT acaid AS codigo, acadsc AS descricao
							 FROM painel.acao where acastatus = 'A' order by descricao";
				$db->monta_combo("cboAcao", $sql,'S', 'Selecione', 'ajax', '','Selecione o item caso queira filtrar por ação!', '244',false,"cboAcao");
				?>
				</td>
			</tr>
			<tr>
				<td class="subtitulodireita">Regionalização</td>
				<td><?php
				$sql = "SELECT regid AS codigo, regdescricao AS descricao
							 FROM painel.regionalizacao where regstatus = 'A' order by descricao";
				$db->monta_combo("cboRegionalizacao", $sql,'S', 'Selecione', 'ajax', '','Selecione o item caso queira filtrar por regionalização!', '244',false,"cboRegionalizacao");
				?>
				</td>
			</tr>
			<tr>
				<td class="subtitulodireita">Periodicidade</td>
				<td><?php
				$sql = "SELECT perid AS codigo, perdsc AS descricao
							 FROM painel.periodicidade where perstatus = 'A' order by descricao";
				$db->monta_combo("cboPeriodicidade", $sql,'S', 'Selecione', 'ajax', '','Selecione o item caso queira filtrar por periodicidade!', '244',false,"cboPeriodicidade");
				?>
				</td>
			</tr>
			<tr>
			<tr>
				<td class="subtituloesquerda" align=left>Agrupar Unidades <input type=checkbox name=ckagruparunidades id=ckagruparunidades valu=true></td>
			</tr>
			<tr>
				<td class="subtituloesquerda" colspan="2"><strong>Selecione os
						Indicadores</strong>
				</td>
			</tr>
			<tr>
				<td id="id_indicador" class="subtituloesquerda" colspan="2"><?php
				if ($filtrar)
				{
					filtrarIndicadores();
				}
				else
				{
				?>
				<select onclick="selecionarIndicador();" id='slIndicadores' name='slIndicadores' onchange="selecionarIndicador();" multiple size='8' style='width: 700px;'
					class='CampoEstilo'>
						
				</select>
				<?php 
				}
				?>
				</td>
			<tr>
			<tr>
				<td class="subtituloesquerda" colspan="2"><strong>Indicadores
						Selecionados</strong>
				</td>
			</tr>
			<tr>
				<td id="id_indicador_selecionado" class="subtituloesquerda" colspan="2">
				<select onclick="removerIndicador(this.value,'slIndicadoresSelecionados',true);"   size='8' name='slIndicadoresSelecionados'
					id='slIndicadoresSelecionados' style='width: 700px;'
					class='CampoEstilo'>
				</select>
				</td>
			<tr>
			<tr>
				<td >
				<input type="button" value = "OK" onclick="enviaIndicadores();">
				<input type="button" value="Cancelar" onclick="window.close();">
				<input type="hidden" name = "comMesmaUnidadeIndicador" id = "comMesmaUnidadeIndicador" value = "">
				</td>

			<tr>
		</table>
	</form>
</body>
</html>
