<?php
class Model_Painel_Indicador extends Zend_Db_Table 
{
protected $_schema = 'painel';
    protected $_name = 'caixapesquisa';

    public function getGrafico(){

    }

    public function getDetalhamentoEscolas($codInep)
    {

        $sql = "select
				ind.indnome,
				exo.exodsc,
				sec.secdsc,
				aca.acadsc,
				ind.indobjetivo,
				ind.indcumulativo,
				ind.regid,
				unm.unmdesc,
				ind.indformula,
				ind.indtermos,
				ind.indfontetermo,
				ind.indobservacao,
				ind.indvispadrao,
				per.perdsc,
				est.estdsc,
				col.coldsc,
				reg.regdescricao,
				ume.umedesc
			from
				painel.indicador ind
			left join
				painel.eixo exo ON exo.exoid = ind.exoid
			left join
				painel.secretaria sec ON sec.secid = ind.secid
			left join
				painel.acao aca ON aca.acaid = ind.acaid
			left join
				painel.unidademedicao unm ON unm.unmid = ind.unmid
			left join
				painel.periodicidade per ON per.perid = ind.perid
			left join
				painel.unidademeta ume on ind.umeid = ume.umeid
			left join
				painel.estilo est ON est.estid = ind.estid
			left join
				painel.coleta col ON col.colid = ind.colid
			left join
				painel.regionalizacao reg ON reg.regid = ind.regid
			where
				ind.indid = {$codInep}
			limit 1";


        return $this->getDefaultAdapter()->query($sql)->fetchAll();


    }

    public function getMapaEscolas($indid,$dshcod)
    {


        $sql1 = "SELECT dpe.dpeanoref FROM painel.seriehistorica seh
					  	 LEFT JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid
						 WHERE seh.indid='" . $indid . "' AND seh.sehstatus='A'";

        $ano = $this->getDefaultAdapter()->query($sql1)->fetchColumn(0);

        $sql = "SELECT muncod, mundescricao, estuf, munmedlat, munmedlog, CASE WHEN unmid IN('5','1','2') THEN CASE WHEN unmid='5' THEN 'R$ '||trim(to_char(SUM(qtde), '999g999g999g999d99')) ELSE trim(to_char(SUM(qtde), '999g999g999g999d99')) END ELSE trim(to_char(SUM(qtde), '999g999g999g999')) END as qtde, CASE WHEN indqtdevalor = TRUE THEN trim(to_char(sum(valor), '999g999g999g999d99')) END as valor FROM (
            SELECT	mun.muncod,
					mun.mundescricao,
					est.estuf,

					st_y(mun.munlatlong) as munmedlat,

			        st_x(mun.munlatlong) as munmedlog,


				CASE 	WHEN d.indcumulativo='S' THEN sum(d.qtde)
					WHEN d.indcumulativo='N' THEN
						CASE WHEN d.sehstatus='A' THEN sum(d.qtde)
						ELSE 0 END
					WHEN d.indcumulativo='A' THEN
						CASE when d.dpeanoref='{$ano}' THEN sum(d.qtde)
						ELSE 0 END
					END as qtde,
				CASE 	WHEN d.indcumulativovalor='S' THEN sum(d.valor)
					WHEN d.indcumulativovalor='N' THEN
						CASE when d.sehstatus='A' THEN sum(d.valor)
						ELSE 0 END
					WHEN d.indcumulativovalor='A' then
						CASE when d.dpeanoref='{$ano}' THEN sum(d.valor)
						ELSE 0 end
					END as valor,
				d.indqtdevalor,
				d.unmid

	       FROM painel.v_detalheindicadorsh d
	       LEFT JOIN territoriosgeo.municipio mun ON mun.muncod = d.dshcodmunicipio
	       LEFT JOIN territorios.estado est ON est.estuf = d.dshuf
	       LEFT JOIN territorios.mesoregiao mes ON mes.mescod = mun.mescod
	       WHERE d.indid='{$indid}' AND d.dshcod='{$dshcod}'
		   GROUP BY mun.muncod, est.estuf, mundescricao, mun.munlatlong, d.indcumulativo, d.sehstatus, d.dpeanoref, d.indcumulativovalor, d.indqtdevalor, d.unmid
	       ORDER BY est.estuf, mundescricao
		) as foo
		WHERE qtde!=0 OR valor!=0
		GROUP BY muncod, mundescricao, estuf, munmedlat, munmedlog, indqtdevalor, unmid
		ORDER BY estuf, mundescricao";

        return $this->getDefaultAdapter()->query($sql)->fetchAll();
    }


        public function getMapaMunicipios($indid,$dshcod){


            $sql1 = "SELECT dpe.dpeanoref FROM painel.seriehistorica seh
					  	 LEFT JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid
						 WHERE seh.indid='".$indid."' AND seh.sehstatus='A'";

            $ano = $this->getDefaultAdapter()->query($sql1)->fetchColumn(0);

            $sql = "SELECT muncod, mundescricao, estuf, munmedlat, munmedlog, CASE WHEN unmid IN('5','1','2') THEN CASE WHEN unmid='5' THEN 'R$ '||trim(to_char(SUM(qtde), '999g999g999g999d99')) ELSE trim(to_char(SUM(qtde), '999g999g999g999d99')) END ELSE trim(to_char(SUM(qtde), '999g999g999g999')) END as qtde, CASE WHEN indqtdevalor = TRUE THEN trim(to_char(sum(valor), '999g999g999g999d99')) END as valor FROM (
            SELECT	mun.muncod,
					mun.mundescricao,
					est.estuf,

					st_y(mun.munlatlong) as munmedlat,

			        st_x(mun.munlatlong) as munmedlog,


				CASE 	WHEN d.indcumulativo='S' THEN sum(d.qtde)
					WHEN d.indcumulativo='N' THEN
						CASE WHEN d.sehstatus='A' THEN sum(d.qtde)
						ELSE 0 END
					WHEN d.indcumulativo='A' THEN
						CASE when d.dpeanoref='{$ano}' THEN sum(d.qtde)
						ELSE 0 END
					END as qtde,
				CASE 	WHEN d.indcumulativovalor='S' THEN sum(d.valor)
					WHEN d.indcumulativovalor='N' THEN
						CASE when d.sehstatus='A' THEN sum(d.valor)
						ELSE 0 END
					WHEN d.indcumulativovalor='A' then
						CASE when d.dpeanoref='{$ano}' THEN sum(d.valor)
						ELSE 0 end
					END as valor,
				d.indqtdevalor,
				d.unmid

	       FROM painel.v_detalheindicadorsh d
	       LEFT JOIN territoriosgeo.municipio mun ON mun.muncod = d.dshcodmunicipio
	       LEFT JOIN territorios.estado est ON est.estuf = d.dshuf
	       LEFT JOIN territorios.mesoregiao mes ON mes.mescod = mun.mescod
	       WHERE d.indid='{$indid}' AND d.muncod='{$dshcod}'
		   GROUP BY mun.muncod, est.estuf, mundescricao, mun.munlatlong, d.indcumulativo, d.sehstatus, d.dpeanoref, d.indcumulativovalor, d.indqtdevalor, d.unmid
	       ORDER BY est.estuf, mundescricao
		) as foo
		WHERE qtde!=0 OR valor!=0
		GROUP BY muncod, mundescricao, estuf, munmedlat, munmedlog, indqtdevalor, unmid
		ORDER BY estuf, mundescricao";

            return $this->getDefaultAdapter()->query($sql)->fetchAll();

        }

        public function getMapaIndicadores($indid,$dshcod){


            $sql1 = "SELECT dpe.dpeanoref FROM painel.seriehistorica seh
					  	 LEFT JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid
						 WHERE seh.indid='".$indid."' AND seh.sehstatus='A'";

            $ano = $this->getDefaultAdapter()->query($sql1)->fetchColumn(0);

            $sql = "SELECT muncod, mundescricao, estuf, munmedlat, munmedlog, CASE WHEN unmid IN('5','1','2') THEN CASE WHEN unmid='5' THEN 'R$ '||trim(to_char(SUM(qtde), '999g999g999g999d99')) ELSE trim(to_char(SUM(qtde), '999g999g999g999d99')) END ELSE trim(to_char(SUM(qtde), '999g999g999g999')) END as qtde, CASE WHEN indqtdevalor = TRUE THEN trim(to_char(sum(valor), '999g999g999g999d99')) END as valor FROM (
            SELECT	mun.muncod,
					mun.mundescricao,
					est.estuf,

					st_y(mun.munlatlong) as munmedlat,

			        st_x(mun.munlatlong) as munmedlog,


				CASE 	WHEN d.indcumulativo='S' THEN sum(d.qtde)
					WHEN d.indcumulativo='N' THEN
						CASE WHEN d.sehstatus='A' THEN sum(d.qtde)
						ELSE 0 END
					WHEN d.indcumulativo='A' THEN
						CASE when d.dpeanoref='{$ano}' THEN sum(d.qtde)
						ELSE 0 END
					END as qtde,
				CASE 	WHEN d.indcumulativovalor='S' THEN sum(d.valor)
					WHEN d.indcumulativovalor='N' THEN
						CASE when d.sehstatus='A' THEN sum(d.valor)
						ELSE 0 END
					WHEN d.indcumulativovalor='A' then
						CASE when d.dpeanoref='{$ano}' THEN sum(d.valor)
						ELSE 0 end
					END as valor,
				d.indqtdevalor,
				d.unmid

	       FROM painel.v_detalheindicadorsh d
	       LEFT JOIN territoriosgeo.municipio mun ON mun.muncod = d.dshcodmunicipio
	       LEFT JOIN territorios.estado est ON est.estuf = d.dshuf
	       LEFT JOIN territorios.mesoregiao mes ON mes.mescod = mun.mescod
	       WHERE d.indid='{$indid}' AND d.indid='{$dshcod}'
		   GROUP BY mun.muncod, est.estuf, mundescricao, mun.munlatlong, d.indcumulativo, d.sehstatus, d.dpeanoref, d.indcumulativovalor, d.indqtdevalor, d.unmid
	       ORDER BY est.estuf, mundescricao
		) as foo
		WHERE qtde!=0 OR valor!=0
		GROUP BY muncod, mundescricao, estuf, munmedlat, munmedlog, indqtdevalor, unmid
		ORDER BY estuf, mundescricao";

            return $this->getDefaultAdapter()->query($sql)->fetchAll();

        }

        public function getMapaEstados($indid,$dshcod){


            $sql1 = "SELECT dpe.dpeanoref FROM painel.seriehistorica seh
					  	 LEFT JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid
						 WHERE seh.indid='".$indid."' AND seh.sehstatus='A'";

            $ano = $this->getDefaultAdapter()->query($sql1)->fetchColumn(0);

            $sql = "SELECT muncod, mundescricao, estuf, munmedlat, munmedlog, CASE WHEN unmid IN('5','1','2') THEN CASE WHEN unmid='5' THEN 'R$ '||trim(to_char(SUM(qtde), '999g999g999g999d99')) ELSE trim(to_char(SUM(qtde), '999g999g999g999d99')) END ELSE trim(to_char(SUM(qtde), '999g999g999g999')) END as qtde, CASE WHEN indqtdevalor = TRUE THEN trim(to_char(sum(valor), '999g999g999g999d99')) END as valor FROM (
            SELECT	mun.muncod,
					mun.mundescricao,
					est.estuf,

					st_y(mun.munlatlong) as munmedlat,

			        st_x(mun.munlatlong) as munmedlog,


				CASE 	WHEN d.indcumulativo='S' THEN sum(d.qtde)
					WHEN d.indcumulativo='N' THEN
						CASE WHEN d.sehstatus='A' THEN sum(d.qtde)
						ELSE 0 END
					WHEN d.indcumulativo='A' THEN
						CASE when d.dpeanoref='{$ano}' THEN sum(d.qtde)
						ELSE 0 END
					END as qtde,
				CASE 	WHEN d.indcumulativovalor='S' THEN sum(d.valor)
					WHEN d.indcumulativovalor='N' THEN
						CASE when d.sehstatus='A' THEN sum(d.valor)
						ELSE 0 END
					WHEN d.indcumulativovalor='A' then
						CASE when d.dpeanoref='{$ano}' THEN sum(d.valor)
						ELSE 0 end
					END as valor,
				d.indqtdevalor,
				d.unmid

	       FROM painel.v_detalheindicadorsh d
	       LEFT JOIN territoriosgeo.municipio mun ON mun.muncod = d.dshcodmunicipio
	       LEFT JOIN territorios.estado est ON est.estuf = d.dshuf
	       LEFT JOIN territorios.mesoregiao mes ON mes.mescod = mun.mescod
	       WHERE d.indid='{$indid}' AND d.estuf='{$dshcod}'
		   GROUP BY mun.muncod, est.estuf, mundescricao, mun.munlatlong, d.indcumulativo, d.sehstatus, d.dpeanoref, d.indcumulativovalor, d.indqtdevalor, d.unmid
	       ORDER BY est.estuf, mundescricao
		) as foo
		WHERE qtde!=0 OR valor!=0
		GROUP BY muncod, mundescricao, estuf, munmedlat, munmedlog, indqtdevalor, unmid
		ORDER BY estuf, mundescricao";

            return $this->getDefaultAdapter()->query($sql)->fetchAll();

        }

    public function getDetalhamentoIndicador($indid){

        $sql = "select
				ind.indnome,
				exo.exodsc,
				sec.secdsc,
				aca.acadsc,
				aca.acaid,
				ind.indobjetivo,
				ind.indcumulativo,
				ind.regid,
				unm.unmdesc,
				ind.indformula,
				ind.indtermos,
				ind.indfontetermo,
				ind.indobservacao,
				ind.indvispadrao,
				per.perdsc,
				est.estdsc,
				col.coldsc,
				reg.regdescricao,
				ume.umedesc
			from
				painel.indicador ind
			left join
				painel.eixo exo ON exo.exoid = ind.exoid
			left join
				painel.secretaria sec ON sec.secid = ind.secid
			left join
				painel.acao aca ON aca.acaid = ind.acaid
			left join
				painel.unidademedicao unm ON unm.unmid = ind.unmid
			left join
				painel.periodicidade per ON per.perid = ind.perid
			left join
				painel.unidademeta ume on ind.umeid = ume.umeid
			left join
				painel.estilo est ON est.estid = ind.estid
			left join
				painel.coleta col ON col.colid = ind.colid
			left join
				painel.regionalizacao reg ON reg.regid = ind.regid
			where
				ind.indid = {$indid}
			limit 1";


        return $this->getDefaultAdapter()->query($sql)->fetchAll();


        //IMPLEMENTAR POSTERIORMENTE
        //$sqlRes = "	select distinct entnome, coalesce('(' || entnumdddcomercial || ') ' || entnumcomercial,'N/A') as telefone from painel.responsavel res inner join entidade.entidade ent ON res.entid = ent.entid where res.indid = $indid and entnome is not null";

    }

    public function getTabelaIndicadorEscolas($indid, $dshcod){
        $sql ="select
					dpeid,
					dpedsc,

					sum(qtde)::integer  as dshqtde,
					sum(valor) as dshvalor
				from (
						select
							dp.dpeid,
							d.indid,
							dp.dpedsc,
							dp.dpedatainicio,
							dp.dpedatafim,
							case when d.indcumulativo = 'N' then
				        			case when (
						                        select
						                        	d1.dpeid
						                        from
						                        	painel.detalheperiodicidade d1
												inner join
													painel.seriehistorica sh on sh.dpeid=d1.dpeid
												where
													d1.dpedatainicio>=dp.dpedatainicio
												and
													d1.dpedatafim<=dp.dpedatafim
												and
													sh.indid=d.indid
												and
													sehstatus <> 'I'

												order by
													d1.dpedatainicio desc
												limit
													1
				                				) = d.dpeid then sum(d.qtde)
				                	else 0 end
								else sum(d.qtde)
							end as qtde,
							case when d.indcumulativovalor = 'N' then
				        			case when (
				                        		select
				                        			d1.dpeid
				                        		from
				                        			painel.detalheperiodicidade d1
				                                inner join
				                                	painel.seriehistorica sh on sh.dpeid=d1.dpeid
				                                where
				                                	d1.dpedatainicio>=dp.dpedatainicio
				                                and
				                                	d1.dpedatafim<=dp.dpedatafim
				                                and
				                                	sh.indid=d.indid
												and
													sehstatus <> 'I'

				                                order by
													d1.dpedatainicio desc
												limit
													1
				                				) = d.dpeid then sum(d.valor)
				                			else 0 end
									else sum(d.valor)
							end as valor
						from
							painel.v_detalheindicadorsh d
						inner join
							painel.detalheperiodicidade dp on d.dpedatainicio>=dp.dpedatainicio and d.dpedatafim<=dp.dpedatafim
						-- periodo que vc quer exibir
						where
							dp.perid = 3
						-- indicador que vc quer exibir
						and
							d.indid = {$indid}
						and
							sehstatus <> 'I'
						 and d.dshcod = '{$dshcod}'
						--range de data compreendida no periodo

						group by
							d.indid,
							d.dpeid,
							dp.dpedsc,
							dp.dpeid,
							dp.dpedatainicio,
							dp.dpedatafim,
							d.indcumulativo,
							d.indcumulativovalor
							,d.dshcod
					) foo
				group by

					dpedatainicio,
					dpedatafim,
					dpeid,
					dpedsc,
					indid
				order by
					dpedatainicio";
        return $this->getDefaultAdapter()->query($sql)->fetchAll();
    }

    public function getTabelaIndicadorMunicipios($indid, $dshcod){
        $sql ="select
					dpeid,
					dpedsc,

					sum(qtde)::integer  as dshqtde,
					sum(valor) as dshvalor
				from (
						select
							dp.dpeid,
							d.indid,
							dp.dpedsc,
							dp.dpedatainicio,
							dp.dpedatafim,
							case when d.indcumulativo = 'N' then
				        			case when (
						                        select
						                        	d1.dpeid
						                        from
						                        	painel.detalheperiodicidade d1
												inner join
													painel.seriehistorica sh on sh.dpeid=d1.dpeid
												where
													d1.dpedatainicio>=dp.dpedatainicio
												and
													d1.dpedatafim<=dp.dpedatafim
												and
													sh.indid=d.indid
												and
													sehstatus <> 'I'

												order by
													d1.dpedatainicio desc
												limit
													1
				                				) = d.dpeid then sum(d.qtde)
				                	else 0 end
								else sum(d.qtde)
							end as qtde,
							case when d.indcumulativovalor = 'N' then
				        			case when (
				                        		select
				                        			d1.dpeid
				                        		from
				                        			painel.detalheperiodicidade d1
				                                inner join
				                                	painel.seriehistorica sh on sh.dpeid=d1.dpeid
				                                where
				                                	d1.dpedatainicio>=dp.dpedatainicio
				                                and
				                                	d1.dpedatafim<=dp.dpedatafim
				                                and
				                                	sh.indid=d.indid
												and
													sehstatus <> 'I'

				                                order by
													d1.dpedatainicio desc
												limit
													1
				                				) = d.dpeid then sum(d.valor)
				                			else 0 end
									else sum(d.valor)
							end as valor
						from
							painel.v_detalheindicadorsh d
						inner join
							painel.detalheperiodicidade dp on d.dpedatainicio>=dp.dpedatainicio and d.dpedatafim<=dp.dpedatafim
						-- periodo que vc quer exibir
						where
							dp.perid = 3
						-- indicador que vc quer exibir
						and
							d.indid = {$indid}
						and
							sehstatus <> 'I'
						 and d.muncod = '{$dshcod}'
						--range de data compreendida no periodo

						group by
							d.indid,
							d.dpeid,
							dp.dpedsc,
							dp.dpeid,
							dp.dpedatainicio,
							dp.dpedatafim,
							d.indcumulativo,
							d.indcumulativovalor
							,d.dshcod
					) foo
				group by

					dpedatainicio,
					dpedatafim,
					dpeid,
					dpedsc,
					indid
				order by
					dpedatainicio";
        return $this->getDefaultAdapter()->query($sql)->fetchAll();
    }

    public function getTabelaIndicadorEstados($indid, $dshcod){
        $sql ="select
					dpeid,
					dpedsc,

					sum(qtde)::integer  as dshqtde,
					sum(valor) as dshvalor
				from (
						select
							dp.dpeid,
							d.indid,
							dp.dpedsc,
							dp.dpedatainicio,
							dp.dpedatafim,
							case when d.indcumulativo = 'N' then
				        			case when (
						                        select
						                        	d1.dpeid
						                        from
						                        	painel.detalheperiodicidade d1
												inner join
													painel.seriehistorica sh on sh.dpeid=d1.dpeid
												where
													d1.dpedatainicio>=dp.dpedatainicio
												and
													d1.dpedatafim<=dp.dpedatafim
												and
													sh.indid=d.indid
												and
													sehstatus <> 'I'

												order by
													d1.dpedatainicio desc
												limit
													1
				                				) = d.dpeid then sum(d.qtde)
				                	else 0 end
								else sum(d.qtde)
							end as qtde,
							case when d.indcumulativovalor = 'N' then
				        			case when (
				                        		select
				                        			d1.dpeid
				                        		from
				                        			painel.detalheperiodicidade d1
				                                inner join
				                                	painel.seriehistorica sh on sh.dpeid=d1.dpeid
				                                where
				                                	d1.dpedatainicio>=dp.dpedatainicio
				                                and
				                                	d1.dpedatafim<=dp.dpedatafim
				                                and
				                                	sh.indid=d.indid
												and
													sehstatus <> 'I'

				                                order by
													d1.dpedatainicio desc
												limit
													1
				                				) = d.dpeid then sum(d.valor)
				                			else 0 end
									else sum(d.valor)
							end as valor
						from
							painel.v_detalheindicadorsh d
						inner join
							painel.detalheperiodicidade dp on d.dpedatainicio>=dp.dpedatainicio and d.dpedatafim<=dp.dpedatafim
						-- periodo que vc quer exibir
						where
							dp.perid = 3
						-- indicador que vc quer exibir
						and
							d.indid = {$indid}
						and
							sehstatus <> 'I'
						 and d.estuf = '{$dshcod}'
						--range de data compreendida no periodo

						group by
							d.indid,
							d.dpeid,
							dp.dpedsc,
							dp.dpeid,
							dp.dpedatainicio,
							dp.dpedatafim,
							d.indcumulativo,
							d.indcumulativovalor
							,d.dshcod
					) foo
				group by

					dpedatainicio,
					dpedatafim,
					dpeid,
					dpedsc,
					indid
				order by
					dpedatainicio";
        return $this->getDefaultAdapter()->query($sql)->fetchAll();
    }

    public function getTabelaIndicadorIndicadores($indid, $dshcod){
        $sql ="select
					dpeid,
					dpedsc,

					sum(qtde)::integer  as dshqtde,
					sum(valor) as dshvalor
				from (
						select
							dp.dpeid,
							d.indid,
							dp.dpedsc,
							dp.dpedatainicio,
							dp.dpedatafim,
							case when d.indcumulativo = 'N' then
				        			case when (
						                        select
						                        	d1.dpeid
						                        from
						                        	painel.detalheperiodicidade d1
												inner join
													painel.seriehistorica sh on sh.dpeid=d1.dpeid
												where
													d1.dpedatainicio>=dp.dpedatainicio
												and
													d1.dpedatafim<=dp.dpedatafim
												and
													sh.indid=d.indid
												and
													sehstatus <> 'I'

												order by
													d1.dpedatainicio desc
												limit
													1
				                				) = d.dpeid then sum(d.qtde)
				                	else 0 end
								else sum(d.qtde)
							end as qtde,
							case when d.indcumulativovalor = 'N' then
				        			case when (
				                        		select
				                        			d1.dpeid
				                        		from
				                        			painel.detalheperiodicidade d1
				                                inner join
				                                	painel.seriehistorica sh on sh.dpeid=d1.dpeid
				                                where
				                                	d1.dpedatainicio>=dp.dpedatainicio
				                                and
				                                	d1.dpedatafim<=dp.dpedatafim
				                                and
				                                	sh.indid=d.indid
												and
													sehstatus <> 'I'

				                                order by
													d1.dpedatainicio desc
												limit
													1
				                				) = d.dpeid then sum(d.valor)
				                			else 0 end
									else sum(d.valor)
							end as valor
						from
							painel.v_detalheindicadorsh d
						inner join
							painel.detalheperiodicidade dp on d.dpedatainicio>=dp.dpedatainicio and d.dpedatafim<=dp.dpedatafim
						-- periodo que vc quer exibir
						where
							dp.perid = 3
						-- indicador que vc quer exibir
						and
							d.indid = {$indid}
						and
							sehstatus <> 'I'

						--range de data compreendida no periodo

						group by
							d.indid,
							d.dpeid,
							dp.dpedsc,
							dp.dpeid,
							dp.dpedatainicio,
							dp.dpedatafim,
							d.indcumulativo,
							d.indcumulativovalor
							,d.dshcod
					) foo
				group by

					dpedatainicio,
					dpedatafim,
					dpeid,
					dpedsc,
					indid
				order by
					dpedatainicio";
        return $this->getDefaultAdapter()->query($sql)->fetchAll();
    }

    public function getUnidademeta($indid){
        $sql = "select  umedesc from painel.indicador ind
					join painel.unidademeta unime on unime.umeid = ind.umeid
					where ind.indid = {$indid}";
        return $this->getDefaultAdapter()->query($sql)->fetchAll();

    }
}
