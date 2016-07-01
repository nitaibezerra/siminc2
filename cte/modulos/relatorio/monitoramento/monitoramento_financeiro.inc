<?php
//verifica_sessao();

if ( isset( $_REQUEST['buscar'] ) )
{
	include "monitoramento_financeiro_resultado.inc";
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

	function toggleFiltroMunicipio(){
		document.getElementById('filtroMunicipio').style.display = document.getElementById('municipal').checked ? '' : 'none';
	}

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
		
		if ( !document.getElementById('anoreferencia').value )
		{
			alert( 'O campo "Ano do Convênio" é de preenchimento obrigatório!' );
			return;
		}
		
		if ( !document.getElementById('mesano').value )
		{
			alert( 'O campo "Mês/Ano do Monitoramento" é de preenchimento obrigatório!' );
			return;
		}
		
		selectAllOptions( agrupador );
//		selectAllOptions( document.getElementById( 'regcod' ) );
		selectAllOptions( document.getElementById( 'estuf' ) );
		selectAllOptions( document.getElementById( 'muncod' ) );
		selectAllOptions( document.getElementById( 'scsid' ) );
		
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
//						'regiao' => array(
//							'codigo'    => 'regiao',
//							'descricao' => 'Região'
//						),
						'estado' => array(
							'codigo'    => 'estado',
							'descricao' => 'Estado'
						),
						'municipio' => array(
							'codigo'    => 'municipio',
							'descricao' => 'Município'
						),
						'convenio' => array(
							'codigo'    => 'convenio',
							'descricao' => 'Convênio'
						),
						'subacao' => array(
							'codigo'    => 'subacao',
							'descricao' => 'Subação'
						),
						'item' => array(
							'codigo'    => 'item',
							'descricao' => 'Item de Composição'
						),
					);
					
					// exibe agrupador
					$agrupador->setOrigem( 'naoAgrupador', null, $origem );
					$agrupador->setDestino( 'agrupador', null, $destino );
					$agrupador->exibir();
					
				?>
			</td>
		</tr>
		
		<!-- DEMANDA ------------------------------------------------------- -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Demanda da ação
			</td>
			<td>
				<input type="radio" onclick="toggleFiltroMunicipio();" checked="checked" name="tiporelatorio" id="estadual" value="E"/>
				<label for="estadual">Estadual</label>
				&nbsp;&nbsp;&nbsp;
				<input type="radio" onclick="toggleFiltroMunicipio();" name="tiporelatorio" id="municipal" value="M"/>
				<label for="municipal">Municipal</label>
			</td>
		</tr>		
		
		<!-- ANO DO CONVÊNIO ----------------------------------------------- -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Ano do Convênio:
			</td>
			<td>
				<select name="anoreferencia" id="anoreferencia">
						<option value="">selecione</option>
		  				<option value="2007">2007</option>
		  				<option value="2008">2008</option>
		  				<option value="2009">2009</option>
		  				<option value="2010">2010</option>
		  				<option value="2011">2011</option>
					</select> <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
		</tr>	
		
		<!-- ANO DO CONVÊNIO ----------------------------------------------- -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Mês/Ano do Monitoramento (MM/YYYY):
			</td>
			<td>
				<?php echo campo_texto('mesano', "S", "S", '', 10, 25 , '##/####', '', 'left', '', 0, 'id="mesano"');?>
			</td>
		</tr>	
		
		<!-- REGIAO -------------------------------------------------------- -->
		<!--<tr>
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
		
		--><!-- ESTADO -------------------------------------------------------- -->
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
		<tr id="filtroMunicipio" style="display: none;">
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
		
		<!-- ESTADO DO ITEM DE COMPOSIÇÃO --------------------------------------------- -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Estado do Item de Composição
			</td>
			<td>
				<?php
				
				$sqlEstadoItem = "select scsid as codigo, scsdesc as descricao
								  from cte.statuscomposicaosubacao
								  order by scsdesc";
				
				combo_popup( "scsid", $sqlEstadoItem, "Estado do Item de Composição", "400x400", 0, array(), "", "S", false, false, 5, 400 );
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