<?php

	if ($_REQUEST['painel'])
	{
		extract ($_REQUEST);

	}
	/* Componente Historico de consulta.
	 * Salva os POST da Consulta efetuado
	* Código implementado em 17/08/2012.
	* Luciano F. Ribeiro.
	*/
	if($_REQUEST['requisicao'] == 'salvarConsulta'){
		$existe_rel = 0;

		$sql = "select prtid from public.parametros_tela where prtdsc = '".$_POST['titulo_consulta']."' and usucpf ='".$_SESSION['usucpf']."'and mnuid =".$_SESSION['mnuid'].";";

		$existe_rel = $db->pegaUm( $sql );

		if ($existe_rel > 0){
			$sql = sprintf(
					"UPDATE public.parametros_tela SET prtdsc = '%s', prtobj = '%s', prtpublico = 'FALSE', usucpf = '%s', mnuid = %d, prtobservacao = '%s' WHERE prtid = %d",
					$_POST['titulo_consulta'],
					addslashes( addslashes( serialize( $_REQUEST ) ) ),
					$_SESSION['usucpf'],
					$_SESSION['mnuid'],
					$_POST['obs_consulta'],
					$existe_rel
			);
			$db->executar( $sql );
			$db->commit();
		}else{
			$sql = sprintf(
					"INSERT INTO public.parametros_tela ( prtdsc, prtobj, prtpublico, usucpf, mnuid, prtobservacao) VALUES ( '%s', '%s', %s, '%s', %d, '%s' )",
					$_POST['titulo_consulta'],
					addslashes( addslashes( serialize( $_REQUEST ) ) ),
					'FALSE',
					$_SESSION['usucpf'],
					$_SESSION['mnuid'],
					$_POST['obs_consulta']
			);
			$db->executar( $sql );
			$db->commit();
		}
		$db->sucesso('relatorio/download');
		die;
	}

	/* Componente Historico de consulta.
	 * Transforma consulta em pública.
	 * Código implementado em 17/08/2012.
	 * Luciano F. Ribeiro.
	 */
	if ( $_REQUEST['prtid'] && $_REQUEST['publico'] ){
		$sql = sprintf(
				"UPDATE public.parametros_tela SET prtpublico = case when prtpublico = true then false else true end WHERE prtid = %d",
				$_REQUEST['prtid']
		);
		$db->executar( $sql );
		$db->commit();
		$db->sucesso('relatorio/download');
		die;
	}

	/* Componente Historico de consulta.
	 * Exclui consulta.
	 * Código implementado em 17/08/2012.
	 * Luciano F. Ribeiro.
	 */
	if ( $_REQUEST['prtid'] && $_REQUEST['excluir'] == 1 ) {
		$sql = sprintf(
				"DELETE from public.parametros_tela WHERE prtid = %d",
				$_REQUEST['prtid']
		);
		$db->executar( $sql );
		$db->commit();
		$db->sucesso('relatorio/download');
		die;
	}

	/* Componente Historico de consulta.
	 * Carrega a consulta salva no historico, consulta dados em banco.
	 * Código implementado em 17/08/2012.
	 * Luciano F. Ribeiro.
	 */
	if ( $_REQUEST['prtid'] && $_REQUEST['carregar'] == 1 ){

		$sql   = sprintf("select prtobj from public.parametros_tela where prtid = ".$_REQUEST['prtid']);
		$itens = $db->pegaUm( $sql );
		$dados = unserialize( stripslashes( stripslashes($itens) ) );

		extract($dados);
		$_REQUEST = $dados;
		$_POST = $dados;
	}


ini_set("memory_limit","500M");
set_time_limit(0);

$origem[TPENSPROF]  = array();
$destino[TPENSPROF] = array('estuf' => array(
							'codigo'    => 'estuf',
							'descricao' => 'UF'
							),
							'mundescricao' => array(
								'codigo'    => 'mundescricao',
								'descricao' => 'Município'
							),
							'muncod' => array(
								'codigo'    => 'muncod',
								'descricao' => 'Código do município'
							),
							'entid' => array(
								'codigo'    => 'entid',
								'descricao' => 'Código do campus'
							),
							'entunicod' => array(
								'codigo'    => 'entunicod',
								'descricao' => 'Código da unidade (UO)'
							),
							'unidadeorc' => array(
								'codigo'    => 'unidadeorc',
								'descricao' => 'Instituto'
							),
							'entnome' => array(
								'codigo'    => 'entnome',
								'descricao' => 'Campus'
							),
							'entsig' => array(
								'codigo'    => 'entsig',
								'descricao' => 'Sigla da unidade'
							),
							'exidsc' => array(
								'codigo'    => 'exidsc',
								'descricao' => 'Existência'
							),
							'cmpsituacao' => array(
								'codigo'    => 'cmpsituacao',
								'descricao' => 'Situação'
							),
							'cmpinstalacao' => array(
								'codigo'    => 'cmpinstalacao',
								'descricao' => 'Instalações'
							),
							'obrascampus' => array(
								'codigo'    => 'obrascampus',
								'descricao' => 'Obras no campus'
							),
							'cmpsituacaoobra' => array(
								'codigo'    => 'cmpsituacaoobra',
								'descricao' => 'Situação da obra no campus/uned'
							),
							'matriculas' => array(
								'codigo'    => 'matriculas',
								'descricao' => 'MATRÍCULAS'
							),
							'datacriacao' => array(
								'codigo'    => 'datacriacao',
								'descricao' => 'Data de criação'
							),
							'cptdsc' => array(
								'codigo'    => 'cptdsc',
								'descricao' => 'Tipo'
							),
							'cmpdataimplantacao' => array(
								'codigo'    => 'cmpdataimplantacao',
								'descricao' => 'Data do início das atividades'
							),
							'cmpdatainauguracao' => array(
								'codigo'    => 'cmpdatainauguracao',
								'descricao' => 'Data de Inauguração'
							),
							'insprevisto' => array(
								'codigo'    => 'insprevisto',
								'descricao' => 'Investimento atual (Previsto)'
							),
							'insrealizado' => array(
								'codigo'    => 'insrealizado',
								'descricao' => 'Investimento total (Realizado)'
							),
							'insrealizado2005_2010' => array(
								'codigo'    => 'insrealizado2005_2010',
								'descricao' => 'Investimentos realizados 2005/2010'
							),
							'insprevisto2011_2014' => array(
								'codigo'    => 'insprevisto2011_2014',
								'descricao' => 'Investimentos previstos 2011/2014'
							),
							'insrealizadomaior2011' => array(
								'codigo'    => 'insrealizadomaior2011',
								'descricao' => 'Investimentos realizados a partir de 2011'
							),
							'dirigentecampus' => array(
								'codigo'    => 'dirigentecampus',
								'descricao' => 'Dirigente do campus'
							),
							'enderecocampus' => array(
									'codigo'    => 'enderecocampus',
									'descricao' => 'Endereço do Campus'
							),
							'enderecounidadeorc' => array(
									'codigo'    => 'enderecounidadeorc',
									'descricao' => 'Endereço do Instituto'
							),
							'emailcampus' => array(
								'codigo'    => 'emailcampus',
								'descricao' => 'E-mail do campus'
							),
							'telefonecampus' => array(
								'codigo'    => 'telefonecampus',
								'descricao' => 'Telefone do campus'
							),
							'latitude' => array(
								'codigo'    => 'latitude',
								'descricao' => 'Latitude'
							),
							'longitude' => array(
								'codigo'    => 'longitude',
								'descricao' => 'Longitude'
							)

					);

$origem[TPENSSUP] = array();
$destino[TPENSSUP] = array('estuf' => array(
							'codigo'    => 'estuf',
							'descricao' => 'UF'
						),
						'mundescricao' => array(
							'codigo'    => 'mundescricao',
							'descricao' => 'Município'
						),
						'muncod' => array(
							'codigo'    => 'muncod',
							'descricao' => 'Código do município'
						),
						'entid' => array(
							'codigo'    => 'entid',
							'descricao' => 'Código do campus'
						),
						'entunicod' => array(
							'codigo'    => 'entunicod',
							'descricao' => 'Código da unidade (UO)'
						),
						'unidadeorc' => array(
							'codigo'    => 'unidadeorc',
							'descricao' => 'Universidade'
						),
						'entnome' => array(
							'codigo'    => 'entnome',
							'descricao' => 'Campus'
						),
						'entsig' => array(
							'codigo'    => 'entsig',
							'descricao' => 'Sigla da unidade'
						),
						'exidsc' => array(
							'codigo'    => 'exidsc',
							'descricao' => 'Existência'
						),
						'cmpsituacao' => array(
							'codigo'    => 'cmpsituacao',
							'descricao' => 'Situação'
						),
						'cmpinstalacao' => array(
							'codigo'    => 'cmpinstalacao',
							'descricao' => 'Instalações'
						),
						'obrascampus' => array(
							'codigo'    => 'obrascampus',
							'descricao' => 'Obras no campus'
						),
						'docenteprevisto' => array(
							'codigo'    => 'docenteprevisto',
							'descricao' => 'DOCENTE (PREVISTO)'
						),
						'tecnicoprevisto' => array(
							'codigo'    => 'tecnicoprevisto',
							'descricao' => 'TÉCNICO (PREVISTO)'
						),
						'matprevisto' => array(
							'codigo'    => 'matprevisto',
							'descricao' => 'MAT (PREVISTO)'
						),
						'vagasprevisto' => array(
							'codigo'    => 'vagasprevisto',
							'descricao' => 'VAGAS (PREVISTO)'
						),
						'cursoprevisto' => array(
							'codigo'    => 'cursoprevisto',
							'descricao' => 'CURSO (PREVISTO)'
						),
						'investimentoprevisto' => array(
							'codigo'    => 'investimentoprevisto',
							'descricao' => 'INVESTIMENTO (PREVISTO)'
						),
						'bolsasmestradoprevisto' => array(
							'codigo'    => 'bolsasmestradoprevisto',
							'descricao' => 'BOLSAS DE MESTRADO (PREVISTO)'
						),
						'bolsasdoutoradoprevisto' => array(
							'codigo'    => 'bolsasdoutoradoprevisto',
							'descricao' => 'BOLSAS DE DOUTORADO (PREVISTO)'
						),
						'bolsasposdoutoradoprevisto' => array(
							'codigo'    => 'bolsasposdoutoradoprevisto',
							'descricao' => 'BOLSAS DE PÓS-DOUTORADO (PREVISTO)'
						),
						'docenterealizado' => array(
							'codigo'    => 'docenterealizado',
							'descricao' => 'DOCENTE (REALIZADO)'
						),
						'tecnicorealizado' => array(
							'codigo'    => 'tecnicorealizado',
							'descricao' => 'TÉCNICO (REALIZADO)'
						),
//						'matrealizado' => array(
//							'codigo'    => 'matrealizado',
//							'descricao' => 'MAT (REALIZADO)'
//						),
						'vagasrealizado' => array(
							'codigo'    => 'vagasrealizado',
							'descricao' => 'VAGAS (REALIZADO)'
						),
						'cursorealizado' => array(
							'codigo'    => 'cursorealizado',
							'descricao' => 'CURSO (REALIZADO)'
						),
						'investimentorealizado' => array(
							'codigo'    => 'investimentorealizado',
							'descricao' => 'INVESTIMENTO (REALIZADO)'
						),
						'bolsasmestradorealizado' => array(
							'codigo'    => 'bolsasmestradorealizado',
							'descricao' => 'BOLSAS DE MESTRADO (REALIZADO)'
						),
						'bolsasdoutoradorealizado' => array(
							'codigo'    => 'bolsasdoutoradorealizado',
							'descricao' => 'BOLSAS DE DOUTORADO (REALIZADO)'
						),
						'bolsasposdoutoradorealizado' => array(
							'codigo'    => 'bolsasposdoutoradorealizado',
							'descricao' => 'BOLSAS DE PÓS-DOUTORADO (REALIZADO)'
						),
						'matriculasgradpresen' => array(
							'codigo'    => 'matriculasgradpresen',
							'descricao' => 'MATRÍCULAS (GRADUAÇÃO PRESENCIAL)'
						),
						'datacriacao' => array(
							'codigo'    => 'datacriacao',
							'descricao' => 'Data de criação'
						),
						'cptdsc' => array(
							'codigo'    => 'cptdsc',
							'descricao' => 'Tipo'
						),
						'cmpdataimplantacao' => array(
							'codigo'    => 'cmpdataimplantacao',
							'descricao' => 'Data do início das atividades'
						),
						'cmpdatainauguracao' => array(
							'codigo'    => 'cmpdatainauguracao',
							'descricao' => 'Data de Inauguração'
						),
						'insvtot' => array(
							'codigo'    => 'insvtot',
							'descricao' => 'Investimento total'
						),
						'dirigentecampus' => array(
							'codigo'    => 'dirigentecampus',
							'descricao' => 'Dirigente do campus'
						),
						'enderecocampus' => array(
							'codigo'    => 'enderecocampus',
							'descricao' => 'Endereço do Campus'
						),
						'enderecounidadeorc' => array(
								'codigo'    => 'enderecounidadeorc',
								'descricao' => 'Endereço da Universidade'
						),
						'emailcampus' => array(
							'codigo'    => 'emailcampus',
							'descricao' => 'E-mail do campus'
						),
						'telefonecampus' => array(
							'codigo'    => 'telefonecampus',
							'descricao' => 'Telefone do campus'
						),
						'latitude' => array(
							'codigo'    => 'latitude',
							'descricao' => 'Latitude'
						),
						'longitude' => array(
							'codigo'    => 'longitude',
							'descricao' => 'Longitude'
						),
						'insrealizado2005_2010' => array(
							'codigo'    => 'insrealizado2005_2010',
							'descricao' => 'Investimentos realizados 2005/2010'
						),
						'insprevisto2011_2014' => array(
							'codigo'    => 'insprevisto2011_2014',
							'descricao' => 'Investimentos previstos 2011/2014'
						),
						'insrealizadomaior2011' => array(
							'codigo'    => 'insrealizadomaior2011',
							'descricao' => 'Investimentos realizados a partir de 2011'
						)

				);


function mostrar_profissional($filtroSQL = false) {
	global $db, $destino;

	$sql =  "
            SELECT  edc.estuf as estuf,
                    mun.mundescricao,
                    mun.muncod,
                    ent.entid,
                    ent.entnome as entnome,
                    uor.entsig,
                    ex.exidsc,

                    CASE
                        WHEN cam.cmpsituacao = 'F' THEN 'Funcionando'
                        WHEN cam.cmpsituacao = 'N' THEN 'Não Funcionando'
                    END as cmpsituacao,

                    CASE
                        WHEN cam.cmpinstalacao = 'P' THEN 'Instalações Provisórias'
                        WHEN cam.cmpinstalacao = 'D' THEN 'Instalações Definitivas'
                        WHEN cam.cmpinstalacao = 'S' THEN 'Sem Instalações'
                    END as cmpinstalacao,

                    CASE
                        WHEN cam.cmpsituacaoobra = 'L' THEN 'Licitação de Obras'
                        WHEN cam.cmpsituacaoobra = 'A' THEN 'Obras em Andamento'
                        WHEN cam.cmpsituacaoobra = 'C' THEN 'Obras Concluídas'
                        WHEN cam.cmpsituacaoobra = 'E' THEN 'Elaboração de Projetos'
                        WHEN cam.cmpsituacaoobra = 'D' THEN 'Dominialidade de Imóvel'
                    END as cmpsituacaoobra,

                    (SELECT sum(dsh.dshqtde::integer) as matriculas FROM painel.seriehistorica sh INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                        WHERE sh.sehstatus <> 'I' AND sh.indid IN (2575) AND dpe.dpeanoref = '2005' AND entid = cam.entid) as ano2005,

                    (SELECT sum(dsh.dshqtde::integer) as matriculas FROM painel.seriehistorica sh INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                        WHERE sh.sehstatus <> 'I' AND sh.indid IN (2575) AND dpe.dpeanoref = '2006' AND entid = cam.entid) as ano2006,

                    (SELECT sum(dsh.dshqtde::integer) as matriculas FROM painel.seriehistorica sh INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                        WHERE sh.sehstatus <> 'I' AND sh.indid IN (2575) AND dpe.dpeanoref = '2007' AND entid = cam.entid) as ano2007,

                    (SELECT sum(dsh.dshqtde::integer) as matriculas FROM painel.seriehistorica sh INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                        WHERE sh.sehstatus <> 'I' AND sh.indid IN (2575) AND dpe.dpeanoref = '2008' AND entid = cam.entid) as ano2008,

                    (SELECT sum(dsh.dshqtde::integer) as matriculas FROM painel.seriehistorica sh INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                        WHERE sh.sehstatus <> 'I' AND sh.indid IN (2575) AND dpe.dpeanoref = '2009' AND entid = cam.entid) as ano2009,

                    (SELECT sum(dsh.dshqtde::integer) as matriculas FROM painel.seriehistorica sh INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                        WHERE sh.sehstatus <> 'I' AND sh.indid IN (2575) AND dpe.dpeanoref = '2010' AND entid = cam.entid) as ano2010,

                    (SELECT sum(dsh.dshqtde::integer) as matriculas FROM painel.seriehistorica sh INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                        WHERE sh.sehstatus <> 'I' AND sh.indid IN (2575) AND dpe.dpeanoref = '2011' AND entid = cam.entid) as ano2011,

                    (SELECT sum(dsh.dshqtde::integer) as matriculas FROM painel.seriehistorica sh INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                        WHERE sh.sehstatus <> 'I' AND sh.indid IN (2575) AND dpe.dpeanoref = '2012' AND entid = cam.entid) as ano2012,

                    (SELECT sum(dsh.dshqtde::integer) as matriculas FROM painel.seriehistorica sh INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                        WHERE sh.sehstatus <> 'I' AND sh.indid IN (2575) AND dpe.dpeanoref = '2013' AND entid = cam.entid) as ano2013,

 (SELECT sum(dsh.dshqtde::integer) as matriculas FROM painel.seriehistorica sh INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                        WHERE sh.sehstatus <> 'I' AND sh.indid IN (2575) AND dpe.dpeanoref = '2014' AND entid = cam.entid) as ano2014,
                        
                    to_char(cam.cmpdatainauguracao::date,'DD/MM/YYYY') as cmpdatainauguracao,

                    CASE WHEN cam.cmpdataimplantacao != ''
                        THEN substr(cam.cmpdataimplantacao, 5, 2) || '/' || substr(cam.cmpdataimplantacao, 1, 4)
                        ELSE ''
                    END as cmpdataimplantacao,

                    (SELECT trim(to_char(SUM(cpivalor), '999g999g999g999d99')) FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_INVS_PREVISTO_PROF."' AND cmi.cpitabnum=1) AS insprevisto,

                    (SELECT trim(to_char(SUM(cpivalor), '999g999g999g999d99')) FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_INVS_REALIZADO_PROF."' AND cmi.cpitabnum=1) AS insrealizado,

                    (SELECT trim(to_char(SUM(cpivalor), '999g999g999g999d99')) FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_INVS_REALIZADO_PROF."' AND cmi.cpiano BETWEEN '2005' AND '2010' AND cmi.cpitabnum=1) AS insrealizado2005_2010,

                    (SELECT trim(to_char(SUM(cpivalor), '999g999g999g999d99')) FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_INVS_PREVISTO_PROF."' AND cmi.cpiano BETWEEN '2011' AND '2014' AND cmi.cpitabnum=1) AS insprevisto2011_2014,

                    (SELECT trim(to_char(SUM(cpivalor), '999g999g999g999d99')) FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_INVS_REALIZADO_PROF."' AND cmi.cpiano >= '2011' AND cmi.cpitabnum=1) AS insrealizadomaior2011,

                    (SELECT CASE WHEN count(*) >= 1 THEN 'Sim' ELSE 'Não' END FROM obras.obrainfraestrutura oi WHERE stoid = 1 AND obsstatus = 'A' AND entidcampus = cam.entid) AS obrascampus,

                    CASE WHEN cam.datacriacao != ''
                        THEN substr(cam.datacriacao, 5, 2) || '/' || substr(cam.datacriacao, 1, 4)
                        ELSE ''
                    END as datacriacao,

                    ct.cptdsc,

                    (SELECT DISTINCT entnome FROM entidade.entidade etn JOIN entidade.funcaoentidade fnt ON etn.entid = fnt.entid AND fnt.funid = 24 AND fuestatus = 'A' JOIN entidade.funentassoc fea ON fea.fueid = fnt.fueid WHERE fea.entid = ent.entid AND entstatus = 'A' ) as dirigentecampus,

                    uor.entunicod,
                    uor.entnome as unidadeorc,
                    COALESCE(edc.endcep, '') as cepcampus,
                    COALESCE(edc.endbai, '') as bairrocampus,
                    COALESCE(edc.endlog, '') as enderecocampus,
                    COALESCE(edc.endnum, '') as numerocampus,
                    COALESCE(endo.endcep, '') as cepuniversidade,
                    COALESCE(endo.endbai, '') as bairrouniversidade,
                    COALESCE(endo.endlog, '') as enderecouniversidade,
                    COALESCE(endo.endnum, '') as numerouniversidade,
                    replace(ent.entemail, ';' ,' / ' ) as emailcampus,
                    '(' || COALESCE(ent.entnumdddcomercial, '') || ')' || COALESCE(ent.entnumcomercial, '') as telefonecampus,
                    edc.medlatitude as latitude,
                    edc.medlongitude as longitude

            FROM academico.campus cam
            
            LEFT JOIN academico.existencia ex ON ex.exiid = cam.exiid
            LEFT JOIN entidade.entidade ent ON ent.entid = cam.entid
            LEFT JOIN entidade.endereco edc ON edc.entid = ent.entid
            LEFT JOIN territorios.municipio mun ON mun.muncod = edc.muncod
            LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid
            LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid
            LEFT JOIN entidade.entidade uor ON uor.entid = fea.entid
            LEFT JOIN entidade.endereco endo ON endo.entid = uor.entid
            LEFT JOIN entidade.funcaoentidade fen2 ON fen2.entid = uor.entid
            LEFT JOIN academico.orgaouo tuo ON tuo.funid = fen2.funid
            LEFT JOIN academico.campustipo ct ON ct.cptid = cam.cptid

            WHERE ent.entstatus = 'A' " . ( ($filtroSQL) ? "AND " . implode(" AND ",$filtroSQL) : "" ) . "

            ORDER BY edc.estuf, uor.entsig, ent.entnome
        ";
       // ver($sql, d);
	$dadoscsv = $db->carregar($sql);

	if($dadoscsv[0]) {

		$colspan = false;
		$rowspan = false;
		if($_REQUEST['agrupador_'.$_REQUEST['orgid']]) {

			foreach($_REQUEST['agrupador_'.$_REQUEST['orgid']] as $col) {

				if($destino[$_REQUEST['orgid']][$col]['codigo'] == "matriculas") {

                                        $qntRegistros = count($_REQUEST['agrupador_2'])+9;
					$colspan = count($destino[$_REQUEST['orgid']])+2;
					$rowspan = 2;

				} else {

					if(!$colspan) $colspan = count($destino[$_REQUEST['orgid']]);
                                        if(!$qntRegistros) $qntRegistros = count($_REQUEST['agrupador_2']);
					if(!$rowspan) $rowspan = 1;
				}

			}
		}
		/*
		 * GERANDO CABEÇALHO EXCEL, CSV E HTML
		 */
		$cc=0;
		$xls = new GeraExcel();

		$html = "<script language=\"JavaScript\" src=\"../../includes/funcoes.js\"></script>
				 <link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/Estilo.css\"/>
				 <link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/listagem.css\"/>
				 <table class='tabela'><tr bgcolor=\"#C0C0C0\"><td align='center'>
						MINISTÉRIO DA EDUCAÇÃO<br>
						Responsável pela informação: Secretaria de Educação Profissional e Tecnológica (SETEC)<br>
						<br>
						Expansão da Educação Profissional e Tecnológica
						</td>
					</tr>
				 </table>
				 <table class='tabela'>";

//ver($_REQUEST['agrupador_'.$_REQUEST['orgid']],d);
		if($_REQUEST['agrupador_'.$_REQUEST['orgid']]) {

			$html .= "<tr>";

			foreach($_REQUEST['agrupador_'.$_REQUEST['orgid']] as $col) {

				if($destino[$_REQUEST['orgid']][$col]['codigo'] == "matriculas") {

					if(!$colspan) $colspan = count($destino[$_REQUEST['orgid']])+2;

					$html .= "<td colspan='10' class='SubTituloCentro'>MATRÍCULAS</td>";
					$htmlaux = "
						<tr>
							<td class='SubTituloCentro'>2005</td>
							<td class='SubTituloCentro'>2006</td>
							<td class='SubTituloCentro'>2007</td>
							<td class='SubTituloCentro'>2008</td>
							<td class='SubTituloCentro'>2009</td>
							<td class='SubTituloCentro'>2010</td>
							<td class='SubTituloCentro'>2011</td>
							<td class='SubTituloCentro'>2012</td>
							<td class='SubTituloCentro'>2013</td>
                                                        <td class='SubTituloCentro'>2014</td>
						</tr>";

                    if($_REQUEST['orgid'] == 2){
                        $cabecalhoTituloEspecifico = 'atendidas';
                    } else {
                        $cabecalhoTituloEspecifico = 'ofertadas';
                    }


					$xls->MontaConteudoString(0, $cc, "Matrículas $cabecalhoTituloEspecifico - Ano 2005");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "Matrículas $cabecalhoTituloEspecifico - Ano 2006");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "Matrículas $cabecalhoTituloEspecifico - Ano 2007");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "Matrículas $cabecalhoTituloEspecifico - Ano 2008");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "Matrículas $cabecalhoTituloEspecifico - Ano 2009");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "Matrículas $cabecalhoTituloEspecifico - Ano 2010");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "Matrículas $cabecalhoTituloEspecifico - Ano 2011");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "Matrículas $cabecalhoTituloEspecifico - Ano 2012");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "Matrículas $cabecalhoTituloEspecifico - Ano 2013");
					$cc++;
                                        $xls->MontaConteudoString(0, $cc, "Matrículas $cabecalhoTituloEspecifico - Ano 2014");
					$cc++;

                        		$csv .= "Matrículas ofertadas - Ano 2005;Matrículas ofertadas - Ano 2006;Matrículas ofertadas - Ano 2007;Matrículas ofertadas - Ano 2008;Matrículas ofertadas - Ano 2009;Matrículas ofertadas - Ano 2010;Matrículas ofertadas - Ano 2011;Matrículas ofertadas - Ano 2012;Matrículas ofertadas - Ano 2013;Matrículas ofertadas - Ano 2014".(($cc == $qntRegistros)?"\n":";");

				} elseif($destino[$_REQUEST['orgid']][$col]['codigo'] == "enderecocampus"){

					if(!$colspan) $colspan = count($destino[$_REQUEST['orgid']])+2;
                                        $qntRegistros = $qntRegistros + 3;

					$html .= "<td rowspan='".$rowspan."' class='SubTituloCentro'>CEP do Campus</td><td rowspan='".$rowspan."' class='SubTituloCentro'>Bairro do Campus</td><td rowspan='".$rowspan."' class='SubTituloCentro'>Endereço do Campus</td><td rowspan='".$rowspan."' class='SubTituloCentro'>Número do Campus</td>";

					$xls->MontaConteudoString(0, $cc, "CEP do Campus");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "Bairro do Campus");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "Endereço do Campus");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "Número do Campus");
					$cc++;

					$csv .= "CEP do Campus;Bairro do Campus;Endereço do Campus;Número do Campus".(($cc == $qntRegistros)?"\n":";");

				} elseif($destino[$_REQUEST['orgid']][$col]['codigo'] == "enderecounidadeorc"){

                                    $qntRegistros = $qntRegistros + 3;
                                    
					$html .= "<td rowspan='".$rowspan."' class='SubTituloCentro'>CEP do Instituto</td><td rowspan='".$rowspan."' class='SubTituloCentro'>Bairro do Instituto</td><td rowspan='".$rowspan."' class='SubTituloCentro'>Endereço do Instituto</td><td rowspan='".$rowspan."' class='SubTituloCentro'>Número do Instituto</td>";

					$xls->MontaConteudoString(0, $cc, "CEP do Instituto");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "Bairro do Instituto");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "Endereço do Instituto");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "Número do Instituto");
					$cc++;

					$csv .= "CEP do Instituto;Bairro do Instituto;Endereço do Instituto;Número do Instituto".(($cc == $qntRegistros)?"\n":";");

				} else {
					$html .= "<td rowspan='".$rowspan."' class='SubTituloCentro'>".$destino[$_REQUEST['orgid']][$col]['descricao']."</td>";

					$xls->MontaConteudoString(0, $cc, $destino[$_REQUEST['orgid']][$col]['descricao']);
					$cc++;
					$csv .= $destino[$_REQUEST['orgid']][$col]['descricao'].(($cc == $qntRegistros)?" \n":";");
                  }

			}
//ver($csv,d);

			$html .= "</tr>";

			if($htmlaux) {
				$html .= $htmlaux;
			}
		}
		/*
		 * FIM - GERANDO CABEÇALHO EXCEL, CSV E HTML
		 */


		$i=1;
		foreach($dadoscsv as $registro) {

			$cc=0;
			if (fmod($i,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';

			if($_REQUEST['agrupador_'.$_REQUEST['orgid']]) {

				$html .= "<tr bgcolor='".$marcado."'>";

				foreach($_REQUEST['agrupador_'.$_REQUEST['orgid']] as $col) {

					if($destino[$_REQUEST['orgid']][$col]['codigo'] == "matriculas") {

						$html .= "<td>".$registro['ano2005']."</td>
								  <td>".$registro['ano2006']."</td>
								  <td>".$registro['ano2007']."</td>
								  <td>".$registro['ano2008']."</td>
								  <td>".$registro['ano2009']."</td>
								  <td>".$registro['ano2010']."</td>
								  <td>".$registro['ano2011']."</td>
					 			  <td>".$registro['ano2012']."</td>
					 			  <td>".$registro['ano2013']."</td>
                                                                  <td>".$registro['ano2014']."</td>
					 			  ";

						$xls->MontaConteudoString($i, $cc, $registro['ano2005']);
						$cc++;
						$xls->MontaConteudoString($i, $cc, $registro['ano2006']);
						$cc++;
						$xls->MontaConteudoString($i, $cc, $registro['ano2007']);
						$cc++;
						$xls->MontaConteudoString($i, $cc, $registro['ano2008']);
						$cc++;
						$xls->MontaConteudoString($i, $cc, $registro['ano2009']);
						$cc++;
						$xls->MontaConteudoString($i, $cc, $registro['ano2010']);
						$cc++;
						$xls->MontaConteudoString($i, $cc, $registro['ano2011']);
						$cc++;
						$xls->MontaConteudoString($i, $cc, $registro['ano2012']);
						$cc++;
						$xls->MontaConteudoString($i, $cc, $registro['ano2013']);
						$cc++;
                                                $xls->MontaConteudoString($i, $cc, $registro['ano2014']);
						$cc++;

						$csv .= $registro['ano2005'].";".$registro['ano2006'].";".$registro['ano2007'].";".$registro['ano2008'].";".$registro['ano2009'].";".$registro['ano2010'].";".$registro['ano2011'].";".$registro['ano2012'].";".$registro['ano2013'].";".$registro['ano2014'].(($cc == $qntRegistros)?"\n":";");

					} elseif($destino[$_REQUEST['orgid']][$col]['codigo'] == "enderecocampus") {

						$tam = strlen( $registro['cepcampus'] );
                       	$cep = substr( $registro['cepcampus'], 0, 5 ) . '-' . substr( $registro['cepcampus'], 5, $tam );

						$html .= "<td>".$cep."</td>
								  <td>".$registro['bairrocampus']."</td>
					 			  <td>".$registro['enderecocampus']."</td>
					 			  <td>".$registro['numerocampus']."</td>";

						$xls->MontaConteudoString($i, $cc, $cep);
						$cc++;
						$xls->MontaConteudoString($i, $cc, $registro['bairrocampus']);
						$cc++;
						$xls->MontaConteudoString($i, $cc, $registro['enderecocampus']);
						$cc++;
						$xls->MontaConteudoString($i, $cc, $registro['numerocampus']);
						$cc++;

						$csv .= $cep.";".$registro['bairrocampus'].";".$registro['enderecocampus'].";".$registro['numerocampus'].(($cc == $qntRegistros)?"\n":";");

					} elseif($destino[$_REQUEST['orgid']][$col]['codigo'] == "enderecounidadeorc") {

						$tam = strlen( $registro['cepuniversidade'] );
                       	$cep = substr( $registro['cepuniversidade'], 0, 5 ) . '-' . substr( $registro['cepuniversidade'], 5, $tam );

						$html .= "<td>".$cep."</td>
								  <td>".$registro['bairrouniversidade']."</td>
					 			  <td>".$registro['enderecouniversidade']."</td>
					 			  <td>".$registro['numerouniversidade']."</td>";

						$xls->MontaConteudoString($i, $cc, $cep);
						$cc++;
						$xls->MontaConteudoString($i, $cc, $registro['bairrouniversidade']);
						$cc++;
						$xls->MontaConteudoString($i, $cc, $registro['enderecouniversidade']);
						$cc++;
						$xls->MontaConteudoString($i, $cc, $registro['numerouniversidade']);
						$cc++;

						$csv .= $cep.";".$registro['bairrouniversidade'].";".$registro['enderecouniversidade'].";".$registro['numerouniversidade'].(($cc == $qntRegistros)?"\n":";");

					} else {
						$html .= "<td>".$registro[$col]."</td>";

						$xls->MontaConteudoString($i, $cc, $registro[$col]);
						$cc++;
 
						$csv .= $registro[$col].(($cc == $qntRegistros)?"\n":";");

					}
				}
//ver($csv,d);
				$html .= "</tr>";
			}

			$i++;
		}

		$html .= "</table>";
		$msg = $_POST['obs_consulta'];
		//Roda pé em HTML: Observação feita pelo usuário na mantagem do relatório.
		$html .= "
			<table class='tabela'>
				<tr bgcolor=\"#C0C0C0\">
					<td colspan='27' align='center'>\n
						OBSERVAÇÃO: ".$msg."
					</td>
				</tr>
			</table>";

		//Roda pé em XLS: Observação feita pelo usuário na mantagem do relatório.
		$xls->MontaConteudoString($i+1, 0, 'OBSERVAÇÃO:');
		$xls->MontaConteudoString($i+1, 1, $msg);

		//Roda pé em CSV: Observação feita pelo usuário na mantagem do relatório.
		$csv .= "\n\n"."OBSERVAÇÃO: ".";".$msg;


	} else {

		$html = "<link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/Estilo.css\"/>
				<link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/listagem.css\"/>
				<table class='tabela'><tr bgcolor=\"#C0C0C0\">
					<tr>
						<td align='center'>MINISTÉRIO DA EDUCAÇÃO<br>Responsável pela informação: Secretaria de Educação Profissional e Tecnológica (SETEC)<br><br>Expansão da Educação Profissional e Tecnológica</td>
					</tr>
					<tr>
						<td style='color:#ff0000; text-align:center'>Não foram encontrados registros.</td>
					</tr>
				</table>";

	}


	$nome_arquivo = "relatorio_nota_tecnica_".date('d-m-Y_H_i').".csv";
	switch(substr($_POST['exibir'],10,3)) {
		case 'csv':
			header("Content-type: application/octet-stream");
			header("Content-Disposition: attachment; filename=\"$nome_arquivo\"");
			echo $csv;
			break;
		case 'xls':
			ob_clean();
			$xls->GeraArquivo();
			break;
		case 'htm':
			echo $html;
			break;

	}
	exit;
}
//fim da função


function mostrar_superior($filtroSQL = false) {

	global $db, $destino;

	$sql_campos = "";

	if(in_array('docenteprevisto', $_POST['agrupador_1'])){
		$sql_campos .= "-- Docente (Previsto)
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_DOCENTE_PREV_SUP."' AND cmi.cpiano = '2005' AND cmi.cpitabnum=1) AS docenteprevano2005,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_DOCENTE_PREV_SUP."' AND cmi.cpiano = '2006' AND cmi.cpitabnum=1) AS docenteprevano2006,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_DOCENTE_PREV_SUP."' AND cmi.cpiano = '2007' AND cmi.cpitabnum=1) AS docenteprevano2007,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_DOCENTE_PREV_SUP."' AND cmi.cpiano = '2008' AND cmi.cpitabnum=1) AS docenteprevano2008,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_DOCENTE_PREV_SUP."' AND cmi.cpiano = '2009' AND cmi.cpitabnum=1) AS docenteprevano2009,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_DOCENTE_PREV_SUP."' AND cmi.cpiano = '2010' AND cmi.cpitabnum=1) AS docenteprevano2010,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_DOCENTE_PREV_SUP."' AND cmi.cpiano = '2011' AND cmi.cpitabnum=1) AS docenteprevano2011,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_DOCENTE_PREV_SUP."' AND cmi.cpiano = '2012' AND cmi.cpitabnum=1) AS docenteprevano2012,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_DOCENTE_PREV_SUP."' AND cmi.cpiano = '2013' AND cmi.cpitabnum=1) AS docenteprevano2013,
				 ";
	}
	if(in_array('tecnicoprevisto', $_POST['agrupador_1'])){
		$sql_campos .= "-- Técnico (Previsto)
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_TECNICO_PREV_SUP."' AND cmi.cpiano = '2005' AND cmi.cpitabnum=1) AS tecnicoprevano2005,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_TECNICO_PREV_SUP."' AND cmi.cpiano = '2006' AND cmi.cpitabnum=1) AS tecnicoprevano2006,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_TECNICO_PREV_SUP."' AND cmi.cpiano = '2007' AND cmi.cpitabnum=1) AS tecnicoprevano2007,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_TECNICO_PREV_SUP."' AND cmi.cpiano = '2008' AND cmi.cpitabnum=1) AS tecnicoprevano2008,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_TECNICO_PREV_SUP."' AND cmi.cpiano = '2009' AND cmi.cpitabnum=1) AS tecnicoprevano2009,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_TECNICO_PREV_SUP."' AND cmi.cpiano = '2010' AND cmi.cpitabnum=1) AS tecnicoprevano2010,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_TECNICO_PREV_SUP."' AND cmi.cpiano = '2011' AND cmi.cpitabnum=1) AS tecnicoprevano2011,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_TECNICO_PREV_SUP."' AND cmi.cpiano = '2012' AND cmi.cpitabnum=1) AS tecnicoprevano2012,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_TECNICO_PREV_SUP."' AND cmi.cpiano = '2013' AND cmi.cpitabnum=1) AS tecnicoprevano2013,
				 ";
	}
	if(in_array('matprevisto', $_POST['agrupador_1'])){
		$sql_campos .= "-- Mat (Previsto)
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_MAT_PREV_SUP."' AND cmi.cpiano = '2005' AND cmi.cpitabnum=1) AS matprevano2005,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_MAT_PREV_SUP."' AND cmi.cpiano = '2006' AND cmi.cpitabnum=1) AS matprevano2006,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_MAT_PREV_SUP."' AND cmi.cpiano = '2007' AND cmi.cpitabnum=1) AS matprevano2007,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_MAT_PREV_SUP."' AND cmi.cpiano = '2008' AND cmi.cpitabnum=1) AS matprevano2008,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_MAT_PREV_SUP."' AND cmi.cpiano = '2009' AND cmi.cpitabnum=1) AS matprevano2009,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_MAT_PREV_SUP."' AND cmi.cpiano = '2010' AND cmi.cpitabnum=1) AS matprevano2010,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_MAT_PREV_SUP."' AND cmi.cpiano = '2011' AND cmi.cpitabnum=1) AS matprevano2011,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_MAT_PREV_SUP."' AND cmi.cpiano = '2012' AND cmi.cpitabnum=1) AS matprevano2012,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_MAT_PREV_SUP."' AND cmi.cpiano = '2013' AND cmi.cpitabnum=1) AS matprevano2013,
				 ";
	}
	if(in_array('vagasprevisto', $_POST['agrupador_1'])){
		$sql_campos .= "-- Vagas (Previsto)
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_VAGAS_PREV_SUP."' AND cmi.cpiano = '2005' AND cmi.cpitabnum=1) AS vagasprevano2005,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_VAGAS_PREV_SUP."' AND cmi.cpiano = '2006' AND cmi.cpitabnum=1) AS vagasprevano2006,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_VAGAS_PREV_SUP."' AND cmi.cpiano = '2007' AND cmi.cpitabnum=1) AS vagasprevano2007,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_VAGAS_PREV_SUP."' AND cmi.cpiano = '2008' AND cmi.cpitabnum=1) AS vagasprevano2008,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_VAGAS_PREV_SUP."' AND cmi.cpiano = '2009' AND cmi.cpitabnum=1) AS vagasprevano2009,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_VAGAS_PREV_SUP."' AND cmi.cpiano = '2010' AND cmi.cpitabnum=1) AS vagasprevano2010,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_VAGAS_PREV_SUP."' AND cmi.cpiano = '2011' AND cmi.cpitabnum=1) AS vagasprevano2011,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_VAGAS_PREV_SUP."' AND cmi.cpiano = '2012' AND cmi.cpitabnum=1) AS vagasprevano2012,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_VAGAS_PREV_SUP."' AND cmi.cpiano = '2013' AND cmi.cpitabnum=1) AS vagasprevano2013,
				 ";
	}
	if(in_array('cursoprevisto', $_POST['agrupador_1'])){
		$sql_campos .= "-- Cursos (Previsto)
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_CURSO_PREV_SUP."' AND cmi.cpiano = '2005' AND cmi.cpitabnum=1) AS cursoprevano2005,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_CURSO_PREV_SUP."' AND cmi.cpiano = '2006' AND cmi.cpitabnum=1) AS cursoprevano2006,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_CURSO_PREV_SUP."' AND cmi.cpiano = '2007' AND cmi.cpitabnum=1) AS cursoprevano2007,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_CURSO_PREV_SUP."' AND cmi.cpiano = '2008' AND cmi.cpitabnum=1) AS cursoprevano2008,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_CURSO_PREV_SUP."' AND cmi.cpiano = '2009' AND cmi.cpitabnum=1) AS cursoprevano2009,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_CURSO_PREV_SUP."' AND cmi.cpiano = '2010' AND cmi.cpitabnum=1) AS cursoprevano2010,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_CURSO_PREV_SUP."' AND cmi.cpiano = '2011' AND cmi.cpitabnum=1) AS cursoprevano2011,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_CURSO_PREV_SUP."' AND cmi.cpiano = '2012' AND cmi.cpitabnum=1) AS cursoprevano2012,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_CURSO_PREV_SUP."' AND cmi.cpiano = '2013' AND cmi.cpitabnum=1) AS cursoprevano2013,
				 ";
	}
	if(in_array('investimentoprevisto', $_POST['agrupador_1'])){
		$sql_campos .= "-- Investimentos (Previsto)
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_INVESTIMENTO_PREV_SUP."' AND cmi.cpiano = '2005' AND cmi.cpitabnum=1) AS investimentoprevano2005,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_INVESTIMENTO_PREV_SUP."' AND cmi.cpiano = '2006' AND cmi.cpitabnum=1) AS investimentoprevano2006,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_INVESTIMENTO_PREV_SUP."' AND cmi.cpiano = '2007' AND cmi.cpitabnum=1) AS investimentoprevano2007,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_INVESTIMENTO_PREV_SUP."' AND cmi.cpiano = '2008' AND cmi.cpitabnum=1) AS investimentoprevano2008,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_INVESTIMENTO_PREV_SUP."' AND cmi.cpiano = '2009' AND cmi.cpitabnum=1) AS investimentoprevano2009,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_INVESTIMENTO_PREV_SUP."' AND cmi.cpiano = '2010' AND cmi.cpitabnum=1) AS investimentoprevano2010,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_INVESTIMENTO_PREV_SUP."' AND cmi.cpiano = '2011' AND cmi.cpitabnum=1) AS investimentoprevano2011,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_INVESTIMENTO_PREV_SUP."' AND cmi.cpiano = '2012' AND cmi.cpitabnum=1) AS investimentoprevano2012,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_INVESTIMENTO_PREV_SUP."' AND cmi.cpiano = '2013' AND cmi.cpitabnum=1) AS investimentoprevano2013,
				 ";
	}
	if(in_array('bolsasmestradoprevisto', $_POST['agrupador_1'])){
		$sql_campos .= "-- BOLSAS DE MESTRADO (Previsto)
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_MESTRADO_PREV_SUP."' AND cmi.cpiano = '2005' AND cmi.cpitabnum=1) AS bolsasmestradoprevano2005,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_MESTRADO_PREV_SUP."' AND cmi.cpiano = '2006' AND cmi.cpitabnum=1) AS bolsasmestradoprevano2006,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_MESTRADO_PREV_SUP."' AND cmi.cpiano = '2007' AND cmi.cpitabnum=1) AS bolsasmestradoprevano2007,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_MESTRADO_PREV_SUP."' AND cmi.cpiano = '2008' AND cmi.cpitabnum=1) AS bolsasmestradoprevano2008,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_MESTRADO_PREV_SUP."' AND cmi.cpiano = '2009' AND cmi.cpitabnum=1) AS bolsasmestradoprevano2009,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_MESTRADO_PREV_SUP."' AND cmi.cpiano = '2010' AND cmi.cpitabnum=1) AS bolsasmestradoprevano2010,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_MESTRADO_PREV_SUP."' AND cmi.cpiano = '2011' AND cmi.cpitabnum=1) AS bolsasmestradoprevano2011,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_MESTRADO_PREV_SUP."' AND cmi.cpiano = '2012' AND cmi.cpitabnum=1) AS bolsasmestradoprevano2012,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_MESTRADO_PREV_SUP."' AND cmi.cpiano = '2013' AND cmi.cpitabnum=1) AS bolsasmestradoprevano2013,
				 ";
	}
	if(in_array('bolsasdoutoradoprevisto', $_POST['agrupador_1'])){
		$sql_campos .= "-- BOLSAS DE DOUTORADO (Previsto)
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_DOUTORADO_PREV_SUP."' AND cmi.cpiano = '2005' AND cmi.cpitabnum=1) AS bolsasdoutoradoprevano2005,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_DOUTORADO_PREV_SUP."' AND cmi.cpiano = '2006' AND cmi.cpitabnum=1) AS bolsasdoutoradoprevano2006,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_DOUTORADO_PREV_SUP."' AND cmi.cpiano = '2007' AND cmi.cpitabnum=1) AS bolsasdoutoradoprevano2007,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_DOUTORADO_PREV_SUP."' AND cmi.cpiano = '2008' AND cmi.cpitabnum=1) AS bolsasdoutoradoprevano2008,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_DOUTORADO_PREV_SUP."' AND cmi.cpiano = '2009' AND cmi.cpitabnum=1) AS bolsasdoutoradoprevano2009,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_DOUTORADO_PREV_SUP."' AND cmi.cpiano = '2010' AND cmi.cpitabnum=1) AS bolsasdoutoradoprevano2010,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_DOUTORADO_PREV_SUP."' AND cmi.cpiano = '2011' AND cmi.cpitabnum=1) AS bolsasdoutoradoprevano2011,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_DOUTORADO_PREV_SUP."' AND cmi.cpiano = '2012' AND cmi.cpitabnum=1) AS bolsasdoutoradoprevano2012,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_DOUTORADO_PREV_SUP."' AND cmi.cpiano = '2013' AND cmi.cpitabnum=1) AS bolsasdoutoradoprevano2013,
				 ";
	}
	if(in_array('bolsasposdoutoradoprevisto', $_POST['agrupador_1'])){
		$sql_campos .= "-- BOLSAS DE PÓS-DOUTORADO (Previsto)
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_POSDOUTORADO_PREV_SUP."' AND cmi.cpiano = '2005' AND cmi.cpitabnum=1) AS bolsasposdoutoradoprevano2005,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_POSDOUTORADO_PREV_SUP."' AND cmi.cpiano = '2006' AND cmi.cpitabnum=1) AS bolsasposdoutoradoprevano2006,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_POSDOUTORADO_PREV_SUP."' AND cmi.cpiano = '2007' AND cmi.cpitabnum=1) AS bolsasposdoutoradoprevano2007,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_POSDOUTORADO_PREV_SUP."' AND cmi.cpiano = '2008' AND cmi.cpitabnum=1) AS bolsasposdoutoradoprevano2008,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_POSDOUTORADO_PREV_SUP."' AND cmi.cpiano = '2009' AND cmi.cpitabnum=1) AS bolsasposdoutoradoprevano2009,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_POSDOUTORADO_PREV_SUP."' AND cmi.cpiano = '2010' AND cmi.cpitabnum=1) AS bolsasposdoutoradoprevano2010,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_POSDOUTORADO_PREV_SUP."' AND cmi.cpiano = '2011' AND cmi.cpitabnum=1) AS bolsasposdoutoradoprevano2011,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_POSDOUTORADO_PREV_SUP."' AND cmi.cpiano = '2012' AND cmi.cpitabnum=1) AS bolsasposdoutoradoprevano2012,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_POSDOUTORADO_PREV_SUP."' AND cmi.cpiano = '2013' AND cmi.cpitabnum=1) AS bolsasposdoutoradoprevano2013,
				 ";
	}
	if(in_array('docenterealizado', $_POST['agrupador_1'])){
		$sql_campos .= "-- Docente (Realizado)
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_DOCENTE_REALIZ_SUP."' AND cmi.cpiano = '2005' AND cmi.cpitabnum=1) AS docenterealizano2005,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_DOCENTE_REALIZ_SUP."' AND cmi.cpiano = '2006' AND cmi.cpitabnum=1) AS docenterealizano2006,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_DOCENTE_REALIZ_SUP."' AND cmi.cpiano = '2007' AND cmi.cpitabnum=1) AS docenterealizano2007,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_DOCENTE_REALIZ_SUP."' AND cmi.cpiano = '2008' AND cmi.cpitabnum=1) AS docenterealizano2008,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_DOCENTE_REALIZ_SUP."' AND cmi.cpiano = '2009' AND cmi.cpitabnum=1) AS docenterealizano2009,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_DOCENTE_REALIZ_SUP."' AND cmi.cpiano = '2010' AND cmi.cpitabnum=1) AS docenterealizano2010,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_DOCENTE_REALIZ_SUP."' AND cmi.cpiano = '2011' AND cmi.cpitabnum=1) AS docenterealizano2011,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_DOCENTE_REALIZ_SUP."' AND cmi.cpiano = '2012' AND cmi.cpitabnum=1) AS docenterealizano2012,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_DOCENTE_REALIZ_SUP."' AND cmi.cpiano = '2013' AND cmi.cpitabnum=1) AS docenterealizano2013,
				 ";
	}
	if(in_array('tecnicorealizado', $_POST['agrupador_1'])){
		$sql_campos .= "-- Técnico (Realizado)
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_TECNICO_REALIZ_SUP."' AND cmi.cpiano = '2005' AND cmi.cpitabnum=1) AS tecnicorealizano2005,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_TECNICO_REALIZ_SUP."' AND cmi.cpiano = '2006' AND cmi.cpitabnum=1) AS tecnicorealizano2006,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_TECNICO_REALIZ_SUP."' AND cmi.cpiano = '2007' AND cmi.cpitabnum=1) AS tecnicorealizano2007,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_TECNICO_REALIZ_SUP."' AND cmi.cpiano = '2008' AND cmi.cpitabnum=1) AS tecnicorealizano2008,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_TECNICO_REALIZ_SUP."' AND cmi.cpiano = '2009' AND cmi.cpitabnum=1) AS tecnicorealizano2009,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_TECNICO_REALIZ_SUP."' AND cmi.cpiano = '2010' AND cmi.cpitabnum=1) AS tecnicorealizano2010,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_TECNICO_REALIZ_SUP."' AND cmi.cpiano = '2011' AND cmi.cpitabnum=1) AS tecnicorealizano2011,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_TECNICO_REALIZ_SUP."' AND cmi.cpiano = '2012' AND cmi.cpitabnum=1) AS tecnicorealizano2012,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_TECNICO_REALIZ_SUP."' AND cmi.cpiano = '2013' AND cmi.cpitabnum=1) AS tecnicorealizano2013,
				 ";
	}
	if(in_array('matrealizado', $_POST['agrupador_1'])){
		$sql_campos .= "-- Mat (Realizado)
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_MAT_REALIZ_SUP."' AND cmi.cpiano = '2005' AND cmi.cpitabnum=1) AS matrealizano2005,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_MAT_REALIZ_SUP."' AND cmi.cpiano = '2006' AND cmi.cpitabnum=1) AS matrealizano2006,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_MAT_REALIZ_SUP."' AND cmi.cpiano = '2007' AND cmi.cpitabnum=1) AS matrealizano2007,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_MAT_REALIZ_SUP."' AND cmi.cpiano = '2008' AND cmi.cpitabnum=1) AS matrealizano2008,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_MAT_REALIZ_SUP."' AND cmi.cpiano = '2009' AND cmi.cpitabnum=1) AS matrealizano2009,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_MAT_REALIZ_SUP."' AND cmi.cpiano = '2010' AND cmi.cpitabnum=1) AS matrealizano2010,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_MAT_REALIZ_SUP."' AND cmi.cpiano = '2011' AND cmi.cpitabnum=1) AS matrealizano2011,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_MAT_REALIZ_SUP."' AND cmi.cpiano = '2012' AND cmi.cpitabnum=1) AS matrealizano2012,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_MAT_REALIZ_SUP."' AND cmi.cpiano = '2013' AND cmi.cpitabnum=1) AS matrealizano2013,
				 ";
	}
	if(in_array('vagasrealizado', $_POST['agrupador_1'])){
		$sql_campos .= "-- Vagas (Realizado)
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_VAGAS_REALIZ_SUP."' AND cmi.cpiano = '2005' AND cmi.cpitabnum=1) AS vagasrealizano2005,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_VAGAS_REALIZ_SUP."' AND cmi.cpiano = '2006' AND cmi.cpitabnum=1) AS vagasrealizano2006,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_VAGAS_REALIZ_SUP."' AND cmi.cpiano = '2007' AND cmi.cpitabnum=1) AS vagasrealizano2007,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_VAGAS_REALIZ_SUP."' AND cmi.cpiano = '2008' AND cmi.cpitabnum=1) AS vagasrealizano2008,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_VAGAS_REALIZ_SUP."' AND cmi.cpiano = '2009' AND cmi.cpitabnum=1) AS vagasrealizano2009,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_VAGAS_REALIZ_SUP."' AND cmi.cpiano = '2010' AND cmi.cpitabnum=1) AS vagasrealizano2010,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_VAGAS_REALIZ_SUP."' AND cmi.cpiano = '2011' AND cmi.cpitabnum=1) AS vagasrealizano2011,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_VAGAS_REALIZ_SUP."' AND cmi.cpiano = '2012' AND cmi.cpitabnum=1) AS vagasrealizano2012,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_VAGAS_REALIZ_SUP."' AND cmi.cpiano = '2013' AND cmi.cpitabnum=1) AS vagasrealizano2013,
				 ";
	}
	if(in_array('cursorealizado', $_POST['agrupador_1'])){
		$sql_campos .= "-- Cursos (Realizado)
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_CURSO_REALIZ_SUP."' AND cmi.cpiano = '2005' AND cmi.cpitabnum=1) AS cursorealizano2005,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_CURSO_REALIZ_SUP."' AND cmi.cpiano = '2006' AND cmi.cpitabnum=1) AS cursorealizano2006,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_CURSO_REALIZ_SUP."' AND cmi.cpiano = '2007' AND cmi.cpitabnum=1) AS cursorealizano2007,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_CURSO_REALIZ_SUP."' AND cmi.cpiano = '2008' AND cmi.cpitabnum=1) AS cursorealizano2008,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_CURSO_REALIZ_SUP."' AND cmi.cpiano = '2009' AND cmi.cpitabnum=1) AS cursorealizano2009,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_CURSO_REALIZ_SUP."' AND cmi.cpiano = '2010' AND cmi.cpitabnum=1) AS cursorealizano2010,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_CURSO_REALIZ_SUP."' AND cmi.cpiano = '2011' AND cmi.cpitabnum=1) AS cursorealizano2011,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_CURSO_REALIZ_SUP."' AND cmi.cpiano = '2012' AND cmi.cpitabnum=1) AS cursorealizano2012,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_CURSO_REALIZ_SUP."' AND cmi.cpiano = '2013' AND cmi.cpitabnum=1) AS cursorealizano2013,
				 ";
	}
	if(in_array('investimentorealizado', $_POST['agrupador_1'])){
		$sql_campos .= "-- Investimentos (Realizado)
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_INVESTIMENTO_REALIZ_SUP."' AND cmi.cpiano = '2005' AND cmi.cpitabnum=1) AS investimentorealizano2005,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_INVESTIMENTO_REALIZ_SUP."' AND cmi.cpiano = '2006' AND cmi.cpitabnum=1) AS investimentorealizano2006,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_INVESTIMENTO_REALIZ_SUP."' AND cmi.cpiano = '2007' AND cmi.cpitabnum=1) AS investimentorealizano2007,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_INVESTIMENTO_REALIZ_SUP."' AND cmi.cpiano = '2008' AND cmi.cpitabnum=1) AS investimentorealizano2008,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_INVESTIMENTO_REALIZ_SUP."' AND cmi.cpiano = '2009' AND cmi.cpitabnum=1) AS investimentorealizano2009,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_INVESTIMENTO_REALIZ_SUP."' AND cmi.cpiano = '2010' AND cmi.cpitabnum=1) AS investimentorealizano2010,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_INVESTIMENTO_REALIZ_SUP."' AND cmi.cpiano = '2011' AND cmi.cpitabnum=1) AS investimentorealizano2011,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_INVESTIMENTO_REALIZ_SUP."' AND cmi.cpiano = '2012' AND cmi.cpitabnum=1) AS investimentorealizano2012,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_INVESTIMENTO_REALIZ_SUP."' AND cmi.cpiano = '2013' AND cmi.cpitabnum=1) AS investimentorealizano2013,
				 ";
	}
	if(in_array('bolsasmestradorealizado', $_POST['agrupador_1'])){
		$sql_campos .= "-- BOLSAS DE MESTRADO (Realizado)
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_MESTRADO_REALIZ_SUP."' AND cmi.cpiano = '2005' AND cmi.cpitabnum=1) AS bolsasmestradorealizano2005,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_MESTRADO_REALIZ_SUP."' AND cmi.cpiano = '2006' AND cmi.cpitabnum=1) AS bolsasmestradorealizano2006,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_MESTRADO_REALIZ_SUP."' AND cmi.cpiano = '2007' AND cmi.cpitabnum=1) AS bolsasmestradorealizano2007,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_MESTRADO_REALIZ_SUP."' AND cmi.cpiano = '2008' AND cmi.cpitabnum=1) AS bolsasmestradorealizano2008,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_MESTRADO_REALIZ_SUP."' AND cmi.cpiano = '2009' AND cmi.cpitabnum=1) AS bolsasmestradorealizano2009,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_MESTRADO_REALIZ_SUP."' AND cmi.cpiano = '2010' AND cmi.cpitabnum=1) AS bolsasmestradorealizano2010,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_MESTRADO_REALIZ_SUP."' AND cmi.cpiano = '2011' AND cmi.cpitabnum=1) AS bolsasmestradorealizano2011,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_MESTRADO_REALIZ_SUP."' AND cmi.cpiano = '2012' AND cmi.cpitabnum=1) AS bolsasmestradorealizano2012,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_MESTRADO_REALIZ_SUP."' AND cmi.cpiano = '2013' AND cmi.cpitabnum=1) AS bolsasmestradorealizano2013,
				 ";
	}
	if(in_array('bolsasdoutoradorealizado', $_POST['agrupador_1'])){
		$sql_campos .= "-- BOLSAS DE DOUTORADO (Realizado)
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_DOUTORADO_REALIZ_SUP."' AND cmi.cpiano = '2005' AND cmi.cpitabnum=1) AS bolsasdoutoradorealizano2005,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_DOUTORADO_REALIZ_SUP."' AND cmi.cpiano = '2006' AND cmi.cpitabnum=1) AS bolsasdoutoradorealizano2006,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_DOUTORADO_REALIZ_SUP."' AND cmi.cpiano = '2007' AND cmi.cpitabnum=1) AS bolsasdoutoradorealizano2007,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_DOUTORADO_REALIZ_SUP."' AND cmi.cpiano = '2008' AND cmi.cpitabnum=1) AS bolsasdoutoradorealizano2008,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_DOUTORADO_REALIZ_SUP."' AND cmi.cpiano = '2009' AND cmi.cpitabnum=1) AS bolsasdoutoradorealizano2009,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_DOUTORADO_REALIZ_SUP."' AND cmi.cpiano = '2010' AND cmi.cpitabnum=1) AS bolsasdoutoradorealizano2010,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_DOUTORADO_REALIZ_SUP."' AND cmi.cpiano = '2011' AND cmi.cpitabnum=1) AS bolsasdoutoradorealizano2011,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_DOUTORADO_REALIZ_SUP."' AND cmi.cpiano = '2012' AND cmi.cpitabnum=1) AS bolsasdoutoradorealizano2012,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_DOUTORADO_REALIZ_SUP."' AND cmi.cpiano = '2013' AND cmi.cpitabnum=1) AS bolsasdoutoradorealizano2013,
				 ";
	}
	if(in_array('bolsasposdoutoradorealizado', $_POST['agrupador_1'])){
		$sql_campos .= "-- BOLSAS DE PÓS-DOUTORADO (Realizado)
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_POSDOUTORADO_REALIZ_SUP."' AND cmi.cpiano = '2005' AND cmi.cpitabnum=1) AS bolsasposdoutoradorealizano2005,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_POSDOUTORADO_REALIZ_SUP."' AND cmi.cpiano = '2006' AND cmi.cpitabnum=1) AS bolsasposdoutoradorealizano2006,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_POSDOUTORADO_REALIZ_SUP."' AND cmi.cpiano = '2007' AND cmi.cpitabnum=1) AS bolsasposdoutoradorealizano2007,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_POSDOUTORADO_REALIZ_SUP."' AND cmi.cpiano = '2008' AND cmi.cpitabnum=1) AS bolsasposdoutoradorealizano2008,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_POSDOUTORADO_REALIZ_SUP."' AND cmi.cpiano = '2009' AND cmi.cpitabnum=1) AS bolsasposdoutoradorealizano2009,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_POSDOUTORADO_REALIZ_SUP."' AND cmi.cpiano = '2010' AND cmi.cpitabnum=1) AS bolsasposdoutoradorealizano2010,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_POSDOUTORADO_REALIZ_SUP."' AND cmi.cpiano = '2011' AND cmi.cpitabnum=1) AS bolsasposdoutoradorealizano2011,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_POSDOUTORADO_REALIZ_SUP."' AND cmi.cpiano = '2012' AND cmi.cpitabnum=1) AS bolsasposdoutoradorealizano2012,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_BOLSAS_POSDOUTORADO_REALIZ_SUP."' AND cmi.cpiano = '2013' AND cmi.cpitabnum=1) AS bolsasposdoutoradorealizano2013,
				 ";
	}
	if(in_array('matriculasgradpresen', $_POST['agrupador_1'])){
		$sql_campos .= "-- Matrículas (Graduação Presencial)
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_MATRICULAS_GRADUACAO_PRESENCIAL_SUP."' AND cmi.cpiano = '2005' AND cmi.cpitabnum=1) AS matriculasgradpresenano2005,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_MATRICULAS_GRADUACAO_PRESENCIAL_SUP."' AND cmi.cpiano = '2006' AND cmi.cpitabnum=1) AS matriculasgradpresenano2006,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_MATRICULAS_GRADUACAO_PRESENCIAL_SUP."' AND cmi.cpiano = '2007' AND cmi.cpitabnum=1) AS matriculasgradpresenano2007,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_MATRICULAS_GRADUACAO_PRESENCIAL_SUP."' AND cmi.cpiano = '2008' AND cmi.cpitabnum=1) AS matriculasgradpresenano2008,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_MATRICULAS_GRADUACAO_PRESENCIAL_SUP."' AND cmi.cpiano = '2009' AND cmi.cpitabnum=1) AS matriculasgradpresenano2009,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_MATRICULAS_GRADUACAO_PRESENCIAL_SUP."' AND cmi.cpiano = '2010' AND cmi.cpitabnum=1) AS matriculasgradpresenano2010,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_MATRICULAS_GRADUACAO_PRESENCIAL_SUP."' AND cmi.cpiano = '2011' AND cmi.cpitabnum=1) AS matriculasgradpresenano2011,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_MATRICULAS_GRADUACAO_PRESENCIAL_SUP."' AND cmi.cpiano = '2012' AND cmi.cpitabnum=1) AS matriculasgradpresenano2012,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_MATRICULAS_GRADUACAO_PRESENCIAL_SUP."' AND cmi.cpiano = '2013' AND cmi.cpitabnum=1) AS matriculasgradpresenano2013,
				 ";
	}

	$sql =  "SELECT edc.estuf,
					mun.mundescricao,
					mun.muncod,
					ent.entid,
					ent.entnome,
					uor.entsig,
					ex.exidsc,
					CASE
						WHEN cam.cmpsituacao = 'F' THEN 'Funcionando'
						WHEN cam.cmpsituacao = 'N' THEN 'Não Funcionando'
				 		END as cmpsituacao,
					CASE
						WHEN cam.cmpinstalacao = 'P' THEN 'Instalações Provisórias'
						WHEN cam.cmpinstalacao = 'D' THEN 'Instalações Definitivas'
						WHEN cam.cmpinstalacao = 'S' THEN 'Sem Instalações'
						END as cmpinstalacao,
					CASE
						WHEN cam.cmpsituacaoobra = 'L' THEN 'Licitação de Obras'
						WHEN cam.cmpsituacaoobra = 'A' THEN 'Obras em Andamento'
						WHEN cam.cmpsituacaoobra = 'C' THEN 'Obras Concluídas'
						END as cmpsituacaoobra,
				 $sql_campos
				 to_char(cam.cmpdatainauguracao::date,'DD/MM/YYYY') as cmpdatainauguracao,
				 CASE WHEN cam.cmpdataimplantacao != '' THEN substr(cam.cmpdataimplantacao, 5, 2) || '/' || substr(cam.cmpdataimplantacao, 1, 4) ELSE '' END as cmpdataimplantacao,
				 (SELECT trim(to_char(SUM(cpivalor), '999g999g999g999d99')) FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_INVESTIMENTO_PREV_SUP."' AND cmi.cpitabnum=1) AS insvtot,
				 (SELECT trim(to_char(SUM(cpivalor), '999g999g999g999d99')) FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_INVESTIMENTO_REALIZ_SUP."' AND cmi.cpiano BETWEEN '2005' AND '2010' AND cmi.cpitabnum=1) AS insrealizado2005_2010,
				 (SELECT trim(to_char(SUM(cpivalor), '999g999g999g999d99')) FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_INVESTIMENTO_PREV_SUP."' AND cmi.cpiano BETWEEN '2011' AND '2014' AND cmi.cpitabnum=1) AS insprevisto2011_2014,
				 (SELECT trim(to_char(SUM(cpivalor), '999g999g999g999d99')) FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_INVESTIMENTO_REALIZ_SUP."' AND cmi.cpiano >= '2011' AND cmi.cpitabnum=1) AS insrealizadomaior2011,
				 (SELECT CASE WHEN count(*) >= 1 THEN 'Sim' ELSE 'Não' END FROM obras.obrainfraestrutura oi WHERE stoid = 1 AND obsstatus = 'A' AND entidcampus = cam.entid) AS obrascampus,
				 CASE WHEN cam.datacriacao != '' THEN substr(cam.datacriacao, 5, 2) || '/' || substr(cam.datacriacao, 1, 4) ELSE '' END as datacriacao,
				 ct.cptdsc,
				 (SELECT entnome FROM entidade.entidade etn
				  INNER JOIN entidade.funcaoentidade fnt ON etn.entid = fnt.entid AND fnt.funid=24
				  INNER JOIN entidade.funentassoc fea ON fea.fueid = fnt.fueid
				  WHERE fea.entid=ent.entid) as dirigentecampus,
				  uor.entunicod,
				  uor.entnome as unidadeorc,
				  COALESCE(edc.endcep, '') as cepcampus,
				  COALESCE(edc.endbai, '') as bairrocampus,
				  COALESCE(edc.endlog, '') as enderecocampus,
				  COALESCE(edc.endnum, '') as numerocampus,
				  COALESCE(endo.endcep, '') as cepuniversidade,
				  COALESCE(endo.endbai, '') as bairrouniversidade,
				  COALESCE(endo.endlog, '') as enderecouniversidade,
				  COALESCE(endo.endnum, '') as numerouniversidade,
				  ent.entemail as emailcampus,
				  '(' || COALESCE(ent.entnumdddcomercial, '') || ')' || COALESCE(ent.entnumcomercial, '') as telefonecampus
		  FROM academico.campus cam
		  	  LEFT JOIN academico.existencia ex ON ex.exiid = cam.exiid
			  LEFT JOIN entidade.entidade ent ON ent.entid = cam.entid
			  LEFT JOIN entidade.endereco edc ON edc.entid = ent.entid
			  LEFT JOIN territorios.municipio mun ON mun.muncod = edc.muncod
			  LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid
			  LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid
			  LEFT JOIN entidade.entidade uor ON uor.entid = fea.entid
			  LEFT JOIN entidade.endereco endo ON endo.entid = uor.entid
			  LEFT JOIN entidade.funcaoentidade fen2 ON fen2.entid = uor.entid
			  LEFT JOIN academico.orgaouo tuo ON tuo.funid = fen2.funid
			  LEFT JOIN academico.campustipo ct ON ct.cptid = cam.cptid
		  WHERE ent.entstatus = 'A' " . ( ($filtroSQL) ? "AND " . implode(" AND ",$filtroSQL) : "" ) . "
		  AND uor.entid NOT IN (388754) --Retirando a BAHIA
		  ORDER BY edc.estuf, uor.entsig, ent.entnome";
	$dadoscsv = $db->carregar($sql);

	if($dadoscsv[0]) {

		$colspan = false;
		$rowspan = false;
		if($_REQUEST['agrupador_'.$_REQUEST['orgid']]) {

			foreach($_REQUEST['agrupador_'.$_REQUEST['orgid']] as $col) {

				if($destino[$_REQUEST['orgid']][$col]['codigo'] == "matriculas") {

					$colspan = count($destino[$_REQUEST['orgid']])+3;
					$rowspan = 2;

				} else {

					if(!$colspan) $colspan = count($destino[$_REQUEST['orgid']]);
					if(!$rowspan) $rowspan = 1;
				}

			}
		}

		/*
		 * GERANDO CABEÇALHO EXCEL, CSV E HTML
		 */
		$cc=0;
		$xls = new GeraExcel();

		$html = "<script language=\"JavaScript\" src=\"../../includes/funcoes.js\"></script>
				 <link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/Estilo.css\"/>
				 <link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/listagem.css\"/>
				 <table class='tabela'><tr bgcolor=\"#C0C0C0\"><td align='center'>
						MINISTÉRIO DA EDUCAÇÃO<br>
						Responsável pela informação: Secretaria de Educação Profissional e Tecnológica (SETEC)<br>
						<br>
						Expansão da Educação Profissional e Tecnológica
						</td>
					</tr>
				 </table>
				 <table class='tabela'>";



		if($_REQUEST['agrupador_'.$_REQUEST['orgid']]) {

			$html .= "<tr>";
			//$htmlaux .= "<tr>";

			foreach($_REQUEST['agrupador_'.$_REQUEST['orgid']] as $col) {

				if($destino[$_REQUEST['orgid']][$col]['codigo'] == "docenteprevisto") {

					$html .= "<td colspan='9' class='SubTituloCentro'>DOCENTE (PREVISTO)</td>";
					$htmlaux = "<td class='SubTituloCentro'>2005</td><td class='SubTituloCentro'>2006</td><td class='SubTituloCentro'>2007</td><td class='SubTituloCentro'>2008</td><td class='SubTituloCentro'>2009</td><td class='SubTituloCentro'>2010</td><td class='SubTituloCentro'>2011</td><td class='SubTituloCentro'>2012</td><td class='SubTituloCentro'>2013</td>";

					$xls->MontaConteudoString(0, $cc, "DOCENTE (PREVISTO) - 2005");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "DOCENTE (PREVISTO) - 2006");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "DOCENTE (PREVISTO) - 2007");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "DOCENTE (PREVISTO) - 2008");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "DOCENTE (PREVISTO) - 2009");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "DOCENTE (PREVISTO) - 2010");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "DOCENTE (PREVISTO) - 2011");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "DOCENTE (PREVISTO) - 2012");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "DOCENTE (PREVISTO) - 2013");
					$cc++;

					$csv .= "DOCENTE (PREVISTO) - 2005;DOCENTE (PREVISTO) - 2006;DOCENTE (PREVISTO) - 2007;DOCENTE (PREVISTO) - 2008;DOCENTE (PREVISTO) - 2009;DOCENTE (PREVISTO) - 2010;DOCENTE (PREVISTO) - 2011;DOCENTE (PREVISTO) - 2012;DOCENTE (PREVISTO) - 2013".(($cc == $colspan)?"\n":";");

				} elseif($destino[$_REQUEST['orgid']][$col]['codigo'] == "tecnicoprevisto") {

					$html .= "<td colspan='9' class='SubTituloCentro'>TÉCNICO (PREVISTO)</td>";
					$htmlaux .= "<td class='SubTituloCentro'>2005</td><td class='SubTituloCentro'>2006</td><td class='SubTituloCentro'>2007</td><td class='SubTituloCentro'>2008</td><td class='SubTituloCentro'>2009</td><td class='SubTituloCentro'>2010</td><td class='SubTituloCentro'>2011</td><td class='SubTituloCentro'>2012</td><td class='SubTituloCentro'>2013</td>";

					$xls->MontaConteudoString(0, $cc, "TÉCNICO (PREVISTO) - 2005");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "TÉCNICO (PREVISTO) - 2006");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "TÉCNICO (PREVISTO) - 2007");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "TÉCNICO (PREVISTO) - 2008");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "TÉCNICO (PREVISTO) - 2009");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "TÉCNICO (PREVISTO) - 2010");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "TÉCNICO (PREVISTO) - 2011");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "TÉCNICO (PREVISTO) - 2012");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "TÉCNICO (PREVISTO) - 2013");
					$cc++;

					$csv .= "TÉCNICO (PREVISTO) - 2005;TÉCNICO (PREVISTO) - 2006;TÉCNICO (PREVISTO) - 2007;TÉCNICO (PREVISTO) - 2008;TÉCNICO (PREVISTO) - 2009;TÉCNICO (PREVISTO) - 2010;TÉCNICO (PREVISTO) - 2011;TÉCNICO (PREVISTO) - 2012;TÉCNICO (PREVISTO) - 2013".(($cc == $colspan)?"\n":";");

				} elseif($destino[$_REQUEST['orgid']][$col]['codigo'] == "matprevisto") {

					$html .= "<td colspan='9' class='SubTituloCentro'>MAT (PREVISTO)</td>";
					$htmlaux .= "<td class='SubTituloCentro'>2005</td><td class='SubTituloCentro'>2006</td><td class='SubTituloCentro'>2007</td><td class='SubTituloCentro'>2008</td><td class='SubTituloCentro'>2009</td><td class='SubTituloCentro'>2010</td><td class='SubTituloCentro'>2011</td><td class='SubTituloCentro'>2012</td><td class='SubTituloCentro'>2013</td>";

					$xls->MontaConteudoString(0, $cc, "MAT (PREVISTO) - 2005");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "MAT (PREVISTO) - 2006");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "MAT (PREVISTO) - 2007");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "MAT (PREVISTO) - 2008");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "MAT (PREVISTO) - 2009");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "MAT (PREVISTO) - 2010");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "MAT (PREVISTO) - 2011");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "MAT (PREVISTO) - 2012");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "MAT (PREVISTO) - 2013");
					$cc++;

					$csv .= "MAT (PREVISTO) - 2005;MAT (PREVISTO) - 2006;MAT (PREVISTO) - 2007;MAT (PREVISTO) - 2008;MAT (PREVISTO) - 2009;MAT (PREVISTO) - 2010;MAT (PREVISTO) - 2011;MAT (PREVISTO) - 2012;MAT (PREVISTO) - 2013".(($cc == $colspan)?"\n":";");

				} elseif($destino[$_REQUEST['orgid']][$col]['codigo'] == "vagasprevisto") {

					$html .= "<td colspan='9' class='SubTituloCentro'>VAGAS (PREVISTO)</td>";
					$htmlaux .= "<td class='SubTituloCentro'>2005</td><td class='SubTituloCentro'>2006</td><td class='SubTituloCentro'>2007</td><td class='SubTituloCentro'>2008</td><td class='SubTituloCentro'>2009</td><td class='SubTituloCentro'>2010</td><td class='SubTituloCentro'>2011</td><td class='SubTituloCentro'>2013</td><td class='SubTituloCentro'>2013</td>";

					$xls->MontaConteudoString(0, $cc, "VAGAS (PREVISTO) - 2005");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "VAGAS (PREVISTO) - 2006");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "VAGAS (PREVISTO) - 2007");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "VAGAS (PREVISTO) - 2008");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "VAGAS (PREVISTO) - 2009");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "VAGAS (PREVISTO) - 2010");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "VAGAS (PREVISTO) - 2011");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "VAGAS (PREVISTO) - 2012");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "VAGAS (PREVISTO) - 2013");
					$cc++;

					$csv .= "VAGAS (PREVISTO) - 2005;VAGAS (PREVISTO) - 2006;VAGAS (PREVISTO) - 2007;VAGAS (PREVISTO) - 2008;VAGAS (PREVISTO) - 2009;VAGAS (PREVISTO) - 2010;VAGAS (PREVISTO) - 2011;VAGAS (PREVISTO) - 2012;VAGAS (PREVISTO) - 2013".(($cc == $colspan)?"\n":";");

				} elseif($destino[$_REQUEST['orgid']][$col]['codigo'] == "cursoprevisto") {

					$html .= "<td colspan='9' class='SubTituloCentro'>CURSO (PREVISTO)</td>";
					$htmlaux .= "<td class='SubTituloCentro'>2005</td><td class='SubTituloCentro'>2006</td><td class='SubTituloCentro'>2007</td><td class='SubTituloCentro'>2008</td><td class='SubTituloCentro'>2009</td><td class='SubTituloCentro'>2010</td><td class='SubTituloCentro'>2011</td><td class='SubTituloCentro'>2012</td><td class='SubTituloCentro'>2013</td>";

					$xls->MontaConteudoString(0, $cc, "CURSO (PREVISTO) - 2005");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "CURSO (PREVISTO) - 2006");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "CURSO (PREVISTO) - 2007");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "CURSO (PREVISTO) - 2008");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "CURSO (PREVISTO) - 2009");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "CURSO (PREVISTO) - 2010");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "CURSO (PREVISTO) - 2011");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "CURSO (PREVISTO) - 2012");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "CURSO (PREVISTO) - 2013");
					$cc++;

					$csv .= "CURSO (PREVISTO) - 2005;CURSO (PREVISTO) - 2006;CURSO (PREVISTO) - 2007;CURSO (PREVISTO) - 2008;CURSO (PREVISTO) - 2009;CURSO (PREVISTO) - 2010;CURSO (PREVISTO) - 2011;CURSO (PREVISTO) - 2012;CURSO (PREVISTO) - 2013".(($cc == $colspan)?"\n":";");

				} elseif($destino[$_REQUEST['orgid']][$col]['codigo'] == "investimentoprevisto") {

					$html .= "<td colspan='9' class='SubTituloCentro'>INVESTIMENTO (PREVISTO)</td>";
					$htmlaux .= "<td class='SubTituloCentro'>2005</td><td class='SubTituloCentro'>2006</td><td class='SubTituloCentro'>2007</td><td class='SubTituloCentro'>2008</td><td class='SubTituloCentro'>2009</td><td class='SubTituloCentro'>2010</td><td class='SubTituloCentro'>2011</td><td class='SubTituloCentro'>2012</td><td class='SubTituloCentro'>2013</td>";

					$xls->MontaConteudoString(0, $cc, "INVESTIMENTO (PREVISTO) - 2005");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "INVESTIMENTO (PREVISTO) - 2006");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "INVESTIMENTO (PREVISTO) - 2007");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "INVESTIMENTO (PREVISTO) - 2008");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "INVESTIMENTO (PREVISTO) - 2009");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "INVESTIMENTO (PREVISTO) - 2010");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "INVESTIMENTO (PREVISTO) - 2011");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "INVESTIMENTO (PREVISTO) - 2012");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "INVESTIMENTO (PREVISTO) - 2013");
					$cc++;

					$csv .= "INVESTIMENTO (PREVISTO) - 2005;INVESTIMENTO (PREVISTO) - 2006;INVESTIMENTO (PREVISTO) - 2007;INVESTIMENTO (PREVISTO) - 2008;INVESTIMENTO (PREVISTO) - 2009;INVESTIMENTO (PREVISTO) - 2010;INVESTIMENTO (PREVISTO) - 2011;INVESTIMENTO (PREVISTO) - 2012;INVESTIMENTO (PREVISTO) - 2013".(($cc == $colspan)?"\n":";");

				} elseif($destino[$_REQUEST['orgid']][$col]['codigo'] == "bolsasmestradoprevisto") {

					$html .= "<td colspan='9' class='SubTituloCentro'>BOLSAS DE MESTRADO (PREVISTO)</td>";
					$htmlaux .= "<td class='SubTituloCentro'>2005</td><td class='SubTituloCentro'>2006</td><td class='SubTituloCentro'>2007</td><td class='SubTituloCentro'>2008</td><td class='SubTituloCentro'>2009</td><td class='SubTituloCentro'>2010</td><td class='SubTituloCentro'>2011</td><td class='SubTituloCentro'>2012</td><td class='SubTituloCentro'>2013</td>";

					$xls->MontaConteudoString(0, $cc, "BOLSAS DE MESTRADO (PREVISTO) - 2005");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "BOLSAS DE MESTRADO (PREVISTO) - 2006");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "BOLSAS DE MESTRADO (PREVISTO) - 2007");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "BOLSAS DE MESTRADO (PREVISTO) - 2008");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "BOLSAS DE MESTRADO (PREVISTO) - 2009");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "BOLSAS DE MESTRADO (PREVISTO) - 2010");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "BOLSAS DE MESTRADO (PREVISTO) - 2011");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "BOLSAS DE MESTRADO (PREVISTO) - 2012");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "BOLSAS DE MESTRADO (PREVISTO) - 2013");
					$cc++;

					$csv .= "BOLSAS DE MESTRADO (PREVISTO) - 2005;BOLSAS DE MESTRADO (PREVISTO) - 2006;BOLSAS DE MESTRADO (PREVISTO) - 2007;BOLSAS DE MESTRADO (PREVISTO) - 2008;BOLSAS DE MESTRADO (PREVISTO) - 2009;BOLSAS DE MESTRADO (PREVISTO) - 2010;BOLSAS DE MESTRADO (PREVISTO) - 2011;BOLSAS DE MESTRADO (PREVISTO) - 2012;BOLSAS DE MESTRADO (PREVISTO) - 2013".(($cc == $colspan)?"\n":";");

				} elseif($destino[$_REQUEST['orgid']][$col]['codigo'] == "bolsasdoutoradoprevisto") {

					$html .= "<td colspan='9' class='SubTituloCentro'>BOLSAS DE DOUTORADO (PREVISTO)</td>";
					$htmlaux .= "<td class='SubTituloCentro'>2005</td><td class='SubTituloCentro'>2006</td><td class='SubTituloCentro'>2007</td><td class='SubTituloCentro'>2008</td><td class='SubTituloCentro'>2009</td><td class='SubTituloCentro'>2010</td><td class='SubTituloCentro'>2011</td><td class='SubTituloCentro'>2012</td><td class='SubTituloCentro'>2013</td>";

					$xls->MontaConteudoString(0, $cc, "BOLSAS DE DOUTORADO (PREVISTO) - 2005");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "BOLSAS DE DOUTORADO (PREVISTO) - 2006");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "BOLSAS DE DOUTORADO (PREVISTO) - 2007");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "BOLSAS DE DOUTORADO (PREVISTO) - 2008");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "BOLSAS DE DOUTORADO (PREVISTO) - 2009");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "BOLSAS DE DOUTORADO (PREVISTO) - 2010");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "BOLSAS DE DOUTORADO (PREVISTO) - 2011");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "BOLSAS DE DOUTORADO (PREVISTO) - 2012");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "BOLSAS DE DOUTORADO (PREVISTO) - 2013");
					$cc++;

					$csv .= "BOLSAS DE DOUTORADO (PREVISTO) - 2005;BOLSAS DE DOUTORADO (PREVISTO) - 2006;BOLSAS DE DOUTORADO (PREVISTO) - 2007;BOLSAS DE DOUTORADO (PREVISTO) - 2008;BOLSAS DE DOUTORADO (PREVISTO) - 2009;BOLSAS DE DOUTORADO (PREVISTO) - 2010;BOLSAS DE DOUTORADO (PREVISTO) - 2011;BOLSAS DE DOUTORADO (PREVISTO) - 2012;BOLSAS DE DOUTORADO (PREVISTO) - 2013".(($cc == $colspan)?"\n":";");

				} elseif($destino[$_REQUEST['orgid']][$col]['codigo'] == "bolsasposdoutoradoprevisto") {

					$html .= "<td colspan='9' class='SubTituloCentro'>BOLSAS DE PÓS-DOUTORADO (PREVISTO)</td>";
					$htmlaux .= "<td class='SubTituloCentro'>2005</td><td class='SubTituloCentro'>2006</td><td class='SubTituloCentro'>2007</td><td class='SubTituloCentro'>2008</td><td class='SubTituloCentro'>2009</td><td class='SubTituloCentro'>2010</td><td class='SubTituloCentro'>2011</td><td class='SubTituloCentro'>2012</td><td class='SubTituloCentro'>2013</td>";

					$xls->MontaConteudoString(0, $cc, "BOLSAS DE PÓS-DOUTORADO (PREVISTO) - 2005");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "BOLSAS DE PÓS-DOUTORADO (PREVISTO) - 2006");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "BOLSAS DE PÓS-DOUTORADO (PREVISTO) - 2007");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "BOLSAS DE PÓS-DOUTORADO (PREVISTO) - 2008");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "BOLSAS DE PÓS-DOUTORADO (PREVISTO) - 2009");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "BOLSAS DE PÓS-DOUTORADO (PREVISTO) - 2010");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "BOLSAS DE PÓS-DOUTORADO (PREVISTO) - 2011");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "BOLSAS DE PÓS-DOUTORADO (PREVISTO) - 2012");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "BOLSAS DE PÓS-DOUTORADO (PREVISTO) - 2013");
					$cc++;

					$csv .= "BOLSAS DE PÓS-DOUTORADO (PREVISTO) - 2005;BOLSAS DE PÓS-DOUTORADO (PREVISTO) - 2006;BOLSAS DE PÓS-DOUTORADO (PREVISTO) - 2007;BOLSAS DE PÓS-DOUTORADO (PREVISTO) - 2008;BOLSAS DE PÓS-DOUTORADO (PREVISTO) - 2009;BOLSAS DE PÓS-DOUTORADO (PREVISTO) - 2010;BOLSAS DE PÓS-DOUTORADO (PREVISTO) - 2011;BOLSAS DE PÓS-DOUTORADO (PREVISTO) - 2012;BOLSAS DE PÓS-DOUTORADO (PREVISTO) - 2013".(($cc == $colspan)?"\n":";");

				} elseif($destino[$_REQUEST['orgid']][$col]['codigo'] == "docenterealizado") {

					$html .= "<td colspan='9' class='SubTituloCentro'>DOCENTE (REALIZADO)</td>";
					$htmlaux .= "<td class='SubTituloCentro'>2005</td><td class='SubTituloCentro'>2006</td><td class='SubTituloCentro'>2007</td><td class='SubTituloCentro'>2008</td><td class='SubTituloCentro'>2009</td><td class='SubTituloCentro'>2010</td><td class='SubTituloCentro'>2011</td><td class='SubTituloCentro'>2012</td><td class='SubTituloCentro'>2013</td>";

					$xls->MontaConteudoString(0, $cc, "DOCENTE (REALIZADO) - 2005");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "DOCENTE (REALIZADO) - 2006");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "DOCENTE (REALIZADO) - 2007");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "DOCENTE (REALIZADO) - 2008");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "DOCENTE (REALIZADO) - 2009");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "DOCENTE (REALIZADO) - 2010");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "DOCENTE (REALIZADO) - 2011");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "DOCENTE (REALIZADO) - 2012");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "DOCENTE (REALIZADO) - 2013");
					$cc++;

					$csv .= "DOCENTE (REALIZADO) - 2005;DOCENTE (REALIZADO) - 2006;DOCENTE (REALIZADO) - 2007;DOCENTE (REALIZADO) - 2008;DOCENTE (REALIZADO) - 2009;DOCENTE (REALIZADO) - 2010;DOCENTE (REALIZADO) - 2011;DOCENTE (REALIZADO) - 2012;DOCENTE (REALIZADO) - 2013".(($cc == $colspan)?"\n":";");

				} elseif($destino[$_REQUEST['orgid']][$col]['codigo'] == "tecnicorealizado") {

					$html .= "<td colspan='9' class='SubTituloCentro'>TÉCNICO (REALIZADO)</td>";
					$htmlaux .= "<td class='SubTituloCentro'>2005</td><td class='SubTituloCentro'>2006</td><td class='SubTituloCentro'>2007</td><td class='SubTituloCentro'>2008</td><td class='SubTituloCentro'>2009</td><td class='SubTituloCentro'>2010</td><td class='SubTituloCentro'>2011</td><td class='SubTituloCentro'>2012</td><td class='SubTituloCentro'>2013</td>";

					$xls->MontaConteudoString(0, $cc, "TÉCNICO (REALIZADO) - 2005");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "TÉCNICO (REALIZADO) - 2006");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "TÉCNICO (REALIZADO) - 2007");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "TÉCNICO (REALIZADO) - 2008");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "TÉCNICO (REALIZADO) - 2009");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "TÉCNICO (REALIZADO) - 2010");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "TÉCNICO (REALIZADO) - 2011");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "TÉCNICO (REALIZADO) - 2012");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "TÉCNICO (REALIZADO) - 2013");
					$cc++;

					$csv .= "TÉCNICO (REALIZADO) - 2005;TÉCNICO (REALIZADO) - 2006;TÉCNICO (REALIZADO) - 2007;TÉCNICO (REALIZADO) - 2008;TÉCNICO (REALIZADO) - 2009;TÉCNICO (REALIZADO) - 2010;TÉCNICO (REALIZADO) - 2011;TÉCNICO (REALIZADO) - 2012;TÉCNICO (REALIZADO) - 2013".(($cc == $colspan)?"\n":";");

				} elseif($destino[$_REQUEST['orgid']][$col]['codigo'] == "matrealizado") {

					$html .= "<td colspan='9' class='SubTituloCentro'>MAT (REALIZADO)</td>";
					$htmlaux .= "<td class='SubTituloCentro'>2005</td><td class='SubTituloCentro'>2006</td><td class='SubTituloCentro'>2007</td><td class='SubTituloCentro'>2008</td><td class='SubTituloCentro'>2009</td><td class='SubTituloCentro'>2010</td><td class='SubTituloCentro'>2011</td><td class='SubTituloCentro'>2012</td><td class='SubTituloCentro'>2013</td>";

					$xls->MontaConteudoString(0, $cc, "MAT (REALIZADO) - 2005");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "MAT (REALIZADO) - 2006");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "MAT (REALIZADO) - 2007");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "MAT (REALIZADO) - 2008");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "MAT (REALIZADO) - 2009");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "MAT (REALIZADO) - 2010");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "MAT (REALIZADO) - 2011");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "MAT (REALIZADO) - 2012");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "MAT (REALIZADO) - 2013");
					$cc++;

					$csv .= "MAT (REALIZADO) - 2005;MAT (REALIZADO) - 2006;MAT (REALIZADO) - 2007;MAT (REALIZADO) - 2008;MAT (REALIZADO) - 2009;MAT (REALIZADO) - 2010;MAT (REALIZADO) - 2011;MAT (REALIZADO) - 2012;MAT (REALIZADO) - 2013".(($cc == $colspan)?"\n":";");

				} elseif($destino[$_REQUEST['orgid']][$col]['codigo'] == "vagasrealizado") {

					$html .= "<td colspan='9' class='SubTituloCentro'>VAGAS (REALIZADO)</td>";
					$htmlaux .= "<td class='SubTituloCentro'>2005</td><td class='SubTituloCentro'>2006</td><td class='SubTituloCentro'>2007</td><td class='SubTituloCentro'>2008</td><td class='SubTituloCentro'>2009</td><td class='SubTituloCentro'>2010</td><td class='SubTituloCentro'>2011</td><td class='SubTituloCentro'>2012</td><td class='SubTituloCentro'>2013</td>";

					$xls->MontaConteudoString(0, $cc, "VAGAS (REALIZADO) - 2005");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "VAGAS (REALIZADO) - 2006");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "VAGAS (REALIZADO) - 2007");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "VAGAS (REALIZADO) - 2008");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "VAGAS (REALIZADO) - 2009");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "VAGAS (REALIZADO) - 2010");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "VAGAS (REALIZADO) - 2011");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "VAGAS (REALIZADO) - 2012");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "VAGAS (REALIZADO) - 2013");
					$cc++;

					$csv .= "VAGAS (REALIZADO) - 2005;VAGAS (REALIZADO) - 2006;VAGAS (REALIZADO) - 2007;VAGAS (REALIZADO) - 2008;VAGAS (REALIZADO) - 2009;VAGAS (REALIZADO) - 2010;VAGAS (REALIZADO) - 2011;VAGAS (REALIZADO) - 2012;VAGAS (REALIZADO) - 2013".(($cc == $colspan)?"\n":";");

				} elseif($destino[$_REQUEST['orgid']][$col]['codigo'] == "cursorealizado") {

					$html .= "<td colspan='9' class='SubTituloCentro'>CURSO (REALIZADO)</td>";
					$htmlaux .= "<td class='SubTituloCentro'>2005</td><td class='SubTituloCentro'>2006</td><td class='SubTituloCentro'>2007</td><td class='SubTituloCentro'>2008</td><td class='SubTituloCentro'>2009</td><td class='SubTituloCentro'>2010</td><td class='SubTituloCentro'>2011</td><td class='SubTituloCentro'>2012</td><td class='SubTituloCentro'>2013</td>";

					$xls->MontaConteudoString(0, $cc, "CURSO (REALIZADO) - 2005");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "CURSO (REALIZADO) - 2006");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "CURSO (REALIZADO) - 2007");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "CURSO (REALIZADO) - 2008");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "CURSO (REALIZADO) - 2009");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "CURSO (REALIZADO) - 2010");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "CURSO (REALIZADO) - 2011");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "CURSO (REALIZADO) - 2012");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "CURSO (REALIZADO) - 2013");
					$cc++;

					$csv .= "CURSO (REALIZADO) - 2005;CURSO (REALIZADO) - 2006;CURSO (REALIZADO) - 2007;CURSO (REALIZADO) - 2008;CURSO (REALIZADO) - 2009;CURSO (REALIZADO) - 2010;CURSO (REALIZADO) - 2011;CURSO (REALIZADO) - 2012;CURSO (REALIZADO) - 2013".(($cc == $colspan)?"\n":";");

				} elseif($destino[$_REQUEST['orgid']][$col]['codigo'] == "investimentorealizado") {

					$html .= "<td colspan='9' class='SubTituloCentro'>INVESTIMENTO (REALIZADO)</td>";
					$htmlaux .= "<td class='SubTituloCentro'>2005</td><td class='SubTituloCentro'>2006</td><td class='SubTituloCentro'>2007</td><td class='SubTituloCentro'>2008</td><td class='SubTituloCentro'>2009</td><td class='SubTituloCentro'>2010</td><td class='SubTituloCentro'>2011</td><td class='SubTituloCentro'>2012</td><td class='SubTituloCentro'>2013</td>";

					$xls->MontaConteudoString(0, $cc, "INVESTIMENTO (REALIZADO) - 2005");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "INVESTIMENTO (REALIZADO) - 2006");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "INVESTIMENTO (REALIZADO) - 2007");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "INVESTIMENTO (REALIZADO) - 2008");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "INVESTIMENTO (REALIZADO) - 2009");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "INVESTIMENTO (REALIZADO) - 2010");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "INVESTIMENTO (REALIZADO) - 2011");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "INVESTIMENTO (REALIZADO) - 2012");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "INVESTIMENTO (REALIZADO) - 2013");
					$cc++;

					$csv .= "INVESTIMENTO (REALIZADO) - 2005;INVESTIMENTO (REALIZADO) - 2006;INVESTIMENTO (REALIZADO) - 2007;INVESTIMENTO (REALIZADO) - 2008;INVESTIMENTO (REALIZADO) - 2009;INVESTIMENTO (REALIZADO) - 2010;INVESTIMENTO (REALIZADO) - 2011;INVESTIMENTO (REALIZADO) - 2012;INVESTIMENTO (REALIZADO) - 2013".(($cc == $colspan)?"\n":";");

				} elseif($destino[$_REQUEST['orgid']][$col]['codigo'] == "bolsasmestradorealizado") {

					$html .= "<td colspan='9' class='SubTituloCentro'>BOLSAS DE MESTRADO (REALIZADO)</td>";
					$htmlaux .= "<td class='SubTituloCentro'>2005</td><td class='SubTituloCentro'>2006</td><td class='SubTituloCentro'>2007</td><td class='SubTituloCentro'>2008</td><td class='SubTituloCentro'>2009</td><td class='SubTituloCentro'>2010</td><td class='SubTituloCentro'>2011</td><td class='SubTituloCentro'>2012</td><td class='SubTituloCentro'>2013</td>";

					$xls->MontaConteudoString(0, $cc, "BOLSAS DE MESTRADO (REALIZADO) - 2005");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "BOLSAS DE MESTRADO (REALIZADO) - 2006");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "BOLSAS DE MESTRADO (REALIZADO) - 2007");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "BOLSAS DE MESTRADO (REALIZADO) - 2008");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "BOLSAS DE MESTRADO (REALIZADO) - 2009");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "BOLSAS DE MESTRADO (REALIZADO) - 2010");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "BOLSAS DE MESTRADO (REALIZADO) - 2011");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "BOLSAS DE MESTRADO (REALIZADO) - 2012");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "BOLSAS DE MESTRADO (REALIZADO) - 2013");
					$cc++;

					$csv .= "BOLSAS DE MESTRADO (REALIZADO) - 2005;BOLSAS DE MESTRADO (REALIZADO) - 2006;BOLSAS DE MESTRADO (REALIZADO) - 2007;BOLSAS DE MESTRADO (REALIZADO) - 2008;BOLSAS DE MESTRADO (REALIZADO) - 2009;BOLSAS DE MESTRADO (REALIZADO) - 2010;BOLSAS DE MESTRADO (REALIZADO) - 2011;BOLSAS DE MESTRADO (REALIZADO) - 2012;BOLSAS DE MESTRADO (REALIZADO) - 2013".(($cc == $colspan)?"\n":";");

				} elseif($destino[$_REQUEST['orgid']][$col]['codigo'] == "bolsasdoutoradorealizado") {

					$html .= "<td colspan='9' class='SubTituloCentro'>BOLSAS DE DOUTORADO (REALIZADO)</td>";
					$htmlaux .= "<td class='SubTituloCentro'>2005</td><td class='SubTituloCentro'>2006</td><td class='SubTituloCentro'>2007</td><td class='SubTituloCentro'>2008</td><td class='SubTituloCentro'>2009</td><td class='SubTituloCentro'>2010</td><td class='SubTituloCentro'>2011</td><td class='SubTituloCentro'>2012</td><td class='SubTituloCentro'>2013</td>";

					$xls->MontaConteudoString(0, $cc, "BOLSAS DE DOUTORADO (REALIZADO) - 2005");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "BOLSAS DE DOUTORADO (REALIZADO) - 2006");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "BOLSAS DE DOUTORADO (REALIZADO) - 2007");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "BOLSAS DE DOUTORADO (REALIZADO) - 2008");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "BOLSAS DE DOUTORADO (REALIZADO) - 2009");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "BOLSAS DE DOUTORADO (REALIZADO) - 2010");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "BOLSAS DE DOUTORADO (REALIZADO) - 2011");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "BOLSAS DE DOUTORADO (REALIZADO) - 2012");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "BOLSAS DE DOUTORADO (REALIZADO) - 2013");
					$cc++;

					$csv .= "BOLSAS DE DOUTORADO (REALIZADO) - 2005;BOLSAS DE DOUTORADO (REALIZADO) - 2006;BOLSAS DE DOUTORADO (REALIZADO) - 2007;BOLSAS DE DOUTORADO (REALIZADO) - 2008;BOLSAS DE DOUTORADO (REALIZADO) - 2009;BOLSAS DE DOUTORADO (REALIZADO) - 2010;BOLSAS DE DOUTORADO (REALIZADO) - 2011;BOLSAS DE DOUTORADO (REALIZADO) - 2012;BOLSAS DE DOUTORADO (REALIZADO) - 2013".(($cc == $colspan)?"\n":";");

				} elseif($destino[$_REQUEST['orgid']][$col]['codigo'] == "bolsasposdoutoradorealizado") {

					$html .= "<td colspan='9' class='SubTituloCentro'>BOLSAS DE PÓS-DOUTORADO (REALIZADO)</td>";
					$htmlaux .= "<td class='SubTituloCentro'>2005</td><td class='SubTituloCentro'>2006</td><td class='SubTituloCentro'>2007</td><td class='SubTituloCentro'>2008</td><td class='SubTituloCentro'>2009</td><td class='SubTituloCentro'>2010</td><td class='SubTituloCentro'>2011</td><td class='SubTituloCentro'>2012</td><td class='SubTituloCentro'>2013</td>";

					$xls->MontaConteudoString(0, $cc, "BOLSAS DE PÓS-DOUTORADO (REALIZADO) - 2005");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "BOLSAS DE PÓS-DOUTORADO (REALIZADO) - 2006");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "BOLSAS DE PÓS-DOUTORADO (REALIZADO) - 2007");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "BOLSAS DE PÓS-DOUTORADO (REALIZADO) - 2008");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "BOLSAS DE PÓS-DOUTORADO (REALIZADO) - 2009");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "BOLSAS DE PÓS-DOUTORADO (REALIZADO) - 2010");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "BOLSAS DE PÓS-DOUTORADO (REALIZADO) - 2011");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "BOLSAS DE PÓS-DOUTORADO (REALIZADO) - 2012");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "BOLSAS DE PÓS-DOUTORADO (REALIZADO) - 2013");
					$cc++;

					$csv .= "BOLSAS DE PÓS-DOUTORADO (REALIZADO) - 2005;BOLSAS DE PÓS-DOUTORADO (REALIZADO) - 2006;BOLSAS DE PÓS-DOUTORADO (REALIZADO) - 2007;BOLSAS DE PÓS-DOUTORADO (REALIZADO) - 2008;BOLSAS DE PÓS-DOUTORADO (REALIZADO) - 2009;BOLSAS DE PÓS-DOUTORADO (REALIZADO) - 2010;BOLSAS DE PÓS-DOUTORADO (REALIZADO) - 2011;BOLSAS DE PÓS-DOUTORADO (REALIZADO) - 2012;BOLSAS DE PÓS-DOUTORADO (REALIZADO) - 2013".(($cc == $colspan)?"\n":";");

				} elseif($destino[$_REQUEST['orgid']][$col]['codigo'] == "matriculasgradpresen") {

					$html .= "<td colspan='9' class='SubTituloCentro'>MATRÍCULAS (GRADUAÇÃO PRESENCIAL)</td>";
					$htmlaux .= "<td class='SubTituloCentro'>2005</td><td class='SubTituloCentro'>2006</td><td class='SubTituloCentro'>2007</td><td class='SubTituloCentro'>2008</td><td class='SubTituloCentro'>2009</td><td class='SubTituloCentro'>2010</td><td class='SubTituloCentro'>2011</td><td class='SubTituloCentro'>2012</td><td class='SubTituloCentro'>2013</td>";

					$xls->MontaConteudoString(0, $cc, "MATRÍCULAS (GRADUAÇÃO PRESENCIAL) - 2005");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "MATRÍCULAS (GRADUAÇÃO PRESENCIAL) - 2006");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "MATRÍCULAS (GRADUAÇÃO PRESENCIAL) - 2007");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "MATRÍCULAS (GRADUAÇÃO PRESENCIAL) - 2008");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "MATRÍCULAS (GRADUAÇÃO PRESENCIAL) - 2009");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "MATRÍCULAS (GRADUAÇÃO PRESENCIAL) - 2010");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "MATRÍCULAS (GRADUAÇÃO PRESENCIAL) - 2011");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "MATRÍCULAS (GRADUAÇÃO PRESENCIAL) - 2012");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "MATRÍCULAS (GRADUAÇÃO PRESENCIAL) - 2013");
					$cc++;

					$csv .= "MATRÍCULAS (GRADUAÇÃO PRESENCIAL) - 2005;MATRÍCULAS (GRADUAÇÃO PRESENCIAL) - 2006;MATRÍCULAS (GRADUAÇÃO PRESENCIAL) - 2007;MATRÍCULAS (GRADUAÇÃO PRESENCIAL) - 2008;MATRÍCULAS (GRADUAÇÃO PRESENCIAL) - 2009;MATRÍCULAS (GRADUAÇÃO PRESENCIAL) - 2010;MATRÍCULAS (GRADUAÇÃO PRESENCIAL) - 2011;MATRÍCULAS (GRADUAÇÃO PRESENCIAL) - 2012;MATRÍCULAS (GRADUAÇÃO PRESENCIAL) - 2013".(($cc == $colspan)?"\n":";");

				} elseif($destino[$_REQUEST['orgid']][$col]['codigo'] == "enderecocampus"){
					$rowspan = 2;
					$html .= "<td rowspan='".$rowspan."' class='SubTituloCentro'>CEP do Campus</td><td rowspan='".$rowspan."' class='SubTituloCentro'>Bairro do Campus</td><td rowspan='".$rowspan."' class='SubTituloCentro'>Endereço do Campus</td><td rowspan='".$rowspan."' class='SubTituloCentro'>Número do Campus</td>";

					$xls->MontaConteudoString(0, $cc, "CEP do Campus");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "Bairro do Campus");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "Endereço do Campus");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "Número do Campus");
					$cc++;

					$csv .= "CEP do Campus;Bairro do Campus;Endereço do Campus;Número do Campus".(($cc == $colspan)?"\n":";");

				} elseif($destino[$_REQUEST['orgid']][$col]['codigo'] == "enderecounidadeorc"){

					$rowspan = 2;
					$html .= "<td rowspan='".$rowspan."' class='SubTituloCentro'>CEP da Universidade</td><td rowspan='".$rowspan."' class='SubTituloCentro'>Bairro da Universidade</td><td rowspan='".$rowspan."' class='SubTituloCentro'>Endereço da Universidade</td><td rowspan='".$rowspan."' class='SubTituloCentro'>Número da Universidade</td>";

					$xls->MontaConteudoString(0, $cc, "CEP da Universidade");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "Bairro da Universidade");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "Endereço da Universidade");
					$cc++;
					$xls->MontaConteudoString(0, $cc, "Número da Universidade");
					$cc++;

					$csv .= "CEP da Universidade;Bairro da Universidade;Endereço da Universidade;Número da Universidade".(($cc == $colspan)?"\n":";");

				} else {

					$rowspan = 2;
					$html .= "<td rowspan='".$rowspan."' class='SubTituloCentro'>".$destino[$_REQUEST['orgid']][$col]['descricao']."</td>";

					$xls->MontaConteudoString(0, $cc, $destino[$_REQUEST['orgid']][$col]['descricao']);
					$cc++;

					$csv .= $destino[$_REQUEST['orgid']][$col]['descricao'].(($cc == $colspan)?"\n":";");

				}

			}

			$html .= "</tr>";

			if($htmlaux) {
				$html .= $htmlaux;
			}
		}
		/*
		 * FIM - GERANDO CABEÇALHO EXCEL, CSV E HTML
		 */


		$i=1;
		foreach($dadoscsv as $registro) {

			$cc=0;
			if (fmod($i,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';

			if($_REQUEST['agrupador_'.$_REQUEST['orgid']]) {

				$html .= "<tr bgcolor='".$marcado."'>";

				foreach($_REQUEST['agrupador_'.$_REQUEST['orgid']] as $col) {

					if($destino[$_REQUEST['orgid']][$col]['codigo'] == "docenteprevisto") {
						$html .= "<td>".number_format($registro['docenteprevano2005'], 0, ',', '.')."</td>
								  <td>".number_format($registro['docenteprevano2006'], 0, ',', '.')."</td>
								  <td>".number_format($registro['docenteprevano2007'], 0, ',', '.')."</td>
								  <td>".number_format($registro['docenteprevano2008'], 0, ',', '.')."</td>
								  <td>".number_format($registro['docenteprevano2009'], 0, ',', '.')."</td>
								  <td>".number_format($registro['docenteprevano2010'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['docenteprevano2011'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['docenteprevano2012'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['docenteprevano2013'], 0, ',', '.')."</td>
					 			  ";

						$xls->MontaConteudoString($i, $cc, number_format($registro['docenteprevano2005'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['docenteprevano2006'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['docenteprevano2007'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['docenteprevano2008'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['docenteprevano2009'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['docenteprevano2010'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['docenteprevano2011'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['docenteprevano2012'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['docenteprevano2013'], 0, ',', '.'));
						$cc++;

						$csv .= number_format($registro['docenteprevano2005'], 0, ',', '.').";".number_format($registro['docenteprevano2006'], 0, ',', '.').";".number_format($registro['docenteprevano2007'], 0, ',', '.').";".number_format($registro['docenteprevano2008'], 0, ',', '.').";".number_format($registro['docenteprevano2009'], 0, ',', '.').";".number_format($registro['docenteprevano2010'], 0, ',', '.').";".number_format($registro['docenteprevano2011'], 0, ',', '.').";".number_format($registro['docenteprevano2012'], 0, ',', '.').";".number_format($registro['docenteprevano2013'], 0, ',', '.').(($cc == $colspan)?"\n":";");

					} elseif($destino[$_REQUEST['orgid']][$col]['codigo'] == "tecnicoprevisto") {
						$html .= "<td>".number_format($registro['tecnicoprevano2005'], 0, ',', '.')."</td>
								  <td>".number_format($registro['tecnicoprevano2006'], 0, ',', '.')."</td>
								  <td>".number_format($registro['tecnicoprevano2007'], 0, ',', '.')."</td>
								  <td>".number_format($registro['tecnicoprevano2008'], 0, ',', '.')."</td>
								  <td>".number_format($registro['tecnicoprevano2009'], 0, ',', '.')."</td>
								  <td>".number_format($registro['tecnicoprevano2010'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['tecnicoprevano2011'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['tecnicoprevano2012'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['tecnicoprevano2013'], 0, ',', '.')."</td>
					 			  ";

						$xls->MontaConteudoString($i, $cc, number_format($registro['tecnicoprevano2005'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['tecnicoprevano2006'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['tecnicoprevano2007'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['tecnicoprevano2008'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['tecnicoprevano2009'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['tecnicoprevano2010'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['tecnicoprevano2011'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['tecnicoprevano2012'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['tecnicoprevano2013'], 0, ',', '.'));
						$cc++;

						$csv .= number_format($registro['tecnicoprevano2005'], 0, ',', '.').";".number_format($registro['tecnicoprevano2006'], 0, ',', '.').";".number_format($registro['tecnicoprevano2007'], 0, ',', '.').";".number_format($registro['tecnicoprevano2008'], 0, ',', '.').";".number_format($registro['tecnicoprevano2009'], 0, ',', '.').";".number_format($registro['tecnicoprevano2010'], 0, ',', '.').";".number_format($registro['tecnicoprevano2011'], 0, ',', '.').";".number_format($registro['tecnicoprevano2012'], 0, ',', '.').";".number_format($registro['tecnicoprevano2013'], 0, ',', '.').(($cc == $colspan)?"\n":";");

					} elseif($destino[$_REQUEST['orgid']][$col]['codigo'] == "matprevisto") {
						$html .= "<td>".number_format($registro['matprevano2005'], 0, ',', '.')."</td>
								  <td>".number_format($registro['matprevano2006'], 0, ',', '.')."</td>
								  <td>".number_format($registro['matprevano2007'], 0, ',', '.')."</td>
								  <td>".number_format($registro['matprevano2008'], 0, ',', '.')."</td>
								  <td>".number_format($registro['matprevano2009'], 0, ',', '.')."</td>
								  <td>".number_format($registro['matprevano2010'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['matprevano2011'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['matprevano2012'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['matprevano2013'], 0, ',', '.')."</td>
					 			  ";

						$xls->MontaConteudoString($i, $cc, number_format($registro['matprevano2005'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['matprevano2006'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['matprevano2007'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['matprevano2008'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['matprevano2009'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['matprevano2010'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['matprevano2011'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['matprevano2012'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['matprevano2013'], 0, ',', '.'));
						$cc++;

						$csv .= number_format($registro['matprevano2005'], 0, ',', '.').";".number_format($registro['matprevano2006'], 0, ',', '.').";".number_format($registro['matprevano2007'], 0, ',', '.').";".number_format($registro['matprevano2008'], 0, ',', '.').";".number_format($registro['matprevano2009'], 0, ',', '.').";".number_format($registro['matprevano2010'], 0, ',', '.').";".number_format($registro['matprevano2011'], 0, ',', '.').";".number_format($registro['matprevano2012'], 0, ',', '.').";".number_format($registro['matprevano2013'], 0, ',', '.').(($cc == $colspan)?"\n":";");

					} elseif($destino[$_REQUEST['orgid']][$col]['codigo'] == "vagasprevisto") {

						$html .= "<td>".number_format($registro['vagasprevano2005'], 0, ',', '.')."</td>
								  <td>".number_format($registro['vagasprevano2006'], 0, ',', '.')."</td>
								  <td>".number_format($registro['vagasprevano2007'], 0, ',', '.')."</td>
								  <td>".number_format($registro['vagasprevano2008'], 0, ',', '.')."</td>
								  <td>".number_format($registro['vagasprevano2009'], 0, ',', '.')."</td>
								  <td>".number_format($registro['vagasprevano2010'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['vagasprevano2011'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['vagasprevano2012'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['vagasprevano2013'], 0, ',', '.')."</td>
					 			  ";

						$xls->MontaConteudoString($i, $cc, number_format($registro['vagasprevano2005'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['vagasprevano2006'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['vagasprevano2007'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['vagasprevano2008'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['vagasprevano2009'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['vagasprevano2010'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['vagasprevano2011'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['vagasprevano2012'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['vagasprevano2013'], 0, ',', '.'));
						$cc++;

						$csv .= number_format($registro['vagasprevano2005'], 0, ',', '.').";".number_format($registro['vagasprevano2006'], 0, ',', '.').";".number_format($registro['vagasprevano2007'], 0, ',', '.').";".number_format($registro['vagasprevano2008'], 0, ',', '.').";".number_format($registro['vagasprevano2009'], 0, ',', '.').";".number_format($registro['vagasprevano2010'], 0, ',', '.').";".number_format($registro['vagasprevano2011'], 0, ',', '.').";".number_format($registro['vagasprevano2012'], 0, ',', '.').";".number_format($registro['vagasprevano2013'], 0, ',', '.').(($cc == $colspan)?"\n":";");

					} elseif($destino[$_REQUEST['orgid']][$col]['codigo'] == "cursoprevisto") {
						$html .= "<td>".number_format($registro['cursoprevano2005'], 0, ',', '.')."</td>
								  <td>".number_format($registro['cursoprevano2006'], 0, ',', '.')."</td>
								  <td>".number_format($registro['cursoprevano2007'], 0, ',', '.')."</td>
								  <td>".number_format($registro['cursoprevano2008'], 0, ',', '.')."</td>
								  <td>".number_format($registro['cursoprevano2009'], 0, ',', '.')."</td>
								  <td>".number_format($registro['cursoprevano2010'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['cursoprevano2011'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['cursoprevano2012'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['cursoprevano2013'], 0, ',', '.')."</td>
					 			  ";

						$xls->MontaConteudoString($i, $cc, number_format($registro['cursoprevano2005'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['cursoprevano2006'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['cursoprevano2007'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['cursoprevano2008'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['cursoprevano2009'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['cursoprevano2010'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['cursoprevano2011'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['cursoprevano2012'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['cursoprevano2013'], 0, ',', '.'));
						$cc++;

						$csv .= number_format($registro['cursoprevano2005'], 0, ',', '.').";".number_format($registro['cursoprevano2006'], 0, ',', '.').";".number_format($registro['cursoprevano2007'], 0, ',', '.').";".number_format($registro['cursoprevano2008'], 0, ',', '.').";".number_format($registro['cursoprevano2009'], 0, ',', '.').";".number_format($registro['cursoprevano2010'], 0, ',', '.').";".number_format($registro['cursoprevano2011'], 0, ',', '.').";".number_format($registro['cursoprevano2012'], 0, ',', '.').";".number_format($registro['cursoprevano2013'], 0, ',', '.').(($cc == $colspan)?"\n":";");

					} elseif($destino[$_REQUEST['orgid']][$col]['codigo'] == "investimentoprevisto") {
						$html .= "<td>".number_format($registro['investimentoprevano2005'], 2, ',', '.')."</td>
								  <td>".number_format($registro['investimentoprevano2006'], 2, ',', '.')."</td>
								  <td>".number_format($registro['investimentoprevano2007'], 2, ',', '.')."</td>
								  <td>".number_format($registro['investimentoprevano2008'], 2, ',', '.')."</td>
								  <td>".number_format($registro['investimentoprevano2009'], 2, ',', '.')."</td>
								  <td>".number_format($registro['investimentoprevano2010'], 2, ',', '.')."</td>
					 			  <td>".number_format($registro['investimentoprevano2011'], 2, ',', '.')."</td>
					 			  <td>".number_format($registro['investimentoprevano2012'], 2, ',', '.')."</td>
					 			  <td>".number_format($registro['investimentoprevano2013'], 2, ',', '.')."</td>
					 			  ";

						$xls->MontaConteudoString($i, $cc, number_format($registro['investimentoprevano2005'], 2, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['investimentoprevano2006'], 2, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['investimentoprevano2007'], 2, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['investimentoprevano2008'], 2, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['investimentoprevano2009'], 2, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['investimentoprevano2010'], 2, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['investimentoprevano2011'], 2, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['investimentoprevano2012'], 2, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['investimentoprevano2013'], 2, ',', '.'));
						$cc++;

						$csv .= number_format($registro['investimentoprevano2005'], 2, ',', '.').";".number_format($registro['investimentoprevano2006'], 2, ',', '.').";".number_format($registro['investimentoprevano2007'], 2, ',', '.').";".number_format($registro['investimentoprevano2008'], 2, ',', '.').";".number_format($registro['investimentoprevano2009'], 2, ',', '.').";".number_format($registro['investimentoprevano2010'], 2, ',', '.').";".number_format($registro['investimentoprevano2011'], 2, ',', '.').";".number_format($registro['investimentoprevano2012'], 2, ',', '.').";".number_format($registro['investimentoprevano2013'], 2, ',', '.').(($cc == $colspan)?"\n":";");

					} elseif($destino[$_REQUEST['orgid']][$col]['codigo'] == "bolsasmestradoprevisto") {
						$html .= "<td>".number_format($registro['bolsasmestradoprevano2005'], 0, ',', '.')."</td>
								  <td>".number_format($registro['bolsasmestradoprevano2006'], 0, ',', '.')."</td>
								  <td>".number_format($registro['bolsasmestradoprevano2007'], 0, ',', '.')."</td>
								  <td>".number_format($registro['bolsasmestradoprevano2008'], 0, ',', '.')."</td>
								  <td>".number_format($registro['bolsasmestradoprevano2009'], 0, ',', '.')."</td>
								  <td>".number_format($registro['bolsasmestradoprevano2010'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['bolsasmestradoprevano2011'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['bolsasmestradoprevano2012'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['bolsasmestradoprevano2013'], 0, ',', '.')."</td>
					 			  ";

						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasmestradoprevano2005'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasmestradoprevano2006'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasmestradoprevano2007'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasmestradoprevano2008'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasmestradoprevano2009'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasmestradoprevano2010'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasmestradoprevano2011'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasmestradoprevano2012'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasmestradoprevano2013'], 0, ',', '.'));
						$cc++;

						$csv .= number_format($registro['bolsasmestradoprevano2005'], 0, ',', '.').";".number_format($registro['bolsasmestradoprevano2006'], 0, ',', '.').";".number_format($registro['bolsasmestradoprevano2007'], 0, ',', '.').";".number_format($registro['bolsasmestradoprevano2008'], 0, ',', '.').";".number_format($registro['bolsasmestradoprevano2009'], 0, ',', '.').";".number_format($registro['bolsasmestradoprevano2010'], 0, ',', '.').";".number_format($registro['bolsasmestradoprevano2011'], 0, ',', '.').";".number_format($registro['bolsasmestradoprevano2012'], 0, ',', '.').";".number_format($registro['bolsasmestradoprevano2013'], 0, ',', '.').(($cc == $colspan)?"\n":";");

					} elseif($destino[$_REQUEST['orgid']][$col]['codigo'] == "bolsasdoutoradoprevisto") {
						$html .= "<td>".number_format($registro['bolsasdoutoradoprevano2005'], 0, ',', '.')."</td>
								  <td>".number_format($registro['bolsasdoutoradoprevano2006'], 0, ',', '.')."</td>
								  <td>".number_format($registro['bolsasdoutoradoprevano2007'], 0, ',', '.')."</td>
								  <td>".number_format($registro['bolsasdoutoradoprevano2008'], 0, ',', '.')."</td>
								  <td>".number_format($registro['bolsasdoutoradoprevano2009'], 0, ',', '.')."</td>
								  <td>".number_format($registro['bolsasdoutoradoprevano2010'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['bolsasdoutoradoprevano2011'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['bolsasdoutoradoprevano2012'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['bolsasdoutoradoprevano2013'], 0, ',', '.')."</td>
					 			  ";

						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasdoutoradoprevano2005'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasdoutoradoprevano2006'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasdoutoradoprevano2007'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasdoutoradoprevano2008'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasdoutoradoprevano2009'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasdoutoradoprevano2010'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasdoutoradoprevano2011'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasdoutoradoprevano2012'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasdoutoradoprevano2013'], 0, ',', '.'));
						$cc++;

						$csv .= number_format($registro['bolsasdoutoradoprevano2005'], 0, ',', '.').";".number_format($registro['bolsasdoutoradoprevano2006'], 0, ',', '.').";".number_format($registro['bolsasdoutoradoprevano2007'], 0, ',', '.').";".number_format($registro['bolsasdoutoradoprevano2008'], 0, ',', '.').";".number_format($registro['bolsasdoutoradoprevano2009'], 0, ',', '.').";".number_format($registro['bolsasdoutoradoprevano2010'], 0, ',', '.').";".number_format($registro['bolsasdoutoradoprevano2011'], 0, ',', '.').";".number_format($registro['bolsasdoutoradoprevano2012'], 0, ',', '.').";".number_format($registro['bolsasdoutoradoprevano2013'], 0, ',', '.').(($cc == $colspan)?"\n":";");

					} elseif($destino[$_REQUEST['orgid']][$col]['codigo'] == "bolsasposdoutoradoprevisto") {
						$html .= "<td>".number_format($registro['bolsasposdoutoradoprevano2005'], 0, ',', '.')."</td>
								  <td>".number_format($registro['bolsasposdoutoradoprevano2006'], 0, ',', '.')."</td>
								  <td>".number_format($registro['bolsasposdoutoradoprevano2007'], 0, ',', '.')."</td>
								  <td>".number_format($registro['bolsasposdoutoradoprevano2008'], 0, ',', '.')."</td>
								  <td>".number_format($registro['bolsasposdoutoradoprevano2009'], 0, ',', '.')."</td>
								  <td>".number_format($registro['bolsasposdoutoradoprevano2010'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['bolsasposdoutoradoprevano2011'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['bolsasposdoutoradoprevano2012'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['bolsasposdoutoradoprevano2013'], 0, ',', '.')."</td>
					 			  ";

						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasposdoutoradoprevano2005'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasposdoutoradoprevano2006'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasposdoutoradoprevano2007'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasposdoutoradoprevano2008'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasposdoutoradoprevano2009'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasposdoutoradoprevano2010'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasposdoutoradoprevano2011'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasposdoutoradoprevano2012'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasposdoutoradoprevano2013'], 0, ',', '.'));
						$cc++;

						$csv .= number_format($registro['bolsasposdoutoradoprevano2005'], 0, ',', '.').";".number_format($registro['bolsasposdoutoradoprevano2006'], 0, ',', '.').";".number_format($registro['bolsasposdoutoradoprevano2007'], 0, ',', '.').";".number_format($registro['bolsasposdoutoradoprevano2008'], 0, ',', '.').";".number_format($registro['bolsasposdoutoradoprevano2009'], 0, ',', '.').";".number_format($registro['bolsasposdoutoradoprevano2010'], 0, ',', '.').";".number_format($registro['bolsasposdoutoradoprevano2011'], 0, ',', '.').";".number_format($registro['bolsasposdoutoradoprevano2012'], 0, ',', '.').";".number_format($registro['bolsasposdoutoradoprevano2013'], 0, ',', '.').(($cc == $colspan)?"\n":";");

					} elseif($destino[$_REQUEST['orgid']][$col]['codigo'] == "docenterealizado") {
						$html .= "<td>".number_format($registro['docenterealizano2005'], 0, ',', '.')."</td>
								  <td>".number_format($registro['docenterealizano2006'], 0, ',', '.')."</td>
								  <td>".number_format($registro['docenterealizano2007'], 0, ',', '.')."</td>
								  <td>".number_format($registro['docenterealizano2008'], 0, ',', '.')."</td>
								  <td>".number_format($registro['docenterealizano2009'], 0, ',', '.')."</td>
								  <td>".number_format($registro['docenterealizano2010'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['docenterealizano2011'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['docenterealizano2012'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['docenterealizano2013'], 0, ',', '.')."</td>
					 			  ";

						$xls->MontaConteudoString($i, $cc, number_format($registro['docenterealizano2005'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['docenterealizano2006'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['docenterealizano2007'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['docenterealizano2008'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['docenterealizano2009'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['docenterealizano2010'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['docenterealizano2011'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['docenterealizano2012'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['docenterealizano2013'], 0, ',', '.'));
						$cc++;

						$csv .= number_format($registro['docenterealizano2005'], 0, ',', '.').";".number_format($registro['docenterealizano2006'], 0, ',', '.').";".number_format($registro['docenterealizano2007'], 0, ',', '.').";".number_format($registro['docenterealizano2008'], 0, ',', '.').";".number_format($registro['docenterealizano2009'], 0, ',', '.').";".number_format($registro['docenterealizano2010'], 0, ',', '.').";".number_format($registro['docenterealizano2011'], 0, ',', '.').";".number_format($registro['docenterealizano2012'], 0, ',', '.').";".number_format($registro['docenterealizano2013'], 0, ',', '.').(($cc == $colspan)?"\n":";");

					} elseif($destino[$_REQUEST['orgid']][$col]['codigo'] == "tecnicorealizado") {
						$html .= "<td>".number_format($registro['tecnicorealizano2005'], 0, ',', '.')."</td>
								  <td>".number_format($registro['tecnicorealizano2006'], 0, ',', '.')."</td>
								  <td>".number_format($registro['tecnicorealizano2007'], 0, ',', '.')."</td>
								  <td>".number_format($registro['tecnicorealizano2008'], 0, ',', '.')."</td>
								  <td>".number_format($registro['tecnicorealizano2009'], 0, ',', '.')."</td>
								  <td>".number_format($registro['tecnicorealizano2010'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['tecnicorealizano2011'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['tecnicorealizano2012'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['tecnicorealizano2013'], 0, ',', '.')."</td>
					 			  ";

						$xls->MontaConteudoString($i, $cc, number_format($registro['tecnicorealizano2005'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['tecnicorealizano2006'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['tecnicorealizano2007'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['tecnicorealizano2008'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['tecnicorealizano2009'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['tecnicorealizano2010'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['tecnicorealizano2011'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['tecnicorealizano2012'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['tecnicorealizano2013'], 0, ',', '.'));
						$cc++;

						$csv .= number_format($registro['tecnicorealizano2005'], 0, ',', '.').";".number_format($registro['tecnicorealizano2006'], 0, ',', '.').";".number_format($registro['tecnicorealizano2007'], 0, ',', '.').";".number_format($registro['tecnicorealizano2008'], 0, ',', '.').";".number_format($registro['tecnicorealizano2009'], 0, ',', '.').";".number_format($registro['tecnicorealizano2010'], 0, ',', '.').";".number_format($registro['tecnicorealizano2011'], 0, ',', '.').";".number_format($registro['tecnicorealizano2012'], 0, ',', '.').";".number_format($registro['tecnicorealizano2013'], 0, ',', '.').(($cc == $colspan)?"\n":";");

					} elseif($destino[$_REQUEST['orgid']][$col]['codigo'] == "matrealizado") {
						$html .= "<td>".number_format($registro['matrealizano2005'], 0, ',', '.')."</td>
								  <td>".number_format($registro['matrealizano2006'], 0, ',', '.')."</td>
								  <td>".number_format($registro['matrealizano2007'], 0, ',', '.')."</td>
								  <td>".number_format($registro['matrealizano2008'], 0, ',', '.')."</td>
								  <td>".number_format($registro['matrealizano2009'], 0, ',', '.')."</td>
								  <td>".number_format($registro['matrealizano2010'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['matrealizano2011'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['matrealizano2012'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['matrealizano2013'], 0, ',', '.')."</td>
					 			  ";

						$xls->MontaConteudoString($i, $cc, number_format($registro['matrealizano2005'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['matrealizano2006'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['matrealizano2007'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['matrealizano2008'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['matrealizano2009'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['matrealizano2010'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['matrealizano2011'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['matrealizano2012'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['matrealizano2013'], 0, ',', '.'));
						$cc++;

						$csv .= number_format($registro['matrealizano2005'], 0, ',', '.').";".number_format($registro['matrealizano2006'], 0, ',', '.').";".number_format($registro['matrealizano2007'], 0, ',', '.').";".number_format($registro['matrealizano2008'], 0, ',', '.').";".number_format($registro['matrealizano2009'], 0, ',', '.').";".number_format($registro['matrealizano2010'], 0, ',', '.').";".number_format($registro['matrealizano2011'], 0, ',', '.').";".number_format($registro['matrealizano2012'], 0, ',', '.').";".number_format($registro['matrealizano2013'], 0, ',', '.').(($cc == $colspan)?"\n":";");

					} elseif($destino[$_REQUEST['orgid']][$col]['codigo'] == "vagasrealizado") {

						$html .= "<td>".number_format($registro['vagasrealizano2005'], 0, ',', '.')."</td>
								  <td>".number_format($registro['vagasrealizano2006'], 0, ',', '.')."</td>
								  <td>".number_format($registro['vagasrealizano2007'], 0, ',', '.')."</td>
								  <td>".number_format($registro['vagasrealizano2008'], 0, ',', '.')."</td>
								  <td>".number_format($registro['vagasrealizano2009'], 0, ',', '.')."</td>
								  <td>".number_format($registro['vagasrealizano2010'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['vagasrealizano2011'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['vagasrealizano2012'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['vagasrealizano2013'], 0, ',', '.')."</td>
					 			  ";

						$xls->MontaConteudoString($i, $cc, number_format($registro['vagasrealizano2005'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['vagasrealizano2006'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['vagasrealizano2007'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['vagasrealizano2008'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['vagasrealizano2009'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['vagasrealizano2010'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['vagasrealizano2011'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['vagasrealizano2012'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['vagasrealizano2013'], 0, ',', '.'));
						$cc++;

						$csv .= number_format($registro['vagasrealizano2005'], 0, ',', '.').";".number_format($registro['vagasrealizano2006'], 0, ',', '.').";".number_format($registro['vagasrealizano2007'], 0, ',', '.').";".number_format($registro['vagasrealizano2008'], 0, ',', '.').";".number_format($registro['vagasrealizano2009'], 0, ',', '.').";".number_format($registro['vagasrealizano2010'], 0, ',', '.').";".number_format($registro['vagasrealizano2011'], 0, ',', '.').";".number_format($registro['vagasrealizano2012'], 0, ',', '.').";".number_format($registro['vagasrealizano2013'], 0, ',', '.').(($cc == $colspan)?"\n":";");

					} elseif($destino[$_REQUEST['orgid']][$col]['codigo'] == "cursorealizado") {
						$html .= "<td>".number_format($registro['cursorealizano2005'], 0, ',', '.')."</td>
								  <td>".number_format($registro['cursorealizano2006'], 0, ',', '.')."</td>
								  <td>".number_format($registro['cursorealizano2007'], 0, ',', '.')."</td>
								  <td>".number_format($registro['cursorealizano2008'], 0, ',', '.')."</td>
								  <td>".number_format($registro['cursorealizano2009'], 0, ',', '.')."</td>
								  <td>".number_format($registro['cursorealizano2010'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['cursorealizano2011'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['cursorealizano2012'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['cursorealizano2013'], 0, ',', '.')."</td>
					 			  ";

						$xls->MontaConteudoString($i, $cc, number_format($registro['cursorealizano2005'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['cursorealizano2006'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['cursorealizano2007'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['cursorealizano2008'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['cursorealizano2009'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['cursorealizano2010'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['cursorealizano2011'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['cursorealizano2012'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['cursorealizano2013'], 0, ',', '.'));
						$cc++;

						$csv .= number_format($registro['cursorealizano2005'], 0, ',', '.').";".number_format($registro['cursorealizano2006'], 0, ',', '.').";".number_format($registro['cursorealizano2007'], 0, ',', '.').";".number_format($registro['cursorealizano2008'], 0, ',', '.').";".number_format($registro['cursorealizano2009'], 0, ',', '.').";".number_format($registro['cursorealizano2010'], 0, ',', '.').";".number_format($registro['cursorealizano2011'], 0, ',', '.').";".number_format($registro['cursorealizano2012'], 0, ',', '.').";".number_format($registro['cursorealizano2013'], 0, ',', '.').(($cc == $colspan)?"\n":";");

					} elseif($destino[$_REQUEST['orgid']][$col]['codigo'] == "investimentorealizado") {
						$html .= "<td>".number_format($registro['investimentorealizano2005'], 2, ',', '.')."</td>
								  <td>".number_format($registro['investimentorealizano2006'], 2, ',', '.')."</td>
								  <td>".number_format($registro['investimentorealizano2007'], 2, ',', '.')."</td>
								  <td>".number_format($registro['investimentorealizano2008'], 2, ',', '.')."</td>
								  <td>".number_format($registro['investimentorealizano2009'], 2, ',', '.')."</td>
								  <td>".number_format($registro['investimentorealizano2010'], 2, ',', '.')."</td>
					 			  <td>".number_format($registro['investimentorealizano2011'], 2, ',', '.')."</td>
					 			  <td>".number_format($registro['investimentorealizano2012'], 2, ',', '.')."</td>
					 			  <td>".number_format($registro['investimentorealizano2013'], 2, ',', '.')."</td>
					 			  ";

						$xls->MontaConteudoString($i, $cc, number_format($registro['investimentorealizano2005'], 2, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['investimentorealizano2006'], 2, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['investimentorealizano2007'], 2, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['investimentorealizano2008'], 2, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['investimentorealizano2009'], 2, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['investimentorealizano2010'], 2, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['investimentorealizano2011'], 2, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['investimentorealizano2012'], 2, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['investimentorealizano2013'], 2, ',', '.'));
						$cc++;

						$csv .= number_format($registro['investimentorealizano2005'], 2, ',', '.').";".number_format($registro['investimentorealizano2006'], 2, ',', '.').";".number_format($registro['investimentorealizano2007'], 2, ',', '.').";".number_format($registro['investimentorealizano2008'], 2, ',', '.').";".number_format($registro['investimentorealizano2009'], 2, ',', '.').";".number_format($registro['investimentorealizano2010'], 2, ',', '.').";".number_format($registro['investimentorealizano2011'], 2, ',', '.').";".number_format($registro['investimentorealizano2012'], 2, ',', '.').";".number_format($registro['investimentorealizano2013'], 2, ',', '.').(($cc == $colspan)?"\n":";");

					} elseif($destino[$_REQUEST['orgid']][$col]['codigo'] == "bolsasmestradorealizado") {
						$html .= "<td>".number_format($registro['bolsasmestradorealizano2005'], 0, ',', '.')."</td>
								  <td>".number_format($registro['bolsasmestradorealizano2006'], 0, ',', '.')."</td>
								  <td>".number_format($registro['bolsasmestradorealizano2007'], 0, ',', '.')."</td>
								  <td>".number_format($registro['bolsasmestradorealizano2008'], 0, ',', '.')."</td>
								  <td>".number_format($registro['bolsasmestradorealizano2009'], 0, ',', '.')."</td>
								  <td>".number_format($registro['bolsasmestradorealizano2010'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['bolsasmestradorealizano2011'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['bolsasmestradorealizano2012'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['bolsasmestradorealizano2013'], 0, ',', '.')."</td>
					 			  ";

						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasmestradorealizano2005'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasmestradorealizano2006'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasmestradorealizano2007'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasmestradorealizano2008'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasmestradorealizano2009'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasmestradorealizano2010'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasmestradorealizano2011'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasmestradorealizano2012'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasmestradorealizano2013'], 0, ',', '.'));
						$cc++;

						$csv .= number_format($registro['bolsasmestradorealizano2005'], 0, ',', '.').";".number_format($registro['bolsasmestradorealizano2006'], 0, ',', '.').";".number_format($registro['bolsasmestradorealizano2007'], 0, ',', '.').";".number_format($registro['bolsasmestradorealizano2008'], 0, ',', '.').";".number_format($registro['bolsasmestradorealizano2009'], 0, ',', '.').";".number_format($registro['bolsasmestradorealizano2010'], 0, ',', '.').";".number_format($registro['bolsasmestradorealizano2011'], 0, ',', '.').";".number_format($registro['bolsasmestradorealizano2012'], 0, ',', '.').";".number_format($registro['bolsasmestradorealizano2013'], 0, ',', '.').(($cc == $colspan)?"\n":";");

					} elseif($destino[$_REQUEST['orgid']][$col]['codigo'] == "bolsasdoutoradorealizado") {
						$html .= "<td>".number_format($registro['bolsasdoutoradorealizano2005'], 0, ',', '.')."</td>
								  <td>".number_format($registro['bolsasdoutoradorealizano2006'], 0, ',', '.')."</td>
								  <td>".number_format($registro['bolsasdoutoradorealizano2007'], 0, ',', '.')."</td>
								  <td>".number_format($registro['bolsasdoutoradorealizano2008'], 0, ',', '.')."</td>
								  <td>".number_format($registro['bolsasdoutoradorealizano2009'], 0, ',', '.')."</td>
								  <td>".number_format($registro['bolsasdoutoradorealizano2010'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['bolsasdoutoradorealizano2011'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['bolsasdoutoradorealizano2012'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['bolsasdoutoradorealizano2013'], 0, ',', '.')."</td>
					 			  ";

						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasdoutoradorealizano2005'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasdoutoradorealizano2006'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasdoutoradorealizano2007'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasdoutoradorealizano2008'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasdoutoradorealizano2009'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasdoutoradorealizano2010'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasdoutoradorealizano2011'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasdoutoradorealizano2012'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasdoutoradorealizano2013'], 0, ',', '.'));
						$cc++;

						$csv .= number_format($registro['bolsasdoutoradorealizano2005'], 0, ',', '.').";".number_format($registro['bolsasdoutoradorealizano2006'], 0, ',', '.').";".number_format($registro['bolsasdoutoradorealizano2007'], 0, ',', '.').";".number_format($registro['bolsasdoutoradorealizano2008'], 0, ',', '.').";".number_format($registro['bolsasdoutoradorealizano2009'], 0, ',', '.').";".number_format($registro['bolsasdoutoradorealizano2010'], 0, ',', '.').";".number_format($registro['bolsasdoutoradorealizano2011'], 0, ',', '.').";".number_format($registro['bolsasdoutoradorealizano2012'], 0, ',', '.').";".number_format($registro['bolsasdoutoradorealizano2013'], 0, ',', '.').(($cc == $colspan)?"\n":";");

					} elseif($destino[$_REQUEST['orgid']][$col]['codigo'] == "bolsasposdoutoradorealizado") {
						$html .= "<td>".number_format($registro['bolsasposdoutoradorealizano2005'], 0, ',', '.')."</td>
								  <td>".number_format($registro['bolsasposdoutoradorealizano2006'], 0, ',', '.')."</td>
								  <td>".number_format($registro['bolsasposdoutoradorealizano2007'], 0, ',', '.')."</td>
								  <td>".number_format($registro['bolsasposdoutoradorealizano2008'], 0, ',', '.')."</td>
								  <td>".number_format($registro['bolsasposdoutoradorealizano2009'], 0, ',', '.')."</td>
								  <td>".number_format($registro['bolsasposdoutoradorealizano2010'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['bolsasposdoutoradorealizano2011'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['bolsasposdoutoradorealizano2012'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['bolsasposdoutoradorealizano2013'], 0, ',', '.')."</td>
					 			  ";

						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasposdoutoradorealizano2005'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasposdoutoradorealizano2006'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasposdoutoradorealizano2007'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasposdoutoradorealizano2008'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasposdoutoradorealizano2009'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasposdoutoradorealizano2010'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasposdoutoradorealizano2011'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasposdoutoradorealizano2012'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['bolsasposdoutoradorealizano2013'], 0, ',', '.'));
						$cc++;

						$csv .= number_format($registro['bolsasposdoutoradorealizano2005'], 0, ',', '.').";".number_format($registro['bolsasposdoutoradorealizano2006'], 0, ',', '.').";".number_format($registro['bolsasposdoutoradorealizano2007'], 0, ',', '.').";".number_format($registro['bolsasposdoutoradorealizano2008'], 0, ',', '.').";".number_format($registro['bolsasposdoutoradorealizano2009'], 0, ',', '.').";".number_format($registro['bolsasposdoutoradorealizano2010'], 0, ',', '.').";".number_format($registro['bolsasposdoutoradorealizano2011'], 0, ',', '.').";".number_format($registro['bolsasposdoutoradorealizano2012'], 0, ',', '.').";".number_format($registro['bolsasposdoutoradorealizano2013'], 0, ',', '.').(($cc == $colspan)?"\n":";");

					} elseif($destino[$_REQUEST['orgid']][$col]['codigo'] == "matriculasgradpresen") {
						$html .= "<td>".number_format($registro['matriculasgradpresenano2005'], 0, ',', '.')."</td>
								  <td>".number_format($registro['matriculasgradpresenano2006'], 0, ',', '.')."</td>
								  <td>".number_format($registro['matriculasgradpresenano2007'], 0, ',', '.')."</td>
								  <td>".number_format($registro['matriculasgradpresenano2008'], 0, ',', '.')."</td>
								  <td>".number_format($registro['matriculasgradpresenano2009'], 0, ',', '.')."</td>
								  <td>".number_format($registro['matriculasgradpresenano2010'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['matriculasgradpresenano2011'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['matriculasgradpresenano2012'], 0, ',', '.')."</td>
					 			  <td>".number_format($registro['matriculasgradpresenano2013'], 0, ',', '.')."</td>
					 			  ";

						$xls->MontaConteudoString($i, $cc, number_format($registro['matriculasgradpresenano2005'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['matriculasgradpresenano2006'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['matriculasgradpresenano2007'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['matriculasgradpresenano2008'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['matriculasgradpresenano2009'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['matriculasgradpresenano2010'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['matriculasgradpresenano2011'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['matriculasgradpresenano2012'], 0, ',', '.'));
						$cc++;
						$xls->MontaConteudoString($i, $cc, number_format($registro['matriculasgradpresenano2013'], 0, ',', '.'));
						$cc++;

						$csv .= number_format($registro['matriculasgradpresenano2005'], 0, ',', '.').";".number_format($registro['matriculasgradpresenano2006'], 0, ',', '.').";".number_format($registro['matriculasgradpresenano2007'], 0, ',', '.').";".number_format($registro['matriculasgradpresenano2008'], 0, ',', '.').";".number_format($registro['matriculasgradpresenano2009'], 0, ',', '.').";".number_format($registro['matriculasgradpresenano2010'], 0, ',', '.').";".number_format($registro['matriculasgradpresenano2011'], 0, ',', '.').";".number_format($registro['matriculasgradpresenano2012'], 0, ',', '.').";".number_format($registro['matriculasgradpresenano2013'], 0, ',', '.').(($cc == $colspan)?"\n":";");

					} elseif($destino[$_REQUEST['orgid']][$col]['codigo'] == "enderecocampus") {

						$tam = strlen( $registro['cepcampus'] );
                       	$cep = substr( $registro['cepcampus'], 0, 5 ) . '-' . substr( $registro['cepcampus'], 5, $tam );

						$html .= "<td>".$cep."</td>
								  <td>".$registro['bairrocampus']."</td>
					 			  <td>".$registro['enderecocampus']."</td>
					 			  <td>".$registro['numerocampus']."</td>";

						$xls->MontaConteudoString($i, $cc, $cep);
						$cc++;
						$xls->MontaConteudoString($i, $cc, $registro['bairrocampus']);
						$cc++;
						$xls->MontaConteudoString($i, $cc, $registro['enderecocampus']);
						$cc++;
						$xls->MontaConteudoString($i, $cc, $registro['numerocampus']);
						$cc++;

						$csv .= $cep.";".$registro['bairrocampus'].";".$registro['enderecocampus'].";".$registro['numerocampus'].(($cc == $colspan)?"\n":";");

					} elseif($destino[$_REQUEST['orgid']][$col]['codigo'] == "enderecounidadeorc") {

						$tam = strlen( $registro['cepuniversidade'] );
                       	$cep = substr( $registro['cepuniversidade'], 0, 5 ) . '-' . substr( $registro['cepuniversidade'], 5, $tam );

                       	$html .= "<td>".$cep."</td>
								  <td>".$registro['bairrouniversidade']."</td>
					 			  <td>".$registro['enderecouniversidade']."</td>
					 			  <td>".$registro['numerouniversidade']."</td>";

						$xls->MontaConteudoString($i, $cc, $cep);
						$cc++;
						$xls->MontaConteudoString($i, $cc, $registro['bairrouniversidade']);
						$cc++;
						$xls->MontaConteudoString($i, $cc, $registro['enderecouniversidade']);
						$cc++;
						$xls->MontaConteudoString($i, $cc, $registro['numerouniversidade']);
						$cc++;

						$csv .= $cep.";".$registro['bairrouniversidade'].";".$registro['enderecouniversidade'].";".$registro['numerouniversidade'].(($cc == $colspan)?"\n":";");

					} else {
						$html .= "<td>".$registro[$col]."</td>";

						$xls->MontaConteudoString($i, $cc, $registro[$col]);
						$cc++;

						$csv .= $registro[$col].(($cc == $colspan)?"\n":";");
					}

				}

				$html .= "</tr>";
			}


			$i++;

		}

		$html .= "</table>";

		$msg = $_POST['obs_consulta'];
		//Roda pé em HTML: Observação feita pelo usuário na mantagem do relatório.
		$html .= "
			<table class='tabela'>
				<tr bgcolor=\"#C0C0C0\">
					<td colspan='27' align='center'>\n
						OBSERVAÇÃO: ".$msg."
					</td>
				</tr>
			</table>";

		//Roda pé em XLS: Observação feita pelo usuário na mantagem do relatório.
		$xls->MontaConteudoString($i+1, 0, 'OBSERVAÇÃO:');
		$xls->MontaConteudoString($i+1, 1, $msg);

		//Roda pé em CSV: Observação feita pelo usuário na mantagem do relatório.
		$csv .= "\n\n"."OBSERVAÇÃO: ".";".$msg;

	} else {
		$html = "<link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/Estilo.css\"/>
				 <link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/listagem.css\"/>
				 <table class='tabela'><tr bgcolor=\"#C0C0C0\"><td colspan='27' align='center'>MINISTÉRIO DA EDUCAÇÃO<br>Responsável pela informação: Secretaria de Educação Profissional e Tecnológica (SETEC)<br><br>Expansão da Educação Profissional e Tecnológica</td></tr><tr><td style='color:#ff0000; text-align:center'>Não foram encontrados registros.</td></tr></table>";
	}

	$nome_arquivo = "relatorio_nota_tecnica_".date('d-m-Y_H_i');
	switch(substr($_POST['exibir'],10,3)) {
		case 'csv':
			header("Content-type: application/octet-stream");
			header("Content-Disposition: attachment; filename=\"$nome_arquivo\"");
			echo $csv;
			break;
		case 'xls':
			ob_clean();
			$xls->GeraArquivo();
			break;
		case 'htm':
			echo $html;
			break;

	}
	exit;
}


//if(substr($_POST['exibir'],0,9) == "relatorio") {

if(	($_POST['exibir'] == 'relatorio_html' || $_POST['exibir'] == 'relatorio_xls' || $_POST['exibir'] == 'relatorio_csv')	){

	$filtroSQL[] = "tuo.orgid IN('".$_POST['orgid']."')";
	$filtroSQL[] = "fen.funid IN (" . ($_POST['orgid'] == 2 ? 17 : 18) . ")";

	unset($vals);
	if($_POST['estuf'][0]) {
		foreach($_POST['estuf'] as $uf) {
			$vals[] = $uf;
		}
		$filtroSQL[] = "edc.estuf IN('".implode("','",$vals)."')";
	}
	unset($vals);
	if($_POST['muncod'][0]) {
		foreach($_POST['muncod'] as $municipio) {
			$vals[] = $municipio;
		}
		$filtroSQL[] = "edc.muncod IN('".implode("','",$vals)."')";
	}
	unset($vals);
	if($_POST['unidades'][0]) {
		foreach($_POST['unidades'] as $uni) {
			$vals[] = $uni;
		}
		$filtroSQL[] = "uor.entid IN('".implode("','",$vals)."')";
	}
	if($_POST['exiid']) {
		$filtroSQL[] = "cam.exiid = '".$_POST['exiid']."'";
	}
	if($_POST['cmpsituacao']) {
		$filtroSQL[] = "cam.cmpsituacao = '".$_POST['cmpsituacao']."'";
	}
	if($_POST['cmpinstalacao']) {
		$filtroSQL[] = "cam.cmpinstalacao = '".$_POST['cmpinstalacao']."'";
	}
	if($_POST['cmpsituacaoobra']) {
		$filtroSQL[] = "cam.cmpsituacaoobra = '".$_POST['cmpsituacaoobra']."'";
	}
	/* Gerar cabeçalho XLS */
	$nomeDoArquivoXls = "relatorio_nota_tecnica_".date('d-m-Y_H_i');
	switch($_REQUEST['orgid']) {
		case TPENSSUP:
			mostrar_superior($filtroSQL);
			break;
		case TPENSPROF:
			mostrar_profissional($filtroSQL);
			break;
	}

}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';
require_once APPRAIZ . "adodb/adodb.inc.php";


echo "<br/>";
$titulo_modulo = "Sistema de Informações Gerenciais";
monta_titulo( $titulo_modulo,'Relatório Nota Técnica');
?>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>

<script>

	function showColunas(v) {
		document.getElementById('coluna_1').style.display = 'none';
		document.getElementById('coluna_2').style.display = 'none';
		document.getElementById('coluna_'+v).style.display = '';
	}

	function abrirPopup(){
		window.open('', 'abrirPopup', 'toolbar=no,location=no,directories=no,status=no,menubar=no,scrollbars=yes,resizable=yes,copyhistory=yes,width=800,height=600');
	}

	function abrepopupUnidade(){
		for(i=0;i<document.formulario.orgid.length;i++) {
			if(document.formulario.orgid[i].checked) {
				var itemselecionado = document.formulario.orgid[i].value;
			}
		}
		janela = window.open('/sig/combo_unidades.php?tipoensino='+itemselecionado,'Unidades','width=400,height=400,toolbar=yes,status=yes,scrollbars=1,top=200,left=480');
		janela.focus();
	}

	function exibirRelatorio(){
		document.getElementById('carregar').value = "";

		var formulario = document.formulario
		var radioselecionado = false;

		for(i=0;i<document.formulario.orgid.length;i++) {
			if(document.formulario.orgid[i].checked) {
				radioselecionado = true;
			}
		}
		if(!radioselecionado) {
			alert('"Tipo de ensino" é obrigatório.');
			return false;
		}

		selectAllOptions(document.formulario.agrupador_1);
		selectAllOptions(document.formulario.agrupador_2);

		selectAllOptions(document.formulario.estuf);
		selectAllOptions(document.formulario.unidades);
		selectAllOptions(document.formulario.muncod);

		formulario.submit();
	}

	/*
	 * Altera a visibilidade do Bloco - visualizar consulta 'ABRE' o bloco.
	 * @param string indica o campo a ser mostrado/escondido
	 * @return void
	 */
	function onOffBloco(bloco){
		var div_on 	= document.getElementById( bloco + '_div_filtros_on' );
		var div_off = document.getElementById( bloco + '_div_filtros_off' );
		var img 	= document.getElementById( bloco + '_img' );
		var input 	= document.getElementById( bloco + '_flag' );

		if(div_on.style.display == 'none'){
			div_on.style.display 	= 'block';
			div_off.style.display 	= 'none';
			input.value 			= '0';
			img.src 				= '/imagens/menos.gif';
		}else{
			div_on.style.display 	= 'none';
			div_off.style.display 	= 'block';
			input.value 			= '1';
			img.src	 				= '/imagens/mais.gif';
		}
	}

	 function tornar_publico( prtid ){
		document.getElementById('publico').value = '1';
		document.getElementById('prtid').value = prtid;
		document.getElementById('formulario').action = 'academico.php?modulo=relatorio/download&acao=A';
		document.getElementById('formulario').target = '_self';
		document.getElementById('formulario').submit();
	}

	function excluir_relatorio( prtid ){
		if(confirm( 'Tem certeja que deseja excluir o registro?' )){
			document.getElementById('excluir').value = '1';
			document.getElementById('prtid').value = prtid;
			document.getElementById('formulario').action = 'academico.php?modulo=relatorio/download&acao=A';
			document.getElementById('formulario').target = '_self';
			document.getElementById('formulario').submit();
		}
	}

	function carregar_relatorio( prtid ){
		document.getElementById('carregar').value = '1';
		document.getElementById('prtid').value = prtid;
		document.getElementById('formulario').action = 'academico.php?modulo=relatorio/download&acao=A';
		document.getElementById('formulario').target = '_blank';
		document.getElementById('formulario').submit();
	}

	function salvarConsultaEfetuada(){
		var formulario = document.formulario;

		if ( document.getElementById('titulo_consulta').value == '' ) {
			alert( 'É necessário informar a descrição do relatório!' );
			document.getElementById('titulo_consulta').focus();
			return;
		}

		jQuery("#div_selecao").css("visibility", "visible");

		var nomesExistentes = new Array();
		<?php
			$sqlNomesConsulta = "SELECT prtdsc FROM public.parametros_tela Where usucpf ='".$_SESSION['usucpf']."'and mnuid =".$_SESSION['mnuid'].";";
			$nomesExistentes = $db->carregar( $sqlNomesConsulta );
			if($nomesExistentes){
				foreach($nomesExistentes as $linhaNome){
					print "nomesExistentes[nomesExistentes.length] = '" . str_replace( "'", "\'", $linhaNome['prtdsc'] ) . "';";
				}
			}
		?>
		var confirma = true;
		var i, j = nomesExistentes.length;
		for(i = 0; i < j; i++){
			if(nomesExistentes[i] == document.getElementById('titulo_consulta').value){
				confirma = confirm( 'Deseja alterar a consulta já existente?');
				break;
			}
		}
		if(!confirma){
			return;
		}

		selectAllOptions(document.getElementById('agrupador_1'));
		selectAllOptions(document.getElementById('agrupador_2'));

		selectAllOptions(document.formulario.estuf);
		selectAllOptions(document.formulario.unidades);
		selectAllOptions(document.formulario.muncod);
	}

	function escolhaRelatorio(){
		jQuery("#div_selecao").css("visibility", "hidden");
    	document.getElementById('formulario').action = 'academico.php?modulo=relatorio/download&acao=A&requisicao=salvarConsulta';
		document.getElementById('formulario').target = '_self';
		document.getElementById('formulario').submit();
	}

</script>

<style type="text/css">
  	#div_selecao {
  		visibility: hidden;
    	border-style: solid;
    	border-width: 1px;
    	borborder-color: black;
    	background-color: #DCDCDC;
    	width: 23%;
    	height: 13%;
    	position: absolute;
    	top: 48%;
    	left: 35%;
	}

</style>


<form method="post" name="formulario" id="formulario" target="abrirPopup">

	<input type="hidden" name="publico" id="publico" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado -->
	<input type="hidden" name="prtid" id="prtid" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado, passa o prtid -->
	<input type="hidden" name="carregar" id="carregar" value=""/> <!-- indica se foi clicado para carregar o relatório -->
	<input type="hidden" name="excluir" id="excluir" value=""/> <!-- indica se foi clicado para excluir o relatório já gravado -->
	<input type="hidden" id="exibir" name="exibir" value="">

	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">

		<!-- HISTORICO DE CONSULTAS - MINHAS CONSULTAS -->
		<table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
			<tr>
				<td onclick="javascript:onOffBloco('minhasconsultas');" >
					<img border="0" src="/imagens/mais.gif" id="minhasconsultas_img"/>&nbsp;
					Minhas Consultas
					<input type="hidden" id="minhasconsultas_flag" name="minhasconsultas_flag" value="0" />
				</td>
			</tr>
		</table>

		<div name="minhasconsultas_div_filtros_off" id="minhasconsultas_div_filtros_off"></div>
		<div id="minhasconsultas_div_filtros_on" style="display:none;">
			<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-top:none;">
				<tr>
					<td width="195" class="SubTituloDireita" valign="top">Consultas</td>
						<?php
							$sql = sprintf("
									Select Case
											When prtpublico = false
												Then '<img border=\"0\" src=\"../imagens/grupo.gif\" title=\" Publicar \" onclick=\"tornar_publico(' || prtid || ')\">&nbsp;&nbsp;
													  <img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ')\">&nbsp;&nbsp;
													  <img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">'
												Else '<img border=\"0\" src=\"../imagens/grupo_bloqueado.gif\" title=\" Sem ação \">&nbsp;&nbsp;
													  <img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ')\">&nbsp;&nbsp;
													  <img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">'
											End as acao,
											'<b>' || prtdsc || '</b>' as descricao,
											'<b>' || prtobservacao || '</b>' as observacao
									From public.parametros_tela
									Where mnuid = %d AND usucpf = '%s'",
									$_SESSION['mnuid'],
									$_SESSION['usucpf']
							);
							$cabecalho = array('Ação', 'Nome', 'Observações');
						?>
					<td>
						<?php $db->monta_lista_simples( $sql, $cabecalho, 50, 50, 'N', '95%', null ); ?>
					</td>
				</tr>
			</table>
		</div>
		<!-- FIM - HISTORICO DE CONSULTAS - MINHAS CONSULTAS -->


		<!-- CONSULTAS PUBLICAS - CONSULTAS QUE USUÁRIOS DISPONIBILIZÃO -->
		<table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
			<tr>
				<td onclick="javascript:onOffBloco('outros');">
					<img border="0" src="/imagens/mais.gif" id="outros_img"/>&nbsp;
					Relatórios Gerenciais
					<input type="hidden" id="outros_flag" name="outros_flag" value="0" />
				</td>
			</tr>
		</table>

		<div id="outros_div_filtros_off"></div>
		<div id="outros_div_filtros_on" style="display:none;">
			<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-top:none;">
				<tr>
					<td width="195" class="SubTituloDireita" valign="top">Relatórios:</td>
					<td>
					<?php
						$sql = sprintf("
									Select	Case
											  When prtpublico = true and usucpf = '%s'
												Then '<img border=\"0\" src=\"../imagens/usuario.gif\" title=\" Despublicar \" onclick=\"tornar_publico(' || prtid || ');\">&nbsp;&nbsp;
													  <img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ');\">&nbsp;&nbsp;
													  <img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">'
												Else '<img border=\"0\" src=\"../imagens/usuario_bloqueado.gif\" title=\" Sem ação \">&nbsp;&nbsp;
													  <img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ');\">&nbsp;&nbsp;
													  <img border=\"0\" src=\"../imagens/excluir_01.gif\" title=\" Sem ação \">'
											End as acao,
											'<b>' || prtdsc || '</b>' as descricao,
											'<b>' || prtobservacao || '</b>' as observacao
									From public.parametros_tela
									Where mnuid = %d and prtpublico = TRUE
								",
								$_SESSION['usucpf'],
								$_SESSION['mnuid'],
								$_SESSION['usucpf']);

						$cabecalho = array('Ação', 'Nome', 'Observações');
						$db->monta_lista_simples( $sql, $cabecalho, 50, 50, null, null, null );
					?>
					</td>
				</tr>
			</table>
		</div>
		<!-- FIM DA CONSULTAS PUBLICAS - CONSULTAS QUE USUÁRIOS DISPONIBILIZÃO -->

		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
			<tr>
				<td class="SubTituloDireita">Definir título da consulta</td>
				<td>
					<?= campo_texto( 'titulo_consulta', 'N', 'S', '', 65, 60, '', '', 'left', '', 0, 'id="titulo_consulta"'); ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Definir observações para consulta</td>
				<td>
					<?= campo_textarea( 'obs_consulta', 'N', 'S', '', 69, 3, '', '', '', '', '', 'id="obs_consulta"'); ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" valign="top">Tipo de ensino</td>
				<td>
					<?
						$sqlTipoEnsino = "SELECT * FROM academico.orgao WHERE orgid IN (1,2) ORDER BY orgdesc";
						$tipoen = $db->carregar($sqlTipoEnsino);
						if($tipoen[0]) {
							$prim = true;
							foreach($tipoen as $tpe) {
								echo "<input type='radio' name='orgid' value='".$tpe['orgid']."' ".(($prim)?"checked":"")." onclick=\"if(this.checked){showColunas('".$tpe['orgid']."');}\"> ".$tpe['orgdesc']."&nbsp;&nbsp;";
								$prim = false;
							}
						}
					?>
				</td>
			</tr>
			<tr id="coluna_1" style="display:none;">
				<td class="SubTituloDireita" valign="top" width="20%">Colunasss</td>
				<td width="80%">
<?php
$agrupadorHtml =
<<<EOF
    <table>
        <tr valign="middle">
            <td>
                <select id="{NOME_ORIGEM}" name="{NOME_ORIGEM}[]" multiple="multiple" size="4" style="width: 260px; height: 120px;" onDblClick="moveSelectedOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );" class="combo campoEstilo"></select>
            </td>
            <td>
                <img src="../imagens/rarrow_one.gif" style="padding: 5px" onClick="moveSelectedOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );"/><br/>

                <img src="../imagens/rarrow_all.gif" style="padding: 5px" onClick="moveAllOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );"/><br/>
                <img src="../imagens/larrow_all.gif" style="padding: 5px" onClick="moveAllOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, ''); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );"/><br/>

                <img src="../imagens/larrow_one.gif" style="padding: 5px" onClick="moveSelectedOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, '' ); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );"/><br/>
            </td>
            <td>
                <select id="{NOME_DESTINO}" name="{NOME_DESTINO}[]" multiple="multiple" size="4" style="width: 260px; height: 120px;" onDblClick="moveSelectedOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, '' ); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );" class="combo campoEstilo"></select>
            </td>
            <td>
                <img src="../imagens/uarrow.gif" style="padding: 5px" onClick="subir( document.getElementById( '{NOME_DESTINO}' ) );"/><br/>
                <img src="../imagens/darrow.gif" style="padding: 5px" onClick="descer( document.getElementById( '{NOME_DESTINO}' ) );"/><br/>
            </td>
        </tr>
    </table>
    <script type="text/javascript" language="javascript">
        limitarQuantidade( document.getElementById( '{NOME_DESTINO}' ), {QUANTIDADE_DESTINO} );
        limitarQuantidade( document.getElementById( '{NOME_ORIGEM}' ), {QUANTIDADE_ORIGEM} );
        {POVOAR_ORIGEM}
        {POVOAR_DESTINO}
        sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );
    </script>
EOF;

	$agrupador = new Agrupador( 'formulario', $agrupadorHtml );
	//exibe agrupador
	$agrupador->setOrigem( 'naoAgrupador_'.TPENSSUP, null, $origem[TPENSSUP] );
	$agrupador->setDestino( 'agrupador_'.TPENSSUP, null, $destino[TPENSSUP] );
	$agrupador->exibir();
?>
				</td>
			</tr>
			<tr id="coluna_2" style="display:none;">
				<td class="SubTituloDireita" valign="top" width="20%">Colunas</td>
				<td width="80%">
<?php
$agrupadorHtml =
<<<EOF
    <table>
        <tr valign="middle">
            <td>
                <select id="{NOME_ORIGEM}" name="{NOME_ORIGEM}[]" multiple="multiple" size="4" style="width: 260px; height: 120px;" onDblClick="moveSelectedOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );" class="combo campoEstilo"></select>
            </td>
            <td>
                <img src="../imagens/rarrow_one.gif" style="padding: 5px" onClick="moveSelectedOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );"/><br/>

                <img src="../imagens/rarrow_all.gif" style="padding: 5px" onClick="moveAllOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );"/><br/>
                <img src="../imagens/larrow_all.gif" style="padding: 5px" onClick="moveAllOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, ''); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );"/><br/>

                <img src="../imagens/larrow_one.gif" style="padding: 5px" onClick="moveSelectedOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, '' ); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );"/><br/>
            </td>
            <td>
                <select id="{NOME_DESTINO}" name="{NOME_DESTINO}[]" multiple="multiple" size="4" style="width: 260px; height: 120px;" onDblClick="moveSelectedOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, '' ); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );" class="combo campoEstilo"></select>
            </td>
            <td>
                <img src="../imagens/uarrow.gif" style="padding: 5px" onClick="subir( document.getElementById( '{NOME_DESTINO}' ) );"/><br/>
                <img src="../imagens/darrow.gif" style="padding: 5px" onClick="descer( document.getElementById( '{NOME_DESTINO}' ) );"/><br/>
            </td>
        </tr>
    </table>
    <script type="text/javascript" language="javascript">
        limitarQuantidade( document.getElementById( '{NOME_DESTINO}' ), {QUANTIDADE_DESTINO} );
        limitarQuantidade( document.getElementById( '{NOME_ORIGEM}' ), {QUANTIDADE_ORIGEM} );
        {POVOAR_ORIGEM}
        {POVOAR_DESTINO}
        sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );
    </script>
EOF;

	$agrupador = new Agrupador( 'formulario', $agrupadorHtml );
	//exibe agrupador
	$agrupador->setOrigem( 'naoAgrupador_'.TPENSPROF, null, $origem[TPENSPROF] );
	$agrupador->setDestino( 'agrupador_'.TPENSPROF, null, $destino[TPENSPROF] );
	$agrupador->exibir();
?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">Unidades da Federação</td>
			<td>
				<?php
					$sqlComboEstado = "
							select estuf as codigo, estdescricao as descricao
							from territorios.estado
							order by estdescricao
					";
					$estuf = array(0 => array("codigo" => "", "descricao" => "Todas"));
					combo_popup("estuf", $sqlComboEstado, "Estado", "400x400", 0, array(), "", "S", false, false, 5, 400);
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">Municípios</td>
			<td>
				<?php
					$sqlComboMunicipio = "
							select muncod as codigo, estuf || ' - ' || mundescricao as descricao
							from territorios.municipio
							order by estuf, mundescricao
					";
					$muncod = array(0 => array("codigo" => "", "descricao" => "Todas"));
					combo_popup("muncod", $sqlComboMunicipio, "Município", "400x400", 0, array(), "", "S", false, false, 5, 400);
				?>
			</td>
			</tr>
		    <tr>
		        <td class="SubTituloDireita" valign="top">Unidades</td>
		        <td>
		        <select multiple="multiple" size="5" name="unidades[]" id="unidades" ondblclick="abrepopupUnidade();" class="CampoEstilo" style="width:400px;">
		        <option value="">Todas</option>
		        </select>
		        </td>
			</tr>
		    <tr>
		        <td class="SubTituloDireita" valign="top">Existência</td>
		        <td>
		        <?
				$sql = "SELECT exiid as codigo, exidsc as descricao FROM academico.existencia WHERE existatus='A'";
				$db->monta_combo('exiid', $sql, 'S', 'Selecione', '', '', '', '200', 'N', 'exiid', '');
		        ?>
		        </td>
		  	</tr>
		    <tr>
		        <td class="SubTituloDireita" valign="top">Situação</td>
		        <td><input type="radio" name="cmpsituacao" value="" <?=$cmpsituacao?'':'checked' ?>> Todos <input type="radio" <?=$cmpsituacao=='F'?'checked':'' ?> name="cmpsituacao" value="F"> Funcionando <input type="radio" name="cmpsituacao" <?=$cmpsituacao=='N'?'checked':'' ?>  value="N"> Não Funcionando</td>
		   	</tr>
		    <tr>
		        <td class="SubTituloDireita" valign="top">Instalações</td>
		        <td><input type="radio" name="cmpinstalacao" value="" <?=$cmpinstalacao?'':'checked' ?>> Todos <input type="radio" name="cmpinstalacao"  <?=$cmpinstalacao=='P'?'checked':'' ?>  value="P"> Provisória <input type="radio" <?=$cmpinstalacao=='D'?'checked':'' ?> name="cmpinstalacao" value="D"> Definitiva <input type="radio"  <?=$cmpinstalacao=='S'?'checked':'' ?>  name="cmpinstalacao" value="S"> Sem Instalações</td>
		   	</tr>
		   	<!--
			<tr>
				<td class="SubTituloDireita" valign="top">Situação de Obras no Campus</td>
				<td><input type="radio" name="cmpsituacaoobra" value="" checked> Todos <input type="radio" name="cmpsituacaoobra" value="L"> Licitação da Obra <input type="radio" name="cmpsituacaoobra" value="A"> Obras em Andamento <input type="radio" name="cmpsituacaoobra" value="C">  Obras Concluídas <input type="radio" name="cmpsituacaoobra" value="E">  Elaboração de Projetos  <input type="radio" name="cmpsituacaoobra" value="D">  Dominialidade de Imóvel</td>
			</tr>
			-->
		   	<tr bgcolor="#C0C0C0">
		   		<td>&nbsp;</td>
				<td>
					<input type="submit" value="Gerar CSV" onclick="document.getElementById('exibir').value='relatorio_csv';exibirRelatorio();">
					<input type="submit" value="Gerar XLS" onclick="document.getElementById('exibir').value='relatorio_xls';exibirRelatorio();">
					<input type="submit" value="Gerar HTML" onclick="document.getElementById('exibir').value='relatorio_html';exibirRelatorio();">

					<input type="button" value="Salvar Consulta" onclick="javascript:salvarConsultaEfetuada();" style="cursor: pointer;"/>
				</td>
		   	</tr>
		</table>
			<div id="div_selecao" name="div_selecao">
				<br><b style="margin-left: 6%;">Selecione o tipo de relatório que deseja que seja gerado</b><br>
				<input type="radio" name="rd_relatorio" id="rd_relatorio" value="relatorio_html" onclick="document.getElementById('exibir').value='relatorio_html';escolhaRelatorio();">Relatório em HTML <br/>
				<input type="radio" name="rd_relatorio" id="rd_relatorio" value="relatorio_xls" onclick="document.getElementById('exibir').value='relatorio_xls';escolhaRelatorio();">Relatório em XLS<br/>
				<input type="radio" name="rd_relatorio" id="rd_relatorio" value="relatorio_csv" onclick="document.getElementById('exibir').value='relatorio_csv';escolhaRelatorio();">Relatório em CSV
			</div>
		</form>
<script>
showColunas('2');
</script>
<?php
if ($_REQUEST['painel'])
echo "<script>$(document).ready(function (){
	moveAllOptions(  document.getElementById( 'agrupador_2' ), document.getElementById( 'naoAgrupador_2' ), true, '');
	$('#agrupador_2').append(new Option('Instituto', 'unidadeorc', true, true));
	$('#agrupador_2').append(new Option('Campus', 'entnome', true, true));
	$('#agrupador_2').append(new Option('Existência', 'exidsc', true, true));
	$('#agrupador_2').append(new Option('Situação', 'cmpsituacao', true, true));
	$('#agrupador_2').append(new Option('Instalações', 'cmpinstalacao', true, true));

	document.getElementById('exibir').value='relatorio_html';
	exibirRelatorio();

});</script>"

?>