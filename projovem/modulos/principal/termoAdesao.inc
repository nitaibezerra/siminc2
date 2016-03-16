<?php

if($_REQUEST['req']) {
	$_REQUEST['req']($_REQUEST);
	exit;
}



include_once APPRAIZ . 'includes/cabecalho.inc';
echo '<br>';


$sql = "SELECT mundescricao as municipio, estuf as uf
				FROM territorios.municipio
				WHERE muncod = '".$_SESSION['projovem']['muncod']."'";
$mun = $db->pegaLinha($sql);


monta_titulo('Termo de Adesão', $mun['municipio'].' - '.$mun['uf']);

?>

<form id="form" nme="form" method="POST">

<center>
<table bgcolor="#f5f5f5" align="center" class="tabela" >
	<tr>
		<td width="90%" align="center">
			<br />
			<div style="overflow: auto; height:400px; width: 70%; background-color: rgb(250,250,250); border-width:1px; border-style:solid;" align="center" >
				<div style="width: 95%; magin-top: 10px; ">
					<br />
					
					
					
					
					
											
						<style>
						<!--
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
							text-align:center;
							mso-pagination:widow-orphan;
							page-break-after:avoid;
							mso-outline-level:1;
							mso-layout-grid-align:none;
							text-autospace:none;
							font-size:11.0pt;
							font-family:Arial;
							mso-font-kerning:0pt;}
						h2
							{mso-style-next:Normal;
							margin:0cm;
							margin-bottom:.0001pt;
							text-align:center;
							mso-pagination:widow-orphan;
							page-break-after:avoid;
							mso-outline-level:2;
							mso-layout-grid-align:none;
							text-autospace:none;
							font-size:12.0pt;
							font-family:"Times New Roman";}
						h3
							{mso-style-next:Normal;
							margin:0cm;
							margin-bottom:.0001pt;
							text-align:center;
							line-height:150%;
							mso-pagination:widow-orphan;
							page-break-after:avoid;
							mso-outline-level:3;
							mso-layout-grid-align:none;
							text-autospace:none;
							font-size:10.0pt;
							font-family:Arial;}
						p.MsoBodyText, li.MsoBodyText, div.MsoBodyText
							{margin:0cm;
							margin-bottom:.0001pt;
							text-align:justify;
							mso-pagination:widow-orphan;
							mso-layout-grid-align:none;
							text-autospace:none;
							font-size:10.0pt;
							font-family:Arial;
							mso-fareast-font-family:"Times New Roman";}
						@page Section1
							{size:612.0pt 792.0pt;
							margin:70.85pt 3.0cm 70.85pt 3.0cm;
							mso-header-margin:36.0pt;
							mso-footer-margin:36.0pt;
							mso-paper-source:0;}
						div.Section1
							{page:Section1;}
						-->
						</style>
						<!--[if gte mso 10]>
						<style>
						 /* Style Definitions */
						 table.MsoNormalTable
							{mso-style-name:"Tabela normal";
							mso-tstyle-rowband-size:0;
							mso-tstyle-colband-size:0;
							mso-style-noshow:yes;
							mso-style-parent:"";
							mso-padding-alt:0cm 5.4pt 0cm 5.4pt;
							mso-para-margin:0cm;
							mso-para-margin-bottom:.0001pt;
							mso-pagination:widow-orphan;
							font-size:10.0pt;
							font-family:"Times New Roman";
							mso-ansi-language:#0400;
							mso-fareast-language:#0400;
							mso-bidi-language:#0400;}
						</style>
						<![endif]--><!--[if gte mso 9]><xml>
						 <o:shapedefaults v:ext="edit" spidmax="2050"/>
						</xml><![endif]--><!--[if gte mso 9]><xml>
						 <o:shapelayout v:ext="edit">
						  <o:idmap v:ext="edit" data="1"/>
						 </o:shapelayout></xml><![endif]-->
						</head>
						
						<body lang=PT-BR style='tab-interval:35.4pt'>
						
						<div class=Section1>
						
						<div style='mso-element:frame;mso-element-frame-hspace:7.05pt;mso-element-wrap:
						around;mso-element-anchor-vertical:paragraph;mso-element-anchor-horizontal:
						margin;mso-element-top:-9.3pt;mso-height-rule:exactly'>
						
						<table cellspacing=0 cellpadding=0 hspace=0 vspace=0 align=left>
						 <tr>
						  <td valign=top align=left style='padding-top:0cm;padding-right:7.05pt;
						  padding-bottom:0cm;padding-left:7.05pt'>
						  <p class=MsoNormal align=center style='text-align:center;mso-element:frame;
						  mso-element-frame-hspace:7.05pt;mso-element-wrap:around;mso-element-anchor-vertical:
						  paragraph;mso-element-anchor-horizontal:margin;mso-element-top:-9.3pt;
						  mso-height-rule:exactly'><!--[if gte vml 1]><v:shapetype id="_x0000_t75"
						   coordsize="21600,21600" o:spt="75" o:preferrelative="t" path="m@4@5l@4@11@9@11@9@5xe"
						   filled="f" stroked="f">
						   <v:stroke joinstyle="miter"/>
						   <v:formulas>
						    <v:f eqn="if lineDrawn pixelLineWidth 0"/>
						    <v:f eqn="sum @0 1 0"/>
						    <v:f eqn="sum 0 0 @1"/>
						    <v:f eqn="prod @2 1 2"/>
						    <v:f eqn="prod @3 21600 pixelWidth"/>
						    <v:f eqn="prod @3 21600 pixelHeight"/>
						    <v:f eqn="sum @0 0 1"/>
						    <v:f eqn="prod @6 1 2"/>
						    <v:f eqn="prod @7 21600 pixelWidth"/>
						    <v:f eqn="sum @8 21600 0"/>
						    <v:f eqn="prod @7 21600 pixelHeight"/>
						    <v:f eqn="sum @10 21600 0"/>
						   </v:formulas>
						   <v:path o:extrusionok="f" gradientshapeok="t" o:connecttype="rect"/>
						   <o:lock v:ext="edit" aspectratio="t"/>
						  </v:shapetype><v:shape id="_x0000_i1025" type="#_x0000_t75" style='width:58.5pt;
						   height:63pt' o:ole="" fillcolor="window">
						   <v:imagedata src="Backup%20de%20TERMO%20DE%20ADESÃO%20-%20PJU%20-%20MEC%20-%20Estados-%2025-08-2011%20(2)_arquivos/image001.wmz"
						    o:title=""/>
						  </v:shape><![endif]--><![if !vml]><![endif]><!--[if gte mso 9]><xml>
						   <o:OLEObject Type="Embed" ProgID="Word.Picture.8" ShapeID="_x0000_i1025"
						    DrawAspect="Content" ObjectID="_1379254178">
						   </o:OLEObject>
						  </xml><![endif]--></p>
						  </td>
						 </tr>
						</table>
						
						</div>
						
						<center><img width=78 height=84 src="/imagens/brasao.gif"></center>
						
						
						<p class=MsoNormal align=center style='text-align:center;mso-layout-grid-align:
						none;text-autospace:none'><b><span style='font-family:Arial'><o:p>&nbsp;</o:p></span></b></p>
						
						<h2><span style='font-family:Arial'>MINISTÉRIO DA EDUCAÇÃO<o:p></o:p></span></h2>
						
						<h1><span style='font-size:12.0pt'>SECRETARIA DE EDUCAÇÃO CONTINUADA,
						ALFABETIZAÇÃO, <o:p></o:p></span></h1>
						
						<h1><span style='font-size:12.0pt'>DIVERSIDADE E INCLUSÃO<o:p></o:p></span></h1>
						
						<h1 style='line-height:150%'><span style='font-size:12.0pt;line-height:150%'><o:p>&nbsp;</o:p></span></h1>
						
						<h1 style='line-height:150%'><span style='font-size:12.0pt;line-height:150%'>TERMO
						DE ADESÃO<o:p></o:p></span></h1>
						
						<p class=MsoNormal style='line-height:150%;mso-layout-grid-align:none;
						text-autospace:none'><span style='font-family:Arial'><o:p>&nbsp;</o:p></span></p>
						
						<p class=MsoBodyText style='line-height:150%'><span style='font-size:12.0pt;
						line-height:150%'>O Estado do XXXXXX, doravante denominado Estado, representado
						por seu (sua) Secretário(a) de Educação, XXXXXXX, CPF nº XXXXXXXXXX, RG nº
						XXXXXXXX, expedido pela XXXXXX, devidamente estabelecido à XXXXXXXXXXXXXXXXXX,
						resolve firmar o presente Termo de Adesão ao <a name="OLE_LINK3">Programa Nacional
						de Inclusão de Jovens – Projovem Urbano</a>, consideradas as seguintes
						condições:<o:p></o:p></span></p>
						
						<p class=MsoNormal style='line-height:150%;mso-layout-grid-align:none;
						text-autospace:none'><span style='font-family:Arial'><o:p>&nbsp;</o:p></span></p>
						
						<p class=MsoNormal style='text-align:justify;line-height:150%;mso-layout-grid-align:
						none;text-autospace:none'><b><span style='font-family:Arial'>CLÁUSULA PRIMEIRA
						– Do Objeto<o:p></o:p></span></b></p>
						
						<p class=MsoNormal style='text-align:justify;line-height:150%;mso-layout-grid-align:
						none;text-autospace:none'><span style='font-family:Arial'>O presente termo tem
						por objeto a adesão do Estado ao Programa Nacional de Inclusão de Jovens – Projovem
						Urbano, instituído nos termos da Lei nº 11.692 de 10 de junho de 2008 e
						regulamentado pelo Decreto nº 6.629 de 04 de novembro de 2008.<o:p></o:p></span></p>
						
						<p class=MsoNormal style='text-align:justify;line-height:150%;mso-layout-grid-align:
						none;text-autospace:none'><span style='font-family:Arial'><o:p>&nbsp;</o:p></span></p>
						
						<p class=MsoNormal style='text-align:justify;line-height:150%;mso-layout-grid-align:
						none;text-autospace:none'><span style='font-family:Arial'><o:p>&nbsp;</o:p></span></p>
						
						<p class=MsoNormal style='text-align:justify;line-height:150%;mso-layout-grid-align:
						none;text-autospace:none'><b><span style='font-family:Arial'>CLÁUSULA SEGUNDA –
						O Estado se compromete a:<o:p></o:p></span></b></p>
						
						<p class=MsoNormal style='text-align:justify;line-height:150%;mso-layout-grid-align:
						none;text-autospace:none'><span style='font-family:Arial'><o:p>&nbsp;</o:p></span></p>
						
						<p class=MsoBodyText style='line-height:150%'><span style='font-size:12.0pt;
						line-height:150%'>1. Atingir a seguinte meta de atendimento de jovens para o
						Projovem Urbano, no período de 2012:<o:p></o:p></span></p>
						
						<p class=MsoBodyText style='line-height:150%'><span style='font-size:12.0pt;
						line-height:150%'><o:p>&nbsp;</o:p></span></p>
						
						<div align=center>
						
						<table class=MsoNormalTable border=1 cellspacing=0 cellpadding=0
						 style='border-collapse:collapse;border:none;mso-border-alt:solid windowtext .5pt;
						 mso-padding-alt:0cm 3.5pt 0cm 3.5pt;mso-border-insideh:.5pt solid windowtext;
						 mso-border-insidev:.5pt solid windowtext'>
						 <tr style='mso-yfti-irow:0;mso-yfti-firstrow:yes'>
						  <td width=200 valign=top style='width:149.65pt;border:solid windowtext 1.0pt;
						  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt'>
						  <p class=MsoNormal align=center style='text-align:center;line-height:150%;
						  mso-layout-grid-align:none;text-autospace:none'><b><span style='font-family:
						  Arial'>Ano<o:p></o:p></span></b></p>
						  </td>
						  <td width=200 valign=top style='width:149.65pt;border:solid windowtext 1.0pt;
						  border-left:none;mso-border-left-alt:solid windowtext .5pt;mso-border-alt:
						  solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt'>
						  <p class=MsoNormal align=center style='text-align:center;line-height:150%;
						  mso-layout-grid-align:none;text-autospace:none'><b><span style='font-family:
						  Arial'>2012<o:p></o:p></span></b></p>
						  </td>
						 </tr>
						 <tr style='mso-yfti-irow:1;mso-yfti-lastrow:yes'>
						  <td width=200 valign=top style='width:149.65pt;border:solid windowtext 1.0pt;
						  border-top:none;mso-border-top-alt:solid windowtext .5pt;mso-border-alt:solid windowtext .5pt;
						  padding:0cm 3.5pt 0cm 3.5pt'>
						  <p class=MsoNormal align=center style='text-align:center;line-height:150%;
						  mso-layout-grid-align:none;text-autospace:none'><b><span style='font-family:
						  Arial'>Meta<o:p></o:p></span></b></p>
						  </td>
						  <td width=200 valign=top style='width:149.65pt;border-top:none;border-left:
						  none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
						  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
						  mso-border-alt:solid windowtext .5pt;padding:0cm 3.5pt 0cm 3.5pt'>
						  <p class=MsoNormal style='text-align:justify;line-height:150%;mso-layout-grid-align:
						  none;text-autospace:none'><b><span style='font-family:Arial'><o:p>&nbsp;</o:p></span></b></p>
						  </td>
						 </tr>
						</table>
						
						</div>
						
						<p class=MsoNormal style='text-align:justify;line-height:150%;mso-layout-grid-align:
						none;text-autospace:none'><span style='font-family:Arial'><o:p>&nbsp;</o:p></span></p>
						
						<p class=MsoNormal style='text-align:justify;line-height:150%;mso-layout-grid-align:
						none;text-autospace:none'><span style='font-family:Arial'><o:p>&nbsp;</o:p></span></p>
						
						<p class=MsoNormal style='text-align:justify;line-height:150%;mso-layout-grid-align:
						none;text-autospace:none'><span style='font-family:Arial'>2. Cumprir as
						seguintes diretrizes abaixo:<o:p></o:p></span></p>
						
						<p class=MsoNormal style='text-align:justify;line-height:150%;mso-layout-grid-align:
						none;text-autospace:none'><span style='font-family:Arial'><o:p>&nbsp;</o:p></span></p>
						
						<p class=MsoNormal style='text-align:justify;line-height:150%;mso-layout-grid-align:
						none;text-autospace:none'><span style='font-family:Arial'>I - estabelecer como
						foco a aprendizagem, realizando todos os esforços necessários para garantir a
						certificação <st4:PersonName ProductID="em Ensino Fundamental" w:st="on">em Ensino
						 Fundamental</st4:PersonName> – EJA, em qualificação profissional como formação
						inicial dos alunos matriculados no Projovem Urbano;<o:p></o:p></span></p>
						
						<p class=MsoNormal style='text-align:justify;line-height:150%;mso-layout-grid-align:
						none;text-autospace:none'><span style='font-family:Arial'><o:p>&nbsp;</o:p></span></p>
						
						<p class=MsoNormal style='text-align:justify;line-height:150%;mso-layout-grid-align:
						none;text-autospace:none'><span style='font-family:Arial;color:black'>II – <st1:hm
						w:st="on">priorizar</st1:hm> o atendimento aos <st2:verbetes w:st="on">jovens</st2:verbetes>
						<st2:verbetes w:st="on">negros</st2:verbetes>, <st2:verbetes w:st="on">em</st2:verbetes>
						<st2:verbetes w:st="on">conformidade</st2:verbetes> <st2:verbetes w:st="on">com</st2:verbetes>
						os <st2:verbetes w:st="on">demais</st2:verbetes> <st2:verbetes w:st="on">critérios</st2:verbetes>
						de <st2:verbetes w:st="on">matrícula</st2:verbetes> estabelecidos pelo Programa;<o:p></o:p></span></p>
						
						<p class=MsoNormal style='text-align:justify;line-height:150%;mso-layout-grid-align:
						none;text-autospace:none'><span style='font-family:Arial;color:red'><o:p>&nbsp;</o:p></span></p>
						
						<p class=MsoNormal style='text-align:justify;line-height:150%;mso-layout-grid-align:
						none;text-autospace:none'><span style='font-family:Arial'>III - responsabilizar-se
						<st2:verbetes w:st="on">pela</st2:verbetes> <st1:dm w:st="on">divulgação</st1:dm>
						do ProJovem <st2:verbetes w:st="on">Urbano</st2:verbetes> <st2:verbetes w:st="on">em</st2:verbetes>
						<st1:dm w:st="on">nível</st1:dm> <st2:verbetes w:st="on">local</st2:verbetes>, <st1:dm
						w:st="on">inclusive</st1:dm> <st2:verbetes w:st="on">quanto</st2:verbetes> aos <st2:verbetes
						w:st="on">processos</st2:verbetes> de <st2:verbetes w:st="on">matrícula</st2:verbetes>
						a serem realizados <st1:dm w:st="on">pelo</st1:dm> estado, mobilizando a <st2:verbetes
						w:st="on">comunidade</st2:verbetes> e <st2:verbetes w:st="on">suas</st2:verbetes>
						<st2:verbetes w:st="on">lideranças</st2:verbetes>, os jovens, pais e
						responsáveis, bem como os <st2:verbetes w:st="on">meios</st2:verbetes> <st2:verbetes
						w:st="on">políticos</st2:verbetes> e administrativos;<o:p></o:p></span></p>
						
						<p class=MsoNormal style='text-align:justify;line-height:150%;mso-layout-grid-align:
						none;text-autospace:none'><span style='font-family:Arial'><o:p>&nbsp;</o:p></span></p>
						
						<p class=MsoNormal style='text-align:justify;line-height:150%;mso-layout-grid-align:
						none;text-autospace:none'><span style='font-family:Arial'>IV – empreender
						esforços para viabilizar a expedição dos documentos necessários para a
						matrícula dos jovens a serem atendidos pelo Programa;<o:p></o:p></span></p>
						
						<p class=MsoNormal style='text-align:justify;line-height:150%;mso-layout-grid-align:
						none;text-autospace:none'><span style='font-family:Arial'><o:p>&nbsp;</o:p></span></p>
						
						<p class=MsoNormal style='text-align:justify;line-height:150%;mso-layout-grid-align:
						none;text-autospace:none'><span style='font-family:Arial'>V – matricular os
						alunos por meio de Sistema de Matrícula, Acompanhamento de Freqüência e
						Certificação do Aluno do Projovem Urbano a ser disponibilizado pela Secretaria
						de Educação Continuada, Alfabetização, Diversidade e Inclusão - SECADI/MEC,
						sendo esta a única forma de garantir a inclusão dos jovens no Programa, bem
						como ser responsável pela fidedignidade das informações lançadas no referido
						sistema;<o:p></o:p></span></p>
						
						<p class=MsoNormal style='text-align:justify;line-height:150%;mso-layout-grid-align:
						none;text-autospace:none'><span style='font-family:Arial'><o:p>&nbsp;</o:p></span></p>
						
						<p class=MsoNormal style='text-align:justify;line-height:150%;mso-layout-grid-align:
						none;text-autospace:none'><span style='font-family:Arial'>VI - <st1:hdm w:st="on">garantir</st1:hdm>
						o <st2:verbetes w:st="on">acesso</st2:verbetes> e <st2:verbetes w:st="on">condições</st2:verbetes>
						de <st2:verbetes w:st="on">permanência</st2:verbetes> das <st2:verbetes w:st="on">pessoas</st2:verbetes>
						do <st2:verbetes w:st="on">público</st2:verbetes> <st2:verbetes w:st="on">alvo</st2:verbetes>
						da <st2:verbetes w:st="on">educação</st2:verbetes> <st2:verbetes w:st="on">especial</st2:verbetes>,
						<st2:verbetes w:st="on">por</st2:verbetes> <st2:verbetes w:st="on">meio</st2:verbetes>
						de <st2:verbetes w:st="on">recursos</st2:verbetes> de <st3:sinonimos w:st="on">acessibilidade</st3:sinonimos>,
						no <st2:verbetes w:st="on">curso</st2:verbetes> do ProJovem <st2:verbetes
						w:st="on">Urbano</st2:verbetes>;<o:p></o:p></span></p>
						
						<p class=MsoNormal style='text-align:justify;line-height:150%;mso-layout-grid-align:
						none;text-autospace:none'><span style='font-family:Arial'><o:p>&nbsp;</o:p></span></p>
						
						<p class=MsoNormal style='text-align:justify;line-height:150%;mso-layout-grid-align:
						none;text-autospace:none'><span style='font-family:Arial'>VII – desenvolver o
						Projeto Pedagógico Integrado do Programa em suas três dimensões, garantindo sua
						execução conforme legislação do Projovem Urbano e orientações da Secretaria de
						Educação Continuada, Alfabetização, Diversidade e Inclusão - SECADI/MEC;<o:p></o:p></span></p>
						
						<p class=MsoNormal style='text-align:justify;line-height:150%;mso-layout-grid-align:
						none;text-autospace:none'><span style='font-family:Arial'><o:p>&nbsp;</o:p></span></p>
						
						<p class=MsoNormal style='text-align:justify;line-height:150%;mso-layout-grid-align:
						none;text-autospace:none'><span style='font-family:Arial'>VIII - acompanhar
						cada aluno do ProJovem Urbano, individualmente, mediante registro mensal de
						freqüência, por meio do Sistema de Matrícula,<span style='mso-spacerun:yes'> 
						</span>Acompanhamento da Freqüência e Certificação do Aluno do Projovem Urbano;<o:p></o:p></span></p>
						
						<p class=MsoNormal style='text-align:justify;line-height:150%;mso-layout-grid-align:
						none;text-autospace:none'><span style='font-family:Arial'><o:p>&nbsp;</o:p></span></p>
						
						<p class=MsoNormal style='text-align:justify;line-height:150%;mso-layout-grid-align:
						none;text-autospace:none'><span style='font-family:Arial'>IX – prevenir e combater
						a evasão pelo acompanhamento individual das razões da não-freqüência do
						educando e implantar medidas para superá-la;<o:p></o:p></span></p>
						
						<p class=MsoNormal style='text-align:justify;line-height:150%;mso-layout-grid-align:
						none;text-autospace:none'><span style='font-family:Arial'><o:p>&nbsp;</o:p></span></p>
						
						<p class=MsoNormal style='text-align:justify;line-height:150%;mso-layout-grid-align:
						none;text-autospace:none'><span style='font-family:Arial'>X – promover
						articulação com as redes municipais de educação para que utilizem como critério
						para a disponibilização de vagas na educação infantil, crianças filhas de
						alunos do Programa;<o:p></o:p></span></p>
						
						<p class=MsoNormal style='text-align:justify;line-height:150%;mso-layout-grid-align:
						none;text-autospace:none'><span style='font-family:Arial;color:red'><o:p>&nbsp;</o:p></span></p>
						
						<p class=MsoNormal style='text-align:justify;line-height:150%;mso-layout-grid-align:
						none;text-autospace:none'><span style='font-family:Arial'>XI – <st1:hdm w:st="on">garantir</st1:hdm>
						o <st2:verbetes w:st="on">funcionamento</st2:verbetes> do <st2:verbetes w:st="on">Comitê</st2:verbetes>
						Gestor do <st2:verbetes w:st="on">Programa</st2:verbetes> no <st2:verbetes
						w:st="on">âmbito</st2:verbetes> <st2:verbetes w:st="on">local</st2:verbetes>, <st2:verbetes
						w:st="on">sob</st2:verbetes> <st2:verbetes w:st="on">coordenação</st2:verbetes>
						da <st2:verbetes w:st="on">Secretaria</st2:verbetes> de Educação, <st1:dm
						w:st="on">composto</st1:dm> pelo Conselho de Juventude, por <st2:verbetes
						w:st="on">órgãos</st2:verbetes> de <st2:verbetes w:st="on">políticas</st2:verbetes>
						de juventude, <st2:verbetes w:st="on">quando</st2:verbetes> existir na
						localidade, <st1:dm w:st="on">bem</st1:dm> <st2:verbetes w:st="on">como</st2:verbetes>
						pelas <st2:verbetes w:st="on">demais</st2:verbetes> <st2:verbetes w:st="on">secretarias</st2:verbetes>
						e <st2:verbetes w:st="on">órgãos</st2:verbetes> <st2:verbetes w:st="on">afins</st2:verbetes>,
						observada a intersetorialidade necessária para a execução das ações previstas
						pelo Programa; <o:p></o:p></span></p>
						
						<p class=MsoNormal style='text-align:justify;line-height:150%;mso-layout-grid-align:
						none;text-autospace:none'><span style='font-family:Arial'><o:p>&nbsp;</o:p></span></p>
						
						<p class=MsoNormal style='text-align:justify;line-height:150%;mso-layout-grid-align:
						none;text-autospace:none'><span style='font-family:Arial'>XII – <st1:hdm w:st="on">garantir</st1:hdm>
						a <st2:verbetes w:st="on">oferta</st2:verbetes> de <st2:verbetes w:st="on">Educação</st2:verbetes>
						de <st2:verbetes w:st="on">Jovens</st2:verbetes> e <st2:verbetes w:st="on">Adultos</st2:verbetes>
						– <st2:verbetes w:st="on">ensino</st2:verbetes> <st2:verbetes w:st="on">médio</st2:verbetes>
						nas <st2:verbetes w:st="on">escolas</st2:verbetes> de <st2:verbetes w:st="on">sua</st2:verbetes>
						<st2:verbetes w:st="on">rede</st2:verbetes>, favorecendo a continuidade de <st2:verbetes
						w:st="on">educação</st2:verbetes> dos <st2:verbetes w:st="on">jovens</st2:verbetes>
						atendidos <st1:dm w:st="on">pelo</st1:dm> Programa;<o:p></o:p></span></p>
						
						<p class=MsoNormal style='text-align:justify;line-height:150%;mso-layout-grid-align:
						none;text-autospace:none'><span style='font-family:Arial'><o:p>&nbsp;</o:p></span></p>
						
						<p class=MsoNormal style='text-align:justify;line-height:150%;mso-layout-grid-align:
						none;text-autospace:none'><span style='font-family:Arial'>XIII - Concordar
						integralmente com os termos da Resolução nº <span style='background:yellow;
						mso-highlight:yellow'>xxx</span> publicada pelo Fundo Nacional de
						Desenvolvimento da Educação – FNDE/MEC em <span style='background:yellow;
						mso-highlight:yellow'>xx de xx</span> de 2011, que estabelece os<span
						style='color:black'> critérios e as normas de transferência automática de
						recursos financeiros do ProJovem Urbano para a execução das ações do Programa; </span><o:p></o:p></span></p>
						
						<p class=MsoNormal style='text-align:justify;line-height:150%;mso-layout-grid-align:
						none;text-autospace:none'><b><span style='font-family:Arial'><o:p>&nbsp;</o:p></span></b></p>
						
						<p class=MsoNormal style='margin-left:.6pt;text-align:justify;text-indent:-.6pt;
						line-height:150%;mso-layout-grid-align:none;text-autospace:none'><span
						style='font-family:Arial'>XII - Autorizar o FNDE/MEC a estornar ou bloquear
						valores creditados indevidamente na conta corrente do Estado, mediante
						solicitação direta ao agente financeiro depositário dos recursos ou procedendo
						ao desconto nas parcelas subseqüentes.<o:p></o:p></span></p>
						
						<p class=MsoNormal style='margin-left:.6pt;text-align:justify;text-indent:-.6pt;
						line-height:150%;mso-layout-grid-align:none;text-autospace:none'><span
						style='font-family:Arial'><o:p>&nbsp;</o:p></span></p>
						
						<p class=MsoNormal style='text-align:justify;line-height:150%;mso-layout-grid-align:
						none;text-autospace:none'><span style='font-family:Arial'>XIII - Restituir ao
						FNDE/MEC, no prazo de 05 (cinco) dias, a contar do recebimento da notificação,
						os valores de que trata o item 5 deste Termo, acrescidos de juros e correção
						monetária, no caso de inexistir saldo suficiente na conta corrente e não
						havendo repasses futuros a serem efetuados<a name="OLE_LINK20">.</a><o:p></o:p></span></p>
						
						<p class=MsoNormal style='text-align:justify;line-height:150%;mso-layout-grid-align:
						none;text-autospace:none'><span style='font-family:Arial'><o:p>&nbsp;</o:p></span></p>
						
						<p class=MsoNormal style='text-align:justify;line-height:150%;mso-layout-grid-align:
						none;text-autospace:none'><span style='font-family:Arial'>XIV - Atualizar
						permanentemente junto à Secretaria de Educação Continuada, Alfabetização,
						Diversidade e Inclusão/MEC, as informações prestadas no Plano de Implementação
						do Programa, sob pena <span style='color:black'>de suspensão de pagamento de
						parcelas subseqüentes até a regularização</span> das informações.<o:p></o:p></span></p>
						
						<p class=MsoNormal style='text-align:justify;line-height:150%;mso-layout-grid-align:
						none;text-autospace:none'><span style='font-family:Arial'><o:p>&nbsp;</o:p></span></p>
						
						<p class=MsoNormal align=center style='text-align:center;line-height:150%;
						mso-layout-grid-align:none;text-autospace:none'><span style='font-family:Arial'>________________,
						_____ de setembro de 2011.<o:p></o:p></span></p>
						
						<p class=MsoNormal style='text-align:justify;line-height:150%;mso-layout-grid-align:
						none;text-autospace:none'><b><span style='font-family:Arial'><o:p>&nbsp;</o:p></span></b></p>
						
						<h3><span style='font-size:12.0pt;line-height:150%'>Secretário(a) Estadual de
						Educação<o:p></o:p></span></h3>
						
						<p class=MsoNormal style='line-height:150%'><o:p>&nbsp;</o:p></p>
						
						</div>					
					
					
					
					
					
					
					
					
					
					
					
					
				</div>
			</div>
		</td>
	</tr>
	<tr>
		<td align="center">
			<div style="float: center; width: 70%; ">
                    
					<input type="button" name="aceito" value="Aceito" onclick="aceitox('S');"/>
                    &nbsp;
                    <input type="button" name="naceito" value="Não Aceito" onclick="aceitox('N');"/>
                    &nbsp;
                    <input type="button" name="sair" value="Sair" onclick="location.href = 'projovem.php?modulo=inicio&acao=C'" />
					
    				
			</div>
		</td>
	</tr>
</table>
</center>
</form>

<script>

function aceitox(id){
	
	alert("Operação realizada com sucesso!");
	if(id == 'S'){
		location.href='projovem.php?modulo=principal/instrucao&acao=A';
	}
	else{
		location.href='projovem.php?modulo=inicio&acao=C';		
	}
}


</script>
