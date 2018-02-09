<?php
include APPRAIZ. '/includes/Agrupador.php';
require_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( "Relatórios Gerencial", '<b>Filtros de Pesquisa</b>');

function agrupador(){
	return array(
				array('codigo' => 'ptrcod',						'descricao' => 'PTA'),
				);
}

function colunas(){
	return array(
				array('codigo' => 'ptrcod',					'descricao' => 'PTA'),
				array('codigo' => 'numprocesso', 			'descricao' => 'Nº do Processo'),
				array('codigo' => 'numempenho', 			'descricao' => 'Nº do Empenho'),
				array('codigo' => 'estado', 				'descricao' => 'UF da Entidade'),
				array('codigo' => 'mundescricao', 			'descricao' => 'Município'),
				array('codigo' => 'numfnde',				'descricao' => 'Nº do convênio FNDE'),
				array('codigo' => 'numsiafi',				'descricao' => 'Nº do convênio SIAFI'),
				array('codigo' => 'valor_do_projeto',		'descricao' => 'Valor do Projeto'),
				array('codigo' => 'valor_empenhado',		'descricao' => 'Valor Empenhado'),
				array('codigo' => 'valor_pagamento',		'descricao' => 'Detalhamento do Pagamento'),
				array('codigo' => 'partido',				'descricao' => 'Partido'),
								
				);
}
?>
<html>
<head>
	<script type="text/javascript" src="/includes/prototype.js"></script>
</head>
<body>
<form action="emenda.php?modulo=relatorio/popUpRelatorioGerencial&acao=A" method="post" name="formulario" id="formulario">
<input type="hidden" name="limpaSession" id="limpaSession" value="true">
<table id="table" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<!--<tr>
		<td class="SubTituloDireita" style="width: 33%" valign="top"><b>Ordenação</b></td>
		<td>
			<?php
			
			$matriz = agrupador();
			$campoAgrupador = new Agrupador( 'formulario' );
			$campoAgrupador->setOrigem( 'agrupadorOrigem', null, $matriz );
			$campoAgrupador->setDestino( 'agrupador', null, array(
																	/*array('codigo' => 'autid',
					  													  'descricao' => 'Autor')*/			
																	));
			$campoAgrupador->exibir();
			
			?>
			
		</td>
	</tr>
	--><tr>
		<td class="SubTituloDireita" style="width: 33%" valign="top"><b>Colunas</b></td>
		<td>
			<?php
			
			$coluna = colunas();
			$campoColuna = new Agrupador( 'formulario' );
			$campoColuna->setOrigem( 'colunaOrigem', null, $coluna );
			$campoColuna->setDestino( 'coluna', null, array(
															array('codigo' => 'ptrcod',					'descricao' => 'PTA'),
															array('codigo' => 'numprocesso', 			'descricao' => 'Nº do Processo'),
															array('codigo' => 'numempenho', 			'descricao' => 'Nº do Empenho'),
															array('codigo' => 'estado', 				'descricao' => 'UF da Entidade'),
															array('codigo' => 'mundescricao', 			'descricao' => 'Município'),
															array('codigo' => 'numfnde',				'descricao' => 'Nº do convênio FNDE'),
															array('codigo' => 'numsiafi',				'descricao' => 'Nº do convênio SIAFI'),
															array('codigo' => 'valor_do_projeto',		'descricao' => 'Valor do Projeto'),
															array('codigo' => 'valor_empenhado',		'descricao' => 'Valor Empenhado'),
															array('codigo' => 'valor_pagamento',		'descricao' => 'Detalhamento do Pagamento'),
															array('codigo' => 'partido',				'descricao' => 'Partido'),
															));
			$campoColuna->exibir();
			?>
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita"><b>Número do PTA:</b></td>
		<td>
			<?php 
				$ptrcod = $_REQUEST["ptrcod"];
				echo campo_texto( 'ptrcod', 'N', 'S', '', 12, 8, '[#]', '', 'left', '', 0, '');
			?>
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita"><b>Foi Gerado Convênio:</b></td>
		<td>
			<input type="radio" value="S" id="geradoconvenio" name="geradoconvenio" <? if($_REQUEST["geradoconvenio"] == "S") { echo "checked"; } ?> /> Sim
			<input type="radio" value="N" id="geradoconvenio" name="geradoconvenio" <? if($_REQUEST["geradoconvenio"] == "N") { echo "checked"; } ?> /> Não
			<input type="radio" value="" id="geradoconvenio" name="geradoconvenio"  <? if($_REQUEST["geradoconvenio"] == "") { echo "checked"; } ?> /> Todos
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita"><b>Empenhado:</b></td>
		<td>
			<input type="radio" value="S" id="empenhado" name="empenhado" <? if($_REQUEST["empenhado"] == "S") { echo "checked"; } ?> /> Sim
			<input type="radio" value="N" id="empenhado" name="empenhado" <? if($_REQUEST["empenhado"] == "N") { echo "checked"; } ?> /> Não
			<input type="radio" value="" id="empenhado" name="empenhado"  <? if($_REQUEST["empenhado"] == "") { echo "checked"; } ?> /> Todos
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita"><b>Pagamento Solicitado:</b></td>
		<td>
			<input type="radio" value="S" id="pagamento" name="pagamento" <? if($_REQUEST["pagamento"] == "S") { echo "checked"; } ?> /> Sim
			<input type="radio" value="N" id="pagamento" name="pagamento" <? if($_REQUEST["pagamento"] == "N") { echo "checked"; } ?> /> Não
			<input type="radio" value="" id="pagamento" name="pagamento"  <? if($_REQUEST["pagamento"] == "") { echo "checked"; } ?> /> Todos
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita"><b>Pagamento Efetivado:</b></td>
		<td>
			<input type="radio" value="S" id="pagamentoE" name="pagamentoE" <? if($_REQUEST["pagamentoE"] == "S") { echo "checked"; } ?> /> Sim
			<input type="radio" value="N" id="pagamentoE" name="pagamentoE" <? if($_REQUEST["pagamentoE"] == "N") { echo "checked"; } ?> /> Não
			<input type="radio" value="" id="pagamentoE" name="pagamentoE"  <? if($_REQUEST["pagamentoE"] == "") { echo "checked"; } ?> /> Todos
		</td>
	</tr>
	<tr>
	<?php
		//Estado
		$estuf = $_REQUEST["estuf"];		
		$sql = "SELECT 
				  estuf as codigo,
				  estuf||' - '||estdescricao as descricao
				FROM 
				  territorios.estado 
				ORDER BY estdescricao";

		$stSqlCarregados = "";
		mostrarComboPopup( '<b>Estado</b>', 'estuf',  $sql, $stSqlCarregados, 'Selecione o(s) Estados(s)' ); 				
	?>
	</tr>
	<tr>
	<?php /*
		//Entidade
		$enbid = $_REQUEST["enbid"];
		
		$sql = "SELECT DISTINCT 
					eb.enbid as codigo,
					eb.enbnome as descricao
				FROM
					emenda.entidadebeneficiada eb
					inner join emenda.planotrabalho ptr on ptr.enbid = eb.enbid and ptr.ptrstatus = 'A' and eb.enbstatus = 'A'
				ORDER BY
					eb.enbnome";

		$stSqlCarregados = "";
		mostrarComboPopup( '<b>Entidade Beneficiada</b>', 'enbid',  $sql, $stSqlCarregados, 'Selecione a(s) Entidade Beneficiada(s)' ); 
			
		//Autor da Emenda
		$autid = $_REQUEST["autid"];
		
		$sql = "SELECT DISTINCT 
					ea.autid as codigo,
					ea.autnome as descricao
				FROM
					emenda.autor ea
				INNER JOIN
					emenda.emenda ee ON ea.autid = ee.autid
				ORDER BY
					ea.autnome";

		$stSqlCarregados = "";
		mostrarComboPopup( '<b>Autor da Emenda</b>', 'autid',  $sql, $stSqlCarregados, 'Selecione o(s) Autor(es) da Emenda' );
			
		//Especificação
		$sql = "SELECT DISTINCT
				    e.espid as codigo,
				    e.espid|| ' - ' ||e.espnome as descricao
				FROM
				    emenda.especificacao e
				    inner join emenda.iniciativaespecificacao ie
				    	on ie.espid = e.espid
				    inner join emenda.ptiniciativaespecificacao ptie
				    	on ptie.iceid = ie.iceid
				    inner join emenda.ptiniciativa pti
				    	on pti.ptiid = ptie.ptiid
				    inner join emenda.planotrabalho ptr
				    	on ptr.ptrid = pti.ptrid
				WHERE 
				    --e.espstatus = 'A' and
				    ptie.ptestatus <> 'P'
				    and ptr.ptrexercicio = '".$_SESSION['exercicio']."'
				ORDER BY
					e.espid";
		
		$stSqlCarregados = "";
		mostrarComboPopup('<b>Especificações</b>', 'espid',  $sql, $stSqlCarregados, 'Selecione a(s) Especificações(s)' );
		
		//Iniciativas
		$sql = "SELECT DISTINCT
				    ini.iniid as codigo,
				    ini.iniid||' - '||ini.ininome as descricao
				FROM 
				    emenda.ptiniciativa pti
				    inner join emenda.iniciativa ini
				        on ini.iniid = pti.iniid
				        --and ini.inistatus = 'A'
				    inner join emenda.planotrabalho ptr
				    	on ptr.ptrid = pti.ptrid
				    /*inner join emenda.responsavel res
				    	on res.resid = ptr.resid
				WHERE
				    /*ptr.ptrstatus = 'A'
				    and ptr.ptrexercicio = '".$_SESSION['exercicio']."'
					--and ini.inistatus = 'A'
				    --and res.resstatus = 'A'
				ORDER BY
				    descricao";
		
		$stSqlCarregados = "";
		mostrarComboPopup('<b>Iniciativa</b>', 'iniid',  $sql, $stSqlCarregados, 'Selecione a(s) Iniciativas(s)' );
		
		
		//Tipo de Ensino
		$sql = "SELECT 
				  	tpeid as codigo,
				  	tpeid || '-' || tpedsc as descricao
				FROM 
				  	emenda.tipoensino
				WHERE
					tpestatus = 'A'";
		
		$stSqlCarregados = "";
		mostrarComboPopup('<b>Tipo de Ensino</b>', 'tpeid',  $sql, $stSqlCarregados, 'Selecione o(s) Tipo(s) de Ensino(s)' );
*/
	?>
	</tr>
	<tr bgcolor="#D0D0D0">
		<td colspan="2" style="text-align: center;">
			<input type="button" value="Pesquisar" onclick="geraPopRelatorio();" style="cursor: pointer;"/>
		</td>
	</tr>
</table>
</form>
</body>
<script type="text/javascript">
	function onOffCampo( campo ){
		var div_on = document.getElementById( campo + '_campo_on' );
		var div_off = document.getElementById( campo + '_campo_off' );
		var input = document.getElementById( campo + '_campo_flag' );
		
		if ( div_on.style.display == 'none' ){
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			input.value = '1';
		}else{
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			input.value = '0';
		}
	}
	function geraPopRelatorio(){
		var form = $('formulario');
		
		if (form.elements['coluna'][0] == null){
			alert('Selecione pelo menos uma coluna!');
			return false;
		}
		if($('estuf_campo_flag').value == "1"){
			selectAllOptions( form.estuf );
		}
				
		//selectAllOptions( form.agrupador );
		selectAllOptions( form.coluna );
		form.target = 'page';
		var janela = window.open('emenda.php?modulo=relatorio/popUpRelatorioGeralPTA&acao=A','page','toolbar=no,location=no,status=no,menubar=yes,scrollbars=yes,resizable=yes,fullscreen=yes');
		janela.focus();
		form.submit();
	}
</script>
</html>