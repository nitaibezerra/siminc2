<?php

/*
	Sistema Simec
	Setor responsável: SPO-MEC
	Desenvolvedor: Equipe Consultores Simec
	Analista: Adonias Malosso (malosso@gmail.com)
	Programador: Renan de Lima Barbosa (e-mail: renandelima@gmail.com)
	Módulo: popupRemessa.inc
	Finalidade: Exibir dados de uma remessa
*/

// inicia sistema

include 'config.inc';
include APPRAIZ . 'includes/classes_simec.inc';
include APPRAIZ . 'includes/funcoes.inc';
$db = new cls_banco();

function cabecalhoBrasao()
{
	global $db;
	?>
	<table width="100%" border="0" cellpadding="0" cellspacing="0" style="border-bottom: 1px solid;">
		<tr bgcolor="#ffffff">
			<td valign="top" width="50" rowspan="2"><img src="/imagens/brasao.gif" height="45" border="0"></td>
			<td nowrap align="left" valign="middle" height="1" style="padding:5px 0 0 0;">
				<?php echo NOME_SISTEMA; ?><br/>
				Acompanhamento da Execução Orçamentária<br/>
				MEC / SE - Secretaria Executiva <br />
				SPO - Subsecretaria de Planejamento e Orçamento
			</td>
			<td align="right" valign="middle" height="1" style="padding:5px 0 0 0;">
				Impresso por: <b><?= $_SESSION['usunome'] ?></b><br/>
				Hora da Impressão: <?= date( 'd/m/Y - H:i:s' ) ?><br />
			</td>
		</tr>
		<tr>
			<td colspan="2" align="center" valign="top" style="padding:0 0 5px 0;">
				<b><font style="font-size:14px;">Remessa</font></b>
			</td>
		</tr>
	</table>
	<?
}

$remid = (integer) $_REQUEST['remid'];

// captura dados gerais da remessa
$sql = "select * from elabrev.remessa where remid = " . $remid;
$remessa = $db->recuperar( $sql );

$sql = "
	select
		a.unicod,	a.prgcod,
		a.acacod,	a.loccod,
		n.ndpcod,	d.iducod,
		d.foncod,	i.idocod,
		d.dpavalor
	from elabrev.remessa r
		inner join elabrev.despesaacao d on d.remid = r.remid
		inner join public.naturezadespesa n on n.ndpid = d.ndpid
		inner join public.idoc i on i.idoid = d.idoid
		inner join elabrev.ppaacao_orcamento a on a.acaid = d.acaid
	where
		r.remid = {$remid}
";

// captura os itens de remanejamentos da remessa
$itens = $db->carregar( $sql );
if ( !$itens )
{
	$itens = array();
}

?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
	<head>
		<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
		<meta http-equiv="Cache-Control" content="no-cache">
		<meta http-equiv="Pragma" content="no-cache">
		<meta http-equiv="Expires" content="-1">
		<title>Remessa</title>
		<style type="text/css">
			
			@media print {.notprint { display: none }}

			@media screen {
				.notscreen { display: none;  }
				.topo { position: absolute; top: 0px; margin: 0; padding: 5px; position: fixed; background-color: #ffffff;}
			}

			*{margin:0; padding:0; border:none; font-size:8px;font-family:Arial;}
			.noPadding{padding:0;}
			
			table{width:18cm;border-collapse:collapse;}
			th, td{font-weight:normal;padding:4px;vertical-align:top;}
			thead{display:table-header-group;}
			
			span.topo { position: absolute; top: 3px; margin: 0; padding: 5px; position: fixed; background-color: #f0f0f0; border: 1px solid #909090; cursor:pointer; }
			span.topo:hover { background-color: #d0d0d0; }
			
		</style>
		<script type="text/javascript">
			self.focus();
		</script>
	</head>
	<body>
		<table cellpadding="0" cellspacing="0" border="0">
			<thead>
				<tr>
					<th class="noPadding" align="left">
						<?php cabecalhoBrasao(); ?>
					</th>
				</tr>
			</thead>
			<tbody>
				<tr>
					<td class="noPadding" align="left">
					
						<?php dbg( $remessa ); ?>
						<?php dbg( $itens ); ?>
					</td>
				</tr>
			</tbody>
		</table>
	</body>
</html>