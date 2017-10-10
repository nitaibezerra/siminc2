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

monta_titulo('PROJOVEM', $mun['municipio'].' - '.$mun['uf']);
echo '<br>';

$menu = array(0 => array("id" => 1, "descricao" => "Instruções", 	"link" => "/projovem/projovem.php?modulo=principal/instrucao&acao=A"),
				  1 => array("id" => 2, "descricao" => "Plano de implementação", 		 	"link" => "#")
			  	  );

echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);

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
 /* Style Definitions */
 p.MsoNormal, li.MsoNormal, div.MsoNormal
	{mso-style-parent:"";
	margin:0cm;
	margin-bottom:.0001pt;
	mso-pagination:widow-orphan;
	font-size:12.0pt;
	font-family:"Times New Roman";
	mso-fareast-font-family:"Times New Roman";}
a:link, span.MsoHyperlink
	{color:blue;
	text-decoration:underline;
	text-underline:single;}
a:visited, span.MsoHyperlinkFollowed
	{color:purple;
	text-decoration:underline;
	text-underline:single;}
@page Section1
	{size:612.0pt 792.0pt;
	margin:70.85pt 63.0pt 53.95pt 3.0cm;
	mso-header-margin:35.4pt;
	mso-footer-margin:35.4pt;
	mso-paper-source:0;}
div.Section1
	{page:Section1;}
 /* List Definitions */
 @list l0
	{mso-list-id:126435458;
	mso-list-type:hybrid;
	mso-list-template-ids:979511980 68550671 68550681 68550683 68550671 68550681 68550683 68550671 68550681 68550683;}
@list l0:level1
	{mso-level-tab-stop:36.0pt;
	mso-level-number-position:left;
	text-indent:-18.0pt;}
@list l0:level2
	{mso-level-number-format:alpha-lower;
	mso-level-tab-stop:72.0pt;
	mso-level-number-position:left;
	text-indent:-18.0pt;}
@list l1
	{mso-list-id:641420765;
	mso-list-type:hybrid;
	mso-list-template-ids:-1132003702 68550671 68550681 68550683 68550671 68550681 68550683 68550671 68550681 68550683;}
@list l1:level1
	{mso-level-tab-stop:36.0pt;
	mso-level-number-position:left;
	text-indent:-18.0pt;}
@list l2
	{mso-list-id:1214274099;
	mso-list-type:hybrid;
	mso-list-template-ids:1287162088 68550671 68550681 68550683 68550671 68550681 68550683 68550671 68550681 68550683;}
@list l2:level1
	{mso-level-tab-stop:36.0pt;
	mso-level-number-position:left;
	text-indent:-18.0pt;}
ol
	{margin-bottom:0cm;}
ul
	{margin-bottom:0cm;}
</style>

<div class=Section1>

<p class=MsoNormal align=center style='text-align:center'><b style='mso-bidi-font-weight:
normal'><span style='font-size:11.0pt'>INSTRUÇÕES PARA PREENCHIMENTO DOS
CADASTROS<o:p></o:p></span></b></p>

<p class=MsoNormal style='text-align:justify'><span style='font-size:11.0pt'><o:p>&nbsp;</o:p></span></p>

<p class=MsoNormal style='text-align:justify'><span style='font-size:11.0pt'>A
partir deste ano, as Secretarias de Educação precisam cadastrar neste módulo do
PAR os(as) diretores(as) e coordenadores(as) pedagógicos(as) que elas desejam
que participem dos cursos de especialização oferecidos pela Escola de Gestores
nos próximos dois anos, 2012 e 2013.<o:p></o:p></span></p>

<p class=MsoNormal style='text-align:justify'><span style='font-size:11.0pt'><o:p>&nbsp;</o:p></span></p>

<p class=MsoNormal style='text-align:justify'><span style='font-size:11.0pt'>Para
tanto, leia atentamente as instruções de preenchimento e, antes de avançar,
certifique-se de que todos os profissionais que serão cadastrados estão
efetivamente interessados e dispostos a participar dos cursos a distância
oferecidos pelo MEC em parceria com as universidades. Caso deseje conhecer
melhor o programa antes de continuar o preenchimento, acesse o site do MEC e
entenda o programa. Acesse <a href="http://www.mec.gov.br/">www.mec.gov.br</a>
&gt; Secretaria de Educação Básica &gt; Programas e ações &gt; Escola de
Gestores.<o:p></o:p></span></p>

<p class=MsoNormal style='text-align:justify'><span style='font-size:11.0pt'><o:p>&nbsp;</o:p></span></p>

<p class=MsoNormal style='text-align:justify'><span style='font-size:11.0pt'>As
regras de preenchimento são as seguintes:<o:p></o:p></span></p>

<p class=MsoNormal style='text-align:justify'><span style='font-size:11.0pt'><o:p>&nbsp;</o:p></span></p>

<ol style='margin-top:0cm' start=1 type=1>
 <li class=MsoNormal style='text-align:justify;mso-list:l1 level1 lfo2;
     tab-stops:list 36.0pt'><span style='font-size:11.0pt'>Os municípios
     disporão, para cada curso, de uma quantidade de vagas proporcional à
     população, conforme quadro abaixo.<o:p></o:p></span></li>
</ol>

<p class=MsoNormal style='margin-left:18.0pt;text-align:justify'><span
style='font-size:11.0pt'><o:p>&nbsp;</o:p></span></p>

<ol style='margin-top:0cm' start=2 type=1>

 <li class=MsoNormal style='text-align:justify;mso-list:l1 level1 lfo2;
     tab-stops:list 36.0pt'><span style='font-size:11.0pt'>No momento de
     cadastramento dos candidatos no <?php echo SIGLA_SISTEMA; ?>, a Secretaria de Educação poderá
     inserir até duas vezes o número total de vagas disponibilizadas, uma vez
     que nem todos os inscritos cumprirão os requisitos mínimos (ser
     funcionário efetivo, possuir curso superior e encontrar-se em exercício na
     função) e ainda precisarão submeter-se ao processo seletivo das
     universidades.<o:p></o:p></span></li>
</ol>

<p class=MsoNormal style='text-align:justify'><span style='font-size:11.0pt'><o:p>&nbsp;</o:p></span></p>

<table class=MsoTableGrid border=1 cellspacing=0 cellpadding=0 width=468
 style='width:351.0pt;margin-left:41.4pt;border-collapse:collapse;border:none;
 mso-border-alt:solid windowtext .5pt;mso-yfti-tbllook:191;mso-padding-alt:
 0cm 5.4pt 0cm 5.4pt;mso-border-insideh:.5pt solid windowtext;mso-border-insidev:
 .5pt solid windowtext'>
 <tr style='mso-yfti-irow:0;mso-yfti-firstrow:yes'>
  <td width=468 colspan=3 valign=top style='width:351.0pt;border:solid windowtext 1.0pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt'>
  <p class=MsoNormal style='text-align:justify'><span style='font-size:11.0pt'>REDES
  MUNICIPAIS - Atendimento 2012-2013<o:p></o:p></span></p>
  </td>

 </tr>
 <tr style='mso-yfti-irow:1'>
  <td width=192 style='width:144.0pt;border:solid windowtext 1.0pt;border-top:
  none;mso-border-top-alt:solid windowtext .5pt;mso-border-alt:solid windowtext .5pt;
  padding:0cm 5.4pt 0cm 5.4pt'>
  <p class=MsoNormal style='text-align:justify'><span style='font-size:11.0pt'>POPULAÇÃO<o:p></o:p></span></p>
  </td>
  <td width=144 style='width:108.0pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='font-size:11.0pt'>Vagas por curso<o:p></o:p></span></p>
  </td>

  <td width=132 style='width:99.0pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='font-size:11.0pt'>Número máximo de inscrições<o:p></o:p></span></p>
  </td>
 </tr>
 <tr style='mso-yfti-irow:2;height:12.6pt'>
  <td width=192 style='width:144.0pt;border:solid windowtext 1.0pt;border-top:
  none;mso-border-top-alt:solid windowtext .5pt;mso-border-alt:solid windowtext .5pt;
  padding:0cm 5.4pt 0cm 5.4pt;height:12.6pt'>
  <p class=MsoNormal style='text-align:justify'><span style='font-size:11.0pt'>Até
  10 mil hab.<o:p></o:p></span></p>
  </td>

  <td width=144 style='width:108.0pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt;height:12.6pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='font-size:11.0pt'>1<o:p></o:p></span></p>
  </td>
  <td width=132 style='width:99.0pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt;height:12.6pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='font-size:11.0pt'>2<o:p></o:p></span></p>
  </td>
 </tr>
 <tr style='mso-yfti-irow:3;height:8.8pt'>

  <td width=192 style='width:144.0pt;border:solid windowtext 1.0pt;border-top:
  none;mso-border-top-alt:solid windowtext .5pt;mso-border-alt:solid windowtext .5pt;
  padding:0cm 5.4pt 0cm 5.4pt;height:8.8pt'>
  <p class=MsoNormal style='text-align:justify'><span style='font-size:11.0pt'>De
  <st1:metricconverter ProductID="10.001 a" w:st="on">10.001 a</st1:metricconverter>
  50 mil hab.<o:p></o:p></span></p>
  </td>
  <td width=144 style='width:108.0pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt;height:8.8pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='font-size:11.0pt'>3<o:p></o:p></span></p>
  </td>

  <td width=132 style='width:99.0pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt;height:8.8pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='font-size:11.0pt'>6<o:p></o:p></span></p>
  </td>
 </tr>
 <tr style='mso-yfti-irow:4;height:6.4pt'>
  <td width=192 style='width:144.0pt;border:solid windowtext 1.0pt;border-top:
  none;mso-border-top-alt:solid windowtext .5pt;mso-border-alt:solid windowtext .5pt;
  padding:0cm 5.4pt 0cm 5.4pt;height:6.4pt'>
  <p class=MsoNormal style='text-align:justify'><span style='font-size:11.0pt'>De
  <st1:metricconverter ProductID="50.001 a" w:st="on">50.001 a</st1:metricconverter>

  100 mil hab.<o:p></o:p></span></p>
  </td>
  <td width=144 style='width:108.0pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt;height:6.4pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='font-size:11.0pt'>8<o:p></o:p></span></p>
  </td>
  <td width=132 style='width:99.0pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt;height:6.4pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='font-size:11.0pt'>16<o:p></o:p></span></p>

  </td>
 </tr>
 <tr style='mso-yfti-irow:5;height:11.6pt'>
  <td width=192 style='width:144.0pt;border:solid windowtext 1.0pt;border-top:
  none;mso-border-top-alt:solid windowtext .5pt;mso-border-alt:solid windowtext .5pt;
  padding:0cm 5.4pt 0cm 5.4pt;height:11.6pt'>
  <p class=MsoNormal style='text-align:justify'><span style='font-size:11.0pt'>De
  <st1:metricconverter ProductID="100.001 a" w:st="on">100.001 a</st1:metricconverter>
  500 mil hab.<o:p></o:p></span></p>
  </td>

  <td width=144 style='width:108.0pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt;height:11.6pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='font-size:11.0pt'>15<o:p></o:p></span></p>
  </td>
  <td width=132 style='width:99.0pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt;height:11.6pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='font-size:11.0pt'>30<o:p></o:p></span></p>
  </td>
 </tr>
 <tr style='mso-yfti-irow:6;height:7.8pt'>

  <td width=192 style='width:144.0pt;border:solid windowtext 1.0pt;border-top:
  none;mso-border-top-alt:solid windowtext .5pt;mso-border-alt:solid windowtext .5pt;
  padding:0cm 5.4pt 0cm 5.4pt;height:7.8pt'>
  <p class=MsoNormal style='text-align:justify'><span style='font-size:11.0pt'>Acima
  de 500.001 hab.*<o:p></o:p></span></p>
  </td>
  <td width=144 style='width:108.0pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt;height:7.8pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='font-size:11.0pt'>30<o:p></o:p></span></p>
  </td>
  <td width=132 style='width:99.0pt;border-top:none;border-left:none;
  border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;
  mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt;height:7.8pt'>
  <p class=MsoNormal align=center style='text-align:center'><span
  style='font-size:11.0pt'>60<o:p></o:p></span></p>

  </td>
 </tr>
 <tr style='mso-yfti-irow:7;mso-yfti-lastrow:yes'>
  <td width=468 colspan=3 style='width:351.0pt;border:solid windowtext 1.0pt;
  border-top:none;mso-border-top-alt:solid windowtext .5pt;mso-border-alt:solid windowtext .5pt;
  padding:0cm 5.4pt 0cm 5.4pt'>
  <p class=MsoNormal style='text-align:justify'><span style='font-size:11.0pt'>*
  Exceto Brasília, que entra na cota do DF<o:p></o:p></span></p>
  </td>
 </tr>
</table>

<p class=MsoNormal style='text-align:justify'><span style='font-size:11.0pt'><o:p>&nbsp;</o:p></span></p>

<ol style='margin-top:0cm' start=3 type=1>
 <li class=MsoNormal style='text-align:justify;mso-list:l1 level1 lfo2;
     tab-stops:list 36.0pt'><span style='font-size:11.0pt'>No caso das redes
     estaduais, serão abertas vagas em número correspondente a 15% do total de
     escolas da rede, em cada curso. Logo, se o estado tiver 5 mil escolas,
     serão disponibilizadas até 750 vagas para os dois anos, 2012 e 2013, e
     poderão ser inscritos até 1.500 candidatos.<o:p></o:p></span></li>
</ol>

<p class=MsoNormal style='margin-left:18.0pt;text-align:justify'><span
style='font-size:11.0pt'><o:p>&nbsp;</o:p></span></p>

<ol style='margin-top:0cm' start=4 type=1>
 <li class=MsoNormal style='text-align:justify;mso-list:l1 level1 lfo2;
     tab-stops:list 36.0pt'><span style='font-size:11.0pt'>Reiteramos que é
     fundamental que os(as) candidatos(as) estejam em exercício efetivo na rede
     pública, estadual ou municipal, não sendo permitida a participação de
     profissionais fora das escolas. Neste caso, as inscrições serão
     automaticamente canceladas na fase de análise pelo MEC.<o:p></o:p></span></li>
</ol>

<p class=MsoNormal style='text-align:justify'><span style='font-size:11.0pt'><o:p>&nbsp;</o:p></span></p>

<ol style='margin-top:0cm' start=5 type=1>
 <li class=MsoNormal style='text-align:justify;mso-list:l1 level1 lfo2;
     tab-stops:list 36.0pt'><span style='font-size:11.0pt'>Só é permitida a
     inscrição de um(a) candidato(a) por escola, que pode ser o(a) diretor(a)
     ou vice-diretor(a), no caso do Curso de Gestão Escolar, ou um membro da
     Equipe Pedagógica que exerça a função de Coordenador(a) Pedagógico(a) ou
     similar.<o:p></o:p></span></li>
</ol>

<p class=MsoNormal style='text-align:justify'><span style='font-size:11.0pt'><o:p>&nbsp;</o:p></span></p>

<ol style='margin-top:0cm' start=6 type=1>
 <li class=MsoNormal style='text-align:justify;mso-list:l1 level1 lfo2;
     tab-stops:list 36.0pt'><span style='font-size:11.0pt'>Na hora de preencher
     os cadastros, insira informações válidas, especialmente o e-mail do
     candidato, pois toda a comunicação sobre o processo seletivo será feita
     por e-mail. Após preencher todos os campos, não esqueça de clicar em
     "Salvar". O sistema exibirá os dados dos candidatos inscritos numa tabela.<o:p></o:p></span></li>
</ol>

<p class=MsoNormal style='text-align:justify'><span style='font-size:11.0pt'><o:p>&nbsp;</o:p></span></p>

<ol style='margin-top:0cm' start=7 type=1>
 <li class=MsoNormal style='text-align:justify;mso-list:l1 level1 lfo2;
     tab-stops:list 36.0pt'><span style='font-size:11.0pt'>Caso deseje alterar
     alguma informação, clique no ícone verde ao lado do nome. Caso deseje
     excluir um candidato inscrito, clique no ícone vermelho ao lado do nome do
     candidato.<o:p></o:p></span></li>
</ol>

<p class=MsoNormal style='text-align:justify'><span style='font-size:11.0pt'><o:p>&nbsp;</o:p></span></p>

<ol style='margin-top:0cm' start=8 type=1>
 <li class=MsoNormal style='text-align:justify;mso-list:l1 level1 lfo2;
     tab-stops:list 36.0pt'><span style='font-size:11.0pt'>Após concluir todas
     as inscrições, mesmo que elas não atinjam o limite máximo, a Secretaria
     deve clicar no ícone localizado do lado direito da tela onde está escrito
     "Enviar para pré-análise". Só acione este comando depois de certificar-se
     de que todos os candidatos foram inscritos nos dois cursos (veja as abas
     na parte superior da tela, onde encontra-se escrito "Gestão Escolar" e
     "Coordenação Pedagógica").<o:p></o:p></span></li>
</ol>

<p class=MsoNormal style='text-align:justify'><span style='font-size:11.0pt'><o:p>&nbsp;</o:p></span></p>

<ol style='margin-top:0cm' start=9 type=1>

 <li class=MsoNormal style='text-align:justify;mso-list:l1 level1 lfo2;
     tab-stops:list 36.0pt'><span style='font-size:11.0pt'>Depois de enviar
     para pré-análise do MEC, as secretarias e os candidatos devem aguardar o
     comunicado do MEC informando quais as inscrições pré-selecionadas e para
     quais universidades os nomes foram enviados. <o:p></o:p></span></li>
</ol>

<p class=MsoNormal style='text-align:justify'><span style='font-size:11.0pt'><o:p>&nbsp;</o:p></span></p>

<ol style='margin-top:0cm' start=10 type=1>
 <li class=MsoNormal style='text-align:justify;mso-list:l1 level1 lfo2;
     tab-stops:list 36.0pt'><span style='font-size:11.0pt'>Após a pré-seleção, o(a)
     candidato(a) deve aguardar um comunicado da respectiva universidade,
     informando se e quando poderá ofertar os cursos e o cronograma estimado.<o:p></o:p></span></li>
</ol>

<p class=MsoNormal style='text-align:justify'><span style='font-size:11.0pt'><o:p>&nbsp;</o:p></span></p>

<p class=MsoNormal style='text-align:justify'><span style='font-size:11.0pt'>Em
caso de dúvida, encaminhe um e-mail para <u><a
href="mailto:escoladegestores@mec.gov.br"><span style='font-size:12.0pt;
color:windowtext'>escoladegestores@mec.gov.br</span></a></u>.<o:p></o:p></span></p>

<p class=MsoNormal><o:p>&nbsp;</o:p></p>

</div>				
					
					
					
					
					
					
					
					
					
					
					
					
				</div>
			</div>
		</td>
	</tr>
	<tr>
		<td align="center">
			
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
