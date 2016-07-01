<?php
//verifica_sessao();

if ( isset( $_REQUEST['buscar'] ) )
{
	include "monitoramento_tecnico_resultado.inc";
	exit();
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
monta_titulo( "Relatório de Monitoramento Técnico", "" );
//monta_titulo_cte( $titulo_modulo );

?>
<script type="text/javascript">

	function exibirRelatorio()
	{
		var formulario = document.filtro;
		
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
		selectAllOptions( document.getElementById( 'muncod' ) );
		selectAllOptions( document.getElementById( 'frmid' ) );
		selectAllOptions( document.getElementById( 'perid' ) );
		selectAllOptions( document.getElementById( 'estid' ) );
		selectAllOptions( document.getElementById( 'trscod' ) );
		selectAllOptions( document.getElementById( 'dimid' ) );
		selectAllOptions( document.getElementById( 'ardid' ) );
		selectAllOptions( document.getElementById( 'indid' ) );
		
		// submete formulario
		formulario.target = 'resultadoCteGeral';
		var janela = window.open( '', 'resultadoCteGeral', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		formulario.submit();
		janela.focus();
	}

</script>
<form action="" method="post" name="filtro">
	<input type="hidden" name="buscar" value="1"/>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">

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
						'regiao' => array(
							'codigo'    => 'regiao',
							'descricao' => 'Região'
						),
						'estado' => array(
							'codigo'    => 'estado',
							'descricao' => 'Estado'
						),
						'municipio' => array(
							'codigo'    => 'municipio',
							'descricao' => 'Município'
						),
						'dimensao' => array(
							'codigo'    => 'dimensao',
							'descricao' => 'Dimensão'
						),
						'area' => array(
							'codigo'    => 'area',
							'descricao' => 'Área'
						),
						'indicador' => array(
							'codigo'    => 'indicador',
							'descricao' => 'Indicador'
						),
						'subacao' => array(
							'codigo'    => 'subacao',
							'descricao' => 'Subação'
						),
						'periodo' => array(
							'codigo'    => 'periodo',
							'descricao' => 'Período'
						),
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
				$sqlComboEstado = "
					select
						estuf as codigo,
						estdescricao as descricao
					from territorios.estado
					order by
						estdescricao
				";
				combo_popup( "estuf", $sqlComboEstado, "Estado", "400x400", 0, array(), "", "S", false, false, 5, 400 );
				?>
			</td>
		</tr>
		
		<!-- MUNICIPIO -------------------------------------------------------- -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Municípios
			</td>
			<td>
				<?php
				//verificar se possui acesso a município restrito
				if(!cte_possuiPerfilSemVinculo()) {	
					$codMunicipios = cte_pegarMunicipiosPermitidos();	
					if(count($codMunicipios) > 0) {
						$codMunicipios = implode(',', $codMunicipios);	
						$wh = " and m.muncod in ('"	. $codMunicipios . "') ";		
					}
				}
				else {
					$wh = '';
				}
			
				$sqlComboMunicipio = "
					select
						m.muncod as codigo,
						m.estuf || ' - ' || m.mundescricao as descricao
					from territorios.municipio m
						inner join cte.instrumentounidade i on
							i.muncod = m.muncod and
							i.mun_estuf = m.estuf
						inner join cte.pontuacao p on
							p.inuid = i.inuid
						inner join cte.acaoindicador a on
							a.ptoid = p.ptoid
						inner join cte.subacaoindicador s on
							s.aciid = a.aciid
					where
						i.itrid = " . INSTRUMENTO_DIAGNOSTICO_MUNICIPAL . "
						$wh 
					group by
						m.muncod,
						m.estuf,
						m.mundescricao
					order by
						m.estuf,
						m.mundescricao
				";
				//dbg($sqlComboMunicipio, 1);					
				combo_popup( "muncod", $sqlComboMunicipio, "Município", "400x400", 0, array(), "", "S", false, false, 5, 400 );
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
						itrid = '" . INSTRUMENTO_DIAGNOSTICO_MUNICIPAL . "'
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
						d.itrid = '" . INSTRUMENTO_DIAGNOSTICO_MUNICIPAL . "'
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
						d.itrid = '" . INSTRUMENTO_DIAGNOSTICO_MUNICIPAL . "'
					order by
						d.dimcod,
						a.ardcod,
						i.indcod
				";
				combo_popup( "indid", $sqlComboIndicador, "Indicadores", "400x400", 0, array(), "", "S", false, false, 5, 400 );
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
		
		<!-- PERÍODO DE REFERÊNCIA --------------------------------------------- -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Período de Referência
			</td>
			<td>
				<?php
				
				$sqlPeriodoReferencia = "select perid as codigo, perdsc as descricao
										 from cte.periodoreferencia
										 order by perdsc";
				
				combo_popup( "perid", $sqlPeriodoReferencia, "Período de Referência", "400x400", 0, array(), "", "S", false, false, 5, 400 );
				?>
			</td>
		</tr>
		
		<!-- ESTADO DA EXECUÇÃO --------------------------------------------- -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Estado da Execução
			</td>
			<td>
				<?php
				
				$sqlEstadoExecucao = "select estid as codigo, estdsc as descricao
									  from cte.estadosubacao
									  order by estdsc";
				
				combo_popup( "estid", $sqlEstadoExecucao, "Estado da Execução", "400x400", 0, array(), "", "S", false, false, 5, 400 );
				?>
			</td>
		</tr>
		
		<!-- TIPOS DE RESTRIÇÃO --------------------------------------------- -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Tipos de Restrição
			</td>
			<td>
				<?php
				
				$sqlTipoRestricao = "select trscod as codigo, trsdsc as descricao
									 from public.tiporestricao
									 order by trsdsc";
				
				combo_popup( "trscod", $sqlTipoRestricao, "Tipos de Restrição", "400x400", 0, array(), "", "S", false, false, 5, 400 );
				?>
			</td>
		</tr>
		
		<tr>
			<td class="SubTituloDireita" valign="top">
				&nbsp;
			</td>
			<td class="SubTituloDireita" style="text-align:left;">
				<input type="button" name="filtrar" value="Visualizar" onclick="exibirRelatorio();" />
			</td>
		</tr>
		
	</table>
</form>