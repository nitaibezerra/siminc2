<?php
// salva os POST na tabela
if ($_REQUEST['tipo'] == 'salvar'){
	$existe_rel = 0;
	$sql = sprintf("select
						prtid
					from
						public.parametros_tela
					where
						prtdsc = '%s'
						AND mnuid = %d
						AND (usucpf = '%s' OR prtpublico = %s)"
					, $_REQUEST['titulo']
					, $_SESSION['mnuid']
					, $_SESSION['usucpf']
					, 'true');
	$existe_rel = $db->pegaUm( $sql );
	if ($existe_rel > 0){
		$sql = sprintf(
						"UPDATE
							public.parametros_tela
						 SET
						 	prtdsc = '%s',
						 	prtobj = '%s',
						 	--prtpublico = 'FALSE',
						 	usucpf = '%s',
						 	mnuid = %d
						 WHERE
						 	prtid = %d",
						$_REQUEST['titulo'],
						addslashes( addslashes( serialize( $_REQUEST ) ) ),
						$_SESSION['usucpf'],
						$_SESSION['mnuid'],
						$existe_rel
					);
		$db->executar( $sql );
		$db->commit();
	}
	else
	{
		$sql = sprintf(
			"INSERT INTO public.parametros_tela
				( prtdsc, prtobj, prtpublico, usucpf, mnuid
			 ) VALUES (
			 	'%s', '%s', %s, '%s', %d )",
			$_REQUEST['titulo'],
			addslashes( addslashes( serialize( $_REQUEST ) ) ),
			'FALSE',
			$_SESSION['usucpf'],
			$_SESSION['mnuid']
		);
		$db->executar( $sql );
		$db->commit();
	}
	?>
	<script type="text/javascript">
		alert('Operação realizada com sucesso!');
		location.href = window.location;
	</script>
	<?
	die;

// transforma consulta em pública
}elseif ( $_REQUEST['prtid'] && $_REQUEST['publico'] == 1){
	$sql = sprintf("UPDATE public.parametros_tela
					SET
						prtpublico = case when prtpublico = true
										then false
										else true
									 end
					WHERE prtid = %d",
		$_REQUEST['prtid']
	);
	$db->executar( $sql );
	$db->commit();
	?>
	<script type="text/javascript">
		location.href = window.location;
	</script>
	<?
	die;
// FIM transforma consulta em pública
// remove consulta
}elseif ( $_REQUEST['prtid'] && $_REQUEST['excluir'] == 1 ) {
	$sql = sprintf("DELETE from public.parametros_tela WHERE prtid = %d",
					$_REQUEST['prtid']);
	$db->executar( $sql );
	$db->commit();
	?>
		<script type="text/javascript">
			location.href = window.location;
		</script>
	<?
	die;
// FIM remove consulta
}elseif ($_POST){
	include 'resultDespesa.inc';
	die;
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

echo "<br>";
$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo = "Relatório de Despesas";
monta_titulo( $titulo_modulo, 'Selecione os filtros e agrupadores desejados' );
?>
<script type="text/javascript">
<!--

function obras_exibeRelatorioGeral(tipo){

	var formulario = document.filtro;
	var agrupador  = document.getElementById( 'agrupador' );
	var coluna     = document.getElementById( 'coluna' );

	if (tipo == 'relatorio'){
		formulario.target = 'resultDespesa';
		var janela = window.open( '', 'resultDespesa', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		janela.focus();
		formulario.submit();
		return;
	}

	selectAllOptions( agrupador );
	selectAllOptions( coluna );

	if ( !agrupador.options.length ){
		alert( 'Favor selecionar ao menos um item para agrupar o resultado!' );
		return false;
	}

	if ( !coluna.options.length ){
		alert( 'Favor selecionar ao menos uma coluna!' );
		return false;
	}

	document.getElementById('tipo').value = tipo;

	selectAllOptions( document.getElementById( 'rfdid' ) );
	selectAllOptions( document.getElementById( 'uexid' ) );
	selectAllOptions( document.getElementById( 'ptrid' ) );
	selectAllOptions( document.getElementById( 'ndpid' ) );

	document.getElementById('publico').value 	= '';
	document.getElementById('prtid').value	 	= '';
	document.getElementById('carregar').value 	= '';
	document.getElementById('excluir').value 	= '';

	if (tipo == 'exibir'){
		formulario.target = 'resultDespesa';
		var janela = window.open( '', 'resultDespesa', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		janela.focus();
	}else if (tipo == 'salvar'){

		if ( formulario.titulo.value == '' ) {
			alert( 'É necessário informar o título do relatório!' );
			formulario.titulo.focus();
			return;
		}
		var nomesExistentes = new Array();
		<?php
			$sqlNomesConsulta = "SELECT
									prtdsc
								FROM
									public.parametros_tela
								WHERE
									mnuid = {$_SESSION['mnuid']}
									AND (usucpf = '{$_SESSION['usucpf']}' OR prtpublico = true)";
			$nomesExistentes = $db->carregar( $sqlNomesConsulta );
			if ( $nomesExistentes ){
				foreach ( $nomesExistentes as $linhaNome )
				{
					print "nomesExistentes[nomesExistentes.length] = '" . str_replace( "'", "\'", $linhaNome['prtdsc'] ) . "';";
				}
			}
		?>
		var i, j = nomesExistentes.length;
		for ( i = 0; i < j; i++ ){
			if ( nomesExistentes[i] == formulario.titulo.value ){
				if (!confirm( 'Deseja alterar a consulta já existente?' )){
					return;
				}
				break;
			}
		}

		formulario.target = '_self';
	}

	formulario.submit();
}
//-->
</script>
<form action="" method="post" name="filtro">
<input name="tipo" type="hidden" id="tipo">
<input type="hidden" name="publico" id="publico" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado -->
<input type="hidden" name="prtid" id="prtid" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado, passa o prtid -->
<input type="hidden" name="carregar" id="carregar" value=""/> <!-- indica se foi clicado para carregar o relatório -->
<input type="hidden" name="excluir" id="excluir" value=""/> <!-- indica se foi clicado para excluir o relatório já gravado -->
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
<tr>
	<td class="SubTituloDireita" width="20%">Título</td>
	<td>
		<?= campo_texto( 'titulo', 'N', 'S', '', 65, 60, '', '', 'left', '', 0, 'id="titulo"'); ?>
	</td>
</tr>
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
			    'exercicio'					=> array(
                                        				'codigo'    => 'exercicio',
                                        				'descricao' => 'Ano'
                									),
				'rfdnome'					=> array(
														'codigo'    => 'rfdnome',
														'descricao' => 'Referência'
													),
				'descricao'					=> array(
														'codigo'    => 'descricao',
														'descricao' => 'Unidade'
													),
				'ptres'						=> array(
														'codigo'    => 'ptres',
														'descricao' => 'PTRES'
													),
				'ndpdsc'					=> array(
														'codigo'    => 'ndpdsc',
														'descricao' => 'Natureza Despesa'
													)
			);

			// exibe agrupador
			$agrupador->setOrigem( 'naoAgrupador', null, $origem );
			$agrupador->setDestino( 'agrupador', null, $destino );
			$agrupador->exibir();
		?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita">Colunas</td>
	<td>
		<?php

			// Início dos agrupadores
			$agrupador = new Agrupador('filtro','');

			// Dados padrão de destino (nulo)
			$destino = isset( $agrupador2 ) ? $agrupador2 : array();

			// Dados padrão de origem
			$origem = array(
				'dpgvalor'    	   => array(
										'codigo'    => 'dpgvalor',
										'descricao' => 'Valor Programado'
									),
				'dpevalor'         => array(
										'codigo'    => 'dpevalor',
										'descricao' => 'Valor Executado'
									),
				'dpenudocumento' 	=> array(
										'codigo'    => 'dpenudocumento',
										'descricao' => 'Número do Documento'
									),
				'dpeobservacoes'   	=> array(
										'codigo'    => 'dpeobservacoes',
										'descricao' => 'Observações'
									),
				'saldo'   			=> array(
										'codigo'    => 'saldo',
										'descricao' => 'Saldo'
									)
			);

			// exibe agrupador
			$agrupador->setOrigem( 'naoColuna', null, $origem );
			$agrupador->setDestino( 'coluna', null, $destino );
			$agrupador->exibir();
		?>
	</td>
</tr>

<!-- OUTROS FILTROS -->
<table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
	<tr>
		<td onclick="javascript:onOffBloco( 'outros' );">
			<img border="0" src="/imagens/mais.gif" id="outros_img"/>&nbsp;
			Relatórios Gerenciais
			<input type="hidden" id="outros_flag" name="outros_flag" value="0" />
		</td>
	</tr>
</table>
<div id="outros_div_filtros_off">
</div>

<div id="outros_div_filtros_on" style="display:none;">
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-top:none;">
			<tr>
				<td width="195" class="SubTituloDireita" valign="top">Relatórios:</td>
				<?php

					if( $db->testa_superuser() || possuiPerfil(PERFIL_SUPERVISORMEC) ||
						possuiPerfil(PERFIL_ADMINISTRADOR) ){
					 	$bt_publicar = "<img border=\"0\" src=\"../imagens/usuario.gif\" title=\" Despublicar \" onclick=\"tornar_publico(' || prtid || ');\">&nbsp;&nbsp;";
					}

					$sql = sprintf(
						"SELECT Case when prtpublico = true and usucpf = '%s' then '{$bt_publicar}<img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ');\">&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">' else '<img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ');\">&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">' end as acao, '' || prtdsc || '' as descricao FROM public.parametros_tela WHERE mnuid = %d AND prtpublico = TRUE",
						$_SESSION['usucpf'],
						$_SESSION['mnuid'],
						$_SESSION['usucpf']
					);
					$cabecalho = array('Ação', 'Nome');
				?>
				<td><?php $db->monta_lista_simples( $sql, $cabecalho, 50, 50, null, null, null ); ?>
				</td>
			</tr>
	</table>
</div>
<script language="javascript">	//alert( document.formulario.agrupador_combo.value );	</script>

<!-- FIM OUTROS FILTROS -->

<!-- MINHAS CONSULTAS -->

<table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
	<tr>
		<td onclick="javascript:onOffBloco( 'minhasconsultas' );">
			<img border="0" src="/imagens/mais.gif" id="minhasconsultas_img"/>&nbsp;
			Minhas Consultas
			<input type="hidden" id="minhasconsultas_flag" name="minhasconsultas_flag" value="0" />
		</td>
	</tr>
</table>
<div id="minhasconsultas_div_filtros_off">
</div>
<div id="minhasconsultas_div_filtros_on" style="display:none;">
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-top:none;">
			<tr>
				<td width="195" class="SubTituloDireita" valign="top">Consultas</td>
				<?php

					$sql = sprintf(
						"SELECT
							CASE WHEN prtpublico = false THEN '<img border=\"0\" src=\"../imagens/grupo.gif\" title=\" Publicar \" onclick=\"tornar_publico(' || prtid || ')\">&nbsp;&nbsp;
															   <img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ')\">&nbsp;&nbsp;
															   <img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">'
														 ELSE '<img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ')\">&nbsp;&nbsp;
														 	   <img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">'
							END as acao,
							'' || prtdsc || '' as descricao
						 FROM
						 	public.parametros_tela
						 WHERE
						 	mnuid = %d AND usucpf = '%s'",
						$_SESSION['mnuid'],
						$_SESSION['usucpf']
					);

					$cabecalho = array('Ação', 'Nome');
				?>
				<td>
					<?php $db->monta_lista_simples( $sql, $cabecalho, 50, 50, 'N', '80%', null ); ?>
				</td>
			</tr>
	</table>
</div>
<!-- FIM MINHAS CONSULTAS -->

<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">

       	<?php
				// Referência
				$sql = "SELECT
							rfdid AS codigo,
							rfdnome AS descricao
						FROM
							sisplan.referenciadespesa
						WHERE
							rfdstatus = 'A' AND
							rfdano = '{$_SESSION['exercicio']}'
						ORDER BY
							rfdano,rfdordem";
                $stSqlCarregados = "";
                mostrarComboPopup( 'Referências', 'rfdid',  $sql, $stSqlCarregados, 'Selecione a(s) Referência(s)' );

                // Unidades Executoras
				$sql = "SELECT * FROM planointerno.unidadeexecutora_uo ";
                $stSqlCarregados = "";
                mostrarComboPopup( 'Unidades', 'uexid',  $sql, $stSqlCarregados, 'Selecione a(s) Unidade(s)' );

				// PTRES
				$sql = "SELECT
							ptr.ptrid AS codigo,
							ptr.ptres ||' - '|| ptr.prgcod || '.' || ptr.acacod || '.' || ptr.unicod || '.' || ptr.loccod || ' - ' || aca.acadsc AS descricao
						FROM
							sisplan.ptres ptr
						JOIN
						 	monitora.acao aca ON aca.acacod = ptr.acacod AND
												 aca.prgcod = ptr.prgcod AND
												 aca.loccod = ptr.loccod AND
												 aca.unicod = ptr.unicod AND
												 aca.prgano = '".$_SESSION['exercicio']."' AND
						 			             		aca.acastatus = 'A' AND
						 			             		aca.acasnrap IS FALSE
						WHERE
							ptr.ptrstatus = 'A' AND
							ptr.exercicio = '{$_SESSION['exercicio']}'
						ORDER BY
							2";
                $stSqlCarregados = "";
                mostrarComboPopup( 'PTRES', 'ptrid',  $sql, $stSqlCarregados, 'Selecione o(s) PTRES' );

				// Natureza Despesa
				$sql = "SELECT
							ndpid AS codigo,
							ndpcod || ' - ' || ndpdsc AS descricao
						FROM
							naturezadespesa
						WHERE
							ndpsubelementostatus = 'A'
						ORDER BY
							ndpid";
                $stSqlCarregados = "";
                mostrarComboPopup( 'Natureza Despesa', 'ndpid',  $sql, $stSqlCarregados, 'Selecione a(s) Natureza(s)' );

        ?>
		<tr>
			<td bgcolor="#CCCCCC"></td>
			<td bgcolor="#CCCCCC">
				<input type="button" value="Visualizar" 	 onclick="obras_exibeRelatorioGeral('exibir');" style="cursor: pointer;"/>
				<input type="button" value="Visualizar XLS"  onclick="obras_exibeRelatorioGeral('exibirXLS');" style="cursor: pointer;"/>
				<input type="button" value="Salvar Consulta" onclick="obras_exibeRelatorioGeral('salvar');" style="cursor: pointer;"/>
			</td>
		</tr>
		</table>
</form>
<script>
function tornar_publico( prtid ){
	document.filtro.publico.value = '1';
	document.filtro.prtid.value = prtid;
	document.filtro.target = '_self';
	document.filtro.submit();
}

function excluir_relatorio( prtid ){
	document.filtro.excluir.value = '1';
	document.filtro.prtid.value = prtid;
	document.filtro.target = '_self';
	document.filtro.submit();
}

function carregar_consulta( prtid ){
	document.filtro.carregar.value = '1';
	document.filtro.prtid.value = prtid;
	document.filtro.target = '_self';
	document.filtro.submit();
}

function carregar_relatorio( prtid ){
	document.filtro.prtid.value = prtid;
	obras_exibeRelatorioGeral( 'relatorio' );
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
</script>
