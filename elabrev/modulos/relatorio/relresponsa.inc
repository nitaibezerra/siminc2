<?php
/*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Leticia Jacome ()
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Henrique Xavier Couto (henriquecouto@mec.gov.br)
   Módulo:relresponsa.inc
   Finalidade: permitir a construção de relatorio de responsabilidades
    */


if ( isset( $_REQUEST['form'] ) == true )
{
	include 'relresponsa_mostra.inc'; 
	exit();
}

// variáveis que alimentam o formulário
// variáveis dos filtros gerais
$titulo = '';
$ano = '';
$nao_mostra_filtro_impressao = false;
$agrupador = array();
$sumarizador = '';
$publico = '';
$prtid = '';
$excluir = '';
$escala = '1';

// FIM variáveis que alimentam o formulário

$ano = $_REQUEST['ano'] ? $_REQUEST['ano'] : $_SESSION['exercicio'];


// variáveis que alimentam o formulário
// variáveis dos combos
// variáveis devem ser alimentadas pelos querys antecedentes aos combos
$orgao = array();

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( 'Relatório de Responsabilidades', 'Relatório Geral' );

?>
<script type="text/javascript">

	/**
	 * Alterar visibilidade de um bloco.
	 * 
	 * @param string indica o bloco a ser mostrado/escondido
	 * @return void
	 */
	function onOffBloco( bloco )
	{
		var div_on = document.getElementById( bloco + '_div_filtros_on' );
		var Arrayoff = new Array();

		if ( bloco == 'a' ) { Arrayoff[0] = 'u'; Arrayoff[1] = 'p'; };
		if ( bloco == 'u' ) { Arrayoff[0] = 'a'; Arrayoff[1] = 'p'; };
		if ( bloco == 'p' ) { Arrayoff[0] = 'a'; Arrayoff[1] = 'u'; };

		for (i=0; i < 2; i++)
			{
				document.getElementById( Arrayoff[i] + '_div_filtros_on' ).style.display = 'none';
			}
		div_on.style.display = 'block';

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
	
	/**
	 * Realiza submissão de formulário. Caso o exercício (ano) tenha sido
	 * alterado a submissão é realizada para a própria página, caso contrário
	 * para uma nova janela.
	 * 
	 * @return void
	 */
	function submeterFormulario( tipo )
	{
		var formulario = document.formulario;
		var nomerel = '';
		var qtd = 0;
		<? $qtdrel = 0; ?>
		prepara_formulario();
//		selectAllOptions( formulario.agrupador );
		
		if ( tipo == 'relatorio' )
		{
			formulario.action = 'elabrev.php?modulo=relatorio/relresponsa&acao=A';
			window.open( '', 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
			formulario.target = 'relatorio';
		}
		else
		{
			if ( tipo == 'planilha' )
			{
				formulario.action = 'financeiro.php?modulo=relatorio/geral&acao=R&tipoRelatorio=xls';
			}
			else if ( tipo == 'salvar' ) 
			{

				if ( document.formulario.titulo.value == '' ) 
				{
					alert( 'É necessário informar a descrição do relatório!' );
					document.formulario.titulo.focus();
					return;
				}
				var nomesExistentes = new Array();
				<?php
					$sqlNomesConsulta = "select prtdsc from public.parametros_tela";
					$nomesExistentes = $db->carregar( $sqlNomesConsulta );
					if ( $nomesExistentes )
					{
						foreach ( $nomesExistentes as $linhaNome )
						{
							print "nomesExistentes[nomesExistentes.length] = '" . str_replace( "'", "\'", $linhaNome['prtdsc'] ) . "';";
						}
					}
				?>
				var confirma = true;
				var i, j = nomesExistentes.length;
				for ( i = 0; i < j; i++ )
				{
					if ( nomesExistentes[i] == document.formulario.titulo.value )
					{
						confirma = confirm( 'Deseja alterar a consulta já existente?' );
						break;
					}
				}
				if ( !confirma )
				{
					return;
				}
				formulario.action = 'financeiro.php?modulo=relatorio/geral&acao=R&salvar=1';
			}
			else
			{
				formulario.action = '';
			}
			formulario.target = '_top';
		}
		formulario.submit();
	}
	
	function alterarAno()
	{
		var formulario = document.formulario;
		formulario.alterar_ano.value = '1';
		submeterFormulario('relatorio');
	}
	
</script>
<form action="" method="post" name="formulario">
	
	<input type="hidden" name="form" value="1"/> <!-- indica envio de formulário -->
	<input type="hidden" name="alterar_ano" value="0"/> <!-- indica se há mudança de ano no formulário -->
	<input type="hidden" name="publico" value="<?= $publico ?>"/> <!-- indica se foi clicado para tornar o relatório público ou privado -->
	<input type="hidden" name="prtid" value="<?= $prtid ?>"/> <!-- indica se foi clicado para tornar o relatório público ou privado, passa o prtid -->
	<input type="hidden" name="carregar" value=""/> <!-- indica se foi clicado para carregar o relatório -->
	<input type="hidden" name="excluir" value="<?= $excluir ?>"/> <!-- indica se foi clicado para excluir o relatório já gravado -->

	<!-- ----------------------------------------------------------------------- -->
	
	<!-- INSTITUCIONAL -->
	
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
			<tr>
				<td width="195" class="SubTituloDireita" valign="top" align="left">&nbsp;Por ação</td>
				<td><input type="radio" name="geral" value="a" id="geral" onclick="onOffBloco( 'a' );">
				</td>
			</tr>
			<tr>
				<td width="195" class="SubTituloDireita" valign="top">&nbsp;Por unidade</td>
				<td valign="top"><input type="radio" name="geral" value="u" id="geral" onclick="onOffBloco( 'u' );">&nbsp;&nbsp;
				&nbsp;Listar ações<input type="checkbox" name="lsacaounid" value="1" id="lsacaounid" >
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" valign="top">&nbsp;Por programa</td>
				<td><input type="radio" name="geral" value="p" id="geral" onclick="onOffBloco( 'p' );">&nbsp;&nbsp;
				&nbsp;Listar unidades<input type="checkbox" name="lsunidadeprog" value="1" id="lsunidade" >&nbsp;&nbsp;
				&nbsp;Listar ações<input type="checkbox" name="lsacaoprog" value="1" id="lsacaoprog" >
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" valign="top">&nbsp;Listar apenas itens sem vínculo</td>
				<td><input type="checkbox" name="vinculo" value="1" id="vinculo" >
				</td>
			</tr>
	</table>
	<div id="a_div_filtros_on" style="display:none;">
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
			<tr>
				<td width="195" valign="top" align="right">
					Equipes
				</td>
				<td>&nbsp;</td>
			</tr>
			<tr>
				<td width="195" class="SubTituloDireita" valign="top" >
					&nbsp;
					Equipe de Planejamento Ação
				</td>
				<td><input type="checkbox" name="equipe_planeja_acao" value="1" class="">
				</td>
			</tr>
			<tr>
				<td width="195" class="SubTituloDireita" valign="top" >
					&nbsp;
					Coordenador de Planejamento Ação
				</td>
				<td><input type="checkbox" name="coord_planeja_acao" value="1" class="">
				</td>
			</tr>

			
		</table>
	</div>

	<div id="u_div_filtros_on" style="display:none;">
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
			<tr>
				<td width="195" valign="top" align="right">
					Equipes
				</td>
				<td>&nbsp;</td>
			</tr>
			<tr>
				<td width="195" class="SubTituloDireita" valign="top"  >
					&nbsp;
					Equipe de Planejamento Unidade
				</td>
				<td><input type="checkbox" name="equipe_planeja_unidade" value="1" class="">
				</td>
			</tr>
			<tr>
				<td width="195" class="SubTituloDireita" valign="top" >
					&nbsp;
					Coordenador de Planejamento Unidade
				</td>
				<td><input type="checkbox" name="coord_planeja_unidade" value="1" class="">
				</td>
			</tr>
		</table>
	</div>

	<div id="p_div_filtros_on" style="display:none;">
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
			<tr>
				<td width="195" valign="top" align="right">
					Equipes
				</td>
				<td>&nbsp;</td>
			</tr>
			<tr>
				<td width="195" class="SubTituloDireita" valign="top" >
					&nbsp;
					UMA Unidade
				</td>
				<td><input type="checkbox" name="uma_unidade" value="1" class="">
				</td>
			</tr>
			<tr>
				<td width="195" class="SubTituloDireita" valign="top" >
					&nbsp;
					UMA Programa
				</td>
				<td><input type="checkbox" name="uma_programa" value="1" class="">
				</td>
			</tr>
			<tr>
				<td width="195" class="SubTituloDireita" valign="top" >
					&nbsp;
					Coordenador UMA
				</td>
				<td><input type="checkbox" name="uma_coord" value="1" class="">
				</td>
			</tr>
		</table>
	</div>
	
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
			<tr>
				<td width="195" class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'programa' );">
					Programa
					<input type="hidden" id="programa_campo_flag" name="programa_campo_flag" value="0"/>
				</td>
				<td>
					<div id="programa_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'programa' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
					<div id="programa_campo_on" style="display:none;">
						<? 
						
						$sql_combo = "select distinct p.prgid as codigo,p.prgcod, p.prgcod || ' - ' || p.prgdsc as descricao from elabrev.ppaprograma_proposta p where p.orgcod ='".$_SESSION['ittorgao']."' and p.prsano = '" . $ano . "' order by p.prgcod  ";

						if ( $_REQUEST['programa'] && $_REQUEST['programa'][0] != '' )	
						{
							$sql_carregados = "select p.prgid as codigo, p.prgcod || ' - ' || p.prgdsc as descricao from elabrev.elaboracaorevisao r inner join elabrev.ppaprograma_proposta p on r.eracod = p.prgid where r.prsano = '" . $ano . "' and p.prgid in ('".implode("','",$_REQUEST['programa'])."') group by p.prgcod, p.prgdsc ";
							$programa=$db->carregar( $sql_carregados );
						}
						?>
						<? combo_popup( 'programa', $sql_combo, 'Selecione o(s) Programa(s)', '400x400', 0, array(), '', 'S', false, false ); ?>
					</div>
				</td>
			</tr>
			<? if ( $programa )  { ?>	<script type="text/javascript"> onOffCampo( 'programa' ); </script> <? } ?>
			<tr>
				<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'acacod' );">
					Ação
					<input type="hidden" id="acacod_campo_flag" name="acacod_campo_flag" value="0"/>
				</td>
				<td>
					<div id="acacod_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'acacod' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
					<div id="acacod_campo_on" style="display:none;">
						<? $sql_combo = "select distinct p.acaid as codigo, p.acacod || ' - ' || p.acadsc as descricao from elabrev.ppaacao_proposta p where p.prsano = '" . $ano . "' order by descricao"; 
						if ( $_REQUEST['acacod'] && $_REQUEST['acacod'][0] != '' )
						{
							$sql_carregados = "select acao.acacod as codigo, acao.acacod || ' - ' || acao.acadsc as descricao from financeiro.reporcfin r inner join ( select distinct acacod, acadsc, prgano from monitora.acao where prgano = '2006' and acasnrap = 'f' group by acacod, acadsc, prgano ) acao on acao.acacod = r.acacod and acao.prgano = r.rofano where r.rofano = '" . $ano . "' and acao.acacod in ('".implode("','",$_REQUEST['acacod'])."') group by acao.acacod, acao.acadsc ";
							$acacod = $db->carregar( $sql_carregados );
						}
						?>
						<? combo_popup( 'acacod', $sql_combo, 'Selecione a(s) Ação(ões)', '400x400', 0, array(), '', 'S', false, false ); ?>
					</div>
				</td>
			</tr>
			<? if ( $acacod )  { ?>	<script type="text/javascript"> onOffCampo( 'acacod' ); </script> <? } ?>
			<tr>
				<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'uo' );">
					Unidades Orçamentárias
					<input type="hidden" id="uo_campo_flag" name="uo_campo_flag" value="0"/>
				</td>
				<td>
					<div id="uo_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'uo' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
					<div id="uo_campo_on" style="display:none;">
						<? $sql_combo = "select u.unicod as codigo, u.unicod || ' - ' || u.unidsc as descricao from financeiro.reporcfin r inner join public.unidade u on r.unicod = u.unicod where r.rofano = '" . $ano . "' group by u.unicod, u.unidsc order by u.unicod, u.unidsc"; 
						if ( $_REQUEST['uo'] && $_REQUEST['uo'][0] != '' )
						{
							$sql_carregados = "select u.unicod as codigo, u.unicod || ' - ' || u.unidsc as descricao from financeiro.reporcfin r inner join public.unidade u on r.unicod = u.unicod where r.rofano = '" . $ano . "' and u.unicod in ('".implode("','",$_REQUEST['uo'])."') group by u.unicod, u.unidsc ";
							$uo=$db->carregar( $sql_carregados );
						}
						?>
						<? combo_popup( 'uo', $sql_combo, 'Selecione a(s) Unidade(s)', '400x400', 0, array(), '', 'S', false, false ); ?>
					</div>
				</td>
			</tr>
			<? if ( $uo )  { ?>	<script type="text/javascript"> onOffCampo( 'uo' ); </script> <? } ?>
		</table>
	
	
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-top:none;">
			<tr>
				<td align="center">
					<input type="button" name="Gerar Relatório" value="Gerar Relatório" onclick="javascript:submeterFormulario( 'relatorio' );"/>
					&nbsp;
					<input type="button" name="Exportar Planilha" value="Exportar Planilha" onclick="javascript:submeterFormulario( 'planilha' );"/>
				</td>
			</tr>
	</table>
	
	<!-- ----------------------------------------------------------------------- -->

</form>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<script type="text/javascript">
	
	//onOffBloco( 'outros' );
	//onOffBloco( 'minhasconsultas' );

	function tornar_publico( prtid )
	{
//		location.href = '?modulo=<?//= $modulo ?>&acao=R&prtid='+ prtid + '&publico=1';
		document.formulario.publico.value = '1';
		document.formulario.prtid.value = prtid;
		document.formulario.submit();
	}
	
	function excluir_relatorio( prtid )
	{
		document.formulario.excluir.value = '1';
		document.formulario.prtid.value = prtid;
		document.formulario.target = '_self';
		document.formulario.submit();
	}
	
	function carregar_consulta( prtid )
	{
		document.formulario.carregar.value = '1';
		document.formulario.prtid.value = prtid;
		document.formulario.target = '_self';
//		submeterFormulario('salvar');
//		alert( submeterFormulario('salvar') );
		document.formulario.submit();
	}
	
	function carregar_relatorio( prtid )
	{
		document.formulario.prtid.value = prtid;
		document.formulario.alterar_ano.value = '0';
		submeterFormulario( 'relatorio' );
	}

</script>