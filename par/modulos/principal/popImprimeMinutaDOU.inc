<?
$texto = pegaTermoCompromissoArquivo( $_REQUEST['dopid'], '' );

$larguraRel = 650;
?>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
	<head>
		<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
		<meta http-equiv="Cache-Control" content="no-cache">
		<meta http-equiv="Pragma" content="no-cache">
		<meta http-equiv="Expires" content="-1">
		<title>Minuta de Convênio do PTA</title>
		<style type="text/css" media="print">
			
			@media print {.notprint { display: none }}
			
			@media screen {
			.notscreen { display: none;  }
			.div_rolagem{ overflow-x: auto; overflow-y: auto; width:<?=$larguraRel?>px;height:450px;}
			.topo { position: absolute; top: 0px; margin: 0; padding: 5px; position: fixed; background-color: #ffffff;}
			}
			
			.alignRight{text-align:right !important;width:100px;}
			.alignRightTit{text-align:right !important;padding:4px;}
			.colunaValor{width:80px;text-align:right;border-right: 1px solid #CCCCCC;padding: 2px 3px 2px 3px;}
			.alignCenter{ text-align:center !important;}
			.alignLeft{text-align:left !important;}
			.bold{font-weight:bold !important;}
			.italic{font-style:italic !important;}
			.noPadding{padding:0;}
			
			#tabelaTitulos th{border:2px solid black;}
			
			table{width:<?=$larguraRel?>px !important;border-collapse:saparate;}
			th, td{font-weight:normal;vertical-align:top;}
			thead{display:table-header-group;}
			table, tr{page-break-inside:avoid;}
			a{text-decoration:none;color:#3030aa;}
			a:hover{text-decoration:underline;color:#aa3030;}
			span.topo { position: absolute; top: 3px; margin: 0; padding: 5px; position: fixed; background-color: #f0f0f0; border: 1px solid #909090; cursor:pointer; }
			span.topo:hover { background-color: #d0d0d0; }
			
			@media print {.notprint { display: none } .div_rolagem{display: none} }	
			@media screen {.notscreen { display: none; }
			
			.div_rolagem{ overflow-x: auto; overflow-y: auto; height: 50px;}
			
		</style>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		
 <!--   <link rel="stylesheet" type="text/css" href="normal.css"  media="screen" />
		<link rel="stylesheet" type="text/css" href="impressao.css"  media="print" /> -->
		
	</head>
	<body>
		<div id="topo">		
		<table width="100%" class="tabela" align="center">
				<thead>
					<tr>
						<td class="noPadding" align="left">
							<table width="100%" border="0" cellpadding="0" cellspacing="0" class="notscreen1 debug">
								<tr bgcolor="#ffffff">
									<td valign="top" align="center"><img src="../imagens/brasao.gif" width="45" height="45" border="0"></td>
								</tr>
								<tr bgcolor="#ffffff">
									<td colspan="2" align="center" valign="top" style="padding:0 0 5px 0;">
										<b><font style="font-size:11px;font-family:arial;color:#9F9F9F;">MINISTÉRIO DA EDUCAÇÃO<br />FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO</font></b>
									</td>
								</tr>
							</table>
						</td>
					</tr>
				</thead>
			<tbody>
				<tr>
					<td class="noPadding" align="left">
						
					</td>
				</tr>
			</tbody>
		</table>
		</div>
		<div id="conteudo" style="font-weight:normal;vertical-align:top; FONT: 8pt Arial,verdana">
			<?=html_entity_decode($texto)?>
		</div>
	</body>
	<table width="100%" align="center">
		<tr><td style="text-align: center;" class="div_rolagem">
				<input type="button" value="Imprimir" onclick="window.print();">
				<input type="button" value="Fechar" onclick="window.close();">
			</td></tr>
	</table>
</html>