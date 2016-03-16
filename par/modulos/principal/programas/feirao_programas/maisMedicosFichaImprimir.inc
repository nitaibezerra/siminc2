<?php
include_once '_funcoes_maismedicos.php';

if($_SESSION['par']['rqmid']){
	$infor = visualizarInfor($_SESSION['par']['rqmid']);
	extract($infor);
}

$sus_representante = verificarExisteGestorRepres();
$parceria = verificarParceria();
$adesao = verificarAdesao();
$ficha = recuperaDadosFicha($_SESSION['par']['rqmid']);
extract($ficha);

$mes = date('m');
switch ($mes){
	case 1: $mes = "Janeiro"; break;
	case 2: $mes = "Fevereiro"; break;
	case 3: $mes = "Março"; break;
	case 4: $mes = "Abril"; break;
	case 5: $mes = "Maio"; break;
	case 6: $mes = "Junho"; break;
	case 7: $mes = "Julho"; break;
	case 8: $mes = "Agosoto"; break;
	case 9: $mes = "Setembro"; break;
	case 10: $mes = "Outubro"; break;
	case 11: $mes = "Novembro"; break;
	case 12: $mes = "Dezembro"; break;
}
?>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<html>
<head>
    <title>Mais Médicos - Ficha de Avaliação Técnico</title>
    <style>
        @media print {
                .notprint {
                    display: none;
                }
                .div_rolagem {
                    display: none;
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
            padding: 5px;
            -moz-border-radius:3px 3px;
            -webkit-border-radius:3px 3px;
            border-radius:3px 3px;
            width:98%;
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
<table border="0" width="105%" cellspacing="1" cellpadding="5" border="0" align="center">
 	<tr>
 		<td>
			<table width="105%" border="0" cellpadding="0" cellspacing="0" class="notscreen1 debug">
				<tr bgcolor="#ffffff">
					<td valign="top" align="center">
						<img src="../imagens/brasao.gif" width="45" height="45" border="0">
						<br><b>MINISTÉRIO DA EDUCAÇÃO<br/>
						SECRETARIA DE REGULAÇÃO E SUPERVISÃO DA EDUCAÇÃO SUPERIOR</b> <br />
					</td>
				</tr>
			</table>
 		</td>
 	</tr>
</table>
<br>
<table border="0" width="105%" cellspacing="1" cellpadding="5" border="0" align="center">
 	<tr>
 		<td>
			<table width="105%" border="0" cellpadding="0" cellspacing="0" class="notscreen1 debug">
				<tr bgcolor="#ffffff">
					<td valign="top" align="center">
						<p align="center" style="margin-bottom: 0px;"><font face="Arial, sans-serif" size="2"><b>Ficha de Avaliação</b></font></p><br>
					</td>
				</tr>
				<tr bgcolor="#ffffff">
					<td valign="top" align="center">
						<p align="center" style="margin-bottom: 0px;"><font face="Arial, sans-serif" size="2"><b><?php echo recuperaMunicipioEstado(); ?></b></font></p><br>
					</td>
				</tr>
			</table>
 		</td>
 	</tr>
</table>
<table id="termo" width="105%" align="center" border="0" cellpadding="3" cellspacing="1">
	<tr>
		<td style="font-size: 12px; font-family:arial;">
			<table align="center" border="0" cellspacing="1" cellpadding="3" width="90%">
				<tr>
					<td align="left"><b><font size="2">1. PRIMEIRA ETAPA DA PRÉ-SELEÇÃO</font></b></td>
				</tr>
				<tr>
					<td align="left"><b>O município atende aos critérios abaixo:</b></td>
				</tr>
				<tr>
					<td align="center">
						<div class="bordaarredonda">
						<font size="2" style="text-align: justify;"><b>1.1 Ter 70 mil ou mais habitantes?</b>&nbsp;<?php echo $rqmquestao03 > 70000 ? 'Sim' : 'Não'; ?></font>
						<p align="justify"><font size="2"><?php echo $rqmobsaval70mil; ?></font></p>
						</div>
						<br>
						<div class="bordaarredonda">
						<font size="2" style="text-align: justify;"><b>1.2 Não se constitui capital de estado?</b>&nbsp;<?php echo $rqmquestao04 == 't' ? 'Sim' : 'Não'; ?></font>
						<p align="justify"><font size="2"><?php echo $rqmobsavacapitalestado; ?></font></p>
						</div>
						<br>
						<div class="bordaarredonda">
						<font size="2" style="text-align: justify;"><b>1.3 Não possui curso de medicina em seu território?</b>&nbsp;<?php echo $rqmquestao05 == 't' ? 'Sim' : 'Não'; ?></font>
						<p align="justify"><font size="2"><?php echo $rqmobsavalcursomedicina; ?></font></p>
						</div>
					</td>
				</tr>
				<tr>
					<td height="25"></td>
				</tr>
				<tr>
					<td align="left"><b><font size="2">2. SEGUNDA ETAPA DA PRÉ-ANÁLISE</font></b></td>
				</tr>
				<tr>
					<td align="center">
						<div class="bordaarredonda">
						<font size="2" style="text-align: justify;"><b>2.1 número de leitos disponíveis SUS por aluno maior ou igual a 5 (cinco), ou seja, para um curso com 50 vagas, o município deverá possuir, no mínimo, 250 leitos disponíveis SUS?</b>&nbsp;<?php echo $rqmquestao06 > 250 ? 'Sim' : 'Não'; ?></font>
						<p align="justify"><font size="2"><?php echo $rqmobsaval250leitos; ?></font></p>
						</div>
						<br>
						<div class="bordaarredonda">
						<font size="2" style="text-align: justify;"><b>2.2 Estabeleceu parcerias?</b>&nbsp;<?php echo $parceria == 't' ? 'Sim' : 'Não'; ?></font></p>
						<p align="justify"><font size="2"><?php echo $rqmobsavalparceria; ?></font></p>
						</div>
						<br>
						<div class="bordaarredonda">
						<font size="2" style="text-align: justify;"><b>2.3 Número de alunos por equipe de atenção básica menor ou igual a 3 (três), considerando o mínimo de 17 (dezessete) equipes?</b>&nbsp;<?php echo $rqmquestao07 <= 3 ? 'checked="Sim' : 'Não'; ?></font>
						<p align="justify"><font size="2"><?php echo $rqmobsavalalunoequipbasica; ?></font></p>
						</div>
						<br>
						<div class="bordaarredonda">
						<font size="2" style="text-align: justify;"><b>2.4 Existência de leitos de urgência e emergência ou Pronto Socorro?</b>&nbsp;<?php echo $rqmquestao08 == 't' ? 'Sim' : 'Não'; ?></font></p>
						<p align="justify"><font size="2"><?php echo $rqmobsavalleitourgencia; ?></font></p>
						</div>
						<br>
						<div class="bordaarredonda">
						<font size="2" style="text-align: justify;"><b>2.5 Existência de, pelo menos 3 (três), Programas de Residência Médica nas especialidades prioritárias: (1) Clínica Médica; (2) Cirurgia; (3)Ginecologia-Obstetrícia; (4) Pediatria e (5) Medicina de Família e Comunidade?</b>&nbsp;<?php echo $rqmquestao10 == 't' ? 'Sim' : 'Não'; ?></font>
						<p align="justify"><font size="2"><?php echo $rqmobsavalresidmedica; ?></font></p>
						</div>
						<br>
						<div class="bordaarredonda">
						<font size="2" style="text-align: justify;"><b>2.6 Aderiu ao Termo de Compromisso de Residência?</b>&nbsp;<?php echo $rqmaceitetermoresidencia == 't' ? 'Sim' : 'Não'; ?></font>
						<p align="justify"><font size="2"><?php echo $rqmobstermoresidencia; ?></font></p>
						</div>
						<br>
						<div class="bordaarredonda">
						<font size="2" style="text-align: justify;"><b>2.7 Adesão pelo município ao Programa Nacional de Melhoria do Acesso e da Qualidade na Atenção Básica - PMAQ, do Ministério da Saúde?</b>&nbsp;<?php echo $rqmquestao11 == 't' ? 'Sim' : 'Não'; ?></font>
						<p align="justify"><font size="2"><?php echo $rqmobsavalpmaq; ?></font></p>
						</div>
						<br>
						<div class="bordaarredonda">
						<font size="2" style="text-align: justify;"><b>2.8 Existência de Centro de Atenção Psicossocial - CAPS?</b>&nbsp;<?php echo $rqmquestao12 == 't' ? 'Sim' : 'Não'; ?></font>
						<p align="justify"><font size="2"><?php echo $rqmobsavalcaps; ?></font></p>
						</div>
						<br>
						<div class="bordaarredonda">
						<font size="2" style="text-align: justify;"><b>2.9 Hospital de ensino ou unidade hospitalar com potencial para hospital de ensino, conforme legislação de regência?</b>&nbsp;<?php echo $rqmquestao13 == 't' ? 'Sim' : 'Não'; ?></font>
						<p align="justify"><font size="2"><?php echo $rqmobsavalhospensino; ?></font></p>
						</div>
						<br>
						<div class="bordaarredonda">
						<font size="2" style="text-align: justify;"><b>2.10 Existência de hospital com mais de 100 (cem) leitos exclusivos para o curso?</b>&nbsp;<?php echo $rqmquestao14 == 't' ? 'Sim' : 'Não'; ?></font>
						<p align="justify"><font size="2"><?php echo $rqmobsaval100leitos; ?></font></p>
						</div>
						<br>
						<div class="bordaarredonda">
						<font size="2" style="text-align: justify;"><b>2.11 Indicou o gestor local do sus e o representante legal?</b>&nbsp;<?php echo $sus_representante == 't' ? 'Sim' : 'Não'; ?></font>
						<p align="justify"><font size="2"><?php echo $rqmobsavalgestorlocal; ?></font></p>
						</div>
						<br>
						<div class="bordaarredonda">
						<font size="2" style="text-align: justify;"><b>2.12 Aderiu ao Termo de Adesão de municípios?</b>&nbsp;<?php echo $adesao == 'S' ? 'Sim' : 'Não'; ?></font></p>
						<p align="justify"><font size="2"><?php echo $rqmobsavaltermomunic; ?></font></p>
						</div>
						<br>
						<div class="bordaarredonda">
						<font size="2" style="text-align: justify;"><b>2.13 Apresentou a documentação comprobatória?</b>&nbsp;<?php echo $rqmsitdoccomprobatoria == 't' ? 'Sim' : 'Não'; ?></font>
							<div class="bordaarredonda">
							<font size="2" style="text-align: justify;"><b>a) oficio de apresentaçao apresentado assinado pelo dirigente municipal?</b>&nbsp;<?php echo $rqmoficiodirigente == 't' ? 'Sim' : 'Não'; ?></font>
							<p align="justify"><font size="2"><?php echo $rqmobsavaldocoficio; ?></font></p>
							</div>
							<br>
							<div class="bordaarredonda">
							<font size="2" style="text-align: justify;"><b>b) cópia do documento de RG e CPF do dirigente municipal e do gestor local do SUS?</b>&nbsp;<?php echo $rqmcopiargcpfdir == 't' ? 'Sim' : 'Não'; ?></font>
							<p align="justify"><font size="2"><?php echo $rqmobsavaldocrgcpf; ?></font></p>
							</div>
							<br>
							<div class="bordaarredonda">
							<font size="2" style="text-align: justify;"><b>c) Projeto de melhorias da estrutura de equipamentos e programas de saúde existentes no município?</b>&nbsp;<?php echo $rqmprojetomelhoria == 't' ? 'Sim' : 'Não'; ?></font>
							<p align="justify"><font size="2"><?php echo $rqmobsavalprojetomelhoria; ?></font></p>
							</div>
							<br>
							<div class="bordaarredonda">
							<font size="2" style="text-align: justify;"><b>d) documentos comprobatórios dos itens 3.1 e 3.3 do Edital?</b>&nbsp;<?php echo $rqmdocitem3133 == 't' ? 'Sim' : 'Não'; ?></font>
							<p align="justify"><font size="2"><?php echo $rqmobsavalitens3133; ?></font></p>
							</div>
							<br>
							<div class="bordaarredonda">
							<font size="2" style="text-align: justify;"><b>e) documentos comprobatórios dos itens 4.1, 4.2 e 4.3 do Edital?</b>&nbsp;<?php echo $rqmdocitem414243 == 't' ? 'Sim' : 'Não'; ?></font>
							<p align="justify"><font size="2"><?php echo $rqmobsaval414243; ?></font></p>
							</div>
						</div>
						<br>
						<div class="bordaarredonda">
						<font size="2" style="text-align: justify;"><b>2.14 Síntese da avaliação?</font>
						<p align="justify"><font size="2"><?php echo $rqmsintesesegundaetapa; ?></font></p>
						</div>
						<br>
					</td>
				</tr>
				<tr>
					<td height="25"></td>
				</tr>
				<tr>
					<td align="left"><b><font size="2">3. TERCEIRA ETAPA DA PRÉ-ANÁLISE</font></b></td>
				</tr>
				<tr>
					<td align="left"><b>O projeto de melhorias atende aos objetivos do edital nº 03, quanto à:</b></td>
				</tr>
				<tr>
					<td align="center">
						<div class="bordaarredonda">
						<font size="2" style="text-align: justify;"><b>3.1 Ampliação de equipes de atenção básica?</b>&nbsp;<?php echo $ampliacaoequipebasica; ?></font>
						<p align="justify"><font size="2"><?php echo $rqmobsampliacaoequipebasica; ?></font></p>
						</div>
						<br>
						<div class="bordaarredonda">
						<font size="2" style="text-align: justify;"><b>3.2 Ampliação de leitos disponíveis SUS (especificar o tipo de leito)?</b>&nbsp;<?php echo $ampliacaoleitos; ?></font>
						<p align="justify"><font size="2"><?php echo $rqmobsampliacaoleitos; ?></font></p>
						</div>
						<br>
						<div class="bordaarredonda">
						<font size="2" style="text-align: justify;"><b>3.3 Criação/ampliação de leitos de urgência e emergência?</b>&nbsp;<?php echo $leitosurgencia; ?></font>
						<p align="justify"><font size="2"><?php echo $rqmobsleitosurgencia; ?></font></p>
						</div>
						<br>
						<div class="bordaarredonda">
						<font size="2" style="text-align: justify;"><b>3.4 Hospital de ensino ou unidade com potencial de hospital de ensino?</b>&nbsp;<?php echo $hospitalensino; ?></font>
						<p align="justify"><font size="2"><?php echo $rqmobshospitalensino; ?></font></p>
						</div>
						<br>
						<div class="bordaarredonda">
						<font size="2" style="text-align: justify;"><b>3.5 Construção/Ampliação de hospital com leitos exclusivos para o curso?</b>&nbsp;<?php echo $hospitalleitoscurso; ?></font>
						<p align="justify"><font size="2"><?php echo $rqmobshospitalleitoscurso; ?></font></p>
						</div>
						<br>
						<div class="bordaarredonda">
						<font size="2" style="text-align: justify;"><b>3.6 Criação/ampliação de programas de saúde?</b>&nbsp;<?php echo $ampliacaoprogsaude; ?></font>
						<p align="justify"><font size="2"><?php echo $rqmobsampliacaoprogsaude; ?></font></p>
						</div>
						<br>
						<div class="bordaarredonda">
						<font size="2" style="text-align: justify;"><b>3.7 Outras informações?</b></font>
						<p align="justify"><font size="2"><?php echo $rqmoutrainfo; ?></font></p>
						</div>
						<br>
						<div class="bordaarredonda">
						<font size="2" style="text-align: justify;"><b>3.8 Síntese da avaliação e sugestão de deferimento ou indeferimento?</b></font>
						<p align="justify"><font size="2"><?php echo $rqmsinteseterceiraetapa; ?></font></p>
						</div>
						<br>
						<p align="right" style="margin-right: 0.98in;"><font size="2">Brasília-DF <?php echo date('d'); ?> de <?php echo $mes; ?> 2013</font></p>
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
<table id="termo" width="105%" align="center" border="0" cellpadding="3" cellspacing="1">
	<tr>
		<td class="div_rolagem" style="text-align: center;">
			<input type="button" name="impressao" id="impressao" value="Imprimir" onclick="window.print();" />
			<input type="button" name="fechar" id="fechar" value="Fechar" onclick="window.close();" />
		</td>
	</tr>
</table>
</body>
</html>