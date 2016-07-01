<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<?php

include APPRAIZ . "includes/registraracesso.php";

print '<br/>';

$sql_prgid = "SELECT prgid FROM cte.programa WHERE prgdsc = 'OUTROS'";
$prgid_outros = $db->pegaUm($sql_prgid);


$ptostatus = isset( $_REQUEST['ptostatus'] ) ? $_REQUEST['ptostatus'] : 'A';

$ssuid = (integer) $_REQUEST['ssuid'];

if ( $ssuid != 0 )
{
	if ( $ssuid == -1 )
	{
		$where = " and ss.ssuid is null ";
	}
	else
	{
		$where = " and ss.ssuid = " . $ssuid . " ";
	}
}

$sql = "
		select
			d.dimid,
			d.dimcod || ' - ' || d.dimdsc as dimensao,
			area.ardcod || ' - ' || area.arddsc as area,
			area.ardid,			
			i.indcod || ' - ' || i.inddsc as indicador,
			i.indid,
			i.indcod,
			c.ctrpontuacao as pontuacao,
			p.ptojustificativa,
			p.ptodemandamunicipal,
			p.ptodemandaestadual,
			Case acao.acilocalizador
				when 'E' then 'Estadual'
				when 'M' then 'Municipal'
				end as Tipo,
			acao.aciid,
			acao.ptoid,
			acao.acidsc,
			acao.acirpns,
			acao.acicrg,
			to_char(acao.acidtinicial,'dd/mm/yyyy') as acidtinicial,
			to_char(acao.acidtfinal,'dd/mm/yyyy') as acidtfinal,
			acao.acirstd,
			acao.acilocalizador,
			subacao.psuid,
			subacao.sbaparecer,
			subacao.sbaid,
			subacao.undid,
			subacao.frmid,
			subacao.foaid,
			subacao.sbadsc,
			subacao.sbastgmpl,
			subacao.sbaprm,
			subacao.sbapcr,
			coalesce(subacao.sba0ano, 0) as sba0ano,
			coalesce(subacao.sba1ano, 0) as sba1ano,
			coalesce(subacao.sba2ano, 0) as sba2ano,
			coalesce(subacao.sba3ano, 0) as sba3ano,
			coalesce(subacao.sba4ano, 0) as sba4ano,
			coalesce(subacao.sbaunt,0) as sbaunt,
			subacao.sbauntdsc,
			subacao.sba0ini,
			subacao.sba0fim,
			subacao.sba1ini,
			subacao.sba1fim,
			subacao.sba2ini,
			subacao.sba2fim,
			subacao.sba3ini,
			subacao.sba3fim,
			subacao.sba4ini,
			subacao.sba4fim,
			coalesce(u.unddsc,'') as unddsc,
			coalesce(f.frmdsc,'') as frmdsc,
			c.crtdsc,
			c.ctrpontuacao,
			f.frmid,
			ss.ssuid,
            prgid
		from
			cte.dimensao d
			inner join cte.areadimensao area ON area.dimid = d.dimid
			inner join cte.indicador i ON i.ardid = area.ardid
			inner join cte.pontuacao p ON p.indid = i.indid and p.ptostatus = '" . $ptostatus . "'
			inner join cte.criterio c ON c.crtid = p.crtid
			inner join cte.instrumentounidade iu ON iu.inuid = p.inuid
			inner join cte.acaoindicador acao ON acao.ptoid = p.ptoid
			inner join cte.subacaoindicador subacao ON subacao.aciid = acao.aciid
			inner join cte.unidademedida u on u.undid = subacao.undid
			inner join cte.formaexecucao f on f.frmid = subacao.frmid
			
			left join cte.statussubacao ss on ss.ssuid = subacao.ssuid
			
		where
			iu.inuid = '" . ( (integer) $_SESSION['inuid'] ) . "'
			" . $where . "
		order by
			d.dimcod,
			area.ardcod,
			i.indcod, subacao.oid
--        limit 2
";

//dbg($sql,1 );
$dado = $db->carregar($sql);
$i=0;

$totalreg = count($dado);

$novaDimensao = $dado[$i]['dimid'];
$novaArea = $dado[$i]['ardid'];
$novoIndicador = $dado[$i]['indid'];
$novaAcao = $dado[$i]['aciid'];
$novasubAcao = $dado[$i]['sbaid'];

$totalGeralAno0 = 0;
$totalGeralAno1 = 0;
$totalGeralAno2 = 0;
$totalGeralAno3 = 0;
$totalGeralAno4 = 0;


$sqlStatusSubacao = "select * from cte.statussubacao";
$statusSubacao = $db->carregar( $sqlStatusSubacao );
$statusSubacao = $statusSubacao ? $statusSubacao : array();

$visao = $_REQUEST['visao'] == 'analitico' ? 'analitico' : 'sintetico';

$exibirDados = $visao == 'analitico';

$sql =
	"select ps.psuid, ps.psudescricao, gp.gpsid, gp.gpsdescricao
	from cte.parecersubacao ps
	inner join cte.grupoparecersubacao gp on gp.gpsid = ps.gpsid
	order by gp.gpsdescricao, ps.psudescricao";
$pareceresPadroes = $db->carregar( $sql );
$pareceresPadroes = $pareceresPadroes ? $pareceresPadroes : array();

$itrid = cte_pegarItrid( $_SESSION['inuid'] );
$capturaTipo = $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ? "E" : "M";

?>

<script type="text/javascript">

function desformataValor( entrada )
{
	var saida = parseInt( trim( entrada.replace( /[\.,]/g , "" ) ) ) - 0;
	if ( entrada.indexOf( ',' ) > -1 )
	{
		saida /= 100;
	}
	return saida;
}

function formataValor( entrada )
{
	return MascaraMonetario( entrada + '' );
}

function atualizaTotal( sbaid )
{
	var qtdAno0  = document.getElementById( 'sba0ano_' + sbaid ).value - 0;
	var qtdAno1  = document.getElementById( 'sba1ano_' + sbaid ).value - 0;
	var qtdAno2  = document.getElementById( 'sba2ano_' + sbaid ).value - 0;
	var qtdAno3  = document.getElementById( 'sba3ano_' + sbaid ).value - 0;
	var qtdAno4  = document.getElementById( 'sba4ano_' + sbaid ).value - 0;
	var qtdTotal = qtdAno0 + qtdAno1 + qtdAno2 + qtdAno3 + qtdAno4;
	var vlrUnitario = desformataValor( document.getElementById( 'sbaunt_' + sbaid ).value );
	var vlrAno0     = qtdAno0 * vlrUnitario;
	var vlrAno1     = qtdAno1 * vlrUnitario;
	var vlrAno2     = qtdAno2 * vlrUnitario;
	var vlrAno3     = qtdAno3 * vlrUnitario;
	var vlrAno4     = qtdAno4 * vlrUnitario;
	var vlrTotal    = vlrAno0 + vlrAno1 + vlrAno2 + vlrAno3 + vlrAno4;
	
	document.getElementById( 'fisicoTotal_' + sbaid ).innerHTML    = qtdTotal;
	document.getElementById( 'valorTotalAno0_' + sbaid ).innerHTML = formataValor( vlrAno0 );
	document.getElementById( 'valorTotalAno1_' + sbaid ).innerHTML = formataValor( vlrAno1 );
	document.getElementById( 'valorTotalAno2_' + sbaid ).innerHTML = formataValor( vlrAno2 );
	document.getElementById( 'valorTotalAno3_' + sbaid ).innerHTML = formataValor( vlrAno3 );
	document.getElementById( 'valorTotalAno4_' + sbaid ).innerHTML = formataValor( vlrAno4 );
	document.getElementById( 'valorTotal_' + sbaid ).innerHTML     = formataValor( vlrTotal );
}

function alterarStatusSubacao( sbaid )
{
	sbaid -= 0;
	
	psuid      = document.getElementById( 'psuid_' + sbaid ).value;
	sbaparecer = document.getElementById( 'sbaparecer_' + sbaid ).value;
	ssuid      = document.getElementById( 'ssuid_' + sbaid ).value;
	
	possuiExtras = !!document.getElementById( 'sbastgmpl_' + sbaid );
	if ( possuiExtras )
	{
		sbastgmpl  = document.getElementById( 'sbastgmpl_' + sbaid ).value;
		sbaprm     = document.getElementById( 'sbaprm_'    + sbaid ).value;
		undid      = document.getElementById( 'undid_'     + sbaid ).value;
		frmid      = document.getElementById( 'frmid_'     + sbaid ).value;
		foaid      = document.getElementById( 'foaid_'     + sbaid ).value;
		sbapcr     = document.getElementById( 'sbapcr_'    + sbaid ).value;
		sba0ano    = document.getElementById( 'sba0ano_'   + sbaid ).value;
		sba1ano    = document.getElementById( 'sba1ano_'   + sbaid ).value;
		sba2ano    = document.getElementById( 'sba2ano_'   + sbaid ).value;
		sba3ano    = document.getElementById( 'sba3ano_'   + sbaid ).value;
		sba4ano    = document.getElementById( 'sba4ano_'   + sbaid ).value;
		sba0ini    = document.getElementById( 'sba0ini_'   + sbaid ).value;
		sba1ini    = document.getElementById( 'sba1ini_'   + sbaid ).value;
		sba2ini    = document.getElementById( 'sba2ini_'   + sbaid ).value;
		sba3ini    = document.getElementById( 'sba3ini_'   + sbaid ).value;
		sba4ini    = document.getElementById( 'sba4ini_'   + sbaid ).value;
		sba0fim    = document.getElementById( 'sba0fim_'   + sbaid ).value;
		sba1fim    = document.getElementById( 'sba1fim_'   + sbaid ).value;
		sba2fim    = document.getElementById( 'sba2fim_'   + sbaid ).value;
		sba3fim    = document.getElementById( 'sba3fim_'   + sbaid ).value;
		sba4fim    = document.getElementById( 'sba4fim_'   + sbaid ).value;
		sbaunt     = document.getElementById( 'sbaunt_'    + sbaid ).value;
		sbauntdsc  = document.getElementById( 'sbauntdsc_' + sbaid ).value;
		prgid      = document.getElementById( 'prgid_'     + sbaid ).value;
	}
	
	var request =  window.XMLHttpRequest ? new XMLHttpRequest : new window.ActiveXObject( 'Microsoft.XmlHttp' );
	request.open( 'POST', '../cte/ajax.php', true );
	request.setRequestHeader( "Content-Type", "application/x-www-form-urlencoded; charset=iso-8859-1" );
	request.onreadystatechange = function()
	{
		if ( request.readyState == 4 )
		{
			var resposta = trim( request.responseText );
			if ( resposta == 'sucesso' )
			{
				alert( 'Alteração do parecer da subação realizada com sucesso.' );
			}
			else
			{
				alert( 'Falha ao alterar alterar o parecer da subação.' );
			}
		}
	}
	
	psuid = escape( psuid );
	sbaparecer = escape( sbaparecer );
	ssuid = escape( ssuid );
	
	requisicaoExtra = '';
	if ( possuiExtras )
	{
		requisicaoExtra = '' +
			'&sbastgmpl=' + escape( sbastgmpl ) +
			'&sbaprm='    + escape( sbaprm ) +
			'&undid='     + escape( undid ) +
			'&frmid='     + escape( frmid ) +
			'&foaid='     + escape( foaid ) +
			'&sbapcr='    + escape( sbapcr ) +
			'&sba0ano='   + escape( sba0ano ) +
			'&sba1ano='   + escape( sba1ano ) +
			'&sba2ano='   + escape( sba2ano ) +
			'&sba3ano='   + escape( sba3ano ) +
			'&sba4ano='   + escape( sba4ano ) +
			'&sba0ini='   + escape( sba0ini ) +
			'&sba1ini='   + escape( sba1ini ) +
			'&sba2ini='   + escape( sba2ini ) +
			'&sba3ini='   + escape( sba3ini ) +
			'&sba4ini='   + escape( sba4ini ) +
			'&sba0fim='   + escape( sba0fim ) +
			'&sba1fim='   + escape( sba1fim ) +
			'&sba2fim='   + escape( sba2fim ) +
			'&sba3fim='   + escape( sba3fim ) +
			'&sba4fim='   + escape( sba4fim ) +
			'&sbaunt='    + escape( sbaunt )  +
			'&sbauntdsc=' + escape( sbauntdsc ) +
            '&prgid='     + escape( prgid );
	}
	
	request.send(
		'evento=alterar_status_subacao' +
		
		'&sbaid='      + escape( sbaid ) +
		'&psuid='      + escape( psuid ) +
		'&sbaparecer=' + escape( sbaparecer ) +
		'&ssuid='      + escape( ssuid ) +
		
		requisicaoExtra
	);
}



var unidadesMedida = new Array();
<?php
    $sql = "SELECT
                undid as codigo,
                unddsc as descricao
            FROM
                cte.unidademedida
            WHERE
                undtipo = '" . $capturaTipo . "'
            ORDER BY
                descricao";

    foreach ($db->carregar($sql) as $unidade) {
        echo "unidadesMedida.push(new Array('" .$unidade['codigo']. "', '" .addslashes(trim($unidade['descricao'])). "'));\n";
    }
?>
function carregarUnidadesMedida(container, sel)
{
    container = document.getElementById(container);

    if (container.options.length > 1) {
        return false;
    }

    while(container.options.length)
        container.options[0] = null;

    container.options[0] = new Option("Selecione", "0", true, false);

    container.setAttribute("disabled", "disabled");

    for (var i = 0; i < unidadesMedida.length; i++) {
        container.options[i + 1] = new Option(unidadesMedida[i][1],
                                              unidadesMedida[i][0],
                                              false,
                                              sel == unidadesMedida[i][0]);

        if (sel == unidadesMedida[i][0]) {
            container.options[i + 1].setAttribute('selected', 'selected');
        }
    }

    container.removeAttribute("disabled");
}


function exibeCampo(sbaidAtual)
{
    if (document.all)
        document.getElementById( 'div_sbaprm_' + sbaidAtual ).style.display = 'block';
    else
        document.getElementById( 'div_sbaprm_' + sbaidAtual ).style.display = 'table-row';

    if (document.getElementById( 'prgid_' + sbaidAtual ).value == <?=$prgid_outros; ?>) {
        if (document.all)
            document.getElementById( 'div_sbaprm_' + sbaidAtual ).style.display = 'block';
        else
            document.getElementById( 'div_sbaprm_' + sbaidAtual ).style.display = 'table-row';
    } else {
        document.getElementById('sbaprm_' + sbaidAtual).value = '';
        document.getElementById( 'div_sbaprm_' + sbaidAtual  ).style.display = 'none';
    }
}


var dadosPrograma = new Array();
<?php
    $sql = "SELECT prgid as codigo, substr(prgdsc, 1, 100) as descricao FROM cte.programa ORDER BY prgdsc ASC";
    foreach ($db->carregar($sql) as $unidade) {
        echo "dadosPrograma.push(new Array('" .$unidade['codigo']. "', '" .addslashes(trim($unidade['descricao'])). "'));\n";
    }
?>
function carregarDados(container, sel) {

    if (container.options.length > 1) {
        return false;
    }

    while(container.options.length)
        container.options[0] = null;

    container.options[0] = new Option("Selecione", "0", true, false);

    container.setAttribute("disabled", "disabled");

    for (var i = 0; i < dadosPrograma.length; i++) {
        container.options[i + 1] = new Option(dadosPrograma[i][1],
                                              dadosPrograma[i][0],
                                              false,
                                              sel == dadosPrograma[i][0]);

        if (sel == dadosPrograma[i][0]) {
            container.options[i + 1].setAttribute('selected', 'selected');
        }
    }

    container.removeAttribute("disabled");
}


function removerSubacao( sbaid, el )
{
	if ( !window.confirm( 'Deseja realmente excluir essa subação?' ) )
	{
		return;
	}
	
	var request =  window.XMLHttpRequest ? new XMLHttpRequest : new window.ActiveXObject( 'Microsoft.XmlHttp' );
	request.open( 'POST', '../cte/ajax.php', true );
	request.setRequestHeader( "Content-Type", "application/x-www-form-urlencoded; charset=iso-8859-1" );
	request.onreadystatechange = function()
	{
		if ( request.readyState == 4 )
		{
			if (trim(request.responseText) == 'sucesso')
			{
                var tr       = document.getElementById(el);
                var table    = tr.parentNode.parentNode;
                var id       = tr.rowIndex;
                tr           = null;

                table.deleteRow(id);
				alert('Sub-ação excluída com sucesso.');
			}
			else
			{
				alert( 'Falha ao remover a subação.' );
			}
		}
	}

	request.send(
		'evento=remover_subacao' +
		'&sbaid=' + sbaid
	);
}
</script>

<form action="" name="visao" method="post">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubTituloDireita" width="30%">
				<b>Visualização</b>
			</td>
			<td width="70%">
				<select name="visao">
					<option value="analitico" <?= $visao == 'analitico' ? 'selected="selected"' : '' ?>>Analítico</option>
					<option value="sintetico" <?= $visao == 'sintetico' ? 'selected="selected"' : '' ?>>Sintético</option>
				</select>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="30%">
				<b>Status</b>
			</td>
			<td width="70%">
				<select name="ssuid">
					<option value="">Todos</option>
					<option value="-1" <?= $ssuid == -1 ? ' selected="selected" ' : '' ?>>Sem status</option>
					<? foreach ( $statusSubacao as $status ) : ?>
						<option
							value="<?= $status['ssuid'] ?>"
							<?= $status['ssuid'] == $ssuid ? ' selected="selected" ' : '' ?>
						>
							<?= $status['ssudescricao'] ?>
						</option>
					<? endforeach; ?>
				</select>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="30%">
				&nbsp;
			</td>
			<td width="70%">
				<input type="submit" name="filtrar" value="Filtrar"/>
			</td>
		</tr>
	</table>
</form>

<form name="formularioFake">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<?php if( !isset( $cabecalhoImprecao ) || $cabecalhoImprecao !== false ): ?>		
<thead>
	<th colspan="2" align="left">
	<?
	$estuf = cte_pegarEstuf( $_SESSION['inuid'] );
	$sql = "select estdescricao from territorios.estado where estuf = '" . $estuf . "'";
	$estado = $db->pegaUm($sql);
	?>
	<h1 class="notprint">PAR do Estado:  <?=$estado?></h1>
	
		<table width="100%" bgcolor="#ffffff" border="0" cellpadding="0" cellspacing="0" class="notscreen" style="position: absolute; position: fixed;border-bottom: 1px solid;">
				<tr> 
					<td><img src="../imagens/brasao.gif" width="50" height="50" border="0"></td>
					<td height="20" nowrap>
						<b>SIMEC</b>- Sistema Integrado de Ministério da Educação<br>
						Ministério da Educação / SE - Secretaria Executiva<br>
						<b>.:: PAR Analítico do Estado:  <?=$estado?></b><br>	
					</td>
					<td height="20" align="right">
						Impresso por: <strong><?= $_SESSION['usunome']; ?></strong><br>
						Órgão: <?= $_SESSION['usuorgao']; ?><br>
						Hora da Impressão: <?= date("d/m/Y - H:i:s") ?>
					</td>
				</tr>
				<tr> 
					<td colspan="2">&nbsp;</td>
				</tr>
				</th>
			</table>
</thead>
<?php endif; ?>
<? 
while ( $i < $totalreg ) {

	$dimensao = $dado[$i]['dimid'];
		?>
					<tr>
						<td class="SubTituloDireita">
							<b>Dimensão</b>
						</td>
						<td class="" style="text-align:left">	
							<?=$dado[$i]['dimensao']; ?>
						</td>
					</tr>
					<?
					while ($dimensao == $novaDimensao)
					{
						if($i >= $totalreg ) break;

						$area = $dado[$i]['ardid'];

				?>
								<tr>
									<td class="SubTituloDireita">
										<b>Área</b>
									</td>
									<td class="" style="text-align:left">	
										<?=$dado[$i]['area']; ?>
									</td>
								</tr>
								
						
								<?

								while ($area == $novaArea )
								{
									if($i >= $totalreg ) break;

									$indicador = $dado[$i]['indid'];
									while ($indicador == $novoIndicador )
									{
										if($i >= $totalreg ) break;
										//print 'Indicador'.$indicador.'<br>';
										//print 'IndicadorNOVO'.$novoIndicador.'<br>';

												?>
													<tr>
														<td class="SubTituloDireita">
															<b>Indicador</b>
														</td>
														<td class="" style="text-align:left">	
															<?=$dado[$i]['indicador']; ?>
														</td>
													</tr>
													<tr>
														<td class="SubTituloDireita">
															<b>Critério / Pontuação</b>
														</td>
														<td class="" style="text-align:left">	
															<?echo $dado[$i]['ctrpontuacao'] . ' - '.$dado[$i]['crtdsc']; ?>
														</td>
													</tr>
													<? if ( $exibirDados ) : ?>
														<tr>
															<td class="SubTituloDireita">
																<b>Justificativa</b>
															</td>
															<td class="" style="text-align:left">	
																<?echo $dado[$i]['ptojustificativa']; ?>
															</td>
														</tr>
														<?if( $dado[$i]['ptodemandaestadual']){?>
														<tr>
															<td class="SubTituloDireita">
																<b>Demanda para Rede Estadual</b>
															</td>
															<td class="" style="text-align:left">	
																<?echo $dado[$i]['ptodemandaestadual']; ?>
															</td>
														</tr>
														<?}?>
														<?if($dado[$i]['ptodemandamunicipal']){?>
														<tr>
															<td class="SubTituloDireita">
																<b>Demanda para Redes Municipais</b>
															</td>
															<td class="" style="text-align:left">	
																<?echo $dado[$i]['ptodemandamunicipal']; ?>
															</td>
														</tr>
														<?}?>
													<? endif; ?>
													<?

													$acao = $dado[$i]['aciid'];
													while ($acao == $novaAcao)
													{
														if($i >= $totalreg ) break;
														//print 'AçãoFORA'.$acao.'<br>';
														//print 'NovaAçãoFORA'.$novaAcao.'<br><p>';
											?>
																<tr>
																	<td class="SubTituloDireita">
																		<b>Ação</b>
																	</td>
																	<td class="" style="text-align:left">
																		<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="left" >
																			<tr>
																				<td  class="SubTituloDireita" width="30%">
																					Demanda:
																				</td>
																				
																				<td width="70%">
																					<?=$dado[$i]['tipo'];?>
																				</td>
																			</tr>	
																			<tr>
																				<td  class="SubTituloDireita">
																					Descrição da Ação:
																				</td>
																				
																				<td>
																					<?=$dado[$i]['acidsc']; ?>
																				</td>
																			</tr>
																			<? if ( $exibirDados ) : ?>
																				<tr>
																					<td  class="SubTituloDireita">
																						Nome do Responsável:
																					</td>
																					
																					<td>
																						<?=$dado[$i]['acirpns']; ?>
																					</td>
																				</tr>	
																				<tr>
																					<td  class="SubTituloDireita">
																						Cargo do Responsável:
																					</td>
																					
																					<td>
																						<?=$dado[$i]['acicrg']; ?>
																					</td>
																				</tr>			 
																				<tr>
																					<td  class="SubTituloDireita">
																						Período Inicial:
																					</td>
																					
																					<td>
																						<?=$dado[$i]['acidtinicial']; ?>
																					</td>
																				</tr>
																				<tr>
																					<td  class="SubTituloDireita">
																						Período Final:
																					</td>
																					
																					<td>
																						<?=$dado[$i]['acidtfinal']; ?>
																					</td>
																				</tr>
																				<tr>
																					<td  class="SubTituloDireita">
																						Resultado Esperado:
																					</td>
																					
																					<td>
																						<?=$dado[$i]['acirstd']; ?>
																					</td>
																				</tr>
																			<?php endif; ?>
																		</table>	
																	</td>
																</tr>			
																<?
																$subacao = $dado[$i]['sbaid'];
																while ($acao == $novaAcao )
																{
																	mostra_subacao();
																	if($i >= $totalreg ) break;
																}
													}}

														?>	
												<tr>
													<td class="SubTituloDireita" >
													<b>Total Geral por Indicador</b>
													</td>
													<td>			
														<table class="listagem" width="100%">
															<thead>
																<th align="center">
																	<b>2007</b>
																</th>
																<th align="center">
																	<b>2008</b>
																</th>
																<th align="center">
																	<b>2009</b>
																</th>
																<th align="center">
																	<b>2010</b>
																</th>
																<th align="center">
																	<b>2011</b>
																</th>
																<th align="center">
																	<b>Total</b>
																</th>
															</thead>
															<tr>
																<td align="right">
																	<?=number_format($totalGeralIndicadorAno0,2,',','.');?>
																</td>
																<td align="right">
																	<?=number_format($totalGeralIndicadorAno1,2,',','.');?>
																</td>
																<td align="right">
																	<?=number_format($totalGeralIndicadorAno2,2,',','.');?>
																</td>
																<td align="right">
																	<?=number_format($totalGeralIndicadorAno3,2,',','.');?>
																</td>
																<td align="right">
																	<?=number_format($totalGeralIndicadorAno4,2,',','.');?>
																</td>
																<td align="right">
																	<?=number_format($totalGeralIndicadorAno0 + $totalGeralIndicadorAno1 + $totalGeralIndicadorAno2 + $totalGeralIndicadorAno3 + $totalGeralIndicadorAno4 ,2,',','.');?>
																</td>
															</tr>
														</table>
													</td>
												</tr>
												<?
												$totalGeralIndicadorAno0 = 0; 
												$totalGeralIndicadorAno1 = 0;
												$totalGeralIndicadorAno2 = 0;
												$totalGeralIndicadorAno3 = 0;
												$totalGeralIndicadorAno4 = 0;

								}
							?>
						<tr>
							<td class="SubTituloDireita" >
							<b>Total Geral por Área</b>
							</td>
							<td>			
								<table class="listagem" width="100%">
									<thead>
										<th align="center">
											<b>2007</b>
										</th>
										<th align="center">
											<b>2008</b>
										</th>
										<th align="center">
											<b>2009</b>
										</th>
										<th align="center">
											<b>2010</b>
										</th>
										<th align="center">
											<b>2011</b>
										</th>
										<th align="center">
											<b>Total</b>
										</th>
									</thead>
									<tr>
										<td align="right">
											<?=number_format($totalGeralAreaAno0,2,',','.');?>
										</td>
										<td align="right">
											<?=number_format($totalGeralAreaAno1,2,',','.');?>
										</td>
										<td align="right">
											<?=number_format($totalGeralAreaAno2,2,',','.');?>
										</td>
										<td align="right">
											<?=number_format($totalGeralAreaAno3,2,',','.');?>
										</td>
										<td align="right">
											<?=number_format($totalGeralAreaAno4,2,',','.');?>
										</td>
										<td align="right">
											<?=number_format($totalGeralAreaAno0 + $totalGeralAreaAno1 + $totalGeralAreaAno2 + $totalGeralAreaAno3+ $totalGeralAreaAno4,2,',','.');?>
										</td>
									</tr>
								</table>
							</td>
						</tr>
						<? 
						$totalGeralAreaAno0 = 0;
						$totalGeralAreaAno1 = 0;
						$totalGeralAreaAno2 = 0;
						$totalGeralAreaAno3 = 0;
						$totalGeralAreaAno4 = 0;

					}
?>
		<tr>
							<td class="SubTituloDireita" >
							<b>Total Geral por Dimensão</b>
							</td>
							<td>			
								<table class="listagem" width="100%">
									<thead>
										<th align="center">
											<b>2007</b>
										</th>
										<th align="center">
											<b>2008</b>
										</th>
										<th align="center">
											<b>2009</b>
										</th>
										<th align="center">
											<b>2010</b>
										</th>
										<th align="center">
											<b>2011</b>
										</th>
										<th align="center">
											<b>Total</b>
										</th>
									</thead>
									<tr>
										<td align="right">
											<?=number_format($totalGeralDimensaAno0,2,',','.');?>
										</td>
										<td align="right">
											<?=number_format($totalGeralDimensaAno1,2,',','.');?>
										</td>
										<td align="right">
											<?=number_format($totalGeralDimensaAno2,2,',','.');?>
										</td>
										<td align="right">
											<?=number_format($totalGeralDimensaAno3,2,',','.');?>
										</td>
										<td align="right">
											<?=number_format($totalGeralDimensaAno4,2,',','.');?>
										</td>
										<td align="right">
											<?=number_format($totalGeralDimensaAno0 + $totalGeralDimensaAno1 + $totalGeralDimensaAno2 + $totalGeralDimensaAno3+ $totalGeralDimensaAno4,2,',','.');?>
										</td>
									</tr>
								</table>
							</td>
						</tr>
						<?
						$totalGeralDimensaAno0 = 0; 
						$totalGeralDimensaAno1 = 0;
						$totalGeralDimensaAno2 = 0;
						$totalGeralDimensaAno3 = 0;
						$totalGeralDimensaAno4 = 0;


	}?>	
<tr>
	<td class="SubTituloDireita"  colspan="2" style="text-align: center;">
	<b>Total Geral</b>
	</td>
</tr>
<tr>
	<td colspan="2" >
		<table class="listagem" width="100%">
			<thead>
				<th align="center">
					<b>2007</b>
				</th>
				<th align="center">
					<b>2008</b>
				</th>
				<th align="center">
					<b>2009</b>
				</th>
				<th align="center">
					<b>2010</b>
				</th>
				<th align="center">
					<b>2011</b>
				</th>
				<th align="center">
					<b>Total</b>
				</th>
			</thead>
			<tr>
				<td align="right">
					<?=number_format($totalGeralAno0,2,',','.');?>
				</td>
				<td align="right">
					<?=number_format($totalGeralAno1,2,',','.');?>
				</td>
				<td align="right">
					<?=number_format($totalGeralAno2,2,',','.');?>
				</td>
				<td align="right">
					<?=number_format($totalGeralAno3,2,',','.');?>
				</td>
				<td align="right">
					<?=number_format($totalGeralAno4,2,',','.');?>
				</td>
				<td align="right">
					<?=number_format($totalGeralAno0 + $totalGeralAno1 + $totalGeralAno2 + $totalGeralAno3 + $totalGeralAno4 ,2,',','.');?>
				</td>
			</tr>
		</table>
	</td>
</tr>
</table>

</form>

<?php

function mostra_subacao(){

	global $dado, $i, $totalreg;
	global $total, $totalGeralAno0 , $totalGeralAno1, $totalGeralAno2, $totalGeralAno3, $totalGeralAno4;
	global $totalGeralIndicadorAno0 ,$totalGeralIndicadorAno1, $totalGeralIndicadorAno2, $totalGeralIndicadorAno3, $totalGeralIndicadorAno4;
	global $totalGeralAreaAno0 , $totalGeralAreaAno1 , $totalGeralAreaAno2, $totalGeralAreaAno3, $totalGeralAreaAno4;
	global $totalGeralDimensaAno0,$totalGeralDimensaAno1, $totalGeralDimensaAno2, $totalGeralDimensaAno3, $totalGeralDimensaAno4;
	global $novaDimensao, $novaArea, $novoIndicador, $novaAcao, $novasubAcao;
	global $exibirDados, $statusSubacao;
	global $pareceresPadroes;
	global $capturaTipo, $db, $prgid_outros;

    $sbaidAtual = (integer) $dado[$i]['sbaid'];
		?>
					<tr id="tr_subacao_<?php echo $sbaidAtual; ?>">
						<td class="SubTituloDireita">
							<b>Sub-Ação</b>
						</td>
						<td class="" style="text-align:left">	
							<table  class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="left" >
								<tr>
									<td  class="SubTituloDireita" width="30%">
										Descrição da Subação:
									</td>
									<td width="70%">
										<?=$dado[$i]['sbadsc']; ?>
									</td>
								</tr>
								<tr>
									<td  class="SubTituloDireita" width="30%">
										Parecer Padrão
									</td>
									<td width="70%">
										<?php reset( $pareceresPadroes ); ?>
										<?php if( $pareceresPadroes ): ?>
											<select
												name="psuid[]"
												id="psuid_<?= $sbaidAtual ?>"
											>
												<option>
												<?php foreach( $pareceresPadroes as $chave => $parecer ): ?>
													<?php if( $pareceresPadroes[$chave]['gpsid'] != $pareceresPadroes[$chave-1]['gpsid'] ): ?>
														<optgroup label="<?= $parecer['gpsdescricao'] ?>">
													<?php endif; ?>
													<option
														value="<?= $parecer['psuid'] ?>"
														<?= $dado[$i]['psuid'] == $parecer['psuid'] ? "selected='selected'" : "" ?>
													>
														<?= $parecer['psudescricao'] ?>
													</option>
													<?php if( $pareceresPadroes[$chave]['gpsid'] != $pareceresPadroes[$chave+1]['gpsid'] ): ?>
														</optgroup>
													<?php endif; ?>
												<?php endforeach; ?>
											</select>
										<?php endif; ?>
									</td>
								</tr>
								<tr>
									<td  class="SubTituloDireita" width="30%">
										Parecer
									</td>
									<td width="70%">
										<?php
											$nomeCampo = "sbaparecer_" . $sbaidAtual;
											global $$nomeCampo;
											$$nomeCampo = $dado[$i]['sbaparecer'];
											echo campo_textarea( $nomeCampo, 'N', $habil, '', 70, 3, 0 );
										?>
									</td>
								</tr>
								<tr>
									<td  class="SubTituloDireita" width="30%">
										Status do Parecer
									</td>
									<td width="70%">
										<select
											name="ssuid[]"
											id="ssuid_<?= $sbaidAtual ?>"
										>
											<option value="">Sem status</option>
											<? foreach ( $statusSubacao as $status ) : ?>
												<option
													value="<?= $status['ssuid'] ?>"
													<?= $status['ssuid'] == $dado[$i]['ssuid'] ? ' selected="selected" ' : '' ?>
												>
													<?= $status['ssudescricao'] ?>
												</option>
											<? endforeach; ?>
										</select>
									</td>
								</tr>
								<?
								$ano0 = $dado[$i]['sbaunt'] * $dado[$i]['sba0ano'];
								$ano1 = $dado[$i]['sbaunt'] * $dado[$i]['sba1ano'];
								$ano2 = $dado[$i]['sbaunt'] * $dado[$i]['sba2ano'];
								$ano3 = $dado[$i]['sbaunt'] * $dado[$i]['sba3ano'];
								$ano4 = $dado[$i]['sbaunt'] * $dado[$i]['sba4ano'];

								$totalFinal = $ano0 + $ano1 + $ano2 + $ano3 + $ano4;

								$totalGeralAno0 += $ano0;
								$totalGeralAno1 += $ano1;
								$totalGeralAno2 += $ano2;
								$totalGeralAno3 += $ano3;
								$totalGeralAno4 += $ano4;

								$totalGeralIndicadorAno0 += $ano0;
								$totalGeralIndicadorAno1 += $ano1;
								$totalGeralIndicadorAno2 += $ano2;
								$totalGeralIndicadorAno3 += $ano3;
								$totalGeralIndicadorAno4 += $ano4;

								$totalGeralAreaAno0 += $ano0;
								$totalGeralAreaAno1 += $ano1;
								$totalGeralAreaAno2 += $ano2;
								$totalGeralAreaAno3 += $ano3;
								$totalGeralAreaAno4 += $ano4;

								$totalGeralDimensaAno0 += $ano0;
								$totalGeralDimensaAno1 += $ano1;
								$totalGeralDimensaAno2 += $ano2;
								$totalGeralDimensaAno3 += $ano3;
								$totalGeralDimensaAno4 += $ano4;
								?>
								<? if ( $exibirDados ) : ?>
									<tr>
										<td  class="SubTituloDireita">
											Forma de Atendimento:
										</td>
										<td>
											<?php
											$sql = "
												SELECT
													foaid as codigo,
													foadsc as descricao
												FROM cte.formaatendimento
												ORDER BY
													foadsc
											";
											$nomeCampo = "foaid_" . $sbaidAtual;
											global $$nomeCampo;
											$$nomeCampo = $dado[$i]['foaid'];
											echo $db->monta_combo(
												$nomeCampo,
												$sql, 'S', 'Escolha...',
												'', '', '',
												'', 'N', $nomeCampo
											);
											?>
										</td>
									</tr>
									<tr>
										<td  class="SubTituloDireita">
											Estratégia de Implementação:
										</td>
										<td>
											<?php
											$nomeCampo = "sbastgmpl_" . $sbaidAtual;
											global $$nomeCampo;
											$$nomeCampo = $dado[$i]['sbastgmpl'];
											echo campo_textarea(
												$nomeCampo,
												'N', 'S', '',
												70, 3, 0
											);
											?>
										</td>
									</tr>
									<tr>
										<td  class="SubTituloDireita">
											Programa:
										</td>
										<td>
											<?php
											$nomeCampo = "prgid_" . $sbaidAtual;
											global $$nomeCampo;
											$$nomeCampo = $dado[$i]['prgid'];
											?>

                                          <select onchange="return exibeCampo('<?php echo $sbaidAtual ?>')" name="<?php echo $nomeCampo; ?>" id="<?php echo $nomeCampo; ?>" style="width: 500px;">
                                          </select>
                                          <script type="text/javascript">carregarDados(document.getElementById('<?php echo $nomeCampo; ?>'), '<?php echo $$nomeCampo; ?>');</script>

										</td>
									</tr>
                                    <tr id="div_sbaprm_<?php echo $sbaidAtual; ?>" <?php echo ($prgid_outros != $dado[$i]['prgid'] ? 'style="display: none"' : ''); ?>>
										<td  class="SubTituloDireita">
                                            Descrição do programa:
										</td>
										<td>
											<?php
											$nomeCampo = "sbaprm_" . $sbaidAtual;
											global $$nomeCampo;
											$$nomeCampo = $dado[$i]['sbaprm'];
											echo campo_texto(
												$nomeCampo,
												'N', 'S', '',
												50, 255, '',
												'', 'left', '',
												0, 'id="' . $nomeCampo . '"'
											);
											?>
										</td>
                                    </tr>

									<tr>
										<td  class="SubTituloDireita">
											Unidade de Medida:
										</td>
										<td>
											<?php
											$sql = "
												SELECT
													undid as codigo,
													unddsc as descricao
												FROM cte.unidademedida
												WHERE
													undtipo = '" . $capturaTipo . "'
												ORDER BY
													descricao
											";
											$nomeCampo = "undid_" . $sbaidAtual;
											global $$nomeCampo;
											$$nomeCampo = $dado[$i]['undid'];
/*
											echo $db->monta_combo(
												$nomeCampo,
												$sql, 'S', 'Escolha...',
												'', '', '',
												'', 'N', $nomeCampo
											);
 */
											?>
                                          <select class="CampoEstilo" name="<?php echo $nomeCampo; ?>" id="<?php echo $nomeCampo; ?>">
                                          </select>
                                          <script type="text/javascript">carregarUnidadesMedida('<?php echo $nomeCampo; ?>', '<?php echo $$nomeCampo; ?>');</script>
										</td>
									</tr>
									<tr>
										<td  class="SubTituloDireita">
											Forma de Execução:
										</td>
										<td>
											<?php
											$sql = "
												SELECT
													frmid as codigo,
													frmdsc as descricao
												FROM cte.formaexecucao
												WHERE
													frmtipo = '" . $capturaTipo . "'
												ORDER BY
													descricao
											";
											$nomeCampo = "frmid_" . $sbaidAtual;
											global $$nomeCampo;
											$$nomeCampo = $dado[$i]['frmid'];
											echo $db->monta_combo(
												$nomeCampo,
												$sql, 'S', 'Escolha...',
												'', '', '',
												'', 'N', $nomeCampo
											);
											?>
										</td>
									</tr>
									<tr>
										<td  class="SubTituloDireita">
											Instituição Parceira (se houver):
										</td>
										<td>
											<?php
											$nomeCampo = "sbapcr_" . $sbaidAtual;
											global $$nomeCampo;
											$$nomeCampo = $dado[$i]['sbapcr'];
											echo campo_texto(
												$nomeCampo,
												'', 'S', '',
												50, 255, '',
												'', 'left', '',
												0, 'id="' . $nomeCampo . '"'
											);
											?>
										</td>
									</tr>
									<tr>
										<td  class="SubTituloDireita">
											Quantidades e Cronograma Físico
										</td>									
										<td>
											<table class="listagem" width="100%">
												<thead> 
													<th>&nbsp;</th>	
													<th align="center"><b>2007</b></th>
													<th align="center"><b>2008</b></th>
													<th align="center"><b>2009</b></th>
													<th align="center"><b>2010</b></th>
													<th align="center"><b>2011</b></th>
													<th align="center"><b>Total</b></th>
												</thead>
												<tr>
													<td align="right"><b>Quantidades:</b></td>
													<td align="right">
														<?php
														$nomeCampo = "sba0ano_" . $sbaidAtual;
														global $$nomeCampo;
														$$nomeCampo = $dado[$i]['sba0ano'];
														echo campo_texto(
															$nomeCampo,
															'', 'S', '',
															10, 20, '#######',
															'', 'right', '',
															0, 'id="' . $nomeCampo . '" onBlur="MouseBlur(this);"',
															'atualizaTotal( ' . $sbaidAtual . ' );'
														);
														?>
													</td>
													<td align="right">
														<?php
														$nomeCampo = "sba1ano_" . $sbaidAtual;
														global $$nomeCampo;
														$$nomeCampo = $dado[$i]['sba1ano'];
														echo campo_texto(
															$nomeCampo,
															'', 'S', '',
															10, 20, '#######',
															'', 'right', '',
															0, 'id="' . $nomeCampo . '" onBlur="MouseBlur(this);"',
															'atualizaTotal( ' . $sbaidAtual . ' );'
														);
														?>
													</td>
													<td align="right">
														<?php
														$nomeCampo = "sba2ano_" . $sbaidAtual;
														global $$nomeCampo;
														$$nomeCampo = $dado[$i]['sba2ano'];
														echo campo_texto(
															$nomeCampo,
															'', 'S', '',
															10, 20, '#######',
															'', 'right', '',
															0, 'id="' . $nomeCampo . '" onBlur="MouseBlur(this);"',
															'atualizaTotal( ' . $sbaidAtual . ' );'
														);
														?>
													</td>
													<td align="right">
														<?php
														$nomeCampo = "sba3ano_" . $sbaidAtual;
														global $$nomeCampo;
														$$nomeCampo = $dado[$i]['sba3ano'];
														echo campo_texto(
															$nomeCampo,
															'', 'S', '',
															10, 20, '#######',
															'', 'right', '',
															0, 'id="' . $nomeCampo . '" onBlur="MouseBlur(this);"',
															'atualizaTotal( ' . $sbaidAtual . ' );'
														);
														?>
													</td>
													<td align="right">
														<?php
														$nomeCampo = "sba4ano_" . $sbaidAtual;
														global $$nomeCampo;
														$$nomeCampo = $dado[$i]['sba4ano'];
														echo campo_texto(
															$nomeCampo,
															'', 'S', '',
															10, 20, '#######',
															'', 'right', '',
															0, 'id="' . $nomeCampo . '" onBlur="MouseBlur(this);"',
															'atualizaTotal( ' . $sbaidAtual . ' );'
														);
														?>
													</td>
													<td align="right">
														<span id="fisicoTotal_<?= $sbaidAtual ?>">
														<?php
														$total = 	
															$dado[$i]['sba0ano'] +
															$dado[$i]['sba1ano'] +
															$dado[$i]['sba2ano'] +
															$dado[$i]['sba3ano'] +
															$dado[$i]['sba4ano'];
														echo number_format( $total, 0 );
														?>
														</span>
													</td>
												</tr>											
												<tr>
													<td align="right">
														<b>Cronograma Físico:</b>
													</td>
													<td align="right">
														<?php
														$nomeCampo = "sba0ini_" . $sbaidAtual;
														global $$nomeCampo;
														$$nomeCampo = $dado[$i]['sba0ini'];
														echo '&nbsp;de&nbsp' , campo_texto(
															$nomeCampo,
															'', 'S', '',
															7, 7, '##/####',
															'', 'right', '',
															0, 'id="' . $nomeCampo . '"'
														);
														echo '<br/>&nbsp;até&nbsp';
														$nomeCampo = "sba0fim_" . $sbaidAtual;
														global $$nomeCampo;
														$$nomeCampo = $dado[$i]['sba0fim'];
														echo campo_texto(
															$nomeCampo,
															'', 'S', '',
															7, 7, '##/####',
															'', 'right', '',
															0, 'id="' . $nomeCampo . '"'
														);
														?>
													</td>
													<td align="right">
														<?php
														$nomeCampo = "sba1ini_" . $sbaidAtual;
														global $$nomeCampo;
														$$nomeCampo = $dado[$i]['sba1ini'];
														echo '&nbsp;de&nbsp' , campo_texto(
															$nomeCampo,
															'', 'S', '',
															7, 7, '##/####',
															'', 'right', '',
															0, 'id="' . $nomeCampo . '"'
														);
														echo '<br/>&nbsp;até&nbsp';
														$nomeCampo = "sba1fim_" . $sbaidAtual;
														global $$nomeCampo;
														$$nomeCampo = $dado[$i]['sba1fim'];
														echo campo_texto(
															$nomeCampo,
															'', 'S', '',
															7, 7, '##/####',
															'', 'right', '',
															0, 'id="' . $nomeCampo . '"'
														);
														?>
													</td>
													<td align="right">
														<?php
														$nomeCampo = "sba2ini_" . $sbaidAtual;
														global $$nomeCampo;
														$$nomeCampo = $dado[$i]['sba2ini'];
														echo '&nbsp;de&nbsp' , campo_texto(
															$nomeCampo,
															'', 'S', '',
															7, 7, '##/####',
															'', 'right', '',
															0, 'id="' . $nomeCampo . '"'
														);
														echo '<br/>&nbsp;até&nbsp';
														$nomeCampo = "sba2fim_" . $sbaidAtual;
														global $$nomeCampo;
														$$nomeCampo = $dado[$i]['sba2fim'];
														echo campo_texto(
															$nomeCampo,
															'', 'S', '',
															7, 7, '##/####',
															'', 'right', '',
															0, 'id="' . $nomeCampo . '"'
														);
														?>
													</td>
													<td align="right">
														<?php
														$nomeCampo = "sba3ini_" . $sbaidAtual;
														global $$nomeCampo;
														$$nomeCampo = $dado[$i]['sba3ini'];
														echo '&nbsp;de&nbsp' , campo_texto(
															$nomeCampo,
															'', 'S', '',
															7, 7, '##/####',
															'', 'right', '',
															0, 'id="' . $nomeCampo . '"'
														);
														echo '<br/>&nbsp;até&nbsp';
														$nomeCampo = "sba3fim_" . $sbaidAtual;
														global $$nomeCampo;
														$$nomeCampo = $dado[$i]['sba3fim'];
														echo campo_texto(
															$nomeCampo,
															'', 'S', '',
															7, 7, '##/####',
															'', 'right', '',
															0, 'id="' . $nomeCampo . '"'
														);
														?>
													</td>
													<td align="right">
														<?php
														$nomeCampo = "sba4ini_" . $sbaidAtual;
														global $$nomeCampo;
														$$nomeCampo = $dado[$i]['sba4ini'];
														echo '&nbsp;de&nbsp' , campo_texto(
															$nomeCampo,
															'', 'S', '',
															7, 7, '##/####',
															'', 'right', '',
															0, 'id="' . $nomeCampo . '"'
														);
														echo '<br/>&nbsp;até&nbsp';
														$nomeCampo = "sba4fim_" . $sbaidAtual;
														global $$nomeCampo;
														$$nomeCampo = $dado[$i]['sba4fim'];
														echo campo_texto(
															$nomeCampo,
															'', 'S', '',
															7, 7, '##/####',
															'', 'right', '',
															0, 'id="' . $nomeCampo . '"'
														);
														?>
													</td>
													<td align="right">
														&nbsp;
													</td>
												</tr>											
											</table>
											<table 	class="listagem" width="100%"  cellspacing="1" cellpadding="3" >
												<tr>
													<td align="right">
														<b>Valor Unitário:</b>
													</td>
													<td align="right">
														<?php
														$nomeCampo = "sbaunt_" . $sbaidAtual;
														global $$nomeCampo;
														$$nomeCampo = number_format( $dado[$i]['sbaunt'], 2, ',', '.' );
														echo campo_texto(
															$nomeCampo,
															'', 'S', '',
															10, 20, '###.###.###.###.###.###,##',
															'', 'right', '',
															0, 'id="' . $nomeCampo . '" onBlur="MouseBlur(this);"',
															'atualizaTotal( ' . $sbaidAtual . ' );'
														);
														?>
													</td>
												</tr>
												<tr>
													<td align="right">
														<b>Detalhamento da Composição<br/>do Valor Unitário:</b>  	
													</td>
													<td align="left">
														<?php
														$nomeCampo = "sbauntdsc_" . $sbaidAtual;
														global $$nomeCampo;
														$$nomeCampo = $dado[$i]['sbauntdsc'];
														echo campo_textarea(
															$nomeCampo,
															'N', 'S' , '',
															100, 10, 2000
														);
														?>
													</td>
												</tr>
											</table>
											<table class="listagem" width="100%"  cellspacing="1" cellpadding="3" >
												<thead>
													<th>&nbsp;</th>	
													<th align="center"><b>2007</b></th>
													<th align="center"><b>2008</b></th>
													<th align="center"><b>2009</b></th>
													<th align="center"><b>2010</b></th>
													<th align="center"><b>2011</b></th>
													<th align="center"><b>Total</b></th>
												</thead>
												<tr>
													<td align="right">
														<b>Valores Anuais:</b>
													</td>
													<td align="right">
														<span id="valorTotalAno0_<?= $sbaidAtual ?>">
															<?= number_format( $ano0, 2, ',', '.' ) ; ?>
														</span>
													</td>
													<td align="right">
														<span id="valorTotalAno1_<?= $sbaidAtual ?>">
															<?= number_format( $ano1, 2, ',', '.' ) ; ?>
														</span>
													</td>
													<td align="right">
														<span id="valorTotalAno2_<?= $sbaidAtual ?>">
															<?= number_format( $ano2, 2, ',', '.' ) ; ?>
														</span>
													</td>
													<td align="right">
														<span id="valorTotalAno3_<?= $sbaidAtual ?>">
															<?= number_format( $ano3, 2, ',', '.' ) ; ?>
														</span>
													</td>
													<td align="right">
														<span id="valorTotalAno4_<?= $sbaidAtual ?>">
															<?= number_format( $ano4, 2, ',', '.' ) ; ?>
														</span>
													</td>
													<td align="right">
														<span id="valorTotal_<?= $sbaidAtual ?>">
															<?= number_format( $totalFinal, 2, ',', '.' ) ; ?>
														</span>
													</td>
												</tr>		
											</table>																																
										</td>
									</tr>
								<?php endif; ?>
								<tr>
									<td  class="SubTituloDireita" width="30%">
										&nbsp;
									</td>
									<td width="70%">
										<input
											type="button"
											name="Gravar Parecer"
											value="Gravar Parecer"
											onclick="alterarStatusSubacao( <?= $sbaidAtual ?> )"
										/>
										<input
											type="button"
											name="Excluir Subação"
											value="Excluir Subação"
											onclick="removerSubacao( <?= $sbaidAtual ?>, 'tr_subacao_<?php echo $sbaidAtual; ?>' )"
										/>
									</td>
								</tr>
							</table>	
						</td>
					</tr>
					
				<?		$i++;			

				$novaDimensao = $dado[$i]['dimid'];
				$novaArea = $dado[$i]['ardid'];
				$novoIndicador = $dado[$i]['indid'];
				$novaAcao = $dado[$i]['aciid'];
				$novasubAcao = $dado[$i]['sbaid'];

}

include_once APPRAIZ . "includes/estouvivo.php";

?>