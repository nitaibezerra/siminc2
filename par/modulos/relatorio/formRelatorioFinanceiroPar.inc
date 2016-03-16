<?php
include APPRAIZ. '/includes/Agrupador.php';
require_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( "Relatórios Financeiro do PAR", '<b>Filtros de Pesquisa</b>');

function agrupador(){
	return array(
				array('codigo' => 'ptrcod',						
					  'descricao' => 'PTA'),
				);
}

function colunas(){
	return array(
				array('codigo' => 'uf',						'descricao' => 'UF'),
				array('codigo' => 'local', 					'descricao' => 'Local'),
				array('codigo' => 'prpnumeroprocesso',		'descricao' => 'Nº do Processo'),
				array('codigo' => 'valor_subacao', 			'descricao' => 'Valor da Subação'),
				array('codigo' => 'empenho', 				'descricao' => 'Empenho'),
				array('codigo' => 'pagamento', 				'descricao' => 'Pagamento'),
				array('codigo' => 'sbdplanointerno', 		'descricao' => 'P.I.'),
				array('codigo' => 'sbdptres', 				'descricao' => 'PTRES'),
				array('codigo' => 'programa', 				'descricao' => 'Programa'),
				array('codigo' => 'codigo_subacao',			'descricao' => 'Código da Subação'),
				array('codigo' => 'statuspar', 				'descricao' => 'Status do PAR'),
				array('codigo' => 'statussubacao', 			'descricao' => 'Status da Subação'),
				array('codigo' => 'forma',		 			'descricao' => 'Forma de Execução'),
				array('codigo' => 'dimensao',	 			'descricao' => 'Dimensão'),
				array('codigo' => 'tiposubacao',			'descricao' => 'Tipo da Subação'),
				array('codigo' => 'subacao',				'descricao' => 'Descrição da Subação'),
				array('codigo' => 'qtditemcomposicao',		'descricao' => 'Quantidade de Itens'),
				array('codigo' => 'itemcomposicao',			'descricao' => 'Item de Composição'),
				array('codigo' => 'documento',				'descricao' => 'Documento')
				);
}
?>
<html>
<head>
	<script type="text/javascript" src="/includes/prototype.js"></script>
</head>
<body>
<form action="par.php?modulo=relatorio/resultRelatorioFinanceiroPar&acao=A" method="post" name="formulario" id="formulario">
<input type="hidden" name="limpaSession" id="limpaSession" value="true">
<table id="table" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td class="subtitulodireita"><b>Nível do Resultado:</b></td>
            <td>
                <input type="radio" value="processo" id="nivel_resultado" name="nivel_resultado" <?php if($_REQUEST["nivel_resultado"] == "processo" || empty($_REQUEST["nivel_resultado"])) { echo "checked"; } ?> /> Processo
                <input type="radio" value="empenho" id="nivel_resultado" name="nivel_resultado" <?php if($_REQUEST["nivel_resultado"] == "empenho") { echo "checked"; } ?> /> Empenho
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
															array('codigo' => 'uf',						'descricao' => 'UF'),
															array('codigo' => 'local', 					'descricao' => 'Local'),
															array('codigo' => 'prpnumeroprocesso',		'descricao' => 'Nº do Processo'),
															array('codigo' => 'valor_subacao', 			'descricao' => 'Valor da Subação'),
															array('codigo' => 'empenho', 				'descricao' => 'Empenho'),
															array('codigo' => 'pagamento', 				'descricao' => 'Pagamento'),
															array('codigo' => 'sbdplanointerno', 		'descricao' => 'P.I.'),
															array('codigo' => 'sbdptres', 				'descricao' => 'PTRES'),
															array('codigo' => 'programa', 				'descricao' => 'Programa'),
															array('codigo' => 'codigo_subacao',			'descricao' => 'Código da Subação'),
															array('codigo' => 'statuspar', 				'descricao' => 'Status do PAR'),
															array('codigo' => 'statussubacao', 			'descricao' => 'Status da Subação'),
															array('codigo' => 'forma',		 			'descricao' => 'Forma de Execução'),
															array('codigo' => 'dimensao',	 			'descricao' => 'Dimensão'),
															array('codigo' => 'tiposubacao',			'descricao' => 'Tipo da Subação'),
															array('codigo' => 'subacao',				'descricao' => 'Descrição da Subação'),
															array('codigo' => 'qtditemcomposicao',		'descricao' => 'Quantidade de Itens'),
															array('codigo' => 'itemcomposicao',			'descricao' => 'Item de Composição'),
															array('codigo' => 'documento',				'descricao' => 'Documento')
															));
			$campoColuna->exibir();
			?>
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita"><b>Esfera:</b></td>
		<td>
			<input type="radio" value="E" id="esfera" name="esfera" <? if($_REQUEST["esfera"] == "S") { echo "checked"; } ?> /> Estadual
			<input type="radio" value="M" id="esfera" name="esfera" <? if($_REQUEST["esfera"] == "N") { echo "checked"; } ?> /> Municipal
			<input type="radio" value="" id="esfera" name="esfera"  <? if($_REQUEST["esfera"] == "") { echo "checked"; } ?> /> Todas
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita"><b>Termo Validado:</b></td>
		<td>
			<input type="radio" value="S" id="termovalidado" name="termovalidado" <? if($_REQUEST["termovalidado"] == "S") { echo "checked"; } ?> /> Sim
			<input type="radio" value="N" id="termovalidado" name="termovalidado" <? if($_REQUEST["termovalidado"] == "N") { echo "checked"; } ?> /> Não
			<input type="radio" value="" id="termovalidado" name="termovalidado"  <? if($_REQUEST["termovalidado"] == "") { echo "checked"; } ?> /> Todos
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
		<td class="subtitulodireita"><b>Empenho Parcial:</b></td>
		<td>
			<input type="radio" value="S" id="empenhoparcial" name="empenhoparcial" <? if($_REQUEST["empenhoparcial"] == "S") { echo "checked"; } ?> /> Sim
			<input type="radio" value="N" id="empenhoparcial" name="empenhoparcial" <? if($_REQUEST["empenhoparcial"] == "N") { echo "checked"; } ?> /> Não
			<input type="radio" value="" id="empenhoparcial" name="empenhoparcial"  <? if($_REQUEST["empenhoparcial"] == "") { echo "checked"; } ?> /> Todos
		</td>
	</tr>
	<tr>
	<?php
		//Filtro dos Documentos
		$stSql = "SELECT
						mdoid as codigo, 
						mdonome as descricao 
					FROM 
						par.modelosdocumentos 
					WHERE 
						mdostatus = 'A'";
		$stSqlCarregados = "";
		mostrarComboPopup( '<b>Documentos</b>', 'documentos',  $stSql, $stSqlCarregados, 'Selecione o Documento' ); 

		//Filtro do PI da Subação
		$stSql = "SELECT DISTINCT
						plinumplanointerno as codigo,
						plinumplanointerno as descricao
					FROM
					par.planointerno";
		$stSqlCarregados = "";
		mostrarComboPopup( '<b>Plano Interno</b>', 'planointerno',  $stSql, $stSqlCarregados, 'Selecione o Plano Interno' ); 

		//Filtro do PTRES da Subação
		$stSql = "SELECT DISTINCT 
						ptres AS codigo, 
						ptres AS descricao 
					FROM 
						financeiro.empenhopar
					ORDER BY
						ptres";
		$stSqlCarregados = "";
		mostrarComboPopup( '<b>PTRES</b>', 'ptres',  $stSql, $stSqlCarregados, 'Selecione o PTRES' ); 
		
		//Filtro do Programa da Subação
		$stSql = "SELECT DISTINCT 
						prgid AS codigo, 
						prgdsc AS descricao 
					FROM 
						par.programa
					WHERE
						prgstatus = 'A'
					ORDER BY
						prgdsc";
		$stSqlCarregados = "";
		mostrarComboPopup( '<b>Programa</b>', 'programa',  $stSql, $stSqlCarregados, 'Selecione o Programa' ); 
		
		//Filtro do Status do PAR
		$stSql = "SELECT 
						esdid AS codigo, 
						esddsc AS descricao
					FROM 
						workflow.estadodocumento 
					WHERE 
						esdid IN (".WF_DIAGNOSTICO.", ".WF_ELABORACAO.", ".WF_ANALISE.")";
		$stSqlCarregados = "";
		mostrarComboPopup( '<b>Status do PAR</b>', 'statuspar',  $stSql, $stSqlCarregados, 'Selecione o(s) Status do PAR' ); 

		//Filtro do Status da Subação
		$stSql = "SELECT 
						esdid AS codigo, 
						esddsc AS descricao
					FROM 
						workflow.estadodocumento 
					WHERE 
						esdid IN (".WF_SUBACAO_ELABORACAO.", ".WF_SUBACAO_ANALISE.", ".WF_SUBACAO_DILIGENCIA.", ".WF_SUBACAO_AGUARDANDO_ANALISE_GESTOR.", ".WF_SUBACAO_EMPENHO.", ".WF_SUBACAO_PAGAMENTO.", ".WF_SUBACAO_INDEFERIDA.", ".WF_SUBACAO_ANALISE_COMISSAO.")";
		$stSqlCarregados = "";
		mostrarComboPopup( '<b>Status da Subação</b>', 'statussubacao',  $stSql, $stSqlCarregados, 'Selecione o(s) Status' ); 
	
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
		
		//Filtro de Municípios
		$stSql = "SELECT
					muncod AS codigo,
					estuf || ' - ' || mundescricao AS descricao
				  FROM
				  	territorios.municipio
				  ORDER BY
				  	estuf, mundescricao";
		$stSqlCarregados = "";
		mostrarComboPopup( '<b>Município</b>', 'municipio',  $stSql, $stSqlCarregados, 'Selecione o(s) Municípios(s)' ); 
	
		
		//Filtro de Forma de Execução
		$stSql = "SELECT
				      frmid as codigo,
				      frmdsc as descricao
				FROM
					par.formaexecucao
				ORDER BY
				    frmdsc";
		$stSqlCarregados = "";
		mostrarComboPopup( '<b>Forma de Execução</b>', 'forma',  $stSql, $stSqlCarregados, 'Selecione a(s) Forma(s) de Execução' ); 
		
		//Filtro de Dimensão
		$stSql = "SELECT
				      dimcod as codigo,
				      dimcod || ' - ' || dimdsc as descricao
				FROM
					par.dimensao
				WHERE
				      dimstatus = 'A' AND
				      itrid = '".INSTRUMENTO_DIAGNOSTICO_ESTADUAL."'
				ORDER BY
				      dimcod";
		$stSqlCarregados = "";
		mostrarComboPopup( '<b>Dimensão</b>', 'dimensao',  $stSql, $stSqlCarregados, 'Selecione a(s) Dimensão(ões)' ); 
		
		//Filtro de Tipo de Subação
		$stSql = "SELECT 
					ptsid as codigo,
					frmdsc || ' - ' || tps.tpsdescricao as descricao
				FROM 
					par.propostatiposubacao pts
				INNER JOIN par.formaexecucao frm ON frm.frmid = pts.frmid
				INNER JOIN  par.tiposubacao tps on tps.tpsid = pts.tpsid
				WHERE 
					ptsstatus = 'A'
				ORDER BY
					descricao";
		$stSqlCarregados = "";
		mostrarComboPopup( '<b>Tipo de Subação</b>', 'tiposubacao',  $stSql, $stSqlCarregados, 'Selecione o(s) Tipo(s) de Subação(ões)');
		
		//Filtro de Subação
		$stSql = "SELECT 
						ppsid as codigo,
						d.dimcod || '.' || a.arecod || '.' || i.indcod || '.' || pps.ppsordem || ' - ' || pps.ppsdsc as descricao
					FROM par.dimensao d
					INNER JOIN par.area a ON a.dimid = d.dimid
					INNER JOIN par.indicador i ON i.areid = a.areid
					INNER JOIN par.propostasubacao pps ON pps.indid = i.indid
					ORDER BY
						descricao";
		$stSqlCarregados = "";
		mostrarComboPopup( '<b>Subação</b>', 'subacao',  $stSql, $stSqlCarregados, 'Selecione a(s) Subação(ões)');
		
		//Filtro de Grupo de Municípios
		$stSql = "select * from (
					(select 
						tpmid as codigo,
						case when tpmid = 140 then 'Municípios de até 10.000 habitantes' when tpmid = 141 then 'Municípios de 10.001 a 20.000 habitantes' else tpmdsc end as descricao
					from  
						territorios.tipomunicipio
					where 
						tpmid IN (1, 16, 17, 139, 140, 141, 150, 151, 152, 154, 162, 167)
					)								
					union
					(select 
						gtmid as codigo,
						gtmdsc as descricao 
					from  
						territorios.grupotipomunicipio
					where 
						gtmid = 5)
					order by descricao) as tbl";
			$stSqlCarregados = "";

		mostrarComboPopup( '<b>Grupo de Municípios</b>', 'grupo',  $stSql, $stSqlCarregados, 'Selecione o(s) Grupo(s) de Município(s)', $where);
		
		//Filtro de Itens de Composição
		$stSql = "SELECT 
					picid as codigo,
					picdescricao as descricao 
				FROM 
					par.propostaitemcomposicao
				WHERE
					picstatus = 'A'
				ORDER BY
					picdescricao";
		$stSqlCarregados = "";
		mostrarComboPopup( '<b>Itens de Composição</b>', 'itemcomposicao',  $stSql, $stSqlCarregados, 'Selecione o(s) Item(ns) de Composição');
	?>
	</tr>
	<tr>
	<?php 
		// FILTROS
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
		selectAllOptions( form.estuf	   	 );
		selectAllOptions( form.municipio 	 );
		selectAllOptions( form.programa		 );
		selectAllOptions( form.statussubacao );
		selectAllOptions( form.statuspar 	 );
		selectAllOptions( form.documentos	 );
		selectAllOptions( form.planointerno  );
		selectAllOptions( form.ptres		 );
		selectAllOptions( form.forma 	   	 );
		selectAllOptions( form.dimensao  	 );
		selectAllOptions( form.tiposubacao   );
		selectAllOptions( form.subacao 		 );
		selectAllOptions( form.grupo  		 );
		selectAllOptions( form.itemcomposicao);
		form.target = 'page';
		var janela = window.open('par.php?modulo=relatorio/resultRelatorioFinanceiroPar&acao=A','page','toolbar=no,location=no,status=no,menubar=yes,scrollbars=yes,resizable=yes,fullscreen=yes');
		janela.focus();
		form.submit();
	}
</script>
</html>