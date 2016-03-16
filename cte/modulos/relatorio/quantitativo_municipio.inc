<?php
ini_set("memory_limit","64M");

if ( isset( $_REQUEST['buscar'] ) )
{
	include "quantitativo_municipio_resultado.inc";
	exit();
}
else if ( isset( $_REQUEST['detalhamento'] ) )
{
	include "quantitativo_municipio_resultado_detalhe.inc";
	exit();
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( "Relatório Geral", "" );

?>
<script type="text/javascript">

	function exibirRelatorio()
	{
		var formulario = document.filtro;
		
		selectAllOptions( document.getElementById( 'estuf' ) );
		
		// submete formulario
		//alert( 'blz!' ); return;
		formulario.target = 'resultadoCteQuantitativo';
		
		var janela = window.open( '', 'resultadoCteQuantitativo', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		formulario.submit();
		janela.focus();
	}

</script>
<form action="" method="post" name="filtro">
	<input type="hidden" name="buscar" value="1"/>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		
		<!-- VALOR --------------------------------------------------------- -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Agrupador
			</td>
			<td>
				
				<b>Localidade</b>
				<br/>
				<input type="radio" name="agrupador" id="agrupador_microregiao" value="microregiao"/>
				<label for="agrupador_microregiao">Microregião</label>
				<br/>
				<input type="radio" name="agrupador" id="agrupador_mesoregiao" value="mesoregiao"/>
				<label for="agrupador_mesoregiao">Mesoregião</label>
				<br/>
				<input type="radio" name="agrupador" id="agrupador_uf" value="uf" checked="checked"/>
				<label for="agrupador_uf">Unidade da Federação</label>
				<br/>
				<input type="radio" name="agrupador" id="agrupador_regiao" value="regiao"/>
				<label for="agrupador_regiao">Região</label>
				<br/>
				<input type="radio" name="agrupador" id="agrupador_pais" value="pais"/>
				<label for="agrupador_pais">País</label>
				
				<br/><br/>
				
				<b>Grupo de municípios</b>
				<br/>
				<?php
					$sql = "
						select
							gtmid, 
							gtmdsc
						from territorios.grupotipomunicipio
						where
							gtmstatus = 'A'
					";
					$grupos = $db->carregar( $sql );
					$grupos = $grupos ? $grupos : array();
				?>
				<?php foreach ( $grupos as $grupo ) : ?>
					<input type="radio" name="agrupador" id="agrupador_<?= $grupo['gtmid'] ?>" value="<?= $grupo['gtmid'] ?>"/>
					<label for="agrupador_<?= $grupo['gtmid'] ?>"><?= $grupo['gtmdsc'] ?></label>
					<br/>
				<?php endforeach; ?>
				
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