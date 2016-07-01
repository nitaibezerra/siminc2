<?php
	
class PropostaSubacao extends Modelo
{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.propostasubacao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "ppsid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'ppsid' => null, 
									  	'ppsdsc' => null, 
									  	'ppsordem' => null,
    									'ppaid' => null,
    									'frmid' => null,
									    'prgid' => null,
									    'indid' => null,
									    'ppsestrategiaimplementacao' => null,
									    'undid' => null,
									    'ppspeso' => null,
    									'foaid' => null,
    									'ppsobra' => null,
    									'ppsptres' => null,
    									'ppsnaturezadespesa' => null,
    									'ppscronograma' => null,
    									'ppsmonitora' => null,
    									'ptsid' => null,
    									'ppsobjetivo' => null,
    									'ppstexto' => null,
    									'ppscobertura' => null
									  );
									  
									  
	public function recuperaDadosFormGuiaSubacao($indid)
	{
		
		$sql = "SELECT  d.dimcod as codigodimensao,			
			            a.arecod as codigoarea,			
			            i.indcod as codigoindicador,			
			            d.dimdsc as descricaodimensao,			
			            a.aredsc as descricaoarea,			
			            i.inddsc as descricaoindicador,			
			            c.crtid  as idcriterio,			
			            c.crtdsc as descricaocriterio,			
			            c.crtpontuacao as pontuacao			
			            --cps.crtid as idcriteriosubacao			
			FROM par.dimensao d			
			INNER JOIN par.area a ON a.dimid = d.dimid			
			INNER JOIN par.indicador i ON i.areid = a.areid			
			INNER JOIN par.criterio c ON c.indid = i.indid			
			--LEFT JOIN par.criteriopropostasubacao cps ON cps.crtid = c.crtid			
			WHERE i.indid = {$indid}		
			ORDER BY c.crtpontuacao";
		
		return $this->carregar($sql);
	}
	
	public function recuperarSubacaoGuia($indid)
	{
		$sql = "SELECT DISTINCT
				  sub.ppsid,
				  sub.ppsdsc,
				  sub.ppsordem,
				  sub.frmid,
				  sub.prgid,
				  sub.indid,
				  sub.ppsestrategiaimplementacao,
				  sub.undid,
				  sub.ppspeso,
				  sub.foaid,
				  sub.ppsobra,
				  sub.ppsptres,
				  sub.ppsnaturezadespesa,
				  sub.ppscronograma,
				  sub.ppsmonitora,
				  sub.ptsid				    
				FROM {$this->stNomeTabela} sub
				WHERE sub.indid = {$indid}
				ORDER BY sub.ppsordem";
		
		return $this->carregar($sql);
	}
	
	public function recuperarSubacoesPorCriterio($crtid)
	{
		$sql = "SELECT * FROM par.propostasubacao pps
				INNER JOIN par.criteriopropostasubacao cps ON pps.ppsid = cps.ppsid
				WHERE crtid = {$crtid} order by ppsordem";

		return $this->carregar($sql);
	}
	
	public function carregaPropostaSubacaoPorSbaid($sbaid)
	{
		$sql = "SELECT DISTINCT 
                  itr.itrdsc,
                  dim.dimdsc,
                  are.aredsc,
                  ind.inddsc,
                  -- pps.ppsdsc,
                  'Construção de unidade de Educação Infantil' AS ppsdsc,
                  'Ações do PAC 2' as foadescricao,
                  -- foa.foadescricao,
                  'Quando a rede existente de ensino fundamental, na área rural, em assentamentos, comunidades indígenas e/ou quilombolas, possui prédios próprios ou cedidos com infraestrutura física adequada. As instalações apresentam condições adequadas de segurança, salubridade e conforto ambiental. Além disso, as instalações garantem acessibilidade, os banheiros são adequados e compatíveis com a faixa etária dos usuários. Os prédios apresentam espaços adequados e atendem aos padrões mínimos. Todas as escolas da rede possuem salas de recursos multifuncionais implantadas e dispõem de espaço físico adequado para seu funcionamento. A infraestrutura, de modo geral, apresenta salas de aula em quantidade suficiente para a demanda, existem laboratórios de Informática e Ciências, sala de recursos audiovisuais, biblioteca, cozinha, refeitório, quadra de esporte e espaços de lazer e convivência.'
                  as estrategiaimplementacao,
                  -- sba.estrategiaimplementacao,
                  'Unidade Escolar' as unddsc,
                  -- und.unddsc,
                  -- frm.frmdsc,
                  'MEC - Transferência voluntária' as frmdsc,
                  CASE WHEN pps.ppscronograma = 1 THEN 'Global'
                       WHEN pps.ppscronograma = 2 THEN 'Por escola'
                       ELSE 'CPF'
                  END AS crono
            FROM par.propostasubacao pps
            INNER JOIN par.indicador ind ON ind.indid = pps.indid
            INNER JOIN par.criterio crt ON crt.indid = ind.indid
            INNER JOIN par.propostaacao ppa ON ppa.crtid = crt.crtid
            INNER JOIN par.area are ON are.areid = ind.areid
            INNER JOIN par.dimensao dim ON dim.dimid = are.dimid
            INNER JOIN par.instrumento itr ON itr.itrid = dim.itrid
            INNER JOIN par.subacaotemporaria sba ON pps.ppsid = sba.ppsid
            INNER JOIN par.formaatendimento foa ON foa.foaid = pps.foaid
            INNER JOIN par.unidademedida und ON und.undid = pps.undid
            INNER JOIN par.formaexecucao frm ON frm.frmid = pps.frmid
            WHERE sba.subid = {$sbaid} ";

		return $this->pegaLinha($sql);
	}
	
	public function recuperarMunicipiosPorSubacao($ppsid)
	{
		$sql = "SELECT mun.estuf, mun.muncod , mundescricao FROM par.propostasubacaomunicipio pps
				INNER JOIN territorios.municipio mun ON mun.muncod = pps.muncod
				WHERE ppsid = {$ppsid} order by mun.estuf, mundescricao";

		return $this->carregar($sql);
	}
	
	public function recuperarEstadosPorSubacao($ppsid)
	{
		$sql = "SELECT est.estuf, est.estdescricao FROM par.propostasubacaoestados ppe
				INNER JOIN territorios.estado est ON est.estuf = ppe.estuf
				WHERE ppsid = {$ppsid} order by estdescricao";

		return $this->carregar($sql);
	}
									  
}									  