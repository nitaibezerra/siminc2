<?php

if ( isset( $_REQUEST['buscar'] ) ){
	/* configurações do relatorio - Memoria limite de 1024 Mbytes */
	ini_set("memory_limit", "1024M");
	set_time_limit(0);
	/* FIM configurações - Memoria limite de 1024 Mbytes */
	
	include "geral_escola_ativa_resultado.inc";
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
monta_titulo( "Relatório de Escola Ativa", "" );

?>

<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript">

	function exibirRelatorio(tipoxls)
	{
		var formulario = document.filtro;

		formulario.tipovisualizacao.value = tipoxls;
		
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
		selectAllOptions( document.getElementById( 'esdid' ) );
		selectAllOptions( document.getElementById( 'ano' ) );

		
		
		// submete formulario
		if(tipoxls=='xls'){
			formulario.submit();
		}else{
			formulario.target = 'resultadoCteGeral';
			var janela = window.open( '', 'resultadoCteGeral', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
			formulario.submit();
			janela.focus();
		}
	}
	
	function atualizarDemanda( tipo ){
	
		var linhaMunicipio = document.getElementById( 'linhaMunicipio' ); 
	
		if( tipo == 'M' ){
			linhaMunicipio.style.display = '';
		}
		else{
			linhaMunicipio.style.display = 'none';
		}
	
	}
	
	function atualizarTipoRel(tipo) {
		window.location = '?modulo=relatorio/geral_escola_ativa&acao=A&tiporel='+tipo;
	}
	
</script>
<form action="" method="post" name="filtro">
	<input type="hidden" name="buscar" value="1"/>
	<input type="hidden" name="tipovisualizacao" value=""/>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<!-- TIPO DE RELATORIO ------------------------------------------------------- -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Tipo de Relatório
			</td>
			<td>
			<?
			if(!$_REQUEST['tiporel']) {
				$_REQUEST['tiporel'] = "relatorioescolas";
			}
			$tiporel = $_REQUEST['tiporel'];
			
			$tiposrel = array(0 => array("codigo" => "relatorioescolas", "descricao" => "Relatório de Escolas"), 
							 1 => array("codigo" => "relatoriodistribuicaomaterial", "descricao" => "Relatório para Distribuição do Material")
							 );
			$db->monta_combo('tiporel', $tiposrel, 'S', '', 'atualizarTipoRel', '', '', '', 'N', 'tiporel');
			?>
			</td>
		</tr>		
		<!-- DEMANDA ------------------------------------------------------- -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Demanda da ação
			</td>
			<td>
				<input type="radio" onclick="atualizarDemanda( this.value )" checked="checked" name="demanda" id="municipal" value="M"/>
				<label for="municipal">Municipal</label>
				&nbsp;&nbsp;&nbsp;
				<input type="radio" onclick="atualizarDemanda( this.value )" name="demanda" id="estadual" value="E"/>
				<label for="estadual">Estadual</label>
			</td>
		</tr>		
		
		<!--  -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Categoria
			</td>
			<td>
				<label><input type="checkbox" name="capitais" value="1" class="normal" style="margin-top: -1px;" <?= $_REQUEST['capitais'] == 1 ? 'checked="checked"' : '' ?>/> Capitais</label>
				<label><input type="checkbox" name="grandesCidades" value="1" class="normal" style="margin-top: -1px;" <?= $_REQUEST['grandescidades'] == 1 ? 'checked="checked"' : '' ?>/> Grandes Cidades</label>
				<label><input type="checkbox" name="indigena" value="1" class="normal" style="margin-top: -1px;" <?= $_REQUEST['indigena'] == 1 ? 'checked="checked"' : '' ?>/> Comunidade Indígena</label>
				<label><input type="checkbox" name="quilombola" value="1" class="normal" style="margin-top: -1px;" <?= $_REQUEST['quilombola'] == 1 ? 'checked="checked"' : '' ?>/> Comunidade Quilombola</label>
				<label><input type="checkbox" name="cidadania" value="1" class="normal" style="margin-top: -1px;" <?= $_REQUEST['cidadania'] == 1 ? 'checked="checked"' : '' ?>/> Territórios da Cidadania</label>
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
					
					switch($tiporel) {
						
						case 'relatoriodistribuicaomaterial':
							$origem = array(
								'mundescricao' => array(
									'codigo'    => 'mundescricao',
									'descricao' => 'Município'
								),
								'entnomesec' => array(
									'codigo'    => 'entnomesec',
									'descricao' => 'Secretaria'
								),
								'esddsc' => array(
									'codigo'    => 'esddsc',
									'descricao' => 'Situação'
								)
							);
							break;
						
						
						
						case 'relatorioescolas':
							$origem = array(
								'mundescricao' => array(
									'codigo'    => 'mundescricao',
									'descricao' => 'Município'
								),
								'entnome' => array(
									'codigo'    => 'entnome',
									'descricao' => 'Escola'
								),
								'esddsc' => array(
									'codigo'    => 'esddsc',
									'descricao' => 'Situação'
								)
							);
							break;
					}
					
					// exibe agrupador
					$agrupador->setOrigem( 'naoAgrupador', null, $origem );
					$agrupador->setDestino( 'agrupador', null, $destino );
					$agrupador->exibir();
					
				?>
			</td>
		</tr>
		
		<!-- FORMATO DE SAÍDA -------------------------------------------------------- -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Formato de saída
			</td>
			<?php 
				if($_REQUEST['tipoSaida'] == 'listagem'){
					$check_listagem = "checked=\"checked\"";
				} else {
					$check_agrupado = "checked=\"checked\"";
				}
			?>
			<td>
				<input type="radio" name="tipoSaida" id="agrupado" value="agrupado" <?php echo $check_agrupado; ?> /> <label for="agrupado">Agrupado</label>
				<input type="radio" name="tipoSaida" id="listagem" value="listagem" <?php echo $check_listagem; ?> /> <label for="listagem">Listagem</label>
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
		
		<!-- MUNICIPIO -------------------------------------------------------- -->
		<tr id="linhaMunicipio">
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

		<!-- SITUAÇÃO -------------------------------------------------------- -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Situação
			</td>
			<td>
				<?php
								
				$sqlComboSituacao = "
					select  esdid as codigo,
							esddsc as descricao
					from workflow.estadodocumento
					where tpdid = 7
					order by esdordem
				";
				combo_popup( "esdid", $sqlComboSituacao, "Situação", "400x400", 0, array(), "", "S", false, false, 5, 400 );
				?>
			</td>
		</tr>

		<!-- ANO -------------------------------------------------------- -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Ano
			</td>
			<td>
				<?php
								
				$sqlComboAno = "
						   select '2010' as codigo, '2009' as descricao
  						   union all
						   select '2011' as codigo, '2010' as descricao
						   union all
						   select '2012' as codigo, '2011' as descricao
						   union all
						   select '2013' as codigo, '2012' as descricao
						   union all
						   select '2014' as codigo, '2013' as descricao
						   union all
						   select '2015' as codigo, '2014' as descricao
				";
				combo_popup( "ano", $sqlComboAno, "Ano", "400x400", 0, array(), "", "S", false, false, 5, 400 );
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
					onclick="exibirRelatorio('html');"
				/>
				<input
					type="button"
					name="filtrarxls"
					value="Visualizar XLS"
					onclick="exibirRelatorio('xls');"
				/>
			</td>
		</tr>
		
	</table>
</form>