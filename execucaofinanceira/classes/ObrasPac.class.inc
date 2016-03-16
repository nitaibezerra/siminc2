<?php
class ObrasPac {
	
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