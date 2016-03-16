<?php
include_once '_funcoes_maismedicos.php';
$parecer = recuperaDadosParecer($_SESSION['par']['rqmid']);
extract($parecer);
?>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<html>
<head>
    <title>Mais Médicos - Parecer Técnico</title>
    <style>
        @media print {
            .notprint {
                display: none;
            }
            .div_rolagem {
                display: none
            }
        }
        .div_rolagem{
            overflow-x: auto;
            overflow-y: auto;
            height: 50px;
        }
        .notscreen {
            display: none;
        }
        .bordaarredonda{
            background:#FFFFFF;
            color:#000; border: #000 1px solid;
            padding: 10px;
            -moz-border-radius:10px 10px;
            -webkit-border-radius:10px 10px;
            border-radius:10px 10px;
            width:95%;
            text-align:left;
        }
        .quebra {
            page-break-after: always !important;
            height: 0px;
            clear: both;
        }
    </style>
</head>
<body>
<table border="0" width="85%" cellspacing="1" cellpadding="5" border="0" align="center">
 	<tr>
 		<td>
			<table width="95%" border="0" cellpadding="0" cellspacing="0" class="notscreen1 debug">
				<tr bgcolor="#ffffff">
					<td valign="top" align="center">
						<img src="../imagens/brasao.gif" width="45" height="45" border="0">
						<br><b>MINISTÉRIO DA EDUCAÇÃO<br/>
						SECRETARIA DE REGULAÇÃO E SUPERVISÃO DA EDUCAÇÃO SUPERIOR</b> <br />
					</td>
				</tr>
				<tr bgcolor="#ffffff">
					<td valign="top" align="center">
						<br />
						<p align="justify">PRIMEIRO EDITAL DE PRÉ-SELEÇÃO DE MUNICÍPIOS PARA IMPLANTAÇÃO DE CURSO DE GRADUAÇÃO EM MEDICINA
						POR INSTITUIÇÃO DE EDUCAÇÃO SUPERIOR PRIVADA</p>
					</td>
				</tr>
			</table>
 		</td>
 	</tr>
</table>
<br>
<table border="0" width="85%" cellspacing="1" cellpadding="5" border="0" align="center">
 	<tr>
 		<td>
			<table width="95%" border="0" cellpadding="0" cellspacing="0" class="notscreen1 debug">
				<tr bgcolor="#ffffff">
					<td valign="top" align="center">
						<p align="center" style="margin-bottom: 0px;"><font face="Arial, sans-serif" size="3"><b>Parecer Técnico</b></font></p><br>
					</td>
				</tr>
			</table>
 		</td>
 	</tr>
</table>
<br>
<table id="termo" width="95%" align="center" border="0" cellpadding="3" cellspacing="1">
	<tr>
		<td style="font-size: 12px; font-family:arial;">
			<table align="center" border="0" cellspacing="1" cellpadding="3" width="85%">
				<tr>
					<td align="center">
						<div class="bordaarredonda">
							<p align="justify"><font size="3"><b>1. Dados do município:</b><?php echo str_replace("-","/",recuperaMunicipioEstado()); ?></font></p>
						</div>
						<br>
						<div class="bordaarredonda">
						<p align="justify"><font size="3"><b>2. Introdução</b></font></p>
						<p align="justify"><font size="3"><?php echo $rqmparintroducao; ?></font></p>
						</div>
						<br>
						<div class="bordaarredonda">
						<p align="justify"><font size="3"><b>3. Análise</b></font></p>
						<p align="justify"><font size="3"><?php echo $rqmparanalise; ?></font></p>
						</div>
						<br>
						<div class="bordaarredonda">
						<p align="justify"><font size="3"><b>4. Conclusão</b></font></p>
						<p align="justify"><font size="3"><?php echo $rqmparconclusao; ?></font></p>
						</div>
						<br>
						<p align="right" style="margin-right: 0.98in;"><font size="3">Brasília-DF____de____________2013</font></p>
					</td>
				</tr>
				<tr>
					<td><p style="text-align: right;">&nbsp;</p></td>
				</tr>
			</table>
		</td>
	</tr>
</table>
<br>
<table id="termo" width="95%" align="center" border="0" cellpadding="3" cellspacing="1">
	<tr>
		<td class="div_rolagem" style="text-align: center;">
			<input type="button" name="impressao" id="impressao" value="Imprimir" onclick="window.print();" />
			<input type="button" name="fechar" id="fechar" value="Fechar" onclick="window.close();" />
		</td>
	</tr>
</table>
</body>
</html>