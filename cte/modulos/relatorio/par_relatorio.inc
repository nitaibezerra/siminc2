<?php
//echo 'teste';
//exit;
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

	$estuf = cte_pegarEstuf($_SESSION["inuid"]);
	
// ----- RECUPERA NOME DO ESTADO -----------------------------------------------
	$sqliuf = "select * from territorios.estado where estuf = '" 
				. $estuf . "'";
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
		font-size:12px;
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
			<p><?=date("Y") ?></p>
		</div>
	</div>
	<ul>
	<?if(trim($inueqploc) != ""){ ?>
	<div id="conteudo" style="page-break-before: always;">
		
		<center><h1>Equipe envolvida na elabora&ccedil;&atilde;o</h1></center>
		
		<div class="texto" style="page-break-after: always;">
			<p><?php echo $inueqploc ?></p> 
			
	</div>
		
	<? } ?>
		
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
			conjunto com especialistas do MEC no dia <?=date("d") ?> do mês de 
			<?php echo mesAtual();?> de <?=date("Y") ?>.
			</p>
			<p align="right">
			<br /><br /><br /><br />
			      _______________________________________
			<br />Secret&aacute;rio (a) de Estado de Educa&ccedil;&atilde;o
			</p>
		</div>
		<div id="conteudo" class="texto" style="page-break-after: always;">
		<h1><li>Dados Básicos de identificação</li></h1>
		<h2>1.1 Do Estado</h2>
		<div class="texto">
			<p><?php //echo $inuddsdmgr ?></p>
			<iframe src = "http://portal.mec.gov.br/ideuf/layout/infEstado.php?estado=<?php echo $estuf ?>" width="100%"  frameborder="0" height="225px"></iframe>
		</div>
		<!-- 
		<h2>1.2 Do Governo Estadual</h2>
		<div class="texto">
			<p><?php //echo $inucadsec ?></p>
		</div>
		 -->
		</div>
		<div id="conteudo" class="texto" style="page-break-after: always;">
		<h1><li>Resultados do Diagnóstico <i>in loco</i></li></h1>
		<div class="texto">
			<p>Neste item agregam-se as planilhas resultantes dos quadros de&nbsp;&nbsp; 
			 <strong>Sistematização dos Critérios de Pontuação</strong> e <strong>Sistematização 
			Geral por Dimensão</strong> da Parte III do Instrumento do Campo, 
			conforme segue:</p>
		</div>
		<div class="tabela"><?
		
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
					inner join cte.pontuacao p on p.crtid = c.crtid
				where 
					p.ptostatus = 'A'
					and d.dimstatus = 'A'
					and ad.ardstatus = 'A'  
					and i.indstatus = 'A'
					and p.inuid = %d
				" , 
				$_SESSION['inuid']
			);
$resultado = $db->carregar($sql);

?>
			<?php if($resultado):?>
				<table border="0" width="95%" cellspacing="0" cellpadding="4" align="center" bgcolor="#DCDCDC" class="listagem">
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
						<td>
								<?php print(trim($val["ptodemandaestadual"]) == "")?"":"<p><b>Estadual:</b> ".trim($val["ptodemandaestadual"]) . "</p>" ;?>
								<?php print(trim($val["ptodemandamunicipal"]) == "")?"":"<p><b>Municipal:</b> ".trim($val["ptodemandamunicipal"]) . "</p>";?>
						</td>
					</tr>
					<?php if($cor == '#dfdfdf') $cor = '#ffffff'; else $cor = '#dfdfdf'; ?>
					<?php endforeach; ?>
				</table>
			<?php else: ?>
				<table class="tabela" align="center" bgcolor="#fafafa"><tr><td align="center" style="color:red;">Nenhum Indicador Pontuado.</td></tr></table>
			<?php endif; ?>
		</div>
		</div>
		
		
		
		
		
		
		
		<div id="conteudo" class="texto" style="page-break-after: always;">
			<div class="texto">
				<p>A partir da sistematização acima, a totalização da pontuação por
				dimensão ficou distribuída conforme Quadro 2.</p>
			</div>
		
		
		<div class="texto">
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
							group by ins.itrid, d.dimcod, d.dimdsc, c.ctrpontuacao , d.dimid" 
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

			if( isset($relatorio)): ?>
						<table border="0" width="95%" cellspacing="0" cellpadding="4" align="center" bgcolor="#DCDCDC" class="listagem">
							<thead>
							<tr style="border-bottom:4px solid black;">
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
								<tr bgcolor="<?=$cor?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$cor?>';">
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
								<tr >
										<td colspan="6" align="right">*n/a :  Não se Aplica.</td>
								</tr>
								<tfoot>
						</table>
					<?php else: ?>
						<table class="tabela" align="center" bgcolor="#fafafa"><tr><td align="center" style="color:red;">Nenhum Indicador Pontuado.</td></tr></table>
					<?php endif; ?>
				
				
		</div>
		
		
		</div>
		
		<div id="conteudo" class="tabela" style="page-break-after: always;">
		
	
		
		<h1><li>Definição das ações que farão parte do Plano de Ações Articuladas - PAR</li></h1>
		<h2>3.1 Detalhamento das ações</h2>
		
<?php

print '<br/>';

//monta_titulo( $titulo_modulo, '&nbsp;' );

$ptostatus = isset( $_REQUEST['ptostatus'] ) ? $_REQUEST['ptostatus'] : 'A';
$sql ="
	select
			d.dimid,
			d.dimcod || ' - ' || d.dimdsc as dimensao,
			area.ardcod || ' - ' || area.arddsc as area,
			area.ardid,			
			i.indcod || ' - ' || i.inddsc as indicador,
			i.indid,
			i.indcod,
			c.ctrpontuacao as pontuacao,
			p.ptojustificativa,
			p.ptodemandamunicipal,
			p.ptodemandaestadual,
			Case acao.acilocalizador
				when 'E' then 'Estadual'
				when 'M' then 'Municipal'
				end as Tipo,
			acao.aciid,
			acao.ptoid,
			acao.acidsc,
			acao.acirpns,
			acao.acicrg,
			to_char(acao.acidtinicial,'dd/mm/yyyy') as acidtinicial,
			to_char(acao.acidtfinal,'dd/mm/yyyy') as acidtfinal,
			acao.acirstd,
			acao.acilocalizador,
			subacao.sbaid,
			subacao.sbaporescola,
			prg.prgdsc as sbaprm,
			coalesce(u.unddsc,'') as unddsc,
			coalesce(f.frmdsc,'') as frmdsc,
			c.crtdsc,
			c.ctrpontuacao,
			f.frmid,
			subacao.sbadsc,
			subacao.sbastgmpl,
			subacao.sbapcr,
			subacao.sbauntdsc,
			length(p.ptodemandaestadual) as demandaestadual, 
			length(p.ptodemandamunicipal) as demandamunicipal
		from
			cte.dimensao d
			inner join cte.areadimensao area ON area.dimid = d.dimid
			inner join cte.indicador i ON i.ardid = area.ardid
			inner join cte.pontuacao p ON p.indid = i.indid and p.ptostatus = '" . $ptostatus . "'
			inner join cte.criterio c ON c.crtid = p.crtid
			inner join cte.instrumentounidade iu ON iu.inuid = p.inuid
			inner join cte.acaoindicador acao ON acao.ptoid = p.ptoid 
			inner join cte.subacaoindicador subacao ON subacao.aciid = acao.aciid
			inner join cte.unidademedida u on u.undid = subacao.undid
			inner join cte.formaexecucao f on f.frmid = subacao.frmid
			left join cte.programa prg on prg.prgid = subacao.prgid	
		where
			iu.inuid = '".$_SESSION['inuid']."' 
			
		order by
			d.dimcod,  
			area.ardcod,
			i.indcod, p.ptoid, acao.aciid, subacao.sbadsc
";
/*
 * length(p.ptodemandaestadual) as demandaestadual, 
		length(p.ptodemandamunicipal) as demandamunicipal, 
 * 
$sql = "
		select
			d.dimid,
			d.dimcod || ' - ' || d.dimdsc as dimensao,
			area.ardcod || ' - ' || area.arddsc as area,
			area.ardid,			
			i.indcod || ' - ' || i.inddsc as indicador,
			i.indid,
			i.indcod,
			c.ctrpontuacao as pontuacao,
			p.ptojustificativa,
			p.ptodemandamunicipal,
			p.ptodemandaestadual,
			Case acao.acilocalizador
			when 'E' then 'Estadual'
			when 'M' then 'Municipal'
			end as Tipo,
			acao.aciid,
			acao.ptoid,
			acao.acidsc,
			acao.acirpns,
			acao.acicrg,
			to_char(acao.acidtinicial,'dd/mm/yyyy') as acidtinicial,
			to_char(acao.acidtfinal,'dd/mm/yyyy') as acidtfinal,
			acao.acirstd,
			acao.acilocalizador,
			subacao.sbaid,
			subacao.undid,
			subacao.frmid,
			subacao.sbadsc,
			subacao.sbastgmpl,
			prg.prgdsc as sbaprm,
			subacao.sbapcr,
			coalesce(subacao.sba0ano, 0) as sba0ano,
			coalesce(subacao.sba1ano, 0) as sba1ano,
			coalesce(subacao.sba2ano, 0) as sba2ano,
			coalesce(subacao.sba3ano, 0) as sba3ano,
			coalesce(subacao.sba4ano, 0) as sba4ano,
			coalesce(subacao.sbaunt,0) as sbaunt,
			subacao.sbauntdsc,
			subacao.sba0ini,
			subacao.sba0fim,
			subacao.sba1ini,
			subacao.sba1fim,
			subacao.sba2ini,
			subacao.sba2fim,
			subacao.sba3ini,
			subacao.sba3fim,
			subacao.sba4ini,
			subacao.sba4fim,
			coalesce(u.unddsc,'') as unddsc,
			coalesce(f.frmdsc,'') as frmdsc,
			c.crtdsc,
			c.ctrpontuacao,
			f.frmid
		from
			cte.dimensao d
			inner join cte.areadimensao area ON area.dimid = d.dimid
			inner join cte.indicador i ON i.ardid = area.ardid
			inner join cte.pontuacao p ON p.indid = i.indid and p.ptostatus = '" . $ptostatus . "'
			inner join cte.criterio c ON c.crtid = p.crtid
			inner join cte.instrumentounidade iu ON iu.inuid = p.inuid
			inner join cte.acaoindicador acao ON acao.ptoid = p.ptoid
			inner join cte.subacaoindicador subacao ON subacao.aciid = acao.aciid
			inner join cte.unidademedida u on u.undid = subacao.undid
			inner join cte.formaexecucao f on f.frmid = subacao.frmid
			left join cte.programa prg on prg.prgid = subacao.prgid
		where
			iu.inuid = '".$_SESSION['inuid']."' 
		order by
			d.dimcod,  
			area.ardcod,
			i.indcod, p.ptoid, acao.aciid, subacao.sbaid;
";
*/
//dbg($sql,1 );
$dado = $db->carregar($sql); 
$i= 0;
$totalreg = count($dado);

$novaDimensao = $dado[$i]['dimid'];
$novaArea = $dado[$i]['ardid'];
$novoIndicador = $dado[$i]['indid'];
$novaAcao = $dado[$i]['aciid'];
$novasubAcao = $dado[$i]['sbaid'];

$totalGeralAno0 = 0;
$totalGeralAno1 = 0;
$totalGeralAno2 = 0;
$totalGeralAno3 = 0;
$totalGeralAno4 = 0;
$demanda = 0;

//dbg($dado);


?>


<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<?php if( !isset( $cabecalhoImprecao ) || $cabecalhoImprecao !== false ): ?>		
<thead>
	<th class="tdDegradde01" colspan="2" align="left">
	<?
	$sql = "select estdescricao from territorios.estado where estuf = '" . cte_pegarEstuf( $_SESSION['inuid'] ) . "'";
	$estado = $db->pegaUm($sql);
	?>
</thead>
<?php endif; ?>
<? 
while ( $i < $totalreg ) {
	
	$dimensao = $dado[$i]['dimid'];
		?>
		
					<tr>
						<td class="tdDivisaoItens" colspan="2"></td>
					</tr>
					<tr>
					<tr>
						<td class="SubTituloDireita tdDegradde02">
							<b>Dimensão</b>
						</td>
						<td class="tdDegradde02" style="text-align:left">	
							<?=$dado[$i]['dimensao']; ?>
						</td>
					</tr>
					<?
					while ($dimensao == $novaDimensao)
					{
						if($i >= $totalreg ) break;

						$area = $dado[$i]['ardid'];

				?>
								<tr>
									<td class="SubTituloDireita tdDegradde03">
										<b>Área</b>
									</td>
									<td class="tdDegradde03" style="text-align:left">	
										<?=$dado[$i]['area']; ?>
									</td>
								</tr>
								
						
								<?

								while ($area == $novaArea )
								{
									if($i >= $totalreg ) break;

									$indicador = $dado[$i]['indid'];
									
									while ($indicador == $novoIndicador )
									{
										if($i >= $totalreg ) break;
										//print 'Indicador'.$indicador.'<br>';
										//print 'IndicadorNOVO'.$novoIndicador.'<br>';

												?>
													<tr>
														<td class="SubTituloDireita tdDegradde04">
															<b>Indicador</b>
														</td>
														<td class="tdDegradde04" style="text-align:left">	
															<?=$dado[$i]['indicador']; ?>
														</td>
													</tr>
													<tr>
														<td class="SubTituloDireita tdDegradde05">
															<b>Critério / Pontuação</b>
														</td>
														<td class="tdDegradde05" style="text-align:left">	
															<?echo $dado[$i]['ctrpontuacao'] . ' - '.$dado[$i]['crtdsc']; ?>
														</td>
													</tr>
													<tr>
														<td class="SubTituloDireita">
															<b>Justificativa</b>
														</td>
														<td class="" style="text-align:left">	
															<?echo $dado[$i]['ptojustificativa']; ?>
														</td>
													</tr>
													<?if( $dado[$i]['ptodemandaestadual']){
													?>
													<tr>
														<td class="SubTituloDireita">
															<b>Demanda para Rede Estadual</b>
														</td>
														<td class="" style="text-align:left">	
															<?echo $dado[$i]['ptodemandaestadual']; ?>
														</td>
													</tr>
													<?}?>
													<?if($dado[$i]['ptodemandamunicipal']){
													?>
													<tr>
														<td class="SubTituloDireita">
															<b>Demanda para Redes Municipais</b>
														</td>
														<td class="" style="text-align:left">	
															<?echo $dado[$i]['ptodemandamunicipal']; ?>
														</td>
													</tr>
													<?}?>
										
													<?
													$acao = $dado[$i]['aciid'];
													while ($acao == $novaAcao)
													{
														if($i >= $totalreg ) break;
														//print 'AçãoFORA'.$acao.'<br>';
														//print 'NovaAçãoFORA'.$novaAcao.'<br><p>';
											?>
																<tr>
																	<td class="SubTituloDireita">
																		<b>Ação</b>
																	</td>
																	<td class="" style="text-align:left">
																		<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="left" >
																			<tr>
																				<td  class="SubTituloDireita" style="width:20%;">
																					Demanda:
																				</td>
																				
																				<td>
																					<?=$dado[$i]['tipo'];?>
																				</td>
																			</tr>	
																			<tr>
																				<td  class="SubTituloDireita">
																					Descrição da Ação:
																				</td>
																				
																				<td>
																					<?=$dado[$i]['acidsc']; ?>
																				</td>
																			</tr>
																			<tr>
																				<td  class="SubTituloDireita">
																					Nome do Responsável:
																				</td>
																				
																				<td>
																					<?=$dado[$i]['acirpns']; ?>
																				</td>
																			</tr>	
																			<tr>
																				<td  class="SubTituloDireita">
																					Cargo do Responsável:
																				</td>
																				
																				<td>
																					<?=$dado[$i]['acicrg']; ?>
																				</td>
																			</tr>			 
																			<tr>
																				<td  class="SubTituloDireita">
																					Período Inicial:
																				</td>
																				
																				<td>
																					<?=$dado[$i]['acidtinicial']; ?>
																				</td>
																			</tr>
																			<tr>
																				<td  class="SubTituloDireita">
																					Período Final:
																				</td>
																				
																				<td>
																					<?=$dado[$i]['acidtfinal']; ?>
																				</td>
																			</tr>
																			<tr>
																				<td  class="SubTituloDireita">
																					Resultado Esperado:
																				</td>
																				
																				<td>
																					<?=$dado[$i]['acirstd']; ?>
																				</td>
																			</tr>
																		</table>	
																	</td>
																</tr>			
																<?
																$subacao 	= $dado[$i]['sbaid'];
																$tipo 		= $dado[$i]['tipo'];
																$demandaE 	= $dado[$i]["demandaestadual"];
																$demandaM 	= $dado[$i]["demandamunicipal"];
																	while ($acao == $novaAcao ){
																		mostra_subacao();
																		if($i >= $totalreg ) break;
																	}
																}
															}
														?>	
												<tr>
													<td class="SubTituloDireita" >
													<b>Total Geral por Indicador</b>
													</td>
													<td>			
														<table class="listagem" width="100%">
															<thead>
																<th align="center">
																	<b>2007</b>
																</th>
																<th align="center">
																	<b>2008</b>
																</th>
																<th align="center">
																	<b>2009</b>
																</th>
																<th align="center">
																	<b>2010</b>
																</th>
																<th align="center">
																	<b>2011</b>
																</th>
																<th align="center">
																	<b>Total</b>
																</th>
															</thead>
															<tr>
																<td align="right">
																	<?=number_format($totalGeralIndicadorAno0,2,',','.');?>
																</td>
																<td align="right">
																	<?=number_format($totalGeralIndicadorAno1,2,',','.');?>
																</td>
																<td align="right">
																	<?=number_format($totalGeralIndicadorAno2,2,',','.');?>
																</td>
																<td align="right">
																	<?=number_format($totalGeralIndicadorAno3,2,',','.');?>
																</td>
																<td align="right">
																	<?=number_format($totalGeralIndicadorAno4,2,',','.');?>
																</td>
																<td align="right">
																	<?=number_format($totalGeralIndicadorAno0 + $totalGeralIndicadorAno1 + $totalGeralIndicadorAno2 + $totalGeralIndicadorAno3 + $totalGeralIndicadorAno4 ,2,',','.');?>
																</td>
															</tr>
														</table>
													</td>
												</tr>
												<?
												$totalGeralIndicadorAno0 = 0; 
												$totalGeralIndicadorAno1 = 0;
												$totalGeralIndicadorAno2 = 0;
												$totalGeralIndicadorAno3 = 0;
												$totalGeralIndicadorAno4 = 0;

								}
							?>
						<tr>
							<td class="SubTituloDireita" >
							<b>Total Geral por Área</b>
							</td>
							<td>			
								<table class="listagem" width="100%">
									<thead>
										<th align="center">
											<b>2007</b>
										</th>
										<th align="center">
											<b>2008</b>
										</th>
										<th align="center">
											<b>2009</b>
										</th>
										<th align="center">
											<b>2010</b>
										</th>
										<th align="center">
											<b>2011</b>
										</th>
										<th align="center">
											<b>Total</b>
										</th>
									</thead>
									<tr>
										<td align="right">
											<?=number_format($totalGeralAreaAno0,2,',','.');?>
										</td>
										<td align="right">
											<?=number_format($totalGeralAreaAno1,2,',','.');?>
										</td>
										<td align="right">
											<?=number_format($totalGeralAreaAno2,2,',','.');?>
										</td>
										<td align="right">
											<?=number_format($totalGeralAreaAno3,2,',','.');?>
										</td>
										<td align="right">
											<?=number_format($totalGeralAreaAno4,2,',','.');?>
										</td>
										<td align="right">
											<?=number_format($totalGeralAreaAno0 + $totalGeralAreaAno1 + $totalGeralAreaAno2 + $totalGeralAreaAno3+ $totalGeralAreaAno4,2,',','.');?>
										</td>
									</tr>
								</table>
							</td>
						</tr>
						<?
						$totalGeralAreaAno0 = 0; 
						$totalGeralAreaAno1 = 0;
						$totalGeralAreaAno2 = 0;
						$totalGeralAreaAno3 = 0;
						$totalGeralAreaAno4 = 0;

					}
?>
		<tr>
							<td class="SubTituloDireita" >
							<b>Total Geral por Dimensão</b>
							</td>
							<td>			
								<table class="listagem" width="100%">
									<thead>
										<th align="center">
											<b>2007</b>
										</th>
										<th align="center">
											<b>2008</b>
										</th>
										<th align="center">
											<b>2009</b>
										</th>
										<th align="center">
											<b>2010</b>
										</th>
										<th align="center">
											<b>2011</b>
										</th>
										<th align="center">
											<b>Total</b>
										</th>
									</thead>
									<tr>
										<td align="right">
											<?=number_format($totalGeralDimensaAno0,2,',','.');?>
										</td>
										<td align="right">
											<?=number_format($totalGeralDimensaAno1,2,',','.');?>
										</td>
										<td align="right">
											<?=number_format($totalGeralDimensaAno2,2,',','.');?>
										</td>
										<td align="right">
											<?=number_format($totalGeralDimensaAno3,2,',','.');?>
										</td>
										<td align="right">
											<?=number_format($totalGeralDimensaAno4,2,',','.');?>
										</td>
										<td align="right">
											<?=number_format($totalGeralDimensaAno0 + $totalGeralDimensaAno1 + $totalGeralDimensaAno2 + $totalGeralDimensaAno3+ $totalGeralDimensaAno4,2,',','.');?>
										</td>
									</tr>
								</table>
							</td>
						</tr>
						<?
						$totalGeralDimensaAno0 = 0; 
						$totalGeralDimensaAno1 = 0;
						$totalGeralDimensaAno2 = 0;
						$totalGeralDimensaAno3 = 0;
						$totalGeralDimensaAno4 = 0;


	}?>	
<tr>
	<td class="SubTituloDireita"  colspan="2" style="text-align: center;">
	<b>Total Geral</b>
	</td>
</tr>
<tr>
	<td colspan="2" >
		<table class="listagem" width="100%">
			<thead>
				<th align="center">
					<b>2007</b>
				</th>
				<th align="center">
					<b>2008</b>
				</th>
				<th align="center">
					<b>2009</b>
				</th>
				<th align="center">
					<b>2010</b>
				</th>
				<th align="center">
					<b>2011</b>
				</th>
				<th align="center">
					<b>Total</b>
				</th>
			</thead>
			<tr>
				<td align="right">
					<?=number_format($totalGeralAno0,2,',','.');?>
				</td>
				<td align="right">
					<?=number_format($totalGeralAno1,2,',','.');?>
				</td>
				<td align="right">
					<?=number_format($totalGeralAno2,2,',','.');?>
				</td>
				<td align="right">
					<?=number_format($totalGeralAno3,2,',','.');?>
				</td>
				<td align="right">
					<?=number_format($totalGeralAno4,2,',','.');?>
				</td>
				<td align="right">
					<?=number_format($totalGeralAno0 + $totalGeralAno1 + $totalGeralAno2 + $totalGeralAno3 + $totalGeralAno4 ,2,',','.');?>
				</td>
			</tr>
		</table>
	</td>
</tr>
</table>
	
	
<?
function recuperaQuantidades($sbaid, $cronograma){
	global $db;
	if($cronograma == 't'){
		$sqlQuantidade="SELECT
							sum(ano2007) AS ano2007, -- QUANTIDADE 2007
							sum(ano2008) AS ano2008, -- QUANTIDADE 2008
							sum(ano2009) AS ano2009, -- QUANTIDADE 2009 
							sum(ano2010) AS ano2010, -- QUANTIDADE 2010
							sum(ano2011) AS ano2011,  -- QUANTIDADE 2011 
							sum(qfaqtd)  AS qfaqtd
						FROM (	
						SELECT 
						CASE WHEN qfaano = '2007'  THEN sum(qfaqtd) END AS ano2007, -- QUANTIDADE 2007
						CASE WHEN qfaano = '2008'  THEN sum(qfaqtd) END AS ano2008, -- QUANTIDADE 2008
						CASE WHEN qfaano = '2009'  THEN sum(qfaqtd) END AS ano2009, -- QUANTIDADE 2009 
						CASE WHEN qfaano = '2010'  THEN sum(qfaqtd) END AS ano2010, -- QUANTIDADE 2010
						CASE WHEN qfaano = '2011'  THEN sum(qfaqtd) END AS ano2011, -- QUANTIDADE 2011
						sum(qfaqtd) as qfaqtd  
						FROM cte.qtdfisicoano 
						WHERE sbaid = '".$sbaid."'
						GROUP BY qfaano
						) AS quantidade";
		$quantidade = $db->carregar($sqlQuantidade);
		return $quantidade;
	}else{
		$sqlQuantidade = "
						SELECT
						sum(ano2007) AS ano2007, -- QUANTIDADE 2007
						sum(ano2008) AS ano2008, -- QUANTIDADE 2008
						sum(ano2009) AS ano2009, -- QUANTIDADE 2009 
						sum(ano2010) AS ano2010, -- QUANTIDADE 2010
						sum(ano2011) AS ano2011,  -- QUANTIDADE 2011 
						sum(sptunt)  AS sptunt
						FROM (
							SELECT  
							CASE WHEN sptano = '2007'  THEN sum(sptunt) END AS ano2007, -- QUANTIDADE 2007
							CASE WHEN sptano = '2008'  THEN sum(sptunt) END AS ano2008, -- QUANTIDADE 2008
							CASE WHEN sptano = '2009'  THEN sum(sptunt) END AS ano2009, -- QUANTIDADE 2009 
							CASE WHEN sptano = '2010'  THEN sum(sptunt) END AS ano2010, -- QUANTIDADE 2010
							CASE WHEN sptano = '2011'  THEN sum(sptunt) END AS ano2011, -- QUANTIDADE 2011
							sum(sptunt)  AS sptunt
							FROM cte.subacaoparecertecnico where sbaid =  '".$sbaid."'
							GROUP BY sptano
						) AS quantidade
		";
		$quantidade = $db->carregar($sqlQuantidade);
		return $quantidade;	
	}
}

function racuperaValor($sbaid)
{
    global $db;

    if (($sptunt_2007 = $db->pegaUm('select sptunt from cte.subacaoparecertecnico where sbaid = ' . $sbaid . ' and sptano = 2007')) == false)
        $sptunt_2007 = 1;

    if (($sptunt_2008 = $db->pegaUm('select sptunt from cte.subacaoparecertecnico where sbaid = ' . $sbaid . ' and sptano = 2008')) == false)
        $sptunt_2008 = 1;

    if (($sptunt_2009 = $db->pegaUm('select sptunt from cte.subacaoparecertecnico where sbaid = ' . $sbaid . ' and sptano = 2009')) == false)
        $sptunt_2009 = 1;

    if (($sptunt_2010 = $db->pegaUm('select sptunt from cte.subacaoparecertecnico where sbaid = ' . $sbaid . ' and sptano = 2010')) == false)
        $sptunt_2010 = 1;

    if (($sptunt_2011 = $db->pegaUm('select sptunt from cte.subacaoparecertecnico where sbaid = ' . $sbaid . ' and sptano = 2011')) == false)
        $sptunt_2011 = 1;

    $sql = '
    SELECT
        sum(ano2007) AS ano2007,
        sum(ano2008) AS ano2008,
        sum(ano2009) AS ano2009,
        sum(ano2010) AS ano2010,
        sum(ano2011) AS ano2011
    FROM (
        SELECT
        CASE WHEN cosano = 2007 THEN (cosqtd * cosvlruni)  END AS ano2007,
        CASE WHEN cosano = 2008 THEN (cosqtd * cosvlruni)  END AS ano2008,
        CASE WHEN cosano = 2009 THEN (cosqtd * cosvlruni)  END AS ano2009,
        CASE WHEN cosano = 2010 THEN (cosqtd * cosvlruni)  END AS ano2010,
        CASE WHEN cosano = 2011 THEN (cosqtd * cosvlruni)  END AS ano2011
    FROM cte.composicaosubacao where sbaid = ' . $sbaid . ') AS valores';

    return (array) $db->carregar($sql);
}


function recuperaDadosSub($sbaid){
	global $db;

		$sql="SELECT 
		sum(sba0ini) AS sba0ini, 
		sum(sba1ini) AS sba1ini, 
		sum(sba2ini) AS sba2ini, 
		sum(sba3ini) AS sba3ini, 
		sum(sba4ini) AS sba4ini, 
		
		sum(sba0fim) AS sba0fim, 
		sum(sba1fim) AS sba1fim, 
		sum(sba2fim) AS sba2fim, 
		sum(sba3fim) AS sba3fim, 
		sum(sba4fim) AS sba4fim
		FROM (
			SELECT 			
			CASE WHEN subp.sptano = '2007'  THEN coalesce( subp.sptinicio , 0 )END AS sba0ini, 
			CASE WHEN subp.sptano = '2008'  THEN coalesce( subp.sptinicio , 0 )END AS sba1ini, 
			CASE WHEN subp.sptano = '2009'  THEN coalesce( subp.sptinicio , 0 )END AS sba2ini, 
			CASE WHEN subp.sptano = '2010'  THEN coalesce( subp.sptinicio , 0 )END AS sba3ini, 
			CASE WHEN subp.sptano = '2011'  THEN coalesce( subp.sptinicio , 0 )END AS sba4ini, 
			
			CASE WHEN subp.sptano = '2007'  THEN coalesce( subp.sptfim , 0 )END AS sba0fim, 
			CASE WHEN subp.sptano = '2008'  THEN coalesce( subp.sptfim , 0 )END AS sba1fim, 
			CASE WHEN subp.sptano = '2009'  THEN coalesce( subp.sptfim , 0 )END AS sba2fim, 
			CASE WHEN subp.sptano = '2010'  THEN coalesce( subp.sptfim , 0 )END AS sba3fim, 
			CASE WHEN subp.sptano = '2011'  THEN coalesce( subp.sptfim , 0 )END AS sba4fim
			FROM	cte.subacaoindicador subacao
			--LEFT JOIN (	SELECT sbaid, cosano 
			--		FROM cte.composicaosubacao
			--		GROUP BY sbaid, cosano 	
			--	  ) as csi ON csi.sbaid = subacao.sbaid
			LEFT JOIN cte.subacaoparecertecnico subp ON subp.sbaid = subacao.sbaid --AND csi.cosano = subp.sptano
			WHERE	subacao.sbaid = '".$sbaid."'
		) AS dados";
		
	return $db->carregar($sql);
	
}




function mostra_subacao(){
	global $db;
	global $dado, $i, $totalreg;
	global $total, $totalGeralAno0 ,$totalGeralAno1, $totalGeralAno2, $totalGeralAno3, $totalGeralAno4;
	global $totalGeralIndicadorAno0 , $totalGeralIndicadorAno1, $totalGeralIndicadorAno2, $totalGeralIndicadorAno3, $totalGeralIndicadorAno4;
	global $totalGeralAreaAno0 , $totalGeralAreaAno1, $totalGeralAreaAno2, $totalGeralAreaAno3, $totalGeralAreaAno4;
	global $totalGeralDimensaAno0 ,$totalGeralDimensaAno1, $totalGeralDimensaAno2, $totalGeralDimensaAno3, $totalGeralDimensaAno4;
	global $novaDimensao, $novaArea, $novoIndicador, $novaAcao, $novasubAcao;
	global $tipo, $demandaE, $demandaM ;
	
	
	if($tipo == "Estadual"){
		$demanda = $demandaE;
	}else if($tipo == "Municipal"){
		$demanda = $demandaM;
	}

	if( $demanda > 0 ){

		if($novasubAcao != NULL){
			$sub 		= recuperaDadosSub($novasubAcao);
		
			$valor = 0;
			$valor 		= racuperaValor($novasubAcao);
			$quantidade = recuperaQuantidades($novasubAcao, $dado[$i]['sbaporescola']);
	
			//$valor2008 = $valor[0]['ano2008'];
			
			$valor2007 = $valor[0]['ano2007'] / ($quantidade[0]['ano2007'] > 0 ? $quantidade[0]['ano2007'] : 1) ;
			$valor2008 = $valor[0]['ano2008'] / ($quantidade[0]['ano2008'] > 0 ? $quantidade[0]['ano2008'] : 1);
			$valor2009 = $valor[0]['ano2009'] / ($quantidade[0]['ano2009'] > 0 ? $quantidade[0]['ano2009'] : 1);
			$valor2010 = $valor[0]['ano2010'] / ($quantidade[0]['ano2010'] > 0 ? $quantidade[0]['ano2010'] : 1);
			$valor2011 = $valor[0]['ano2011'] / ($quantidade[0]['ano2011'] > 0 ? $quantidade[0]['ano2011'] : 1);
			$existevalor = $valor2007 + $valor2008 + $valor2009 + $valor2010 + $valor2011;
			
			//$teste = $valor[0]['ano2008'] / ($quantidade[0]['ano2008'] > 0 ? $quantidade[0]['ano2008'] : 1);
		}
	
		if($dado[$i]['sbaporescola'] == 't'){
			$valorGeral = $quantidade[0]['qfaqtd'];
		}else{
			$valorGeral = $quantidade[0]['sptunt'];
		}
		
		for($x=0; $x < 5; $x++){
			switch($sub[0]['sba'.$x.'ini'] ) {
			case '1':
				$sub[0]['sba'.$x.'ini'] = "janeiro";
				break; 
			case '2':
				$sub[0]['sba'.$x.'ini'] = "fevereiro";
				break; 
			case '3':
				$sub[0]['sba'.$x.'ini'] = "março";
				break; 
			case '4':
				$sub[0]['sba'.$x.'ini'] = "abril";
				break; 
			case '5':
				$sub[0]['sba'.$x.'ini'] = "maio";
				break; 	
			case '6':
				$sub[0]['sba'.$x.'ini'] = "junho";
				break;
			case '7':
				$sub[0]['sba'.$x.'ini'] = "julho";
				break;
			case '8':
				$sub[0]['sba'.$x.'ini'] = "agosto";
				break;
			case '9':
				$sub[0]['sba'.$x.'ini'] = "setembro";
				break;
			case '10':
				$sub[0]['sba'.$x.'ini'] = "outubro";
				break;
			case '11':
				$sub[0]['sba'.$x.'ini'] = "novembro";
				break;
			case '12':
				$sub[0]['sba'.$x.'ini'] = "dezembro";
				break;
			}
			
			switch($sub[0]['sba'.$x.'fim'] ) {
			case '1':
				$sub[0]['sba'.$x.'fim'] = "janeiro";
				break; 
			case '2':
				$sub[0]['sba'.$x.'fim'] = "fevereiro";
				break; 
			case '3':
				$sub[0]['sba'.$x.'fim'] = "março";
				break; 
			case '4':
				$sub[0]['sba'.$x.'fim'] = "abril";
				break; 
			case '5':
				$sub[0]['sba'.$x.'fim'] = "maio";
				break; 	
			case '6':
				$sub[0]['sba'.$x.'fim'] = "junho";
				break;
			case '7':
				$sub[0]['sba'.$x.'fim'] = "julho";
				break;
			case '8':
				$sub[0]['sba'.$x.'fim'] = "agosto";
				break;
			case '9':
				$sub[0]['sba'.$x.'fim'] = "setembro";
				break;
			case '10':
				$sub[0]['sba'.$x.'fim'] = "outubro";
				break;
			case '11':
				$sub[0]['sba'.$x.'fim'] = "novembro";
				break;
			case '12':
				$sub[0]['sba'.$x.'fim'] = "dezembro";
				break;
			}
			
		}
		?>
						<tr>
							<td class="SubTituloDireita">
								<b>Sub-Ação</b>
							</td>
							<td class="" style="text-align:left">	
							
								<table  class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="left" >
									<tr>
										<td  class="SubTituloDireita" style="width:20%;" >
											Descrição da Subação:
										</td>
										
										<td>
											<?=$dado[$i]['sbadsc']; ?>
										</td>
									</tr>									
									<tr>
										<td  class="SubTituloDireita">
											Estratégia de Implementação:
										</td>
										
										<td>
											<?=$dado[$i]['sbastgmpl']; ?>
										</td>
									</tr>
									<tr>
										<td  class="SubTituloDireita">
											Programa:
										</td>
										
										<td>
											<?=$dado[$i]['sbaprm']; ?>
										</td>
									</tr>
									<tr>
										<td  class="SubTituloDireita">
											Unidade de Medida:
										</td>
										
										<td>
											<?=$dado[$i]['unddsc']; ?>
										</td>
									</tr>
									<tr>
										<td  class="SubTituloDireita">
											Forma de Execução
										</td>
										
										<td>
											<?=$dado[$i]['frmdsc'];?>
										</td>
									</tr>
									<tr>
										<td  class="SubTituloDireita">
											Instituição Parceira (se houver):
										</td>
										
										<td>
											<?=$dado[$i]['sbapcr']; ?>
										</td>
									</tr>
									<tr>
										<td  class="SubTituloDireita">
											Quantidades e Cronograma Físico
										</td>									
										<td>
											<table class="listagem" width="100%">
												<tr>
													<th>
														&nbsp;	
													</th>	
													<th align="center">
															<b>2007</b>
													</th>											
													<th align="center">
															<b>2008</b>
													</th>																								
													<th align="center">
															<b>2009</b>
													</th>
													<th align="center">
															<b>2010</b>
													</th>
													<th align="center">
															<b>2011</b>
													</th>
													<th align="center">
														<b>Total</b>
													</th>
													</tr>									
												<tr>
													<td align="right">
														<b>Quantidades:</b>
													</td>
													<td align="right">
														<?=number_format($quantidade[0]['ano2007'],0); ?>
													</td>
													<td align="right">
														<?=number_format($quantidade[0]['ano2008'],0); ?>
													</td>
													<td align="right">
														<?=number_format($quantidade[0]['ano2009'],0); ?>
													</td>
													<td align="right">
														<?=number_format($quantidade[0]['ano2010'],0); ?>
													</td>
													<td align="right">
														<?=number_format($quantidade[0]['ano2011'],0); ?>
													</td>
													<td align="right">
														<?
														$total = 	
														$quantidade[0]['ano2007'] +
														$quantidade[0]['ano2008'] +
														$quantidade[0]['ano2009'] +
														$quantidade[0]['ano2010'] +
														$quantidade[0]['ano2011'];
														echo number_format($total,0);
														 ?>
													</td>
												</tr>											
												<tr>
													<td style="width:20%;" align="right">
														<b>Cronograma Físico:</b>
													</td>
													<td align="right">
														<? if(!Empty($sub[0]['sba0ini'])) echo $sub[0]['sba0ini'] . " até " ; ?>  <?=$sub[0]['sba0fim']; ?>
													</td>
													<td align="right">
														<? if(!Empty($sub[0]['sba1ini'])) echo $sub[0]['sba1ini'] . " até " ; ?>  <?=$sub[0]['sba1fim']; ?>
													</td>
													
													<td align="right">
														<?if(!Empty($sub[0]['sba2ini'])) echo $sub[0]['sba2ini'] . " até " ; ?>  <?=$sub[0]['sba2fim']; ?>		
													</td>
													<td align="right">
														<?if(!Empty($sub[0]['sba3ini'])) echo $sub[0]['sba3ini'] . " até " ; ?> <?=$sub[0]['sba3fim']; ?>
													</td>
													<td align="right">
														<?if(!Empty($sub[0]['sba4ini'])) echo $sub[0]['sba4ini'] . " até " ; ?> <?=$sub[0]['sba4fim']; ?>
													</td>
													<td align="right">
														&nbsp;
													</td>
												</tr>											
											
	
											<?php if ( $existevalor > 0 ) { ?>
	
											
											
												<tr>
													<th align="right">
														<b></b>
													</th>
													<th align="right">
														<b>2007</b>
													</th>
													<th align="right">
														<b>2008</b>
													</th>
													<th align="right">
														<b>2009</b>
													</th>
													<th align="right">
														<b>2010</b>
													</th>
													<th align="right">
														<b>2011</b>
													</th>
													<th>
													<b></b>
													</th>
												</tr>
												<tr>
													<td style="width:20%;" align="right">
														<b>Valor Unitário:</b>
													</td>
													<td align="right">
														<?=number_format($valor2007,2,',','.');?>
													</td>
													<td align="right">
														<?=number_format($valor2008,2,',','.');?>
													</td>
													<td align="right">
														<?=number_format($valor2009 ,2,',','.');?>
													</td>
													<td align="right">
														<?=number_format($valor2010 ,2,',','.');?>
													</td>
													<td align="right">
														<?=number_format($valor2011 ,2,',','.');?>
													</td>
													
												</tr>
											
											
											
											
												<tr>
													<th>
														&nbsp;
													</th>
													<th align="center">
														<b>2007</b>
													</th>
													<th align="center">
														<b>2008</b>
													</th>
													<th align="center">
														<b>2009</b>
													</th>
													<th align="center">
														<b>2010</b>
													</th>
													<th align="center">
														<b>2011</b>
													</th>
													<th align="center">
														<b>Total</b>
													</th>
												</tr>
												<tr>
													<?
													$ano0 = $valor[0]['ano2007'];
													$ano1 = $valor[0]['ano2008'];
													$ano2 = $valor[0]['ano2009'];
													$ano3 = $valor[0]['ano2010'];
													$ano4 = $valor[0]['ano2011'];
													
													
													//$ano0 = $dado[$i]['sbaunt'] * $dado[$i]['sba0ano'];
													//$ano1 = $dado[$i]['sbaunt'] * $dado[$i]['sba1ano'];
													//$ano2 = $dado[$i]['sbaunt'] * $dado[$i]['sba2ano'];
													//$ano3 = $dado[$i]['sbaunt'] * $dado[$i]['sba3ano'];
													//$ano4 = $dado[$i]['sbaunt'] * $dado[$i]['sba4ano'];
	
													$total = $ano0 + $ano1 + $ano2 + $ano3 + $ano4;
						
													$totalGeralAno0 += $ano0;	
													$totalGeralAno1 += $ano1;
													$totalGeralAno2 += $ano2;
													$totalGeralAno3 += $ano3;
													$totalGeralAno4 += $ano4;
	
													$totalGeralIndicadorAno0 += $ano0;
													$totalGeralIndicadorAno1 += $ano1;
													$totalGeralIndicadorAno2 += $ano2;
													$totalGeralIndicadorAno3 += $ano3;
													$totalGeralIndicadorAno4 += $ano4;
	
													$totalGeralAreaAno0 += $ano0;
													$totalGeralAreaAno1 += $ano1;
													$totalGeralAreaAno2 += $ano2;
													$totalGeralAreaAno3 += $ano3;
													$totalGeralAreaAno4 += $ano4;
	
													$totalGeralDimensaAno0 += $ano0;
													$totalGeralDimensaAno1 += $ano1;
													$totalGeralDimensaAno2 += $ano2;
													$totalGeralDimensaAno3 += $ano3;
													$totalGeralDimensaAno4 += $ano4;
	
	
													 ?>
													<td  style="width:20%;" align="right">
														<b>Valores Anuais:</b>
													</td>
													<td align="right">
														<?=number_format($ano0,2,',','.');?>
													</td>											
													<td align="right">
														<?=number_format($ano1,2,',','.');?>
													</td>
													<td align="right">
														<?=number_format($ano2,2,',','.');?>
													</td>
													<td align="right">
														<?=number_format($ano3,2,',','.');?>
													</td>
													<td align="right">
														<?=number_format($ano4,2,',','.');?>
													</td>
													<td align="right">
														<?=number_format($total,2,',','.') ?>
													</td>
												</tr>		
											
											<?php }  ?>
												</table>																																	
										</td>
									</tr>
									
									<?php 
									
										if( $dado[$i]['frmdsc'] == "MEC - Transferência voluntária" ){
											
											$sqlComposicao = " select * from cte.composicaosubacao where sbaid = $novasubAcao ";
											if( $detalhamentoComposicao = $db->carregar($sqlComposicao) ){ ?>
												<tr>
													<td colspan="2" style="text-align: center; font-weight: bold; background: #ccc;">Detalhamento dos Itens de Composição</td>
												</tr>	
												<?php
													foreach( $detalhamentoComposicao as $arDetalhe ){
														$arDetalhes[$arDetalhe["cosano"]][] = $arDetalhe;
													}	
													//1782706 - Balcão empréstimo 
													ksort($arDetalhes);
													foreach( $arDetalhes as $nrAno => $arDetalhe ){
												?>
													<tr>
														<td class="SubTituloDireita"><?php echo $nrAno ?></td>
														<td>
															<table class="listagem" width="100%">
																<thead> 
																	<th>Identificação do Item</th>
																	<th>Un. Medida</th>
																	<th>Quantidade</th>
																	<th>Valor Unitário</th>
																	<th>Total</th>
																</thead> 
																<?php
																usort( $arDetalhe, "ordenarArray" ); 
																
																foreach( $arDetalhe as $arValores ){ 
																	$sqlUnidadeMedida = " select undddsc from cte.unidademedidadetalhamento where unddid = ". $arValores["unddid"];
																	$unidadeMedida = $db->pegaUm( $sqlUnidadeMedida );															
																?>
																	<tr>
																		<td><?php echo $arValores["cosdsc"] ?></td>
																		<td><?php echo $unidadeMedida ?></td>
																		<td align="right"><?php echo number_format( $arValores["cosqtd"], 2, '.', '' ) ?></td>
																		<td align="right"><?php echo "R$ ". number_format( $arValores["cosvlruni"], 2, ',', '.' ) ?></td>
																		<td align="right"><?php echo "R$ ". number_format( $arValores["cosqtd"] * $arValores["cosvlruni"], 2, ',', '.' ) ?></td>
																	</tr>
																<?php } ?>	
															</table>
														</td>
													</tr>
												<?php } ?>	
										<?php } ?>
									<?php } ?>
									
								</table>	
							</td>
						</tr>
						
					<?	
			}	
				$i++;	
				$demanda = 0;		
				$novaDimensao = $dado[$i]['dimid'];
				$novaArea = $dado[$i]['ardid'];
				$novoIndicador = $dado[$i]['indid'];
				$novaAcao = $dado[$i]['aciid'];
				$novasubAcao = $dado[$i]['sbaid'];

}

function ordenarArray( $a, $b ){
	return strcmp( strtolower( $a["cosdsc"] ), strtolower( $b["cosdsc"] ) );
}    	
	
	?>		
			
		</div>
		
		
		
		<div id="conteudo" class="tabela" style="page-break-after: always;">
			<?php //$cabecalhoImprecao = false; ?>
			<?php //require_once('impressao.inc'); ?>
		
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
		
	</div></ul>
</div>
</div>


<?php
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

	function montaResponsabilidadeEndereco($estuf,$funcao){
		global $db;
		$sql = "	select
				ent.entnome as nome,
				e.endbai as bairro,
				e.endcep as cep,
				'(' || ent.entnumdddcomercial || ') - ' || ent.entnumcomercial as telcomercial,
				'(' || ent.entnumdddfax || ') - ' || ent.entnumfax as telfax,
				e.estuf as estado,
				e.endlog as rua,
				ent.entemail as email
				m.mundescricao as mundsc
			from
			 
				entidade.endereco e 
				inner join entidade.entidade ent on e.entid = ent.entid
				INNER JOIN entidade.funcaoentidade fe ON fe.entid = ent.entid
				inner join territorios.estado est on est.estuf = e.estuf 
				inner join territorios.municipio m on m.muncod = est.muncodcapital 
			where 	
				e.estuf = '$estuf' and
				fe.funid = $funcao ";


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
			$saida .= "Município: " . $row['mundsc'];
			$saida .= "Fone: " .$row['telcomercial'] ."&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
			$saida .= "Fax: " . $row['telfax'];
			$saida .= "<p>e-mail: ".$row['email'] ."</font> <br>";
		}

		return $saida;
}
	
?>