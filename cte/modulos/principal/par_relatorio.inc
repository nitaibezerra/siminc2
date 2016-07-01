<?php


verifica_sessao();

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
			and d.itrid = %d
			and iu.estuf = '%s'
			and p.ptostatus = '%s'
		" , 
			$_SESSION['inuid'],
			$_SESSION['itrid'],
			$_SESSION['estuf'],
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
			inner join cte.dimensao d on d.itrid = ins.itrid and d.itrid = %d
			inner join cte.areadimensao ad on d.dimid = ad.dimid
			inner join cte.indicador i on i.ardid = ad.ardid
			inner join cte.criterio c on c.indid = i.indid
			left join cte.pontuacao pt on pt.crtid = c.crtid and pt.inuid = %d
		where
			d.dimstatus = 'A'
			and ad.ardstatus = 'A'  
			and i.indstatus = 'A' 
			and iu.estuf = '%s'
			and pt.inuid = %d
			and pt.ptostatus = '%s'
		group by
			ins.itrid,
			d.dimcod,
			d.dimdsc,
			c.ctrpontuacao,
			d.dimid
	", 
			$_SESSION['itrid']				
			,$_SESSION['inuid']				
			,$_SESSION['estuf']
			,$_SESSION['inuid']	,
			$ptostatus			
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

// ----- RECUPERA NOME DO ESTADO -----------------------------------------------
	$sqliuf = "select * from territorios.estado where estuf = '" 
				. $_SESSION["estuf"] . "'";
	$dadosuf = $db->recuperar( $sqliuf );
	if ( is_array( $dadosuf ) )
	{
		$estdescricao = $dadosuf['estdescricao'];
	}
	$estdescricao = $estdescricao == "null" ? "" : $estdescricao;
// ----- FIM RECUPERA NOME DO ESTADO -------------------------------------------

?>

<LINK rel="stylesheet" type="text/css" media="print, handheld" href="../includes/print.css">
<style type="text/css">
<!--
	body
	{
		font-family: sans-serif;
	}
	#geral
	{
		margin: 10px 10px 10px 10px;		
	}
	#geral #capa 
	{
		font: bold;
		text-align: center;
		vertical-align: middle;
	}
	#geral #capa .topo 
	{
		height: 9cm;
		font-size: 18px;
	}
	#geral #capa .meio
	{
		height: 14cm;
		font-size: 20px;
	}
	#geral #capa .rodape
	{
		height: 1cm;
		font-size: 14px;
	}
	#geral #conteudo{}
	#geral #conteudo h1
	{
		font-size: 18px;
		margin-left: 15px;
		list-style: decimal;
		margin-bottom: 30px;
	}
	#geral #conteudo h2
	{
		font-size: 16px;
		margin-left: 30px;
	}
	#geral #conteudo .texto
	{
		font-size: 12px;
		text-align: justify;
	}
	#geral #conteudo .texto ul
	{
		line-height: 30px;
		list-style: disc;
		margin-left: 40px;
		font-size: 12px;
	}
	#geral #conteudo p
	{
		font-size: 12px;
		text-indent: 60px;
		white-space: normal;
		line-height: 30px;
	}
	#geral #conteudo .textoTabela
	{
		font-size: 12px;
	}
	#geral #conteudo .tabela
	{
		font-size: 12px;
		width: 100%;
	}
	#geral #conteudo p
	{
		margin: 10px 10px 10px 10px;
	}
	#relatorioPar tr td
	{
		font-size: 10px;
	}
-->
</style>
<div id="geral">
	<div id="capa">
		<div style="display: none;"></div>
		<div class="topo">
			<p>GOVERNO DO ESTADO<br /><br />DE<br /><br /><a href="?modulo=principal/estrutura_avaliacao&acao=A"><?php echo $estdescricao; ?></a></p>
		</div>
		<div class="meio">
			<p>PLANO DE METAS COMPROMISSO TODOS PELA EDUCAÇÃO
			<br /><br />PLANO DE AÇÕES ARTICULADAS - PAR</p>
		</div>
		<div class="rodape">
			<p>2007</p>
		</div>
	</div>
	<div id="conteudo" style="page-break-before: always;">
		<ul>
		<center><h1>Equipe envolvida na elabora&ccedil;&atilde;o</h1></center>
		<div class="texto" style="page-break-after: always;">
			<p><?php echo $inueqploc ?></p> 
		</div>
		<center><h1 style="margin-top: 150px;">Apresenta&ccedil;ão</h1></center>
		<div class="texto" style="page-break-after: always;">
			<p>
			No momento da assinatura do Termo de Ades&atilde;o ao &nbsp;&nbsp; <i>Plano de Metas 
			Compromisso	Todos pela Educa&ccedil;&atilde;o</i>, assumimos o compromisso de 
			melhorar nossos indicadores educacionais a partir do desenvolvimento de 
			a&ccedil;&otilde;es que possibilitem o cumprimento das diretrizes 
			estabelecidas no referido Termo de Ades&atilde;o e tamb&eacute;m o alcance 
			das metas estabelecidas para o IDEB.
			</p>
			<p>
			Visando promover a melhoria da qualidade da Educa&ccedil;&atilde;o 
			B&aacute;sica oferecida neste Estado nos propomos a cumprir integralmente as 
			a&ccedil;&otilde;es propostas no presente Plano de A&ccedil;&otilde;es 
			Articuladas - PAR e, com a mesma responsabilidade estabeleceremos, em 
			parceria com o MEC, sistemas de acompanhamento e avalia&ccedil;&atilde;o das 
			a&ccedil;&otilde;es a serem desenvolvidas, al&eacute;m disso, &eacute; nosso 
			compromisso	divulgar a evolu&ccedil;&atilde;o dos dados educacionais 
			&agrave; popula&ccedil;&atilde;o local e estimula-la a participar e promover 
			o controle social de todas as a&ccedil;&otilde;es propostas neste documento.
			</p>
			<p>
			Nossa sugest&atilde;o de PAR concentra-se na melhoria gradativa dos 
			resultados educacionais e tem o aluno como o centro de todas as 
			decis&otilde;es. Assim, o seu principal objetivo &eacute; contribuir para o 
			desenvolvimento de aprendizagens, habilidades e compet&ecirc;ncias, atitudes 
			e valores necess&aacute;rios para a sua forma&ccedil;&atilde;o integral.
			</p>
			<p>
			A seguir apresentamos o PAR elaborado a partir do diagn&oacute;stico 
			realizado pela equipe da Secretaria de Estado de Educa&ccedil;&atilde;o em 
			conjunto com especialistas do MEC nos dias ____________ do mês de 
			____________________ de ______________.
			</p>
			<p align="right">
			<br /><br /><br /><br />
			      _______________________________________
			<br />Secret&aacute;rio (a) de Estado de Educa&ccedil;&atilde;o
			</p>
		</div>
		<h1><li>Dados Básicos de identificação</li></h1>
		<h2>1.1 Do Estado</h2>
		<div class="texto">
			<p><?php echo $inuddsdmgr ?></p>
		</div>
		<h2>1.2 Do Governo Estadual</h2>
		<div class="texto">
			<p><?php echo $inucadsec ?></p>
		</div>
		<h2>1.3 Comitê Local "Plano de Metas Compromisso Todos pela Educação</h2>
		<div class="texto" style="page-break-after: always;">
			<p><?php echo $inucadcmt ?></p>
		</div>
		<h1><li>Resultados do Diagnóstico <i>in loco</i></li></h1>
		<div class="texto">
			<p>Neste item agregam-se as planilhas resultantes dos quadros de&nbsp;&nbsp; 
			 <strong>Sistematização dos Critérios de Pontuação</strong> e <strong>Sistematização 
			Geral por Dimensão</strong> da Parte III do Instrumento do Campo, 
			conforme segue:</p>
		</div>
		<div class="tabela">
			<?php if($dadossi):?>
				<table border="0" class="textoTabela">
					<tr bgcolor="grey"> 
						<th colspan="6" align="left" style="color: white;">Quadro 1. Síntese dos conceitos gerados por dimensão e respectivos indicadores</th>
					</tr>
					<?php foreach( $dadossi as $key => $val ): ?>
					<?php if($key == 0 or $val["dimcod"] != $dadossi[$key - 1]["dimcod"]):?>
					<tr bgcolor="grey"> 
						<th colspan="6" align="left"><?php echo $val["dimcod"] . '. ' . $val["dimdsc"];?></th>
					</tr>
					<?php endif;?>
					<?php if($key == 0 or $val["ardcod"] != $dadossi[$key - 1]["ardcod"]):?>
					<tr bgcolor="#cccccc"> 
						<td></td>
						<th colspan="5" align="left"><?php echo $val["ardcod"] . '. ' . $val["arddsc"];?></th>
					</tr>
					<tr bgcolor="#cccccc"> 
						<td></td>
						<td></td>
						<th width="25">indicador</th>
						<th width="25">pontua&ccedil;&atilde;o</th>
						<th>justificativa</th>
						<th>demanda potencial</th>
					</tr>
					<?php $corSD = '#dfdfdf'; ?>
					<?php endif;?>
					<tr bgcolor="<?php echo $corSD; ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$corSD?>';"> 
						<td></td>
						<td></td>
						<td align="center"><?php echo $val["indcod"];?>&nbsp;</td>
						<td align="center"><?php echo $val["ctrpontuacao"];?>&nbsp;</td>
						<td><?php echo $val["ptojustificativa"];?>&nbsp;</td>
						<td>
								<?php print(trim($val["ptodemandaestadual"]) == "")?"":"<p><strong>Estadual:</strong> ".trim($val["ptodemandaestadual"]) . "</p>" ;?>
								<?php print(trim($val["ptodemandamunicipal"]) == "")?"":"<p><strong>Municipal:</strong> ".trim($val["ptodemandamunicipal"]) . "</p>";?>
						</td>
					</tr>
					<?php if($corSD == '#dfdfdf') $corSD = '#ffffff'; else $corSD = '#dfdfdf'; ?>
					<?php endforeach; ?>
				</table>
			<?php endif; ?>
		</div>
		<div class="texto">
			<p>A partir da sistematização acima, a totalização da pontuação por
			dimensão ficou distribuída conforme Quadro 2.</p>
		</div>
		<div class="tabela" style="page-break-after: always;">
			<?php if( isset($relatorioSD)): ?>
				<table border="0" class="textoTabela" width="95%">
					<thead>
					<tr bgcolor="grey">
						<th colspan="6" align="left" style="color: white;">Quadro 2. Total da pontuação gerada por dimensão</th>
					</tr>
					<tr bgcolor="grey">
						<th rowspan="2">Dimensão</th>
						<th colspan="5">Pontuação</th>
					</tr>
					<tr bgcolor="grey">
						<th>4</th>
						<th>3</th>
						<th>2</th>
						<th>1</th>
						<th>n/a</th>
					</tr>
					</thead>
					<?php foreach($relatorioSD as $keyr => $valr ): ?>
						<tr bgcolor="<?=$corSD?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$corSD?>';">
							<td><?php echo $valr["dimdsc"]; ?></td>
							<td align="right"><?php echo (int)$valr["4"]; ?>&nbsp;</td>
							<td align="right"><?php echo (int)$valr["3"]; ?>&nbsp;</td>
							<td align="right"><?php echo (int)$valr["2"]; ?>&nbsp;</td>
							<td align="right"><?php echo (int)$valr["1"]; ?>&nbsp;</td>
							<td align="right"><?php echo (int)$valr["0"]; ?>&nbsp;</td>
						</tr>
						<?php 
							$totalSD["t0"] += (int)$valr["0"];
							$totalSD["t1"] += (int)$valr["1"];
							$totalSD["t2"] += (int)$valr["2"];
							$totalSD["t3"] += (int)$valr["3"];
							$totalSD["t4"] += (int)$valr["4"];
							if($corSD == '#e7e7e7') $corSD = '#ffffff'; else $corSD = '#e7e7e7';
						?>
					<?php endforeach; ?>
						<tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='#FFFFFF';">
							<td align="right"><strong>Total:</strong></td>
							<td align="right"><strong><?php echo $totalSD["t4"]; ?>&nbsp;</strong></td>
							<td align="right"><strong><?php echo $totalSD["t3"]; ?>&nbsp;</strong></td>
							<td align="right"><strong><?php echo $totalSD["t2"]; ?>&nbsp;</strong></td>
							<td align="right"><strong><?php echo $totalSD["t1"]; ?>&nbsp;</strong></td>
							<td align="right"><strong><?php echo $totalSD["t0"]; ?>&nbsp;</strong></td>
						</tr>
						<tfoot>
						<tr >
								<td colspan="6" align="right">*n/a :  Não se Aplica.</td>
						</tr>
						<tfoot>
				</table>
			<?php endif; ?>
		</div>
		<h1><li>Definição das ações que farão parte do Plano de Ações Articuladas - PAR</li></h1>
		<h2>3.1 Detalhamento das ações</h2>
		<div class="texto" style="page-break-after: always;">
			<p>
			Com base nas informações acima, é feito o detalhamento das ações do 
			PAR para o período de quatro anos e o cronograma financeiro para o 
			primeiro ano. O detalhamento compreende:
			</p>
			<p>
			<ul>
				<li>
					especificação da ação e subação o que corresponde a 
					identificação dos diferentes e principais insumos 
					necessários à sua execução;
				</li>
				<li>
					estratégia de implementação, ou seja, como a ação/subação 
					será executada;
				</li>
				<li>
					estimativa do período de realização, com as datas de seu 
					início e término;
				</li>
				<li>
					nomeação do responsável, na SEE, pela execução da ação;
				</li>
				<li>
					resultados esperados;
				</li>
				<li>
					unidade de medida a ser usada como referência básica para o
					dimensionamento físico e financeiro;
				</li>
				<li>
					quantidade anual, prevista para cada ano do período de
					planejamento de 4 anos;
				</li>
				<li>
					custo unitário estimado e custos anuais;
				</li>
				<li>
					origem dos recursos - assinalar a forma e identificar o 
					programa do MEC que poderá apoiar técnica ou financeiramente 
					a ação.
				</li>
			</ul>
			</p>
			<p>
			Os resultados do detalhamento das ações e seus respectivos 
			cronogramas devem ser registrados nos quadros a seguir:
			</p>
			<p style="text-indent: 0px;">
			<strong>Quadro 3.</strong> propostas de desenvolvimento de ações para a 
			Dimensão 1. Gestão Educacional, para o período de quatro anos;<br />
			<strong>Quadro 4.</strong> propostas de desenvolvimento de ações para a 
			Dimensão 2. Formação de Professores e Profissionais de Apoio e 
			Serviço Escolar, para o período de quatro anos;<br />
			<strong>Quadro 5.</strong> propostas de desenvolvimento de ações para a 
			Dimensão 3. Práticas Pedagógicas e Avaliação, para o período de 
			quatro anos;<br />
			<strong>Quadro 6.</strong> propostas de desenvolvimento de ações para a 
			Dimensão 4. Infra-estrutura física e Recursos Pedagógicos, para o 
			período de quatro anos;
			</p>
		</div>
		<div class="tabela" style="page-break-after: always;">
			<?php $cabecalhoImprecao = false; ?>
			<?php require_once('impressao.inc'); ?>
		</div>
		<h1><li>Acompanhamento e Avaliação</li></h1>
		<div class="texto">
			<p>
			Durante a execução do PAR, o acompanhamento sistemático das ações é 
			fundamental e contará, quando necessário, com o apoio da equipe 
			técnica do MEC.
			</p>
			<p>
			A avaliação do processo de implementação do plano será realizada 
			continuamente e deverá captar em que medida as estratégias e opções 
			metodológicas utilizadas no desenvolvimento das ações são adequadas 
			para concretizar os objetivos propostos.
			</p>
			<p>
			Assim, periodicamente serão enviados ao MEC relatórios com a 
			explicitação, estruturada, de todos os procedimentos de 
			acompanhamento e avaliação sistemática da execução do plano e dos 
			resultados alcançados.
			</p>
		</div>
		<!-- GAMB PRA MANTER A NUMERAÇÃO DO TOPICO CONSIDERAÇÕES COMO 6 -->
		<li style="color: white;"></li>
		<!-- FIM GAMB -->
		<h1><li>Considerações Finais</li></h1>
		<div class="texto">
			<p>
			A construção deste Plano de Ações Articuladas significa um grande 
			avanço, pois representa um plano do Estado para a melhoria da 
			Educação Básica.
			</p>
			<p>
			A implantação deste plano depende não somente da mobilização e 
			vontade política, mas também de mecanismos e instrumentos de 
			acompanhamento e avaliação das diversas ações a serem desenvolvidas. 
			Neste sentido, a Secretaria de Estado de Educação será a responsável 
			pela coordenação do processo de implantação e consolidação do PAR.  
			Além dela, desempenhará também um papel essencial nessas funções o 
			Comitê Local do Compromisso Todos pela Educação e a sociedade civil 
			organizada.
			</p>
			<p>
			Diante do exposto solicitamos análise da Comissão Técnica do 
			Ministério da Educação e devidas providências para o desenvolvimento 
			das ações propostas.
			</p>
			<p align="right">
			<br /><br /><br /><br /><br /><br />
			____________________________________<br />
			Secretário (a) de Estado de Educação 
			</p>
		</div>
		</ul>
	</div>
</div>