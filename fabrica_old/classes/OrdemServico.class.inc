<?php

/**
 * Ordem de Servico do <strong>Modulo F�brica</strong>
 * @author rayner
 *
 */
class OrdemServico extends ModeloFabrica
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "fabrica.ordemservico";

    /**
     * Chave primaria
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "odsid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'odsid'                  => null,
        'scsid'                  => null,
        'odsdetalhamento'        => null,
        'odsdtprevinicio'        => null,
        'odsdtprevtermino'       => null,
        'docid'                  => null,
        'odsidpai'               => null,
        'odsqtdpfestimada'       => null,
        'odsqtdpfdetalhada'      => null,
        'odscontratada'          => null,
        'odsidpf'                => null,
        'docidpf'                => null,
        'tosid'                  => null,
        'tpeid'                  => null,
        'odsqtdpffinal'          => null,
        'odsenderecosvn'         => null,
        'odssubtotalpf'          => null,
        'odssubtotalpfdetalhada' => null,
        'ctrid'                  => null,
        'memoid'                 => null,
        'glosaid'                => null,
        'tpdpsid'                => null,
        'dataabertura'           => null
    );
    private $porcentagemDisciplina;
    private $valorUnitarioDePf;
    private $valorAReceberDaOs;
    private $statusOrdemServico;
    protected $stCampos = "os.odsid, os.scsid, os.odsdetalhamento, os.odsdtprevinicio, os.odsdtprevtermino, os.docid, os.odsidpai,  
		os.odsqtdpfestimada, os.odsqtdpfdetalhada, os.odscontratada, os.odsidpf, os.docidpf, os.tosid, os.tpeid, os.odsqtdpffinal, os.odsenderecosvn, 
		os.odssubtotalpf, os.odssubtotalpfdetalhada, os.ctrid , os.memoid, os.glosaid";
    protected $stTodosOsCampos = "*";

    public function setId( $id )
    {
        $this->arAtributos['odsid'] = $id;
    }

    public function setIdSolicitacaoServico( $idSolicitacaoServico )
    {
        $this->arAtributos['scsid'] = $idSolicitacaoServico;
    }

    public function setDetalhamento( $detalhamento )
    {
        $this->arAtributos['odsdetalhamento'] = $detalhamento;
    }

    public function setDataPrevisaoDeInicio( DateTime $dataPrevisaoInicio )
    {
        $this->arAtributos['odsdtprevinicio'] = $dataPrevisaoInicio;
    }

    public function setDataPrevisaoDeTermino( DateTime $dataPrevisaoTermino )
    {
        $this->arAtributos['odsdtprevtermino'] = $dataPrevisaoTermino;
    }

    public function setIdDocumento( $idDocumento )
    {
        $this->arAtributos['docid'] = $idDocumento;
    }

    public function setIdPai( $idPai )
    {
        $this->arAtributos['odsidpai'] = $idPai;
    }

    public function setQtdePfDetalhada( $qtdePfDetalhada )
    {
        $this->arAtributos['odsqtdpfdetalhada'] = $qtdePfDetalhada;
    }

    public function setQtdePfEstimada( $qtdePfEstimada )
    {
        $this->arAtributos['odsqtdpfestimada'] = $qtdePfEstimada;
    }

    public function setContratada( $contratada )
    {
        $this->arAtributos['odscontratada'] = $contratada;
    }

    public function setIdPf( $idPf )
    {
        $this->arAtributos['odsidpf'] = $idPf;
    }

    public function setIdDocumentoPf( $IdDocumentoPf )
    {
        $this->arAtributos['docidpf'] = $IdDocumentoPf;
    }

    public function setIdTipoOrdemServico( $IdTipoOrdemServico )
    {
        $this->arAtributos['tosid'] = $IdTipoOrdemServico;
    }

    public function setIdTipoExecucao( $IdTipoExecucao )
    {
        $this->arAtributos['tpeid'] = $IdTipoExecucao;
    }

    public function setQtdePfFinal( $qtdePfFinal )
    {
        $this->arAtributos['odsqtdpffinal'] = $qtdePfFinal;
    }

    public function setEnderecoSvn( $enderecoSvn )
    {
        $this->arAtributos['odsenderecosvn'] = $enderecoSvn;
    }

    public function setSubTotalPf( $subTotalPf )
    {
        $this->arAtributos['odssubtotalpf'] = $subTotalPf;
    }

    public function setSubTotalPfDetalhada( $subTotalPfDetalhada )
    {
        $this->arAtributos['odssubtotalpfdetalhada'] = $subTotalPfDetalhada;
    }

    public function setIdContrato( $IdContrato )
    {
        $this->arAtributos['ctrid'] = $IdContrato;
    }

    public function setIdMemorando( $idMemorando )
    {
        $this->arAtributos['memoid'] = $idMemorando;
    }

    public function setIdGlosa( $idGlosa )
    {
        $this->arAtributos['glosaid'] = $idGlosa;
    }

    public function setPorcentagemDisciplina( $porcentagemDisciplina )
    {
        $this->porcentagemDisciplina = $porcentagemDisciplina;
    }

    public function setValorUnitarioDePf( $valorUnitarioDePf )
    {
        $this->valorUnitarioDePf = $valorUnitarioDePf;
    }

    public function setValorAReceberDaOs( $valorAReceberDaOs )
    {
        $this->valorAReceberDaOs = $valorAReceberDaOs;
    }

    public function setStatusOrdemServico( $statusOrdemServico )
    {
        $this->statusOrdemServico = $statusOrdemServico;
    }
    
    public function setIdTipoDespesa( $idTipoDespesa )
    {
        $this->arAtributos['tpdpsid'] = $idTipoDespesa;
    }
    
    public function setDataAbertura( $dataabertura )
    {
    	$this->arAtributos['dataabertura'] = $dataabertura;
    }
    
    public function getIdTipoDespesa(  )
    {
        return $this->arAtributos['tpdpsid'] ;
    }

    public function getId()
    {
        return $this->arAtributos['odsid'];
    }

    public function getIdSolicitacaoServico()
    {
        return $this->arAtributos['scsid'];
    }

    public function getDetalhamento()
    {
        return $this->arAtributos['odsdetalhamento'];
    }

    public function getDataPrevisaoDeInicio()
    {
        return $this->arAtributos['odsdtprevinicio'];
    }

    public function getDataPrevisaoDeTermino()
    {
        return $this->arAtributos['odsdtprevtermino'];
    }

    public function getIdDocumento()
    {
        return $this->arAtributos['docid'];
    }

    public function getIdPai()
    {
        return $this->arAtributos['odsidpai'];
    }

    public function getQtdePfDetalhada()
    {
        return $this->arAtributos['odsqtdpfdetalhada'] > 0 ? $this->arAtributos['odsqtdpfdetalhada'] : $this->arAtributos['odsqtdpfestimada'];
    }

    public function getQtdePfDetalhadaReal()
    {
        return $this->arAtributos['odsqtdpfdetalhada'];
    }

    public function getQtdePfEstimada()
    {
        return $this->arAtributos['odsqtdpfestimada'];
    }

    public function getContratada()
    {
        return $this->arAtributos['odscontratada'];
    }

    public function getIdPf()
    {
        return $this->arAtributos['odsidpf'];
    }

    public function getIdDocumentoPf()
    {
        return $this->arAtributos['docidpf'];
    }

    public function getIdTipoOrdemServico()
    {
        return $this->arAtributos['tosid'];
    }

    public function getIdTipoExecucao()
    {
        return $this->arAtributos['tpeid'];
    }

    public function getQtdePfFinal()
    {
        return $this->arAtributos['odsqtdpffinal'];
    }

    public function getEnderecoSvn()
    {
        return $this->arAtributos['odsenderecosvn'];
    }

    public function getSubTotalPf()
    {
        return $this->arAtributos['odssubtotalpf'];
    }

    public function getSubTotalPfDetalhada()
    {
        return $this->arAtributos['odssubtotalpfdetalhada'];
    }

    public function getIdContrato()
    {
        return $this->arAtributos['ctrid'];
    }

    public function getIdMemorando()
    {
        return $this->arAtributos['memoid'];
    }

    public function getIdGlosa()
    {
        return $this->arAtributos['glosaid'];
    }

    public function getPorcentagemDisciplina()
    {

        if ( $this->getStatusOrdemServico() == 300 || $this->getStatusOrdemServico() == 302 )
        {
            return 12;
        }
        return $this->porcentagemDisciplina;
    }

    public function getValorUnitarioDePf()
    {
        return $this->valorUnitarioDePf;
    }

    public function getValorAReceberDaOs()
    {
        return round( ($this->getMenorValorPF() * ($this->getPorcentagemDisciplina() / 100)), 2) * $this->getValorUnitarioDePf();
    }

    public function getStatusOrdemServico()
    {
        return $this->statusOrdemServico;
    }

    public function getValorAReceberGlosado()
    {
        if ( $this->possuiGlosa() )
        {
            $glosa                = new Glosa();
            $glosa                = $glosa->recupereGlosaPeloId( $this->getIdGlosa() );
            $valorParcial 		  = ($this->getMenorValorPF() * ($this->getPorcentagemDisciplina() / 100) - $glosa->getValorEmPf());
            
            if($valorParcial) {
            	$valorParcial = number_format($valorParcial,2);
            }
            else{
            	$valorParcial = 0;
            }
            
            $valorAReceberGlosado = $valorParcial * $this->getValorUnitarioDePf();
            return $valorAReceberGlosado;
        }
        return null;
    }

    public function getMenorValorPF()
    {
        $scsid      = $this->getIdSolicitacaoServico();
        $menorValor = 0;
        $sql        = " SELECT qtd_pf_estimada, qtd_pf_detalhada FROM fabrica.vw_painel_financeiro_empresas WHERE tosid_os = 1 AND id_ss =  $scsid ";
        $linha      = parent::pegaLinha( $sql );

        if ( $this->arAtributos['tosid'] == 1 )
        {
            if ( $this->getStatusOrdemServico() == 300 )
            {
                return $this->getSubTotalPf();
            } else if ( $this->getQtdePfDetalhadaReal() > 0 )
            {
                //$sql_2   = " SELECT qtd_pf_estimada, qtd_pf_detalhada FROM fabrica.vw_painel_financeiro_empresas WHERE tosid_os = 3 AND id_ss =  $scsid ";
                $sql_2 = "SELECT min(odsqtdpfdetalhada) as qtd_pf_detalhada FROM fabrica.ordemservico WHERE tosid = 3 AND scsid = $scsid ";
                $linha_2 = parent::pegaLinha( $sql_2 );

                if ( $linha_2['qtd_pf_detalhada'] < $this->getQtdePfDetalhadaReal() AND $linha_2['qtd_pf_detalhada'] > 0 )
                {
                    $menorValor = $linha_2['qtd_pf_detalhada'];
                } else
                {
                    $menorValor = $this->getQtdePfDetalhadaReal();
                }
            } elseif ( $this->getQtdePfEstimada() > 0 )
            {
                //$sql_2   = " SELECT qtd_pf_estimada, qtd_pf_detalhada FROM fabrica.vw_painel_financeiro_empresas WHERE tosid_os = 2 AND id_ss =  $scsid ";
                $sql_2 = "SELECT min(odsqtdpfestimada) as qtd_pf_estimada FROM fabrica.ordemservico WHERE tosid = 2 AND scsid = $scsid ";
                $linha_2 = parent::pegaLinha( $sql_2 );
                
                if ( $linha_2['qtd_pf_estimada'] < $this->getQtdePfDetalhadaReal() AND $linha_2['qtd_pf_estimada'] > 0 )
                {
                    $menorValor = $linha_2['qtd_pf_estimada'];
                } else
                {
                    $menorValor = $this->getQtdePfDetalhadaReal();
                }
            }
        } elseif ( $this->arAtributos['tosid'] == 2 )
        {
            if ( $linha['qtd_pf_estimada'] < $this->getQtdePfEstimada() )
            {
                $menorValor = $linha['qtd_pf_estimada'];
            } else
            {
                $menorValor = $this->getQtdePfEstimada();
            }
        } elseif ( $this->arAtributos['tosid'] == 3 )
        {
            if ( $linha['qtd_pf_detalhada'] < $this->getQtdePfDetalhadaReal() )
            {
                $menorValor = $linha['qtd_pf_detalhada'];
            } else
            {
                $menorValor = $this->getQtdePfDetalhadaReal();
            }
        }
        return $menorValor;
    }
    
    public function getMenorValorPFEmpresaItem2( $odsid )
    {
        $sql = "SELECT os.scsid, os.odsid,  os.odsqtdpfestimada, os.odsidpai, os.odsqtdpfdetalhada
                    , os.docidpf, os.tosid, os.odssubtotalpf, os.odssubtotalpfdetalhada

                    , ospai.odsid as odsidpai 
                    , ospai.scsid as scsidpai 
                    , ospai.odsqtdpfestimada as odsqtdpfestimadapai
                    , ospai.odsqtdpfdetalhada as odsqtdpfdetalhadapai
                    , ospai.docid as docidpai
                    , ospai.tosid as tosidpai
                    , ospai.odssubtotalpf as odssubtotalpfpai
                    , ospai.odssubtotalpfdetalhada as odssubtotalpfdetalhadapai

                    , CASE
                        WHEN os.tosid = 2 THEN LEAST( os.odssubtotalpf, ospai.odssubtotalpf )
                        WHEN os.tosid = 3 THEN LEAST( os.odssubtotalpf, ospai.odssubtotalpfdetalhada )
                    END as menor_pf_os
                FROM fabrica.ordemservico os

                INNER JOIN fabrica.ordemservico ospai
                    ON os.odsidpai = ospai.odsid
                WHERE os.tosid IN (2,3)
                AND os.odsid = {$odsid}";
                
         $linha = $this->pegaLinha($sql);
         
         return $linha['menor_pf_os'];
    }
    
    public function getMenorValorPFEmpresaItem1( $odsid )
    {
        $sql = "SELECT os.scsid, os.odsid,  os.odsqtdpfestimada, os.odsidpai, os.odsqtdpfdetalhada
                        , os.docidpf, os.tosid, os.odssubtotalpf, os.odssubtotalpfdetalhada

                        , osfilha.odsid as odsidfliha 
                        , osfilha.scsid as scsidfliha 
                        , osfilha.odsqtdpfestimada as odsqtdpfestimadafliha
                        , osfilha.odsqtdpfdetalhada as odsqtdpfdetalhadafliha
                        , osfilha.docid as docidfliha
                        , osfilha.tosid as tosidfliha
                        , osfilha.odssubtotalpf as odssubtotalpffliha
                        , osfilha.odssubtotalpfdetalhada as odssubtotalpfdetalhadafliha

                        , CASE
                            WHEN osfilha.odsid IS NOT NULL AND osfilha.tosid = 2 THEN LEAST( os.odsqtdpfestimada, osfilha.odsqtdpfestimada )
                            WHEN osfilha.odsid IS NOT NULL AND osfilha.tosid = 3 THEN LEAST( os.odsqtdpfdetalhada, osfilha.odsqtdpfdetalhada )
                            WHEN osfilha.odsid IS NULL AND os.odsqtdpfdetalhada IS NOT NULL THEN os.odsqtdpfdetalhada
                            WHEN osfilha.odsid IS NULL AND os.odsqtdpfdetalhada IS NULL AND os.odsqtdpfestimada IS NOT NULL THEN os.odsqtdpfestimada
                        END as menor_pf_os
                FROM fabrica.ordemservico os

                LEFT JOIN fabrica.ordemservico osfilha
                    ON os.odsid = osfilha.odsidpai
                WHERE os.tosid  = 1
                AND os.odsid    = {$odsid}
                ORDER BY osfilha.tosid DESC";
                
         $linha = $this->pegaLinha($sql);
         
         return $linha['menor_pf_os'];
    }

    public function getDataAbertura()
    {
    	return $this->arAtributos['dataabertura'];
    }
    
    private function montaObjeto( $linha )
    {
        $os = new OrdemServico();
        $os->setId( $linha['odsid'] );
        $os->setIdSolicitacaoServico( $linha['scsid'] );
        $os->setDetalhamento( $linha['odsdetalhamento'] );
        $os->setDataPrevisaoDeInicio( new DateTime($linha['odsdtprevinicio']) );
        $os->setDataPrevisaoDeTermino( new DateTime($linha['odsdtprevtermino']) );
        $os->setIdDocumento( $linha['docid'] );
        $os->setIdPai( $linha['odsidpai'] );
        $os->setQtdePfDetalhada( $linha['odsqtdpfdetalhada'] );
        $os->setQtdePfEstimada( $linha['odsqtdpfestimada'] );
        $os->setContratada( $linha['odscontratada'] );
        $os->setIdPf( $linha['odsidpf'] );
        $os->setIdDocumentoPf( $linha['docidpf'] );
        $os->setIdTipoOrdemServico( $linha['tosid'] );
        $os->setIdTipoExecucao( $linha['tpeid'] );
        $os->setQtdePfFinal( $linha['odsqtdpffinal'] );
        $os->setEnderecoSvn( $linha['odsenderecosvn'] );
        $os->setSubTotalPf( $linha['odssubtotalpf'] );
        $os->setSubTotalPfDetalhada( $linha['odssubtotalpfdetalhada'] );
        $os->setIdContrato( $linha['ctrid'] );
        $os->setPorcentagemDisciplina( $linha['porcentagem_disciplinas'] );
        $os->setValorUnitarioDePf( $linha['valor_pf'] );
        $os->setValorAReceberDaOs( $linha['valor_a_receber'] );
        $os->setIdTipoDespesa($linha['tpdpsid']);
        $os->setDataAbertura($linha['dataabertura']);
        
        if ($linha['esdid'] == ''){
        	if ($linha['tosid'] == 1 && (!empty($linha['docid'])) ){
	        	$linha['esdid'] = $this->recuperaEstadoOS($linha['docid']);
        	}
        	if ($linha['tosid'] != 1 && $linha['docidpf'] != ''){
        		$linha['esdid'] = $this->recuperaEstadoOS($linha['docidpf']);
        	}
        }
        
        $os->setStatusOrdemServico( $linha['esdid'] );
        $os->setIdGlosa( $linha['glosaid'] );
        $os->setIdMemorando( $linha['memoid'] );
        return $os;
    }
    
    public function isOsEmpresaItem1(){
    	return $this->getIdDocumentoPf() == '';
    }
    
    public function recuperaEstadoOS( $docid ){
    	$sql = "SELECT esdid FROM workflow.documento where docid = $docid";

        return $this->pegaUm($sql);
    }
    
    public function isCancelada(){
    	if (!$this->isOsEmpresaItem1()){
	    	return $this->getStatusOrdemServico() == 303;
    	} else {
    		return $this->getStatusOrdemServico() == 301 || $this->getStatusOrdemServico() == 302;
    	}
    }
    
    public function recuperePorId( $idOrdemServico )
    {
        $sql   = "SELECT *   
			FROM fabrica.ordemservico os 
			WHERE odsid = $idOrdemServico";
        $linha = parent::pegaLinha( $sql );
        return $this->montaObjeto( $linha );
    }
    
    public function temUrlDosArtefatos( $idOrdemServico )
    {
    	$sql = "SELECT 
					distinct p.prdid, p.prddsc
				FROM fabrica.servicofaseproduto sp
				INNER JOIN fabrica.fasedisciplinaproduto fdp 
					ON fdp.fdpid = sp.fdpid
				INNER JOIN fabrica.fasedisciplina fd 
					ON fd.fsdid = fdp.fsdid
				INNER JOIN fabrica.produto p 
					ON p.prdid  = fdp.prdid
					AND p.prdid <> 44
				INNER JOIN fabrica.analisesolicitacao fas 
					ON fas.ansid = sp.ansid
				INNER JOIN fabrica.ordemservico fos 
					ON fos.scsid = fas.scsid
				WHERE fos.odsid = " . $idOrdemServico . "
					and exists (select 1
							from fabrica.servicofaseproduto e
							inner join fabrica.fasedisciplinaproduto f
								on e.fdpid = f.fdpid
							where f.prdid = p.prdid
							and sp.sfprepositorio is null)";
    	
    	$linha = parent::pegaLinha( $sql );

    	return ( $linha['prdid'] ? false : true );
    }
    
    public function passivelDeGlosaEmpresaItem2(){
    	$passivel    = false;
    	$statusAtual = $this->getStatusOrdemServico();
    	switch ( $statusAtual )
    	{
    		case 275:
    			$passivel = true;
    			break;
    		case 371:
    			$passivel = true;
    			break;
    		case 277:
    			$passivel = true;
    			break;
    	}
    	return $passivel;
    }

    public function passivelDeGlosa()
    {
        $passivel    = false;
        $statusAtual = $this->getStatusOrdemServico();
        switch ( $statusAtual )
        {
            case StatusOrdemServico::EM_AVALIACAO:
                $passivel = true;
                break;
            case StatusOrdemServico::EM_APROVACAO:
                $passivel = true;
                break;
            case StatusOrdemServico::AGUARDANDO_HOMOLOGACAO_GESTOR:
                $passivel = true;
                break;
        }
        return $passivel;
    }

    public function possuiGlosa()
    {
        return $this->getIdGlosa() != null;
    }
    
    public function recupereOrdensServicoItemUmPeloIdSolicitacao( $idSolicitacao ){
    	$sql = "SELECT * from fabrica.ordemservico where scsid = $idSolicitacao and tosid = 1";
    	$resultSet = parent::carregar( $sql );
        if ( $resultSet != null )
        {
            foreach ( $resultSet as $linha )
            {
                $lista[] = $this->montaObjeto( $linha );
            }
        }
        return $lista;
    }

    public function recupereOSQueEstaoVinculadasAoMemorandoFabrica( $idMemorando )
    {
            //Recupera OS candidatas � memorando para SQUADRA
            $sql = "SELECT
                        os.scsid,
                        os.odsid,
                        os.ctrid,
                        CASE
                            WHEN docos.esdid = ".WF_ESTADO_OS_CANCELADA_COM_CUSTO." THEN
                                os.odsqtdpfestimada
                            ELSE
                                os.odsqtdpfdetalhada
                        END as odsqtdpfdetalhada,
                        os.tosid,
                        odssubtotalpf,
                        docos.esdid,
                        os.glosaid,
                        os.memoid,
                        memo.tpdpsid,

                        CASE
                            WHEN docos.esdid = ".WF_ESTADO_OS_CANCELADA_COM_CUSTO." THEN
                                12.00
                            ELSE
                               valor.valor
                        END AS porcentagem_disciplinas,
						
                        /*
                        ( SELECT vpc.vpcvalor FROM fabrica.vw_valorpfcontrato vpc where vpc.odsid = os.odsid ) as valor_pf,
                        CASE
                            WHEN docos.esdid = ".WF_ESTADO_OS_CANCELADA_COM_CUSTO." THEN
                                ROUND(os.odsqtdpfestimada * 12 / 100, 2)
                            ELSE
                                ROUND(os.odsqtdpfdetalhada *  valor.valor / 100, 2)
                        END
                        * ( SELECT vpc.vpcvalor FROM fabrica.vw_valorpfcontrato vpc where vpc.odsid = os.odsid ) as valor_a_receber
						*/
						
						( case when os.ctrid in (2,3) then (SELECT vpc.vpcvalor FROM fabrica.vw_valorpfcontrato vpc where vpc.odsid = os.odsid) else vim.vcmvalorcontratado end ) as valor_pf,
                        CASE
                            WHEN docos.esdid = ".WF_ESTADO_OS_CANCELADA_COM_CUSTO." THEN
                                ROUND(os.odsqtdpfestimada * 12 / 100, 2)
                            ELSE
                                ROUND(os.odsqtdpfdetalhada *  valor.valor / 100, 2)
                        END
                        * ( case when os.ctrid in (2,3) then (SELECT vpc.vpcvalor FROM fabrica.vw_valorpfcontrato vpc where vpc.odsid = os.odsid) else vim.vcmvalorcontratado end ) as valor_a_receber
                        
                        FROM fabrica.ordemservico os
                        INNER JOIN workflow.documento docos       ON os.docid  = docos.docid
                        INNER JOIN fabrica.solicitacaoservico ss  ON os.scsid  = ss.scsid AND ss.scsstatus = 'A'
                        INNER JOIN fabrica.analisesolicitacao ans ON ss.scsid  = ans.scsid
                        inner join (SELECT scsid, sum(fadvalor) as valor
                                    FROM (SELECT DISTINCT ans.scsid, ans.ansid,tpe.tpeid, tpe.tpedsc,dsp.dspid, dsp.dspdsc,fas.fasid, fas.fasdsc,fadvalor
                                            FROM fabrica.analisesolicitacao ans
                                                INNER JOIN fabrica.servicofaseproduto sfp on sfp.ansid=ans.ansid
                                                INNER JOIN fabrica.tipoexecucao tpe on tpe.tpeid=sfp.tpeid AND tpe.tpestatus = 'A' and tpe.tpeid = 1
                                                INNER JOIN fabrica.fasedisciplinaproduto fdp on fdp.fdpid=sfp.fdpid AND fdp.fdpstatus = 'A'
                                                INNER JOIN fabrica.produto prd on prd.prdid=fdp.prdid AND prd.prdstatus = 'A'
                                                INNER JOIN fabrica.fasedisciplina fsd on fsd.fsdid=fdp.fsdid
                                                INNER JOIN fabrica.disciplina dsp on dsp.dspid=fsd.dspid AND dsp.dspstatus = 'A'
                                                INNER JOIN fabrica.fase fas on fas.fasid=fsd.fasid AND fas.fasstatus = 'A'
                                            ) artefatos
                                    GROUP BY scsid
                                    ) valor ON valor.scsid= ss.scsid
                LEFT JOIN fabrica.memorando memo ON os.memoid = memo.memoid
                
                --RECUPERA NOVO VALOR_PF
                LEFT JOIN fabrica.vigenciacontratometricaitem vim ON vim.vgcid = ans.vgcid and ans.mtiid = vim.mtiid
                 
                WHERE os.memoid = {$idMemorando} 
                ORDER BY os.scsid";
		//dbg($sql,1);
        $resultSet = parent::carregar( $sql );
        if ( $resultSet != null )
        {
            foreach ( $resultSet as $linha )
            {
                $lista[] = $this->montaObjeto( $linha );
            }
        }
        return $lista;
    }
    
    public function recupereOSQueEstaoEmMemorandoDaFABRICA( $idMemorando )
    {
        //Recupera OS da FABRICA que Estao em memorando
        $sql = "SELECT
			os.scsid, os.odsid,
			os.odsqtdpfdetalhada,
			os.tosid,
			odssubtotalpf,
			esdc.esdid,
			(SELECT sum(fadvalor)  
                        FROM
                            (SELECT
                                distinct ans.scsid, ans.ansid,tpe.tpeid, tpe.tpedsc,dsp.dspid, dsp.dspdsc,fas.fasid, fas.fasdsc,fadvalor
                             FROM
                             fabrica.analisesolicitacao ans
                             INNER JOIN fabrica.servicofaseproduto sfp on sfp.ansid=ans.ansid
                             INNER JOIN fabrica.tipoexecucao tpe on tpe.tpeid=sfp.tpeid AND tpe.tpestatus = 'A' and tpe.tpeid = 1
                             INNER JOIN fabrica.fasedisciplinaproduto fdp on fdp.fdpid=sfp.fdpid AND fdp.fdpstatus = 'A'
                             INNER JOIN fabrica.produto prd on prd.prdid=fdp.prdid AND prd.prdstatus = 'A'
                             INNER JOIN fabrica.fasedisciplina fsd on fsd.fsdid=fdp.fsdid
                             INNER JOIN fabrica.disciplina dsp on dsp.dspid=fsd.dspid AND dsp.dspstatus = 'A'
                             INNER JOIN fabrica.fase fas on fas.fasid=fsd.fasid AND fas.fasstatus = 'A'
                             WHERE
                                ans.scsid=ss.scsid 
                             ORDER BY
                                ans.scsid,ans.ansid,tpe.tpeid,dsp.dspid,fas.fasid) 
                            artefatos) as porcentagem_disciplinas,
					
			(case when os.ctrid in (2,3) then (SELECT vpc.vpcvalor FROM fabrica.vw_valorpfcontrato vpc where vpc.odsid = os.odsid) else vim.vcmvalorcontratado end) as valor_pf,
			os.odsqtdpfdetalhada * (SELECT sum(fadvalor)  
                        FROM
                            (SELECT
                                distinct ans.scsid, ans.ansid,tpe.tpeid, tpe.tpedsc,dsp.dspid, dsp.dspdsc,fas.fasid, fas.fasdsc,fadvalor
                             FROM
                             fabrica.analisesolicitacao ans
                             INNER JOIN fabrica.servicofaseproduto sfp on sfp.ansid=ans.ansid
                             INNER JOIN fabrica.tipoexecucao tpe on tpe.tpeid=sfp.tpeid AND tpe.tpestatus = 'A' and tpe.tpeid = 1 
                             INNER JOIN fabrica.fasedisciplinaproduto fdp on fdp.fdpid=sfp.fdpid AND fdp.fdpstatus = 'A'
                             INNER JOIN fabrica.produto prd on prd.prdid=fdp.prdid AND prd.prdstatus = 'A'
                             INNER JOIN fabrica.fasedisciplina fsd on fsd.fsdid=fdp.fsdid
                             INNER JOIN fabrica.disciplina dsp on dsp.dspid=fsd.dspid AND dsp.dspstatus = 'A'
                             INNER JOIN fabrica.fase fas on fas.fasid=fsd.fasid AND fas.fasstatus = 'A'
                             WHERE
                                ans.scsid=ss.scsid 
                             ORDER BY
                                ans.scsid,ans.ansid,tpe.tpeid,dsp.dspid,fas.fasid) 
                            artefatos) / 100 * (case when os.ctrid in (2,3) then (SELECT vpc.vpcvalor FROM fabrica.vw_valorpfcontrato vpc where vpc.odsid = os.odsid) else vim.vcmvalorcontratado end) as valor_a_receber,
            os.glosaid,
            os.memoid
			
			FROM fabrica.solicitacaoservico ss
			INNER JOIN workflow.documento d
				ON ss.docid = d.docid
				
			INNER JOIN workflow.estadodocumento esdc
				ON d.esdid = esdc.esdid
				
			LEFT JOIN fabrica.ordemservico os
				ON ss.scsid = os.scsid and os.tosid = 1
				
			LEFT JOIN fabrica.analisesolicitacao ans
				ON ans.scsid = ss.scsid
				
			INNER JOIN demandas.sistemadetalhe sd
				ON sd.sidid = ss.sidid 
				
			LEFT JOIN fabrica.contrato c
				ON c.ctrid = ans.ctrid
                
			--RECUPERA NOVO VALOR_PF
            LEFT JOIN fabrica.vigenciacontratometricaitem vim ON vim.vgcid = ans.vgcid and ans.mtiid = vim.mtiid
				
			WHERE os.memoid = {$idMemorando} 
				AND ss.scsstatus = 'A'
			ORDER BY os.scsid";

        $resultSet = parent::carregar( $sql );
        if ( $resultSet != null )
        {
            foreach ( $resultSet as $linha )
            {
                $lista[] = $this->montaObjeto( $linha );
            }
        }
        return $lista;
    }

    public function recupereOSQueEstaoVinculadasAoMemorandoAUDITORA( $idMemorando )
    {
        //Recupera OS da Eficacia que Estao em memorando
        $sql = "SELECT
            os.ctrid,
			os.scsid, os.odsid,
			os.odsqtdpfestimada,
			os.odsqtdpfdetalhada,
			os.tosid,
			odssubtotalpf,
			(SELECT sum(fadvalor)  
                        FROM
                            (SELECT
                                distinct ans.scsid, ans.ansid,tpe.tpeid, tpe.tpedsc,dsp.dspid, dsp.dspdsc,fas.fasid, fas.fasdsc,fadvalor
                             FROM
                             fabrica.analisesolicitacao ans
                             INNER JOIN fabrica.servicofaseproduto sfp on sfp.ansid=ans.ansid
                             INNER JOIN fabrica.tipoexecucao tpe on tpe.tpeid=sfp.tpeid AND tpe.tpestatus = 'A' and tpe.tpeid = 1
                             INNER JOIN fabrica.fasedisciplinaproduto fdp on fdp.fdpid=sfp.fdpid AND fdp.fdpstatus = 'A'
                             INNER JOIN fabrica.produto prd on prd.prdid=fdp.prdid AND prd.prdstatus = 'A'
                             INNER JOIN fabrica.fasedisciplina fsd on fsd.fsdid=fdp.fsdid
                             INNER JOIN fabrica.disciplina dsp on dsp.dspid=fsd.dspid AND dsp.dspstatus = 'A'
                             INNER JOIN fabrica.fase fas on fas.fasid=fsd.fasid AND fas.fasstatus = 'A'
                             WHERE
                                ans.scsid=ss.scsid 
                             ORDER BY
                                ans.scsid,ans.ansid,tpe.tpeid,dsp.dspid,fas.fasid) 
                            artefatos) as porcentagem_disciplinas,
					
			( case when os.ctrid in (2,3) then (SELECT vpc.vpcvalor FROM fabrica.vw_valorpfcontrato vpc where vpc.odsid = os.odsid) else vim.vcmvalorcontratado end ) as valor_pf,
			CASE WHEN (os.odsqtdpfdetalhada > 0 )THEN 
				os.odsqtdpfdetalhada *  ( case when os.ctrid in (2,3) then (SELECT vpc.vpcvalor FROM fabrica.vw_valorpfcontrato vpc where vpc.odsid = os.odsid) else vim.vcmvalorcontratado end )
			ELSE 
				odsqtdpfestimada *  ( case when os.ctrid in (2,3) then (SELECT vpc.vpcvalor FROM fabrica.vw_valorpfcontrato vpc where vpc.odsid = os.odsid) else vim.vcmvalorcontratado end )
			END  as valor_a_receber,
                            
            os.glosaid,
            os.memoid
			
			FROM fabrica.solicitacaoservico ss

			LEFT JOIN fabrica.ordemservico os
				ON ss.scsid = os.scsid
				
			INNER JOIN workflow.documento d
				ON os.docidpf = d.docid
				
			INNER JOIN workflow.estadodocumento esdc
				ON d.esdid = esdc.esdid
				
			LEFT JOIN fabrica.analisesolicitacao ans
				ON ans.scsid = ss.scsid
				
			INNER JOIN demandas.sistemadetalhe sd
				ON sd.sidid = ss.sidid 
			/*	
			LEFT JOIN fabrica.contrato c
				ON c.ctrid = ans.ctrid
			*/	
			--RECUPERA NOVO VALOR_PF
            LEFT JOIN fabrica.vigenciacontratometricaitem vim ON vim.vgcid = os.vgcid and os.mtiid = vim.mtiid
            	
			WHERE os.memoid = {$idMemorando} AND ss.scsstatus = 'A' AND os.tosid in (2, 3) ORDER BY os.scsid";
				

        $resultSet = parent::carregar( $sql );
        if ( $resultSet != null )
        {
            foreach ( $resultSet as $linha )
            {
                $lista[] = $this->montaObjeto( $linha );
            }
        }
        return $lista;
    }
    
    public function recupereOSQueEstaoEmMemorandoDaAUDITORA( $idMemorando )
    {
        //Recupera OS da Eficacia que Estao em memorando
        $sql = "SELECT
			os.scsid, os.odsid,
			os.odsqtdpfestimada,
			os.odsqtdpfdetalhada,
			os.tosid,
			odssubtotalpf,
			(SELECT sum(fadvalor)  
                        FROM
                            (SELECT
                                distinct ans.scsid, ans.ansid,tpe.tpeid, tpe.tpedsc,dsp.dspid, dsp.dspdsc,fas.fasid, fas.fasdsc,fadvalor
                             FROM
                             fabrica.analisesolicitacao ans
                             INNER JOIN fabrica.servicofaseproduto sfp on sfp.ansid=ans.ansid
                             INNER JOIN fabrica.tipoexecucao tpe on tpe.tpeid=sfp.tpeid AND tpe.tpestatus = 'A' and tpe.tpeid = 1
                             INNER JOIN fabrica.fasedisciplinaproduto fdp on fdp.fdpid=sfp.fdpid AND fdp.fdpstatus = 'A'
                             INNER JOIN fabrica.produto prd on prd.prdid=fdp.prdid AND prd.prdstatus = 'A'
                             INNER JOIN fabrica.fasedisciplina fsd on fsd.fsdid=fdp.fsdid
                             INNER JOIN fabrica.disciplina dsp on dsp.dspid=fsd.dspid AND dsp.dspstatus = 'A'
                             INNER JOIN fabrica.fase fas on fas.fasid=fsd.fasid AND fas.fasstatus = 'A'
                             WHERE
                                ans.scsid=ss.scsid 
                             ORDER BY
                                ans.scsid,ans.ansid,tpe.tpeid,dsp.dspid,fas.fasid) 
                            artefatos) as porcentagem_disciplinas,
					
			( case when os.ctrid in (2,3) then (SELECT vpc.vpcvalor FROM fabrica.vw_valorpfcontrato vpc where vpc.odsid = os.odsid) else vim.vcmvalorcontratado end ) as valor_pf,
			CASE WHEN (os.odsqtdpfdetalhada > 0 )THEN 
				os.odsqtdpfdetalhada *  ( case when os.ctrid in (2,3) then (SELECT vpc.vpcvalor FROM fabrica.vw_valorpfcontrato vpc where vpc.odsid = os.odsid) else vim.vcmvalorcontratado end )
			ELSE 
				odsqtdpfestimada *  ( case when os.ctrid in (2,3) then (SELECT vpc.vpcvalor FROM fabrica.vw_valorpfcontrato vpc where vpc.odsid = os.odsid) else vim.vcmvalorcontratado end )
			END  as valor_a_receber,
                            
            os.glosaid,
            os.memoid
			
			FROM fabrica.solicitacaoservico ss

			LEFT JOIN fabrica.ordemservico os
				ON ss.scsid = os.scsid
				
			INNER JOIN workflow.documento d
				ON os.docidpf = d.docid
				
			INNER JOIN workflow.estadodocumento esdc
				ON d.esdid = esdc.esdid
				
			LEFT JOIN fabrica.analisesolicitacao ans
				ON ans.scsid = ss.scsid
				
			INNER JOIN demandas.sistemadetalhe sd
				ON sd.sidid = ss.sidid 
				
			LEFT JOIN fabrica.contrato c
				ON c.ctrid = ans.ctrid
            
			--RECUPERA NOVO VALOR_PF
            LEFT JOIN fabrica.vigenciacontratometricaitem vim ON vim.vgcid = ans.vgcid and ans.mtiid = vim.mtiid
				
			WHERE os.memoid = {$idMemorando}
            AND ss.scsstatus = 'A'
				AND os.tosid in (2, 3)
			ORDER BY os.scsid";
				

        $resultSet = parent::carregar( $sql );
        if ( $resultSet != null )
        {
            foreach ( $resultSet as $linha )
            {
                $lista[] = $this->montaObjeto( $linha );
            }
        }
        return $lista;
    }
    
//    public function recupereOSQueEstaoEmMemorandoDaAUDITORA( $idMemorando )
//    {
//        //Recupera OS da Eficacia que Estao em memorando
//        $sql = "SELECT
//			os.scsid, os.odsid,
//			os.odsqtdpfestimada,
//			os.odsqtdpfdetalhada,
//			os.tosid,
//			odssubtotalpf,
//			(SELECT sum(fadvalor)  
//                        FROM
//                            (SELECT
//                                distinct ans.scsid, ans.ansid,tpe.tpeid, tpe.tpedsc,dsp.dspid, dsp.dspdsc,fas.fasid, fas.fasdsc,fadvalor
//                             FROM
//                             fabrica.analisesolicitacao ans
//                             INNER JOIN fabrica.servicofaseproduto sfp on sfp.ansid=ans.ansid
//                             INNER JOIN fabrica.tipoexecucao tpe on tpe.tpeid=sfp.tpeid AND tpe.tpestatus = 'A' and tpe.tpeid = 1
//                             INNER JOIN fabrica.fasedisciplinaproduto fdp on fdp.fdpid=sfp.fdpid AND fdp.fdpstatus = 'A'
//                             INNER JOIN fabrica.produto prd on prd.prdid=fdp.prdid AND prd.prdstatus = 'A'
//                             INNER JOIN fabrica.fasedisciplina fsd on fsd.fsdid=fdp.fsdid
//                             INNER JOIN fabrica.disciplina dsp on dsp.dspid=fsd.dspid AND dsp.dspstatus = 'A'
//                             INNER JOIN fabrica.fase fas on fas.fasid=fsd.fasid AND fas.fasstatus = 'A'
//                             WHERE
//                                ans.scsid=ss.scsid 
//                             ORDER BY
//                                ans.scsid,ans.ansid,tpe.tpeid,dsp.dspid,fas.fasid) 
//                            artefatos) as porcentagem_disciplinas,
//					
//			( SELECT vpc.vpcvalor FROM fabrica.vw_valorpfcontrato vpc where vpc.odsid = os.odsid ) as valor_pf,
//			CASE WHEN (os.odsqtdpfdetalhada > 0 )THEN 
//				os.odsqtdpfdetalhada *  ( SELECT vpc.vpcvalor FROM fabrica.vw_valorpfcontrato vpc where vpc.odsid = os.odsid )
//			ELSE 
//				odsqtdpfestimada * ( SELECT vpc.vpcvalor FROM fabrica.vw_valorpfcontrato vpc where vpc.odsid = os.odsid )
//			END  as valor_a_receber,
//                            
//            os.glosaid,
//            os.memoid
//			
//			FROM fabrica.solicitacaoservico ss
//
//			LEFT JOIN fabrica.ordemservico os
//				ON ss.scsid = os.scsid
//				
//			INNER JOIN workflow.documento d
//				ON os.docidpf = d.docid
//				
//			INNER JOIN workflow.estadodocumento esdc
//				ON d.esdid = esdc.esdid
//				
//			LEFT JOIN fabrica.analisesolicitacao ans
//				ON ans.scsid = ss.scsid
//				
//			INNER JOIN demandas.sistemadetalhe sd
//				ON sd.sidid = ss.sidid 
//				
//			LEFT JOIN fabrica.contrato c
//				ON c.ctrid = ans.ctrid
//				
//			WHERE d.esdid = 371
//				AND ss.scsstatus = 'A'
//				AND os.tosid in (2, 3)
//				AND os.memoid = $idMemorando";
//
//        $resultSet  = parent::carregar( $sql );
//        $lista      = array();
//        if ( $resultSet != null )
//        {
//            foreach ( $resultSet as $linha )
//            {
//                $lista[] = $this->montaObjeto( $linha );
//            }
//        }
//        return $lista;
//    }

   public function temTermo( $ssId = null){
    	
    	//$retorno = false;
    	$retorno = '0';
    	
    	if( !empty($ssId) ){
    		
    		$sql = " select count(*) as total from fabrica.termo where scsid = ". $ssId;
    		$sql.= " and tptid in(". TIPO_TERMO_RECEBIMENTO_PROVISORIO .", ".TIPO_TERMO_SOLICITACAO_SERVICO.")";
    		
    		$resultSet = parent::carregar( $sql );
    		
    		if( $resultSet[0]['total'] != '0' ){
    			$retorno = '1';
    		}
    	}
    	
    	return $retorno;
   }

    public function recupereTodasOsCandidatasParaMemorandoDaFABRICA( $formMemoTpDpsId = null, $memoId = null, $empresaId = null, $anomemorando = null )
    {
    	
            $tmpTpDpsId_Inner = '';
            $tmpTpDpsId_Where = '';
            $tmpMemoId_Where  = '';
            $tmpEmpresaId_Where  = '';

            if( !empty($formMemoTpDpsId) )
            {
                $tmpTpDpsId_Inner = ' INNER JOIN fabrica.tiposervico tpser on ans.tpsid = tpser.tpsid ';
                $tmpTpDpsId_Where = ' AND tpser.tpdpsid = ' . $formMemoTpDpsId; 
            }

            if( !empty($memoId) )
            {
                $tmpMemoId_Where = ' OR os.memoid = ' . $memoId;
            }
            
     		if( !empty($empresaId) )
            {
                $tmpEmpresaId_Where = " AND os.ctrid in ( select ctrid from fabrica.contrato where ctrstatus = 'A' and entidcontratado = {$empresaId})"; 
            }
			
            if ( !empty($anomemorando) )
            {
            	$anoMemorando_Where = " AND to_char(ss.dataabertura, 'yyyy') = '{$anomemorando}'";
            }
            
            //Recupera OS candidatas � memorando para SQUADRA
            $sql = "SELECT
                        os.scsid,
                        os.odsid,
                        CASE
                            WHEN docos.esdid = ".WF_ESTADO_OS_CANCELADA_COM_CUSTO." THEN
                                os.odsqtdpfestimada
                            ELSE
                                os.odsqtdpfdetalhada
                        END as odsqtdpfdetalhada,
                        os.tosid,
                        odssubtotalpf,
                        docos.esdid,
                        os.glosaid,
                        os.memoid,
                        memo.tpdpsid,

                        CASE
                            WHEN docos.esdid = ".WF_ESTADO_OS_CANCELADA_COM_CUSTO." THEN
                                12.00
                            ELSE
                               valor.valor
                        END AS porcentagem_disciplinas,

                        ( case when os.ctrid in (2,3) then (SELECT vpc.vpcvalor FROM fabrica.vw_valorpfcontrato vpc where vpc.odsid = os.odsid) else vim.vcmvalorcontratado end ) as valor_pf,
                        CASE
                            WHEN docos.esdid = ".WF_ESTADO_OS_CANCELADA_COM_CUSTO." THEN
                                ROUND( ROUND(os.odsqtdpfestimada * 12 / 100, 2) * ( case when os.ctrid in (2,3) then (SELECT vpc.vpcvalor FROM fabrica.vw_valorpfcontrato vpc where vpc.odsid = os.odsid) else vim.vcmvalorcontratado end ) , 2 )
                            ELSE
                                ROUND( ROUND(os.odsqtdpfdetalhada *  valor.valor / 100, 2) * ( case when os.ctrid in (2,3) then (SELECT vpc.vpcvalor FROM fabrica.vw_valorpfcontrato vpc where vpc.odsid = os.odsid) else vim.vcmvalorcontratado end ) , 2 )
                        END
                        as valor_a_receber,
						to_char(ss.dataabertura, 'dd/mm/yyyy') as dataabertura
                        FROM fabrica.ordemservico os
                        INNER JOIN workflow.documento docos       ON os.docid  = docos.docid
                        INNER JOIN fabrica.solicitacaoservico ss  ON os.scsid  = ss.scsid AND ss.scsstatus = 'A'
                        INNER JOIN fabrica.analisesolicitacao ans ON ss.scsid  = ans.scsid
                        inner join (SELECT scsid, sum(fadvalor) as valor
                                    FROM (SELECT DISTINCT ans.scsid, ans.ansid,tpe.tpeid, tpe.tpedsc,dsp.dspid, dsp.dspdsc,fas.fasid, fas.fasdsc,fadvalor
                                            FROM fabrica.analisesolicitacao ans
                                                INNER JOIN fabrica.servicofaseproduto sfp on sfp.ansid=ans.ansid
                                                INNER JOIN fabrica.tipoexecucao tpe on tpe.tpeid=sfp.tpeid AND tpe.tpestatus = 'A' and tpe.tpeid = 1
                                                INNER JOIN fabrica.fasedisciplinaproduto fdp on fdp.fdpid=sfp.fdpid AND fdp.fdpstatus = 'A'
                                                INNER JOIN fabrica.produto prd on prd.prdid=fdp.prdid AND prd.prdstatus = 'A'
                                                INNER JOIN fabrica.fasedisciplina fsd on fsd.fsdid=fdp.fsdid
                                                INNER JOIN fabrica.disciplina dsp on dsp.dspid=fsd.dspid AND dsp.dspstatus = 'A'
                                                INNER JOIN fabrica.fase fas on fas.fasid=fsd.fasid AND fas.fasstatus = 'A'
                                            ) artefatos
                                    GROUP BY scsid
                                    ) valor ON valor.scsid= ss.scsid
                		LEFT JOIN fabrica.memorando memo ON os.memoid = memo.memoid

                		--RECUPERA NOVO VALOR_PF
                		LEFT JOIN fabrica.vigenciacontratometricaitem vim ON vim.vgcid = ans.vgcid and ans.mtiid = vim.mtiid
                		
                        ". $tmpTpDpsId_Inner ."
                        
                        WHERE docos.esdid     in (". WF_ESTADO_OS_AGUARDANDO_PAGAMENTO.", ".WF_ESTADO_OS_CANCELADA_COM_CUSTO.")
                        AND os.tosid         = ".TIPO_OS_GERAL."
                        AND (os.memoid IS NULL ". $tmpMemoId_Where .")
                        AND ans.ansgarantia = 'f'
                        ". $tmpTpDpsId_Where ."
                        ". $tmpEmpresaId_Where ."
                        ". $anoMemorando_Where ."
                        ORDER BY ss.scsid";
                //echo('<pre>');die($sql);
            $resultSet = parent::carregar( $sql );
            if ( $resultSet != null )
            {
                foreach ( $resultSet as $linha )
                {
                    $lista[] = $this->montaObjeto( $linha );
                }
            }
            return $lista;
    }

    public function recupereTodasOsCandidatasParaMemorandoDaAUDITORA( $formMemoTpDpsId = null, $memoId = null )
    {
		$tmpTpDpsId_Inner = '';
    	$tmpTpDpsId_Where = '';
    	$tmpMemoId		  = '';
    	    	 
//    	if( !empty($formMemoTpDpsId) )
//    	{
//    		$tmpTpDpsId_Inner = ' INNER JOIN fabrica.tiposervico tpser on ans.tpsid = tpser.tpsid ';
//    		$tmpTpDpsId_Where = ' AND tpser.tpdpsid = ' . $formMemoTpDpsId;
//    	}
    	
    	if( !empty($memoId) ){
    		$tmpMemoId = " and os.memoid<> ".$memoId;
    	}

        //Recupera OS candidatas � memorando para EFICACIA
        $sql = "SELECT
			os.scsid, os.odsid,
			os.odsqtdpfestimada,
			os.odsqtdpfdetalhada,
			os.tosid,
			odssubtotalpf,
			(SELECT sum(fadvalor)  
                        FROM
                            (SELECT
                                distinct ans.scsid, ans.ansid,tpe.tpeid, tpe.tpedsc,dsp.dspid, dsp.dspdsc,fas.fasid, fas.fasdsc,fadvalor
                             FROM
                             fabrica.analisesolicitacao ans
                             INNER JOIN fabrica.servicofaseproduto sfp on sfp.ansid=ans.ansid
                             INNER JOIN fabrica.tipoexecucao tpe on tpe.tpeid=sfp.tpeid AND tpe.tpestatus = 'A' and tpe.tpeid = 1
                             INNER JOIN fabrica.fasedisciplinaproduto fdp on fdp.fdpid=sfp.fdpid AND fdp.fdpstatus = 'A'
                             INNER JOIN fabrica.produto prd on prd.prdid=fdp.prdid AND prd.prdstatus = 'A'
                             INNER JOIN fabrica.fasedisciplina fsd on fsd.fsdid=fdp.fsdid
                             INNER JOIN fabrica.disciplina dsp on dsp.dspid=fsd.dspid AND dsp.dspstatus = 'A'
                             INNER JOIN fabrica.fase fas on fas.fasid=fsd.fasid AND fas.fasstatus = 'A'
                             WHERE
                                ans.scsid=ss.scsid 
                             ORDER BY
                                ans.scsid,ans.ansid,tpe.tpeid,dsp.dspid,fas.fasid) 
                            artefatos) as porcentagem_disciplinas,
					
			( case when os.ctrid in (2,3) then (SELECT vpc.vpcvalor FROM fabrica.vw_valorpfcontrato vpc where vpc.odsid = os.odsid) else vim.vcmvalorcontratado end ) as valor_pf,
			CASE WHEN (os.odsqtdpfdetalhada > 0 )THEN 
				os.odsqtdpfdetalhada *  ( case when os.ctrid in (2,3) then (SELECT vpc.vpcvalor FROM fabrica.vw_valorpfcontrato vpc where vpc.odsid = os.odsid) else vim.vcmvalorcontratado end )
			ELSE 
				odsqtdpfestimada *  ( case when os.ctrid in (2,3) then (SELECT vpc.vpcvalor FROM fabrica.vw_valorpfcontrato vpc where vpc.odsid = os.odsid) else vim.vcmvalorcontratado end )
			END  as valor_a_receber,
            os.glosaid,
            os.memoid
			
			FROM fabrica.solicitacaoservico ss

			LEFT JOIN fabrica.ordemservico os
				ON ss.scsid = os.scsid
				
			INNER JOIN workflow.documento d
				ON os.docidpf = d.docid
				
			INNER JOIN workflow.estadodocumento esdc
				ON d.esdid = esdc.esdid
				
			LEFT JOIN fabrica.analisesolicitacao ans
				ON ans.scsid = ss.scsid
			
			INNER JOIN demandas.sistemadetalhe sd
				ON sd.sidid = ss.sidid 
				
			LEFT JOIN fabrica.contrato c
				ON c.ctrid = ans.ctrid

			--RECUPERA NOVO VALOR_PF
            LEFT JOIN fabrica.vigenciacontratometricaitem vim ON vim.vgcid = ans.vgcid and ans.mtiid = vim.mtiid
				
			". $tmpTpDpsId_Inner ."
				
			WHERE d.esdid = ". WF_ESTADO_CPF_AGUARDANDO_PAGAMENTO ."
				AND ss.scsstatus = 'A'
				AND os.tosid in (". TIPO_OS_CONTAGEM_ESTIMADA .", ". TIPO_OS_CONTAGEM_DETALHADA .")
				AND os.odsid NOT IN (select os.odsid from fabrica.ordemservico os where os.memoid is not null $tmpMemoId)
				". $tmpTpDpsId_Where ."	
			ORDER BY 
				os.scsid";
    	
    	//die( $sql );
		//return $sql;
        $resultSet  = parent::carregar( $sql );
        $lista      = array();
        if ( $resultSet != null )
        {
            foreach ( $resultSet as $linha )
            {
                $lista[] = $this->montaObjeto( $linha );
            }
        }
        return $lista;
    }
    
    /**
     * Recupera a porcentagem de disciplinas contratadas de uma solicita��o 
     * a partir de sua ordem de servi�o vinculada
     * 
     * @param int $odsId 
     * @return  float
     */
//    public function recuperarPorcentagemDisciplinasContratadas( $odsId )
//    {
//        $sql    = "SELECT artefatos.scsid, doc.esdid as estado_ss, sum(fadvalor) as porcentagem_contratada
//                    FROM
//                        ( 
//                            SELECT DISTINCT ans.scsid, ans.ansid,tpe.tpeid, tpe.tpedsc,dsp.dspid, dsp.dspdsc,fas.fasid, fas.fasdsc,fadvalor
//                             FROM
//                             fabrica.analisesolicitacao ans
//                             INNER JOIN fabrica.servicofaseproduto sfp 
//                                ON sfp.ansid=ans.ansid
//                             INNER JOIN fabrica.tipoexecucao tpe 
//                                ON tpe.tpeid=sfp.tpeid 
//                                AND tpe.tpestatus = 'A' 
//                                AND tpe.tpeid = 1
//                             INNER JOIN fabrica.fasedisciplinaproduto fdp 
//                                ON fdp.fdpid=sfp.fdpid 
//                                AND fdp.fdpstatus = 'A'
//                             INNER JOIN fabrica.produto prd 
//                                ON prd.prdid=fdp.prdid 
//                                AND prd.prdstatus = 'A'
//                             INNER JOIN fabrica.fasedisciplina fsd 
//                                ON fsd.fsdid=fdp.fsdid
//                             INNER JOIN fabrica.disciplina dsp 
//                                ON dsp.dspid=fsd.dspid 
//                                AND dsp.dspstatus = 'A'
//                             INNER JOIN fabrica.fase fas 
//                                ON fas.fasid=fsd.fasid AND fas.fasstatus = 'A'
//                             INNER JOIN fabrica.ordemservico ods
//                                ON ans.scsid    = ods.scsid
//                                AND ods.odsid   = {$odsId}
//                             ORDER BY ans.scsid,ans.ansid,tpe.tpeid,dsp.dspid,fas.fasid
//                        ) artefatos
//                                
//                    INNER JOIN fabrica.solicitacaoservico ss
//                                ON artefatos.scsid = ss.scsid
//                    INNER JOIN workflow.documento doc
//                                ON ss.docid = doc.docid
//                    GROUP BY artefatos.scsid, doc.esdid";
//        
//       $porcentagemDisciplina   = parent::pegaLinha($sql);
//       
//       if( $porcentagemDisciplina['estado_ss'] == WF_ESTADO_CANCELADA_COM_CUSTO )
//       {
//           $porcentagemDisciplina['porcentagem_contratada'] = 12;
//       }
//       
//       return (float) $porcentagemDisciplina['porcentagem_contratada'];
//    }

    public function recupereTodasOsCandidatasParaMemorandoDaFABRICAInclusiveQueEstaoEmMemorando()
    {
        //Recupera OS candidatas � memorando para SQUADRA
        $sql = "SELECT
			os.scsid, os.odsid,
			os.odsqtdpfdetalhada,
			os.tosid,
			odssubtotalpf,
			esdc.esdid,
			(SELECT sum(fadvalor)  
                        FROM
                            (SELECT
                                distinct ans.scsid, ans.ansid,tpe.tpeid, tpe.tpedsc,dsp.dspid, dsp.dspdsc,fas.fasid, fas.fasdsc,fadvalor
                             FROM
                             fabrica.analisesolicitacao ans
                             INNER JOIN fabrica.servicofaseproduto sfp on sfp.ansid=ans.ansid
                             INNER JOIN fabrica.tipoexecucao tpe on tpe.tpeid=sfp.tpeid AND tpe.tpestatus = 'A' and tpe.tpeid = 1
                             INNER JOIN fabrica.fasedisciplinaproduto fdp on fdp.fdpid=sfp.fdpid AND fdp.fdpstatus = 'A'
                             INNER JOIN fabrica.produto prd on prd.prdid=fdp.prdid AND prd.prdstatus = 'A'
                             INNER JOIN fabrica.fasedisciplina fsd on fsd.fsdid=fdp.fsdid
                             INNER JOIN fabrica.disciplina dsp on dsp.dspid=fsd.dspid AND dsp.dspstatus = 'A'
                             INNER JOIN fabrica.fase fas on fas.fasid=fsd.fasid AND fas.fasstatus = 'A'
                             WHERE
                                ans.scsid=ss.scsid 
                             ORDER BY
                                ans.scsid,ans.ansid,tpe.tpeid,dsp.dspid,fas.fasid) 
                            artefatos) as porcentagem_disciplinas,
					
			( case when os.ctrid in (2,3) then (SELECT vpc.vpcvalor FROM fabrica.vw_valorpfcontrato vpc where vpc.odsid = os.odsid) else vim.vcmvalorcontratado end ) as valor_pf,
			os.odsqtdpfdetalhada * (SELECT sum(fadvalor)  
                        FROM
                            (SELECT
                                distinct ans.scsid, ans.ansid,tpe.tpeid, tpe.tpedsc,dsp.dspid, dsp.dspdsc,fas.fasid, fas.fasdsc,fadvalor
                             FROM
                             fabrica.analisesolicitacao ans
                             INNER JOIN fabrica.servicofaseproduto sfp on sfp.ansid=ans.ansid
                             INNER JOIN fabrica.tipoexecucao tpe on tpe.tpeid=sfp.tpeid AND tpe.tpestatus = 'A' and tpe.tpeid = 1 
                             INNER JOIN fabrica.fasedisciplinaproduto fdp on fdp.fdpid=sfp.fdpid AND fdp.fdpstatus = 'A'
                             INNER JOIN fabrica.produto prd on prd.prdid=fdp.prdid AND prd.prdstatus = 'A'
                             INNER JOIN fabrica.fasedisciplina fsd on fsd.fsdid=fdp.fsdid
                             INNER JOIN fabrica.disciplina dsp on dsp.dspid=fsd.dspid AND dsp.dspstatus = 'A'
                             INNER JOIN fabrica.fase fas on fas.fasid=fsd.fasid AND fas.fasstatus = 'A'
                             WHERE
                                ans.scsid=ss.scsid 
                             ORDER BY
                                ans.scsid,ans.ansid,tpe.tpeid,dsp.dspid,fas.fasid) 
                            artefatos) / 100 * ( case when os.ctrid in (2,3) then (SELECT vpc.vpcvalor FROM fabrica.vw_valorpfcontrato vpc where vpc.odsid = os.odsid) else vim.vcmvalorcontratado end ) as valor_a_receber,
            os.glosaid,
            os.memoid
			
			FROM fabrica.solicitacaoservico ss
			INNER JOIN workflow.documento d
				ON ss.docid = d.docid
				
			INNER JOIN workflow.estadodocumento esdc
				ON d.esdid = esdc.esdid
				
			LEFT JOIN fabrica.ordemservico os
				ON ss.scsid = os.scsid and os.tosid = 1
				
			LEFT JOIN fabrica.analisesolicitacao ans
				ON ans.scsid = ss.scsid
				
			INNER JOIN demandas.sistemadetalhe sd
				ON sd.sidid = ss.sidid 
				
			LEFT JOIN fabrica.contrato c
				ON c.ctrid = ans.ctrid

			--RECUPERA NOVO VALOR_PF
            LEFT JOIN fabrica.vigenciacontratometricaitem vim ON vim.vgcid = ans.vgcid and ans.mtiid = vim.mtiid
				
			WHERE d.esdid IN (361,300)
				AND ss.scsstatus = 'A'
			ORDER BY 
				os.scsid";

        $resultSet  = parent::carregar( $sql );
        $lista      = array();
        if ( $resultSet != null )
        {
            foreach ( $resultSet as $linha )
            {
                $lista[] = $this->montaObjeto( $linha );
            }
        }
        return $lista;
    }
    
    /**
     * Recupera a porcentagem de disciplinas contratadas de uma solicita��o 
     * a partir de sua ordem de servi�o vinculada
     * 
     * @param int $odsId 
     * @return  float
     */
    public function recuperarPorcentagemDisciplinasContratadas( $odsId )
    {
        $sql    = "SELECT artefatos.scsid, doc.esdid as estado_ss, sum(fadvalor) as porcentagem_contratada
                    FROM
                        ( 
                            SELECT DISTINCT ans.scsid, ans.ansid,tpe.tpeid, tpe.tpedsc,dsp.dspid, dsp.dspdsc,fas.fasid, fas.fasdsc,fadvalor
                             FROM
                             fabrica.analisesolicitacao ans
                             INNER JOIN fabrica.servicofaseproduto sfp 
                                ON sfp.ansid=ans.ansid
                             INNER JOIN fabrica.tipoexecucao tpe 
                                ON tpe.tpeid=sfp.tpeid 
                                AND tpe.tpestatus = 'A' 
                                AND tpe.tpeid = 1
                             INNER JOIN fabrica.fasedisciplinaproduto fdp 
                                ON fdp.fdpid=sfp.fdpid 
                                AND fdp.fdpstatus = 'A'
                             INNER JOIN fabrica.produto prd 
                                ON prd.prdid=fdp.prdid 
                                AND prd.prdstatus = 'A'
                             INNER JOIN fabrica.fasedisciplina fsd 
                                ON fsd.fsdid=fdp.fsdid
                             INNER JOIN fabrica.disciplina dsp 
                                ON dsp.dspid=fsd.dspid 
                                AND dsp.dspstatus = 'A'
                             INNER JOIN fabrica.fase fas 
                                ON fas.fasid=fsd.fasid AND fas.fasstatus = 'A'
                             INNER JOIN fabrica.ordemservico ods
                                ON ans.scsid    = ods.scsid
                                AND ods.odsid   = {$odsId}
                             ORDER BY ans.scsid,ans.ansid,tpe.tpeid,dsp.dspid,fas.fasid
                        ) artefatos
                                
                    INNER JOIN fabrica.solicitacaoservico ss
                                ON artefatos.scsid = ss.scsid
                    INNER JOIN workflow.documento doc
                                ON ss.docid = doc.docid
                    GROUP BY artefatos.scsid, doc.esdid";
        
       $porcentagemDisciplina   = parent::pegaLinha($sql);
       
       if( $porcentagemDisciplina['estado_ss'] == WF_ESTADO_CANCELADA_COM_CUSTO )
       {
           $porcentagemDisciplina['porcentagem_contratada'] = 12;
       }
       
       return (float) $porcentagemDisciplina['porcentagem_contratada'];
    }

    /**
     * Recupera as os dispon�veis para apresenta��o no memorando
     * que n�o estejam vinculadas h� nenhuma memorando j� criado
     * ou vinculadas h� um determinado memorando
     * @param int $idMemorando
     * @return array
     */
    public function recupereTodasOsCandidatasParaMemorandoDaFABRICAMenosQueEstaoEmMemorando( $memoId = null, $formMemoTpDpsId = null, $empresaId = null, $anomemorando = null )
    {
            $tmpTpDpsId_Inner = '';
            $tmpTpDpsId_Where = '';
            $tmpMemoId_Where  = '';

            if( !empty($formMemoTpDpsId) )
            {
                $tmpTpDpsId_Inner = ' INNER JOIN fabrica.tiposervico tpser on ans.tpsid = tpser.tpsid ';
                $tmpTpDpsId_Where = ' AND tpser.tpdpsid = ' . $formMemoTpDpsId; 
            }

            if( !empty($memoId) )
            {
                $tmpMemoId_Where = ' OR os.memoid = ' . $memoId;
            }
            
            if( !empty($empresaId) )
            {
                $tmpEmpresaId_Where = " AND os.ctrid in ( select ctrid from fabrica.contrato where ctrstatus = 'A' and entidcontratado = {$empresaId})"; 
            }
			
            if ( !empty($anomemorando) )
            {
            	$anoMemorando_Where = " AND to_char(ss.dataabertura, 'yyyy') = '{$anomemorando}'";
            }
            
            //Recupera OS candidatas  memorando para SQUADRA
            $sql = "SELECT
                        os.scsid,
                        os.odsid,
                        CASE
                            WHEN docos.esdid = ".WF_ESTADO_OS_CANCELADA_COM_CUSTO." THEN
                                os.odsqtdpfestimada
                            ELSE
                                os.odsqtdpfdetalhada
                        END as odsqtdpfdetalhada,
                        os.tosid,
                        odssubtotalpf,
                        docos.esdid,
                        os.glosaid,
                        os.memoid,
                		memo.tpdpsid,
                

                        CASE
                            WHEN docos.esdid = ".WF_ESTADO_OS_CANCELADA_COM_CUSTO." THEN
                                12.00
                            ELSE
                               valor.valor
                        END AS porcentagem_disciplinas,
						/*
                        ( SELECT vpc.vpcvalor FROM fabrica.vw_valorpfcontrato vpc where vpc.odsid = os.odsid ) as valor_pf,
                        CASE
                            WHEN docos.esdid = ".WF_ESTADO_OS_CANCELADA_COM_CUSTO." THEN
                                ROUND( ROUND(os.odsqtdpfestimada * 12 / 100, 2) * ( SELECT vpc.vpcvalor FROM fabrica.vw_valorpfcontrato vpc where vpc.odsid = os.odsid ), 2)
                            ELSE
                                ROUND( ROUND(os.odsqtdpfdetalhada *  valor.valor / 100, 2) * ( SELECT vpc.vpcvalor FROM fabrica.vw_valorpfcontrato vpc where vpc.odsid = os.odsid ), 2)
                        END
                        as valor_a_receber
						*/
						( case when os.ctrid in (2,3) then (SELECT vpc.vpcvalor FROM fabrica.vw_valorpfcontrato vpc where vpc.odsid = os.odsid) else vim.vcmvalorcontratado end ) as valor_pf,
                        CASE
                            WHEN docos.esdid = ".WF_ESTADO_OS_CANCELADA_COM_CUSTO." THEN
                                ROUND( ROUND(os.odsqtdpfestimada * 12 / 100, 2) * ( case when os.ctrid in (2,3) then (SELECT vpc.vpcvalor FROM fabrica.vw_valorpfcontrato vpc where vpc.odsid = os.odsid) else vim.vcmvalorcontratado end ), 2)
                            ELSE
                                ROUND( ROUND(os.odsqtdpfdetalhada *  valor.valor / 100, 2) * ( case when os.ctrid in (2,3) then (SELECT vpc.vpcvalor FROM fabrica.vw_valorpfcontrato vpc where vpc.odsid = os.odsid) else vim.vcmvalorcontratado end ), 2)
                        END
                        as valor_a_receber,
                        to_char(ss.dataabertura, 'dd/mm/yyyy') as dataabertura
                        FROM fabrica.ordemservico os
                        INNER JOIN workflow.documento docos       ON os.docid  = docos.docid
                        INNER JOIN fabrica.solicitacaoservico ss  ON os.scsid  = ss.scsid AND ss.scsstatus = 'A'". $anoMemorando_Where ."
                        INNER JOIN fabrica.analisesolicitacao ans ON ss.scsid  = ans.scsid
                        inner join (SELECT scsid, sum(fadvalor) as valor
                                    FROM (SELECT DISTINCT ans.scsid, ans.ansid,tpe.tpeid, tpe.tpedsc,dsp.dspid, dsp.dspdsc,fas.fasid, fas.fasdsc,fadvalor
                                            FROM fabrica.analisesolicitacao ans
                                                INNER JOIN fabrica.servicofaseproduto sfp on sfp.ansid=ans.ansid
                                                INNER JOIN fabrica.tipoexecucao tpe on tpe.tpeid=sfp.tpeid AND tpe.tpestatus = 'A' and tpe.tpeid = 1
                                                INNER JOIN fabrica.fasedisciplinaproduto fdp on fdp.fdpid=sfp.fdpid AND fdp.fdpstatus = 'A'
                                                INNER JOIN fabrica.produto prd on prd.prdid=fdp.prdid AND prd.prdstatus = 'A'
                                                INNER JOIN fabrica.fasedisciplina fsd on fsd.fsdid=fdp.fsdid
                                                INNER JOIN fabrica.disciplina dsp on dsp.dspid=fsd.dspid AND dsp.dspstatus = 'A'
                                                INNER JOIN fabrica.fase fas on fas.fasid=fsd.fasid AND fas.fasstatus = 'A'
                                            ) artefatos
                                    GROUP BY scsid
                                    ) valor ON valor.scsid= ss.scsid
                		LEFT JOIN fabrica.memorando memo ON os.memoid = memo.memoid
                		
                		--RECUPERA NOVO VALOR_PF
                        LEFT JOIN fabrica.vigenciacontratometricaitem vim ON vim.vgcid = ans.vgcid and ans.mtiid = vim.mtiid
                        
                        ". $tmpTpDpsId_Inner ."
                        
                        WHERE docos.esdid     in (". WF_ESTADO_OS_AGUARDANDO_PAGAMENTO.", ".WF_ESTADO_OS_CANCELADA_COM_CUSTO.")
                        AND os.tosid         = ".TIPO_OS_GERAL."
                        AND (os.memoid IS NULL ". $tmpMemoId_Where .")
                        AND ans.ansgarantia = 'f'
                        ". $tmpTpDpsId_Where ."
                        ". $tmpEmpresaId_Where ."
                        ORDER BY ss.scsid";
//die($sql);
			//dbg($sql,1);
			//ver($sql);
            $resultSet = parent::carregar( $sql );
            if ( $resultSet != null )
            {
                foreach ( $resultSet as $linha )
                {
                    $lista[] = $this->montaObjeto( $linha );
                }
            }
            return $lista;
    }

    public function recupereTodasOsCandidatasParaMemorandoDaAUDITORAInclusiveQueEstaoEmMemorando()
    {
        //Recupera OS candidatas � memorando para EFICACIA
        $sql = "SELECT
			os.scsid, os.odsid,
			os.odsqtdpfestimada,
			os.odsqtdpfdetalhada,
			os.tosid,
			odssubtotalpf,
			(SELECT sum(fadvalor)  
                        FROM
                            (SELECT
                                distinct ans.scsid, ans.ansid,tpe.tpeid, tpe.tpedsc,dsp.dspid, dsp.dspdsc,fas.fasid, fas.fasdsc,fadvalor
                             FROM
                             fabrica.analisesolicitacao ans
                             INNER JOIN fabrica.servicofaseproduto sfp on sfp.ansid=ans.ansid
                             INNER JOIN fabrica.tipoexecucao tpe on tpe.tpeid=sfp.tpeid AND tpe.tpestatus = 'A' and tpe.tpeid = 1
                             INNER JOIN fabrica.fasedisciplinaproduto fdp on fdp.fdpid=sfp.fdpid AND fdp.fdpstatus = 'A'
                             INNER JOIN fabrica.produto prd on prd.prdid=fdp.prdid AND prd.prdstatus = 'A'
                             INNER JOIN fabrica.fasedisciplina fsd on fsd.fsdid=fdp.fsdid
                             INNER JOIN fabrica.disciplina dsp on dsp.dspid=fsd.dspid AND dsp.dspstatus = 'A'
                             INNER JOIN fabrica.fase fas on fas.fasid=fsd.fasid AND fas.fasstatus = 'A'
                             WHERE
                                ans.scsid=ss.scsid 
                             ORDER BY
                                ans.scsid,ans.ansid,tpe.tpeid,dsp.dspid,fas.fasid) 
                            artefatos) as porcentagem_disciplinas,
					
			( case when os.ctrid in (2,3) then (SELECT vpc.vpcvalor FROM fabrica.vw_valorpfcontrato vpc where vpc.odsid = os.odsid) else vim.vcmvalorcontratado end ) as valor_pf,
			CASE WHEN (os.odsqtdpfdetalhada > 0 )THEN 
				os.odsqtdpfdetalhada *  ( case when os.ctrid in (2,3) then (SELECT vpc.vpcvalor FROM fabrica.vw_valorpfcontrato vpc where vpc.odsid = os.odsid) else vim.vcmvalorcontratado end )
			ELSE 
				odsqtdpfestimada *  ( case when os.ctrid in (2,3) then (SELECT vpc.vpcvalor FROM fabrica.vw_valorpfcontrato vpc where vpc.odsid = os.odsid) else vim.vcmvalorcontratado end )
			END  as valor_a_receber,
            os.glosaid,
            os.memoid
			
			FROM fabrica.solicitacaoservico ss

			LEFT JOIN fabrica.ordemservico os
				ON ss.scsid = os.scsid
				
			INNER JOIN workflow.documento d
				ON os.docidpf = d.docid
				
			INNER JOIN workflow.estadodocumento esdc
				ON d.esdid = esdc.esdid
				
			LEFT JOIN fabrica.analisesolicitacao ans
				ON ans.scsid = ss.scsid
				
			INNER JOIN demandas.sistemadetalhe sd
				ON sd.sidid = ss.sidid 
				
			LEFT JOIN fabrica.contrato c
				ON c.ctrid = ans.ctrid

			--RECUPERA NOVO VALOR_PF
            LEFT JOIN fabrica.vigenciacontratometricaitem vim ON vim.vgcid = ans.vgcid and ans.mtiid = vim.mtiid
				
			WHERE d.esdid = 371
				AND ss.scsstatus = 'A'
				AND os.tosid in (2, 3)
			ORDER BY 
				os.scsid";

        $resultSet = parent::carregar( $sql );
        if ( $resultSet != null )
        {
            foreach ( $resultSet as $linha )
            {
                $lista[] = $this->montaObjeto( $linha );
            }
        }
        return $lista;
    }

    public function recupereTodasOsCandidatasParaMemorandoDaAUDITORAMenosQueEstaoEmMemorando( $idMemorando = null, $formmemotpdpsid=null, $empresaId = null, $anomemorando = null )
    {
        $tmpTpDpsId_Inner = '';
        $tmpTpDpsId_Where = '';
        
        $andIdMemorando = '';
        $tmpEmpresaId_Where = '';
        
        /*
        if( !empty($formmemotpdpsid) )
        {
            $andTipoDespesa = "AND tps.tpdpsid = {$formmemotpdpsid}";
        }
		*/
    	if( !empty($formmemotpdpsid) )
        {
        	$tmpTpDpsId_Inner = ' INNER JOIN fabrica.tiposervico tpser on ans.tpsid = tpser.tpsid ';
            $tmpTpDpsId_Where = ' AND tpser.tpdpsid = ' . $formmemotpdpsid; 
        }

        if( !empty($idMemorando) )
        {
        	$andIdMemorando = "OR os.memoid = {$idMemorando}" ;
        }
        
        if( !empty($empresaId) )
        {
        	$tmpEmpresaId_Where = " AND os.ctrid in ( select ctrid from fabrica.contrato where ctrstatus = 'A' and entidcontratado = {$empresaId})"; 
        }   
        
        if ( !empty($anomemorando) )
        {
        	$anoMemorando_Where = " AND to_char(ss.anoabertura, 'yyyy') = '{$anomemorando}'";
        }
        
        //Recupera OS candidatas � memorando para EFICACIA
        $sql = "SELECT
			os.scsid, os.odsid,
			os.odsqtdpfestimada,
			os.odsqtdpfdetalhada,
			os.tosid,
			odssubtotalpf, 
            memo.tpdpsid, 
			(SELECT sum(fadvalor)  
                        FROM
                            (SELECT
                                distinct ans.scsid, ans.ansid,tpe.tpeid, tpe.tpedsc,dsp.dspid, dsp.dspdsc,fas.fasid, fas.fasdsc,fadvalor
                             FROM
                             fabrica.analisesolicitacao ans
                             INNER JOIN fabrica.servicofaseproduto sfp on sfp.ansid=ans.ansid
                             INNER JOIN fabrica.tipoexecucao tpe on tpe.tpeid=sfp.tpeid AND tpe.tpestatus = 'A' and tpe.tpeid = 1
                             INNER JOIN fabrica.fasedisciplinaproduto fdp on fdp.fdpid=sfp.fdpid AND fdp.fdpstatus = 'A'
                             INNER JOIN fabrica.produto prd on prd.prdid=fdp.prdid AND prd.prdstatus = 'A'
                             INNER JOIN fabrica.fasedisciplina fsd on fsd.fsdid=fdp.fsdid
                             INNER JOIN fabrica.disciplina dsp on dsp.dspid=fsd.dspid AND dsp.dspstatus = 'A'
                             INNER JOIN fabrica.fase fas on fas.fasid=fsd.fasid AND fas.fasstatus = 'A'
                             WHERE
                                ans.scsid=ss.scsid 
                             ORDER BY
                                ans.scsid,ans.ansid,tpe.tpeid,dsp.dspid,fas.fasid) 
                            artefatos) as porcentagem_disciplinas,
			/*		
			( SELECT vpc.vpcvalor FROM fabrica.vw_valorpfcontrato vpc where vpc.odsid = os.odsid ) as valor_pf,
			CASE WHEN (os.odsqtdpfdetalhada > 0 )THEN 
				os.odsqtdpfdetalhada *  ( SELECT vpc.vpcvalor FROM fabrica.vw_valorpfcontrato vpc where vpc.odsid = os.odsid )
			ELSE 
				odsqtdpfestimada *  ( SELECT vpc.vpcvalor FROM fabrica.vw_valorpfcontrato vpc where vpc.odsid = os.odsid )
			END  as valor_a_receber,
			*/
			( case when os.ctrid in (2,3) then (SELECT vpc.vpcvalor FROM fabrica.vw_valorpfcontrato vpc where vpc.odsid = os.odsid) else vim.vcmvalorcontratado end ) as valor_pf,
			CASE WHEN (os.odsqtdpfdetalhada > 0 )THEN 
				os.odsqtdpfdetalhada *  ( case when os.ctrid in (2,3) then (SELECT vpc.vpcvalor FROM fabrica.vw_valorpfcontrato vpc where vpc.odsid = os.odsid) else vim.vcmvalorcontratado end )
			ELSE 
				odsqtdpfestimada *  ( case when os.ctrid in (2,3) then (SELECT vpc.vpcvalor FROM fabrica.vw_valorpfcontrato vpc where vpc.odsid = os.odsid) else vim.vcmvalorcontratado end )
			END  as valor_a_receber,
            os.glosaid,
            os.memoid,
        	to_char(ss.dataabertura, 'dd/mm/yyyy') as dataabertura
			
			FROM fabrica.solicitacaoservico ss

			LEFT JOIN fabrica.ordemservico os
				ON ss.scsid = os.scsid
            LEFT JOIN fabrica.memorando memo
                ON os.memoid = memo.memoid
				
			INNER JOIN workflow.documento d
				ON os.docidpf = d.docid
				
			INNER JOIN workflow.estadodocumento esdc
				ON d.esdid = esdc.esdid
				
			LEFT JOIN fabrica.analisesolicitacao ans
				ON ans.scsid = ss.scsid
				
			INNER JOIN demandas.sistemadetalhe sd
				ON sd.sidid = ss.sidid 
			/*	
			LEFT JOIN fabrica.contrato c
				ON c.ctrid = ans.ctrid
            LEFT JOIN fabrica.tiposervico tps
                ON ans.tpsid = tps.tpsid
            */    
			--RECUPERA NOVO VALOR_PF
            LEFT JOIN fabrica.vigenciacontratometricaitem vim ON vim.vgcid = os.vgcid and os.mtiid = vim.mtiid
            	
            ". $tmpTpDpsId_Inner ."
            
			WHERE d.esdid = 371
				AND ss.scsstatus = 'A'
				AND os.tosid in (2, 3)
                AND (os.memoid IS NULL {$andIdMemorando})
                {$tmpTpDpsId_Where}
                {$tmpEmpresaId_Where}
                {$anoMemorando_Where}
                AND ans.ansgarantia = 'f'
			ORDER BY 
				os.scsid, os.odsid";
		//dbg($sql,1);
        $resultSet = parent::carregar( $sql );
        if ( $resultSet != null )
        {
            foreach ( $resultSet as $linha )
            {
                $lista[] = $this->montaObjeto( $linha );
            }
        }
        return $lista;
    }

    public function recuperaOrdemServicoItemUmPorIdSolicitacao( $idSolicitacao )
    {
        $sql   = "SELECT os.odsid, os.scsid, os.odsdetalhamento, os.odsdtprevinicio, os.odsdtprevtermino, os.docid, os.odsidpai,  
		os.odsqtdpfestimada, os.odsqtdpfdetalhada, os.odscontratada, os.docidpf, os.tosid, os.tpeid, os.odsqtdpffinal, os.odsenderecosvn, 
		os.odssubtotalpf, os.odssubtotalpfdetalhada, os.ctrid , os.memoid, os.glosaid, esdc.esdid
				FROM fabrica.ordemservico os
				INNER JOIN workflow.documento d
					ON os.docid = d.docid
				INNER JOIN workflow.estadodocumento esdc
					ON d.esdid = esdc.esdid WHERE os.scsid = $idSolicitacao AND os.tosid = 1";
        $linha = parent::pegaLinha( $sql );
        return $this->montaObjeto( $linha );
    }

    public function equals( $ordemServico )
    {
        return $ordemServico->getId() == $this->getId();
    }
    
    /**
     * Recupera as OS vinculadas h� um memorando da empresa do item 1
     * 
     * @param int $idMemorando
     * @return array
     */
    public function recuperarOSMemorando( $idMemorando )
    {
        
        $sql = "SELECT os.scsid, os.odsid, os.odsqtdpfdetalhada, os.tosid, odssubtotalpf, docos.esdid, os.glosaid, os.memoid, 
                            (SELECT sum(fadvalor)  
                                        FROM
                                            (SELECT
                                                distinct ans.scsid, ans.ansid,tpe.tpeid, tpe.tpedsc,dsp.dspid, dsp.dspdsc,fas.fasid, fas.fasdsc,fadvalor
                                            FROM
                                            fabrica.analisesolicitacao ans
                                            INNER JOIN fabrica.servicofaseproduto sfp on sfp.ansid=ans.ansid
                                            INNER JOIN fabrica.tipoexecucao tpe on tpe.tpeid=sfp.tpeid AND tpe.tpestatus = 'A' and tpe.tpeid = 1
                                            INNER JOIN fabrica.fasedisciplinaproduto fdp on fdp.fdpid=sfp.fdpid AND fdp.fdpstatus = 'A'
                                            INNER JOIN fabrica.produto prd on prd.prdid=fdp.prdid AND prd.prdstatus = 'A'
                                            INNER JOIN fabrica.fasedisciplina fsd on fsd.fsdid=fdp.fsdid
                                            INNER JOIN fabrica.disciplina dsp on dsp.dspid=fsd.dspid AND dsp.dspstatus = 'A'
                                            INNER JOIN fabrica.fase fas on fas.fasid=fsd.fasid AND fas.fasstatus = 'A'
                                            WHERE
                                                ans.scsid=ss.scsid 
                                            ORDER BY
                                                ans.scsid,ans.ansid,tpe.tpeid,dsp.dspid,fas.fasid) 
                                            artefatos) as porcentagem_disciplinas,

                            ( case when os.ctrid in (2,3) then (SELECT vpc.vpcvalor FROM fabrica.vw_valorpfcontrato vpc where vpc.odsid = os.odsid) else vim.vcmvalorcontratado end ) as valor_pf,
                            os.odsqtdpfdetalhada * (SELECT sum(fadvalor)  
                                        FROM
                                            (SELECT
                                                distinct ans.scsid, ans.ansid,tpe.tpeid, tpe.tpedsc,dsp.dspid, dsp.dspdsc,fas.fasid, fas.fasdsc,fadvalor
                                            FROM
                                            fabrica.analisesolicitacao ans
                                            INNER JOIN fabrica.servicofaseproduto sfp on sfp.ansid=ans.ansid
                                            INNER JOIN fabrica.tipoexecucao tpe on tpe.tpeid=sfp.tpeid AND tpe.tpestatus = 'A' and tpe.tpeid = 1 
                                            INNER JOIN fabrica.fasedisciplinaproduto fdp on fdp.fdpid=sfp.fdpid AND fdp.fdpstatus = 'A'
                                            INNER JOIN fabrica.produto prd on prd.prdid=fdp.prdid AND prd.prdstatus = 'A'
                                            INNER JOIN fabrica.fasedisciplina fsd on fsd.fsdid=fdp.fsdid
                                            INNER JOIN fabrica.disciplina dsp on dsp.dspid=fsd.dspid AND dsp.dspstatus = 'A'
                                            INNER JOIN fabrica.fase fas on fas.fasid=fsd.fasid AND fas.fasstatus = 'A'
                                            WHERE
                                                ans.scsid=ss.scsid 
                                            ORDER BY
                                                ans.scsid,ans.ansid,tpe.tpeid,dsp.dspid,fas.fasid) 
                                            artefatos) / 100 * (case when os.ctrid in (2,3) then (SELECT vpc.vpcvalor FROM fabrica.vw_valorpfcontrato vpc where vpc.odsid = os.odsid) else vim.vcmvalorcontratado end) as valor_a_receber       

                    FROM fabrica.ordemservico os
                    INNER JOIN workflow.documento docos
                        ON os.docid = docos.docid
                    INNER JOIN fabrica.solicitacaoservico ss
                        ON os.scsid = ss.scsid
                        AND ss.scsstatus = 'A'
                    INNER JOIN fabrica.analisesolicitacao ans
                        ON ss.scsid	= ans.scsid
                    INNER JOIN fabrica.memorando memo
                        ON memo.memoid = os.memoid
                    INNER JOIN fabrica.tipoglosamemorando tpglmemo
                        ON memo.tpglmemoid = tpglmemo.tpglmemoid

                    --RECUPERA NOVO VALOR_PF
                	LEFT JOIN fabrica.vigenciacontratometricaitem vim ON vim.vgcid = ans.vgcid and ans.mtiid = vim.mtiid
                        
                    WHERE memo.memoid = {$idMemorando}";
        
        $resultSet = parent::carregar( $sql );
        $lista      = array();
        if ( $resultSet != null )
        {
            foreach ( $resultSet as $linha )
            {
                $lista[] = $this->montaObjeto( $linha );
            }
        }
        return $lista;
        
    }
    
    /**
     * Recupera as OS vinculadas h� um memorando da empresa do item 2
     * @param int $idMemorando
     * @return array
     */
    public function recuperarOSMemorandoEmpresaItem2( $idMemorando )
    {
        
        $sql = "SELECT os.scsid, os.odsid, os.odsqtdpfdetalhada, os.tosid, odssubtotalpf, docos.esdid, os.glosaid, os.memoid, 
                            (SELECT sum(fadvalor)  
                                        FROM
                                            (SELECT
                                                distinct ans.scsid, ans.ansid,tpe.tpeid, tpe.tpedsc,dsp.dspid, dsp.dspdsc,fas.fasid, fas.fasdsc,fadvalor
                                            FROM
                                            fabrica.analisesolicitacao ans
                                            INNER JOIN fabrica.servicofaseproduto sfp on sfp.ansid=ans.ansid
                                            INNER JOIN fabrica.tipoexecucao tpe on tpe.tpeid=sfp.tpeid AND tpe.tpestatus = 'A' and tpe.tpeid = 1
                                            INNER JOIN fabrica.fasedisciplinaproduto fdp on fdp.fdpid=sfp.fdpid AND fdp.fdpstatus = 'A'
                                            INNER JOIN fabrica.produto prd on prd.prdid=fdp.prdid AND prd.prdstatus = 'A'
                                            INNER JOIN fabrica.fasedisciplina fsd on fsd.fsdid=fdp.fsdid
                                            INNER JOIN fabrica.disciplina dsp on dsp.dspid=fsd.dspid AND dsp.dspstatus = 'A'
                                            INNER JOIN fabrica.fase fas on fas.fasid=fsd.fasid AND fas.fasstatus = 'A'
                                            WHERE
                                                ans.scsid=ss.scsid 
                                            ORDER BY
                                                ans.scsid,ans.ansid,tpe.tpeid,dsp.dspid,fas.fasid) 
                                            artefatos) as porcentagem_disciplinas,

                            ( case when os.ctrid in (2,3) then (SELECT vpc.vpcvalor FROM fabrica.vw_valorpfcontrato vpc where vpc.odsid = os.odsid) else vim.vcmvalorcontratado end ) as valor_pf,
                            os.odsqtdpfdetalhada * (SELECT sum(fadvalor)  
                                        FROM
                                            (SELECT
                                                distinct ans.scsid, ans.ansid,tpe.tpeid, tpe.tpedsc,dsp.dspid, dsp.dspdsc,fas.fasid, fas.fasdsc,fadvalor
                                            FROM
                                            fabrica.analisesolicitacao ans
                                            INNER JOIN fabrica.servicofaseproduto sfp on sfp.ansid=ans.ansid
                                            INNER JOIN fabrica.tipoexecucao tpe on tpe.tpeid=sfp.tpeid AND tpe.tpestatus = 'A' and tpe.tpeid = 1 
                                            INNER JOIN fabrica.fasedisciplinaproduto fdp on fdp.fdpid=sfp.fdpid AND fdp.fdpstatus = 'A'
                                            INNER JOIN fabrica.produto prd on prd.prdid=fdp.prdid AND prd.prdstatus = 'A'
                                            INNER JOIN fabrica.fasedisciplina fsd on fsd.fsdid=fdp.fsdid
                                            INNER JOIN fabrica.disciplina dsp on dsp.dspid=fsd.dspid AND dsp.dspstatus = 'A'
                                            INNER JOIN fabrica.fase fas on fas.fasid=fsd.fasid AND fas.fasstatus = 'A'
                                            WHERE
                                                ans.scsid=ss.scsid 
                                            ORDER BY
                                                ans.scsid,ans.ansid,tpe.tpeid,dsp.dspid,fas.fasid) 
                                            artefatos) / 100 * (case when os.ctrid in (2,3) then (SELECT vpc.vpcvalor FROM fabrica.vw_valorpfcontrato vpc where vpc.odsid = os.odsid) else vim.vcmvalorcontratado end) as valor_a_receber       

                    FROM fabrica.ordemservico os
                    INNER JOIN workflow.documento docos
                        ON os.docidpf = docos.docid
                    INNER JOIN fabrica.solicitacaoservico ss
                        ON os.scsid = ss.scsid
                        AND ss.scsstatus = 'A'
                    INNER JOIN fabrica.analisesolicitacao ans
                        ON ss.scsid	= ans.scsid
                    INNER JOIN fabrica.memorando memo
                        ON memo.memoid = os.memoid
                    INNER JOIN fabrica.tipoglosamemorando tpglmemo
                        ON memo.tpglmemoid = tpglmemo.tpglmemoid
                    
                    --RECUPERA NOVO VALOR_PF
                	LEFT JOIN fabrica.vigenciacontratometricaitem vim ON vim.vgcid = ans.vgcid and ans.mtiid = vim.mtiid
                        
                    WHERE memo.memoid = {$idMemorando}";
        
        $resultSet = parent::carregar( $sql );
        $lista      = array();
        if ( $resultSet != null )
        {
            foreach ( $resultSet as $linha )
            {
                $lista[] = $this->montaObjeto( $linha );
            }
        }
        return $lista;
        
    }
    
    /**
     * Retorna a OS do tipo DETALHADA vinculada h� uma 
     * os do tipo GERAL
     * @param int $odsid
     * @return OrdemServico
     */
    public function recuperarOSDetalhadaVinculada( $odsid )
    {
        $sql    = "SELECT * 
                    FROM fabrica.ordemservico
                    WHERE odsidpai = {$odsid}
                    and tosid = ". TIPO_OS_CONTAGEM_DETALHADA;
                    
        $linha = parent::pegaLinha( $sql );
        return $this->montaObjeto( $linha );
    }
    
    
    /**
     * Retorna a data de entrada do estado em que 
     * a OS se encontra atualmente.
     * 
     * @param int $odsId 
     * @return DateTime $dataEntrada
     */
    public function dataEntradaEstado( $odsId, $esdId )
    {
        
        
        $sql = "SELECT os.odsid
                        , CASE
                            WHEN os.docid IS NOT NULL THEN hd.htddata
                            WHEN os.docidpf IS NOT NULL THEN hdpf.htddata	
                        END AS htddata
                        , CASE
                            WHEN os.docid IS NOT NULL THEN doc.esdid
                            WHEN os.docidpf IS NOT NULL THEN docpf.esdid 	
                        END AS esdidteste                         
                        , CASE
                            WHEN os.docid IS NOT NULL THEN doc.docdatainclusao
                            WHEN os.docidpf IS NOT NULL THEN docpf.docdatainclusao 	
                        END AS docdatainclusao
                FROM fabrica.ordemservico os

                LEFT JOIN workflow.documento doc
                    ON os.docid = doc.docid                    
                LEFT JOIN workflow.documento docpf
                    ON os.docidpf = docpf.docid                        

                LEFT JOIN workflow.historicodocumento hd
                    ON hd.docid	= doc.docid 
                LEFT JOIN workflow.historicodocumento hdpf
                    ON hdpf.docid = docpf.docid                                 
        
                LEFT JOIN workflow.acaoestadodoc aed
		    ON hd.aedid = aed.aedid
		LEFT JOIN workflow.acaoestadodoc aedpf
		    ON hdpf.aedid = aedpf.aedid

                WHERE os.odsid          = {$odsId}                
                AND (aed.esdiddestino = {$esdId} OR aedpf.esdiddestino =  {$esdId})
                ORDER BY hd.htddata desc, hdpf.htddata desc
                LIMIT 1";               
                
                
       $dados                   = parent::pegaLinha($sql);
       $estadosSemTramitacao    = array( WF_ESTADO_OS_PENDENTE, WF_ESTADO_CPF_PENDENTE  );
       
       if(  in_array( $esdId, $estadosSemTramitacao ) )
       {
           $dados['htddata'] = $dados['docdatainclusao'];
       }
       
       return new DateTime( $dados['htddata'] );
       
    }
    
	public function mostraListaDeDivergencias($ctrid, $vgcid) {
    	$sql = "SELECT
			    	'<a href=\"fabrica.php?modulo=sistema/geral/memorando/visualizar&acao=A&memo=' || mm.memoid || '\" style=\"color: #0066cc;\">'
			    	|| ( mm.memonumero || '/' || to_char(mm.memodata, 'YYYY') ) || '</a>' AS memonumero,
			    	ss.scsid,
			    	os.odsid,
			    	to_char(os.odsdtprevinicio, 'DD/MM/YYYY') AS previnicio,
			    	to_char(os.odsdtprevtermino, 'DD/MM/YYYY') AS prevtermino,
			    	mmr.pagaraposglosa,
			    	mmr.vlpfunitario,
			    	mmr.vltotal,
			    	CASE WHEN rc.rpcvalor IS NOT NULL THEN rc.rpcvalor ELSE vc.vgcvalorpfcontrato END AS novoValorPF,
			    	( (CASE WHEN rc.rpcvalor IS NOT NULL THEN rc.rpcvalor ELSE vc.vgcvalorpfcontrato END) * mmr.pagaraposglosa ) AS novoValorTotal,
			    	to_char( ( ( (CASE WHEN rc.rpcvalor IS NOT NULL THEN rc.rpcvalor ELSE vc.vgcvalorpfcontrato END) * mmr.pagaraposglosa ) - mmr.vltotal ), '999G999G990D99') AS diferenca,
			    	CASE WHEN rc.rpcvalor IS NOT NULL THEN 'sim' ELSE ' - ' END AS repactuacao
		    	FROM
		    		fabrica.ordemservico os
		    	INNER JOIN fabrica.solicitacaoservico ss
		    		ON os.scsid=ss.scsid
		    	INNER JOIN fabrica.memorando mm
		    		ON os.memoid=mm.memoid
		    	INNER JOIN fabrica.memorandorelatorio mmr
		    		ON os.memoid=mmr.memoid AND os.ctrid=mmr.ctrid AND ss.scsid=mmr.scsid AND os.odsid=mmr.odsid
		    	INNER JOIN fabrica.vigenciacontrato vc
		    		ON os.ctrid=vc.ctrid AND vc.vgcid=$vgcid
		    	LEFT JOIN fabrica.repactuacaocontrato rc
		    		ON os.ctrid=rc.ctrid AND rc.vgcid=vc.vgcid
		    	WHERE
		    		os.ctrid=$ctrid
		    		AND os.odsdtprevinicio >= vc.vgcdtinicio
		    		AND os.odsdtprevinicio <= vc.vgcdtfim
    		";

    	return $this->carregar($sql);
    }
}