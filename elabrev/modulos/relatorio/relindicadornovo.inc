<?php 


if ($_REQUEST['prgid']) $_SESSION['prgid'] = $_REQUEST['prgid'];
else $_REQUEST['prgid'] =$_SESSION['prgid'];
$prgid = (integer) $_REQUEST['prgid'];

if ( $_REQUEST['indnum'] )
{
		//dbg($_REQUEST['act'],1);
		$sql = "select i.*,to_char(i.inddataapuracao, 'dd/mm/yyyy') as inddataapuracao,um.unmdsc, p.perdsc, b.bsgdsc from elabrev.ppaindicador i left join unidademedida um on um.unmcod=i.unmcod left join periodicidade p on p.percod=i.percod left join basegeografica b on b.bsgcod=i.bsgcod where indnum=".$_REQUEST['indnum'];
		$saida = $db->recuperar($sql,$res);
        if(is_array($saida)) {foreach($saida as $k=>$v) ${$k}=$v;}	
}
//


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
	<?
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
				SIMEC- Sistema Integrado de Monitoramento Execução e Controle<br/>
				Acompanhamento da Execução Orçamentária<br/>
				MEC / SE - Secretaria Executiva <br />
				SPO - Subsecretaria de Planejamento e Orçamento
			</td>
			<td align="right" valign="middle" height="1" style="padding:5px 0 0 0;">
				Impresso por: <b><?= $_SESSION['usunome'] ?></b><br/>
				Hora da Impressão: <?= date( 'd/m/Y - H:i:s' ) ?><br />
				Orçamento Fiscal e Seg.Social - EM R$ 1,00 <br />
				<? $whereAcumuladoAte = ''; ?>
				<?// $valoresFiltroAno = $consulta->pegarValoresFiltro( 'ano' ); ?>
				<? if ( count( $valoresFiltroAno ) > 0 ) : ?>
					<? $whereAcumuladoAte = " where rofano in ( '" . implode( "','", $valoresFiltroAno ) . "' ) "; ?>
				<? endif; ?>
				<?// $sqlAcumuladoAte = "select max( rofdata ) as data from financeiro.reporcfin " . $whereAcumuladoAte; ?>
				Acumulado até: <?//= formata_data( $db->pegaUm( $sqlAcumuladoAte ) ) ?>
			</td>
		</tr>
		<tr>
			<td colspan="2" align="center" valign="top" style="padding:0 0 5px 0;">
				<b><font style="font-size:14px;">Relatório de Propostas de Novo Indicador</font></b>
			</td>
		</tr>
	</table>
	<?
}

?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
	<head>
		<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
		<meta http-equiv="Cache-Control" content="no-cache">
		<meta http-equiv="Pragma" content="no-cache">
		<meta http-equiv="Expires" content="-1">
		<title>Relatório de Propostas de Novo Indicador</title>
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
									<th class="bold alignLeft" width="35%"><?= $geral ?></th>
									<th class="bold alignLeft" width="35%" style="padding: 0 0 0 10px;">&nbsp;</th>
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
		      
				      	<table border="1" class="tabelaDados">
							<tr bgcolor="<?= $cor ?>" onmouseover="this.style.backgroundColor = '#ffffcc';" onmouseout="this.style.backgroundColor = '<?= $cor ?>';">
								<td class="alignLeft titulolinha" width="35%" style="padding: 0 0 0 5px;">
									Denominação:
								</td>
								<td width="65%" style="padding: 0 0 0 5px;">
								<?=$inddsc?>
								</td>
							</tr>

							
							<tr bgcolor="<?= $cor ?>" onmouseover="this.style.backgroundColor = '#ffffcc';" onmouseout="this.style.backgroundColor = '<?= $cor ?>';">
								<td class="alignLeft titulolinha" width="35%" style="padding: 0 0 0 5px;">
									Unidade de Medida:
								</td>
								<td width="65%" style="padding: 0 0 0 5px;">
								<?=$unmdsc?>
								</td>
							</tr>
							
							<tr bgcolor="<?= $cor ?>" onmouseover="this.style.backgroundColor = '#ffffcc';" onmouseout="this.style.backgroundColor = '<?= $cor ?>';">
								<td class="alignLeft titulolinha" width="35%" style="padding: 0 0 0 5px;">
									Índice de referência<br>Data de apuração:
								</td>
								<td width="65%" style="padding: 0 0 0 5px;">
								<?=$indvlrreferencia?>
								</td>
							</tr>
							
							<tr bgcolor="<?= $cor ?>" onmouseover="this.style.backgroundColor = '#ffffcc';" onmouseout="this.style.backgroundColor = '<?= $cor ?>';">
								<td class="alignLeft titulolinha" width="35%" style="padding: 0 0 0 5px;">
									Índices esperados ao longo do PPA:
								</td>
								<td width="65%" style="padding: 0 0 0 5px;">
								<?=$indvlrano1?>
								</td>
							</tr>

							<tr bgcolor="<?= $cor ?>" onmouseover="this.style.backgroundColor = '#ffffcc';" onmouseout="this.style.backgroundColor = '<?= $cor ?>';">
								<td class="alignLeft titulolinha" width="35%" style="padding: 0 0 0 5px;">
									Índice ao final do programa:
								</td>
								<td width="65%" style="padding: 0 0 0 5px;">
								<?=$indvlrfinalprg?>
								</td>
							</tr>

							<tr bgcolor="<?= $cor ?>" onmouseover="this.style.backgroundColor = '#ffffcc';" onmouseout="this.style.backgroundColor = '<?= $cor ?>';">
								<td class="alignLeft titulolinha" width="35%" style="padding: 0 0 0 5px;">
									Fonte:
								</td>
								<td width="65%" align="justify" valign="middle" style="padding: 0 0 0 5px;"><?=$inddscfonte?></td>
							</tr>

							<tr bgcolor="<?= $cor ?>" onmouseover="this.style.backgroundColor = '#ffffcc';" onmouseout="this.style.backgroundColor = '<?= $cor ?>';">
								<td class="alignLeft titulolinha" width="35%" style="padding: 0 0 0 5px;">
									Periodicidade:
								</td>
								<td width="65%" style="padding: 0 0 0 5px;">
								<?=$perdsc?>
								</td>
							</tr>
							
							<tr bgcolor="<?= $cor ?>" onmouseover="this.style.backgroundColor = '#ffffcc';" onmouseout="this.style.backgroundColor = '<?= $cor ?>';">
								<td class="alignLeft titulolinha" width="35%" style="padding: 0 0 0 5px;">
									Base geográfica:
								</td>
								<td width="65%" style="padding: 0 0 0 5px;">
								<?=$bsgdsc?>
								</td>
							</tr>
							
							<tr bgcolor="<?= $cor ?>" onmouseover="this.style.backgroundColor = '#ffffcc';" onmouseout="this.style.backgroundColor = '<?= $cor ?>';">
								<td class="alignLeft titulolinha" width="35%" style="padding: 0 0 0 5px;">
									Fórmula:
								</td>
								<td width="65%" style="padding: 0 0 0 5px;">
								<?=$inddscformula?>
								</td>
							</tr>

							<tr bgcolor="<?= $cor ?>" onmouseover="this.style.backgroundColor = '#ffffcc';" onmouseout="this.style.backgroundColor = '<?= $cor ?>';">
								<td class="alignLeft titulolinha" width="35%" style="padding: 0 0 0 5px;">
									Observações:
								</td>
								<td width="65%" style="padding: 0 0 0 5px;">
								<?=$indobs?>
								</td>
							</tr>
							
						</table>
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