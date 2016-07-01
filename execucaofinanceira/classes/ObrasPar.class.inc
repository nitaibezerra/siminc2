<?php
class ObrasPar {
	
	private $obProcesso;
									  
	function __construct( $obProcesso ) {
		$this->obProcesso = $obProcesso;
	}
	
	public function sqlListaAEmpenhar(){
		$sql = "SELECT
				    e.empnumero as numeroempenho,
				    s.sbadsc AS descricao, 
				    sd.sbdano || '&nbsp;' as ano,
				    pc.prcvalortotal AS valortotal,
				    sd.sbdplanointerno as planointerno,
				    sd.sbdptres as ptres,
					COALESCE(SUM(ec.emcpercentualemp), 0) AS percentualempenhado, 
				    COALESCE(SUM(ec.emcvaloremp), 0) AS valorempenhado,
				    CASE WHEN p.tprid = 2 
				    	THEN (99 - coalesce(ec.emcpercentualemp,0))
				    ELSE (100 - coalesce(ec.emcpercentualemp,0)) END as porcentagem,
	                CASE WHEN COALESCE(SUM(ec.emcpercentualemp), 0) <> 0
	                    THEN (pc.prcvalortotal -  COALESCE(SUM(ec.emcvaloremp), 0))
	                    ELSE case when p.tprid = 2 then
	                    		cast((cast( (pc.prcvalortotal * (99 - coalesce(ec.emcpercentualemp,0)) )as numeric) / 100) as numeric)
	                    	else 
	                        	cast((cast( (pc.prcvalortotal * (100 - coalesce(ec.emcpercentualemp,0)) )as numeric) / 100) as numeric)
	                        end
	                END AS valoraempenhar,
	                CASE WHEN COALESCE(SUM(ec.emcpercentualemp), 0) <> 0
						THEN cast((pc.prcvalortotal -  COALESCE(SUM(ec.emcvaloremp), 0)) as numeric)
						ELSE CASE WHEN p.tprid = 2 
						    	THEN cast(cast((pc.prcvalortotal * (99 - coalesce(ec.emcpercentualemp,0))) as numeric) / 100 as numeric)
						    ELSE cast(cast((pc.prcvalortotal * (100 - coalesce(ec.emcpercentualemp,0))) as numeric) / 100 as numeric) END
					END AS valoraempenhartotal,
	                sd.sbdid as codigo
				FROM execucaofinanceira.processos p
					INNER JOIN execucaofinanceira.processocomposicao pc ON p.proid = pc.proid
					LEFT JOIN execucaofinanceira.empenho e ON e.proid = p.proid
					LEFT JOIN execucaofinanceira.empenhocomposicao ec ON ec.empid = e.empid AND ec.sbdid = pc.sbdid
					INNER JOIN par.subacaodetalhe sd ON sd.sbdid = pc.sbdid
					INNER JOIN par.subacao s ON s.sbaid = sd.sbaid
				WHERE 
					p.proid = {$this->obProcesso->proid}
				GROUP BY 
					sd.sbdid,
				    e.empnumero,
				    s.sbadsc,
				    p.tprid,
				    sd.sbdano, 
				    pc.prcvalortotal, 
				    sd.sbdplanointerno, 
				    sd.sbdptres,
					ec.emcpercentualemp";
		
		return $sql;
	}
	
	public function sqlRecuperaDadosPreObras( $preid ){
		$sql = "SELECT 
						p.predescricao || ' - ' || mun.mundescricao || ' - ' || mun.estuf as nome_obra,
						ent.entid as unidade_implantadora,
						p.precep,
						p.prelogradouro,
						p.precomplemento,
						p.prebairro,
						p.muncod,
						p.estuf,
						p.prenumero,
						p.prelatitude,
						p.prelongitude,
						p.preesfera,
						p.ptoid,
                        CASE WHEN pop.sisid = 23 THEN 39 ELSE 40 END AS programa,
                        CASE WHEN i.indcod in (3,4,7,8) THEN 1
                        	ELSE CASE WHEN i.indcod in (5,6,10,9) THEN 2 END
                        END AS modalidadedeensino,
						CASE   WHEN p.ptoid in (1,2,3,4,5,11,12,6,7,13,25,26,31,14,15,16,8,9,23,24,27,28,32,21,10,35,36)   THEN 1 
                        	ELSE CASE WHEN p.ptoid in (29,17,33) THEN 4
                        			ELSE CASE WHEN p.ptoid in (30,18,34) THEN 3 END
                            END
                        END AS tipodeobra,
                        CASE WHEN sisid = 23 -- PAR 
                        	THEN 11
                            ELSE   CASE WHEN sisid = 57 -- EMENDAS 
                                   		THEN 4 ELSE 11 -- BRASIL-PRÓ
                                   END
                        END AS fonte,
                        CASE WHEN (substring(UPPER(p.predescricao), 'INDÍGENA') IS NOT NULL OR substring(UPPER(p.predescricao), 'INDÍGENA') != '' ) OR
                        		(substring(UPPER(p.predescricao), 'INDIGENA') IS NOT NULL OR substring(UPPER(p.predescricao), 'INDIGENA') != '' )
                             THEN 4 -- INDÍGENA
                        	ELSE 
                        	CASE WHEN (substring(UPPER(p.predescricao), 'RURAL') IS NOT NULL OR substring(UPPER(p.predescricao), 'RURAL') != '' )
                            	THEN 1
                                ELSE 2 END
                        END AS classificacaoobra,
                        prevalorobra as valorobra
					FROM 
                    	par.dimensao d
                        INNER JOIN par.area                 a    ON a.dimid = d.dimid 
                        INNER JOIN par.indicador          	i    ON i.areid = a.areid
                        INNER JOIN par.criterio             c    ON c.indid = i.indid
                        INNER JOIN par.pontuacao        	po   ON c.crtid = po.crtid AND ptostatus = 'A'
                        INNER JOIN par.instrumentounidade	iu   ON iu.inuid = po.inuid 
                        INNER JOIN par.acao					ac   ON ac.ptoid = po.ptoid AND ac.acistatus = 'A'
                        INNER JOIN par.subacao				s    ON s.aciid = ac.aciid AND s.sbastatus = 'A' 
                        INNER JOIN par.subacaoobra    		so   ON so.sbaid = s.sbaid
                        INNER JOIN obras.preobra         	p    ON so.preid = p.preid
                        INNER JOIN par.processoobraspar 	pop  ON pop.inuid = iu.inuid and pop.prostatus = 'A'
                        INNER JOIN territorios.municipio 	mun  on p.muncod = mun.muncod
                        INNER JOIN entidade.endereco 		ende ON ende.muncod = p.muncod
                        INNER JOIN entidade.entidade 		ent	 ON ent.entid = ende.entid AND ent.entstatus = 'A'
                        INNER JOIN entidade.funcaoentidade	fen  ON ent.entid = fen.entid AND fen.funid IN (1)									
					WHERE
						p.preid = ".$preid;	
		
		return $sql;
	}

	public function carregaListaDadosProcesso(){
		global $db;
		
		$sql = "SELECT distinct
					pre.predescricao,
				    pre.preano||' ',
				    pc.prcvalortotal
				FROM
					execucaofinanceira.processos p
				    inner join execucaofinanceira.processocomposicao pc on pc.proid = p.proid
				    inner join obras.preobra pre on pre.preid = pc.sbdid
				WHERE
					p.pronumeroprocesso = '{$this->obProcesso->pronumeroprocesso}'
				order by pre.predescricao";
		
		return $sql;
	}
	
	public function sqlListaEmpenhosRealizados(){
		global $db;
		
		$arrPerfil = carregarPerfil();
		
		if( in_array( EXEC_PERFIL_TECNICO_PAGAMENTO, $arrPerfil ) || in_array( EXEC_PERFIL_ADMINISTRADOR, $arrPerfil ) || $db->testa_superuser() ){
			$btnAcoes = '<img src="../imagens/exclusao.gif" border="0" title="Solicitar Empenho Parcial">&nbsp;
						 <img src="../imagens/valida3.gif" border="0" title="Cancelar Empenho">&nbsp;
						 <img src="../imagens/money.gif" border="0" title="Solicitar Pagamento">';
		} else {
			$btnAcoes = '<img src="../imagens/money_01.gif" border="0" title="Solicitar Pagamento">';
		}
		
		$sql = "SELECT DISTINCT
					'<img style=\"cursor:pointer\" id=\"img_dimensao_'|| e.empid ||'\" src=\"/imagens/mais.gif\" onclick=\"carregarDadosEmpenho(this.id,'|| e.empid ||');\" border=\"0\">' as acoes,  
		            e.empnumero AS numerodoprocesso,
		            e.empano||'&nbsp;' AS anodoempenho,
		            e.empvalorempenhado as valorempenhado,
		            cast(((sum(DISTINCT ec.emcvaloremp) * 100) / sum( DISTINCT pc.prcvalortotal)) as numeric(20)) AS porcentagemempenhadaemrelprojeto,
		            '0,00' as valoPago,
		            '<center>$btnAcoes</center>' as btn,
				    '</td></tr>
				            	<tr style=\"display:none\" id=\"listaDadosEmpenho_' || e.empid ||'\" >
				            		<td id=\"trV_' || e.empid ||'\" colspan=8 ></td>
				            </tr>' as btnPagamento
				FROM execucaofinanceira.processos p
					INNER JOIN execucaofinanceira.empenho e ON p.proid = e.proid 
					INNER JOIN execucaofinanceira.empenhocomposicao ec ON ec.empid = e.empid
					INNER JOIN execucaofinanceira.processocomposicao pc ON pc.proid = p.proid
				WHERE p.prostatus = 'A'
					AND e.empstatus = 'A'
					AND p.proid = {$this->obProcesso->proid}
				GROUP BY p.proid,
		             e.empid,  
		             e.empnumero,
		             p.proano,
		             e.empano,
		             e.empvalorempenhado,
		             ec.emcvaloremp";
		return $sql;
	}
}
?>