<?php
function pegaEsfera(){
	
	global $db;
	
	$sql = "SELECT
				upper(pdiesfera)
			FROM
				pdeinterativo2013.pdinterativo
			WHERE
				pdeid = ".$_SESSION['pdeinterativo2013_vars']['pdeid'];
	
	return $db->pegaUm($sql);
}

function recuperaDocentesDiretor( $pdeid = null ){
	
	global $db;
	
	$pdeid = !$pdeid ? $_SESSION['pdeinterativo2013_vars']['pdeid'] : $pdeid;
	
	$sql = "SELECT 
				usu.*,
				CASE WHEN ususexo = 'M'
					THEN 'Masculino'
					ELSE 'Feminino'
				END as genero
			FROM 
				seguranca.usuario usu
			INNER JOIN seguranca.perfilusuario 				 pfl ON usu.usucpf = pfl.usucpf
			INNER JOIN pdeinterativo2013.pessoa 		 		 pes ON pes.usucpf = usu.usucpf
			INNER JOIN pdeinterativo2013.usuarioresponsabilidade rpu ON usu.usucpf = rpu.usucpf AND pfl.pflcod = rpu.pflcod AND rpustatus = 'A'
			INNER JOIN pdeinterativo2013.pdinterativo 		     pde ON pde.pdeid  = pes.pdeid
			WHERE 
				pesstatus = 'A'
				AND
				pde.pdeid = '$pdeid'
				AND
				pes.pflcod = ".PDEESC_PERFIL_DIRETOR;
}

function possuiCurso( $cpf ){
	
	global $db;
	
	$sql = "SELECT
				true
			FROM
				2013.planoformacaodocente
			WHERE
				curid IS NOT NULL AND pfdcpf = '$cpf'";
	$teste = $db->pegaUm($sql);
	
	return $teste == 't' ? true : false;	
}

function recuperaDocentes(){
	
	global $db;

	$sql = "SELECT DISTINCT
				rd.pk_cod_docente
			FROM 
				pdeinterativo2013.respostadocente rd 
			INNER JOIN pdeinterativo2013.pdinterativo pde ON pde.pdeid = rd.pdeid
			WHERE 
				pde.pdicodinep = '".$_SESSION['pdeinterativo2013_vars']['pdicodinep']."' AND rdoformapro = true AND rdovinculo in ('E','T')";
	//ver($_SESSION, $sql);
	$docentes = $db->carregarColuna($sql);
	
	if(count($docentes)>0){
		$db->executar('DELETE FROM pdeinterativo2013.planoformacaodocente WHERE pdeid = '.$_SESSION['pdeinterativo2013_vars']['pdeid'].' AND pk_cod_docente not in ('.implode(',',$docentes).')');
		$db->commit();
		$insere = '';
		$sql = "SELECT DISTINCT
					pk_cod_docente,
					num_cpf
				FROM 
					educacenso_2010.tab_docente 
				WHERE 
					pk_cod_docente in (".(implode(',', $docentes)).") ";
		//ver($sql);
		$docentes = $db->carregar($sql);
		foreach( $docentes as $docente ){
			
			$tes = "SELECT true FROM pdeinterativo2013.planoformacaodocente WHERE pk_cod_docente = '".$docente['pk_cod_docente']."' AND pdeid = ".$_SESSION['pdeinterativo2013_vars']['pdeid']."";
			if(!$db->pegaUm($tes)){
				$insere = "INSERT INTO pdeinterativo2013.planoformacaodocente(pdeid, pk_cod_docente, pfdcpf) VALUES(".$_SESSION['pdeinterativo2013_vars']['pdeid'].",".$docente['pk_cod_docente'].",'".$docente['num_cpf']."');";
				$db->executar($insere);
				$db->commit();
			}
		}

	}
	
	/*$sql = "SELECT DISTINCT
				pf.pfdid, 
				d.no_docente,
				pf.pcfid,
				pf.modid,
				cr.curid,
				coalesce(atedesc,'N/A') as atedesc,
				coalesce(curdesc,'N/A') as curdesc,
				coalesce(pcfdesc,'N/A') as pcfdesc,
				coalesce(ncudesc,'N/A') as ncudesc,
				coalesce(moddesc,'N/A') as moddesc,
				coalesce(curchmim,0)||'/'||coalesce(curchmax,0) as curch,
				coalesce(curpercpremim,0)||'/'||coalesce(curpercpremax,0) as curpercpre,
				CASE 
					WHEN pfdhabilitado	THEN '<img border=\"0\" align=\"top\" src=\"../imagens/check_p.gif\">'
					WHEN pfdhabilitado is null THEN ' - '
					ELSE '<img border=\"0\" align=\"top\" src=\"../imagens/exclui_p.gif\">'
				END as pfdhabilitado,
				pfdnhainteresse,
				pfdnhacurso,
				pfdcpf
			FROM 
				educacenso_".ANO_CENSO.".tab_docente d 
			INNER JOIN pdeinterativo2013.planoformacaodocente pf ON pf.pk_cod_docente = d.pk_cod_docente
			INNER JOIN educacenso_".ANO_CENSO.".tab_dado_docencia  dc ON dc.fk_cod_docente = d.pk_cod_docente AND id_tipo_docente = 1
			LEFT  JOIN catalogocurso.curso                cr ON cr.curid		  = pf.curid
			LEFT  JOIN catalogocurso.areatematica         at ON at.ateid		  = cr.ateid
			LEFT  JOIN catalogocurso.nivelcurso			  nc ON nc.ncuid		  = cr.ncuid
			LEFT  JOIN pdeinterativo2013.periodocursoformacao pe ON pe.pcfid		  = pf.pcfid
			LEFT  JOIN catalogocurso.modalidadecurso	  mo ON mo.modid		  = pf.modid
			WHERE 
				pdeid = ".$_SESSION['pdeinterativo2013_vars']['pdeid']."
			ORDER BY 
				2";*/
//	ver($sql);
	$sql = "
		SELECT DISTINCT pfd.pfdid,
		       tdo.no_docente,
		       pfd.pcfid,
		       pfd.modid,
		       cur.curid,
		       coalesce(ate.atedesc,'N/A') as atedesc,
		       coalesce(cur.curdesc,'N/A') as curdesc,
		       coalesce(pcf.pcfdesc,'N/A') as pcfdesc,
		       coalesce(ncu.ncudesc,'N/A') as ncudesc,
		       coalesce(mod.moddesc,'N/A') as moddesc,
		       coalesce(cur.curchmim,0)||'/'||coalesce(cur.curchmax,0) as curch,
		       coalesce(cur.curpercpremim,0)||'/'||coalesce(cur.curpercpremax,0) as curpercpre,
		       CASE
		           WHEN pfd.pfdhabilitado         THEN '<img border=\"0\" align=\"top\" src=\"../imagens/check_p.gif\">'
		           WHEN pfd.pfdhabilitado is null THEN ' - '
		           ELSE                                '<img border=\"0\" align=\"top\" src=\"../imagens/exclui_p.gif\">'
		       END as pfdhabilitado,
		       pfd.pfdnhainteresse,
		       pfd.pfdnhacurso,
		       pfd.pfdcpf,
		       CASE
		           WHEN snf.esddsc is not null THEN snf.esddsc || ' no SNF'
		           ELSE                             esd.esddsc || ' no PDE'
		       END as situacao
		FROM educacenso_2011.tab_docente tdo 
			INNER JOIN pdeinterativo2013.planoformacaodocente pfd ON pfd.pk_cod_docente  = tdo.pk_cod_docente
			LEFT JOIN educacenso_2011.tab_dado_docencia  tdd ON tdd.fk_cod_docente  = tdo.pk_cod_docente AND tdd.id_tipo_docente = 1
			LEFT  JOIN catalogocurso.curso                cur ON cur.curid           = pfd.curid
			LEFT  JOIN catalogocurso.areatematica         ate ON ate.ateid           = cur.ateid
			LEFT  JOIN catalogocurso.nivelcurso           ncu ON ncu.ncuid           = cur.ncuid
			LEFT  JOIN pdeinterativo2013.periodocursoformacao pcf ON pcf.pcfid           = pfd.pcfid
			LEFT  JOIN catalogocurso.modalidadecurso      mod ON mod.modid           = pfd.modid
			LEFT  JOIN pdeinterativo2013.pdinterativo         pde ON pde.pdeid           = pfd.pdeid
			LEFT  JOIN workflow.documento                 doc ON doc.docid           = pde.formacaodocid
			LEFT  JOIN workflow.estadodocumento           esd ON esd.esdid           = doc.esdid
			LEFT  JOIN (SELECT * FROM 
				dblink( 'host= user= password= port=5432 dbname=',
					'SELECT pri.pdicodinep,
						esd.esddsc
					 FROM snf.prioridadecursoescola pri  
						LEFT JOIN snf.prioridadedocumento  prd ON prd.prdid = pri.prdid
						LEFT JOIN workflow.documento       doc ON doc.docid = prd.docid
						LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid')
				AS resultado(pdicodinep varchar, esddsc varchar)) snf ON snf.pdicodinep = pde.pdicodinep
		WHERE pfd.pdeid = ".$_SESSION['pdeinterativo2013_vars']['pdeid']."
		ORDER BY 2
	";
	//ver($sql);
	return $db->carregar($sql);
}

function recuperaAuxiliares(){
	
	global $db;

	/*$sql = "SELECT DISTINCT
				pf.pfdid, 
				CASE 
					WHEN id_tipo_docente = 2 THEN 'Auxiliar de Educação Infantil '
					WHEN id_tipo_docente = 3 THEN 'Monitor de Atividade Complementar '
					WHEN id_tipo_docente = 4 THEN 'Interprete de Libras'
				END as tipo_docente,
				d.no_docente,
				pf.pcfid,
				pf.modid,
				cr.curid,
				coalesce(atedesc,'N/A') as atedesc,
				coalesce(curdesc,'N/A') as curdesc,
				coalesce(pcfdesc,'N/A') as pcfdesc,
				coalesce(ncudesc,'N/A') as ncudesc,
				coalesce(moddesc,'N/A') as moddesc,
				coalesce(curchmim,0)||'/'||coalesce(curchmax,0) as curch,
				coalesce(curpercpremim,0)||'/'||coalesce(curpercpremax,0) as curpercpre,
				CASE 
					WHEN pfdhabilitado	THEN '<img border=\"0\" align=\"top\" src=\"../imagens/check_p.gif\">'
					WHEN pfdhabilitado is null THEN ' - '
					ELSE '<img border=\"0\" align=\"top\" src=\"../imagens/exclui_p.gif\">'
				END as pfdhabilitado,
				pfdnhainteresse,
				pfdnhacurso,
				pfdcpf
			FROM 
				educacenso_".ANO_CENSO.".tab_docente d 
			INNER JOIN pdeinterativo2013.planoformacaodocente pf ON pf.pk_cod_docente = d.pk_cod_docente
			INNER JOIN educacenso_".ANO_CENSO.".tab_dado_docencia  dc ON dc.fk_cod_docente = d.pk_cod_docente AND id_tipo_docente in (2,3,4)
			LEFT  JOIN catalogocurso.curso                cr ON cr.curid		  = pf.curid
			LEFT  JOIN catalogocurso.areatematica         at ON at.ateid		  = cr.ateid
			LEFT  JOIN catalogocurso.nivelcurso			  nc ON nc.ncuid		  = cr.ncuid
			LEFT  JOIN pdeinterativo2013.periodocursoformacao pe ON pe.pcfid		  = pf.pcfid
			LEFT  JOIN catalogocurso.modalidadecurso	  mo ON mo.modid		  = pf.modid
			WHERE 
				pdeid = ".$_SESSION['pdeinterativo2013_vars']['pdeid']."
			ORDER BY 
				3";*/
//	ver($sql);
	$sql = "
		SELECT DISTINCT pfd.pfdid,
		       CASE
		           WHEN tdd.id_tipo_docente = 2 THEN 'Auxiliar de Educação Infantil '
		           WHEN tdd.id_tipo_docente = 3 THEN 'Monitor de Atividade Complementar '
		           WHEN tdd.id_tipo_docente = 4 THEN 'Interprete de Libras'
		       END as tipo_docente,
		       tdo.no_docente,
		       pfd.pcfid,
		       pfd.modid,
		       cur.curid,
		       coalesce(ate.atedesc,'N/A') as atedesc,
		       coalesce(cur.curdesc,'N/A') as curdesc,
		       coalesce(pcf.pcfdesc,'N/A') as pcfdesc,
		       coalesce(ncu.ncudesc,'N/A') as ncudesc,
		       coalesce(mod.moddesc,'N/A') as moddesc,
		       coalesce(cur.curchmim,0)||'/'||coalesce(cur.curchmax,0) as curch,
		       coalesce(cur.curpercpremim,0)||'/'||coalesce(cur.curpercpremax,0) as curpercpre,
		       CASE
		           WHEN pfd.pfdhabilitado         THEN '<img border=\"0\" align=\"top\" src=\"../imagens/check_p.gif\">'
		           WHEN pfd.pfdhabilitado is null THEN ' - '
		           ELSE                                '<img border=\"0\" align=\"top\" src=\"../imagens/exclui_p.gif\">'
		       END as pfdhabilitado,
		       pfd.pfdnhainteresse,
		       pfd.pfdnhacurso,
		       pfd.pfdcpf,
		       CASE
		           WHEN snf.esddsc is not null THEN snf.esddsc || ' no SNF'
		           ELSE                             esd.esddsc || ' no PDE'
		       END as situacao
		FROM educacenso_2011.tab_docente tdo
			INNER JOIN pdeinterativo2013.planoformacaodocente pfd ON pfd.pk_cod_docente  = tdo.pk_cod_docente
			INNER JOIN educacenso_2011.tab_dado_docencia  tdd ON tdd.fk_cod_docente  = tdo.pk_cod_docente
			AND                                                  tdd.id_tipo_docente in (2,3,4)
			LEFT  JOIN catalogocurso.curso                cur ON cur.curid           = pfd.curid
			LEFT  JOIN catalogocurso.areatematica         ate ON ate.ateid           = cur.ateid
			LEFT  JOIN catalogocurso.nivelcurso           ncu ON ncu.ncuid           = cur.ncuid
			LEFT  JOIN pdeinterativo2013.periodocursoformacao pcf ON pcf.pcfid           = pfd.pcfid
			LEFT  JOIN catalogocurso.modalidadecurso      mod ON mod.modid           = pfd.modid
			LEFT  JOIN pdeinterativo2013.pdinterativo         pde ON pde.pdeid           = pfd.pdeid
			LEFT  JOIN workflow.documento                 doc ON doc.docid           = pde.formacaodocid
			LEFT  JOIN workflow.estadodocumento           esd ON esd.esdid           = doc.esdid
			LEFT  JOIN (SELECT * FROM 
				dblink( 'host= user= password= port=5432 dbname=',
					'SELECT pri.pdicodinep,
						esd.esddsc
					 FROM snf.prioridadecursoescola pri  
						LEFT JOIN snf.prioridadedocumento  prd ON prd.prdid = pri.prdid
						LEFT JOIN workflow.documento       doc ON doc.docid = prd.docid
						LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid')
				AS resultado(pdicodinep varchar, esddsc varchar)) snf ON snf.pdicodinep = pde.pdicodinep
		WHERE pfd.pdeid = ".$_SESSION['pdeinterativo2013_vars']['pdeid']."
		ORDER BY 3
	";
	return $db->carregar($sql);
}

function carregaCursos(){
	
	global $db;
	
	if( $_REQUEST['pfdid'] ){
		
		$where = Array();
		
		if( $db->pegaUm('SELECT true FROM pdeinterativo2013.planoformacaodocente WHERE pk_cod_docente IS NOT NULL AND pfdid = '.$_REQUEST['pfdid']) ){
			
			//Etapa Ensino
			$sql = "SELECT DISTINCT
						t.fk_cod_etapa_ensino
					FROM 
						educacenso_2010.tab_docente d 
					INNER JOIN pdeinterativo2013.planoformacaodocente pf ON pf.pk_cod_docente = d.pk_cod_docente
					INNER JOIN educacenso_2010.tab_docente_disc_turma ddt ON ddt.fk_cod_docente = d.pk_cod_docente
					INNER JOIN educacenso_2010.tab_turma t ON t.pk_cod_turma = ddt.fk_cod_turma
					WHERE 
						pf.pdeid = ".$_SESSION['pdeinterativo2013_vars']['pdeid']."
						AND pf.pfdid = ".$_REQUEST['pfdid'];
			$docenteEtapaEnsino = $db->carregarColuna($sql);
			
			if( $docenteEtapaEnsino[0] != '' ){
				$where[] = 'eta.cod_etapa_ensino in (\''.implode('\',\'',$docenteEtapaEnsino).'\',\'999\')';
			}else{
				$where[] = 'eta.cod_etapa_ensino in (\'999\')';
			}
			
			// Escolaridade
			$sql = "SELECT DISTINCT
						d.fk_cod_escolaridade
					FROM 
						educacenso_".ANO_CENSO.".tab_docente d 
					INNER JOIN pdeinterativo2013.planoformacaodocente     pf ON pf.pk_cod_docente = d.pk_cod_docente
					WHERE 
						pdeid = ".$_SESSION['pdeinterativo2013_vars']['pdeid']."
						AND pfdid = ".$_REQUEST['pfdid'];
			$docenteEscolaridade = $db->pegaUm($sql);
			
			if( $docenteEscolaridade != '' ){
				$where[] = 'peq.cod_escolaridade <= '.$docenteEscolaridade;
			}
			
			//Area de Formação
			$sql = "SELECT DISTINCT
						bf.fk_cod_area_ocde
					FROM 
						educacenso_".ANO_CENSO.".tab_docente d 
					INNER JOIN pdeinterativo2013.planoformacaodocente     pf ON pf.pk_cod_docente = d.pk_cod_docente
					LEFT  JOIN educacenso_".ANO_CENSO.".tab_docente_form_sup   bf ON bf.fk_cod_docente = d.pk_cod_docente
					WHERE 
						pdeid = ".$_SESSION['pdeinterativo2013_vars']['pdeid']."
						AND pfdid = ".$_REQUEST['pfdid']."
					ORDER BY
						1";
			
			$docenteArea = $db->carregarColuna($sql);
			
			if( $docenteEscolaridade > 5 && $docenteArea[0] != '' ){
				$where[] = 'afo.cod_area_ocde in (\''.implode('\',\'',$docenteArea).'\',\'999\')';
			}else{
				$where[] = 'afo.cod_area_ocde in (\'999\')';
			}
			
			// Disciplina que leciona
			$sql = "SELECT DISTINCT
						dt.fk_cod_disciplina
					FROM 
						educacenso_".ANO_CENSO.".tab_docente d 
					INNER JOIN pdeinterativo2013.planoformacaodocente     pf ON pf.pk_cod_docente = d.pk_cod_docente
					LEFT  JOIN educacenso_".ANO_CENSO.".tab_docente_disc_turma dt ON dt.fk_cod_docente = d.pk_cod_docente
					WHERE 
						pdeid = ".$_SESSION['pdeinterativo2013_vars']['pdeid']."
						AND pfdid = ".$_REQUEST['pfdid']."
					ORDER BY
						1";
			
			$docenteDisciplina = $db->carregarColuna($sql);
			
			if( $docenteDisciplina[0] != '' ){
				$where[] = 'dcu.cod_disciplina in ('.implode(',',$docenteDisciplina).',999)';
			}else{
				$where[] = 'dcu.cod_disciplina in (999)';
			}
					
			// Função Docente
			$sql = "SELECT DISTINCT
						dc.id_tipo_docente -- Função
					FROM 
						educacenso_".ANO_CENSO.".tab_docente d 
					INNER JOIN pdeinterativo2013.planoformacaodocente     pf ON pf.pk_cod_docente = d.pk_cod_docente
					LEFT  JOIN educacenso_".ANO_CENSO.".tab_dado_docencia      dc ON dc.fk_cod_docente = d.pk_cod_docente
					WHERE 
						pdeid = ".$_SESSION['pdeinterativo2013_vars']['pdeid']."
						AND pfdid = ".$_REQUEST['pfdid']."
					ORDER BY
						1";
			
			
			$docenteFuncao = $db->carregarColuna($sql);
			
			if( $docenteFuncao[0] != '' ){
				$where[] = 'fep.fexid in ('.implode(',',$docenteFuncao).',999)';
			}else{
				$where[] = 'fep.fexid in (999)';
			}
		
		}else{
			
			//Função Diretor
			$sql = "SELECT DISTINCT
						CASE 
							WHEN tp.tpeid in (2, 7) THEN 5
							WHEN tp.tpeid in (8, 9, 11) THEN 6
						END as fexid
					FROM
						pdeinterativo2013.planoformacaodocente pfd 
					INNER JOIN pdeinterativo2013.pessoatipoperfil      pt ON pt.pesid  = pfd.pesid
					INNER JOIN pdeinterativo2013.tipoperfil            tp ON tp.tpeid  = pt.tpeid AND tp.tpestatus = 'A'
					WHERE
						pfd.pdeid = ".$_SESSION['pdeinterativo2013_vars']['pdeid']."
						AND pfdid = ".$_REQUEST['pfdid']."
					ORDER BY
						1";
			
			$funcao = $db->pegaUm($sql);
			
			if( $funcao != '' ){
				$where[] = 'fep.fexid in ('.$funcao.',999)';
			}else{
				$where[] = 'fep.fexid in (999)';
			}
			
		}
		
		//Localização
		$sql = "SELECT DISTINCT
					pdilocalizacao, --Localização
					id_localizacao_diferenciada, --Localização diferenciada
					pdiesfera --Esfera
				FROM
					pdeinterativo2013.pdinterativo pde
				LEFT JOIN educacenso_".ANO_CENSO.".tab_entidade    ent ON ent.cod_orgao_regional_inep = pde.pdicodinep
				LEFT JOIN educacenso_".ANO_CENSO.".tab_dado_escola des ON des.fk_cod_entidade = ent.pk_cod_entidade
				WHERE
					pdeid = ".$_SESSION['pdeinterativo2013_vars']['pdeid']."
				ORDER BY
					1";
		
		$escola = $db->pegaLinha($sql);
		
		if( $escola['pdilocalizacao'] == 'Rural' ){
			$where[] = 'lesid in (1,3)';
		}elseif( $escola['pdilocalizacao'] == 'Urbana' ){
			$where[] = 'lesid in (2,3)';
		}
		
		if( $escola['id_localizacao_diferenciada'] != '' ){
			$where[] = 'ldeid in '.$escola['id_localizacao_diferenciada']+1;
		}
		
		if( $escola['pdiesfera'] != '' ){
			$where[] = 'cre.redid in (1,'.($escola['pdiesfera']+1).')';
		}else{
			$where[] = 'cre.redid in (1)';
		}
		
		// Modalidade
		$sql = "SELECT DISTINCT
					fk_cod_mod_ensino
				FROM
					pdeinterativo2013.pdinterativo pde
				INNER JOIN educacenso_".ANO_CENSO.".tab_turma turma ON turma.fk_cod_entidade = pde.pdicodinep::bigint
				WHERE 
					fk_cod_mod_ensino IS NOT NULL
					AND pdeid = ".$_SESSION['pdeinterativo2013_vars']['pdeid']."
				ORDER BY
					1";
		
		$modalidade = $db->carregarColuna($sql);
		
		if( $modalidade[0] != '' ){
			$where[] = 'mod.cod_mod_ensino in ('.implode(',',$modalidade).')';
		}else{
			$where[] = 'mod.cod_mod_ensino is null';
		}
		
		// Alteração requerida por Wallace 11/06/2012 as 14:15 Comentar regra abaixo
		// Recursos multi-funcionais
//		$sql = "SELECT DISTINCT 
//					true
//				FROM 
//					painel.indicador i
//				INNER JOIN painel.seriehistorica 		 sh ON sh.indid=i.indid
//				INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
//				INNER JOIN pdeinterativo2013.pdinterativo   pde ON pde.pdicodinep = dsh.dshcod
//				WHERE 
//					i.indid = 268
//					AND pde.pdeid = ".$_SESSION['pdeinterativo2013_vars']['pdeid']."
//					AND (sehvalor IS NOT NULL OR sehqtde IS NOT NULL )
//					AND sehstatus != 'I'
//				ORDER BY
//					1";
//		$indicador = $db->pegaUm($sql);
//		
//		if( $indicador == '' ){
//			$where[] = '( cursalamulti IS NOT TRUE OR cursalamulti IS NULL)';
//		}
		//Fim daalteração Alteração requerida por Wallace 11/06/2012 as 14:15 Comentar regra acima
	}
	
	$sql = "SELECT DISTINCT
				cr.curid,
				coalesce(atedesc,'N/A') as atedesc,
				coalesce(curdesc,'N/A') as curdesc,
				coalesce(ncudesc,'N/A') as ncudesc,
				coalesce(curchmim,0)||'/'||coalesce(curchmax,0) as curch,
				coalesce(curpercpremim,0)||'/'||coalesce(curpercpremax,0) as curpercpre
			FROM
				catalogocurso.curso cr
			INNER JOIN catalogocurso.areatematica 	   				  at  ON at.ateid  = cr.ateid
			INNER JOIN catalogocurso.cursorede	  	   				  cre ON cre.curid  = cr.curid
			INNER JOIN catalogocurso.nivelcurso	  	   				  nc  ON nc.ncuid  = cr.ncuid
			INNER JOIN workflow.documento		  	   				  doc ON doc.docid = cr.docid AND doc.esdid = 403
			INNER JOIN catalogocurso.publicoalvo_curso 				  peq ON peq.curid = cr.curid
			INNER JOIN catalogocurso.areaformacaocurso 				  afo ON afo.curid = cr.curid
			INNER JOIN catalogocurso.diciplinacurso    				  dcu ON dcu.curid = cr.curid
			INNER JOIN catalogocurso.funcaoexercida_curso_publicoalvo fep ON fep.curid = cr.curid
			INNER JOIN catalogocurso.tab_mod_ensino_curso			  mod ON mod.curid = cr.curid
			INNER JOIN catalogocurso.etapaensino_curso_publicoAlvo	  eta ON eta.curid = cr.curid
			WHERE
				--to_char(curinicio,'YYYY') <= to_char(now(),'YYYY') 
				--AND
				(to_char(curfim,'YYYY') >= to_char(now(),'YYYY') OR curfim is null )
				AND 
				curstatus = 'A'
				".(count($where) > 0 ? ' AND '.implode(' AND ', $where) : '' )."
			ORDER BY
				2,3";
//	ver($sql,d);
	return $db->carregar($sql);
}

function carregaCurso( $pfdid ){
	
	global $db;
	
	$sql = "SELECT
				curid,
				pcfid,
				modid,
				pfdemail,
				pfdtel,
				pfdcel,
				pfdnhainteresse,
				coalesce(d.no_docente, pes.pesnome) as no_docente
			FROM
				pdeinterativo2013.planoformacaodocente pfd
			INNER JOIN educacenso_".ANO_CENSO.".tab_docente d ON d.num_cpf = pfd.pfdcpf
			LEFT  JOIN pdeinterativo2013.pessoa pes ON pes.pesid = pfd.pesid
			WHERE
				pfdid = $pfdid
				AND pfd.pdeid = ".$_SESSION['pdeinterativo2013_vars']['pdeid'];
	return $db->pegaLinha($sql);
}

function verificaCurso( $pfdid ){
	
	global $db;
	
	$pfdcpf = $db->pegaUm('SELECT pfdcpf FROM pdeinterativo2013.planoformacaodocente WHERE pfdid = '.$_REQUEST['pfdid']); 
	
//	if( $pk_docente ){
		
		$sql = "SELECT
					pde.pdenome as escola,
					mundescricao||' - '||pde.estuf as municipio,
					pdinumtelefone as tel,
					pdeemail as email,
					curdesc as curso
				FROM
					pdeinterativo2013.planoformacaodocente pfd
				INNER JOIN pdeinterativo2013.pdinterativo pde ON pde.pdeid  = pfd.pdeid
				INNER JOIN territorios.municipio      mun ON mun.muncod = pde.muncod
				INNER JOIN catalogocurso.curso 		  cur ON cur.curid  = pfd.curid
				WHERE
					pfdcpf = '$pfdcpf'
					AND pfdid <> ".$_REQUEST['pfdid'];
		if( $_SESSION['usucpf'] == '' ){
			ver($sql);
		}
		
//	}else{
//		
//		$pesid = $db->pegaUm('SELECT pesid FROM pdeinterativo2013.planoformacaodocente WHERE pesid IS NOT NULL AND pfdid = '.$_REQUEST['pfdid']);
//		
//		$sql = "SELECT
//					pde.pdenome as escola,
//					mundescricao||' - '||pde.estuf as municipio,
//					pdinumtelefone as tel,
//					pdeemail as email,
//					curdesc as curso
//				FROM
//					pdeinterativo2013.planoformacaodocente pfd
//				INNER JOIN pdeinterativo2013.pdinterativo pde ON pde.pdeid  = pfd.pdeid
//				INNER JOIN territorios.municipio      mun ON mun.muncod = pde.muncod
//				INNER JOIN catalogocurso.curso 		  cur ON cur.curid  = pfd.curid
//				WHERE
//					pfd.pesid = $pesid
//					AND pfd.pdeid != ".$_SESSION['pdeinterativo2013_vars']['pdeid'];
//		
//	}
	
	return $db->pegaLinha($sql);
}

function recuperaDiretoriaEquipe(){
	
	global $db;

	$sql = "SELECT DISTINCT
			    pe.pesid
			FROM
				pdeinterativo2013.pessoa pe
			    LEFT  JOIN seguranca.usuario usu ON usu.usucpf = pe.usucpf
			    INNER JOIN pdeinterativo2013.pessoatipoperfil ptp on ptp.pesid = pe.pesid
			WHERE
				ptp.pdeid = '".$_SESSION['pdeinterativo2013_vars']['pdeid']."'
			    AND pe.pesstatus = 'A'
			    AND (ptp.tpeid in (2,7,8,9,10,11,14) OR ptp.tpeid in (2,7,8,9,10,11,14))";
	$arDiretoria = $db->carregarColuna($sql);
	
	if(count($arDiretoria)>0){
		$db->executar('DELETE FROM pdeinterativo2013.planoformacaodocente WHERE pdeid = '.$_SESSION['pdeinterativo2013_vars']['pdeid'].' AND pk_cod_docente IS NULL and pesid not in ('.implode(',',$arDiretoria).')');
		$db->commit();
		$insere = '';
		$sql = "SELECT DISTINCT
				    pe.pesid,
				    pe.usucpf
				FROM
					pdeinterativo2013.pessoa pe
				    LEFT JOIN seguranca.usuario usu ON usu.usucpf = pe.usucpf
				    INNER JOIN pdeinterativo2013.pessoatipoperfil ptp on ptp.pesid = pe.pesid
				WHERE
					ptp.pdeid = '".$_SESSION['pdeinterativo2013_vars']['pdeid']."'
				    AND pe.pesstatus = 'A'
				    AND (ptp.tpeid in (2,7,8,9,10,11,14) OR ptp.tpeid in (2,7,8,9,10,11,14))";
		
		$arDiretoria = $db->carregar($sql);
		if( is_array($arDiretoria) ){
			foreach( $arDiretoria as $diretoria ){
				
				$tes = "SELECT true FROM pdeinterativo2013.planoformacaodocente WHERE pesid = '".$diretoria['pesid']."' AND pdeid = '".$_SESSION['pdeinterativo2013_vars']['pdeid']."'";
				if(!$db->pegaUm($tes)){
					$insere = "INSERT INTO pdeinterativo2013.planoformacaodocente(pdeid, pesid, pfdcpf) VALUES(".$_SESSION['pdeinterativo2013_vars']['pdeid'].",".$diretoria['pesid'].",'".$diretoria['usucpf']."');";
					$db->executar($insere);
					$db->commit();
				}
			}
		}
	}
	
	/*$sql = "SELECT DISTINCT
				case when ps.pflcod is not null then us.usunome else ps.pesnome end as nome,
                tp.tpedesc,
                tp.tpeid,
				pf.pfdid, 
				pf.pcfid,
				pf.modid,
				cr.curid,
				coalesce(atedesc,'N/A') as atedesc,
				coalesce(curdesc,'N/A') as curdesc,
				coalesce(pcfdesc,'N/A') as pcfdesc,
				coalesce(ncudesc,'N/A') as ncudesc,
				coalesce(moddesc,'N/A') as moddesc,
				coalesce(curchmim,0)||'/'||coalesce(curchmax,0) as curch,
				coalesce(curpercpremim,0)||'/'||coalesce(curpercpremax,0) as curpercpre,
				CASE 
					WHEN pfdhabilitado	THEN 'check_p.gif'
					WHEN pfdhabilitado is null THEN ' - '
					ELSE 'exclui_p.gif'
				END as pfdhabilitado,
				pfdnhainteresse,
				pfdnhacurso,
				pfdcpf
			FROM 
				pdeinterativo2013.pessoa ps
                INNER JOIN pdeinterativo2013.pessoatipoperfil      pt ON pt.pesid  = ps.pesid
                INNER JOIN pdeinterativo2013.tipoperfil 		   tp ON tp.tpeid  = pt.tpeid and tp.tpestatus = 'A'
    			LEFT JOIN  seguranca.usuario 				   us ON us.usucpf = ps.usucpf 
                INNER JOIN  pdeinterativo2013.planoformacaodocente pf ON pf.pesid  = ps.pesid
                LEFT  JOIN catalogocurso.curso                 cr ON cr.curid  = pf.curid
                LEFT  JOIN catalogocurso.areatematica          at ON at.ateid  = cr.ateid
                LEFT  JOIN catalogocurso.nivelcurso			   nc ON nc.ncuid  = cr.ncuid
                LEFT  JOIN pdeinterativo2013.periodocursoformacao  pe ON pe.pcfid  = pf.pcfid
                LEFT  JOIN catalogocurso.modalidadecurso	   mo ON mo.modid  = pf.modid
			WHERE 
				pt.pdeid = {$_SESSION['pdeinterativo2013_vars']['pdeid']}
                AND ps.pesstatus = 'A'
                AND (pt.tpeid in (2,7,8,9,10,11,14) OR pt.tpeid in (2,7,8,9,10,11,14))
			ORDER BY 
				3";*/
	//ver($sql);
	$sql = "
		SELECT DISTINCT case when pes.pflcod is not null then usu.usunome else pes.pesnome end as nome,
		       tpe.tpedesc,
		       tpe.tpeid,
		       pfd.pfdid,
		       pfd.pcfid,
		       pfd.modid,
		       cur.curid,
		       coalesce(ate.atedesc,'N/A') as atedesc,
		       coalesce(cur.curdesc,'N/A') as curdesc,
		       coalesce(pcf.pcfdesc,'N/A') as pcfdesc,
		       coalesce(ncu.ncudesc,'N/A') as ncudesc,
		       coalesce(mod.moddesc,'N/A') as moddesc,
		       coalesce(cur.curchmim,0)||'/'||coalesce(cur.curchmax,0) as curch,
		       coalesce(cur.curpercpremim,0)||'/'||coalesce(cur.curpercpremax,0) as curpercpre,
		       CASE
		           WHEN pfd.pfdhabilitado THEN 'check_p.gif'
		           WHEN pfd.pfdhabilitado is null THEN ' - '
		           ELSE 'exclui_p.gif'
		       END as pfdhabilitado,
		       pfd.pfdnhainteresse,
		       pfd.pfdnhacurso,
		       pfd.pfdcpf,
		       CASE
		           WHEN snf.esddsc is not null THEN snf.esddsc || ' no SNF'
		           ELSE                             esd.esddsc || ' no PDE'
		       END as situacao
		FROM pdeinterativo2013.pessoa pes
			INNER JOIN pdeinterativo2013.pessoatipoperfil     ptp  ON ptp.pesid     = pes.pesid
			INNER JOIN pdeinterativo2013.tipoperfil           tpe  ON tpe.tpeid     = ptp.tpeid
			AND                                                   tpe.tpestatus = 'A'
			LEFT  JOIN seguranca.usuario                  usu  ON usu.usucpf    = pes.usucpf
			INNER JOIN pdeinterativo2013.planoformacaodocente pfd  ON pfd.pesid     = pes.pesid
			LEFT  JOIN catalogocurso.curso                cur  ON cur.curid     = pfd.curid
			LEFT  JOIN catalogocurso.areatematica         ate  ON ate.ateid     = cur.ateid
			LEFT  JOIN catalogocurso.nivelcurso           ncu  ON ncu.ncuid     = cur.ncuid
			LEFT  JOIN pdeinterativo2013.periodocursoformacao pcf  ON pcf.pcfid     = pfd.pcfid
			LEFT  JOIN catalogocurso.modalidadecurso      mod  ON mod.modid     = pfd.modid
			LEFT  JOIN pdeinterativo2013.pdinterativo         pde  ON pde.pdeid     = ptp.pdeid
			LEFT  JOIN workflow.documento                 doc  ON doc.docid     = pde.formacaodocid
			LEFT  JOIN workflow.estadodocumento           esd  ON esd.esdid     = doc.esdid
			LEFT  JOIN (SELECT * FROM 
				dblink( 'host= user= password= port=5432 dbname=',
					'SELECT pri.pdicodinep,
						esd.esddsc
					 FROM snf.prioridadecursoescola pri  
						LEFT JOIN snf.prioridadedocumento  prd ON prd.prdid = pri.prdid
						LEFT JOIN workflow.documento       doc ON doc.docid = prd.docid
						LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid')
				AS resultado(pdicodinep varchar, esddsc varchar)) snf ON snf.pdicodinep = pde.pdicodinep
		WHERE ptp.pdeid     = {$_SESSION['pdeinterativo2013_vars']['pdeid']}
		AND   pes.pesstatus = 'A'
		AND   ptp.tpeid     in (2,7,8,9,10,11,14)
		ORDER BY 3
	";
	return $db->carregar($sql);
}


function toolEtapaCurso( $dados ){
	
	global $db;
	
	$sql = "SELECT
				no_etapa_ensino
			FROM
				catalogocurso.etapaensino_curso_publicoalvo ecp
			INNER JOIN educacenso_".ANO_CENSO.".tab_etapa_ensino etp ON etp.pk_cod_etapa_ensino = ecp.cod_etapa_ensino
			WHERE
				curid = ".$dados['curid'];
	
	$etapas = $db->carregarColuna($sql);
	
	foreach( $etapas as $etapa ){
		echo $etapa.'<br>';
	}
	
}

function detalheDocente( $dados ){
	
	global $db;
	
	$sql = "SELECT DISTINCT
				coalesce(pes.pesnome,d.no_docente) as no_docente,
				coalesce(fu.fexdesc,'Sem função') as fexdesc,
				es.no_escolaridade as no_escolaridade,
				af.no_nome_area_ocde as no_nome_area_ocde,
				pf.pfdemail,
				pf.pfdtel,
				pf.pfdcel
			FROM 
				pdeinterativo2013.planoformacaodocente     pf
			LEFT JOIN pdeinterativo2013.pessoa       	  		pes ON pes.pesid = pf.pesid 
			LEFT JOIN educacenso_".ANO_CENSO.".tab_docente 	   		 d  ON d.num_cpf = pf.pfdcpf
			LEFT JOIN pdeinterativo2013.pdinterativo 	  		pde ON pde.pdeid = pf.pdeid
			LEFT JOIN educacenso_".ANO_CENSO.".tab_entidade    		ent ON ent.cod_orgao_regional_inep = pde.pdicodinep
			LEFT JOIN educacenso_".ANO_CENSO.".tab_dado_escola 		des ON des.fk_cod_entidade = ent.pk_cod_entidade
			LEFT JOIN educacenso_".ANO_CENSO.".tab_docente_form_sup   bf ON bf.fk_cod_docente = d.pk_cod_docente
			LEFT JOIN educacenso_".ANO_CENSO.".tab_dado_docencia      dc ON dc.fk_cod_docente = d.pk_cod_docente
			LEFT JOIN catalogocurso.funcaoexercida		  	 fu ON fu.fexid = dc.id_tipo_docente
			LEFT JOIN educacenso_".ANO_CENSO.".tab_escolaridade       es ON es.pk_cod_escolaridade = d.fk_cod_escolaridade
			LEFT JOIN educacenso_".ANO_CENSO.".tab_area_ocde          af ON af.pk_cod_area_ocde = bf.fk_cod_area_ocde
			WHERE 
				pfdid = ".$dados['pfdid'];
//	ver($sql);
	$docente = $db->pegaLinha($sql);
	
	$sql = "SELECT DISTINCT
				pdilocalizacao, 
				ldedesc as id_localizacao_diferenciada
			FROM 
				pdeinterativo2013.planoformacaodocente     pf 
			INNER JOIN pdeinterativo2013.pdinterativo 	   pde ON pde.pdeid = pf.pdeid
			INNER JOIN educacenso_2010.tab_entidade    ent ON ent.pk_cod_entidade = pde.pdicodinep::bigint
			INNER JOIN educacenso_2010.tab_dado_escola des ON des.fk_cod_entidade = ent.pk_cod_entidade
			INNER JOIN catalogocurso.localizacaodiferenciadaescola lde ON lde.ldeid = des.id_localizacao_diferenciada+1
			WHERE 
				pfdid = ".$dados['pfdid'];
	$escola = $db->pegaLinha($sql);
	
	$sql = "SELECT DISTINCT
				ds.no_disciplina
			FROM 
				pdeinterativo2013.planoformacaodocente     pf
			INNER JOIN educacenso_".ANO_CENSO.".tab_docente 		  d ON pf.pfdcpf = d.num_cpf
			INNER JOIN educacenso_".ANO_CENSO.".tab_docente_disc_turma dt ON dt.fk_cod_docente = d.pk_cod_docente
			INNER JOIN educacenso_".ANO_CENSO.".tab_disciplina         ds ON ds.pk_cod_disciplina = dt.fk_cod_disciplina
			WHERE 
				pfdid = ".$dados['pfdid'];
	$disciplina = $db->carregarColuna($sql);
	
	$sql = "SELECT DISTINCT
				ee.no_etapa_ensino
			FROM 
				pdeinterativo2013.planoformacaodocente     pf 
			INNER JOIN educacenso_".ANO_CENSO.".tab_docente 		   d ON d.num_cpf = pf.pfdcpf
			INNER JOIN educacenso_".ANO_CENSO.".tab_docente_disc_turma dt ON dt.fk_cod_docente = d.pk_cod_docente
			INNER JOIN educacenso_".ANO_CENSO.".tab_turma   		  tu ON tu.pk_cod_turma = dt.fk_cod_turma
			INNER JOIN educacenso_".ANO_CENSO.".tab_etapa_ensino       ee ON ee.pk_cod_etapa_ensino = tu.fk_cod_etapa_ensino
			WHERE 	
				pfdid =".$dados['pfdid'];
	$etapa_ensino = $db->carregarColuna($sql);
	
	$sql = "SELECT DISTINCT
				no_mod_ensino
			FROM 
				pdeinterativo2013.planoformacaodocente     pf
			INNER JOIN educacenso_".ANO_CENSO.".tab_docente 		  d ON d.num_cpf = pf.pfdcpf
			INNER JOIN educacenso_".ANO_CENSO.".tab_docente_disc_turma dt ON dt.fk_cod_docente = d.pk_cod_docente
			INNER JOIN educacenso_".ANO_CENSO.".tab_turma   		 tu ON tu.pk_cod_turma = dt.fk_cod_turma
			INNER JOIN educacenso_".ANO_CENSO.".tab_mod_ensino 	 mo ON mo.pk_cod_mod_ensino = tu.fk_cod_mod_ensino
			WHERE 
				pfdid = ".$dados['pfdid'];
	$mod_ensino = $db->carregarColuna($sql);
	
	// Recursos multi-funcionais
	$sql = "SELECT DISTINCT 
				'Possui'
			FROM 
				painel.indicador i
			INNER JOIN painel.seriehistorica 		 sh ON sh.indid=i.indid
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
			INNER JOIN pdeinterativo2013.pdinterativo   pde ON pde.pdicodinep = dsh.dshcod
			WHERE 
				i.indid = 268
				AND pde.pdeid = ".$_SESSION['pdeinterativo2013_vars']['pdeid']."
				AND (sehvalor IS NOT NULL OR sehqtde IS NOT NULL )
				AND sehstatus != 'I'";
	$indicador = $db->pegaUm($sql);
	
	?>
	<table class="tabela" bgcolor="#f8f8f8" cellSpacing="5" cellPadding="5" align="center">
		<tr>
			<td colspan="2" align="center" style="font-size:15px;" bgcolor="#f8f8f8">
				<b>Dados do CENSO 2010</b>
			</td>
		</tr>
		<tr>
			<td>
				<table class="tabela" bgcolor="#f5f5f5" cellSpacing="2" cellPadding="2" align="center">
					<tr >
						<td bgcolor="#c4c4c4" width="50%" >
							<b>Dados Docente:</b>
						</td>
						<td bgcolor="#c4c4c4" >
							<b>Dados da Escola:</b>
						</td>
					</tr>
					<tr>
						<td valign="top">
							<b>Nome:</b> <?=$docente['no_docente'] ?><br>
							<b>Função Exercida:</b> <?=$docente['fexdesc'] ?><br>
							<b>Nível de Escolaridade:</b> <?=$docente['no_escolaridade'] ?><br>
							<b>Área de Formação:</b> <?=$docente['no_nome_area_ocde'] ?><br>
							<b>E-mail:</b> <?=$docente['pfdemail'] ?><br>
							<b>Telefone:</b> <?=$docente['pfdtel'] ?><br>
							<b>Celular:</b> <?=$docente['pfdcel'] ?><br>
							<b>Disciplina que leciona:</b><br>
							<?php foreach( $disciplina as $disc ){?>
							- <?=$disc ?><br>
							<?php }?>
							<b>Etapa de Ensino em que Leciona:</b><br>
							<?php foreach( $etapa_ensino as $etp ){?>
							- <?=$etp ?><br>
							<?php }?>
							<b>Modalidade de Ensino em que Leciona:</b><br>
							<?php foreach( $mod_ensino as $mod ){?>
							- <?=$mod ?><br>
							<?php }?>
						</td>
						<td valign="top">
							<b>Localização:</b> <?=$escola['pdilocalizacao'] ?><br>
							<b>Localização Diferênciada:</b> <?=$escola['id_localizacao_diferenciada'] ?><br>
							<b>Recursos multi-funcionais:</b> <?=$indicador ?><br>
						</td>
					</tr>
				</td>
			</table>
		</tr>
	</table>
	<?php 
	
}

function carregarOrdemPrioridade(){
	global $db;
	
	$sql = "SELECT DISTINCT
						case when ps.pflcod is not null then us.usunome else ps.pesnome end as nome,
						pf.modid,
						cr.curid,
						coalesce(curdesc,'N/A') as curdesc,
						coalesce(moddesc,'N/A') as moddesc,
						coalesce(curchmim,0)||'/'||coalesce(curchmax,0) as curch,
						coalesce(curpercpremim,0)||'/'||coalesce(curpercpremax,0) as curpercpre
					FROM 
						pdeinterativo2013.pessoa ps
		                INNER JOIN pdeinterativo2013.pessoatipoperfil      pt ON pt.pesid  = ps.pesid
		                INNER JOIN pdeinterativo2013.tipoperfil 		   tp ON tp.tpeid  = pt.tpeid and tp.tpestatus = 'A'
		    			LEFT JOIN  seguranca.usuario 				   us ON us.usucpf = ps.usucpf 
		                INNER JOIN  pdeinterativo2013.planoformacaodocente pf ON pf.pesid  = ps.pesid
		                LEFT  JOIN catalogocurso.curso                 cr ON cr.curid  = pf.curid
		                LEFT  JOIN catalogocurso.areatematica          at ON at.ateid  = cr.ateid
		                LEFT  JOIN catalogocurso.nivelcurso			   nc ON nc.ncuid  = cr.ncuid
		                LEFT  JOIN pdeinterativo2013.periodocursoformacao  pe ON pe.pcfid  = pf.pcfid
		                LEFT  JOIN catalogocurso.modalidadecurso	   mo ON mo.modid  = pf.modid
					WHERE 
						pt.pdeid = {$_SESSION['pdeinterativo2013_vars']['pdeid']}
		                and ps.pesstatus = 'A'
		UNION ALL
		SELECT DISTINCT
						d.no_docente,
						pf.modid,
						cr.curid,
						coalesce(curdesc,'N/A') as curdesc,
						coalesce(moddesc,'N/A') as moddesc,
						coalesce(curchmim,0)||'/'||coalesce(curchmax,0) as curch,
						coalesce(curpercpremim,0)||'/'||coalesce(curpercpremax,0) as curpercpre
					FROM 
						educacenso_".ANO_CENSO.".tab_docente d 
					INNER JOIN pdeinterativo2013.planoformacaodocente pf ON pf.pk_cod_docente = d.pk_cod_docente
					INNER JOIN educacenso_".ANO_CENSO.".tab_dado_docencia  dc ON dc.fk_cod_docente = d.pk_cod_docente
					LEFT  JOIN catalogocurso.curso                cr ON cr.curid		  = pf.curid
					LEFT  JOIN catalogocurso.areatematica         at ON at.ateid		  = cr.ateid
					LEFT  JOIN catalogocurso.nivelcurso			  nc ON nc.ncuid		  = cr.ncuid
					LEFT  JOIN pdeinterativo2013.periodocursoformacao pe ON pe.pcfid		  = pf.pcfid
					LEFT  JOIN catalogocurso.modalidadecurso	  mo ON mo.modid		  = pf.modid
					WHERE 
						id_tipo_docente = 1
						AND pt.pdeid = {$_SESSION['pdeinterativo2013_vars']['pdeid']}
					ORDER BY 
						2";
	
	return $db->carregar($sql);
}

function manterCursoSugerido( $dados ){
	global $db;
	extract( $dados );
	
	$foccurso 			= trim($foccurso) != ''		 	 ? "'".$foccurso."'" 		  : 'null';
	$focdescricao 		= trim($focdescricao) != '' 	 ? "'".substr($focdescricao,0,500)."'" 	  : 'null';
	$focjustificativa 	= trim($focjustificativa) != ''  ? "'".substr($focjustificativa,0,500)."'"  : 'null';
	$focqtdprof 		= trim($focqtdprof) != '' 		 ? $focqtdprof 				  : 'null';
	$focformcontinuada 	= trim($focformcontinuada) != '' ? "'".$focformcontinuada."'" : 'null';
	
	if( empty($focid) ){
		$sql = "INSERT INTO pdeinterativo2013.formacaocursosugerido(foccurso, focdescricao, focjustificativa, focqtdprof, focformcontinuada, pdeid) 
				VALUES ( {$foccurso}, {$focdescricao}, {$focjustificativa}, {$focqtdprof}, {$focformcontinuada}, {$_SESSION['pdeinterativo2013_vars']['pdeid']} ) returning focid";
		
		$focid = $db->pegaUm( $sql );
		
		$sql = "DELETE FROM pdeinterativo2013.cursosugeridofuncao 	  WHERE focid = $focid;
				DELETE FROM pdeinterativo2013.cursosugeridoetapa  	  WHERE focid = $focid;
				DELETE FROM pdeinterativo2013.cursosugeridodisciplina WHERE focid = $focid;
				DELETE FROM pdeinterativo2013.cursosugeridomodalidade WHERE focid = $focid;";
		$db->executar( $sql );
		$db->commit();
		
		if( is_array( $fexid ) && !empty($fexid[0]) ){
			foreach ($fexid as $v) {			
				$sql = "INSERT INTO pdeinterativo2013.cursosugeridofuncao(focid, fexid) 
						VALUES ($focid, $v)";
				$db->executar( $sql );
				$db->commit();
			}
		}
		if( is_array( $pk_cod_etapa_ensino ) && !empty($pk_cod_etapa_ensino[0]) ){
			foreach ($pk_cod_etapa_ensino as $ensino) {			
				$sql = "INSERT INTO pdeinterativo2013.cursosugeridoetapa(focid, pk_cod_etapa_ensino) 
						VALUES ($focid, $ensino)";
				$db->executar( $sql );
				$db->commit();
			}
		}
		if( is_array( $pk_cod_disciplina ) && !empty($pk_cod_disciplina[0]) ){
			foreach ($pk_cod_disciplina as $disciplina) {			
				$sql = "INSERT INTO pdeinterativo2013.cursosugeridodisciplina(focid, pk_cod_disciplina) 
						VALUES ($focid, $disciplina)";
				$db->executar( $sql );
				$db->commit();
			}
		}
		if( is_array( $pk_cod_mod_ensino ) && !empty($pk_cod_mod_ensino[0]) ){
			foreach ($pk_cod_mod_ensino as $mod) {			
				$sql = "INSERT INTO pdeinterativo2013.cursosugeridomodalidade(focid, pk_cod_mod_ensino) 
						VALUES ($focid, $mod)";
				$db->executar( $sql );
				$db->commit();
			}
		}
	} else {
		$sql = "UPDATE pdeinterativo2013.formacaocursosugerido SET 
					foccurso 			= {$foccurso},
					focdescricao 		= {$focdescricao},
					focjustificativa 	= {$focjustificativa},
					focqtdprof 			= {$focqtdprof},
					focformcontinuada 	= {$focformcontinuada},
					pdeid 				= {$_SESSION['pdeinterativo2013_vars']['pdeid']}
				WHERE 
				  	focid = {$focid}";
		$db->executar( $sql );
		$db->commit();
		
		$sql = "DELETE FROM pdeinterativo2013.cursosugeridofuncao 	  WHERE focid = $focid;
				DELETE FROM pdeinterativo2013.cursosugeridoetapa  	  WHERE focid = $focid;
				DELETE FROM pdeinterativo2013.cursosugeridodisciplina WHERE focid = $focid;
				DELETE FROM pdeinterativo2013.cursosugeridomodalidade WHERE focid = $focid;";
		$db->executar( $sql );
		$db->commit();
		
		if( is_array( $fexid ) && !empty($fexid[0]) ){
			foreach ($fexid as $v) {			
				$sql = "INSERT INTO pdeinterativo2013.cursosugeridofuncao(focid, fexid) 
						VALUES ($focid, $v)";
				$db->executar( $sql );
				$db->commit();
			}
		}
		if( is_array( $pk_cod_etapa_ensino ) && !empty($pk_cod_etapa_ensino[0]) ){
			foreach ($pk_cod_etapa_ensino as $ensino) {			
				$sql = "INSERT INTO pdeinterativo2013.cursosugeridoetapa(focid, pk_cod_etapa_ensino) 
						VALUES ($focid, $ensino)";
				$db->executar( $sql );
				$db->commit();
			}
		}
		if( is_array( $pk_cod_disciplina ) && !empty($pk_cod_disciplina[0]) ){
			foreach ($pk_cod_disciplina as $disciplina) {			
				$sql = "INSERT INTO pdeinterativo2013.cursosugeridodisciplina(focid, pk_cod_disciplina) 
						VALUES ($focid, $disciplina)";
				$db->executar( $sql );
				$db->commit();
			}
		}
		if( is_array( $pk_cod_mod_ensino ) && !empty($pk_cod_mod_ensino[0]) ){
			foreach ($pk_cod_mod_ensino as $mod) {			
				$sql = "INSERT INTO pdeinterativo2013.cursosugeridomodalidade(focid, pk_cod_mod_ensino) 
						VALUES ($focid, $mod)";
				$db->executar( $sql );
				$db->commit();
			}
		}
	}
	if( $dados['proximo'] ){
		$db->sucesso('principal/planoestrategico', 'A&aba=formacao_0_planoformacao&aba1=formacao_4_vizualizacao');
	}else{
		$db->sucesso('principal/planoestrategico', 'A&aba=formacao_0_planoformacao&aba1=formacao_1_cursosugerido');
	}

}

function carregaCursoSugerido(){
	global $db;
	
	$sql = "SELECT focid, pdeid, focformcontinuada, foccurso, focdescricao, focjustificativa, focqtdprof
			FROM pdeinterativo2013.formacaocursosugerido WHERE pdeid = {$_SESSION['pdeinterativo2013_vars']['pdeid']}";
	$arrFormacao = $db->pegaLinha( $sql );
	$arrFormacao = $arrFormacao ? $arrFormacao : array();
	return $arrFormacao;
}

function pegaDocid( $pdeid ) {
	
	global $db;
	
	require_once APPRAIZ . 'includes/workflow.php';
	
	$sql = "SELECT
				formacaodocid
			FROM
				pdeinterativo2013.pdinterativo
			WHERE
				pdeid = $pdeid";
	
	$docid = $db->pegaUm( $sql );
	
	if( !$docid ) {
		
		$tpdid = 55;
		
		$docdsc = "Escola PDE Escola N° ".$pdeid;
		
		// cria documento do WORKFLOW
		$docid = wf_cadastrarDocumento( $tpdid, $docdsc );

		// atualiza pap do EMI
		$sql = "UPDATE
					pdeinterativo2013.pdinterativo
				SET 
					formacaodocid = {$docid} 
				WHERE
					pdeid = {$pdeid}";

		$db->executar( $sql );
		$db->commit();
	}
	
	return $docid;
	
}

function desvincularCurso( $request ){
	
	global $db;
	
	$sql = "UPDATE pdeinterativo2013.planoformacaodocente SET
				curid = null,
				pcfid = null,
				modid = null,
				pfdprioridade = null,
				pfdhabilitado = null,
				pfdnhainteresse = null,
				pfdnhacurso = null
			WHERE
				pfdid = ".$request['pfdidExcluir'];
	$db->executar($sql);
	$db->commit();
	echo "<script>
			window.location = 'pdeinterativo2013.php?modulo=principal/planoestrategico&acao=A&aba=formacao_0_planoformacao&aba1=formacao_0_planoformacao&vv=".date("Ymdhis")."';
		  </script>";
}

function salvarPrioridade( $dados ){
	global $db;
	
	if( is_array($dados['pfdprioridade']) && !empty($dados['pfdprioridade']) ){
		foreach ($dados['pfdprioridade'] as $pfdid => $pfdprioridade) {
			$sql = "UPDATE pdeinterativo2013.planoformacaodocente SET
					  pfdprioridade = ".(($pfdprioridade)?$pfdprioridade:"NULL")."
					WHERE 
					  pfdid = $pfdid";
			$db->executar( $sql );
			$db->commit();
		}
	}
	
	if( $dados['action'] == 'continuar' ){
		$db->sucesso('principal/planoestrategico', 'A&aba=formacao_0_planoformacao&aba1=formacao_3_demanda_social');
	} else {
		$db->sucesso('principal/planoestrategico', 'A&aba=formacao_0_planoformacao&aba1=formacao_2_relacaoprioridade');
	}

}

function verificaPreenchimento( $pdeid ){
	
	global $db;
	
	$sql = "SELECT DISTINCT
				'true'
			FROM
				pdeinterativo2013.planoformacaodocente
			WHERE
				pdeid = $pdeid
				AND pfdstatus = 'A'";
	$existe = $db->pegaUm($sql);

	if( $existe == 'true' ){

		$sql = "SELECT DISTINCT
					'true'
				FROM
					pdeinterativo2013.planoformacaodocente
				WHERE
					pdeid = $pdeid
					AND (curid is null
						OR pcfid is null
						OR modid is null
						OR pfdprioridade is null)
					AND pfdnhainteresse is null
					AND pfdnhacurso is null
					AND pfdstatus = 'A'
					AND pfdcpf NOT IN (SELECT pfdcpf FROM pdeinterativo2013.planoformacaodocente WHERE curid IS NOT NULL)";
//ver($sql);
		return $db->pegaUm($sql) == 'true' ? false : true;
	}
	return false;
}

function pegaPendencias( $docid ){
	
	global $db;
	
	$pendencias = '';
	
	$sql = "SELECT
				esdid
			FROM
				workflow.documento
			WHERE
				docid = $docid";
	$esdid = $db->pegaUm($sql);
	
	if( $esdid == WF_EM_ELABORACAO ){
		$pdeid = $_SESSION['pdeinterativo2013_vars']['pdeid'];
		$sql = "SELECT DISTINCT
					'Falta o Preenchimento das seguintes abas:<br>'||
					CASE WHEN ( ( curid is null OR pcfid is null OR modid is null ) AND pfdnhainteresse is null AND pfdnhacurso is null ) 
						THEN '- 2.1 Proposta da Escola<br>' 
						ELSE '' 
					END ||
					CASE WHEN (pfdprioridade is null) 
						THEN '- 2.2 Ordem de Prioridade' 
						ELSE '' 
					END as pendencias
				FROM
					pdeinterativo2013.planoformacaodocente
				WHERE
					pdeid = $pdeid
					AND (curid is null
						OR pcfid is null
						OR modid is null
						OR pfdprioridade is null)
					AND pfdnhainteresse is null
					AND pfdnhacurso is null
					AND pfdstatus = 'A'
					AND pfdcpf NOT IN (SELECT pfdcpf FROM pdeinterativo2013.planoformacaodocente WHERE curid IS NOT NULL)";
		//ver($sql, $pendencias, d);
		$pendencias = $db->pegaUm($sql);
	}
	
	return $pendencias;
}

function tramitaFormacao($dados){
	
	global $db;
	
	require_once APPRAIZ . 'includes/workflow.php';
	
	wf_alterarEstado( $dados['docid'], $dados['aedid'], 'Plano de Formação', array( 'docid' => $dados['docid'], 'pdeid' => $dados['pdeid'] ) );
	
	echo "<script>
			alert('Enviado com sucesso.');
			window.location = 'pdeinterativo2013.php?modulo=principal/planoestrategico&acao=A&aba=formacao_0_planoformacao';
		  </script>";
	
}

function enviarEmail( $pdeid ){
	
	global $db;
	
	$sql = "SELECT DISTINCT
				coalesce(d.no_docente,pesnome) as no_docente,
				pf.pfdemail,
				cur.curdesc,
				ent.no_escola
				
			FROM 
				pdeinterativo2013.planoformacaodocente     pf
			INNER JOIN catalogocurso.curso 	       cur ON cur.curid = pf.curid
			LEFT JOIN pdeinterativo2013.pessoa         pes ON pes.pesid = pf.pesid
			
			LEFT JOIN educacenso_2010.tab_docente	d  ON pf.pk_cod_docente = d.pk_cod_docente OR d.num_cpf = pes.usucpf
			INNER JOIN pdeinterativo2013.pdinterativo   pde ON pde.pdeid = pf.pdeid
			LEFT JOIN educacenso_2010.tb_escola_inep_2010 ent ON ent.pk_cod_entidade = pde.pdicodinep::numeric
			WHERE 
				pf.pfdemail IS NOT NULL
				AND cur.curid IS NOT NULL
				AND pf.pdeid = ".$_SESSION['pdeinterativo2013_vars']['pdeid'];
	
	$docentes = $db->carregar($sql);
	
	foreach($docentes as $docente){
		
		$assunto = "Indicação para o Plano de Formação Continuada";
		
		$conteudo = "Esta mensagem é gerada automaticamente pelo sistema e não precisa ser respondida.
		
					Senhor(a) {$docente['no_docente']},
					Você foi indicado para participar do curso '{$docente['curdesc']}' pela escola '{$docente['no_escola']}'. As propostas das escolas serão analisadas pela Secretaria de Educação e submetidas ao Fórum Estadual de Formação Docente. Acompanhe os próximos passos junto à sua secretaria.
					
					Secretaria de Educação Básica/ MEC";
			
		enviar_email(array('nome'=>SIGLA_SISTEMA. ' - Plano de Formação Continuada', 'email'=>'noreply@mec.gov.br'), $docente['pfdemail'], $assunto, $conteudo, $cc, $cco );
	}
	
	return true;
}

function recuperaCursosDemandaSocial(){
	
	global $db;
	
	$sql = "SELECT DISTINCT
				cur.curid,
				cur.curdesc,
				ate.atedesc,
				ncu.ncudesc,
				cur.curchmim,
				cur.curchmax
			FROM 
				pdeinterativo2013.planoformacaodocente     pf
			INNER JOIN catalogocurso.curso 	      cur ON cur.curid = pf.curid AND curpademsocial is true
			INNER JOIN catalogocurso.areatematica ate ON ate.ateid = cur.ateid
			INNER JOIN catalogocurso.nivelcurso   ncu ON ncu.ncuid = cur.ncuid
			WHERE 
				pf.pdeid = ".$_SESSION['pdeinterativo2013_vars']['pdeid'];
	$cursos = $db->carregar($sql);

	return $cursos;
}

function recuperaPublicoAlvoDemandaSocialCurso( $curid ){
	
	global $db;

	$sql = "SELECT DISTINCT
				pad.paddesc
			FROM 
				catalogocurso.curso cur	
			INNER JOIN catalogocurso.cursodemandasocial 	  cds ON cds.curid = cur.curid
			INNER JOIN catalogocurso.publicoalvodemandasocial pad ON pad.padid = cds.padid
			WHERE 
				cur.curid = ".$curid;
	return $db->carregarColuna($sql);
}

function recuperaModalidadeCurso( $curid ){
	
	global $db;

	$sql = "SELECT DISTINCT
				mod.moddesc
			FROM 
				catalogocurso.curso cur      		 
			INNER JOIN catalogocurso.modalidadecurso_curso moc ON moc.curid = cur.curid
			INNER JOIN catalogocurso.modalidadecurso 	   mod ON mod.modid = moc.modid
			INNER JOIN pdeinterativo2013.planoformacaodocente  pfd ON pfd.modid = mod.modid
			WHERE 
				cur.curid = ".$curid."
				AND pdeid = ".$_SESSION['pdeinterativo2013_vars']['pdeid'];
	return $db->carregarColuna($sql);
}

function pegaSemestresCurso( $curid ){
	
	global $db;

	$sql = "SELECT DISTINCT
				pcf.pcfid,
				pcf.pcfdesc
			FROM 
				pdeinterativo2013.planoformacaodocente pfd     		 
			INNER JOIN pdeinterativo2013.periodocursoformacao pcf ON pcf.pcfid = pfd.pcfid
			WHERE 
				curid = ".$curid."
				AND pdeid = ".$_SESSION['pdeinterativo2013_vars']['pdeid']."
				AND pfdstatus = 'A'
			ORDER BY
				1";
	return $db->carregar($sql);
}

function recuperaProfissionaisCurso( $curid, $pcfid ){
	
	global $db;
	
	$sql = "SELECT DISTINCT
				coalesce(d.no_docente,pes.pesnome) as no_docente,
				replace(to_char(coalesce(d.num_cpf,pes.usucpf)::numeric,'000,000,000-00'),',','.') as num_cpf,
				coalesce(fu.fexdesc,'Sem função encontrada') as fexdesc,
				mod.modid,
				moddesc,
				pf.pfdemail,
				pf.pfdtel,
				pf.pfdcel
			FROM 
				pdeinterativo2013.planoformacaodocente     pf
			INNER JOIN catalogocurso.modalidadecurso mod ON mod.modid = pf.modid
			LEFT  JOIN pdeinterativo2013.pessoa       	  		pes ON pes.pesid = pf.pesid 
			LEFT  JOIN educacenso_2010.tab_docente 	   		 d  ON d.num_cpf = pf.pfdcpf
			LEFT  JOIN pdeinterativo2013.pdinterativo 	  		pde ON pde.pdeid = pf.pdeid
			LEFT  JOIN educacenso_2010.tab_entidade    		ent ON ent.cod_orgao_regional_inep = pde.pdicodinep
			LEFT  JOIN educacenso_2010.tab_dado_escola 		des ON des.fk_cod_entidade = ent.pk_cod_entidade
			LEFT  JOIN educacenso_2010.tab_dado_docencia      dc ON dc.fk_cod_docente = d.pk_cod_docente
			LEFT  JOIN catalogocurso.funcaoexercida		  	 fu ON fu.fexid = dc.id_tipo_docente
			WHERE 
				pf.curid = $curid
				AND pf.pcfid = $pcfid
				AND pf.pdeid = ".$_SESSION['pdeinterativo2013_vars']['pdeid'];
//	ver($sql);
	return $db->carregar($sql);
}

function recuperaMembrosCurso( $curid, $pcfid ){
	
	global $db;
	
	$sql = "SELECT 
				mdsnome, 
				replace(to_char(mdscpf::numeric,'000,000,000-00'),',','.') as mdscpf, 
				padid,
				modid,
				mdsemail, 
			    mdstelefonefixo, 
			    mdscelular 
			FROM 
				catalogocurso.membrosdemandasocial
			WHERE 
				cpdstatus = 'A'
				AND curid = $curid
				AND pcfid = $pcfid
				AND pdeid = ".$_SESSION['pdeinterativo2013_vars']['pdeid'];
	return $db->carregar($sql);
}

function gravarDemandaSocial( $request ){
	
	global $db;
	
	$sql = "UPDATE catalogocurso.membrosdemandasocial SET cpdstatus = 'I' WHERE pdeid = ".$_SESSION['pdeinterativo2013_vars']['pdeid'].";";
	if( is_array($request['mdsnome']) ){
		foreach( $request['mdsnome'] as $curid => $mdsnomes ){
			foreach( $mdsnomes as $pcfid => $mdsnome ){
				foreach( $mdsnome as $k => $nome ){
					if( $nome != '' && $request['padid'][$curid][$pcfid][$k] != '' && $request['modid'][$curid][$pcfid][$k] != '' && $request['mdstelefonefixo'][$curid][$pcfid][$k] != '' ){
						$sql .= "INSERT INTO catalogocurso.membrosdemandasocial(curid,pcfid,padid,modid,pdeid,mdscpf,mdsnome,mdsemail,mdstelefonefixo,mdscelular) 
									VALUES ($curid,$pcfid,
											{$request['padid'][$curid][$pcfid][$k]},
											{$request['modid'][$curid][$pcfid][$k]},
											{$_SESSION['pdeinterativo2013_vars']['pdeid']},
											'".trim(str_replace(Array('.','-'),'',$request['mdscpf'][$curid][$pcfid][$k]))."',
											'{$request['mdsnome'][$curid][$pcfid][$k]}',
											".($request['mdsemail'][$curid][$pcfid][$k] != '' ? "'".$request['mdsemail'][$curid][$pcfid][$k]."'" : 'null').",
											'{$request['mdstelefonefixo'][$curid][$pcfid][$k]}',
											".($request['mdscelular'][$curid][$pcfid][$k] != '' ? "'".$request['mdscelular'][$curid][$pcfid][$k]."'" : 'null')." );";
					}
				}
			}
		}
		$db->executar($sql);
		$db->commit();
		if( $request['proximo'] == 'proximo' ){
			echo "<script>
					alert('Dados gravados com sucesso.');
					window.location = 'pdeinterativo2013.php?modulo=principal/planoestrategico&acao=A&aba=formacao_0_planoformacao&aba1=formacao_1_cursosugerido';
				  </script>";
		}else{
			echo "<script>
					alert('Dados gravados com sucesso.');
					window.location = 'pdeinterativo2013.php?modulo=principal/planoestrategico&acao=A&aba=formacao_0_planoformacao&aba1=formacao_3_demanda_social';
				  </script>";
		}
	}
	echo "<script>
				window.location = 'pdeinterativo2013.php?modulo=principal/planoestrategico&acao=A&aba=formacao_0_planoformacao&aba1=formacao_3_demanda_social';
			  </script>";
}

function pegaPermissao(){
	
	global $db;
	
	$sql = "SELECT
				true
			FROM
				pdeinterativo2013.pdinterativo pde
			INNER JOIN workflow.documento doc ON doc.docid = pde.formacaodocid AND doc.esdid = ".WF_EM_ELABORACAO."
			WHERE
				pde.pdeid = ".$_SESSION['pdeinterativo2013_vars']['pdeid'];
	$permissoes['gravar'] = $db->pegaUm($sql);
	$permissoes['gravar'] = $permissoes['gravar'] == 't' ? true : false;
	return $permissoes;
}

function toolPublicoAlvoDemandaSocialCurso( $request ){
	
	global $db;
	
	$publicos = recuperaPublicoAlvoDemandaSocialCurso( $request['curid'] );
						
	foreach( $publicos as $publico){
		echo "- ".$publico.".<br>";
	}
}
?>
