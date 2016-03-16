<?php

//verifica_sessao();

if ( isset( $_REQUEST['buscar'] )  )
{
	include "geral_municipio_resultado2.inc";
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
//monta_titulo( "Relatório Geral", "" );
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
		selectAllOptions( document.getElementById( 'estuf' ) );
		selectAllOptions( document.getElementById( 'muncod' ) );
		selectAllOptions( document.getElementById( 'undid' ) );
		selectAllOptions( document.getElementById( 'prgid' ) );
		selectAllOptions( document.getElementById( 'sbastgmpl' ) );
		selectAllOptions( document.getElementById( 'ideb' ) );
		selectAllOptions( document.getElementById( 'ssuid' ) );
		//

		formulario.target = 'resultadoCteGeral';
		var janela = window.open( '', 'resultadoCteGeral', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		formulario.submit();
		janela.focus();
	}

</script>
<form action="" method="post" name="filtro">
	<input type="hidden" name="buscar" value="1"/>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		
		
		<!-- GRANDES CIDADE ------------------------------------------------ -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Filtro por Tipo
			</td>
			<td>
				<input type="checkbox" name="grandescidades" id="grandescidades" value="1"/>
				<label for="grandescidades">Grandes Cidades</label>
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
						'formacao' => array(
							'codigo'    => 'formacao',
							'descricao' => 'FORMAÇÃO dos PROFISSIONAIS da EDUCAÇÃO / CURSO'
						),
				
						'estado' => array(
							'codigo'    => 'estado',
							'descricao' => 'Estado'
						),
					
						'unidade' => array(
							'codigo'    => 'unidade',
							'descricao' => 'Unidade de Medida'
						),
						
						'programa' => array(
							'codigo'    => 'programa',
							'descricao' => 'Programa'
						),
				
						'municipio' => array(
							'codigo'    => 'municipio',
							'descricao' => 'Município'
						)
					);
					
					// exibe agrupador
					$agrupador->setOrigem( 'naoAgrupador', null, $origem );
					$agrupador->setDestino( 'agrupador', null, $destino );
					$agrupador->exibir();
					
				?>
			</td>
		</tr>
		
		<!-- CALSSIFICACAO IDEB -------------------------------------------- -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Classificação IDEB
			</td>
			<td>
				<?php
				$sqlComboIDEB = "
					select
						tpmid as codigo,
						tpmdsc as descricao
					from territorios.tipomunicipio
					where
						gtmid = ( select gtmid from territorios.grupotipomunicipio where gtmdsc = 'Classificação IDEB' ) and
						tpmstatus = 'A'
				";
				combo_popup( "ideb", $sqlComboIDEB, "Classificação IDEB", "215x400", 0, array(), "", "S", false, false, 5, 400 );
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
					group by
						m.muncod,
						m.estuf,
						m.mundescricao
					order by
						m.estuf,
						m.mundescricao
				";
				combo_popup( "muncod", $sqlComboMunicipio, "Município", "400x400", 0, array(), "", "S", false, false, 5, 400 );
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
						upper(unddsc) as descricao
					from cte.unidademedida
					where
						undtipo = 'M'
					order by
						upper(unddsc)
				";
				combo_popup( "undid", $sqlComboUnidade, "Unidades de Medida", "400x400", 0, array(), "", "S", false, false, 5, 400 );
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
		<!-- FORMAÇÃO ------------------------------------------------------ -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				 FORMAÇÃO dos PROFISSIONAIS da EDUCAÇÃO / CURSO
			</td>
			<td>
				<?php
				$sqlComboPrograma = "
					select
						trim(s.sbastgmpl) as codigo,
						trim(s.sbastgmpl) as descricao
					from 
						cte.proposicaosubacao ps 
						inner join cte.subacaoindicador s ON s.ppsid = ps.ppsid 
						inner join cte.acaoindicador a ON a.aciid = s.aciid 
						inner join cte.pontuacao p ON p.ptoid = a.ptoid 
						inner join cte.instrumentounidade iu ON iu.inuid = p.inuid 
						inner join territorios.municipio m ON m.muncod = iu.muncod and m.estuf = iu.mun_estuf 
						inner join cte.programa pr ON pr.prgid = s.prgid 
						inner join cte.unidademedida u ON u.undid = s.undid 
					where 
						ps.ppsrel = true and 
						p.ptostatus = 'A' 					
					group by
						s.sbastgmpl
				";
				combo_popup( "sbastgmpl", $sqlComboPrograma, "Formção", "400x400", 0, array(), "", "S", false, false, 5, 400 );
				?>
			</td>
		</tr>
		

		<!-- STATUS SUB AÇÃO ------------------------------------------------------ -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Status de análise
			</td>
			<td>
				<?php
				$sqlComboStatusSubAcao = "
					select
						ssuid 		as codigo,
						ssudescricao 	as descricao
					from cte.statussubacao					
					order by
						ssudescricao
				";
				combo_popup( "ssuid", $sqlComboStatusSubAcao, "Status de Análise", "400x400", 0, array(), "", "S", false, false, 5, 400 );
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
				&nbsp;&nbsp;
				<input type="checkbox" value="xls" name="xls"> Xls
				
			</td>
		</tr>
		
	</table>
</form>