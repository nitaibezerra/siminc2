<?php

	/*
		Sistema Simec
		Setor responsável: SPO-MEC
		Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
		Programador: Henrique Xavier Couto (e-mail: henriquexcouto@gmail.com), Renan de Lima (e-mail: renandelima@gmail.com), Fabrício Mendonça, Renê de Lima (renedelima@gmail.com)
		Módulo: momento.inc
		Finalidade: Manutenção de momentos das ações.
	*/

	/**
	 * Altera o momento de uma ação.
	 * 
	 * @param integer $acaid
	 * @param integer $mtaid
	 * @return boolean
	 */
	function atualiza_momento( $acaid, $mtaid )
	{
		global $db;
		$acaid = (integer) $acaid;
		$mtaid = (integer) $mtaid;
		$sql = 'update elabrev.ppaacao_orcamento set mtaid = ' . $mtaid . ' where acaid = ' . $acaid;
		return (boolean) $db->executar( $sql );
	}

	// manipula envio de formulário
	$unidades_abertas = array();
	$unidades_posicoes = array();
	if ( isset( $_REQUEST['formulario'] ) )
	{
		$sucesso = true;
		$momentos = $_REQUEST['momento'] ? $_REQUEST['momento'] : array() ;
		foreach ( $momentos as $acaid => $mtaid )
		{
			if ( !atualiza_momento( $acaid, $mtaid ) )
			{
				$sucesso = false;
				break;
			}
		}
		if ( $sucesso )
		{
			$db->commit();
		}
		$unidades_abertas = explode( ' ', trim( $_REQUEST['unidades_abertas'] ) );
	}
	// FIM manipula envio de formulário

	// define filtro
	// os filtros são colocados na sessão para que o arquivo momentosub também utilize
	$filtro_sql_unidade = '';
	$_SESSION['momento'] = array();
	if ( isset( $_REQUEST['filtro'] ) )
	{
		if ( count( $_REQUEST['filtro_programa'] ) && !in_array( 'x', $_REQUEST['filtro_programa'] ) )
		{
			$cods = " '" . implode( "','", $_REQUEST['filtro_programa'] ) . "' ";
			$filtro_sql_unidade .= ' a.prgcod in ( ' . $cods . ' ) and ';
			$filtro_programa = $_REQUEST['filtro_programa'];
			$_SESSION['momento']['programa'] = $_REQUEST['filtro_programa'];
		}
		if ( count( $_REQUEST['filtro_detalhamento'] ) && !in_array( 'x', $_REQUEST['filtro_detalhamento'] ) )
		{
			$cods = " '" . implode( "','", $_REQUEST['filtro_detalhamento'] ) . "' ";
			$filtro_sql_unidade .= ' a.acaid in ( select acaid from elabrev.tipodetalhamentoacao where tpdid in ( ' . $cods . ' ) ) and ';
			$filtro_detalhamento = $_REQUEST['filtro_detalhamento'];
			$_SESSION['momento']['detalhamento'] = $_REQUEST['filtro_detalhamento'];
		}
		if ( count( $_REQUEST['filtro_unidade'] ) && !in_array( 'x', $_REQUEST['filtro_unidade'] ) )
		{
			$cods = " '" . implode( "','", $_REQUEST['filtro_unidade'] ) . "' ";
			$filtro_sql_unidade .= ' a.unicod in ( ' . $cods . ' ) and ';
			$filtro_unidade = $_REQUEST['filtro_unidade'];
		}
	}
	if ( !$filtro_programa )
	{
		$filtro_programa = array( 'x' ); // seleciona opção 'todos'
	}
	if ( !$filtro_detalhamento )
	{
		$filtro_detalhamento = array( 'x' ); // seleciona opção 'todos'
	}
	if ( !$filtro_unidade )
	{
		$filtro_unidade = array( 'x' ); // seleciona opção 'todos'
	}
	// FIM define filtro

	// captura as unidades
	$join_responsabilidade = $db->usuarioJoinUnidadesPermitidas();
	$unicods_permitidos = $db->usuarioUnidadesPermitidas();
	if ( count( $unicods_permitidos ) )
	{
		$unicods_permitidos = "'" . implode( "','", $unicods_permitidos ) . "'";
	}
	else
	{
		$unicods_permitidos = ' null ';
	}
	$sql_unidade =
		" select " .
			" u.unicod as codigo, " .
			" u.unidsc as descricao " .
		" from unidade u " .
		" where " .
			" u.unistatus = 'A' and " .
			" u.unicod in ( " . $unicods_permitidos . " ) and " .
			" u.unicod in ( " .
				" select unicod " .
				" from elabrev.ppaacao_orcamento a " .
				" where " .
					$filtro_sql_unidade .
					" a.acasnrap = 'f' and " .
					" a.acastatus = 'A' and " .
					" a.prgano = '" . $_SESSION['exercicio'] . "' " .
			" ) " .
		" order by u.unicod, u.unidsc asc ";
	$recurso_unidade = $db->record_set( $sql_unidade );
	$nlinhas = $db->conta_linhas( $recurso_unidade );
	// FIM captura as unidades

	// captura momentos existentes
	$sql_momento =
		" select mtaid, mtadsc " .
		" from elabrev.momentoacao " .
		" where mtastatus = 'A' " .
		" order by mtadsc ";
	$momentos = $db->carregar( $sql_momento );
	if ( !$momentos )
	{
		$momentos = array();
	}
	$total_momentos = count( $momentos );
	// FIM captura momentos existentes

	include  APPRAIZ . "includes/cabecalho.inc";
	print '<br/>';
	monta_titulo( $titulo_modulo, 'Para detalhar as despesas selecione uma Ação na árvore abaixo - Total de Unidades (' . ( $nlinhas + 1 ) . ')<br>Para ver os Dados Orçamentarios, clique no código da Unidade.' );

?>
<form name="formulario" method="post">
	<input type="hidden" name="filtro" value="1">
	<table width="95%" border="0" cellspacing="0" cellpadding="2" align="center" bgcolor="#f7f7f7" style="border-top: 1px solid #c0c0c0;">
		<tr>
			<td class="" width="38%" align="center">
				<strong>Tipo de Detalhamento</strong><br/>
				<? $sql_filtro_detalhamento = "select tpdid as codigo, tpdcod || ' ' || tpddsc as descricao from elabrev.tipodetalhamento inner join elabrev.tipodetalhamentoacao using ( tpdid ) inner join elabrev.ppaacao_orcamento using ( acaid ) where unicod in ( " . $unicods_permitidos . " ) and tpdstatus = 'A' and tpdano = '" . $_SESSION['exercicio'] . "' group by tpdid, tpdcod, tpddsc order by tpdcod"; ?>
				<?= $db->monta_combo_multiplo( 'filtro_detalhamento', $sql_filtro_detalhamento, 'S', '', '', '-- Todos --', '', 5 ); ?>
			</td>
			<td class="" width="52%" align="center">
				<strong>Unidade Orçamentária</strong><br/>
				<? $sql_filtro_unidade = "select unicod as codigo, unicod || ' ' || unidsc as descricao from unidade where unicod in ( " . $unicods_permitidos . " ) order by unicod, unidsc"; ?>
				<?= $db->monta_combo_multiplo( 'filtro_unidade', $sql_filtro_unidade, 'S', '', '', '-- Todas --', '', 5 ); ?>
			</td>
			<td class="" width="10%" align="center">
				<input type="submit" name="filtrar" value="Filtrar" />
			</td>
		</tr>
	</table>
</form>
<? if ( $nlinhas >= 0 ) : ?>
	<script type="text/javascript">
		/**
		 * O índice indica a linha da tabela de listagem e o valor o código da
		 * unidade. Variável utilizada para botão de expandir todas ações das
		 * unidades e para marcar um momento para todas as unidades. Mais
		 * detalhes nos comentários das funções no fim deste arquivo.
		 *
		 * @var string[]
		 */
		var unidades = new Array();
	</script>
	<form action="" method="post">
		<input type="hidden" name="formulario" value="1"/>
		<input type="hidden" name="unidades_abertas" id="unidades_abertas" value=""/>
		<table width='95%' align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
			<thead>
				<tr>
					<td style="width:65px;" nowrap class="title"><strong>Código</strong></td>
					<td width="100%" nowrap class="title" style="padding:2px;">
						<span onclick="abrirFecharTodasAcoes( false );">
							<img src="../imagens/mais.gif" name="+" border="0" id="imgTodas"/>
						</span>
						<strong>Unidade Orçamentária</strong>
					</td>
					<? foreach ( $momentos as $momento ) : ?>
						<td style="width:55px;" nowrap class="title" align="center">
							<span style="cursor:pointer;" onclick="abrirTodasAcoes( '<?= $momento['mtaid'] ?>' );">
								<strong><?= $momento['mtadsc'] ?></strong><br/>marcar
							</span>
						</td>
					<? endforeach; ?>
				</tr>
			</thead>
			<tbody>
				<? for ( $i = 0; $i <= $nlinhas; $i++ ) : ?>
					<?php extract( $db->carrega_registro( $recurso_unidade, $i ) ); ?>
					<?php $unidades_posicoes[$codigo] = $i ?>
					<?php $marcado = fmod( $i, 2 ) == 0 ? '' : '#F7F7F7'; ?>
					<tr bgcolor="<?= $marcado ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?= $marcado ?>';" >
						<td valign="top" align="right">
							<a href="elabrev.php?modulo=principal/unidade&acao=A&unicod=<?= $codigo; ?>"><?= $codigo; ?><a/>
						</td>
						<td colspan="<?= $total_momentos + 1 ?>" onclick="abrirAcoes( '<?= $codigo ?>', '<?= $i ?>-<?= $codigo ?>', false );">
							<img src="../imagens/mais.gif" name="+" border="0" id="img<?= $i ?>-<?= $codigo ?>"/>
							<?= $descricao ?>
							<script type="text/javascript">
								unidades[<?= $i ?>] = '<?= $codigo ?>';
							</script>
						</td>
					</tr>
					<tr bgcolor="<?= $marcado ?>">
						<td id="td<?= $i ?>-<?= $codigo ?>" colspan="<?= $total_momentos + 2 ?>" style="padding-left:65px; padding-right:0;"></td>
					</tr>
				<? endfor; ?>
				<? $marcado = fmod( $i, 2 ) == 0 ? '' : 'marcado'; ?>
			    <tr class="<?= $marcado ?>">
					<td>&nbsp;</td>
					<td valign="top" class="title" colspan="<?= $total_momentos + 2 ?>"> 
						<strong>Total de Unidades: <?= $i ?></strong>
					</td>
				</tr>
			</tbody>
		</table>
		<p style="text-align: center;">
			<input type="submit" name="Enviar" />
		</p>
	</form>
<? else : ?>
	<table width="95%" border="0" cellspacing="0" cellpadding="2" align="center" bgcolor="#f7f7f7" style="border-top: 1px solid #c0c0c0;">
		<tr>
			<td align="center">Não foram encontrados registros</td>
		</tr>
	</table>
<? endif; ?>
<script type="text/javascript">

	/**
	 * Mostra subitens da unidade. O método também controla o campo para indicar
	 * quais unidades estão com os subitens abertos. Para eveitar alteração do
	 * momento pelo terceiro parâmetro basta enviar o valor falso.
	 *
	 * @param string
	 * @param string
	 * @param string indica qual o momento a ser setado para todas as ações
	 * @return void
	 */
	function abrirAcoes( unicod, id, mtaid )
	{
		var campo_unidades = document.getElementById( 'unidades_abertas' );
		if ( campo_unidades.value.indexOf( unicod ) !== -1 )
		{
			campo_unidades.value = campo_unidades.value.replace( unicod, '' );
		}
		else
		{
			campo_unidades.value += ' ' + unicod;
		}
		var momento = mtaid ? '&momento=' + mtaid : '' ;
		abreconteudo( 'geral/momentosub.php?unicod=' + unicod + momento, id );
	}
	<? foreach ( $unidades_abertas as $codigo ) : ?>
		<? if ( $codigo ) : ?>
			abrirAcoes( '<?= $codigo ?>','<?= $unidades_posicoes[$codigo] ?>-<?= $codigo ?>', false );
		<? endif; ?>
	<? endforeach; ?>

	/**
	 * Similar à função abrirAcoes(), sendo que esta abre ou fecha as açoes de
	 * todas as unidades. Essa função utiliza a variável global 'unidades'.
	 * 
	 * @return void
	 */
	function abrirFecharTodasAcoes()
	{
		var img = document.getElementById( 'imgTodas' );
		if ( img.name == '-' )
		{
			img.name = '+';
			img.src = img.src.replace( 'menos.gif', 'mais.gif' );
		}
		else
		{
			img.name = '-';
			img.src = img.src.replace( 'mais.gif', 'menos.gif' );
		}
		var j = unidades.length;
		for ( var i = 0; i < j; i++ )
		{
			img = document.getElementById( 'img' + i + '-' + unidades[i] );
			if ( img.name == '+' )
			{
				abrirAcoes( unidades[i], i + '-' + unidades[i], false );
			}
			else if ( mtaid )
			{
				selecionarMomentoParaUnidade( unidades[i], false );
			}
		}
	}
	
	/**
	 * Abre as ações de todas as unidades. Para eveitar alteração do momento
	 * pelo terceiro parâmetro basta enviar o valor falso.
	 * 
	 * @param string indica qual o momento a ser setado para todas as ações
	 * @return void
	 */
	function abrirTodasAcoes( mtaid )
	{
		var fechado = document.getElementById( 'imgTodas' ).name == '+';
		var j = unidades.length;
		for ( var i = 0; i < j; i++ )
		{
			if ( fechado )
			{
				abrirAcoes( unidades[i], i + '-' + unidades[i], mtaid )
			}
			else
			{
				selecionarMomentoParaUnidade( unidades[i], mtaid );
			}
		}
		document.getElementById( 'imgTodas' ).name = '-';
	}

	/**
	 * Seleciona todos os botões da unidade (primeiro parametro) para a opção
	 * do momento indicado pelo segundo parametro.
	 *
	 * @param string
	 * @param string
	 * @return void
	 */
	function selecionarMomentoParaUnidade( unicod, mtaid )
	{
		var i = 0;
		var id_base= 'radio_momento_' + unicod + '_' + mtaid;
		var input = null;
		while( input = document.getElementById( id_base + '_' + i++ ) )
		{
			input.checked = true;
		}
	}

</script>