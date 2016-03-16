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
	include 'resultContratacao.inc';
	die;
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

echo "<br>";
$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo = "Relatório de Contratos";
monta_titulo( $titulo_modulo, 'Selecione os filtros e agrupadores desejados' );
?>
<script type="text/javascript">
<!--

function obras_exibeRelatorioGeral(tipo){

	var formulario = document.filtro;
	var agrupador  = document.getElementById( 'agrupador' );
	var coluna     = document.getElementById( 'coluna' );

	if (tipo == 'relatorio'){
		formulario.target = 'resultadoPlanoAcao';
		var janela = window.open( '', 'resultadoPlanoAcao', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
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

	selectAllOptions( document.getElementById( 'stpid' ) );
	selectAllOptions( document.getElementById( 'itrid' ) );

	document.getElementById('publico').value 	= '';
	document.getElementById('prtid').value	 	= '';
	document.getElementById('carregar').value 	= '';
	document.getElementById('excluir').value 	= '';

	if (tipo == 'exibir'){
		formulario.target = 'resultadoPlanoAcao';
		var janela = window.open( '', 'resultadoPlanoAcao', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
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
	<td class="SubTituloDireita">Título</td>
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
			    'objeto'					=> array(
                                        				'codigo'    => 'conobjeto',
                                        				'descricao' => 'Objeto'
                									),
				'atividade'					=> array(
														'codigo'    => 'atidescricao',
														'descricao' => 'Atividade'
													),
				'situacao'					=> array(
														'codigo'    => 'stpdsc',
														'descricao' => 'Situação do processo de contratação'
													),
				'forma'						=> array(
														'codigo'    => 'itrdsc',
														'descricao' => 'Forma de Contratação'
													),
				'situacaoAcompanhamento'	=> array(
														'codigo'    => 'situacao',
														'descricao' => 'Situação do acompanhamento'
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
				'datainicio'    => array(
										'codigo'    => 'condatainicio',
										'descricao' => 'Data de Início'
									),
				'datafim'       => array(
										'codigo'    => 'condatatermino',
										'descricao' => 'Data Prevista de Término'
									),
				'valorprevisto' => array(
										'codigo'    => 'convalorestimado',
										'descricao' => 'Valor Estimado'
									),
				'responsavel'   => array(
										'codigo'    => 'usunome',
										'descricao' => 'Responsável'
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

                $sql = "SELECT stpid AS codigo, stpdsc AS descricao FROM sisplan.situacaoprocesso WHERE stpstatus = 'A' ORDER BY 2";
                $stSqlCarregados = "";
                mostrarComboPopup( 'Situação do processo de contratação', 'stpid',  $sql, $stSqlCarregados, 'Selecione a(s) situação(ões)' );

        ?>



        <?php
                $sql = "select itrid AS codigo, itrdsc AS descricao from sisplan.instrumento order by itrdsc asc";
                $stSqlCarregados = "";
                mostrarComboPopup( 'Forma de Contratação(Modalidade)', 'itrid',  $sql, $stSqlCarregados, 'Selecione a(s) forma(s) de contratação' );

        ?>

        <tr>
            <td class="SubTituloDireita">
                Data de Início:
            </td>
            <td>
                <?php
                    echo campo_data2( 'condatainicio1', 'N', 'S', '', 'S', '' );
                ?>
                &nbsp;a&nbsp;
                <?php
                    echo campo_data2( 'condatainicio2', 'N', 'S', '', 'S', '' );
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Data de Término Prevista:
            </td>
            <td>
                <?php
                    echo campo_data2( 'condatatermino1', 'N', 'S', '', 'S', '' );
                ?>
                &nbsp;a&nbsp;
                <?php
                echo campo_data2( 'condatatermino2', 'N', 'S', '', 'S', '' ); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Fase com atraso:
            </td>
            <td>
                <input type="radio" name="faseAtraso" id="faseAtrasoS" value="sim" /><label for="faseAtrasoS">Sim</label>
                <input type="radio" name="faseAtraso" id="faseAtrasoN" value="nao" /><label for="faseAtrasoN">Não</label>
                <input type="radio" name="faseAtraso" id="faseAtrasoT" value="todas" checked/><label for="faseAtrasoT">Todas</label>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Situação do acompanhamento:
            </td>
            <td>
                <input type="checkbox" name="situacaoConcluida" id="situacaoConcluida" value="concluida" <?= $situacaoConcluida == 'concluida' ? 'checked="checked"' : ''; ?>  /><label for="situacaoConcluida"><span class="divCor" style="background-color: blue;" title="Concluída">&nbsp;&nbsp;&nbsp;&nbsp;</span> Concluída</label>
                <input type="checkbox" name="situacaoEstavel"   id="situacaoEstavel"   value="estavel"   <?= $situacaoEstavel   == 'estavel'   ? 'checked="checked"' : ''; ?>  /><label for="situacaoEstavel"><span class="divCor" style="background-color: green;" title="{labeltexto}">&nbsp;&nbsp;&nbsp;&nbsp;</span> Estável</label>
                <input type="checkbox" name="situacaoAtencao"   id="situacaoAtencao"   value="atencao"   <?= $situacaoAtencao   == 'atencao'   ? 'checked="checked"' : ''; ?>  /><label for="situacaoAtencao"><span class="divCor" style="background-color: #DADF00;" title="{labeltexto}">&nbsp;&nbsp;&nbsp;&nbsp;</span> Merece Atenção</label>
                <input type="checkbox" name="situacaoCritico"   id="situacaoCritico"   value="critico"   <?= $situacaoCritico   == 'critico'   ? 'checked="checked"' : ''; ?>  /><label for="situacaoCritico"><span class="divCor" style="background-color: red;" title="{labeltexto}">&nbsp;&nbsp;&nbsp;&nbsp;</span> Crítico</label>
            </td>
        </tr>
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
