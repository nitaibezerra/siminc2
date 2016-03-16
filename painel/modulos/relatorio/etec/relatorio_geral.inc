<?php
// exibe consulta
if($_POST['relatorio']){
	include "geral_resultado.inc";
	exit;
}


include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

echo "<br>";
$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo = "Relatório E-Tec";
monta_titulo( $titulo_modulo, 'Selecione os filtros e agrupadores desejados' );

?>
<script type="text/javascript">

	function etec_exibeRelatorioGeral() {
		var formulario = document.filtro;
		selectAllOptions( formulario.agrupador );
		
		if ( !formulario.agrupador.options.length ){
			alert( 'Favor selecionar ao menos um item para agrupar o resultado!' );
			return false;
		}
		
		selectAllOptions( document.getElementById('regiao') );
		selectAllOptions( document.getElementById('uf') );
		selectAllOptions( document.getElementById('municipio') );
		selectAllOptions( document.getElementById('ofertante') );
		selectAllOptions( document.getElementById('nomecurso') );
		selectAllOptions( document.getElementById('polo') );
		
		formulario.target = 'resultadoETECGeral';
		var janela = window.open( '', 'resultadoETECGeral', 'width=800,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		janela.focus();
		formulario.submit();
	}
	
	/* Função para substituir todos */
	function replaceAll(str, de, para){
	    var pos = str.indexOf(de);
	    while (pos > -1){
			str = str.replace(de, para);
			pos = str.indexOf(de);
		}
	    return (str);
	}
	/* Função para adicionar linha nas tabelas */

			
				
	/**
	 * Alterar visibilidade de um bloco.
	 * 
	 * @param string indica o bloco a ser mostrado/escondido
	 * @return void
	 */
	function onOffBloco( bloco )
	{
		var div_on = document.getElementById( bloco + '_div_filtros_on' );
		var div_off = document.getElementById( bloco + '_div_filtros_off' );
		var img = document.getElementById( bloco + '_img' );
		var input = document.getElementById( bloco + '_flag' );
		if ( div_on.style.display == 'none' )
		{
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			input.value = '0';
			img.src = '/imagens/menos.gif';
		}
		else
		{
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			input.value = '1';
			img.src = '/imagens/mais.gif';
		}
	}
	
	/**
	 * Alterar visibilidade de um campo.
	 * 
	 * @param string indica o campo a ser mostrado/escondido
	 * @return void
	 */
	function onOffCampo( campo )
	{
		var div_on = document.getElementById( campo + '_campo_on' );
		var div_off = document.getElementById( campo + '_campo_off' );
		var input = document.getElementById( campo + '_campo_flag' );
		if ( div_on.style.display == 'none' )
		{
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			input.value = '1';
		}
		else
		{
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			input.value = '0';
		}
	}

</script>
<form action="" method="post" name="filtro">
<input type="hidden" name="relatorio" value="1">
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">

		<tr>
			<td class="SubTituloDireita">Agrupadores</td>
			<td>
				<?php

					// Início dos agrupadores
					$agrupador = new Agrupador('filtro','');
					
					// Dados padrão de destino (nulo)
					$destino = isset( $agrupador2 ) ? $agrupador2 : array();
					
					// Dados padrão de origem
					$origem = array(
						'coepolo' => array(
							'codigo'    => 'polo',
							'descricao' => 'Polo'
						),
						'coenomecurso' => array(
							'codigo'    => 'nomecurso',
							'descricao' => 'Nome Curso'
						),
						'coeofertante' => array(
							'codigo'    => 'ofertante',
							'descricao' => 'Ofertante'
						),
						'uf' => array(
							'codigo'    => 'uf',
							'descricao' => 'UF'
						),
						'regiao' => array(
							'codigo'    => 'regiao',
							'descricao' => 'Região'
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
		</table>
		
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
			<tr>
			<td class="SubTituloEsquerda" colspan="2">Filtros</td>
			</tr>
			<?php
				// Região
				$stSql = " SELECT
								regcod AS codigo,
								regdescricao AS descricao
							FROM 
								territorios.regiao
							%s
							ORDER BY
								regdescricao ";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Regiões', 'regiao',  $stSql, $stSqlCarregados, 'Selecione a(s) Regiões(s)' );

				// UF
				$stSql = " SELECT
								estuf AS codigo,
								estdescricao AS descricao
							FROM 
								territorios.estado
							ORDER BY
								estdescricao ";
				mostrarComboPopup( 'UF', 'uf',  $stSql, $stSqlCarregados, 'Selecione a(s) UF(s)' );

				// Município
				$stSql = "  SELECT
								tm.muncod AS codigo,
								tm.estuf || ' - ' || tm.mundescricao AS descricao
							FROM 
								territorios.municipio tm
							ORDER BY
								mundescricao ";
				mostrarComboPopup( 'Município', 'municipio',  $stSql, $stSqlCarregados, 'Selecione o(s) Município(s)' );

				// Ofertante
				$stSql = "  SELECT
								coeofertante AS codigo,
								coeofertante AS descricao
							FROM 
								 painel.consultaofertasetec co 
							GROUP BY 
								coeofertante
							ORDER BY
								coeofertante";
				mostrarComboPopup( 'Ofertante', 'ofertante',  $stSql, $stSqlCarregados, 'Selecione o(s) Ofertante' );
				
				// Nome Curso
				$stSql = "SELECT
								coenomecurso AS codigo,
								coenomecurso AS descricao
							FROM 
								 painel.consultaofertasetec 
							GROUP BY 
								coenomecurso
							ORDER BY
								coenomecurso";
				mostrarComboPopup( 'Nome Curso', 'nomecurso',  $stSql, $stSqlCarregados, 'Selecione o(s) Curso(s)' );
				
				// Polo
				$stSql = "  SELECT
								coepolo AS codigo,
								coepolo AS descricao
							FROM 
								 painel.consultaofertasetec co 
							GROUP BY 
								coepolo
							ORDER BY
								coepolo";
				mostrarComboPopup( 'Polo', 'polo',  $stSql, $stSqlCarregados, 'Selecione o(s) Polo(s)' );
				
				
			?>
		<tr>
			<td bgcolor="#CCCCCC"></td>
			<td bgcolor="#CCCCCC">
				<input type="button" value="Visualizar" onclick="etec_exibeRelatorioGeral();"/>
			</td>
		</tr>
	</table>
</form>