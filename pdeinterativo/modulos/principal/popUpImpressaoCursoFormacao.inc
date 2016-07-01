<?php 
$curid = $_REQUEST['curid'];
?>
<html>
	<head>
		<title>Catálogo de Curso</title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css"/>
	</head>
	<body>
		<?php 
		      
		if($_REQUEST['req']!=''){
			$_REQUEST['req']($_REQUEST);
		}
		
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
				INNER JOIN educacenso_".(ANO_CENSO).".tab_etapa_ensino e ON e.pk_cod_etapa_ensino = ec.cod_etapa_ensino
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
		
		$dadosCurso = $curso;
		?>
		<table align="center" width="95%" border="0" cellpadding="0" cellspacing="0" class="listagem2">
			<tr class="SubTituloEsquerda">
				<td style="FONT-SIZE: 12pt;">
					<center>
						Dados Gerais
					</center>
				</td>
			</tr>
		</table>
		<input type="hidden" value="" id="req" name="req"/>
		<input type="hidden" value="" id="link" name="link"/>
		<input type="hidden" value="<?=$curid ?>" id="curid" name="curid"/>
		<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
			<tr>
				<td class="SubTituloDireita" width="15%">
					Curso(Código)
				</td>
				<td>
					<?=$dadosCurso['curdesc'] ?>(<?=$dadosCurso['curid'] ?>)
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="15%">
					Status
				</td>
				<td>
					<?=$dadosCurso['curstatus']=='A'?'Ativo':'Inativo'; ?>
				</td>
			</tr>
		</table>
		<br>
		<table align="center" width="95%" border="0" cellpadding="5" cellspacing="1" class="listagem2">
			<tr class="FundoTitulo">
				<td>
					<b>PARTE 1</b>
				</td>
			</tr>
		</table>
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
				<td class="SubTituloDireita">
					Objetivo
				</td>
				<td>
					<?=$dadosCurso['curobjetivo'] ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">
					Descrição do Curso
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
		</table>
		<br>
		<table align="center" width="95%" border="0" cellpadding="5" cellspacing="1" class="listagem2">
			<tr class="FundoTitulo">
				<td>
					<b>PARTE 2</b>
				</td>
			</tr>
		</table>
		<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
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
		</table>
		<br>
		<table align="center" width="95%" border="0" cellpadding="0" cellspacing="0" class="listagem2">
			<tr class="SubTituloEsquerda">
				<td style="FONT-SIZE: 12pt;">
					<center>
						Organização do Curso
					</center>
				</td>
			</tr>
		</table>
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
						AND orc.curid = ".$_REQUEST['curid']; 
			$cursos = $db->carregar($sql);
			$cabecalho = array("Tipo", "Nome", "Modalidade", "Hora Aula<br> (Mim.)", 
							   "Hora Aula<br> (Máx.)", "Carga Horária Presencial<br>Exigida % (Mim.)", "Carga Horária Presencial<br>Exigida % (max.)", "Descrição da Subdivisão");
			$db->monta_lista_array($cursos, $cabecalho, 50, 20, 'S', '100%', '',$arrayDeTiposParaOrdenacao);
		?>
		<br>
		<table align="center" width="95%" border="0" cellpadding="0" cellspacing="0" class="listagem2">
			<tr class="SubTituloEsquerda">
				<td style="FONT-SIZE: 12pt;">
					<center>
						Público-Alvo
					</center>
				</td>
			</tr>
		</table>
		<?php 
		
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
		
			$sql = "SELECT DISTINCT
						pad.padid as codigo,
						pad.paddesc as descricao
					FROM
						catalogocurso.cursodemandasocial cms
					INNER JOIN catalogocurso.publicoalvodemandasocial pad ON pad.padid = cms.padid 
					WHERE
						curid = ".$curid;
			$retorno['padid'] = $db->carregar($sql);
			
			$dadosPublico = $retorno;
		?>
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
			<tr>
				<td colspan="2" align="center">
					<input type="button" value="Imprimir" onclick="window.print();"/>
				</td>
			</tr>
		</table>
		
	</body>
</html>