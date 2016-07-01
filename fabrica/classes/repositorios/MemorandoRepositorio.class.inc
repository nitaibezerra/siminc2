<?php

class MemorandoRepositorio extends ModeloFabrica
{
    const NAO_EMITIDO   = 'NIMP';
    const EMITIDO       = 'IMPR';
    

    private $fiscalRepositorio;
    private $ordemServicoRepositorio;
    protected $stNomeTabela = "fabrica.memorando";
    protected $arChavePrimaria = array( "memoid" );
    protected $arAtributos = array(
        'memoid'                     => null,
        'memocpfservidorresponsavel' => null,
        'memonumero'                 => null,
        'memodata'                   => null,
        'memoidprestadorservico'     => null,
        'memotexto'                  => null,
        'memostatus'                 => null,
        'tpglmemoid'                 => null,
        'memojustificativaglosa'     => null,
    	'memodscajuste'     		 => null,
    	'memovlrajuste'     		 => null,
    	'tpdpsid'					 => null
    	
    );
    protected $stCampos = "memoid, memocpfservidorresponsavel, memonumero, memodata, memoidprestadorservico, memotexto, memostatus";

    public function __construct()
    {
        parent::__construct();
        $this->fiscalRepositorio = new FiscalRepositorio();
        $this->ordemServicoRepositorio = new OrdemServico();
    }

    public function verificaSeMemorandoExisteNaBase( $numeroMemorando )
    {
        $sql       = "select * from fabrica.memorando where memonumero = $numeroMemorando";
        $linha     = parent::pegaLinha( $sql );
        $memorando = $this->montaObjetoSemListaDeOrdemServico( $linha );
        return $memorando->getId() != "";
    }

    public function recupereTodos()
    {
        $sql       = "select * from fabrica.memorando";
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

    /**
     * Busca um memorando pelo seu identificador
     * @param int $idMemorando
     * @return Memorando|null
     */
    public function recuperePorId( $idMemorando )
    {
        $sql = "SELECT memo.memoid,
                       memo.memocpfservidorresponsavel,
                       memo.memonumero,
                       memo.memodata,
                       memo.memoidprestadorservico,
                       memo.memotexto,
                       memo.memostatus,
                       memo.memojustificativaglosa,
                       memo.memodscajuste,
                       memo.memovlrajuste,
                       tpglmemo.tpglmemoid,
                       tpglmemo.tpglmemodsc,
                       tpglmemo.tpglmemopercvalor,
                       tpglmemo.tpglmemoaplicacaomes,
                       tpglmemo.tpglmemograuvalor,
                       tpglmemo.tpglmemostatus,
                       memo.tpdpsid                      
                FROM fabrica.memorando memo
                LEFT JOIN fabrica.tipoglosamemorando tpglmemo
                    ON memo.tpglmemoid = tpglmemo.tpglmemoid
                WHERE memo.memoid = $idMemorando";

        try
        {
            $linha     = parent::pegaLinha( $sql );
        	
        	if($linha !='' && !is_null($linha) )
            {
	           $memorando = $this->montaObjeto( $linha );
	           return $memorando;
            } else {
            	return null;
            }
        } catch ( Exception $e )
        {
            return null;
        }
    }

    public function recuperePeloNumeroMemorando( $numeroMemorando )
    {
        $sql = "select * from fabrica.memorando where memonumero = $numeroMemorando";
        
        try
        {
            $linha     = parent::pegaLinha( $sql );

            if($linha !='' && !is_null($linha) )
            {
	           $memorando = $this->montaObjeto( $linha );
	           return $memorando;
            } else {
            	return null;
            }
            
        } catch ( Exception $e )
        {
            return null;
        }
    }

    public function recuperePorTipoMemorando( $status )
    {
        $sql       = "select * from fabrica.memorando where memostatus = '$status'";
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
    
    public function recuperaMemoPorFiltro( $perfis, $memoStatus = null, $tpDespesa = null, $memoAno = null )
    {

    	$acoes = "";
    	$acaoVisualizar = " <a href=\"?modulo=sistema/geral/memorando/visualizar&acao=A&memo='|| memo.memoid ||'\">
    							<img id=\"visualizar-'|| memo.memoid ||'\" class=\"botao-visualizar botoes-tabela\"
    							 title=\"Visualizar Memorando\" src=\"../imagens/consultar.gif\"/> </a> ";
    	
    	if( in_array(PERFIL_SUPER_USUARIO, $perfis) 
    			|| in_array(PERFIL_FISCAL_CONTRATO, $perfis) 
    			|| ( !in_array( PERFIL_ESPECIALISTA_SQUADRA, $perfis ) && !in_array( PERFIL_PREPOSTO, $perfis ) ) ){
    		
    		$acoes = " CASE WHEN memo.memostatus = '" .StatusMemorando::MEMORANDO_NAO_IMPRESSO. "'";
    		$acoes.= " 	THEN '<a href=\"?modulo=sistema/geral/memorando/formulario&acao=A&memo='|| memo.memoid ||'&formmemotpdpsid='|| coalesce(memo.tpdpsid::varchar,'') ||'\">";
    		$acoes.= "			 <img style=\"border: none;\" src=\"/imagens/editar_nome.gif\" alt=\"Editar Memorando\" title=\"Editar Memorando\"/></a>";
    		$acoes.= "			 <img id=\"'|| memo.memoid ||'\" src=\"/imagens/excluir.gif\" alt=\"Excluir Memorando\" title=\"Excluir Memorando\" class=\"botao-excluir-memorando botoes-tabela\" /> ";
    		$acoes.= $acaoVisualizar . "' ";
    		$acoes.= "	ELSE ";
    		$acoes.= "		'" . $acaoVisualizar . "' ";
    		$acoes.= "	END ";
    		
    	}else{
    		$acoes.= "'" . $acaoVisualizar . "'";
    	}

    	$acoes.=" AS acoes, ";
    	
    	$sql  = "SELECT ";
    	$sql .= $acoes;
    	$sql .= " 	CASE WHEN memo.memostatus = '" .StatusMemorando::MEMORANDO_NAO_IMPRESSO. "'";
    	$sql .= " 		THEN 'Não emitido' ";
    	$sql .= " 		ELSE 'Emitido' ";
    	$sql .= " 	END AS status, ";
    	$sql .= " 	ent.entnome, ";
    	$sql .= " 	memo.memonumero, ";
    	$sql .= " 	responsavel.usunome, ";
    	$sql .= " 	to_char(memo.memodata, 'DD/MM/YYYY') ";
    	$sql .= " FROM fabrica.memorando AS memo ";
    	$sql .= " INNER JOIN entidade.entidade ent";
    	$sql .= " ON memo.memoidprestadorservico = ent.entid";
    	$sql .= " INNER JOIN (	SELECT DISTINCT u.usucpf,";
		$sql .= "					u.usunome, u.usuemail, u.usufuncao, u.ususexo";
		$sql .= "				FROM fabrica.fiscalcontrato f ";
		$sql .= " 				INNER JOIN seguranca.usuario u ON u.usucpf = f.usucpf ) AS responsavel ";
		$sql .= "				ON  responsavel.usucpf = memo.memocpfservidorresponsavel ";
    	$sql .= " WHERE memo.memoid > 0 ";
    	$sql .= empty($memoStatus) ? "" : " AND memo.memostatus  = '" . $memoStatus . "'";
    	$sql .= empty($tpDespesa)  ? "" : " AND memo.tpdpsid 	= " . $tpDespesa ;
    	$sql .= empty($memoAno)    ? "" : " AND to_char(memo.memodata, 'YYYY') = '" . $memoAno . "'";

    	return $sql;
    }
    
    public function pegarAtributos()
    {
    	return implode( ',' , array_keys( $this->arAtributos ) );
    }
    
    public function recupereMemorandoEmitidoGlosadoPorPrestadorServico( $idPrestadorServico )
    {
        $sql       = "SELECT * 
                      FROM fabrica.memorando 
                      WHERE memostatus = '". MemorandoRepositorio::EMITIDO ."'
                      AND tpglmemoid IS NOT NULL
                      AND memoidprestadorservico = ". $idPrestadorServico;
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

    public function montaObjeto( $linha )
    {
        $memorando = new Memorando();
        $fiscal    = $this->fiscalRepositorio->recuperePorId( $linha['memocpfservidorresponsavel'] );
        $memorando->setFiscal( $fiscal );

        $memorando->setPrestadorServico( $linha['memoidprestadorservico'] );
        
        if ( $linha['memoidprestadorservico'] == PrestadorServico::PRESTADORA_SERVICO_AUDITORA || $linha['memoidprestadorservico'] == PrestadorServico::PRESTADORA_SERVICO_SAA )
        {
            $listaDeOrdemDeServico = $this->ordemServicoRepositorio->recupereOSQueEstaoEmMemorandoDaAUDITORA( $linha['memoid'] );
            $memorando->setListaDeOrdensDeServico( $listaDeOrdemDeServico );
        } else
        {
            $listaDeOrdemDeServico = $this->ordemServicoRepositorio->recupereOSQueEstaoEmMemorandoDaFABRICA( $linha['memoid'] );
            $memorando->setListaDeOrdensDeServico( $listaDeOrdemDeServico );
        }

        $memorando->setDataMemorando( DateTimeUtil::retiraMascaraRetornandoObjetoDateTime( $linha['memodata'] ) );
        $memorando->setId( $linha['memoid'] );
        $memorando->setNumeroMemorando( $linha['memonumero'] );
        $memorando->setTextoMemorando( $linha['memotexto'] );
        $memorando->setStatusMemorando( $linha['memostatus'] );
        $memorando->setGlosaMemorando( $linha['tpglmemoid'] );
        $memorando->setJustificativaGlosaMemorando( $linha['memojustificativaglosa'] );
        $memorando->setDescricaoAjuste( $linha['memodscajuste'] );
        $memorando->setValorAjuste( $linha['memovlrajuste'] );
        $memorando->setTipoDespesaId( $linha['tpdpsid'] );
        $memorando->getPrestadorServicoMemorando( $linha['memoidprestadorservico'] );
        
        return $memorando;
    }

    public function montaObjetoSemListaDeOrdemServico( $linha )
    {
        $memorando = new Memorando();
        $fiscal    = $this->fiscalRepositorio->recuperePorId( $linha['memocpfservidorresponsavel'] );
        $memorando->setFiscal( $fiscal );
        $memorando->setDataMemorando( DateTimeUtil::retiraMascaraRetornandoObjetoDateTime( $linha['memodata'] ) );
        $memorando->setId( $linha['memoid'] );
        $memorando->setNumeroMemorando( $linha['memonumero'] );
        $memorando->setPrestadorServico( $linha['memoidprestadorservico'] );
        $memorando->setTextoMemorando( $linha['memotexto'] );
        $memorando->setStatusMemorando( $linha['memostatus'] );
        $memorando->setGlosaMemorando( $linha['tpglmemoid'] );
        $memorando->setJustificativaGlosaMemorando( $linha['memojustificativaglosa'] );
        $memorando->setDescricaoAjuste( $linha['memodscajuste'] );
        $memorando->setValorAjuste( $linha['memovlrajuste'] );
        return $memorando;
    }

    public function salvar( $memorando )
    {
    	//var_dump( $memorando->getListaDeOrdensDeServico() ); exit;
    	
        if ( $memorando->getId() == '' || $memorando->getId() == null )
        {
            $this->arAtributos['memocpfservidorresponsavel'] = $memorando->getFiscal()->getId();
            $this->arAtributos['memonumero'] 				 = $memorando->getNumeroMemorando();
            $this->arAtributos['memodata'] 					 = $memorando->getDataMemorando()->format( "Y-m-d" );
            $this->arAtributos['memoidprestadorservico'] 	 = $memorando->getPrestadorServicoMemorando();
            $this->arAtributos['memotexto'] 				 = $memorando->getTextoMemorando();
            $this->arAtributos['memostatus'] 				 = $memorando->getStatusMemorando();
            $this->arAtributos['memostatus'] 				 = $memorando->getStatusMemorando();
            $this->arAtributos['tpglmemoid'] 				 = $memorando->getGlosaMemorando();
            $this->arAtributos['memojustificativaglosa'] 	 = $memorando->getJustificativaGlosaMemorando();
            $this->arAtributos['memodscajuste']  			 = $memorando->getDescricaoAjuste( );
            $this->arAtributos['memovlrajuste']  			 = $memorando->getValorAjuste( );
            
            if( $memorando->getTipoDespesaId() != "" ) {
            	$this->arAtributos['tpdpsid'] = $memorando->getTipoDespesaId();
            }

            $id = parent::salvar();
            parent::commit();

            $listaDeOs       = $memorando->getListaDeOrdensDeServico();
            foreach ( $listaDeOs as $os )
            {
                $this->atribuirMemorandoAOrdensDeServico( $id, $os->getId() );
            }
        } else
        {
            
            $cpfServidorResponsavel = $memorando->getFiscal()->getId();
            $numeroMemorando        = $memorando->getNumeroMemorando();
            $dataMemorando          = $memorando->getDataMemorando()->format( "Y-m-d" );
            $idPrestadorServico     = $memorando->getPrestadorServicoMemorando();
            $textoMemorando         = $memorando->getTextoMemorando();
            $statusMemorando        = $memorando->getStatusMemorando();
            $id                     = $memorando->getId();
            $memodscajuste  		= $memorando->getDescricaoAjuste( );
            $memovlrajuste			= $memorando->getValorAjuste( );
            $tpdpsid  			 	= $memorando->getTipoDespesaId( );
            
            $arrUPDATE              = array();
            $arrUPDATE[]            = "memocpfservidorresponsavel = '$cpfServidorResponsavel'";
            $arrUPDATE[]            = "memonumero = $numeroMemorando";
            $arrUPDATE[]            = "memodata = '$dataMemorando'";
            $arrUPDATE[]            = "memoidprestadorservico = $idPrestadorServico";
            $arrUPDATE[]            = "memotexto = '$textoMemorando'";
            $arrUPDATE[]            = "memostatus 		= '$statusMemorando'";
            $arrUPDATE[]			= "memodscajuste 	= '$memodscajuste'";
            
            $glosaMemorando         = $memorando->getGlosaMemorando();
            $justificativaGlosa     = $memorando->getJustificativaGlosaMemorando();
            
            if(!empty($glosaMemorando))
            {
                $arrUPDATE[]            = "tpglmemoid = $glosaMemorando";
                $arrUPDATE[]            = "memojustificativaglosa = '$justificativaGlosa'";
            }else {
                $arrUPDATE[]            = "tpglmemoid = NULL";
                $arrUPDATE[]            = "memojustificativaglosa = NULL";
            }
            
            if(!empty($tpdpsid))
            {
            	$arrUPDATE[]            = "tpdpsid = $tpdpsid";
            }else {
            	$arrUPDATE[]            = "tpdpsid = NULL";
            }
            
            if(!empty($memovlrajuste))
            {
            	$arrUPDATE[]            = "memovlrajuste = $memovlrajuste";
            }else {
            	$arrUPDATE[]            = "memovlrajuste = NULL";
            }
            
            $updateMemorando   = implode( ', ', $arrUPDATE );
            
            $sql = "UPDATE fabrica.memorando 
                                SET {$updateMemorando}
                                WHERE memoid = $id returning memoid";
                                
            if ( parent::executar( $sql ) && parent::commit() )
            {
                $sql       = "UPDATE fabrica.ordemservico set memoid = null where memoid = $id ";
                
                if ( parent::executar( $sql ) && parent::commit() )
                {
                	$sql = "DELETE FROM fabrica.memorandogerado WHERE memoid = $id";
                	parent::executar( $sql ) && parent::commit();
                }
            }
            $listaDeOs = $memorando->getListaDeOrdensDeServico();

            if ( $listaDeOs != null )
            {
                foreach ( $listaDeOs as $os )
                {
                    $this->atribuirMemorandoAOrdensDeServico( $id, $os->getId() );
                }
            }
        }
        return $id;
    }

    private function atribuirMemorandoAOrdensDeServico( $novoIdMemorando, $idOrdemServico )
    {
        $sql = "UPDATE fabrica.ordemservico SET memoid = '$novoIdMemorando' WHERE odsid = $idOrdemServico";
        parent::executar( $sql );
        parent::commit();
    }

    public function removeMemorando( $idMemorando )
    {
        $sql = "UPDATE fabrica.ordemservico SET memoid = null WHERE memoid = $idMemorando";
        if ( parent::executar( $sql ) && parent::commit() )
        {
        	$sql = "DELETE FROM fabrica.memorandogerado WHERE memoid = $idMemorando";
        	
        	if ( parent::executar( $sql ) && parent::commit() )
        	{
        		$sql = "DELETE FROM fabrica.memorandorelatorio WHERE memoid = $idMemorando";
        		
	        	if ( parent::executar( $sql ) && parent::commit() )
	        	{
		            $sql = "DELETE FROM fabrica.memorando WHERE memoid = $idMemorando";
		            return parent::executar( $sql ) && parent::commit();
	        	}
        	}
        }
    }

    public function getDadosGlosaMemorando( $idGlosaMemorando )
    {
        $sql = "SELECT tpglmemograuvalor, tpglmemopercvalor 
                FROM fabrica.tipoglosamemorando
                WHERE tpglmemoid = $idGlosaMemorando";

        return parent::pegaLinha( $sql );
    }
    
    /**
     * Retorna o valor de glosa aplicado em um memorando
     * 
     * @param int $idMemorando 
     * @return float valorGlosa
     */
    public function recuperarValorGlosaMemorando( $idMemorando )
    {
        $ordemServico       = new OrdemServico();
        $memorando          = $this->recuperePorId( $idMemorando );
        $listaOSMemorando   = $ordemServico->recuperarOSMemorando($idMemorando);
        $dadosGlosaMemorando   = '';

        if( !$memorando->possuiGlosa()  ){
            return 0;
        }
        
        $dadosGlosaMemorando = $this->getDadosGlosaMemorando( $memorando->getGlosaMemorando() );
        
        $valorTotalMemorando    = 0;
        foreach ( $listaOSMemorando as $os )
        {
             if($os->possuiGlosa()){
                 $valorTotalAReceber = $os->getValorAReceberGlosado();
             } else {
                 $valorTotalAReceber = $os->getValorAReceberDaOs();
             }

            $valorTotalMemorando += $valorTotalAReceber;
        }
        $glosaMemorando = 0;
        $glosaMemorando = $dadosGlosaMemorando['tpglmemopercvalor'] * ( $valorTotalMemorando / 100 );

        return round( $glosaMemorando, 2 );
    }
    
    /**
     * Retorna o valor de glosa aplicado em um memorando
     * 
     * @param int $idMemorando 
     * @return float valorGlosa
     */
    public function recuperarValorGlosaMemorandoEmpresaItem2( $idMemorando )
    {
        $ordemServico       = new OrdemServico();
        $memorando          = $this->recuperePorId( $idMemorando );
        $listaOSMemorando   = $ordemServico->recuperarOSMemorandoEmpresaItem2($idMemorando);
        $dadosGlosaMemorando   = '';

        if( !$memorando->possuiGlosa()  ){
            return 0;
        }
        
        $dadosGlosaMemorando = $this->getDadosGlosaMemorando( $memorando->getGlosaMemorando() );
        
        $valorTotalMemorando    = 0;
        foreach ( $listaOSMemorando as $os )
        {
            $qtdPFGlosa                 = 0;
            $valorGlosa                 = 0;

            $menorValorPF               = $os->getMenorValorPFEmpresaItem2( $os->getId());
            $valorUnitarioPF            = $os->getValorUnitarioDePf();
            $valorTotalAReceber         = $valorUnitarioPF * $menorValorPF;
            $subTotalQtdePontoFuncao    = $subTotalQtdePontoFuncao + $menorValorPF;

            if ( $os->possuiGlosa() ) {
                $glosa      = new Glosa();
                $glosa      = $glosa->recupereGlosaPeloId( $os->getIdGlosa() );
                $qtdPFGlosa = $glosa->getValorEmPf();
                $valorGlosa = $qtdPFGlosa * $valorUnitarioPF;
            }

            $valorTotalAReceber  -= $valorGlosa;
            $valorTotalMemorando += $valorTotalAReceber;
        }
        $glosaMemorando = 0;
        $glosaMemorando = $dadosGlosaMemorando['tpglmemopercvalor'] * ( $valorTotalMemorando / 100 );

        return round( $glosaMemorando, 2 );
    }
    
    
    

}