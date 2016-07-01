<?php 
/*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Leticia Jacome ()
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Henrique Xavier Couto (henriquecouto@mec.gov.br)
   Módulo:relresponsa.inc
   Finalidade: permitir a visão do relatorio de responsabilidades
    */



	$codResponsa = array(
		'equipe_planeja_acao' => 68,
		'coord_planeja_acao' => 67,
		'equipe_planeja_unidade' => 33,
		'coord_planeja_unidade' => 24,
		'uma_unidade' => 33,
		'uma_programa' => 66,
		'uma_coord' => 36
	);
	
	$dscResponsa = array(
		'equipe_planeja_acao' => 'Equipe de Planejamento Ação',
		'coord_planeja_acao' => 'Coordenador de Planejamento Ação',
		'equipe_planeja_unidade' => 'Equipe de Planejamento Unidade',
		'coord_planeja_unidade' => 'Coordenador de Planejamento Unidade',
		'uma_unidade' => 'UMA Unidade',
		'uma_programa' => 'UMA Programa',
		'uma_coord' => 'Coordenador UMA'
	);

	$tradutor = array(
		'a'	=> array(
			'cod_campo' => 'prop.acaid',
			'cod_alias' => 'acaid',
			'dsc_campo' => 'prop.acadsc',
			'dsc_alias' => 'acadsc',
			'titulo'    => 'Ação'
		),
		'u'	=> array(
			'cod_campo' => 'u.unicod',
			'cod_alias' => 'unicod',
			'dsc_campo' => 'u.unidsc',
			'dsc_alias' => 'unidsc',
			'titulo'    => 'Unidade'
		),
		'p'	=> array(
			'cod_campo' => 'prop.prgid',
			'cod_alias' => 'prgid',
			'dsc_campo' => 'prop.prgdsc',
			'dsc_alias' => 'prgdsc',
			'titulo'    => 'Programa'
		)

	);

function montaFiltroPerfil()
{
	global $codResponsa;
	
	$clausula = '';
	$sinal = '';

	
	if ( $_REQUEST['vinculo'] ) $sinal = '<>'; else $sinal = '=';
	if ( $_REQUEST['equipe_planeja_acao'] == 1 ) $clausula .= 'and pf.pflcod ' . $sinal . $codResponsa['equipe_planeja_acao'];	
	if ( $_REQUEST['coord_planeja_acao'] == 1 ) $clausula .= 'and pf.pflcod ' . $sinal . $codResponsa['coord_planeja_acao'];	
	if ( $_REQUEST['equipe_planeja_unidade'] == 1 ) $clausula .= 'and pf.pflcod ' . $sinal . $codResponsa['equipe_planeja_unidade'];	
	if ( $_REQUEST['coord_planeja_unidade'] == 1 ) $clausula .= 'and pf.pflcod ' . $sinal . $codResponsa['coord_planeja_unidade'];	
	if ( $_REQUEST['uma_unidade'] == 1 ) $clausula .= 'and pf.pflcod ' . $sinal . $codResponsa['uma_unidade'];	
	if ( $_REQUEST['uma_programa'] == 1 ) $clausula .= 'and pf.pflcod ' . $sinal . $codResponsa['uma_programa'];	
	if ( $_REQUEST['uma_coord'] == 1 ) $clausula .= 'and pf.pflcod  ' . $sinal . $codResponsa['uma_coord'];	

	return $clausula;
}

function montaFiltros()
{
	global $db;
	$retorno = '';
	$sinal = '';
	
	if ( $_REQUEST['vinculo'] ) $sinal = 'not';

	
	if ($_REQUEST['geral'] == 'a' && $_REQUEST['acacod'][0] != '' ) $retorno = 	"and prop.acaid ".$sinal." in (".implode(",",$_REQUEST['acacod']).") ";
	if ($_REQUEST['geral'] == 'u' && $_REQUEST['unicod'][0] != '' ) $retorno = 	"and prop.unicod ".$sinal." in ('".implode("','",$_REQUEST['unicod'])."') ";
	if ($_REQUEST['geral'] == 'p' && $_REQUEST['programa'][0] != '' ) $retorno = 	"and prop.prgid ".$sinal." in (".implode(",",$_REQUEST['programa']).") ";

	return $retorno;
}

function montaFiltroGrupo( $condicao )
{
	global $db;
	$retorno = '';
	
	if ($_REQUEST['geral'] == 'a') $retorno = 	'and '.$condicao;
	if ($_REQUEST['geral'] == 'u') $retorno = 	'and '.$condicao;
	if ($_REQUEST['geral'] == 'p') $retorno = 	'and '.$condicao;

	return $retorno;
}

function recuperaResponsavel( $codigo, $tipo )
{
	global $db;
	$dsc_responsavel = '';
	
	$sql = sprintf(
		"select u.usunome from seguranca.usuario u inner join elabrev.usuarioresponsabilidade r ON r.usucpf = u.usucpf where "
	);


	if ( $tipo == 'a') { $sql .= sprintf(" r.acaid = %d and r.rpustatus = 'A' and r.prsano = '%s' ",$codigo,$_SESSION['exercicio']); }
	if ( $tipo == 'u') { $sql .= sprintf(" r.unicod = '%s' and r.rpustatus = 'A' and r.prsano = '%s' ",$codigo,$_SESSION['exercicio']); }
	if ( $tipo == 'p') { $sql .= sprintf(" r.prgid = %d and r.rpustatus = 'A' and r.prsano = '%s' ",$codigo,$_SESSION['exercicio']); }
	

	$dsc_responsavel = $db->pegaUm( $sql );

	return $dsc_responsavel;
}

function monta_From( $agrupador )
{
	$sql = '';
	
	if ( $agrupador == 'u' )
		$sql =  'inner join elabrev.usuarioresponsabilidade usu ON '.
				'	usu.unicod = prop.unicod '.
				'inner join seguranca.perfil pf ON '.
				'	pf.pflcod = usu.pflcod ';
	elseif ( $agrupador == 'a' )
		$sql =  'inner join elabrev.usuarioresponsabilidade usu ON '.
				'	usu.acaid = prop.acaid '.
				'inner join seguranca.perfil pf ON '.
				'	pf.pflcod = usu.pflcod ';
	elseif ( $agrupador == 'p' )
		$sql =  'inner join elabrev.usuarioresponsabilidade usu ON '.
				'	usu.prgid = prop.prgid '.
				'inner join seguranca.perfil pf ON '.
				'	pf.pflcod = usu.pflcod ';

	return $sql;
}

function montaConsulta( $nivel, $clausula, $geral )
{
    if($geral){
	global $db;
	$select = '';
	$from = '';

	if ($geral == 'a') $select = 'select distinct prop.acaid as codigo, prop.acacod ||  \' - \' || prop.acadsc as descricao ';
	if ($geral == 'u') $select = 'select distinct prop.unicod as codigo, prop.unicod || \' - \' || prop.unidsc as descricao ';
	if ($geral == 'p') $select = 'select distinct prop.prgid as codigo, prop.prgcod ||  \' - \' || prop.prgdsc as descricao ';

	$select .= ' from ';

	if ( $geral == 'a' ) $select .= 'elabrev.ppaacao_proposta prop ';
	if ( $geral == 'u' ) $select .= 'unidade prop ';
	if ( $geral == 'p' ) $select .= 'elabrev.ppaprograma_proposta prop ';

	$select .= monta_From( $geral );

	$select .= 'where 1=1 ';
//dbg($select,1);
	if ( $nivel == 0 )
	{
		$select .= montaFiltroPerfil();
		$select .= montaFiltros();
	}
	else 
	{
		$select .= montaFiltroGrupo( $clausula );
		return $select;
		exit();
	}
//	dbg( $select , 1);
	montaDados( $select ); 
    }
}

function montaDados ( $consulta )
{
	global $db;
	
	$geral = $_REQUEST['geral'];
        $itens = $db->carregar( $consulta );
//	dbg( $consulta, 1  );


	if ( $itens )
	{
		foreach ( $itens as $item )
		{
			if ( $geral == 'a' ) cfDesenhaRegistro( $item, 0.5, 0, $geral );
			elseif ( $geral == 'u' )
			{
				cfDesenhaRegistro( $item, 0.5, 0, $geral );
				if ( $_REQUEST['lsacaounid'] == "1") 
				{
						$sql = montaConsulta( 1, 'prop.unicod = '.$item['codigo'], 'a' );
						$nivel = $db->carregar( $sql );
						if ( $nivel != '' )
						{
							cfDesenhaTitulo( 'Ação', 2 );
							foreach ( $nivel as $ls )
							{
								cfDesenhaRegistro( $ls, 2, 0, 'a' );
							}
						}
					espaço( );
						
				}
			}
			
				elseif ( $geral == 'p' )
				{
					cfDesenhaRegistro( $item, 0.5, 0, $geral );
					if ( $_REQUEST['lsacaoprog'] == "1" ) 
					{
						$sql1 = montaConsulta( 1, "prop.prgid = ".$item['codigo']."", 'a' );
						$nivel1 = $db->carregar( $sql1 );
						if ( $nivel1 != '')
						{
							cfDesenhaTitulo( 'Ação', 2 );
							foreach ( $nivel1 as $ls1 )
							{
								cfDesenhaRegistro( $ls1, 2, 0, 'a' );
	
									if ( $_REQUEST['lsunidadeprog'] == "1" ) 
									{
										$sql2 = montaConsulta( 1, "u.acaid = ".$ls1['codigo']."", 'u' );
										$nivel2 = $db->carregar( $sql2 );
										if ( $nivel2 != '')
										{
											cfDesenhaTitulo( 'Unidade', 4 );
											foreach ( $nivel2 as $ls2 )
											{
												cfDesenhaRegistro( $ls2, 4, 0, 'u' );
											}
											
											espaço( );
										}
										
									}
								
							}
							espaço( );
	
						}
	
					}
	
				}
	
		}
		
	}
	else 
	{
		cfDesenhaTitulo( 'Não existem registros para o filtro efetuado', 1 );
	}


}

function espaço( )
{
	?>
	<table>
		<tr >
			<td class="alignLeft titulolinha">
			<hr>
			</td>
		</tr>
	</table>
	<?php
}


function cfDesenhaTitulo( $item, $nivel )
{
	static $cor = '';
	$cor = $cor == '' ? '#f8f8f8' : '';
	?>
	<table class="tabelaDados">
		<tr bgcolor="<?= $cor ?>">
			<td class="alignLeft titulolinha" style="padding: 0 0 0 <?= $nivel * 10 ?>px;">
				<?= $item ?>
			</td>
		</tr>
	</table>
	<?php
}

function cfDesenhaRegistro( $item, $nivel, $agrupador, $grupo )
{
	static $cor = '';
	$cor = $cor == '' ? '#f8f8f8' : '';
	$seta = $nivel > 0 ? '&rsaquo; ' : '' ;
	?>
	<table class="tabelaDados">
		<tr bgcolor="<?= $cor ?>" onmouseover="this.style.backgroundColor = '#ffffcc';" onmouseout="this.style.backgroundColor = '<?= $cor ?>';">
			<td class="alignLeft titulolinha" width="65%" style="padding: 0 0 0 <?= $nivel * 10 ?>px;">
				<a href="" class="alignLeft titulolinha"><?= $item['descricao'] ?></a>
			</td>
			<td class="alignLeft titulolinha" width="35%" style="padding: 0 0 0 5px;">
				<a href="" class="alignLeft titulolinha">&nbsp;<?= recuperaResponsavel( $item['codigo'] , $grupo ) ?></a>
			</td>
		</tr>
	</table>
	<?php
}


function cabecalhoBrasao()
{
	global $db;
	global $consulta;
	?>
	<table width="100%" border="0" cellpadding="0" cellspacing="0" class="notscreen1 debug"  style="border-bottom: 1px solid;">
		<tr bgcolor="#ffffff">
			<td valign="top" width="50" rowspan="2"><img src="../imagens/brasao.gif" width="45" height="45" border="0"></td>
			<td nowrap align="left" valign="middle" height="1" style="padding:5px 0 0 0;">
				SIMEC- Sistema Integrado do Ministério da Educação<br/>
				Módulo de Elaboração e Revisão<br/>
				MEC / CPS - Coordenação de Planejamento Setorial <br />
				SPO - Subsecretaria de Planejamento e Orçamento
			</td>
			<td align="right" valign="middle" height="1" style="padding:5px 0 0 0;">
				Impresso por: <b><?= $_SESSION['usunome'] ?></b><br/>
				Hora da Impressão: <?= date( 'd/m/Y - H:i:s' ) ?><br />
				Relatório de Responsabilidades <br />
			</td>
		</tr>
		<tr>
			<td colspan="2" align="center" valign="top" style="padding:0 0 5px 0;">
				<b><font style="font-size:14px;">Relatório de Responsabilidades</font></b>
			</td>
		</tr>
	</table>
	<?php
}

?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
	<head>
		<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
		<meta http-equiv="Cache-Control" content="no-cache">
		<meta http-equiv="Pragma" content="no-cache">
		<meta http-equiv="Expires" content="-1">
		<title>Relatório de Responsabilidades</title>
		<style type="text/css">
			
			@media print {.notprint { display: none }}

			@media screen {
			.notscreen { display: none;  }
			.div_rolagem{ overflow-x: auto; overflow-y: auto; width:19.5cm;height:350px;}
			.topo { position: absolute; top: 0px; margin: 0; padding: 5px; position: fixed; background-color: #ffffff;}
			}

			*{margin:0; padding:0; border:none; font-size:8px;font-family:Arial;}
			.alignRight{text-align:right !important;}
			.alignCenter{ text-align:center !important;}
			.alignLeft{text-align:left !important;}
			.bold{font-weight:bold !important;}
			.italic{font-style:italic !important;}
			.noPadding{padding:0;}
			.titulo{width:52px;}
			.tituloagrup{font-size:9px;}
			.titulolinha{font-size:9px;}
			
			#tabelaTitulos tr td, #tabelaTitulos tr th{border:2px solid black;border-left:none; border-right:none;}
			#orgao{margin:3px 0 0 0;}
			#orgao tr td{border:1px solid black;border-left:none;border-right:none;font-size:11px;}
			
			div.filtro { page-break-after: always; text-align: center; }
			
			table{width:19cm;border-collapse:collapse;}
			th, td{font-weight:normal;padding:4px;vertical-align:top;}
			thead{display:table-header-group;}
			table, tr{page-break-inside:avoid;}
			a{text-decoration:none;color:#3030aa;}
			a:hover{text-decoration:underline;color:#aa3030;}
			span.topo { position: absolute; top: 3px; margin: 0; padding: 5px; position: fixed; background-color: #f0f0f0; border: 1px solid #909090; cursor:pointer; }
			span.topo:hover { background-color: #d0d0d0; }
			
		</style>
	</head>
	<body>
		<div id="aguarde" style="background-color:#ffffff;position:absolute;color:#000033;top:50%;left:30%;border:2px solid #cccccc; width:300px;">
			<center style="font-size:12px;"><br><img src="../imagens/wait.gif" border="0" align="absmiddle"> Aguarde! Gerando Relatório...<br><br></center>
		</div>
		<script type="text/javascript">
			self.focus();
		</script>
		<?php ob_flush();flush(); ?>
		<?php
		// realiza consulta, mantem dados na sessão para as imagens
//		$itens = $consulta->consultar();
//		$_SESSION['consulta_financeira'] = array();
//		$_SESSION['consulta_financeira']['itens'] = $itens['itens'];
//		$_SESSION['consulta_financeira']['agrupadores'] = $consulta->pegarAgrupadores( true );
		// FIM realiza consulta, mantem dados na sessão para as imagens
		
	$geral = $tradutor[$_REQUEST['geral']]['titulo'];
	if ( $_REQUEST['geral'] == 'u' && $_REQUEST['lsacaounid'] == 1 ) $geral .= ' > Ação';
	if ( $_REQUEST['geral'] == 'p' && $_REQUEST['lsacaoprog'] == 1 ) $geral .= ' > Ação';
	if ( $_REQUEST['geral'] == 'p' && $_REQUEST['lsunidadeprog'] == 1 ) $geral .= ' > Unidade';
	
?>
		<table>
			<thead>
				<tr>
					<th class="noPadding" align="left">
						<? cabecalhoBrasao(); ?>
						<table id="tabelaTitulos" align="left">
							<thead>
								<tr>
									<th class="bold alignLeft" width="65%"><?= $geral ?></th>
									<th class="bold alignLeft" width="35%" style="padding: 0 0 0 10px;">Responsável</th>
								</tr>
							</thead>
						</table>
					</th>
				</tr>
			</thead>
			<tbody>
				<tr>
					<td class="noPadding" align="left">
						<div class="div_rolagem">
							<?php montaConsulta( 0, null, $_REQUEST['geral'] ); ?>
						</div>
					</td>
				</tr>
			</tbody>
		</table>
		<script type="text/javascript" language="javascript">
			document.getElementById( 'aguarde' ).style.visibility = 'hidden';
			document.getElementById('aguarde').style.display = 'none';
		</script>
	</body>
</html>