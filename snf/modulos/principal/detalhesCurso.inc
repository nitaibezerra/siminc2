<?php
if($_REQUEST['requisicaoAjax']){
	$_REQUEST['requisicaoAjax']();
	die;
}

function gerarXmlCursos() {
	global $db;
	
	ini_set("memory_limit", "2048M");
	set_time_limit(0);
	
	if($_SESSION['snf']['curid']){
		$arrWhere[] = "p.curid = '{$_SESSION['snf']['curid']}'";
	}
	if($_SESSION['snf']['ncuid']){
		$arrWhere[] = "p.ncuid = '{$_SESSION['snf']['ncuid']}'";
	}
	if($_SESSION['snf']['modid']){
		$arrWhere[] = "p.modid = '{$_SESSION['snf']['modid']}'";
	}
	if($_SESSION['snf']['ateid']){
		$arrWhere[] = "p.ateid = '{$_SESSION['snf']['ateid']}'";
	}
	if($_SESSION['snf']['pcfid']){
		$arrWhere[] = "p.pcfano in ({$_SESSION['snf']['pcfid']})";
	}
	
	$sql = "select * from (
				select 
				m.mundescricao,
				ST_X(m.munlatlong) as lng,
				ST_Y(m.munlatlong) as lat,
				p.moddesc,
				p.ncudesc,
	            coalesce(sum(p.privagasprevistas),0) +  coalesce(sum(p.pridemvagasprevistas),0) as demanda,
	            p.estuf
				from snf.prioridadecursoescola p
				inner join territoriosgeo.municipio m using(muncod) 
				inner join snf.prioridadedocumento pd using(prdid)
				inner join workflow.documento d using(docid)
				where p.pristatus = 'A'
				".($arrWhere ? " and ".implode(" and ", $arrWhere) : "")."
				and d.esdid = 464 ---FIXO!!!!!
				group by m.munlatlong,m.munlatlong,m.muncod,m.mundescricao,p.moddesc,p.ncudesc,p.estuf ) as foo where demanda > 0";
	
	$dados = $db->carregar($sql);
	if($dados):
		$conteudo .= "<markers> "; // inicia o XML
		foreach($dados as $d):				
			$ajuste = (($_position_ex[$d['lat']][$d['lng']])?($_position_ex[$d['lat']][$d['lng']]*0.0056):0);
			$conteudo .= "<marker "; //inicia um ponto no mapa
			$conteudo .= "mundescricao=\"". $d['mundescricao'] ."\" ";
			$conteudo .= "moddesc=\"". $d['moddesc'] ."\" ";
			$conteudo .= "ncudesc=\"". $d['ncudesc'] ."\" ";
			$conteudo .= "demanda=\"". number_format($d['demanda'],0,"",".") ."\" ";
			$conteudo .= "estuf=\"". $d['estuf'] ."\" ";
			$conteudo .= "lat=\"". $d['lat'] ."\" ";
			$conteudo .= "lng=\"". ($d['lng']+$ajuste) ."\" ";
			$conteudo .= "icon=\"/imagens/icone_capacete_pre1.png\"";
			$conteudo .= "/> ";
			$_position_ex[$d['lat']][$d['lng']]++;
		endforeach;
		
		$conteudo .= "</markers> ";
		header ("Content-Type:text/xml");
		print $conteudo;
		unset($conteudo);
	endif;
}

function gerarXmlOferta() {
	global $db;
	
	ini_set("memory_limit", "2048M");
	set_time_limit(0);
	
	if($_SESSION['snf']['curid']){
		$arrWhere[] = "curid = '{$_SESSION['snf']['curid']}'";
	}

	$sql = "select 
				ST_X(m.munlatlong) as lng,
				ST_Y(m.munlatlong) as lat,
				*
			 from
			(select o.curid, o.ofeid, en.estuf, m.muncod, m.mundescricao, pu.pobdescricao as nome, o.ofeqtde as qtde, ies.insnome
			from snf.oferta o
            inner join snf.polouab pu
            using(pobid)
            inner join snf.instituicaouab iu
            on iu.pobid = pu.pobid
            inner join snf.instituicaoensino ies
            on ies.insid = iu.insid
            inner join snf.enderecopolo en
            on en.endid = pu.endid
            inner join territorios.municipio m
            on m.muncod = en.muncod
			UNION
			select o.curid, o.ofeid, en.estuf, m.muncod, m.mundescricao, pu.poldescricao as nome, o.ofeqtde as qtde, ies.insnome
			from snf.oferta o
            inner join snf.polo pu
            using(polid)
            inner join snf.instituicaoensino ies
            on ies.insid = pu.insid
            inner join snf.enderecopolo en
            on en.endid = pu.endid
            inner join territorios.municipio m
            on m.muncod = en.muncod
			UNION
			select o.curid, o.ofeid, en.estuf, m.muncod, m.mundescricao, ent.entnome as nome, o.ofeqtde as qtde, ies.insnome
			from snf.oferta o
            inner join academico.campus c
            on o.cmpid = c.cmpid 
            inner join entidade.entidade ent
            on c.entid = ent.entid
            inner join entidade.funcaoentidade fen
            on fen.entid = ent.entid
            and fen.funid = 18
            inner join entidade.funentassoc fea
            on fea.fueid = fen.fueid
            inner join entidade.entidade I
            on I.entid = fea.entid
            inner join entidade.funcaoentidade fies
            on fies.entid = I.entid
            and fies.funid in (11,12)
            inner join snf.instituicaoensino ies
            on ies.entid = fies.entid
            inner join entidade.endereco en
            on c.entid = en.entid
            inner join territorios.municipio m
            on m.muncod = en.muncod
			) AS OFERTA
			INNER JOIN
				territoriosgeo.municipio m ON m.muncod = OFERTA.muncod
			WHERE 1=1
			".($arrWhere ? " and ".implode(" and ", $arrWhere) : "");
	
	$dados = $db->carregar($sql);
	if($dados):
		$conteudo .= "<markers> "; // inicia o XML
		foreach($dados as $d):				
			$ajuste = (($_position_ex[$d['lat']][$d['lng']])?($_position_ex[$d['lat']][$d['lng']]*0.0056):0);
			$conteudo .= "<marker "; //inicia um ponto no mapa
			$conteudo .= "mundescricao=\"". $d['mundescricao'] ."\" ";
			$conteudo .= "estuf=\"". $d['estuf'] ."\" ";
			$conteudo .= "nome=\"". $d['nome'] ."\" ";
			$conteudo .= "qtde=\"". number_format($d['qtde'],0,"",".")."\" ";
			$conteudo .= "lat=\"". $d['lat'] ."\" ";
			$conteudo .= "lng=\"". ($d['lng']+$ajuste) ."\" ";
			$conteudo .= "/> ";
			$_position_ex[$d['lat']][$d['lng']]++;
		endforeach;
		$conteudo .= "</markers> ";
		header ("Content-Type:text/xml");
		print $conteudo;
		unset($conteudo);
	endif;
}

function recuperaDetalhesCurso($curid)
{
	global $db;
	$sql = "SELECT
				c.curid,
				curstatus, 
				ateid, 
				curdesc, 
				ncuid, 
				curementa, 
				curfunteome,
				curobjetivo,
				curmetodologia,
				--modid, 
				curchmim, 
				curchmax, 
				curpercpremim,
				curpercpremax,
				curcertificado,
				curnumestudanteidealpre, 
				curnumestudanteminpre, 
				curnumestudantemaxpre,
				curnumestudanteidealdist, 
				curnumestudantemindist, 
				curnumestudantemaxdist,
				to_char(curinicio,'DD/MM/YYYY') as curinicio,
				to_char(curfim,'DD/MM/YYYY') as curfim,
				curofertanacional,
				curcustoaluno,
				curqtdmonitora,
				uteid,
				curinfra,
				usunome, 
				to_char(hicdata,'DD/MM/YYY - HH24:MI:SS') as hicdata,
				--co_interno_uorg,
				cursalamulti,
				lesid,
				ldeid,
				coordid
			FROM
				catalogocurso.curso c 
			LEFT JOIN catalogocurso.etapaensino e ON e.eteid = c.eteid
			LEFT JOIN catalogocurso.historicocurso h ON h.curid = c.curid
			LEFT JOIN seguranca.usuario u ON u.usucpf = h.usucpf
			WHERE
				c.curid = ".$curid;
	$curso = $db->pegaLinha($sql);
	
	$sql = "SELECT 
				e.pk_cod_etapa_ensino as codigo,
				e.no_etapa_ensino as descricao
			FROM 
				catalogocurso.etapaensino_curso ec
			INNER JOIN educacenso_2010.tab_etapa_ensino e ON e.pk_cod_etapa_ensino = ec.cod_etapa_ensino
			WHERE
				ec.curid = ".$curid;
	$curso['cod_etapa_ensino'] = $db->carregar($sql);
	
	$sql = "SELECT 
				r.redid as codigo
			FROM 
				catalogocurso.cursorede cr
			INNER JOIN catalogocurso.rede r ON r.redid = cr.redid
			WHERE
				curid = ".$curid;
	$curso['redid'] = $db->carregarColuna($sql);
	
	$sql = "SELECT 
				mod.modid as codigo
			FROM 
				catalogocurso.modalidadecurso_curso mcu
			INNER JOIN catalogocurso.modalidadecurso mod ON mod.modid = mcu.modid
			WHERE
				mcu.curid = ".$curid;
	$curso['modid'] = $db->carregarColuna($sql);
	
	return $curso;
}

function recuperaDadosPublicoAlvo($curid){
	
	global $db;
	
	if($curid){
		$sql = "SELECT 
					panesid as nesid, 
					paeteid as cod_etapa_ensino,  
					curpaoutrasexig, 
					curpatutor, 
					curpatutortxt, 
       				curpademsocial, 
       				pacod_escolaridade,
       				curpademsocialpercmax
				FROM 
					catalogocurso.curso
				WHERE
					curid = ".$curid;
		$retorno = $db->pegaLinha($sql);

		$sql = "SELECT 
					pk_cod_area_ocde as codigo, 
					no_nome_area_ocde as descricao
				FROM 
					educacenso_2010.tab_area_ocde t
				INNER JOIN catalogocurso.areaformacaocurso a ON a.cod_area_ocde = t.pk_cod_area_ocde
				WHERE
					a.curid = $curid";
		$area = $db->carregar($sql);
		
		$sql = "SELECT 
					pk_cod_disciplina as codigo, 
					no_disciplina as descricao
				FROM 
					educacenso_2010.tab_disciplina t
				INNER JOIN catalogocurso.diciplinacurso a ON a.cod_disciplina = t.pk_cod_disciplina
				WHERE
					a.curid = $curid";
		$disc = $db->carregar($sql);
		
		$sql = "SELECT 
					e.pk_cod_etapa_ensino as codigo,
					e.no_etapa_ensino as descricao
				FROM 
					catalogocurso.etapaensino_curso_publicoAlvo ec
				INNER JOIN educacenso_2010.tab_etapa_ensino e ON e.pk_cod_etapa_ensino = ec.cod_etapa_ensino
				WHERE
					ec.curid = ".$curid;
		$retorno['cod_etapa_ensino'] = $db->carregar($sql);
		
		$sql = "SELECT 
					e.fexid as codigo,
					e.fexdesc as descricao
				FROM 
					catalogocurso.funcaoexercida_curso_publicoAlvo ec
				INNER JOIN catalogocurso.funcaoexercida e ON e.fexid = ec.fexid
				WHERE
					ec.curid = ".$curid;
		$retorno['fexid'] = $db->carregar($sql);
		
		$retorno["cod_area_ocde"] = $area;
		$retorno["cod_disciplina"] = $disc;
		
		$sql = "SELECT
					foo.codigo,
					foo.descricao
				FROM
					((SELECT 
						to_char(pk_cod_escolaridade,'9') as codigo, 
						pk_cod_escolaridade||' - '||no_escolaridade as descricao,
						h.nivel
		  			FROM 
		  				educacenso_2010.tab_escolaridade e
		  			INNER JOIN catalogocurso.hierarquianivelescolaridade h ON h.nivid = e.pk_cod_escolaridade)
		  			UNION ALL
		  			(SELECT 
						to_char(pk_pos_graduacao,'9')||'0' as codigo, 
						pk_pos_graduacao||'0 - '||no_pos_graduacao as descricao,
						h.nivel
					FROM 
						educacenso_2010.tab_pos_graduacao e
		  			INNER JOIN catalogocurso.hierarquianivelescolaridade h ON h.nivid::integer = (e.pk_pos_graduacao||'0')::integer)) as foo
				INNER JOIN catalogocurso.publicoalvo_curso eeq ON eeq.cod_escolaridade = foo.codigo::integer AND eeq.curid = ".$curid;
		$retorno['cod_escolaridade'] = $db->carregar($sql);
		
		$sql = "SELECT
					tmod.pk_cod_mod_ensino
				FROM
					catalogocurso.tab_mod_ensino_curso mod
				INNER JOIN educacenso_2010.tab_mod_ensino tmod ON tmod.pk_cod_mod_ensino = mod.cod_mod_ensino
				WHERE
					curid = ".$curid;
		$retorno['cod_mod_ensino'] = $db->carregarColuna($sql);
		
		$sql = "SELECT
					pad.padid as codigo,
					pad.paddesc as descricao
				FROM
					catalogocurso.cursodemandasocial cms
				INNER JOIN catalogocurso.publicoalvodemandasocial pad ON pad.padid = cms.padid 
				WHERE
					curid = ".$curid;
		$retorno['padid'] = $db->carregar($sql);

		return $retorno;
	}
}

$curid = $_GET['curid'] ? $_GET['curid'] : $_SESSION['snf']['curid'];
if($curid){
	$_SESSION['snf']['curid'] = $curid;
}
if($_GET['ateid']){
	$_SESSION['snf']['ateid'] = $_GET['ateid'];
}
if($_GET['modid']){
	$_SESSION['snf']['modid'] = $_GET['modid'];
}
if($_GET['ncuid']){
	$_SESSION['snf']['ncuid'] = $_GET['ncuid'];
}
if($_GET['pfcid']){
	$_SESSION['snf']['pfcid'] = $_GET['pfcid'];
}
?>
<html>
	<head>
		<title>Detalhes do Curso</title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	</head>
	<body>
		<?php $dadosCurso = recuperaDetalhesCurso($curid); ?>
		<?php monta_titulo('Detalhes do Curso',"{$dadosCurso['curdesc']}({$dadosCurso['curid']})"); ?>
		<?php
			$arrAbas[] = array ("descricao" => "Dados Gerais",
								"id"		=> "1",
								"link"		=> "/snf/snf.php?modulo=principal/detalhesCurso&acao=A");
			$arrAbas[] = array ("descricao" => "Organização do Curso",
								"id"		=> "2",
								"link"		=> "/snf/snf.php?modulo=principal/detalhesCurso&acao=A&aba=organizacao_curso");
			$arrAbas[] = array ("descricao" => "Equipe",
								"id"		=> "3",
								"link"		=> "/snf/snf.php?modulo=principal/detalhesCurso&acao=A&aba=equipe");
			$arrAbas[] = array ("descricao" => "Público Alvo",
								"id"		=> "4",
								"link"		=> "/snf/snf.php?modulo=principal/detalhesCurso&acao=A&aba=publico_alvo");
			$arrAbas[] = array ("descricao" => "Mapa",
								"id"		=> "5",
								"link"		=> "/snf/snf.php?modulo=principal/detalhesCurso&acao=A&aba=mapa");
			$linkAtual = str_replace("&curid=$curid&ateid={$_GET['ateid']}&modid={$_GET['modid']}&ncuid={$_GET['ncuid']}&pfcid={$_GET['pfcid']}","",$_SERVER['REQUEST_URI']);
			echo "<br />";
			echo montarAbasArray($arrAbas, $linkAtual);
		?>
		<?php if(!$_GET['aba']): ?>
			<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
				<tr>
					<td class="SubTituloDireita" width="15%">
						Código / Área Temática
					</td>
					<td>
						<?=$dadosCurso['ateid']?> / <?=$db->pegaUm('SELECT atedesc FROM catalogocurso.areatematica WHERE ateid = '.$dadosCurso['ateid']); ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">
						Código / Nome do Curso
					</td>
					<td>
						<?=$dadosCurso['curid']?> / <?=$dadosCurso['curdesc'] ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">
						Etapa de ensino a que se<br> destina
					</td>
					<td>
						<?php 
							if( is_array( $dadosCurso['cod_etapa_ensino'] ) ){
								foreach($dadosCurso['cod_etapa_ensino'] as $dado){
									echo $dado['descricao']."<br>";
								}
							}
						?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">
						Nivel do Curso
					</td>
					<td>
						<?=$db->pegaUm('SELECT ncudesc FROM catalogocurso.nivelcurso WHERE ncuid ='.$dadosCurso['ncuid']); ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita" width="15%">
						Rede
					</td>
					<td>
						<?php 
							if( is_array($dadosCurso['redid']) ){
								$dados = $db->carregarColuna('SELECT reddesc FROM catalogocurso.rede WHERE redid in ('.implode(',',$dadosCurso['redid']).')'); 
		
								foreach($dados as $dado){
									echo $dado."<br>";
								}
							}
						?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">
						Objetivo
					</td>
					<td>
						<?=$dadosCurso['curobjetivo'] ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">
						Ementa
					</td>
					<td>
						<?=$dadosCurso['curementa'] ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">
						Fundamentos Teóricos Metodológicos
					</td>
					<td>
						<?=$dadosCurso['curfunteome'] ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">
						Título Concedido /<br> Certificação
					</td>
					<td>
						<?=$dadosCurso['curcertificado'] ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita" width="15%">
						Metodologia
					</td>
					<td>
						<?=$dadosCurso['curmetodologia'] ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">
						Modalidade de Ensino
					</td>
					<td>
						<?php 
							$dadosCurso['modid'] = is_array($dadosCurso['modid']) ? $dadosCurso['modid'] : Array();
							$modalidades = $db->carregar('SELECT modid as codigo, moddesc as descricao FROM catalogocurso.modalidadecurso WHERE modid in ('.implode(',',$dadosCurso['modid']).')'); 
							$modids = Array();
							foreach($modalidades as $k => $modalidade){
								echo "<br> - ".$modalidade['descricao'];
								$modids[$k] = $modalidade['codigo']; 
							}
							echo "<br>	";
						?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">
						Curso com Oferta Interestadual
					</td>
					<td>
						<?=$dadosCurso['curofertanacional']=='t'?'Sim':'Não' ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">
						Carga Horária
					</td>
					<td>
						<br>
						<b>Carga horária do curso:</b><br>&nbsp;&nbsp;
						Mínimo:&nbsp;
						<?=$dadosCurso['curchmim'] ?> Horas.
						Máximo:&nbsp;
						<?=$dadosCurso['curchmax'] ?> Horas.<br><br>
						<?php if(in_array(1,$modids)){?>
							<b>Carga horária presencial exigida(%):</b><br>&nbsp;&nbsp;
							Mínimo:&nbsp;
							<?=$dadosCurso['curpercpremim'] ?> %.
							Máximo:&nbsp;
							<?=$dadosCurso['curpercpremax'] ?> %.<br>
						<?php }?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">
						Número de estudantes por<br> turma
					</td>
					<td>
						<b>Presencial:</b><br>&nbsp;&nbsp;
						Ideal
						<?=$dadosCurso['curnumestudanteidealpre'] ?>.
						Mínimo
						<?=$dadosCurso['curnumestudanteminpre'] ?>.
						Máximo
						<?=$dadosCurso['curnumestudantemaxpre'] ?>.
						<br><br>
						<b>A Distância:</b><br>&nbsp;&nbsp;
						Ideal
						<?=$dadosCurso['curnumestudanteidealdist'] ?>.
						Mínimo
						<?=$dadosCurso['curnumestudantemindist'] ?>.
						Máximo
						<?=$dadosCurso['curnumestudantemaxdist'] ?>.
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">
						Periodicidade de Monitoramento
					</td>
					<td>
						A cada 
						<?=$dadosCurso['curqtdmonitora'] ?>
						<?=$db->pegaUm('SELECT utedesc FROM catalogocurso.unidadetempo WHERE uteid ='.$dadosCurso['uteid']); ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">
						Infraestrutura Recomendada
					</td>
					<td>
						<?=$dadosCurso['curinfra'] ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">
						Sala de Recursos Multifuncionais
					</td>
					<td>
						<?=$dadosCurso['cursalamulti']=='t' ? 'Sim' : 'Não' ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">
						Localização da Escola
					</td>
					<td>
						<?=$db->pegaUm('SELECT lesdesc FROM catalogocurso.localizacaoescola WHERE lesid ='.$dadosCurso['lesid']); ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">
						Localização Diferenciada da Escola
					</td>
					<td>
						<?=$db->pegaUm('SELECT ldedesc FROM catalogocurso.localizacaodiferenciadaescola WHERE ldeid ='.$dadosCurso['ldeid']); ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">
						Coordenação responsável no<br> MEC ou na CAPES
					</td>
					<td>
						<?=$db->pegaUm('SELECT coorddesc FROM catalogocurso.coordenacao WHERE coordid = '.$dadosCurso['coordid']); ?>
					</td>
				</tr>
			</table>
		<?php elseif($_GET['aba'] == "organizacao_curso"): ?>
			<?php 
				$sql = "SELECT 
							tiodesc, 
							orcdesc, 
							moddesc, 
							coalesce(orcchmim,0) as mim, 
						    coalesce(orcchmax,0) as max,
							coalesce(orcpercpremim,0)||' %' as permim, 
							coalesce(orcpercpremax,0)||' %' as permax, 
							--CASE WHEN LENGTH(orcementa) > 50
								--THEN SUBSTRING(orcementa FROM 0 FOR 50)||' ...'
								--ELSE 
								orcementa
							--END
						FROM 
							catalogocurso.organizacaocurso orc
						LEFT JOIN catalogocurso.tipoorganizacao tor ON tor.tioid = orc.tioid
						LEFT JOIN catalogocurso.modalidadecurso mod ON mod.modid = orc.modid
						WHERE
							orcstatus = 'A'
							AND orc.curid = ".$curid; 
				$cursos = $db->carregar($sql);
				$cabecalho = array("Tipo", "Nome", "Modalidade", "Hora Aula<br> (Mim.)", 
								   "Hora Aula<br> (Máx.)", "Carga Horária Presencial<br>Exigida % (Mim.)", "Carga Horária Presencial<br>Exigida % (max.)", "Descrição da Subdivisão");
				$db->monta_lista_array($cursos, $cabecalho, 50, 20, 'S', '100%', '',$arrayDeTiposParaOrdenacao);
			?>
		<?php elseif($_GET['aba'] == "equipe"): ?>
			<?php $cabecalho = array("Categoria de Membro<br> de Equipe", "Qtd Função", "Função", "Mínimo", "Máximo", 
					   "Unidade de Referência", "Nivel de Escolaridade", "Atribuição", "Outros Requisitos");
				  $sql = "SELECT 
							camdesc,
							qtdfuncao,
							eqcfuncao,
							eqcminimo, 
							eqcmaximo,
							unrdesc,
							eqcid as nesdesc,
							coalesce(replace(eqcatribuicao, chr(13)||chr(10), ''),'o') as artib, 
							coalesce(eqcoutrosreq,'o') as outr
						FROM 
							catalogocurso.equipecurso e
						LEFT JOIN catalogocurso.categoriamembro c ON c.camid = e.camid
						LEFT JOIN catalogocurso.unidadereferencia u ON u.unrid = e.unrid
						LEFT JOIN catalogocurso.nivelescolaridade n ON n.nesid = e.nesid
						WHERE
							eqcstatus = 'A'
							AND e.curid = ".$curid; 
				$cursos = $db->carregar($sql);
				if($cursos){
					for($i=0;$i<count($cursos);$i++){
						$eqcid = $cursos[$i]['nesdesc'];
						$sql = "(SELECT 
									pk_cod_escolaridade||' - '||no_escolaridade as descricao
					  			FROM 
					  				educacenso_2010.tab_escolaridade e
					  			INNER JOIN catalogocurso.hierarquianivelescolaridade h ON h.nivid = e.pk_cod_escolaridade
					  			WHERE pk_cod_escolaridade in (select cod_escolaridade from catalogocurso.escolaridade_equipe where eqcid = $eqcid)
					  			order by 1
					  			)
					  			UNION ALL
					  			(SELECT 
									pk_pos_graduacao||'0 - '||no_pos_graduacao as descricao
								FROM 
									educacenso_2010.tab_pos_graduacao e
					  			INNER JOIN catalogocurso.hierarquianivelescolaridade h ON h.nivid::integer = (e.pk_pos_graduacao||'0')::integer
					  			WHERE (e.pk_pos_graduacao||'0')::integer in (select cod_escolaridade from catalogocurso.escolaridade_equipe where eqcid = $eqcid)
					  			order by 1
					  			)";
						
						$nivel = $db->carregarColuna($sql);
						if($nivel){
							$cursos[$i]['nesdesc'] = '<div style="width:300px;">'.implode(';<br>',$nivel).'</div>';
						}
						else{
							$cursos[$i]['nesdesc'] = '';
						}
						
					}
				}
				$db->monta_lista_array($cursos, $cabecalho, 50, 20, '', '100%', '',$arrayDeTiposParaOrdenacao); 
			?>
		<?php elseif($_GET['aba'] == "publico_alvo"): ?>
			<?php $dadosPublico = recuperaDadosPublicoAlvo($curid); ?>
			<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
				<tr>
					<td class="SubTituloDireita" width="15%">
						Função Exercida&nbsp;
					</td>
					<td>
						<?php 
							if( is_array($dadosPublico['fexid']) ){
								foreach($dadosPublico['fexid'] as $dados){
									echo $dados['descricao']."<br>";
								}
							}
						?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">
						Nivel de escolaridade permitido&nbsp;
					</td>
					<td>
						<?php 
							if( is_array($dadosPublico['cod_escolaridade']) ){
								foreach($dadosPublico['cod_escolaridade'] as $dado){
									echo $dado['descricao']."<br>";
								}
							}
						?>
					</td>
				</tr>
				<tr id="tr_formacao" style="display<?=in_array($dadosPublico['pacod_escolaridade'],Array(6,7))?"table-row":"none" ?>">
					<td class="SubTituloDireita">
						Area de Formação&nbsp;
					</td>
					<td>	
						<?php 
							if( is_array($dadosPublico['cod_area_ocde']) ){
								foreach($dadosPublico['cod_area_ocde'] as $dado){
									echo $dado['descricao']."<br>";
								}
							}
						?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">
						Disciplina(s) que leciona&nbsp;
					</td>
					<td>
						<?php 
							if( is_array($dadosPublico['cod_disciplina']) ){
								foreach($dadosPublico['cod_disciplina'] as $dado){
									echo $dado['descricao']."<br>";
								}
							}
						?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">
						Etapa de Ensino em que Leciona&nbsp;
					</td>
					<td>
						<?php 
							if( is_array($dadosPublico['cod_etapa_ensino']) ){
								foreach($dadosPublico['cod_etapa_ensino'] as $dado){
									echo $dado['descricao']."<br>";
								}
							}
						?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">
						Modalidade em que leciona&nbsp;
					</td>
					<td>
						<?php 
							if( is_array($dadosPublico['cod_mod_ensino']) && count($dadosPublico['cod_mod_ensino']) ){
								$dados = $db->carregarColuna('SELECT no_mod_ensino FROM educacenso_2010.tab_mod_ensino WHERE pk_cod_mod_ensino in('.implode(',',$dadosPublico['cod_mod_ensino']).')');
								if( is_array($dados) ){
									foreach($dados as $dado){
										echo $dado."<br>";
									}
								}
							}
						?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">
						Outras Exigências&nbsp;
					</td>
					<td>
						<?=$dadosPublico['curpaoutrasexig'] ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">
						Curso disponivel para demanda social?&nbsp;
					</td>
					<td>
						<?=$dadosPublico['curpademsocial']=='t'?'Sim':'Não' ?>
					</td>
				</tr>
				<?php if( $dadosPublico['curpademsocial']=='t' ){?>
				<tr class="tr_demsoc">
					<td class="SubTituloDireita">
						Percentual máximo de participantes na demanda social&nbsp;
					</td>
					<td>
						<?=$dadosPublico['curpademsocialpercmax'] ?>
						%
						<img align="middle" border="0" src="../imagens/obrig.gif" style="margin-bottom:7px">
					</td>
				</tr>
				<tr class="tr_demsoc">
					<td class="SubTituloDireita">
						Público-alvo da demanda social&nbsp;
					</td>
					<td>
						<?php 
							if( is_array($dadosPublico['padid']) && count($dadosPublico['padid']) ){
								$dados = $dadosPublico['padid'];
								if( is_array($dados) ){
									foreach($dados as $dado){
										echo $dado['descricao']."<br>";
									}
								}
							}
						?>
					</td>
				</tr>
				<?php }?>
			</table>
		<?php elseif($_GET['aba'] == "mapa"): ?>
			<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
			<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
				<tr>
					<td>
						<input type="checkbox" checked="checked" name="chk_1" id="chk_demanda" onclick="marcarDemanda()" />Demanda<img src="/imagens/icones/tr.png" /><br/>
						<input type="checkbox" checked="checked" name="chk_2" id="chk_oferta" onclick="marcarOferta()" />Oferta<img style="margin-left:14px" src="/imagens/icones/tb.png" />
					</td>
				</tr>
				<tr>
					<td><div id="map_canvas" style="width:100%;height:600px;position:relative;text-align:center">Aguarde...Carregando.</div></td>
				</tr>
			</table>
			<link href="http://code.google.com/apis/maps/documentation/javascript/examples/default.css" rel="stylesheet" type="text/css" />
			<link href="/includes/maps/maps.css" rel="stylesheet" type="text/css" />
			<script type="text/javascript" src="http://maps.google.com/maps/api/js?sensor=false"></script>
			<script>
				//Variáveis globais
				var map;
				// Array de pontos dos icones
				var pontoMarcadores = new Array();
				// Informações dos pontos
				var infowindow;	
				
				/* Função para subustituir todos */
				function replaceAll(str, de, para){
				    var pos = str.indexOf(de);
				    while (pos > -1){
						str = str.replace(de, para);
						pos = str.indexOf(de);
					}
				    return (str);
				}

				/*
				 * Função de inicialização do google maps
				 */
				function initialize() {
					var myLatLng = new google.maps.LatLng(-14.689881, -52.373047);
				   	var center = myLatLng;
				   	var myOptions = {
				     zoom: 4,
				     center: myLatLng,
				     mapTypeId: google.maps.MapTypeId.ROADMAP
				   };
					map = new google.maps.Map(document.getElementById("map_canvas"),
				        myOptions);
				    // Instantiate an info window to hold step text.
				    stepDisplay = new google.maps.InfoWindow();
				    marcarPontos();
				    marcarPontosOferta();
				} //fim do initialize
				
				function marcarPontos() {
			        
					var filtros = "snf.php?modulo=principal/detalhesCurso&acao=A&requisicaoAjax=gerarXmlCursos";
					jQuery.ajax({
				  		type: "POST",
				  		url: filtros,
				  		async: false,
				  		success: function(data) {
				      	jQuery(data).find("marker").each(function() {
					        var marker = jQuery(this);
					        var latlng = new google.maps.LatLng(parseFloat(marker.attr("lat")),
					                                    		parseFloat(marker.attr("lng")));
					                					                   		
						    var ponto = new google.maps.Marker({
						        position: latlng,
						        map: map,
						        clickable: true,
						        icon: '/imagens/icones/tr.png',
						        zIndex: 3
						    });
						    
						    if(!pontoMarcadores['demanda']) {
						    	pontoMarcadores['demanda'] = new Array();
						    }
						    pontoMarcadores['demanda'].push(ponto);
						    
						   	var html = "<b>Município</b>: "+marker.attr("mundescricao")+"/"+marker.attr("estuf")+"<br/><b>Modalidade</b>: "+marker.attr("moddesc")+"<br/><b>Nível</b>: "+marker.attr("ncudesc")+"<br/><b>Demanda</b>: "+marker.attr("demanda");
						    google.maps.event.addListener(ponto, "click", function() {if (infowindow) infowindow.close(); infowindow = new google.maps.InfoWindow({content: html}); infowindow.open(map, ponto); });
					
					     });
				
				  		}
					});
				
				}// fim marcarPontos
				
				function marcarPontosOferta() {
			        
					var filtros = "snf.php?modulo=principal/detalhesCurso&acao=A&requisicaoAjax=gerarXmlOferta";
					jQuery.ajax({
				  		type: "POST",
				  		url: filtros,
				  		async: false,
				  		success: function(data) {
				      	jQuery(data).find("marker").each(function() {
					        var marker = jQuery(this);
					        var latlng = new google.maps.LatLng(parseFloat(marker.attr("lat")),
					                                    		parseFloat(marker.attr("lng")));
					                					                   		
						    var ponto = new google.maps.Marker({
						        position: latlng,
						        map: map,
						        clickable: true,
						        icon: '/imagens/icones/tb.png',
						        zIndex: 2
						    });
						    if(!pontoMarcadores['oferta']) {
						    	pontoMarcadores['oferta'] = new Array();
						    }
						    pontoMarcadores['oferta'].push(ponto);
						    
						   	var html = "<b>Município</b>: "+marker.attr("mundescricao")+"/"+marker.attr("estuf")+"<br/><b>Unidade de Oferta</b>: "+marker.attr("nome")+"<br/><b>Número de vagas</b>: "+marker.attr("qtde");
						    google.maps.event.addListener(ponto, "click", function() {if (infowindow) infowindow.close(); infowindow = new google.maps.InfoWindow({content: html}); infowindow.open(map, ponto); });
					
					     });
				
				  		}
					});
				
				}
				
				function marcarDemanda()
				{
					if(jQuery("#chk_demanda").attr("checked") == true){
						if(pontoMarcadores['demanda']){
							jQuery.each(pontoMarcadores['demanda'], function(cont,value) { 
							  	if(pontoMarcadores['demanda'][cont]){
							  		pontoMarcadores['demanda'][cont].setMap(map);
								}
							});
						}
					}else{
						if(pontoMarcadores['demanda']){
							jQuery.each(pontoMarcadores['demanda'], function(cont,value) { 
							  	if(pontoMarcadores['demanda'][cont]){
							  		pontoMarcadores['demanda'][cont].setMap(null);
								}
							});
						}
					}
				}
				
				function marcarOferta()
				{
					if(jQuery("#chk_oferta").attr("checked") == true){
						if(pontoMarcadores['oferta']){
							jQuery.each(pontoMarcadores['oferta'], function(cont,value) { 
							  	if(pontoMarcadores['oferta'][cont]){
							  		pontoMarcadores['oferta'][cont].setMap(map);
								}
							});
						}
					}else{
						if(pontoMarcadores['oferta']){
							jQuery.each(pontoMarcadores['oferta'], function(cont,value) { 
							  	if(pontoMarcadores['oferta'][cont]){
							  		pontoMarcadores['oferta'][cont].setMap(null);
								}
							});
						}
					}
				}
				
			</script>
			<script>initialize();</script>
		<?php endif; ?>
		<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
			<tr>
				<td class="SubTituloCentro" ><input type="button" value="Fechar" onclick="window.close()" /></td>
			</tr>
		</table>
	</body>
</html>