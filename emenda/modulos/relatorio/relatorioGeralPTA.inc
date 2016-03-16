<?php
include APPRAIZ. '/includes/Agrupador.php';
require_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( "Relatórios Geral - PTA", '<b>Filtros de Pesquisa</b>');

function agrupador(){
	return array(
				array('codigo' => 'ptrcod',						'descricao' => 'PTA'),
				/*array('codigo' => 'autnome', 					'descricao' => 'Nome do Parlamentar'),
				array('codigo' => 'ptrnumprocessoempenho', 		'descricao' => 'Nº do Processo Empenho'),
				array('codigo' => 'estuf', 						'descricao' => 'UF da Entidade'),
				array('codigo' => 'muncod', 					'descricao' => 'Município'),
				array('codigo' => 'entidade', 					'descricao' => 'Entidade'),
				array('codigo' => 'tpeid',						'descricao' => 'Tipo de Ensino'),
				array('codigo' => 'ptivalortotal',				'descricao' => 'Valor do PTA'),
				array('codigo' => 'anadataconclusao',			'descricao' => 'Data da Análise Técnica'),
				array('codigo' => 'anasituacao',				'descricao' => 'Situação Análise Técnica'),
				array('codigo' => 'uniid',						'descricao' => 'Unidade Gestora'),
				array('codigo' => 'semid',						'descricao' => 'Empenho'),
				array('codigo' => 'emdliberado',				'descricao' => 'Limite Autorizado')*/
				);
}

function colunas(){
	return array(
				array('codigo' => 'ptrcod',					'descricao' => 'PTA'),
				array('codigo' => 'autnome', 				'descricao' => 'Nome do Parlamentar'),
				array('codigo' => 'ptrnumprocessoempenho', 	'descricao' => 'Nº do Processo Empenho'),
				array('codigo' => 'estuf', 					'descricao' => 'UF da Entidade'),
				array('codigo' => 'muncod', 				'descricao' => 'Município'),
				array('codigo' => 'entidade', 				'descricao' => 'Entidade'),
				array('codigo' => 'tpeid',					'descricao' => 'Tipo de Ensino'),
				array('codigo' => 'ininome',				'descricao' => 'Iniciativa do PTA'),
				array('codigo' => 'ptivalortotal',			'descricao' => 'Valor do PTA'),
				array('codigo' => 'anadataconclusao',		'descricao' => 'Data da Análise Técnica'),
				array('codigo' => 'anasituacao',			'descricao' => 'Situação Análise Técnica'),
				array('codigo' => 'unisigla',				'descricao' => 'Unidade Gestora'),
				array('codigo' => 'situacaoempenho',		'descricao' => 'Empenho'),
				array('codigo' => 'emdliberado',			'descricao' => 'Limite Autorizado'),
				array('codigo' => 'pmcnumconveniosiafi',	'descricao' => 'Nº do convênio SIAFI'),
				array('codigo' => 'ptrnumconvenio',			'descricao' => 'Nº do convênio FNDE'),
								
				/*array('codigo' => 'enbcnpj', 					'descricao' => 'CNPJ'),
				array('codigo' => 'parsigla', 					'descricao' => 'Sigla do Partido'),					
				array('codigo' => 'estufator', 					'descricao' => 'UF do Autor'),
				array('codigo' => 'iniidemenda',				'descricao' => 'Iniciativa do Recurso'),				
				array('codigo' => 'emecod',						'descricao' => 'Emenda'),
				array('codigo' => 'fupfuncionalprogramatica',	'descricao' => 'Funcional Programática'),
				array('codigo' => 'fupdsc',						'descricao' => 'SubTítulo'),				
				array('codigo' => 'gndcod',						'descricao' => 'GND'),				
				array('codigo' => 'foncod',						'descricao' => 'Fonte'),				
				array('codigo' => 'mapcod',						'descricao' => 'Mod'),				
				array('codigo' => 'acacod',						'descricao' => 'Ação Orçamentária'),
				array('codigo' => 'tpanome',					'descricao' => 'Tipo de Autor'),
				array('codigo' => 'responsavel',				'descricao' => 'Responsavel'),
				array('codigo' => 'valoremenda',				'descricao' => 'Valor da Emenda'),
				array('codigo' => 'valorentidade',				'descricao' => 'Valor da Entidade'),
				array('codigo' => 'usucpf',						'descricao' => 'CPF Autorizado'),
				array('codigo' => 'ininome',					'descricao' => 'Iniciativa do PTA'),
				array('codigo' => 'espid',						'descricao' => 'Especificação do PTA'),
				
				array('codigo' => 'exfvalor',					'descricao' => 'Valor Empenhado'),				
				array('codigo' => 'vigdatainicio',				'descricao' => 'Data inicio de vigência'),
				array('codigo' => 'vigdatafim',					'descricao' => 'Data fim de vigência'),
				array('codigo' => 'objeto',						'descricao' => 'Objeto da Emenda'),
				array('codigo' => 'orbvalorparcela',			'descricao' => 'Valor Pago'),
				array('codigo' => 'datareferencia',				'descricao' => 'Data de Referência'),
				array('codigo' => 'emeano',						'descricao' => 'Ano Orçamento'),*/
				);
}
?>
<html>
<head>
	<script type="text/javascript" src="/includes/prototype.js"></script>
</head>
<body>
<form action="emenda.php?modulo=relatorio/popUpRelatorioGeralPTA&acao=A" method="post" name="formulario" id="formulario">
<input type="hidden" name="limpaSession" id="limpaSession" value="true">
<table id="table" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
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
	<tr>
		<td class="SubTituloDireita" style="width: 33%" valign="top"><b>Colunas</b></td>
		<td>
			<?php
			
			$coluna = colunas();
			$campoColuna = new Agrupador( 'formulario' );
			$campoColuna->setOrigem( 'colunaOrigem', null, $coluna );
			$campoColuna->setDestino( 'coluna', null, array(
															array('codigo' => 'ptrcod',					'descricao' => 'PTA'),
															array('codigo' => 'autnome', 				'descricao' => 'Nome do Parlamentar'),
															array('codigo' => 'ptrnumprocessoempenho', 	'descricao' => 'Nº do Processo Empenho'),
															array('codigo' => 'estuf', 					'descricao' => 'UF da Entidade'),
															array('codigo' => 'muncod', 				'descricao' => 'Município'),
															array('codigo' => 'entidade', 				'descricao' => 'Entidade'),
															array('codigo' => 'tpeid',					'descricao' => 'Tipo de Ensino'),
															array('codigo' => 'ininome',				'descricao' => 'Iniciativa do PTA'),
															array('codigo' => 'ptivalortotal',			'descricao' => 'Valor do PTA'),
															array('codigo' => 'anadataconclusao',		'descricao' => 'Data da Análise Técnica'),
															array('codigo' => 'anasituacao',			'descricao' => 'Situação Análise Técnica'),
															array('codigo' => 'unisigla',				'descricao' => 'Unidade Gestora'),
															array('codigo' => 'situacaoempenho',		'descricao' => 'Empenho'),
															array('codigo' => 'emdliberado',			'descricao' => 'Limite Autorizado'),
															array('codigo' => 'pmcnumconveniosiafi',	'descricao' => 'Nº do convênio SIAFI'),
															array('codigo' => 'ptrnumconvenio',			'descricao' => 'Nº do convênio FNDE')
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
		<td class="subtitulodireita"><b>Número da Emenda:</b></td>
		<td>
			<?php 
				$emecod = $_REQUEST["emecod"];
				echo campo_texto( 'emecod', 'N', 'S', '', 12, 8, '[#]', '', 'left', '', 0, '');
			?>
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita"><b>Número do Convênio SIAFI:</b></td>
		<td>
			<?php 
				$pmcnumconveniosiafi = $_REQUEST["pmcnumconveniosiafi"];
				echo campo_texto( 'pmcnumconveniosiafi', 'N', 'S', '', 12, 8, '[#]', '', 'left', '', 0, '');
			?>
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita"><b>Número do Convênio FNDE:</b></td>
		<td>
			<?php 
				$ptrnumconvenio = $_REQUEST["ptrnumconvenio"];
				echo campo_texto( 'ptrnumconvenio', 'N', 'S', '', 12, 8, '[#]', '', 'left', '', 0, '');
			?>
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita"><b>Valor do Concedente:</b></td>
		<td>
			<?php 
				$pedvalor = $_REQUEST["pedvalor"];
				echo campo_texto( 'pedvalor', 'N', 'S', '', 30, 20, '[###.]###,##', '', 'left', '', 0, '');
			?>
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita"><b>Situação:</b></td>
		<td colspan="3">
			<?php
				$esdid = $_REQUEST['esdid'];
				$sql = "SELECT esdid as codigo, esddsc as descricao 
						FROM workflow.estadodocumento
						WHERE tpdid = 8 
						ORDER BY esdordem";
				$db->monta_combo( "esdid", $sql, 'S', 'Selecione...', '', '', '', '265' );
			?>
		</td>
	<tr>
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
		<td class="subtitulodireita"><b>Efetuado algum Pagamento:</b></td>
		<td>
			<input type="radio" value="S" id="pagamento" name="pagamento" <? if($_REQUEST["pagamento"] == "S") { echo "checked"; } ?> /> Sim
			<input type="radio" value="N" id="pagamento" name="pagamento" <? if($_REQUEST["pagamento"] == "N") { echo "checked"; } ?> /> Não
			<input type="radio" value="" id="pagamento" name="pagamento"  <? if($_REQUEST["pagamento"] == "") { echo "checked"; } ?> /> Todos
		</td>
	</tr>
	<!-- <tr>
		<td class="subtitulodireita"><b>Emendas com Plano de Trabalho:</b></td>
		<td>
			<input type="radio" value="S" id="emendapta" name="emendapta" <? if($_REQUEST["emendapta"] == "S") { echo "checked"; } ?> /> Sim
			<input type="radio" value="N" id="emendapta" name="emendapta" <? if($_REQUEST["emendapta"] == "N") { echo "checked"; } ?> /> Não
			<input type="radio" value="" id="emendapta" name="emendapta"  <? if($_REQUEST["emendapta"] == "") { echo "checked"; } ?> /> Todos
		</td>
	</tr>-->
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
	<?php 
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
				    inner join emenda.iniciativaespecificacao ie on ie.espid = e.espid
				    inner join emenda.ptiniciativaespecificacao ptie on ptie.iceid = ie.iceid
				    inner join emenda.ptiniciativa pti on pti.ptiid = ptie.ptiid
				    inner join emenda.planotrabalho ptr on ptr.ptrid = pti.ptrid
				    inner join emenda.especificacao_programacaoexercicio epe on epe.espid = e.espid and epe.prsano = '".$_SESSION['exercicio']."'
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
				    	on res.resid = ptr.resid*/
				WHERE
				    /*ptr.ptrstatus = 'A'
				    and*/ ptr.ptrexercicio = '".$_SESSION['exercicio']."'
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
		if($('enbid_campo_flag').value == "1"){
			selectAllOptions( form.enbid );
		}
		if($('autid_campo_flag').value == "1"){
			selectAllOptions( form.autid );
		}
		if($('estuf_campo_flag').value == "1"){
			selectAllOptions( form.estuf );
		}
		if($('espid_campo_flag').value == "1"){
			selectAllOptions( form.espid );
		}
		if($('iniid_campo_flag').value == "1"){
			selectAllOptions( form.iniid );
		}
		if($('tpeid_campo_flag').value == "1"){
			selectAllOptions( form.tpeid );
		}
		
		/*var j = form.coluna.options.length;
		var boEntidade = false;
		for ( var i = 0; i < j; i++ ){
			if(form.coluna.options[i].value == 'entidade' || form.coluna.options[i].value == 'valorentidade'){
				boEntidade = true;
			}
		}
		
		if(boEntidade){
			alert(boEntidade);
			if(form.entid.value != ""){
						
			}
		}*/

		selectAllOptions( form.agrupador );
		selectAllOptions( form.coluna );
		form.target = 'page';
		var janela = window.open('emenda.php?modulo=relatorio/popUpRelatorioGeralPTA&acao=A','page','toolbar=no,location=no,status=no,menubar=yes,scrollbars=yes,resizable=yes,fullscreen=yes');
		janela.focus();
		form.submit();
	}
</script>
</html>