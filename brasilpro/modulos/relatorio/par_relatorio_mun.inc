<?php

cte_verificaSessao();

$ptostatus = isset( $_REQUEST['ptostatus'] ) ? $_REQUEST['ptostatus'] : 'A';

// ----- CARREGA DADOS DO BANCO SOBRE INSTRUMENTOUNIDADE - INU -----------------
$sqlinu = "select * from cte.instrumentounidade where inuid = " .
$_SESSION["inuid"];
$dadosinu = $db->recuperar( $sqlinu );
if ( is_array( $dadosinu ) )
{
	$inueqploc = $dadosinu['inueqploc'];
	$inuddsdmgr = $dadosinu['inuddsdmgr'];
	$inucadsec = $dadosinu['inucadsec'];
	$inucadcmt = $dadosinu['inucadcmt'];
}

$inueqploc = $inueqploc == "null" ? "" : $inueqploc;
$inuddsdmgr = $inuddsdmgr == "null" ? "" : $inuddsdmgr;
$inucadsec = $inucadsec == "null" ? "" : $inucadsec;
$inucadcmt = $inucadcmt == "null" ? "" : $inucadcmt;
// ----- FIM CARREGA DADOS DO BANCO SOBRE INSTRUMENTOUNIDADE - INU -------------

// ----- CARREGA SINTESE POR INDICADOR -----------------------------------------
$sqlsi = sprintf( "
		select distinct
			d.dimcod
			,d.dimdsc
			,ad.ardcod
			,ad.arddsc
			,i.indcod
			,c.ctrpontuacao
			,p.ptojustificativa
			,p.ptodemandamunicipal
			,p.ptodemandaestadual
		from 
			cte.instrumento ins
			inner join cte.instrumentounidade iu on iu.itrid = ins.itrid
			inner join cte.dimensao d on d.itrid = ins.itrid
			inner join cte.areadimensao ad on d.dimid = ad.dimid
			inner join cte.indicador i on i.ardid = ad.ardid
			inner join cte.criterio c on c.indid = i.indid
			inner join cte.pontuacao p on p.crtid = c.crtid and p.inuid = %d
		where 
			d.dimstatus = 'A'
			and ad.ardstatus = 'A'  
			and i.indstatus = 'A'
			and p.ptostatus = '%s'
		" , 
$_SESSION['inuid'],
$ptostatus
);
$dadossi = $db->carregar($sqlsi);
// ----- FIM CARREGA SINTESE POR INDICADOR -------------------------------------

// ----- CARREGA SINTESE POR DIMENSÃO ------------------------------------------
$sqlSD = sprintf( "
		select distinct
			ins.itrid
			,d.dimid
			,d.dimcod || '. ' ||d.dimdsc as dimdsc
			,c.ctrpontuacao 
			,count ( c.ctrpontuacao ) as qtpontos
			
		from cte.instrumento ins
			inner join cte.instrumentounidade iu on iu.itrid = ins.itrid
			inner join cte.dimensao d on d.itrid = ins.itrid
			inner join cte.areadimensao ad on d.dimid = ad.dimid
			inner join cte.indicador i on i.ardid = ad.ardid
			inner join cte.criterio c on c.indid = i.indid
			left join cte.pontuacao pt on pt.crtid = c.crtid
		where
			d.dimstatus = 'A'
			and ad.ardstatus = 'A'  
			and i.indstatus = 'A' 
			and pt.inuid = %d
			and pt.ptostatus = '%s'
		group by
			ins.itrid,
			d.dimcod,
			d.dimdsc,
			c.ctrpontuacao,
			d.dimid
	"
	,$_SESSION['inuid']
	,$ptostatus
	);

	if( $dadosSD = $db->carregar($sqlSD) )
	{
		//percorrendo o resultado e criando um array por dimensao
		foreach($dadosSD as $keySD => $valSD )
		{
			$corSD = $icone ? '#959595' : '#133368';
			$relatorioSD[$valSD["dimid"]]["dimdsc"] = $valSD["dimdsc"];
			switch($valSD["ctrpontuacao"])
			{
				case "0":
					$relatorioSD[$valSD["dimid"]]["0"] = $valSD["qtpontos"];
					break;
				case "1":
					$relatorioSD[$valSD["dimid"]]["1"] = $valSD["qtpontos"];
					break;
				case "2":
					$relatorioSD[$valSD["dimid"]]["2"] = $valSD["qtpontos"];
					break;
				case "3":
					$relatorioSD[$valSD["dimid"]]["3"] = $valSD["qtpontos"];
					break;
				case "4":
					$relatorioSD[$valSD["dimid"]]["4"] = $valSD["qtpontos"];
					break;
			}
		}
	}

	$totalSD["t0"] = 0;
	$totalSD["t1"] = 0;
	$totalSD["t2"] = 0;
	$totalSD["t3"] = 0;
	$totalSD["t4"] = 0;
	$corSD = '#e7e7e7';

	// ----- FIM CARREGA SINTESE POR DIMENSÃO --------------------------------------

	?>

<LINK
	rel="stylesheet" type="text/css" media="print, handheld"
	href="../includes/print.css">
<style type="text/css">
<!--
body {
	font-family: sans-serif;
}

#geral {
	margin: 10px 10px 10px 10px;
}

#geral #capa {
	font: bold;
	text-align: center;
	vertical-align: middle;
}

#geral #capa .topo {
	height: 9cm;
	font-size: 18px;
}

#geral #capa .meio {
	height: 14cm;
	font-size: 20px;
}

#geral #capa .rodape {
	height: 1cm;
	font-size: 14px;
}

#geral #conteudo {
	
}

#geral #conteudo h1 {
	font-size: 18px;
	margin-left: 15px;
	list-style: decimal;
	margin-bottom: 30px;
}

#geral #conteudo h2 {
	font-size: 16px;
	margin-left: 30px;
}

#geral #conteudo .texto {
	font-size: 12px;
	text-align: justify;
}

#geral #conteudo .texto ul {
	line-height: 30px;
	list-style: disc;
	margin-left: 40px;
	font-size: 12px;
}

#geral #conteudo p {
	font-size: 12px;
	text-indent: 60px;
	white-space: normal;
	line-height: 30px;
}

#geral #conteudo .textoTabela {
	font-size: 12px;
}

#geral #conteudo .tabela {
	font-size: 12px;
	width: 100%;
}

#geral #conteudo p {
	margin: 10px 10px 10px 10px;
}

#relatorioPar tr td {
	font-size: 10px;
}
-->
<?
$muncod=cte_pegarMuncod($_SESSION['inuid']);
$mundescricao=cte_pegarMundescricao($muncod);
?>
</style>
<div id="geral">
<div id="capa">

<div class="topo">
<p>PREFEITURA MUNICIPAL DE<br />
<br />
DE<br />
<br />
<a href="?modulo=principal/estrutura_avaliacao&acao=A"><?php echo $mundescricao; ?></a></p>
</div>
<div class="meio">
<p>PLANO DE METAS COMPROMISSO TODOS PELA EDUCAÇÃO <br />
<br />
PLANO DE AÇÕES ARTICULADAS - PAR</p>
</div>
<div class="rodape">
<p>2007</p>
</div>
</div>
<div id="conteudo" class="texto" style="page-break-before: always;">
<ul>
	<center>
	<h1>Equipe envolvida na elabora&ccedil;&atilde;o</h1>
	</center>
	<div class="texto" style="page-break-after: always;">
	<p><?php echo $inueqploc ?></p>
	</div>
	<center>
	<h1 style="margin-top: 150px;">Apresenta&ccedil;ão</h1>
	</center>
	<div class="texto" style="page-break-after: always;">
	<p>No momento da assinatura do Termo de Ades&atilde;o ao &nbsp;&nbsp; <i>Plano
	de Metas Compromisso Todos pela Educa&ccedil;&atilde;o</i>, assumimos o
	compromisso de melhorar nossos indicadores educacionais a partir do
	desenvolvimento de a&ccedil;&otilde;es que possibilitem o cumprimento
	das diretrizes estabelecidas no referido Termo de Ades&atilde;o e
	tamb&eacute;m o alcance das metas estabelecidas para o IDEB.</p>
	<p>Visando promover a melhoria da qualidade da Educa&ccedil;&atilde;o
	B&aacute;sica oferecida neste Estado nos propomos a cumprir
	integralmente as a&ccedil;&otilde;es propostas no presente Plano de
	A&ccedil;&otilde;es Articuladas - PAR e, com a mesma responsabilidade
	estabeleceremos, em parceria com o MEC, sistemas de acompanhamento e
	avalia&ccedil;&atilde;o das a&ccedil;&otilde;es a serem desenvolvidas,
	al&eacute;m disso, &eacute; nosso compromisso divulgar a
	evolu&ccedil;&atilde;o dos dados educacionais &agrave;
	popula&ccedil;&atilde;o local e estimula-la a participar e promover o
	controle social de todas as a&ccedil;&otilde;es propostas neste
	documento.</p>
	<p>Nossa sugest&atilde;o de PAR concentra-se na melhoria gradativa dos
	resultados educacionais e tem o aluno como o centro de todas as
	decis&otilde;es. Assim, o seu principal objetivo &eacute; contribuir
	para o desenvolvimento de aprendizagens, habilidades e
	compet&ecirc;ncias, atitudes e valores necess&aacute;rios para a sua
	forma&ccedil;&atilde;o integral.</p>
	<p>A seguir apresentamos o PAR elaborado a partir do diagn&oacute;stico
	realizado pela equipe da Secretaria Municipal de Educa&ccedil;&atilde;o
	em conjunto com especialistas do MEC nos dias ____________ do mês de
	____________________ de ______________ e convalidado por mim,
	Prefeito(a) Municipal.</p>
	<p align="right"><br />
	<br />
	<br />
	<br />
	_______________________________________ <br/>
	Prefeito(a) Municipal</p>
	</div>
	<h1>
	<li>Dados Básicos de identificação</li>
	</h1>
	<h2>1 Do Município</h2>
	<div class="texto">
	<p><?php echo $inuddsdmgr ?></p>
	<p>
		
	<table width="100%" class="tabela" border="1" bordercolor="black" cellpadding="3"
		cellspacing="0" style="border-color: black; border-style: solid;">
		<tr>
			<td colspan="7"><strong>Nome do Município:</strong></td>
			<td colspan=1"><strong>UF:</strong></td>
		</tr>
		<tr>
			<td colspan="8"><strong>Tabela 1. Informações sobre o município:</strong></td>
		</tr>
		<tr>
			<td colspan="3">População</td>
			<td>PIB</td>
			<td>IDH</td>
			<td>IDI</td>
			<td colspan="2">Taxa analfabetismo</td>
		</tr>
		<tr>
			<td>Urbana</td>
			<td>Rural</td>
			<td>Total</td>
			<td>&nbsp;</td>
			<td>&nbsp;</td>
			<td>&nbsp;</td>
			<td>População de 10 a 15 anos</td>
			<td>População de 15 anos ou mais</td>
		</tr>
		<tr>
			<td>&nbsp;</td>
			<td>&nbsp;</td>
			<td>&nbsp;</td>
			<td>&nbsp;</td>
			<td>&nbsp;</td>
			<td>&nbsp;</td>
			<td>&nbsp;</td>
			<td>&nbsp;</td>
		</tr>
		<tr>
			<td colspan="8">Fonte(1) IBGE - 2000;(2)Índice de desenvolvimento
			Humano - UNESCO - 2000;(3)Índice de Desenvolvimento da Infância -
			Unicef -2004</td>
		</tr>
	</table>
	</p>
	</div>
	<h2>2 Da Prefeitura</h2>
		
	
	<div id="conteudo" class="texto" style="page-break-after: always;">
	<p><?php echo $inucadcmt ?></p>
	<p>
	<h2>1.2.1 Nome Completo do(a) Prefeito (a) Municipal:</h2>
	
	<p><font size="3" face="Arial">__________________________________________</font> <br>
	</p>
	<p>Endereço da prefeitura:</p>
	<p>Rua.........................................................
	.............................................................., nº...............</p>
	<p>Bairro:.................................................CEP:...........................................Estado:...................</p>
	<p>Fone:                                  
	Fax:                          <WBR>
	             e-mail:</font> <br>
	</p>
	<p><b>1.2.2 Nome Completo do (a)  Dirigente
	Municipal de Educação</b>:</p>
	<p>__________________________________________ <br>
	</p>
	<p>Endereço da Secretaria Municipal de
	Educação:</p>
	<p>Rua......................................................................................................................., nº...............</font></p>
	<p>Bairro:.................................................CEP:...........................................Estado:...................</font></p>
	<p>Fone:                         <WBR>
	          Fax:                          <WBR>
	             e-mail:</font> <br>
	</p>
	</p>

	</div>


	<div id="conteudo" class="texto" style="page-break-after: always;"><!-- pagina aqui -->
	<h2><b> 3 Do Conselho Municipal de Educação </b></h2>

	 <br>
	<p><b>1.3.1 Nome Completo do (a) Presidente
	do Conselho Municipal de Educação</b>:</p>
	<p>____________________________________________</p>
	<p>Endereço do Presidente do Conselho
	Municipal de Educação:</p>
	<p>Rua......................................................................................................................., nº...............</font></p>
	<p>Bairro:.................................................CEP:...........................................Estado:...................</font></p>
	<p>Fone:                         <WBR>
	          Fax:                          <WBR>
	             e-mail: <br>
	 <br>
	   </p>
	<p><b>1.3.2 Demais membros do Conselho
	Municipal de Educação</b>:</p>
	<p>Nome:</p>
	<p>Representação:</p>
	<p>Endereço:</p>
	<p>Rua......................................................................................................................., nº...............</font></p>
	<p>Bairro:.................................................CEP:...........................................Estado:....</font></p>
	<p>Fone:                         <WBR>
	          Fax:                          <WBR>
	             e-mail: <br>
	</p>
	<p><b>1.3.2 Demais membros do Conselho
	Municipal de Educação</b>:</p>
	<p>Nome:</p>
	<p>Representação:</p>
	<p>Endereço:</p>
	<p>Rua......................................................................................................................., nº...............</font></p>
	<p>Bairro:.................................................CEP:...........................................Estado:....</font></p>
	<p>Fone:                                  
	Fax:                          <WBR>
	             e-mail: <br>
	</p>
	</div>

	<div id="conteudo" class="texto" style="page-break-after: always;">
	<!-- pagina aqui -->
	<p><b>1.3.2 Demais membros do Conselho
	Municipal de Educação</b>:</p>
	<p>Nome:</p>
	<p>Representação:</p>
	<p>Endereço:</p>
	<p>Rua......................................................................................................................., nº...............</font></p>
	<p>Bairro:.................................................CEP:...........................................Estado:....</font></p>
	<p>Fone:                         <WBR>
	          Fax:                          <WBR>
	             e-mail: <br>
	</p>
	<p><b>1.3.2 Demais membros do Conselho
	Municipal de Educação</b>:</p>
	<p>Nome:</p>
	<p>Representação:</p>
	<p>Endereço:</p>
	<p>Rua......................................................................................................................., nº...............</font></p>
	<p>Bairro:.................................................CEP:...........................................Estado:....</font></p>
	<p>Fone:                                  
	Fax:                          <WBR>
	             e-mail:<br>
	</p>
	<p><b>1.3.2 Demais membros do Conselho
	Municipal de Educação</b>:</p>
	<p>Nome:</p>
	<p>Representação:</p>
	<p>Endereço:</p>
	<p>Rua......................................................................................................................., nº...............</font></p>
	<p>Bairro:.................................................CEP:...........................................Estado:....</font></p>
	<p>Fone:                         <WBR>
	          Fax:                          <WBR>
	             e-mail:</font> <br>
	</p>
	<p><b>1.3.2 Demais membros do Conselho
	Municipal de Educação</b>:</p>
	<p>Nome:</p>
	<p>Representação:</p>
	<p>Endereço:</p>
	<p>Rua.........................................................
	.............................................................., nº...............</p>
	<p>Bairro:.................................................CEP:...........................................Estado:....</font></p>
	<p>Fone:                                  
	Fax:                          <WBR>
	             e-mail: <br>
	</p>
	</div>

	<div id="conteudo" class="texto" style="page-break-after: always;"><!-- pagina aqui -->
	
	<!--
	<p><b>1.4. Comitê Local Plano de Metas
	Compromisso Todos pela Educação</b><br>
	</p>
	<p><b>1.4.1 Nome Completo do Presidente do
	Comitê</b></p>
	<p>____________________________________________</p>
	<p>Endereço do Presidente do Comitê:</p>
	<p>Rua......................................................................................................................, nº...............</font></p>
	<p>Bairro:.................................................CEP:..........................................Estado:..........<WBR>
	.........</p>
	<p>Fone:                                  
	Fax:                          <WBR>
	             e-mail:</font> <br>
	</p>
	<p><b>1.4.2 Demais membros do Comitê</b></p>
	<p>Nome:</p>
	<p>Representação:</p>
	<p>Endereço:</p>
	<p>Rua......................................................................................................................., nº...............</font></p>
	<p>Bairro:.................................................CEP:...........................................Estado:...................</font></p>
	<p>Fone:                         <WBR>
	          Fax:                          <WBR>
	             e-mail:</font> <br>
	</p>
	<p><b>1.4.2 Demais membros do Comitê</b></p>
	<p>Nome:</p>
	<p>Representação:</p>
	<p>Endereço:</p>
	<p>Rua......................................................................................................................., nº...............</font></p>
	<p>Bairro:.................................................CEP:...........................................Estado:...................</font></p>
	<p>Fone:                                <WBR>
	   Fax:                          <WBR>
	             e-mail: <br>
	 <br>
	</p>
	<p><b>1.4.2 Demais membros do Comitê</b></p>
	<p>Nome:</p>
	<p>Representação:</p>
	<p>Endereço:</p>
	<p>Rua......................................................................................................................., nº...............</p>
	<p>Bairro:.................................................CEP:...........................................Estado:...................</font></p>
	<p>Fone:                         <WBR>
	          Fax:                          <WBR>
	             e-mail:</font> <br>
	</p>
	</div>

	<div id="conteudo" class="texto" style="page-break-after: always;"><!-- pagina aqui -->
	
	<h1>
		<li>Resultados do Diagnóstico <i>in loco</i></li>
	</h1>
	<p align="justify">Neste item agregam-se 
as planilhas resultantes dos quadros de <b>Sistematização dos Critérios 
de Pontuação</b> e <b>Sistematização Geral por Dimensão</b> da 
Parte III do Instrumento de Campo. <br></p>
     
<div id="conteudo" class="texto" align="center"><center>
<table width="0" border="1" cellspacing="0">
<tr valign="top"><td width="100%" bgcolor="#E0E0E0" colspan="6" height="10"><font size="2" face="Arial"><b>Quadro 1. Síntese da pontuação  gerada 
  por dimensão e respectivos indicadores</b></font></td></tr>
<tr valign="top"><td width="14%" bgcolor="#E0E0E0" height="17">
  <font size="2" face="Arial">Dimensão</font></td>
  <td width="13%" bgcolor="#E0E0E0"><font size="2" face="Arial">Área</font></td>
  <td width="14%" bgcolor="#E0E0E0"><font size="2" face="Arial">Indicador</font></td>
  <td width="12%" bgcolor="#E0E0E0"><font size="2" face="Arial">Pontuação</font></td>
  <td width="30%" bgcolor="#E0E0E0"><font size="2" face="Arial">Justificativa</font></td>
  <td width="14%" bgcolor="#E0E0E0"><font size="2" face="Arial">Demanda 
  Potencial</font></td></tr>
<tr valign="top"><td width="14%" rowspan="9"> <br> <br>
   <br><font size="2" face="Arial">1</font></td>
  <td width="13%" rowspan="9"> <br> <br> <br><font size="2" face="Arial">1</font></td>
  <td width="14%"><font size="2" face="Arial">1</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="14%"><font size="2" face="Arial">2</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="14%"><font size="2" face="Arial">3</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="14%"><font size="2" face="Arial">4</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="14%"><font size="2" face="Arial">5</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="14%"><font size="2" face="Arial">6</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="14%"><font size="2" face="Arial">7</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="14%"><font size="2" face="Arial">8</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="14%"><font size="2" face="Arial">9</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="14%" rowspan="11"><font size="2" face="Arial"> </font></td>
  <td width="13%" rowspan="3"> <br><font size="2" face="Arial">2</font></td>
  <td width="14%"><font size="2" face="Arial">1</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="14%"><font size="2" face="Arial">2</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="14%"><font size="2" face="Arial">3</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="13%" rowspan="4"> <br><font size="2" face="Arial">3</font></td>
  <td width="14%"><font size="2" face="Arial">1</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="14%"><font size="2" face="Arial">2</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="14%"><font size="2" face="Arial">3</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="14%"><font size="2" face="Arial">4</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="13%" rowspan="2"><font size="2" face="Arial">4</font></td>
  <td width="14%"><font size="2" face="Arial">1</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="14%"><font size="2" face="Arial">2</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="13%" rowspan="2"><font size="2" face="Arial">5</font></td>
  <td width="14%"><font size="2" face="Arial">1</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="14%"><font size="2" face="Arial">2</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="14%" rowspan="10"> <br> <br>
  <font size="2" face="Arial">2</font></td>
  <td width="13%" rowspan="4"><font size="2" face="Arial">1</font></td>
  <td width="14%"><font size="2" face="Arial">1</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="14%"><font size="2" face="Arial">2</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="14%"><font size="2" face="Arial">3</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="14%"><font size="2" face="Arial">4</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="13%" rowspan="3"><font size="2" face="Arial">2</font></td>
  <td width="14%"><font size="2" face="Arial">1</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="14%"><font size="2" face="Arial">2</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="14%"><font size="2" face="Arial">3</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="13%"><font size="2" face="Arial">3</font></td>
  <td width="14%"><font size="2" face="Arial">1</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="13%"><font size="2" face="Arial">4</font></td>
  <td width="14%"><font size="2" face="Arial">1</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="13%"><font size="2" face="Arial">5</font></td>
  <td width="14%"><font size="2" face="Arial">1</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="14%" rowspan="8" height="14"> <br>
   <br> <br><font size="2" face="Arial">3</font></td>
  <td width="13%" rowspan="4"> <br> <br><font size="2" face="Arial">1</font></td>
  <td width="14%"><font size="2" face="Arial">1</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="14%" height="14"><font size="2" face="Arial">2</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="14%" height="14"><font size="2" face="Arial">3</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="14%" height="14"><font size="2" face="Arial">4</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="13%" rowspan="4"> <br><font size="2" face="Arial">2</font></td>
  <td width="14%"><font size="2" face="Arial">1</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="14%"><font size="2" face="Arial">2</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="14%"><font size="2" face="Arial">3</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="14%"><font size="2" face="Arial">4</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="14%" rowspan="15" height="1"> <br>
   <br><font size="2" face="Arial">4</font></td>
  <td width="13%" rowspan="8"><font size="2" face="Arial">1</font></td>
  <td width="14%"><font size="2" face="Arial">1</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="14%" height="1"><font size="2" face="Arial">2</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="14%" height="1"><font size="2" face="Arial">3</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="14%" height="1"><font size="2" face="Arial">4</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="14%" height="1"><font size="2" face="Arial">5</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="14%" height="1"><font size="2" face="Arial">6</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="14%" height="1"><font size="2" face="Arial">7</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="14%" height="1"><font size="2" face="Arial">8</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="13%" rowspan="2"><font size="2" face="Arial">2</font></td>
  <td width="14%"><font size="2" face="Arial">1</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="14%"><font size="2" face="Arial">2</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="13%" rowspan="5"> <br><font size="2" face="Arial">3</font></td>
  <td width="14%"><font size="2" face="Arial">1</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="14%"><font size="2" face="Arial">2</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="14%"><font size="2" face="Arial">3</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="14%"><font size="2" face="Arial">4</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
<tr valign="top"><td width="14%"><font size="2" face="Arial">5</font></td>
  <td width="12%"><font size="2" face="Arial"> </font></td>
  <td width="30%"><font size="2" face="Arial"> </font></td>
  <td width="14%"><font size="2" face="Arial"> </font></td></tr>
</table>

</center></div>
 <br> <br>	
<div>

<div id="conteudo" class="texto" style="page-break-after: always;">
<!-- pagina aqui -->
<p align="justify">     A 
partir da sistematização anterior, a totalização da pontuação 
por dimensão ficou assim distribuída:</font> <br> <br>
</p>

<div id="conteudo" align="left">
<table border="1" cellpadding="0" cellspacing="0">
<tr valign="top"><td width="100%" bgcolor="#E0E0E0" colspan="6"><b>Quadro 2. Total da pontuação gerada por 
  dimensão</b></td></tr>
<tr valign="top"><td width="42%" bgcolor="#E6E6E6" rowspan="2" height="7">
  <b>Dimensão</b></td>
  <td width="57%" bgcolor="#E6E6E6" colspan="5">
  <b>Pontuação</b></td></tr>
<tr valign="top"><td width="12%" bgcolor="#E6E6E6" height="7">
  <b>NSA</b></td>
  <td width="11%" bgcolor="#E6E6E6"><b>4</b></td>
  <td width="11%" bgcolor="#E6E6E6"><b>3</b></td>
  <td width="11%" bgcolor="#E6E6E6"><b>2</b></td>
  <td width="11%" bgcolor="#E6E6E6"><b>1</b></td></tr>
<tr valign="top"><td width="42%">Gestão 
  Educacional</td>
  <td width="12%"> </td>
  <td width="11%"> </td>
  <td width="11%"> </td>
  <td width="11%"> </td>
  <td width="11%"> </td></tr>
<tr valign="top"><td width="42%">Formação 
  de Professores e dos Profissionais de serviço e apoio escolar</font></td>
  <td width="12%"> </td>
  <td width="11%"> </td>
  <td width="11%"> </td>
  <td width="11%"> </td>
  <td width="11%"> </td></tr>
<tr valign="top"><td width="42%">Práticas 
  Pedagógicas e Avaliação</font></td>
  <td width="12%"> </td>
  <td width="11%"> </td>
  <td width="11%"> </td>
  <td width="11%"> </td>
  <td width="11%"> </td></tr>
<tr valign="top"><td width="42%"> Infra-estrutura 
  física e Recursos Pedagógicos</td>
  <td width="12%"> </td>
  <td width="11%"> </td>
  <td width="11%"> </td>
  <td width="11%"> </td>
  <td width="11%"> </td></tr>
<tr valign="top"><td width="42%" bgcolor="#E6E6E6"><b>TOTAL</b></td>
  <td width="12%" bgcolor="#E6E6E6"> </td>
  <td width="11%" bgcolor="#E6E6E6"> </td>
  <td width="11%" bgcolor="#E6E6E6"> </td>
  <td width="11%" bgcolor="#E6E6E6"> </td>
  <td width="11%" bgcolor="#E6E6E6"></td></tr>
</table>
</div>
 <br><p> <br></p>
<p><font size="3" face="Times New Roman"><b></b></font></p>
<p align="justify"><b>3. Definição das 
ações que farão parte do Plano de Ações Articuladas- PAR</b> <br>
</p>
<p align="justify"><b>3.1. Detalhamento 
das ações</b> <br></p>
<p align="justify">     Com 
base nas informações anteriores é feito o detalhamento das ações 
do PAR para o período de quatro anos e o cronograma financeiro para 
o primeiro ano. O detalhamento compreende:</p>
<ul>
<ul type="DISC">
  <li>especificação da ação, 
  o que corresponde a identificação dos diferentes e principais insumos 
  necessários à sua execução;</li>
  <li>caracterização da metodologia, 
  ou seja, como a ação será executada;</li>
  <li>estimativa do período de 
  realização, com as datas de seu início e término;</li>
  <li>nomeação do responsável, 
  na Prefeitura, pela execução da ação;</li>
  <li>resultados esperados;</li>
  <li>unidade de medida a ser usada 
  como referência básica para o dimensionamento físico e financeiro;</li>
  <li>quantidade anual, prevista 
  para cada ano do período de planejamento de 4 anos;  </li>
  <li>custo unitário estimado e 
  custos anuais;</li>
  <li>programa do MEC que poderá 
  apoiar técnica ou financeiramente a ação.</li>
</ul>
</ul>
<br>
</div>	
</div>

<div id="conteudo" class="texto" style="page-break-after: always;">
<!-- pagina aqui -->
<p align="justify">     Os 
resultados do detalhamento das ações e seus respectivos cronogramas 
devem ser registrados nos quadros a seguir:  <br></p>
<p align="justify">Quadro 5 - propostas 
de desenvolvimento das ações que necessitam de apoio financeiro do 
MEC, para o período de quatro anos.</p>
<p align="justify">Quadro 5.1 - Cronograma <br>
</p>
<p align="justify">Quadro 6 - propostas 
de desenvolvimento das ações que necessitam de apoio técnico do MEC.</p>
<p align="justify">Quadro 6.1 - Cronograma <br>
</p>
<p align="justify">Quadro 7 - propostas 
de desenvolvimento de ações que o município se propõe a desenvolver 
sem apoio direto do MEC;</p>
<p align="justify">Quadro 7.1 - Cronograma <br>
</p>
<p align="justify">Quadro 8 - Propostas 
de desenvolvimento de ações que demandam outras formas apoio.</p>
<p align="justify">Quadro 8.1 - Cronograma <br>
</p>
<p align="justify">Solicita-se a apresentação 
dos cronogramas físicos-financeiros apenas para o primeiro ano de execução 
do PAR, quando serão assinados os primeiros convênios e Termos de 
Cooperação. Anualmente serão assinados convênios para execução 
das demais ações do PAR, que não foram contempladas no primeiro convênio. 
Nesta oportunidade serão elaborados os respectivos cronogramas. <br>
 
 <br> <br> <br> <br></p>

</div>

<div id="conteudo" class="texto" style="page-break-after: always;">
<!-- pagina aqui -->
<?php $cabecalhoImprecao = false; ?>
<?php require_once('impressao_mun.inc'); ?>


<p> <br></p>
<p align="justify"></p>
<p><b>4. Acompanhamento e Avaliação</b> <br>
</p>
<p align="justify">     Durante 
a execução do PAR, o acompanhamento sistemático das ações é fundamental 
e contará, quando necessário, com o apoio da equipe técnica do MEC.</p>
<p align="justify">     A 
avaliação do processo de implementação do plano será realizada 
continuamente e deverá captar em que medida as estratégias e opções 
metodológicas utilizadas no desenvolvimento das ações são adequadas 
para concretizar os objetivos propostos.</p>
<p align="justify">     Assim, 
periodicamente serão enviados ao MEC relatórios com a explicitação, 
estruturada, de todos os procedimentos de acompanhamento e avaliação 
sistemática da execução do plano e dos resultados alcançados. <br>
</p>
<p align="justify"><b>6. Considerações 
Finais</b> <br></p>
<p align="justify">     A 
construção deste Plano de Ações Articuladas significa um grande 
avanço, pois representa um plano do Município para a melhoria da Educação 
Básica.</p>
<p align="justify">     A 
implantação com sucesso, deste plano depende não somente da mobilização 
e vontade política, mas também de mecanismos e instrumentos de acompanhamento 
e avaliação das diversas ações a serem desenvolvidas. Neste sentido, 
a Secretaria Municipal de Educação será a responsável pela coordenação 
do processo de implantação e consolidação do PAR, na figura do Dirigente 
Municipal de Educação. Além dela, desempenhará também um papel 
essencial nessas funções o Comitê Local do Compromisso Todos pela 
Educação e a sociedade civil organizada.</p>
<p align="justify">      Diante 
do exposto solicitamos análise da Comissão Técnica do Ministério 
da Educação e devidas providências para o desenvolvimento das ações 
propostas. <br></p>
<p align="right">______________________________________________ <br>
</p>
<p align="right">Prefeito (a)  Municipal  <br>
 <br> <br></p>

</div>


</div>
</div>
