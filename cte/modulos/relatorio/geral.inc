<?php

//verifica_sessao();

	
	//$_SESSION['indice_sessao_combo_popup']['undid'];
	//dbg($_SESSION['indice_sessao_combo_popup']['undid']);
if ( isset( $_REQUEST['buscar'] ) )
{
	include "geral_resultado.inc";
	exit();
}

//Capturando via Request parametro passado pelo AJAX
if( $_REQUEST["req"] == 'atualizarUnidadeMedida' ){

	$acilocalizador = $_REQUEST["acilocalizador"] ? " where undtipo in ( ". stripslashes( $_REQUEST["acilocalizador"] ) ." ) " : "";
	
	$sql = "select undid as codigo, unddsc as descricao
			from cte.unidademedida
			$acilocalizador
			order by unddsc";

			
	$_SESSION['indice_sessao_combo_popup']['undid']['sql'] = $sql;

	die;
	
}

$agrupadorHtml =
<<<EOF
	<table>
		<tr valign="middle">
			<td>
				<select id="{NOME_ORIGEM}" name="{NOME_ORIGEM}[]" multiple="multiple" size="4" style="width: 160px; height: 120px;" onDblClick="moveSelectedOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );" class="combo campoEstilo"></select>
			</td>
			<td>
				<img src="../imagens/rarrow_one.gif" style="padding: 5px" onClick="moveSelectedOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );"/><br/>
				<!--
				<img src="../imagens/rarrow_all.gif" style="padding: 5px" onClick="moveAllOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );"/><br/>
				<img src="../imagens/larrow_all.gif" style="padding: 5px" onClick="moveAllOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, ''); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );"/><br/>
				-->
				<img src="../imagens/larrow_one.gif" style="padding: 5px" onClick="moveSelectedOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, '' ); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );"/><br/>
			</td>
			<td>
				<select id="{NOME_DESTINO}" name="{NOME_DESTINO}[]" multiple="multiple" size="4" style="width: 160px; height: 120px;" onDblClick="moveSelectedOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, '' ); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );" class="combo campoEstilo"></select>
			</td>
			<td>
				<img src="../imagens/uarrow.gif" style="padding: 5px" onClick="subir( document.getElementById( '{NOME_DESTINO}' ) );"/><br/>
				<img src="../imagens/darrow.gif" style="padding: 5px" onClick="descer( document.getElementById( '{NOME_DESTINO}' ) );"/><br/>
			</td>
		</tr>
	</table>
	<script type="text/javascript" language="javascript">
		limitarQuantidade( document.getElementById( '{NOME_DESTINO}' ), {QUANTIDADE_DESTINO} );
		limitarQuantidade( document.getElementById( '{NOME_ORIGEM}' ), {QUANTIDADE_ORIGEM} );
		{POVOAR_ORIGEM}
		{POVOAR_DESTINO}
		sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );
	</script>
EOF;

include APPRAIZ . 'includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( "Relatório Geral", "" );
//monta_titulo_cte( $titulo_modulo );

?>

<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript">

	function exibirRelatorio()
	{
		var formulario = document.filtro;
		
		// verifica se fisico e/ou financeiro esta selecionado
		var fisico = document.getElementById( 'fisico' ).checked;
		var financeiro = document.getElementById( 'financeiro' ).checked;
		if ( !fisico && !financeiro )
		{
			alert( 'Escolha ao menos um dos tipos "Físico" e "Financeiro".' );
			return;
		}
		
		// verifica se solicitado e/ou atendido esta selecionado
		var solicitado = document.getElementById( 'solicitado' ).checked;
		var atendido = document.getElementById( 'atendido' ).checked;
		if ( !solicitado && !atendido )
		{
			alert( 'Escolha ao menos um dos tipos "Solicitado" e "Atendido".' );
			return;
		}
		
		// verifica se municipal e/ou estadual esta selecionado
		var municipal = document.getElementById( 'municipal' ).checked;
		var estadual = document.getElementById( 'estadual' ).checked;
		if ( !municipal && !estadual )
		{
			alert( 'Escolha ao menos uma das demandas "Municipal" e "Estadual".' );
			return;
		}
		
		// verifica se tem algum agrupador selecionado
		agrupador = document.getElementById( 'agrupador' );
		if ( !agrupador.options.length )
		{
			alert( 'Escolha ao menos um item para agrupar o resultado.' );
			return;
		}
		
		selectAllOptions( agrupador );
		selectAllOptions( document.getElementById( 'regcod' ) );
		selectAllOptions( document.getElementById( 'estuf' ) );
		selectAllOptions( document.getElementById( 'dimid' ) );
		selectAllOptions( document.getElementById( 'ardid' ) );
		selectAllOptions( document.getElementById( 'indid' ) );
		selectAllOptions( document.getElementById( 'undid' ) );
		selectAllOptions( document.getElementById( 'plicod' ) );
		selectAllOptions( document.getElementById( 'prgid' ) );
		selectAllOptions( document.getElementById( 'aciparecer' ) );
		selectAllOptions( document.getElementById( 'psuid' ) );
		selectAllOptions( document.getElementById( 'gpsid' ) );
		selectAllOptions( document.getElementById( 'ssuid' ) );
		selectAllOptions( document.getElementById( 'frmid' ) );
		selectAllOptions( document.getElementById( 'foaid' ) );
		
		// submete formulario
		//alert( 'blz!' ); return;
		formulario.target = 'resultadoCteGeral';
		var janela = window.open( '', 'resultadoCteGeral', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		formulario.submit();
		janela.focus();
	}
	
	//função para atualizar Unidade de Medida
	function atualizarUnidadeMedida(){
	
		var acilocalizador = document.getElementsByName( 'acilocalizador[]' );
		
		var stAcilocalizador = "";
		for( i=0; i< acilocalizador.length; i++ ){
			if(	acilocalizador[i].checked ){
				stAcilocalizador += "'" + acilocalizador[i].value + "',";
			}	
		}
		
		stAcilocalizador = stAcilocalizador.substr( 0, stAcilocalizador.lastIndexOf( "," ) );
		 
		return new Ajax.Request( window.location.href,
	                                {
	                                    method: 'post',
	                                    parameters: '&req=atualizarUnidadeMedida&acilocalizador=' + stAcilocalizador,
	                                });
		
	}

</script>
<form action="" method="post" name="filtro">
	<input type="hidden" name="buscar" value="1"/>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		
		<!-- VALOR --------------------------------------------------------- -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Valor
			</td>
			<td>
				<input type="checkbox" name="fisico" id="fisico" value="1"/>
				<label for="fisico">Físico</label>
				&nbsp;&nbsp;&nbsp;
				<input type="checkbox" checked="checked" name="financeiro" id="financeiro" value="1"/>
				<label for="financeiro">Financeiro</label>
				<br/>
				<input type="checkbox" checked="checked" name="solicitado" id="solicitado" value="1"/>
				<label for="solicitado">Original</label>
				&nbsp;&nbsp;&nbsp;
				<input type="checkbox" name="atendido" id="atendido" value="1"/>
				<label for="atendido">Atual</label>
			</td>
		</tr>
		
		<!-- DEMANDA ------------------------------------------------------- -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Demanda da ação
			</td>
			<td>
				<input type="checkbox" onclick="atualizarUnidadeMedida();" checked="checked" name="acilocalizador[]" id="municipal" value="M"/>
				<label for="municipal">Municipal</label>
				&nbsp;&nbsp;&nbsp;
				<input type="checkbox" onclick="atualizarUnidadeMedida();" checked="checked" name="acilocalizador[]" id="estadual" value="E"/>
				<label for="estadual">Estadual</label>
			</td>
		</tr>
		
		<!-- AGRUPADOR ----------------------------------------------------- -->
		<tr>
			<td class="SubTituloDireita" valign="top" width="20%">
				Agrupadores
			</td>
			<td width="80%">
				<?php
					
					// inicia agrupador
					$agrupador = new Agrupador( 'filtro', $agrupadorHtml );
					$destino = array();
					$origem = array(
						'dimensao' => array(
							'codigo'    => 'dimensao',
							'descricao' => 'Dimensão'
						),
						'area' => array(
							'codigo'    => 'area',
							'descricao' => 'Area'
						),
						'indicador' => array(
							'codigo'    => 'indicador',
							'descricao' => 'Indicador'
						),
						'estado' => array(
							'codigo'    => 'estado',
							'descricao' => 'Estado'
						),
						'pais' => array(
							'codigo'    => 'pais',
							'descricao' => 'País'
						),
						'acao' => array(
							'codigo'    => 'acao',
							'descricao' => 'Ação'
						),
						'subacao' => array(
							'codigo'    => 'subacao',
							'descricao' => 'Sub-ação'
						),
						'unidade' => array(
							'codigo'    => 'unidade',
							'descricao' => 'Unidade de Medida'
						),
						'execucao' => array(
							'codigo'    => 'execucao',
							'descricao' => 'Forma de Execução'
						),
						'planointerno' => array(
							'codigo'    => 'planointerno',
							'descricao' => 'Plano Interno'
						),
						'programa' => array(
							'codigo'    => 'programa',
							'descricao' => 'Programa'
						),
						'demanda' => array(
							'codigo'    => 'demanda',
							'descricao' => 'Demanda'
						),
						'regiao' => array(
							'codigo'    => 'regiao',
							'descricao' => 'Região'
						),
						'tipoparecersubacao' => array(
							'codigo'    => 'tipoparecersubacao',
							'descricao' => 'Tipo Análise Subação'
						),
						'grupoparecersubacao' => array(
							'codigo'    => 'grupoparecersubacao',
							'descricao' => 'Grupo Análise Subação'
						),
						'statussubacao' => array(
							'codigo'    => 'statussubacao',
							'descricao' => 'Status Subação'
						)
					);
					
					// exibe agrupador
					$agrupador->setOrigem( 'naoAgrupador', null, $origem );
					$agrupador->setDestino( 'agrupador', null, $destino );
					$agrupador->exibir();
					
				?>
			</td>
		</tr>
		
		<!-- REGIAO -------------------------------------------------------- -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Regiões
			</td>
			<td>
				<?php
				$sqlComboRegiao = "
					select
						regcod as codigo,
						regdescricao as descricao
					from territorios.regiao
					order by
						regdescricao
				";
				combo_popup( "regcod", $sqlComboRegiao, "Regiões", "192x400", 0, array(), "", "S", false, false, 5, 400 );
				?>
			</td>
		</tr>
		
		<!-- ESTADO -------------------------------------------------------- -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Unidades da Federação
			</td>
			<td>
				<?php
				//verificar se possui acesso a estado restrito
				if(!cte_possuiPerfilSemVinculo()) {	
					$codEstados = cte_pegarUfsPermitidas();	
					if(count($codEstados) > 0) {
						$codEstados = implode(',', $codEstados);	
						$wh = "where estuf in ('"	. $codEstados . "') ";		
					}
				}
				else {
					$wh = '';
				}
				
				$sqlComboEstado = "
					select
						estuf as codigo,
						estdescricao as descricao
					from territorios.estado
					$wh
					order by
						estdescricao
				";
				combo_popup( "estuf", $sqlComboEstado, "Estado", "400x400", 0, array(), "", "S", false, false, 5, 400 );
				?>
			</td>
		</tr>
		
		<!-- DIMENSAO ------------------------------------------------------ -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Dimensões
			</td>
			<td>
				<?php
				$sqlComboDimencao = "
					select
						dimid as codigo,
						dimcod || ' - ' || dimdsc as descricao
					from cte.dimensao
					where
						dimstatus = 'A' and
						itrid = '" . INSTRUMENTO_DIAGNOSTICO_ESTADUAL . "'
					order by
						dimcod
				";
				combo_popup( "dimid", $sqlComboDimencao, "Dimensões", "175x400", 0, array(), "", "S", false, false, 5, 400 );
				?>
			</td>
		</tr>
		
		<!-- AREA ---------------------------------------------------------- -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Áreas
			</td>
			<td>
				<?php
				$sqlComboArea = "
					select
						a.ardid as codigo,
						d.dimcod || '.' || a.ardcod || ' - ' || substr( a.arddsc, 0, 95 ) || '...' as descricao
					from cte.areadimensao a
						inner join cte.dimensao d on d.dimid = a.dimid
					where
						a.ardstatus = 'A' and
						d.itrid = '" . INSTRUMENTO_DIAGNOSTICO_ESTADUAL . "'
					order by
						d.dimcod,
						a.ardcod
				";
				combo_popup( "ardid", $sqlComboArea, "Áreas", "400x400", 0, array(), "", "S", false, false, 5, 400 );
				?>
			</td>
		</tr>
		
		<!-- INDICADOR ----------------------------------------------------- -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Indicadores
			</td>
			<td>
				<?php
				$sqlComboIndicador = "
					select
						i.indid as codigo,
						d.dimcod || '.' || a.ardcod || '.' || i.indcod || ' - ' || substr( i.inddsc, 0, 95 ) || '...' as descricao
					from cte.indicador i
						inner join cte.areadimensao a on a.ardid = i.ardid
						inner join cte.dimensao d on d.dimid = a.dimid
					where
						i.indstatus = 'A' and
						d.itrid = '" . INSTRUMENTO_DIAGNOSTICO_ESTADUAL . "'
					order by
						d.dimcod,
						a.ardcod,
						i.indcod
				";
				combo_popup( "indid", $sqlComboIndicador, "Indicadores", "400x400", 0, array(), "", "S", false, false, 5, 400 );
				?>
			</td>
		</tr>
		
		<!-- UNIDADE DE MEDIDA --------------------------------------------- -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Unidades de Medida
			</td>
			<td>
				<?php
						$sqlComboUnidade = "
							select
								undid as codigo,
								unddsc as descricao
							from cte.unidademedida
							order by
								unddsc
						";
						
			combo_popup( "undid", $sqlComboUnidade, "Unidades de Medida", "400x400", 0, array(), "", "S", false, false, 5, 400 );
			?>
			</td>
		</tr>
		
		<!-- PLANO INTERNO ------------------------------------------------------ -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Plano Interno
			</td>
			<td>
				<?php
				$sqlComboPlanoInterno = "
					select
						plicod as codigo,
						plicod || ' - ' ||plidsc as descricao
					from cte.programa p
						inner join financeiro.planointerno pi on pi.plicod = p.prgplanointerno
					order by prgplanointerno
				";
				combo_popup( "plicod", $sqlComboPlanoInterno, "Plano Interno", "400x400", 0, array(), "", "S", false, false, 5, 400 );
				?>
			</td>
		</tr>		
		
		<!-- PROGRAMA ------------------------------------------------------ -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Programas
			</td>
			<td>
				<?php
				$sqlComboPrograma = "
					select
						prgid as codigo,
						prgdsc as descricao
					from cte.programa					
					order by
						prgdsc
				";
				combo_popup( "prgid", $sqlComboPrograma, "Programas", "400x400", 0, array(), "", "S", false, false, 5, 400 );
				?>
			</td>
		</tr>
		
		<!-- PARECER ACAO -------------------------------------------------- -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Análise Ação
			</td>
			<td>
				<?php
				$sqlComboParecerAcao = "
					select
						aciparecer as codigo,
						substr( aciparecer, 0, 95 ) as descricao
					from cte.acaoindicador
					order by
						aciparecer
				";
				combo_popup( "aciparecer", $sqlComboParecerAcao, "Parecer Ação", "400x400", 0, array(), "", "S", false, false, 5, 400 );
				?>
			</td>
		</tr>
		
		<!-- PARECER SUBACAO ----------------------------------------------- -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Tipo Análise Subação
			</td>
			<td>
				<?php
				$sqlComboTipoParecerSubacao = "
					select
						psuid as codigo,
						psudescricao as descricao
					from cte.parecersubacao
					order by
						psudescricao
				";
				combo_popup( "psuid", $sqlComboTipoParecerSubacao, "Tipo Parecer Subação", "335x400", 0, array(), "", "S", false, false, 5, 400 );
				?>
			</td>
		</tr>
		
		<!-- GRUPO PARECER SUBACAO ----------------------------------------- -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Grupo Análise Subação
			</td>
			<td>
				<?php
				$sqlComboGrupoParecerSubacao = "
					select
						gpsid as codigo,
						gpsdescricao as descricao
					from cte.grupoparecersubacao
					order by
						gpsdescricao
				";
				combo_popup( "gpsid", $sqlComboGrupoParecerSubacao, "Grupo Parecer Subação", "143x400", 0, array(), "", "S", false, false, 5, 400 );
				?>
			</td>
		</tr>
		
		<!-- STATUS SUBACAO ------------------------------------------------ -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Status Subação
			</td>
			<td>
				<?php
				$sqlComboStatusSubacao = "
					select
						ssuid as codigo,
						ssudescricao as descricao
					from cte.statussubacao
					order by
						ssudescricao
				";
				combo_popup( "ssuid", $sqlComboStatusSubacao, "Status Subação", "143x400", 0, array(), "", "S", false, false, 5, 400 );
				?>
			</td>
		</tr>
		
		<!-- FORMA EXECUCAO ------------------------------------------------ -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Forma de Execução
			</td>
			<td>
				<?php
				$sqlComboFormaExecucao = "
					select
						frmid as codigo,
						frmdsc as descricao
					from cte.formaexecucao
					where
						frmbrasilpro = false and
						frmtipo		 = 'E'
					order by
						frmdsc
				";
				combo_popup( "frmid", $sqlComboFormaExecucao, "Forma de Execução", "210x400", 0, array(), "", "S", false, false, 5, 400 );
				?>
			</td>
		</tr>
		
		<!-- FORMA ATENDIMENTO --------------------------------------------- -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Forma de Atendimento
			</td>
			<td>
				<?php
				$sqlComboFormaAtendimento = "
					select
						foaid as codigo,
						foadsc as descricao
					from cte.formaatendimento
					order by
						foadsc
				";
				combo_popup( "foaid", $sqlComboFormaAtendimento, "Forma de Atendimento", "160x400", 0, array(), "", "S", false, false, 5, 400 );
				?>
			</td>
		</tr>
		
		<tr>
			<td class="SubTituloDireita" valign="top">
				&nbsp;
			</td>
			<td class="SubTituloDireita" style="text-align:left;">
				<input
					type="button"
					name="filtrar"
					value="Visualizar"
					onclick="exibirRelatorio();"
				/>
			</td>
		</tr>
		
	</table>
</form>