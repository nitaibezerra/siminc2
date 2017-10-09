<?php

/**
 * Relatórios geral de cursos e vagas
 * 
 * @author Fernando Bagno <fernando.silva@mec.gov.br>
 * @since 15/03/2010
 * @version 1.0
 * 
 */

// execuções da tela
switch( $_REQUEST["requisicao"] ){
	
	case "gerarelatorio":
		include "resultadoRelCursos.inc";
		die;
	break;
	
}

// Cabecalho padrão do sistema
include APPRAIZ . "includes/cabecalho.inc";
include APPRAIZ . "includes/Agrupador.php";

// monta o título da tela
print "<br/>";
monta_titulo( "Relatório de Cursos", "" );

?>
<script type="text/javascript">

	function acaValidaRelCursos(){
		
		var formulario = document.getElementById( "formRelCursos" );
		var agrupador  = document.getElementById( "agrupador" );
		
		if ( !agrupador.options.length ){
			alert( 'Favor selecionar ao menos um item para agrupar o resultado!' );
			return false;
		}

		selectAllOptions( formulario.agrupador );
		selectAllOptions( document.getElementById( 'entid' ) );
		selectAllOptions( document.getElementById( 'entidcampus' ) );
		selectAllOptions( document.getElementById( 'pgcid' ) );
		selectAllOptions( document.getElementById( 'tpcid' ) );


		formulario.action = 'academico.php?modulo=relatorio/cursosevagas/relCursos&acao=A';
		var janela = window.open( '', 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		formulario.target = 'relatorio';

		janela.focus();
		
		formulario.submit();
		
	}

</script>
<form method="post" action="" name="formulario" id="formRelCursos">
	<input type="hidden" name="requisicao" value="gerarelatorio" />
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td class="SubTituloDireita">Agrupadores</td>
			<td>
				<?php

					// Início dos agrupadores
					$agrupador = new Agrupador( "formRelCursos", "" );
					
					// Dados padrão de destino (nulo)
					$destino = isset( $agrupador2 ) ? $agrupador2 : array();
					
					// Dados padrão de origem
					$origem = array(
						"unidade" => array(
							"codigo"    => "unidade",
							"descricao" => "Unidades"
						),
						"campus" => array(
							"codigo"    => "campus",
							"descricao" => "Campus"
						),	
						"programa" => array(
							"codigo"    => "programa",
							"descricao" => "Programa"
						),
						"curso" => array(
							"codigo"    => "curso",
							"descricao" => "Curso"
						),
						"nivel" => array(
							"codigo"    => "nivel",
							"descricao" => "Nivel( Graduação / Pós-Graduação )"
						)						
					);
					
					$destino = array(
						"tipo" => array(
							"codigo"    => "tipo",
							"descricao" => "Tipo ( Pactuado / Executado )"
						)
					);
					
					// exibe agrupador
					$agrupador->setOrigem( "naoAgrupador", null, $origem );
					$agrupador->setDestino( "agrupador", null, $destino );
					$agrupador->exibir();
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloCentro" colspan="2">Filtros de Pesquisa</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Unidade</td>
			<td>
				<?php 
				
					$sql = "SELECT DISTINCT
								ee.entid as codigo,
								ee.entnome as descricao
							FROM
								entidade.entidade ee
							INNER JOIN
								public.curso pc ON pc.entid = ee.entid
							INNER JOIN
								academico.cursodetalhe ac ON ac.curid = pc.curid
							INNER JOIN
								entidade.funcaoentidade ef ON ee.entid = ef.entid
							WHERE
								curstatus = 'A' AND funid in ( 11, 12 )";
				
					combo_popup( "entid", $sql, "", "400x400", 0,  array(), "", "S", true, true );
				
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Campus</td>
			<td>
				<?php 
				
					$sql = "SELECT DISTINCT
								ee.entid as codigo,
								ee.entnome as descricao
							FROM
								entidade.entidade ee
							INNER JOIN
								academico.cursodetalhe ac ON ac.entid = ee.entid
							WHERE
								cdtstatus = 'A'
							
								";
				
					combo_popup( "entidcampus", $sql, "", "400x400", 0,  array(), "", "S", true, true );
				
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Programa</td>
			<td>
				<?php 
				
					$sql = "SELECT 
								pgcid as codigo,
								pgcdsc as descricao
							FROM 
								academico.programacurso
							ORDER 
								by pgcdsc";
				
					combo_popup( "pgcid", $sql, "", "400x400", 0,  array(), "", "S", true, true );
				
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Nível</td>
			<td>
				<?php 
				
					$sql = "SELECT DISTINCT
								tpcid as codigo,
								tpcdsc as descricao
							FROM
								public.tipocurso
							WHERE
								tpeid = 1";
				
					combo_popup( "tpcid", $sql, "", "400x400", 0,  array(), "", "S", true, true );
				
				?>
			</td>
		</tr>
		<tr bgcolor="#D0D0D0">
			<td></td>
			<td>
				<input type="button" value="Visualizar" onclick="acaValidaRelCursos();" style="cursor: pointer;"/>
			</td>
		</tr>
	</table>
</form>