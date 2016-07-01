<?php
cte_verificaSessao();

$ptostatus = isset( $_REQUEST['ptostatus'] ) ? $_REQUEST['ptostatus'] : 'A';
set_time_limit(0);
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
			inner join cte.pontuacao p on p.crtid = c.crtid and p.inuid = iu.inuid
		where 
			d.dimstatus = 'A'
			and ad.ardstatus = 'A'  
			and i.indstatus = 'A'
			and iu.inuid = %d
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

<LINK rel="stylesheet" type="text/css"
	media="print, handheld" href="../includes/print.css">
<style type="text/css">
<!--
body {
	font-family: sans-serif;
}

#geral {
	font-size: 12px;
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
$muncod = cte_pegarMuncod( $_SESSION['inuid'] );
$mundescricao = cte_pegarMundescricao( $muncod );

?>
</style>
<div id="geral">
<div id="capa">

<div class="topo">
<p>PREFEITURA MUNICIPAL DE<br />
<br />
DE<br />
<br />
<!-- <a href="?modulo=principal/estrutura_avaliacao&acao=A"> --><?php echo $mundescricao; ?><!-- </a> --></p>
</div>
<div class="meio">
<p>PLANO DE METAS COMPROMISSO TODOS PELA EDUCAÇÃO <br />
<br />
PLANO DE AÇÕES ARTICULADAS - PAR</p>
</div>
<div class="rodape">
<p><?=date("Y")?></p>
</div>
</div>
<div id="conteudo" class="texto" style="page-break-before: always;">
<ul>

<? if(trim($$inueqploc)!="") {?>
	<center>
	<h1>Equipe envolvida na elabora&ccedil;&atilde;o</h1>
	</center>
	<div class="texto" style="page-break-after: always;">
	<p><?php echo $inueqploc ?></p>
	</div>
	<center><? } ?>


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
	B&aacute;sica oferecida neste Munic&iacute;pio nos propomos a cumprir
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
	em conjunto com especialistas do MEC no dia <?=date("d") ?> do mês de <?php echo mesAtual();?>
	de <?=date("Y") ?> e convalidado por mim, Prefeito(a) Municipal.</p>
	<p align="right"><br />
	<br />
	<br />
	<br />
	_______________________________________ <br />
	Prefeito(a) Municipal</p>
	</div>
	<h1>
	<li>Dados Básicos de identificação</li>
	</h1>
	<h2>1.1 Do Município</h2>
	<div class="texto">
	<p><?php // echo $inuddsdmgr ?></p>
	<p><iframe
		src="http://portal.mec.gov.br/ide/2008/gerarTabela.php?estado1=&municipio=<?=$muncod ?>&tbl=1"
		width="100%" frameborder="0" height="400px"></iframe></p>
	</div>
	<h2>1.2 Da Prefeitura</h2>


	<div id="conteudo" class="texto" style="page-break-after: always;">
	<p><?php //echo $inucadcmt ?></p>
	<p>
	<?
	$texto = montaResponsabilidadeEndereco($muncod,2);

	if($texto){
		?>
	
	
	<h2>1.2.1 Nome Completo do(a) Prefeito (a) Municipal:</h2>
	<?
	echo $texto;
}
?>
	</p>
	</div>

	<p><?
	$texto = montaResponsabilidadeEndereco($muncod,6);
	if($texto){
		?>
	
	
	<h2>1.2.2 Nome Completo do (a)  Dirigente Municipal de Educação:</h2>
	<?
	echo $texto;
}
?>

</div>

<div id="conteudo" class="texto" style="page-break-after: always;">
<h1>2. Resultados do Diagnóstico <i>in loco</i></h1>
<p align="justify">Neste item agregam-se as planilhas resultantes dos
quadros de <b>Sistematização dos Critérios de Pontuação</b> e <b>Sistematização
Geral por Dimensão</b> da Parte III do Instrumento de Campo. <br>
</p>
      
<div id="conteudo" class="texto" align="center">
<center><?php
$itrid = cte_pegarItrid( $_SESSION['inuid'] );

$sql = sprintf("select distinct
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
					inner join cte.pontuacao p on p.crtid = c.crtid and p.inuid = iu.inuid
				where 
					p.ptostatus = 'A'
					and d.dimstatus = 'A'
					and ad.ardstatus = 'A'  
					and i.indstatus = 'A'
					and iu.inuid = %d
				" , 
$_SESSION['inuid']
);

$resultado = $db->carregar($sql);

?> <?php if($resultado):?>
<table border="0" width="95%" cellspacing="0" cellpadding="4"
	align="center" bgcolor="#DCDCDC" class="listagem">
	<?php foreach( $resultado as $key => $val ): ?>
	<?php if($key == 0 or $val["dimcod"] != $resultado[$key - 1]["dimcod"]):?>
	<tr>
		<th colspan="6" class="class1"><?php echo $val["dimcod"] . '. ' . $val["dimdsc"];?></th>
	</tr>
	<?php endif;?>
	<?php if($key == 0 or $val["ardcod"] != $resultado[$key - 1]["ardcod"]):?>
	<tr>
		<td></td>
		<th colspan="5" class="class2"><?php echo $val["ardcod"] . '. ' . $val["arddsc"];?></th>
	</tr>
	<tr>
		<td></td>
		<td></td>
		<th width="25">indicador</th>
		<th width="25">pontua&ccedil;&atilde;o</th>
		<th>justificativa</th>
		<th>demanda potencial</th>
	</tr>
	<?php $cor = '#dfdfdf'; ?>
	<?php endif;?>
	<tr bgcolor="<?php echo $cor; ?>">
		<td></td>
		<td></td>
		<td align="center"><?php echo $val["indcod"];?>&nbsp;</td>
		<td align="center"><?php echo $val["ctrpontuacao"];?>&nbsp;</td>
		<td><?php echo $val["ptojustificativa"];?>&nbsp;</td>
		<td><?php print(trim($val["ptodemandaestadual"]) == "")?"":"<p><b>Estadual:</b> ".trim($val["ptodemandaestadual"]) . "</p>" ;?>
		<?php print(trim($val["ptodemandamunicipal"]) == "")?"":"<p><b>Municipal:</b> ".trim($val["ptodemandamunicipal"]) . "</p>";?>
		</td>
	</tr>
	<?php if($cor == '#dfdfdf') $cor = '#ffffff'; else $cor = '#dfdfdf'; ?>
	<?php endforeach; ?>
</table>
	<?php else: ?>
<table class="tabela" align="center" bgcolor="#fafafa">
	<tr>
		<td align="center" style="color: red;">Nenhum Indicador Pontuado.</td>
	</tr>
</table>
	<?php endif; ?></center>
</div>
 <br>
 <br>
</div>



<div id="conteudo" class="texto" style="page-break-after: always;"><!-- pagina aqui -->
<p align="justify">     A partir da sistematização anterior, a
totalização da pontuação por dimensão ficou assim distribuída:</font> <br>
 <br>
</p>

<h2>2.1 Total da pontuação gerada por dimensão</h2>
	<?php
	$itrid = cte_pegarItrid( $_SESSION['inuid'] );

	$sql = sprintf("select distinct
					ins.itrid
					,d.dimid
					,d.dimcod || '. ' ||d.dimdsc as dimdsc
					,c.ctrpontuacao 
					,count ( c.ctrpontuacao ) as qtpontos
					
				from cte.instrumento ins
					inner join cte.instrumentounidade iu on iu.itrid = ins.itrid and iu.inuid = %d
					inner join cte.dimensao d on d.itrid = ins.itrid
					inner join cte.areadimensao ad on d.dimid = ad.dimid
					inner join cte.indicador i on i.ardid = ad.ardid
					inner join cte.criterio c on c.indid = i.indid
					left join cte.pontuacao pt on pt.crtid = c.crtid and pt.indid = i.indid and pt.inuid = %d
				where
					pt.ptostatus = 'A'
					and d.dimstatus = 'A'
					and ad.ardstatus = 'A'  
					and i.indstatus = 'A'
				group by ins.itrid, d.dimcod, d.dimdsc, c.ctrpontuacao , d.dimid
				order by dimdsc" 
				,$_SESSION['inuid']
				,$_SESSION['inuid']
				);

				if( $resultado = $db->carregar($sql) )
				{
					//percorrendo o resultado e criando um array por dimensao
					foreach($resultado as $key => $val )
					{
						$cor = $icone ? '#959595' : '#133368';
						$relatorio[$val["dimid"]]["dimdsc"] = $val["dimdsc"];
						switch($val["ctrpontuacao"])
						{
							case "0":
								$relatorio[$val["dimid"]]["0"] = $val["qtpontos"];
								break;
							case "1":
								$relatorio[$val["dimid"]]["1"] = $val["qtpontos"];
								break;
							case "2":
								$relatorio[$val["dimid"]]["2"] = $val["qtpontos"];
								break;
							case "3":
								$relatorio[$val["dimid"]]["3"] = $val["qtpontos"];
								break;
							case "4":
								$relatorio[$val["dimid"]]["4"] = $val["qtpontos"];
								break;
						}
					}
				}

				$total["t0"] = 0;
				$total["t1"] = 0;
				$total["t2"] = 0;
				$total["t3"] = 0;
				$total["t4"] = 0;
				$cor = '#e7e7e7';
				?> <?php if( isset($relatorio)): ?>
<table border="0" width="95%" cellspacing="0" cellpadding="4"
	align="center" bgcolor="#DCDCDC" class="listagem">
	<thead>
		<tr style="border-bottom: 4px solid black;">
			<th rowspan="2">Dimensão</th>
			<th colspan="5">Pontuação</th>
		</tr>
		<tr>
			<th>4</th>
			<th>3</th>
			<th>2</th>
			<th>1</th>
			<th>n/a</th>
		</tr>
	</thead>
	<?php foreach($relatorio as $keyr => $valr ): ?>
	<tr bgcolor="<?=$cor?>" onmouseover="this.bgColor='#ffffcc';"
		onmouseout="this.bgColor='<?=$cor?>';">
		<td><?php echo $valr["dimdsc"]; ?></td>
		<td align="right"><?php echo (int)$valr["4"]; ?>&nbsp;</td>
		<td align="right"><?php echo (int)$valr["3"]; ?>&nbsp;</td>
		<td align="right"><?php echo (int)$valr["2"]; ?>&nbsp;</td>
		<td align="right"><?php echo (int)$valr["1"]; ?>&nbsp;</td>
		<td align="right"><?php echo (int)$valr["0"]; ?>&nbsp;</td>
	</tr>
	<?php
	$total["t0"] += (int)$valr["0"];
	$total["t1"] += (int)$valr["1"];
	$total["t2"] += (int)$valr["2"];
	$total["t3"] += (int)$valr["3"];
	$total["t4"] += (int)$valr["4"];
	if($cor == '#e7e7e7') $cor = '#ffffff'; else $cor = '#e7e7e7';
	?>
	<?php endforeach; ?>
	<tr>
		<td align="right"><b>Total:</b></td>
		<td align="right"><b><?php echo $total["t4"]; ?>&nbsp;</b></td>
		<td align="right"><b><?php echo $total["t3"]; ?>&nbsp;</b></td>
		<td align="right"><b><?php echo $total["t2"]; ?>&nbsp;</b></td>
		<td align="right"><b><?php echo $total["t1"]; ?>&nbsp;</b></td>
		<td align="right"><b><?php echo $total["t0"]; ?>&nbsp;</b></td>
	</tr>
	<tfoot>
		<tr>
			<td colspan="6" align="right">*n/a : Não se Aplica.</td>
		</tr>
	
	
	<tfoot>

</table>
	<?php else: ?>
<table class="tabela" align="center" bgcolor="#fafafa">
	<tr>
		<td align="center" style="color: red;">Nenhum Indicador Pontuado.</td>
	</tr>
</table>
	<?php endif; ?></div>
<!-- 
<div id="conteudo" class="texto" style="page-break-after: always;">
<!-- pagina aqui
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
<!-- pagina aqui -->
<div id="conteudo" class="texto" style="page-break-after: always;"><?php $cabecalhoImprecao = false; ?>
<h2>3 Ações do Plano de Ações Articuladas - PAR</h2>
<p>


<h2>3.1 Detalhamento das ações</h2>
	<?php require_once('impressao_mun.inc'); ?></div>

<p> <br>


<h2>4. Acompanhamento e Avaliação</h2>
  <br>
</p>
<p align="justify">     Durante a execução do PAR, o acompanhamento
sistemático das ações é fundamental e contará, quando necessário, com o
apoio da equipe técnica do MEC.</p>
<p align="justify">     A avaliação do processo de implementação do
plano será realizada continuamente e deverá captar em que medida as
estratégias e opções metodológicas utilizadas no desenvolvimento das
ações são adequadas para concretizar os objetivos propostos.</p>
<p align="justify">     Assim, periodicamente serão enviados ao MEC
relatórios com a explicitação, estruturada, de todos os procedimentos de
acompanhamento e avaliação sistemática da execução do plano e dos
resultados alcançados. <br>
</p>
<p align="justify">


<h2>5. Considerações Finais</h2>
 <br>
</p>
<p align="justify">     A construção deste Plano de Ações Articuladas
significa um grande avanço, pois representa um plano do Município para a
melhoria da Educação Básica.</p>
<p align="justify">     A implantação com sucesso, deste plano depende
não somente da mobilização e vontade política, mas também de mecanismos
e instrumentos de acompanhamento e avaliação das diversas ações a serem
desenvolvidas. Neste sentido, a Secretaria Municipal de Educação será a
responsável pela coordenação do processo de implantação e consolidação
do PAR, na figura do Dirigente Municipal de Educação. Além dela,
desempenhará também um papel essencial nessas funções o Comitê Local do
Compromisso Todos pela Educação e a sociedade civil organizada.</p>
<p align="justify">      Diante do exposto solicitamos análise da
Comissão Técnica do Ministério da Educação e devidas providências para o
desenvolvimento das ações propostas. <br>
</p>
<p>&nbsp;</p>
<p>&nbsp;</p>
<p>&nbsp;</p>
<p>&nbsp;</p>
<p>&nbsp;</p>
<p>&nbsp;</p>

<p align="right">______________________________________________ <br>
</p>
<p align="right">Prefeito (a)  Municipal  <br>
 <br>
 <br>
</p>

</div>
</div>

</div>
</div>

	<?php
	function montaResponsabilidadeEndereco($muncod,$funcao){
		global $db;
				/*
		$sql = "	select
				ent.entnome as nome,
				e.endbai as bairro,
				e.endcep as cep,
				'(' || ent.entnumdddcomercial || ') - ' || ent.entnumcomercial as telcomercial,
				'(' || ent.entnumdddfax || ') - ' || ent.entnumfax as telfax,
				e.estuf as estado,
				e.endlog as rua,
				ent.entemail as email
	
			from
			 
				entidade.endereco e 
				inner join entidade.entidade ent on e.entid = ent.entid
				INNER JOIN entidade.funcaoentidade fe ON fe.entid = ent.entid
			where 	
				e.muncod = '$muncod' and
				fe.funid = $funcao ";
		//dbg($sql,1);
		*/
				
				$sql = "	SELECT
								entprefeito.entnome as nome,
								entd.endbai as bairro,
								entd.endcep as cep,
								'(' || entprefeito.entnumdddcomercial || ') - ' || entprefeito.entnumcomercial as telcomercial,
								'(' || entprefeito.entnumdddfax || ') - ' || entprefeito.entnumfax as telfax,
								entd.estuf as estado,
								entd.endlog as rua,
								entprefeito.entemail as email
							FROM entidade.entidade entprefeito 
							INNER JOIN entidade.funcaoentidade funprefeito ON entprefeito.entid = funprefeito.entid 
							INNER JOIN entidade.funentassoc feaprefeito ON feaprefeito.fueid = funprefeito.fueid 
							INNER JOIN entidade.entidade entprefeitura ON entprefeitura.entid = feaprefeito.entid 
							INNER JOIN entidade.funcaoentidade funprefeitura ON funprefeitura.entid = entprefeitura.entid 
							INNER JOIN entidade.endereco entd ON entd.entid = entprefeitura.entid
							INNER JOIN territorios.municipio mun ON entd.muncod = mun.muncod 
							WHERE funprefeito.funid='$funcao'  AND entd.muncod = '$muncod'"; //AND funprefeitura.funid=1

		$dado = $db->carregar($sql);


		$saida = "";

		if(count($dado) == 0 or $dado == false) return false;

		foreach ($dado as $row)
		{
			$saida .= "<p><font size='3' face='Arial'>". $row['nome']." </font> <br>";
			$saida .= "</p>";
			$saida .= "<p>Endereço da prefeitura: ".$row['rua']."</p>";
			$saida .= "<p>Bairro: ".$row['bairro'] ."</p>";
			$saida .= "<p>CEP:". $row['cep']."</p>";
			$saida .= "<p>Estado: " . $row['estado'] . "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
			$saida .= "Fone: " .$row['telcomercial'] ."&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
			$saida .= "Fax: " . $row['telfax'];
			$saida .= "<p>e-mail: ".$row['email'] ."</font> <br>";
		}

		return $saida;
	}



	function mesAtual(){

		$mes = (int)date("m");


		$saida  = array();

		$saida[1] 	= "Janeiro";
		$saida[2] 	= "Fevereiro";
		$saida[3] 	= "Março";
		$saida[4] 	= "Abril";
		$saida[5] 	= "Maio";
		$saida[6] 	= "Junho";
		$saida[7] 	= "Julho";
		$saida[8] 	= "Agosto";
		$saida[9] 	= "Setembro";
		$saida[10] 	= "Outubro";
		$saida[11] 	= "Novembro";
		$saida[12] 	= "Dezembro";



		return $saida[$mes];

	}

	?>
