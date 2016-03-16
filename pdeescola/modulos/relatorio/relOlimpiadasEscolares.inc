<?php 

// Exibe consulta
if ( isset( $_REQUEST['pesquisa'] ) == true){
	
	include "relOlimpiadasEscolares_result.inc";
	die;	
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

echo "<br>";

$titulo_modulo = "Relatório Olimpíadas Escolares";
monta_titulo( $titulo_modulo, 'Selecione os filtros e agrupadores desejados' );

?>

<script src="../includes/calendario.js"></script>
<script language="JavaScript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<script type="text/javascript">
<!--

	function exibeRelatorio( ){
		
		var formulario = document.formulario;
		formulario.pesquisa.value='1';

		selectAllOptions( document.getElementById( 'agrupador' ) );
		selectAllOptions( document.getElementById( 'tesid' ) );
		selectAllOptions( document.getElementById( 'estuf' ) );
		selectAllOptions( document.getElementById( 'muncod' ) );
		
		formulario.target = 'resultadoGeral';
		var janela = window.open( '?modulo=relatorio/relOlimpiadasEscolares&acao=A', 'resultadoGeral', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		
		formulario.submit();
		janela.focus();		
	}
	
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
	
//-->
</script>

<form action="" method="post" name="formulario" id="filtro">
	
	<input type="hidden" name="form" value="1"/>
	<input type="hidden" id="req" name="req" value=""/>	
	<input type="hidden" name="pesquisa" value="1"/>
	
	<!-- FILTROS -->
	
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
	
		<tr>
			<td width="195" class="SubTituloDireita">Agrupadores:</td>
			<td>
				<?php 
				
				// Início dos agrupadores
				$agrupador = new Agrupador('filtro','');
				
				// Dados padrão de origem
				$origem = array(
					'tipo' => array(
						'codigo'    => 'tipo',
						'descricao' => 'Tipo'
					),
					'estuf' => array(
						'codigo'    => 'estuf',
						'descricao' => 'UF'
					),
					'mundescricao' => array(
						'codigo'    => 'mundescricao',
						'descricao' => 'Município'
					),					
					'instituicao' => array(
						'codigo'    => 'instituicao',
						'descricao' => 'Instituição'
					)
				);
				
				$destino = array();
											
				// exibe agrupador
				$agrupador->setOrigem( 'naoAgrupador', null, $origem );
				$agrupador->setDestino( 'agrupador', null, $destino );
				$agrupador->exibir();
				
				?>
			</td>
		</tr>
	
		<tr>
			<td class="subtituloDireita">Raio</td>
			<td><?php echo campo_texto('raio', 'S', 'S', '', 8, 4, '###.###', '', ''); ?></td>
		</tr>
		
		<?php
		$arrVisivel = array("descricao");
		$arrOrdem = array("descricao");
		
		// tipos
		$stSql = " select 
							tesid as codigo, 
							tesdesc as descricao 
						from 
							entidade.tipoestrutura
						order by 
							tesdesc";

		$stSqlCarregados = "";
		mostrarComboPopup( 'Tipo:', 'tesid',  $stSql, $stSqlCarregados, 'Selecione o(s) Tipos(s)' ,null,null,null,null,$arrVisivel,$arrOrdem);
		
		// ufs
		$stSql = " select 
							estuf as codigo, 
							estdescricao as descricao 
						from 
							territorios.estado
						order by 
							estuf";

		$stSqlCarregados = "";
		mostrarComboPopup( 'UF:', 'estuf',  $stSql, $stSqlCarregados, 'Selecione a(s) UF(s)' ,null,null,null,null,$arrVisivel,$arrOrdem);
		
		mostrarComboPopupMunicipios();
		?>
					
		<tr>
			<td bgcolor="#CCCCCC"></td>
			<td bgcolor="#CCCCCC">
				<input type="button" value="Visualizar" onclick="exibeRelatorio();" style="cursor: pointer;"/>				
			</td>
		</tr>
		
	</table>
	
	<!-- FIM FILTROS -->
		
</form>