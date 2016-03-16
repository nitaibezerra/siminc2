<?php
/******************** carrega as fun��es gerais *************************/
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "brasilpro/modulos/principal/classes/subacoes.inc";

/******************** Recupera nome do Estado *************************/
$db = new cls_banco();
$sql = "SELECT  e.estdescricao, e.estuf 
		FROM cte.instrumentounidade iu
			INNER JOIN territorios.estado e ON e.estuf = iu.estuf
		WHERE iu.inuid = ".$_SESSION['inuid'];

$nomeEstado = $db->carregar($sql);
unset($sql);

/******************** Recupera Quantitativos *************************/
$dimensao = array(1,2);
$area = array(2,3);
$indicador = array(1,2,3,4,5,6,7,8,9,10);
$formaExecucao = 3;

$relatorio = new subacoes;
$quantitativosCronograma = $relatorio->construct("qtdCronograma",$dimensao, $area, $indicador, $subacoes,$ano, $formaExecucao);

$dados = $quantitativosCronograma["qtdCronograma"];
foreach ($dados as $dados):
	$dados['dimcod'] = $dados['dimcod'] ? $dados['dimcod'] : '';
	$dados['ardcod'] = $dados['ardcod'] ? $dados['ardcod'] : '';
	$dados['indcod'] = $dados['indcod'] ? $dados['indcod'] : '';

	${"total_".$dados['dimcod'].$dados['ardcod']} = $dados['soma'];
	${"soma_".$dados['dimcod'].$dados['ardcod'].$dados['indcod']} = $dados['soma'];
endforeach;

?>

<html xmlns:v="urn:schemas-microsoft-com:vml"
xmlns:o="urn:schemas-microsoft-com:office:office"
xmlns:w="urn:schemas-microsoft-com:office:word"
xmlns="http://www.w3.org/TR/REC-html40">

<head>
<meta http-equiv=Content-Type content="text/html; charset=windows-1252">
<meta name=ProgId content=Word.Document>
<meta name=Generator content="Microsoft Word 9">
<meta name=Originator content="Microsoft Word 9">
<link rel=File-List href="./termocoptecnica_arquivos/filelist.xml">
<title>MINIST�RIO DA EDUCA��O</title>
<!--[if gte mso 9]><xml>
 <o:DocumentProperties>
  <o:Author>MariaChagas</o:Author>
  <o:LastAuthor>alexandredourado</o:LastAuthor>
  <o:Revision>2</o:Revision>
  <o:TotalTime>37</o:TotalTime>
  <o:LastPrinted>2008-08-18T19:42:00Z</o:LastPrinted>
  <o:Created>2008-08-18T17:45:00Z</o:Created>
  <o:LastSaved>2008-08-18T17:45:00Z</o:LastSaved>
  <o:Pages>6</o:Pages>
  <o:Words>2023</o:Words>
  <o:Characters>11532</o:Characters>
  <o:Company>MEC</o:Company>
  <o:Lines>96</o:Lines>
  <o:Paragraphs>23</o:Paragraphs>
  <o:CharactersWithSpaces>14162</o:CharactersWithSpaces>
  <o:Version>9.3821</o:Version>
 </o:DocumentProperties>
</xml><![endif]--><!--[if gte mso 9]><xml>
 <w:WordDocument>
  <w:HyphenationZone>21</w:HyphenationZone>
 </w:WordDocument>
</xml><![endif]-->
<style>
<!--
 /* Font Definitions */
@font-face
	{font-family:Helvetica;
	panose-1:2 11 6 4 2 2 2 2 2 4;
	mso-font-charset:0;
	mso-generic-font-family:swiss;
	mso-font-pitch:variable;
	mso-font-signature:536902279 -2147483648 8 0 511 0;}
@font-face
	{font-family:Tahoma;
	panose-1:2 11 6 4 3 5 4 4 2 4;
	mso-font-charset:0;
	mso-generic-font-family:swiss;
	mso-font-pitch:variable;
	mso-font-signature:1627421319 -2147483648 8 0 66047 0;}
 /* Style Definitions */
p.MsoNormal, li.MsoNormal, div.MsoNormal
	{mso-style-parent:"";
	margin:0cm;
	margin-bottom:.0001pt;
	mso-pagination:widow-orphan;
	font-size:12.0pt;
	font-family:"Times New Roman";
	mso-fareast-font-family:"Times New Roman";}
h1
	{mso-style-next:Normal;
	margin:0cm;
	margin-bottom:.0001pt;
	mso-pagination:widow-orphan;
	page-break-after:avoid;
	mso-outline-level:1;
	font-size:12.0pt;
	font-family:"Times New Roman";
	mso-font-kerning:0pt;
	font-weight:bold;
	font-style:italic;}
h2
	{mso-style-next:Normal;
	margin:0cm;
	margin-bottom:.0001pt;
	text-align:justify;
	text-indent:35.4pt;
	mso-pagination:widow-orphan;
	page-break-after:avoid;
	mso-outline-level:2;
	mso-layout-grid-align:none;
	text-autospace:none;
	font-size:12.0pt;
	font-family:Arial;
	font-weight:bold;}
h3
	{mso-style-next:Normal;
	margin:0cm;
	margin-bottom:.0001pt;
	text-align:justify;
	mso-pagination:widow-orphan;
	page-break-after:avoid;
	mso-outline-level:3;
	font-size:12.0pt;
	font-family:"Times New Roman";
	font-weight:bold;}
p.MsoFooter, li.MsoFooter, div.MsoFooter
	{margin:0cm;
	margin-bottom:.0001pt;
	mso-pagination:widow-orphan;
	tab-stops:center 220.95pt right 441.9pt;
	font-size:12.0pt;
	font-family:"Times New Roman";
	mso-fareast-font-family:"Times New Roman";}
p.MsoBodyText, li.MsoBodyText, div.MsoBodyText
	{margin:0cm;
	margin-bottom:.0001pt;
	text-align:justify;
	mso-pagination:widow-orphan;
	font-size:12.0pt;
	font-family:"Times New Roman";
	mso-fareast-font-family:"Times New Roman";}
p.MsoBodyTextIndent, li.MsoBodyTextIndent, div.MsoBodyTextIndent
	{margin-top:0cm;
	margin-right:0cm;
	margin-bottom:0cm;
	margin-left:225.0pt;
	margin-bottom:.0001pt;
	text-align:justify;
	mso-pagination:widow-orphan;
	font-size:12.0pt;
	font-family:"Times New Roman";
	mso-fareast-font-family:"Times New Roman";
	font-weight:bold;}
p.MsoBodyTextIndent2, li.MsoBodyTextIndent2, div.MsoBodyTextIndent2
	{margin:0cm;
	margin-bottom:.0001pt;
	text-align:justify;
	text-indent:35.4pt;
	mso-pagination:widow-orphan;
	mso-layout-grid-align:none;
	text-autospace:none;
	font-size:12.0pt;
	font-family:"Times New Roman";
	mso-fareast-font-family:"Times New Roman";}
p
	{margin-right:0cm;
	mso-margin-top-alt:auto;
	mso-margin-bottom-alt:auto;
	margin-left:0cm;
	mso-pagination:widow-orphan;
	font-size:12.0pt;
	font-family:"Times New Roman";
	mso-fareast-font-family:"Times New Roman";}
p.Textodebalo, li.Textodebalo, div.Textodebalo
	{mso-style-name:"Texto de bal�o";
	margin:0cm;
	margin-bottom:.0001pt;
	mso-pagination:widow-orphan;
	font-size:8.0pt;
	font-family:Tahoma;
	mso-fareast-font-family:"Times New Roman";}
 /* Page Definitions */
@page
	{mso-footnote-separator:url("./termocoptecnica_arquivos/header.htm") fs;
	mso-footnote-continuation-separator:url("./termocoptecnica_arquivos/header.htm") fcs;
	mso-endnote-separator:url("./termocoptecnica_arquivos/header.htm") es;
	mso-endnote-continuation-separator:url("./termocoptecnica_arquivos/header.htm") ecs;}
@page Section1
	{size:612.0pt 792.0pt;
	margin:70.85pt 3.0cm 70.85pt 3.0cm;
	mso-header-margin:35.4pt;
	mso-footer-margin:35.4pt;
	mso-even-footer:url("./termocoptecnica_arquivos/header.htm") ef1;
	mso-footer:url("./termocoptecnica_arquivos/header.htm") f1;
	mso-paper-source:0;}
div.Section1
	{page:Section1;}
 /* List Definitions */
@list l0
	{mso-list-id:570121866;
	mso-list-type:hybrid;
	mso-list-template-ids:477369902 480429446 68550681 68550683 68550671 68550681 68550683 68550671 68550681 68550683;}
@list l0:level1
	{mso-level-number-format:alpha-lower;
	mso-level-text:"%1\)";
	mso-level-tab-stop:56.25pt;
	mso-level-number-position:left;
	margin-left:56.25pt;
	text-indent:-21.0pt;}
@list l1
	{mso-list-id:1153910426;
	mso-list-type:hybrid;
	mso-list-template-ids:-1934184482 1910816676 68550681 68550683 68550671 68550681 68550683 68550671 68550681 68550683;}
@list l1:level1
	{mso-level-number-format:alpha-lower;
	mso-level-text:"%1\)";
	mso-level-tab-stop:53.25pt;
	mso-level-number-position:left;
	margin-left:53.25pt;
	text-indent:-18.0pt;}
@list l2
	{mso-list-id:1984237767;
	mso-list-type:hybrid;
	mso-list-template-ids:987293976 -601481756 68550681 68550683 68550671 68550681 68550683 68550671 68550681 68550683;}
@list l2:level1
	{mso-level-number-format:roman-upper;
	mso-level-tab-stop:54.0pt;
	mso-level-number-position:left;
	margin-left:54.0pt;
	text-indent:-36.0pt;}
@list l3
	{mso-list-id:2086680047;
	mso-list-type:hybrid;
	mso-list-template-ids:-626758484 44104568 68550681 68550683 68550671 68550681 68550683 68550671 68550681 68550683;}
@list l3:level1
	{mso-level-number-format:alpha-lower;
	mso-level-text:"%1\)";
	mso-level-tab-stop:53.25pt;
	mso-level-number-position:left;
	margin-left:53.25pt;
	text-indent:-18.0pt;}
ol
	{margin-bottom:0cm;}
ul
	{margin-bottom:0cm;}
-->
</style>
<!--[if gte mso 9]><xml>
 <o:shapedefaults v:ext="edit" spidmax="2050"/>
</xml><![endif]--><!--[if gte mso 9]><xml>
 <o:shapelayout v:ext="edit">
  <o:idmap v:ext="edit" data="1"/>
 </o:shapelayout></xml><![endif]-->
</head>

<body lang=PT-BR style='tab-interval:35.4pt'>

<div class=Section1>

<p class=MsoBodyTextIndent>TERMO DE COOPERA��O T�CNICA QUE ENTRE SI CELEBRAM A
UNI�O, REPRESENTADA PELO MINIST�RIO DA EDUCA��O, POR INTERM�DIO DA SECRETARIA
DE EDUCA��O PROFISSIONAL E TECNOL�GICA (SETEC) E A SECRETARIA ESTADUAL DO <b style='text-transform: uppercase' ><?=$nomeEstado[0]['estdescricao']; ?> (<?=$nomeEstado[0]['estuf']; ?>) </b> OBJETIVANDO A IMPLEMENTA��O DO PROGRAMA BRASIL
PROFISSIONALIZADO NO �MBITO DO PLANO DE DESENVOLVIMENTO DA EDUCA��O, NA FORMA
SEGUINTE:</p>

<p class=MsoNormal style='margin-left:225.0pt;text-align:justify'><b><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></b></p>

<p class=MsoNormal style='margin-left:225.0pt;text-align:justify'><b><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></b></p>

<p class=MsoNormal style='text-align:justify'>A Uni�o, por meio do <b>MINIST�RIO
DA EDUCA��O</b>, inscrito no CNPJ/MF sob o n� ...., localizado na<span
style="mso-spacerun: yes">� </span>Esplanada dos Minist�rios, Bloco L,
Bras�lia-DF, doravante denominado MEC, neste ato representado por seu titular,
Ministro de Estado de Educa��o, Senhor <b>FERNANDO HADDAD</b>, nomeado pelo
Decreto Presidencial n�..., de 00/00/2000, brasileiro, casado, portador da
Carteira de Identidade n� ....... e CPF n� ........, com domic�lio especial na
Esplanada dos Minist�rios, Bloco L, 8� andar, Bras�lia-DF,<span
style="mso-spacerun: yes">�� </span>tendo como interveniente a <b>SECRETARIA DE
EDUCA��O PROFISSIONAL E TECNOL�GICA</b>, doravante denominada SETEC, neste ato
representada por seu titular, Secret�rio de Educa��o Profissional e
Tecnol�gica, Senhor <b>ELIEZER MOREIRA PACHECO</b>, nomeado pelo Decreto
Presidencial n�, de 00/00/2000, brasileiro, casado, portador da Carteira de
Identidade n� ....... e CPF n� ........, com domic�lio especial na Esplanada
dos Minist�rios, Bloco L, 4� andar, Bras�lia-DF,<span style="mso-spacerun:
yes">� </span>e a <b>SECRETARIA DE ESTADO DO XXXXX</b>, inscrita no CNPJ/MF sob
o n�<span style="mso-spacerun: yes">� </span>...., localizada na ......,<span
style="mso-spacerun: yes">� </span>doravante denominada SE/RS, neste ato
representada por seu titular, o Secret�rio de Estado....., nomeado pelo.....,
de 00/00/2000, brasileiro, casado, portador da Carteira de Identidade n�
....... e CPF n� ........, com domic�lio especial na ..... <b style='mso-bidi-font-weight:
normal'>XXXXX</b>, considerando que: </p>

<p style='text-align:justify'><span style='mso-bidi-font-size:10.0pt'>I - a
Constitui��o Federal, no �1� do art. 211, estabelece que <span
style='color:black'>a Uni�o organizar� o sistema federal de ensino e o dos
Territ�rios, financiar� as institui��es de ensino p�blicas federais e exercer�,
em mat�ria educacional, fun��o redistributiva e supletiva, de forma a garantir
equaliza��o de oportunidades educacionais e padr�o m�nimo de qualidade do
ensino mediante assist�ncia t�cnica e financeira aos Estados, ao Distrito
Federal e aos Munic�pios; </span><o:p></o:p></span></p>

<p style='text-align:justify'><span style='mso-bidi-font-size:10.0pt'>II - o
Decreto n� 6.094, de 24 de abril de 2007, disp�e sobre a implementa��o do Plano
de Metas Compromisso Todos pela Educa��o, pela Uni�o Federal, em regime de
colabora��o com Munic�pios, Distrito Federal e Estados, e a participa��o das
fam�lias e da comunidade, mediante programas e a��es de assist�ncia t�cnica e
financeira, visando a mobiliza��o social pela melhoria da qualidade da educa��o
b�sica;<o:p></o:p></span></p>

<p style='text-align:justify'><span style='mso-bidi-font-size:10.0pt'>III - o
Decreto n� 6.032, de 12 de dezembro de 2007, institui o Programa Brasil
Profissionalizado;<o:p></o:p></span></p>

<p style='text-align:justify'><span style='mso-bidi-font-size:10.0pt'>IV - a
Resolu��o/FNDE/CD/n� 062, de 12 de dezembro de 2007, estabelece as diretrizes
para a assist�ncia financeira a Estados, Distrito Federal e Munic�pios no
�mbito do Programa Brasil Profissionalizado;<o:p></o:p></span></p>

<p class=MsoNormal style='text-align:justify;mso-layout-grid-align:none;
text-autospace:none'><span style='mso-bidi-font-size:10.0pt'>V - a
Resolu��o/CD/FNDE n� 09, de 29 de fevereiro de 2008, </span><span
style='mso-bidi-font-size:9.5pt'>altera os artigos 1�, 2�, � 3�, 5�, Inciso III
e 8�, � 2�, da Resolu��o CD/FNDE n� 62, de 12 de dezembro de 2007, que
estabelece as diretrizes.<o:p></o:p></span></p>

<p class=MsoNormal style='text-align:justify;mso-layout-grid-align:none;
text-autospace:none'><span style='mso-bidi-font-size:9.5pt'>para a assist�ncia
financeira a Estados, Distrito Federal e Munic�pios, no �mbito do Programa
Brasil Profissionalizado; <o:p></o:p></span></p>

<p class=MsoNormal style='text-align:justify;mso-layout-grid-align:none;
text-autospace:none'><span style='mso-bidi-font-size:9.5pt'><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></span></p>

<p class=MsoNormal style='text-align:justify;mso-layout-grid-align:none;
text-autospace:none'><span style='mso-bidi-font-size:9.5pt'>VI �<span
style="mso-spacerun: yes">� </span>o Termo de Ades�o ao Compromisso Todos pela
Educa��o assinado pelo Estado do <?=$nomeEstado[0]['estdescricao']; ?>, firma o compromisso de
promover a melhoria da qualidade da educa��o b�sica em sua esfera de
compet�ncia, implementando as diretrizes relacionadas no Decreto </span><span
style='mso-bidi-font-size:10.0pt'>n� 6.094, de 24 de abril de 2007, e a��es que
levem ao cumprimento das metas de evolu��o do �ndice de Desenvolvimento da
Educa��o B�sica (IDEB);<o:p></o:p></span></p>

<p class=MsoNormal style='text-align:justify;mso-layout-grid-align:none;
text-autospace:none'><span style='mso-bidi-font-size:10.0pt'><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></span></p>

<p class=MsoNormal style='text-align:justify;mso-layout-grid-align:none;
text-autospace:none'><span style='mso-bidi-font-size:10.0pt'>VII - </span><span
style='mso-bidi-font-size:9.5pt'>o Plano de A��es Articuladas - PAR do Estado
do <?=$nomeEstado[0]['estdescricao']; ?>, estabelece a��es para o alcance da melhoria da educa��o
com base no diagn�stico da situa��o educacional estruturado em quatro
dimens�es: gest�o educacional; forma��o de professores e dos profissionais de
servi�o e apoio escolar; pr�ticas pedag�gicas e avalia��o; e infra-estrutura
f�sica e recursos pedag�gicos.</span><span style='font-size:10.0pt;font-family:
Helvetica'><o:p></o:p></span></p>

<p class=MsoNormal style='text-align:justify'><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></p>

<p class=MsoNormal style='text-align:justify'>Resolvem celebrar o presente <b>TERMO
DE COOPERA��O T�CNICA</b>, mediante as cl�usulas e condi��es seguintes: </p>

<p class=MsoNormal style='text-align:justify'><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></p>

<p class=MsoNormal style='text-align:justify'><b>CL�USULA PRIMEIRA</b> <b>� DO
OBJETO</b></p>

<p class=MsoNormal style='text-align:justify'><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></p>

<p class=MsoNormal style='text-align:justify'><span style='mso-tab-count:1'>����������� </span>O
presente Termo de Coopera��o T�cnica tem por objeto a conjuga��o de esfor�os
entre as partes para a implementa��o do Programa Brasil Profissionalizado no
�mbito do Plano de Desenvolvimento da Educa��o como forma de ampliar e
qualificar a oferta de educa��o profissional e tecnol�gica de n�vel m�dio no
Estado do <?=$nomeEstado[0]['estdescricao']; ?>. </p>

<p class=MsoNormal style='text-align:justify'><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></p>

<p class=MsoNormal style='text-align:justify'><b>PAR�GRAFO �NICO</b> � A
implementa��o do Programa Brasil Profissionalizado no Estado do Rio Grande do
Sul se dar� por interm�dio da execu��o das seguintes a��es:</p>

<p class=MsoNormal style='text-align:justify'><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></p>

<p class=MsoNormal style='margin-left:54.0pt;text-align:justify;text-indent:
-36.0pt;mso-list:l2 level1 lfo1;tab-stops:list 54.0pt'><![if !supportLists]>I.<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span><![endif]>organizar a educa��o profissional e tecnol�gica de n�vel
t�cnico no sistema de ensino;</p>

<p class=MsoNormal style='margin-left:54.0pt;text-align:justify;text-indent:
-36.0pt;mso-list:l2 level1 lfo1;tab-stops:list 54.0pt'><![if !supportLists]>II.<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span><![endif]>promover a gest�o democr�tica no desenvolvimento da educa��o
profissional e tecnol�gica;</p>

<p class=MsoNormal style='margin-left:54.0pt;text-align:justify;text-indent:
-36.0pt;mso-list:l2 level1 lfo1;tab-stops:list 54.0pt'><![if !supportLists]>III.<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span><![endif]>fortalecer a implementa��o coletiva do projeto pol�tico
pedag�gico das institui��es de educa��o profissional e tecnol�gica;</p>

<p class=MsoNormal style='margin-left:54.0pt;text-align:justify;text-indent:
-36.0pt;mso-list:l2 level1 lfo1;tab-stops:list 54.0pt'><![if !supportLists]>IV.<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span><![endif]>implantar plano de carreira, cargos e sal�rios para os
profissionais da educa��o profissional e tecnol�gica, privilegiando o m�rito, a
forma��o e a avalia��o do desempenho;</p>

<p class=MsoNormal style='margin-left:54.0pt;text-align:justify;text-indent:
-36.0pt;mso-list:l2 level1 lfo1;tab-stops:list 54.0pt'><![if !supportLists]>V.<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span><![endif]>promover a forma��o e qualifica��o de professores e dos profissionais
de servi�o e apoio escolar; </p>

<p class=MsoNormal style='margin-left:54.0pt;text-align:justify;text-indent:
-36.0pt;mso-list:l2 level1 lfo1;tab-stops:list 54.0pt'><![if !supportLists]>VI.<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span><![endif]>desenvolver o ensino m�dio com refor�o da base cient�fica,
tecnol�gica e sua liga��o com o mundo do trabalho;</p>

<p class=MsoNormal style='margin-left:54.0pt;text-align:justify;text-indent:
-36.0pt;mso-list:l2 level1 lfo1;tab-stops:list 54.0pt'><![if !supportLists]>VII.<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span><![endif]>promover o ensino m�dio integrado a educa��o profissional e
tecnol�gica </p>

<p class=MsoNormal style='margin-left:54.0pt;text-align:justify;text-indent:
-36.0pt;mso-list:l2 level1 lfo1;tab-stops:list 54.0pt'><![if !supportLists]>VIII.<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span><![endif]>implantar e ampliar a oferta de vaga no �mbito do Programa
Nacional de Integra��o da Educa��o B�sica na Modalidade de Educa��o de Jovens e
Adultos;</p>

<p class=MsoNormal style='margin-left:54.0pt;text-align:justify;text-indent:
-36.0pt;mso-list:l2 level1 lfo1;tab-stops:list 54.0pt'><![if !supportLists]>IX.<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span><![endif]>implantar e ampliar a oferta de vaga por meio da educa��o a
dist�ncia;</p>

<p class=MsoNormal style='margin-left:54.0pt;text-align:justify;text-indent:
-36.0pt;mso-list:l2 level1 lfo1;tab-stops:list 54.0pt'><![if !supportLists]>X.<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span><![endif]>realizar a��es conjuntas, visando a implementa��o de um plano
de monitoramento das a��es do Programa Brasil Profissionalizado;</p>

<p class=MsoNormal style='margin-left:54.0pt;text-align:justify;text-indent:
-36.0pt;mso-list:l2 level1 lfo1;tab-stops:list 54.0pt'><![if !supportLists]>XI.<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span><![endif]><span style="mso-spacerun: yes">�</span>adequar cursos e
programas �s normas e boas pr�ticas pedag�gicas;</p>

<p class=MsoNormal style='margin-left:54.0pt;text-align:justify;text-indent:
-36.0pt;mso-list:l2 level1 lfo1;tab-stops:list 54.0pt'><![if !supportLists]>XII.<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span><![endif]>promover a melhoria da infra-estrutura escolar e de seus
recursos pedag�gicos.</p>

<p class=MsoNormal style='text-align:justify'><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></p>

<h3>CL�USULA SEGUNDA -<span style="mso-spacerun: yes">� </span>DAS OBRIGA��ES<span
style='mso-tab-count:1'>������ </span></h3>

<p class=MsoNormal style='text-align:justify'><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></p>

<p class=MsoNormal style='text-align:justify'>Obrigam-se os part�cipes a
promover os meios necess�rios humanos e materiais para o cumprimento do
disposto no presente TERMO.</p>

<p class=MsoNormal style='text-align:justify'><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></p>

<p class=MsoNormal style='text-align:justify'><span style='mso-tab-count:1'>����������� </span>I
- Compete conjuntamente aos part�cipes:</p>

<p class=MsoNormal style='text-align:justify'><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></p>

<p class=MsoNormal style='margin-left:53.25pt;text-align:justify;text-indent:
-18.0pt;mso-list:l3 level1 lfo2;tab-stops:list 53.25pt'><![if !supportLists]>a)<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; </span><![endif]>desenvolver,
elaborar e prover apoio t�cnico �s a��es para a implementa��o do objeto do
presente Termo;</p>

<p class=MsoNormal style='margin-left:53.25pt;text-align:justify;text-indent:
-18.0pt;mso-list:l3 level1 lfo2;tab-stops:list 53.25pt'><![if !supportLists]>b)<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; </span><![endif]>exercer
a articula��o interinstitucional, nos �mbitos federal, estadual e municipal,
para viabiliza��o do objeto do presente Termo;</p>

<p class=MsoNormal style='margin-left:53.25pt;text-align:justify;text-indent:
-18.0pt;mso-list:l3 level1 lfo2;tab-stops:list 53.25pt'><![if !supportLists]>c)<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; </span><![endif]>disponibilizar
dados e informa��es t�cnicas necess�rias � implementa��o do objeto do presente
Termo;</p>

<p class=MsoNormal style='margin-left:53.25pt;text-align:justify;text-indent:
-18.0pt;mso-list:l3 level1 lfo2;tab-stops:list 53.25pt'><![if !supportLists]>d)<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; </span><![endif]>acompanhar
e avaliar os resultados alcan�ados<span style="mso-spacerun: yes">� </span>nas
atividades objeto do presente Termo, visando � otimiza��o e/ou adequa��o quando
necess�rios;</p>

<p class=MsoNormal style='margin-left:53.25pt;text-align:justify;text-indent:
-18.0pt;mso-list:l3 level1 lfo2;tab-stops:list 53.25pt'><![if !supportLists]>e)<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; </span><![endif]>conduzir
todas as a��es com efici�ncia e dentro de pr�ticas administrativas,
financeiras, t�cnicas adequadas;</p>

<p class=MsoNormal style='margin-left:53.25pt;text-align:justify;text-indent:
-18.0pt;mso-list:l3 level1 lfo2;tab-stops:list 53.25pt'><![if !supportLists]>f)<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span><![endif]>estabelecer uma programa��o m�nima de atividades relativas �
presente coopera��o;</p>

<p class=MsoNormal style='margin-left:53.25pt;text-align:justify;text-indent:
-18.0pt;mso-list:l3 level1 lfo2;tab-stops:list 53.25pt'><![if !supportLists]>g)<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; </span><![endif]>assegurar
aos seus representantes designados, a qualquer tempo, o acesso � documenta��o
necess�ria � efetiva��o do objeto do presente Termo.</p>

<p class=MsoNormal style='margin-left:35.25pt;text-align:justify'><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></p>

<p class=MsoNormal style='margin-left:35.25pt;text-align:justify'>II � Compete
ao Minist�rio da Educa��o:</p>

<p class=MsoNormal style='margin-left:35.25pt;text-align:justify'><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></p>

<p class=MsoNormal style='margin-left:53.25pt;text-align:justify;text-indent:
-18.0pt;mso-list:l1 level1 lfo3;tab-stops:list 53.25pt'><![if !supportLists]>a)<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; </span><![endif]>prestar
assist�ncia financeira ao Estado nas a��es de desenvolvimento e estrutura��o do
ensino m�dio integrado � educa��o profissional e tecnol�gica, com �nfase na
educa��o cient�fica e human�stica, e tamb�m �s escolas que oferecem cursos
subseq�entes e concomitantes que estejam integradas aos arranjos produtivos
locais e regionais;</p>

<p class=MsoNormal style='margin-left:53.25pt;text-align:justify;text-indent:
-18.0pt;mso-list:l1 level1 lfo3;tab-stops:list 53.25pt'><![if !supportLists]>b)<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; </span><![endif]>apoiar
tecnicamente o Estado nas a��es de desenvolvimento e estrutura��o do ensino
m�dio integrado � educa��o profissional e tecnol�gica, com �nfase na educa��o
cient�fica e human�stica, e tamb�m �s escolas que oferecem cursos subseq�entes
e concomitantes que estejam integradas aos arranjos produtivos locais e
regionais;</p>

<p class=MsoNormal style='margin-left:53.25pt;text-align:justify;text-indent:
-18.0pt;mso-list:l1 level1 lfo3;tab-stops:list 53.25pt'><![if !supportLists]>c)<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; </span><![endif]>promover
a articula��o interinstucional para o desenvolvimento do objeto do presente
Termo;</p>

<p class=MsoNormal style='margin-left:53.25pt;text-align:justify;text-indent:
-18.0pt;mso-list:l1 level1 lfo3;tab-stops:list 53.25pt'><![if !supportLists]>d)<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; </span><![endif]>apoiar
a cria��o e a implementa��o de programas de inser��o social e/ou inicia��o
cient�fica;</p>

<p class=MsoNormal style='margin-left:53.25pt;text-align:justify;text-indent:
-18.0pt;mso-list:l1 level1 lfo3;tab-stops:list 53.25pt'><![if !supportLists]>e)<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; </span><![endif]>apoiar<span
style="mso-spacerun: yes">� </span>a cria��o e a implementa��o de programas de
forma��o, capacita��o e qualifica��o de docentes, gestores e t�cnicos
administrativos;</p>

<p class=MsoNormal style='margin-left:53.25pt;text-align:justify;text-indent:
-18.0pt;mso-list:l1 level1 lfo3;tab-stops:list 53.25pt'><![if !supportLists]>f)<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span><![endif]>apoiar a cria��o e a implementa��o de programas de empreendedorismo/cooperativismo,
como Empresa J�nior, Hotel Tecnol�gico e Incubadora;</p>

<p class=MsoNormal style='margin-left:53.25pt;text-align:justify;text-indent:
-18.0pt;mso-list:l1 level1 lfo3;tab-stops:list 53.25pt'><![if !supportLists]>g)<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; </span><![endif]>enviar
equipamentos para instalar laborat�rios padr�es MEC/SETEC/FNDE, cient�ficos e
tecnol�gicos, inclusive salas de apoio presencial para educa��o a dist�ncia,
nas escolas devidamente adequadas ao seu recebimento, em conformidade com o
Manual Operacional e Modelo de Refer�ncia dispostos no Sistema Integrado de
Planejamento, Or�amento e Finan�as do Minist�rio da Educa��o;</p>

<p class=MsoNormal style='margin-left:53.25pt;text-align:justify;text-indent:
-18.0pt;mso-list:l1 level1 lfo3;tab-stops:list 53.25pt'><![if !supportLists]>h)<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; </span><![endif]>prestar
assist�ncia financeira para constru��o de Escolas T�cnicas Padr�o
MEC/SETEC/FNDE em conformidade com o Modelo de Refer�ncia disposto no Sistema
Integrado de Planejamento, Or�amento e Finan�as do Minist�rio da Educa��o;</p>

<p class=MsoNormal style='margin-left:53.25pt;text-align:justify;text-indent:
-18.0pt;mso-list:l1 level1 lfo3;tab-stops:list 53.25pt'><![if !supportLists]>i)<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span><![endif]>fornecer informa��es e dados t�cnicos<span
style="mso-spacerun: yes">� </span>necess�rios ao desenvolvimento das a��es do
objeto do presente Termo;</p>

<p class=MsoNormal style='margin-left:53.25pt;text-align:justify;text-indent:
-18.0pt;mso-list:l1 level1 lfo3;tab-stops:list 53.25pt'><![if !supportLists]>j)<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span><![endif]>apoiar e acompanhar as a��es do part�cipe integrante do
presente Termo;</p>

<p class=MsoNormal style='margin-left:53.25pt;text-align:justify;text-indent:
-18.0pt;mso-list:l1 level1 lfo3;tab-stops:list 53.25pt'><![if !supportLists]>k)<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; </span><![endif]>apoiar
a promo��o de eventos para a forma��o, divulga��o e dissemina��o das a��es do
objeto do presente Termo;</p>

<p class=MsoNormal style='margin-left:53.25pt;text-align:justify;text-indent:
-18.0pt;mso-list:l1 level1 lfo3;tab-stops:list 53.25pt'><![if !supportLists]>l)<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span><![endif]>prestar orienta��es suplementares quanto � metodologia a ser
adotada no planejamento, na execu��o dos trabalhos e na emiss�o dos relat�rios
e demais orienta��es pertinentes a este Termo.</p>

<p class=MsoNormal style='text-align:justify'><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></p>

<p class=MsoNormal style='margin-left:35.25pt;text-align:justify'>III � Compete
ao Estado </p>

<p class=MsoNormal style='margin-left:35.25pt;text-align:justify'><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></p>

<p class=MsoNormal style='margin-left:56.25pt;text-align:justify;text-indent:
-21.0pt;mso-list:l0 level1 lfo4;tab-stops:list 56.25pt'><![if !supportLists]>a)<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span><![endif]>implementar as a��es para o desenvolvimento do Programa Brasil
Profissionalizado, considerando o diagn�stico realizado e o Plano de A��es
Articulada - PAR;</p>

<p class=MsoNormal style='margin-left:56.25pt;text-align:justify;text-indent:
-21.0pt;mso-list:l0 level1 lfo4;tab-stops:list 56.25pt'><![if !supportLists]>b)<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span><![endif]>ofertar <b style='mso-bidi-font-weight:normal'><?= $total_13 ? $total_13 : 0; ?></b>
vagas de educa��o profissional e tecnol�gica nos pr�ximos quatro anos;</p>

<p class=MsoNormal style='margin-left:56.25pt;text-align:justify;text-indent:
-21.0pt;mso-list:l0 level1 lfo4;tab-stops:list 56.25pt'><![if !supportLists]>c)<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span><![endif]>ofertar <b style='mso-bidi-font-weight:normal'><?= ( ($soma_131 ? $soma_131 : 0) + ($soma_132 ? $soma_132 : 0) ); ?></b> vagas de ensino m�dio integrado regular;</p>

<p class=MsoNormal style='margin-left:56.25pt;text-align:justify;text-indent:
-21.0pt;mso-list:l0 level1 lfo4;tab-stops:list 56.25pt'><![if !supportLists]>d)<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span><![endif]>ofertar <b style='mso-bidi-font-weight:normal'><?= $soma_133 ? $soma_133 : 0 ?></b>
<span style="mso-spacerun: yes">�</span>vagas <span style="mso-spacerun:
yes">�</span>de ensino m�dio integrado ind�gena;</p>

<p class=MsoNormal style='margin-left:56.25pt;text-align:justify;text-indent:
-21.0pt;mso-list:l0 level1 lfo4;tab-stops:list 56.25pt'><![if !supportLists]>e)<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span><![endif]>ofertar <b style='mso-bidi-font-weight:normal'><?= $soma_134 ? $soma_134 : 0 ?></b>
<span style="mso-spacerun: yes">�</span>vagas de ensino m�dio integrado para
quilombolas, ribeirinhos e comunidades tradicionais</p>

<p class=MsoNormal style='margin-left:56.25pt;text-align:justify;text-indent:
-21.0pt;mso-list:l0 level1 lfo4;tab-stops:list 56.25pt'><![if !supportLists]>f)<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span><![endif]>ofertar <b style='mso-bidi-font-weight:normal'><?= $soma_135 ? $soma_135 : 0 ?></b>
<span style="mso-spacerun: yes">�</span>vagas de ensino m�dio integrado para
escolas do campo;</p>

<p class=MsoNormal style='margin-left:56.25pt;text-align:justify;text-indent:
-21.0pt;mso-list:l0 level1 lfo4;tab-stops:list 56.25pt'><![if !supportLists]>g)<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span><![endif]>ofertar <b style='mso-bidi-font-weight:normal'><?= $soma_136 ? $soma_136 : 0 ?></b>
<span style="mso-spacerun: yes">�</span>vagas de ensino m�dio integrado para
jovens e adolescentes em conflito com a lei;</p>

<p class=MsoNormal style='margin-left:56.25pt;text-align:justify;text-indent:
-21.0pt;mso-list:l0 level1 lfo4;tab-stops:list 56.25pt'><![if !supportLists]>h)<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span><![endif]>ofertar <b style='mso-bidi-font-weight:normal'><?= $soma_137 ? $soma_137 : 0 ?></b>
<span style="mso-spacerun: yes">�</span>vagas de ensino m�dio integrado na
modalidade educa��o de jovens e adultos-PROEJA;</p>

<p class=MsoNormal style='margin-left:56.25pt;text-align:justify;text-indent:
-21.0pt;mso-list:l0 level1 lfo4;tab-stops:list 56.25pt'><![if !supportLists]>i)<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span><![endif]>ofertar <b style='mso-bidi-font-weight:normal'><?= $soma_138 ? $soma_138 : 0 ?></b>
<span style="mso-spacerun: yes">�</span>vagas de ensino m�dio integrado por meio
da educa��o a dist�ncia no �mbito do Programa e-Tec Brasil;</p>

<p class=MsoNormal style='margin-left:56.25pt;text-align:justify;text-indent:
-21.0pt;mso-list:l0 level1 lfo4;tab-stops:list 56.25pt'><![if !supportLists]>j)<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span><![endif]>ofertar <b style='mso-bidi-font-weight:normal'><?= ( ($soma_139 ? $soma_139 : 0) + ($soma_1310 ? $soma_1310 : 0) ) ?></b> <span style="mso-spacerun: yes">�</span>vagas de ensino m�dio
integrado na forma subseq�ente e concomitante;</p>

<p class=MsoNormal style='margin-left:56.25pt;text-align:justify;text-indent:
-21.0pt;mso-list:l0 level1 lfo4;tab-stops:list 56.25pt'><![if !supportLists]>k)<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span><![endif]>contratar <b style='mso-bidi-font-weight:normal'><?= $soma_124 ? $soma_124 : 0 ?></b>
<span style="mso-spacerun: yes">�</span>professores para atender a expans�o da
educa��o profissional e tecnol�gica nos pr�ximos quatro anos;</p>

<p class=MsoNormal style='margin-left:56.25pt;text-align:justify;text-indent:
-21.0pt;mso-list:l0 level1 lfo4;tab-stops:list 56.25pt'><![if !supportLists]>l)<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span><![endif]>elaborar o projeto pol�tico e pedag�gico de x escolas;</p>

<p class=MsoNormal style='margin-left:56.25pt;text-align:justify;text-indent:
-21.0pt;mso-list:l0 level1 lfo4;tab-stops:list 56.25pt'><![if !supportLists]>m)<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; </span><![endif]>oferecer
cursos para forma��o inicial de <b style='mso-bidi-font-weight:normal'><?= $soma_121 ? $soma_121 : 0 ?></b> professores de educa��o cient�fica, profissional e tecnol�gica;</p>

<p class=MsoNormal style='margin-left:56.25pt;text-align:justify;text-indent:
-21.0pt;mso-list:l0 level1 lfo4;tab-stops:list 56.25pt'><![if !supportLists]>n)<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span><![endif]>oferecer cursos para forma��o continuada de professores <b
style='mso-bidi-font-weight:normal'><?= ( ($soma_221 ? $soma_221 : 0) + ($soma_223 ? $soma_223 : 0) + ($soma_224 ? $soma_224 : 0) + ($soma_225 ? $soma_225 : 0) ) ?></b> </p>

<p class=MsoNormal style='margin-left:56.25pt;text-align:justify;text-indent:
-21.0pt;mso-list:l0 level1 lfo4;tab-stops:list 56.25pt'><![if !supportLists]><b
style='mso-bidi-font-weight:normal'>o)<span style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span></b><![endif]>oferecer cursos para forma��o continuada de profissionais
de servi�o e apoio escolar de educa��o profissional e tecnol�gica professores
de educa��o cient�fica, profissional e tecnol�gica; <b style='mso-bidi-font-weight:
normal'><?= ( ($soma_226 ? $soma_226 : 0) + ($soma_227 ? $soma_227 : 0) + ($soma_228 ? $soma_228 : 0) ) ?><o:p></o:p></b></p>

<p class=MsoNormal style='margin-left:56.25pt;text-align:justify;text-indent:
-21.0pt;mso-list:l0 level1 lfo4;tab-stops:list 56.25pt'><![if !supportLists]>p)<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span><![endif]>adotar pr�ticas pedag�gicas e de avalia��o educacional;</p>

<p class=MsoNormal style='margin-left:56.25pt;text-align:justify;text-indent:
-21.0pt;mso-list:l0 level1 lfo4;tab-stops:list 56.25pt'><![if !supportLists]>q)<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span><![endif]>adequar os cursos t�cnicos ao Cat�logo Nacional de Cursos
T�cnicos;</p>

<p class=MsoNormal style='margin-left:56.25pt;text-align:justify;text-indent:
-21.0pt;mso-list:l0 level1 lfo4;tab-stops:list 56.25pt'><![if !supportLists]>r)<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span><![endif]>proporcionar condi��es e estimular para que 100% as escolas de
ensino m�dio tenham est�gio curricular integrado; </p>

<p class=MsoNormal style='margin-left:56.25pt;text-align:justify;text-indent:
-21.0pt;mso-list:l0 level1 lfo4;tab-stops:list 56.25pt'><![if !supportLists]>s)<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span><![endif]>fomentar a cria��o de programas de inicia��o cient�fica no
�mbito do ensino m�dio;</p>

<p class=MsoNormal style='margin-left:56.25pt;text-align:justify;text-indent:
-21.0pt;mso-list:l0 level1 lfo4;tab-stops:list 56.25pt'><![if !supportLists]>t)<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span><![endif]>adequar a infra-estrutura f�sica das institui��es de educa��o
profissional e tecnol�gica;</p>

<p class=MsoNormal style='margin-left:56.25pt;text-align:justify;text-indent:
-21.0pt;mso-list:l0 level1 lfo4;tab-stops:list 56.25pt'><![if !supportLists]>u)<span
style='font:7.0pt "Times New Roman"'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</span><![endif]>equipar, manter e dar funcionalidade aos recursos pedag�gicos
das institui��es de educa��o profissional e tecnol�gica.</p>

<p class=MsoNormal style='text-align:justify'><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></p>

<h3>CL�USULA TERCEIRA � DA<span style="mso-spacerun: yes">� </span>EXECU��O e
RESPONSABILIDADE</h3>

<p class=MsoNormal style='text-align:justify'><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></p>

<p class=MsoNormal style='text-align:justify'><span style='mso-tab-count:1'>����������� </span>I
� As a��es decorrentes do presente Termo ser�o executadas pelos �rg�os
definidos em suas respectivas estruturas administrativas, com coopera��o
mediante a solicita��o rec�proca;</p>

<p class=MsoNormal style='text-align:justify'><span style='mso-tab-count:1'>����������� </span>II
- Os part�cipes responder�o pelo conte�do t�cnico dos trabalhos executados por
for�a do presente Termo e assumir�o total responsabilidade pela qualidade do
mesmo. </p>

<p class=MsoNormal style='text-align:justify'><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></p>

<h3>CL�USULA QUARTA � DA EFIC�CIA E DA VIG�NCIA</h3>

<p class=MsoNormal style='text-align:justify'><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></p>

<p class=MsoNormal style='text-align:justify'><span style='mso-tab-count:1'>����������� </span>Este
Termo ter� vig�ncia de 04 anos a partir da data de sua publica��o em Di�rio
Oficial da Uni�o, por extrato, podendo ser prorrogado por igual per�odo,
mediante aditamento, limitado � dura��o do Programa Brasil Profissionalizado.</p>

<p class=MsoNormal style='text-align:justify'><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></p>

<h3>CL�USULA QUINTA � DA DEN�NCIA E DA RESCIS�O</h3>

<p class=MsoNormal style='text-align:justify'><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></p>

<p class=MsoBodyText><span style='mso-tab-count:1'>����������� </span>Os
Part�cipes podem rescindir unilateralmente este Termo, denunci�-lo qualquer
tempo, sendo-lhes imputadas as responsabilidades das obriga��es do prazo que
tenha vigido e creditando-lhes, igualmente, os benef�cios adquiridos no mesmo
per�odo.</p>

<p class=MsoNormal style='text-align:justify'><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></p>

<p class=MsoNormal style='text-align:justify'><b>PAR�GRAFO PRIMEIRO </b>� O
presente Termo poder� ser rescindido, de comum acordo entre os part�cipes ou
por inadimpl�ncia de quaisquer cl�usulas condi��es, mediante notifica��o
escrita, com anteced�ncia m�nima de 60 dias de conformidade com a legisla��o em
vigor.</p>

<p class=MsoNormal style='text-align:justify'><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></p>

<p class=MsoNormal style='text-align:justify'><b>PAR�GRAFO SEGUNDO</b> � Na
hip�tese mencionada no caput desta cl�usula, ficar� assegurado o prosseguimento
e conclus�o dos trabalhos em curso, salvo decis�o contr�ria acordada entre os
part�cipes.<span style='font-size:10.0pt;font-family:Arial'><o:p></o:p></span></p>

<p class=MsoNormal style='text-align:justify'><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></p>

<p class=MsoNormal style='text-align:justify'><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></p>

<h3>CL�USULA SEXTA -<span style="mso-spacerun: yes">� </span>DAS ALTERA��ES E
DA PUBLICA��O </h3>

<p class=MsoNormal style='text-align:justify'><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></p>

<p class=MsoNormal style='mso-layout-grid-align:none;text-autospace:none'><b>PAR�GRAFO
PRIMEIRO </b>- O Presente termo poder� ser alterado, de comum acordo entre as
partes mediante � assinatura de Termo Aditivo, obedecidas �s disposi��es legais
aplic�veis.<span style='font-size:10.0pt'><o:p></o:p></span></p>

<p class=MsoNormal style='text-align:justify'><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></p>

<p class=MsoNormal style='mso-layout-grid-align:none;text-autospace:none'><b>PAR�GRAFO
SEGUNDO</b> - A publica��o deste Acordo ser� efetuada em extrato, no Di�rio
Oficial da Uni�o e no Di�rio Oficial do Estado, nos termos do par�grafo �nico
do art.61 da Lei n� 8.666, de 1993 e do art 17, �caput�, da IN/STN n�1 de 1997.<span
style='font-size:10.0pt'><o:p></o:p></span></p>

<p class=MsoNormal style='text-align:justify'><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></p>

<p class=MsoNormal style='text-align:justify'><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></p>

<p class=MsoNormal style='mso-layout-grid-align:none;text-autospace:none'><b>CL�SULA
S�TIMA � DO FORO<o:p></o:p></b></p>

<p class=MsoNormal style='mso-layout-grid-align:none;text-autospace:none'><b><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></b></p>

<p class=MsoNormal style='text-align:justify;text-indent:35.4pt;mso-layout-grid-align:
none;text-autospace:none'>As d�vidas e controv�rsias porventura surgidas na
execu��o deste</p>

<p class=MsoNormal style='text-align:justify;mso-layout-grid-align:none;
text-autospace:none'>Termo de Coopera��o T�cnica, que n�o possam ser dirimidas
administrativamente, ser�o apreciadas e julgadas no Foro da Justi�a Federal,</p>

<p class=MsoBodyText style='mso-layout-grid-align:none;text-autospace:none'>da
Se��o Judici�ria do Distrito Federal.</p>

<p class=MsoNormal style='text-align:justify;mso-layout-grid-align:none;
text-autospace:none'><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></p>

<p class=MsoBodyTextIndent2>E por estarem de pleno acordo com as cl�usulas e
condi��es expressas neste Termo, os participantes citados, o firmaram em tr�s
vias, de igual teor e forma, para que produzam entre si os efeitos legais, na
presen�a das testemunhas, que tamb�m o subscrevem.</p>

<p class=MsoNormal style='text-align:justify;text-indent:35.4pt;mso-layout-grid-align:
none;text-autospace:none'><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></p>

<p class=MsoNormal style='text-align:justify;text-indent:35.4pt;mso-layout-grid-align:
none;text-autospace:none'><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></p>

<p class=MsoNormal align=right style='text-align:right;text-indent:35.4pt;
mso-layout-grid-align:none;text-autospace:none'>Bras�lia-DF, 19 de junho de
2008.</p>

<p class=MsoNormal align=right style='text-align:right;text-indent:35.4pt;
mso-layout-grid-align:none;text-autospace:none'><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></p>

<p class=MsoNormal align=right style='text-align:right;text-indent:35.4pt;
mso-layout-grid-align:none;text-autospace:none'><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></p>

<p class=MsoNormal style='text-align:justify;text-indent:35.4pt;mso-layout-grid-align:
none;text-autospace:none'><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></p>

<h2><span style='font-family:"Times New Roman"'>FERNADO HADDAD<o:p></o:p></span></h2>

<p class=MsoNormal style='text-align:justify;text-indent:35.4pt;mso-layout-grid-align:
none;text-autospace:none'>Ministro de Estado de Educa��o </p>

<p class=MsoNormal style='text-align:justify;text-indent:35.4pt;mso-layout-grid-align:
none;text-autospace:none'><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></p>

<p class=MsoNormal style='text-align:justify;text-indent:35.4pt;mso-layout-grid-align:
none;text-autospace:none'><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></p>

<p class=MsoNormal style='text-align:justify;text-indent:35.4pt;mso-layout-grid-align:
none;text-autospace:none'><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></p>

<h2><span style='font-family:"Times New Roman"'>ELIEZER MOREIRA PACHECO<o:p></o:p></span></h2>

<p class=MsoNormal style='text-align:justify;text-indent:35.4pt;mso-layout-grid-align:
none;text-autospace:none'>Secret�rio de Educa��o Profissional e Tecnol�gica</p>

<p class=MsoNormal style='text-align:justify;text-indent:35.4pt;mso-layout-grid-align:
none;text-autospace:none'><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></p>

<p class=MsoNormal style='text-align:justify;text-indent:35.4pt;mso-layout-grid-align:
none;text-autospace:none'><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></p>

<h2><span style='font-family:"Times New Roman"'>XXXXX<o:p></o:p></span></h2>

<p class=MsoNormal style='text-align:justify;text-indent:35.4pt;mso-layout-grid-align:
none;text-autospace:none'>Secret�rio de Estado de Educa��o do XXXXX</p>

<p class=MsoNormal style='text-align:justify;text-indent:35.4pt;mso-layout-grid-align:
none;text-autospace:none'><![if !supportEmptyParas]>&nbsp;<![endif]><o:p></o:p></p>

<p class=MsoNormal style='text-align:justify;text-indent:35.4pt;mso-layout-grid-align:
none;text-autospace:none'>XXXXX</p>

<p class=MsoNormal style='text-align:justify;text-indent:35.4pt;mso-layout-grid-align:
none;text-autospace:none'>Secretaria de Estado de Ci�ncia e Tecnologia
(Interveniente)</p>

</div>

</body>

</html>
