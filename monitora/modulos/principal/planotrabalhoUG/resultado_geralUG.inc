<?php
ini_set( "memory_limit", "1024M" ); // ...
set_time_limit(0);

include 'funcoes_consulta_financeiroUG.inc'; 
$consulta = new ConsultaFinanceiro(); 

// parâmetros gerais
$consulta->alterarTitulo( $_REQUEST['titulo'] );
$consulta->alterarEscala( $_REQUEST['escala'] );
if ( is_array( $_REQUEST['agrupador'] ) == true )
{
	foreach ( $_REQUEST['agrupador'] as $agrupador )
	{
		$consulta->adicionarAgrupador( $agrupador );
	}
}
// FIM parâmetros gerais

// adiciona filtros
function adicionarFiltro( $campo )
{
	global $consulta;
	if ( $_REQUEST[$campo . '_campo_flag'] && $_REQUEST[$campo][0] != '' )
	{
		$excludente = (boolean) $_REQUEST[$campo . '_campo_excludente'];
		$consulta->adicionarFiltro( $campo, $_REQUEST[$campo], $excludente );
	}
}
adicionarFiltro( 'programa' );
adicionarFiltro( 'acacod' );
adicionarFiltro( 'localizador' );
adicionarFiltro( 'fonte' );
adicionarFiltro( 'fontesiafi' );
adicionarFiltro( 'fontesof' );
adicionarFiltro( 'gnd' );
adicionarFiltro( 'natureza' );
adicionarFiltro( 'usopri' );
adicionarFiltro( 'mapcod' );
adicionarFiltro( 'grf' );
adicionarFiltro( 'iducod' );
adicionarFiltro( 'catecon' );
adicionarFiltro( 'elemento' );
adicionarFiltro( 'esfera' );
adicionarFiltro( 'plicod' );
adicionarFiltro( 'sbacod' );
$consulta->adicionarFiltro( 'ano', $_REQUEST['ano'] );
$consulta->adicionarFiltro( 'ug', $_REQUEST['ug'] );
// FIM adiciona filtros

// salva os POST na tabela
if ( $_REQUEST['salvar'] )
{
	$existe_rel = 0;
	$sql = sprintf(
		"select prtid from public.parametros_tela where prtdsc = '%s'",
		$_REQUEST['titulo']
	);
	$existe_rel = $db->pegaUm( $sql );
	if ($existe_rel > 0) 
	{
		/*
		?>
		<script language="text/javascript">
			if ( !confirm( 'Deseja alterar a consulta já existente?' ) )
			{
				history.back();
			}
		</script>
		<?
		*/
		$sql = sprintf(
			"UPDATE public.parametros_tela SET prtdsc = '%s', prtobj = '%s', prtpublico = 'FALSE', usucpf = '%s', mnuid = %d WHERE prtid = %d",
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
			"INSERT INTO public.parametros_tela ( prtdsc, prtobj, prtpublico, usucpf, mnuid ) VALUES ( '%s', '%s', %s, '%s', %d )",
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
		location.href = '?modulo=<?= $modulo ?>&acao=R';
	</script>
	<?
}
// FIM salva os POST na tabela

// mostra relatório em XLS
if ( $_REQUEST['tipoRelatorio'] == 'xls' )
{
	$qtdAgrupadores = count( $consulta->pegarAgrupadores() ) * 2;
	$tipo_campo = array();
	while( $qtdAgrupadores > 0 )
	{
		array_push( $tipo_campo, 's' );
		$qtdAgrupadores--;
	}
	for( $i = 0; $i < 10; $i++ )
	{
		array_push( $tipo_campo, 'n' );
	}
	header( 'Content-type: application/xls' );
	header( 'Content-Disposition: attachment; filename="planilha_simec.xls"' );
	$db->sql_to_excel( $consulta->montaRequisicao(), 'relorc', '', $tipo_campo );
	exit();
}
// FIM mostra relatório em XLS



function cabecalhoBrasao()
{
	global $db;
	global $consulta;
	?>
	<table width="100%" border="0" cellpadding="0" cellspacing="0" class="notscreen1 debug"  style="border-bottom: 1px solid;">
		<tr bgcolor="#ffffff">
			<td valign="top" width="50" rowspan="2"><img src="../imagens/brasao.gif" width="45" height="45" border="0"></td>
			<td nowrap align="left" valign="middle" height="1" style="padding:5px 0 0 0;">
				<?php echo NOME_SISTEMA; ?><br/>
				Acompanhamento da Execução Orçamentária<br/>
				MEC / SE - Secretaria Executiva <br />
				SPO - Subsecretaria de Planejamento e Orçamento
			</td>
			<td align="right" valign="middle" height="1" style="padding:5px 0 0 0;">
				Impresso por: <b><?= $_SESSION['usunome'] ?></b><br/>
				Hora da Impressão: <?= date( 'd/m/Y - H:i:s' ) ?><br />
				Orçamento Fiscal e Seg.Social - EM R$ <?= number_format( $consulta->pegarEscala(), 2, ',', '.' ) ?><br />
				<? $whereAcumuladoAte = ''; ?>
				<? $valoresFiltroAno = $consulta->pegarValoresFiltro( 'ano' ); ?>
				<? if ( count( $valoresFiltroAno ) > 0 ) : ?>
					<? $whereAcumuladoAte = " where rofano in ( '" . implode( "','", $valoresFiltroAno ) . "' ) "; ?>
				<? endif; ?>
				<? $sqlAcumuladoAte = "select max( rofdata ) as data from financeiro.execucao " . $whereAcumuladoAte; ?>
				Acumulado até: <?= formata_data( $db->pegaUm( $sqlAcumuladoAte ) ) ?>
			</td>
		</tr>
		<tr>
			<td colspan="2" align="center" valign="top" style="padding:0 0 5px 0;">
				<b><font style="font-size:14px;"><?= $consulta->pegarTitulo() ?></font></b>
			</td>
		</tr>
	</table>
	<?
}

?>	
<style type="text/css">
	
	@media print {.notprint { display: none }}

	@media screen {
	.notscreen { display: none;  }
	.div_rolagem{ overflow-x: auto; overflow-y: auto; width:35.5cm;height:350px;}
	.topo { position: absolute; top: 0px; margin: 0; padding: 5px; position: fixed; background-color: #ffffff;}
	}

</style>
		<?php ob_flush();flush(); ?>
		<?php
		// realiza consulta, mantem dados na sessão para as imagens
		$itens = $consulta->consultar();
		$_SESSION['consulta_financeira'] = array();
		$_SESSION['consulta_financeira']['itens'] = $itens['itens'];
		$_SESSION['consulta_financeira']['agrupadores'] = $consulta->pegarAgrupadores( true );
		
		if ( !$_REQUEST['nao_mostra_filtro_impressao'] ) : ?>
			<div id="filtros" class="notscreen filtro">
				<b><font style="font-size:12px;">Filtros</font></b>
				<?
				
				function mostraFiltro( $campo )
				{
					global $consulta;
					global $db;
					$oConsultaFinanceiroTraducao = new ConsultaFinanceiroTraducao();
					$nomeCampoCod = $oConsultaFinanceiroTraducao->pegarAliasCodigo( $campo );
					$nomeCampoDsc = $oConsultaFinanceiroTraducao->pegarAliasDescricao( $campo );
					// trata comportamento diferente da natureza de despesa
					if ( $campo == 'natureza' )
					{
						$nomeCampoCod = "cast(ctecod as varchar) || cast(gndcod as varchar) || mapcod || edpcod";
					}
					$titulo = $oConsultaFinanceiroTraducao->pegarTitulo( $campo );
					$tabela = $oConsultaFinanceiroTraducao->pegarTabela( $campo );
					$filtros = $consulta->pegarValoresFiltro( $campo );
					$excludente = $consulta->filtroExcludente( $campo ) ? ' (excludente) ' : '';
					if ( count( $filtros ) > 0 )
					{
						print "<br/><br/><b>" . $titulo . "</b>" . $excludente . "<br/>";
						$sqlFiltro = "select " . $nomeCampoCod . " as codigo, " . $nomeCampoDsc . " as descricao from " . $tabela . " where " . $nomeCampoCod . " in ( '" . implode( "','", $filtros ) . "' ) group by codigo, descricao order by codigo, descricao";
						foreach ( $db->carregar( $sqlFiltro ) as $itemFiltro )
						{
							print $itemFiltro['codigo'] . " - " . $itemFiltro['descricao'] . "<br/>";
						}
					}
				}
				mostraFiltro( 'orgao' );
				mostraFiltro( 'grupouo' );
				//mostraFiltro( 'uo' );
				mostraFiltro( 'ug' );
				mostraFiltro( 'funcao' );
				mostraFiltro( 'subfuncao' );
				mostraFiltro( 'programa' );
				mostraFiltro( 'acacod' );
				mostraFiltro( 'esfera' );
				mostraFiltro( 'localizador' );
				mostraFiltro( 'grf' );
				mostraFiltro( 'fonte' );
				mostraFiltro( 'fontesiafi' );
				mostraFiltro( 'fontesof' );
				mostraFiltro( 'catecon' );
				mostraFiltro( 'gnd' );
				mostraFiltro( 'mapcod' );
				mostraFiltro( 'elemento' );
				mostraFiltro( 'natureza' );
				mostraFiltro( 'iducod' );
				mostraFiltro( 'usopri' );
				
				?>
			</div>
		<? endif; ?>
		<table class="listagem" width="100%" align="center" border="0" cellpadding="2" cellspacing="0">
				<tr>
					<th align="left">
						<table id="tabelaTitulos" align="left">
							<thead>
								<tr>
									<th width="560px" class="bold alignLeft"><?= $consulta->pegarTituloAgrupador() ?></th>
									<th width="150px" class="title" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" valign="top" align="" bgcolor="">Dotação Inicial<br/>(A)</th>
									<th width="150px" class="title" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" valign="top" align="" bgcolor="">Dotação Autorizada<br/>(B)</th>
									<th width="150px" class="title" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" valign="top" align="" bgcolor="">Valor Proposto<br/>(C)</th>
									<th width="150px" class="title" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" valign="top" align="" bgcolor="">Despesa Empenhada<br/>(D)</th>
									<th width="150px" class="title" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" valign="top" align="" bgcolor="">Despesa Liquidada<br/>(E)</th>
									<th width="150px" class="title" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" valign="top" align="" bgcolor="">Valores Pagos<br/>(F)</th>
									<th width="16px" class="title" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" valign="top" align="" bgcolor="">&nbsp;</th>
								</tr>
							</thead>
						</table>
					</th>
				</tr>
			<tbody>
				<tr>
					<td class="noPadding" align="left">
						<div class="div_rolagem">
							<?php
								$cfAgrupadores = $consulta->pegarAgrupadores();
								cfDesenhaResultado( $itens['itens'] );
							?>
						</div>
					</td>
				</tr>
			</tbody>
		</table>