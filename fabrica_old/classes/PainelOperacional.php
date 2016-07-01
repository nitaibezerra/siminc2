<?php

/**
 * Representa o painel operacional 
 * Apresenta informações de OS/SS
 * 
 * @author Arthur Almeida <arthur.almeida@squadra.com.br>
 */
class PainelOperacional
{

    /**
     *
     * @var cls_banco 
     */
    protected $_db;

    public function __construct( cls_banco $db ) {
        $this->_db = $db;
    }
    
    /**
     * Retorna as solicitacões de serviço 
     * Realiza o filtro por um determinado estado
     * 
     * @param int $esdId 
     * @return string
     */
    public function listarSolicitacaoServicoPorEstado( $esdId )
    {
        $retorno = '';
        
        switch( $esdId )
        {
            case WF_ESTADO_ANALISE:
                $retorno = $this->listarSolicitacaoEmAnalise();
                break;
            case WF_ESTADO_AVALIACAO:
            case WF_ESTADO_APROVACAO:
                $retorno = $this->listarSolicitacaoEmAprovacaoAvaliacao( $esdId );
                break;
            case WF_ESTADO_SS_PAUSA:
                $retorno = $this->listarSolicitacaoEmPausa();
                break;
            default:
                throw new Exception('Estado de SS informado inválido');
        }
        
        return $retorno;
        
    }
    
    public function painelGerenteProjetos( $dados )
    {
        $retorno = '';
        $esdId = $dados["esdId"];
        
        switch( $esdId )
        {
            case WF_ESTADO_PRE_ANALISE:
                $retorno = $this->listarRealizarPreAnalise($dados);
                break;
             case WF_ESTADO_APROVACAO:
                $retorno = $this->listarRealizarAprovarExecucaodeServico($dados);
                break;
            case WF_ESTADO_OS_APROVACAO:
                $retorno = $this->listarHomologarOrdemDeServico($dados);
                break;
            case WF_ESTADO_OS_EXECUCAO:
                $retorno = $this->listarOrdemDeServicoExecucao($dados);
                break;
            case WF_ESTADO_OS_PAUSA:
                $retorno = $this->listarOrdemDeServicoEmPausa($dados);
                break;
            case WF_ESTADO_DETALHAMENTO:
                $retorno = $this->listarSsEmDetalhamento($dados);
                break;
            default:
                throw new Exception('Erro ao registrar serviço.');
        }
        
        return $retorno;
        
    }
    
   
    /**
     * Retorna as ordens de serviço 
     * Realiza o filtro por um determinado estado e seu tipo
     * 
     * @param int $esdId 
     * @param int $tosId - optional
     * @return string
     */
    public function listarOrdemServicoPorEstadoTipo( $esdId, $tosId = null )
    {
        $retorno = '';

        switch( $esdId )
        {
            case WF_ESTADO_OS_VERIFICACAO:
                $retorno = $this->listarOrdemServicoEmAvaliacao();
                break;
            case WF_ESTADO_OS_APROVACAO:
               // exit("aqui");
                $retorno = $this->listarOrdemServicoEmAprovacao();
                break;
            case WF_ESTADO_OS_ATESTO_TECNICO:
                $retorno = $this->listarOrdemServicoEmHomologacao();
                break;
            case WF_ESTADO_OS_AGUARDANDO_PAGAMENTO:
                $retorno = $this->listarOrdemServicoEmPagamento();
                break;
            case WF_ESTADO_OS_PAUSA:
                $retorno = $this->listarOrdemServicoEmPausa();
                break;
            
            case WF_ESTADO_CPF_APROVACAO:
                if( $tosId == TIPO_OS_CONTAGEM_ESTIMADA )
                    $retorno = $this->listarOrdemServicoEstimadaEmAprovacao( $tosId );
                else if( $tosId == TIPO_OS_CONTAGEM_DETALHADA )
                    $retorno = $this->listarOrdemServicoDetalhadaEmAprovacao( $tosId );
                break;
                
            case WF_ESTADO_CPF_AGUARDANDO_PAGAMENTO:
                $retorno = $this->listarOrdemServicoCPFEmPagamento();
                break;
            
            case WF_ESTADO_CPF_DIVERGENCIA:
                $retorno = $this->listarOrdemServicoCPFEmDivergencia();
                break;
            
            default:
                throw new Exception('Estado de OS informado inválido');
        }
        
        return $retorno;
        
    }

    /**
     * Lista as ordem de serviço
     * @return string
     */
    protected function listarOrdemServicoEmAvaliacao()
    {
         
         $sqlOSEmAvaliacao = "SELECT os.scsid, os.odsid, sid.sidabrev
                                , CASE
                                    WHEN ans.mensuravel IS NULL THEN 'Não informado'
                                    WHEN ans.mensuravel = 't' THEN 'Sim'
                                    WHEN ans.mensuravel = 'f' THEN 'Não'
                                  END as mensuravel                                
                                  
                                , to_char(  fabrica.fn_data_previsao_termino_os(os.odsid), 'DD/MM/YYYY' ) as odsdtprevtermino
                                , to_char(entrega.data_entrega, 'DD/MM/YYYY' ) as data_entrega
                                , CASE entrega.data_entrega <= fabrica.fn_data_previsao_termino_os(os.odsid)
                                    WHEN TRUE THEN 'OS EM DIA'
                                    ELSE '<span style=\" color:red; \">OS EM ATRASO</span>'
                                  END as status
                                , os.pf_detalhado as pf_detalhado_1
                                , CASE 
                                    WHEN osdet.esdid = ". WF_ESTADO_CPF_AGUARDANDO_CONTAGEM ." THEN osdet.esddsc
                                    WHEN osdet.esdid IS NULL  THEN '-'
                                    ELSE osdet.pf_detalhado::text
                                  END as pf_detalhado_2
                                , CASE 
                                    WHEN osdet.esdid = ". WF_ESTADO_CPF_AGUARDANDO_CONTAGEM ." THEN
                                        '-'
                                    ELSE
                                        '<a id=\"'|| os.odsid ||'\" class=\"link generica\" title=\"Abrir Ordem de Serviço\" >Avaliar O.S.</a>'
                                  END
                                
                              FROM fabrica.vw_ordem_servico os
                              INNER JOIN fabrica.solicitacaoservico ss
                                ON os.scsid = ss.scsid
                              LEFT JOIN fabrica.analisesolicitacao ans
                                ON ss.scsid = ans.scsid
                              LEFT JOIN demandas.sistemadetalhe sid
                                ON ss.sidid = sid.sidid
                              LEFT JOIN fabrica.fn_entrega_ordem_servico() entrega
                                ON os.odsid = entrega.odsid
                              LEFT JOIN fabrica.vw_ordem_servico osdet
                                ON os.odsid = osdet.odsidpai
                                AND osdet.tosid = ".TIPO_OS_CONTAGEM_DETALHADA ."
                                AND osdet.esdid NOT IN ( ". WF_ESTADO_CPF_CANCELADA .", ". WF_ESTADO_CPF_PENDENTE ." )
                              WHERE os.esdid = ". WF_ESTADO_OS_VERIFICACAO . "
                              AND os.tosid 	= ". TIPO_OS_GERAL ;
        
        //ver($sqlOSEmAvaliacao);
        $cabecalho     = array("SS","OS","Sigla Sistema","Mensurável", "Previsão Término", "Dt. Entrega", "Status", "PF detalhada Emp 1", "PF detalhada Emp 2", "Ação");
        $tamanho       = array("10%","10%","10%","10%", '10%', '10%', '10%', '10%', '10%', '10%');
        $alinhamento   = array("center","center","left","center", 'center', 'center', 'center', 'center', 'center', 'center');

        ob_start();
        $this->_db->monta_lista( $sqlOSEmAvaliacao, $cabecalho, 100,5, 'N', 'center', '', "", $tamanho, $alinhamento );
        $listaSS = ob_get_contents();
        ob_end_clean();
        
        return $listaSS;
    }
  
    /**
     * Lista as ordem de serviço
     * @return string
     */
    protected function listarOrdemServicoEmAprovacao()
    {
         
         $sqlOSEmAvaliacao = "SELECT os.scsid, os.odsid, sid.sidabrev                                    
                                    , to_char(fabrica.fn_data_previsao_termino_os(os.odsid), 'DD/MM/YYYY' ) as odsdtprevtermino
                                    , to_char(entrega.data_entrega, 'DD/MM/YYYY' ) as data_entrega
                                    , CASE entrega.data_entrega <= fabrica.fn_data_previsao_termino_os(os.odsid)
                                            WHEN 'f' THEN 'OS EM DIA'
                                            ELSE '<span style=\" color:red; \">OS EM ATRASO</span>'
                                        END as status
                                    , os.pf_a_pagar
                                    , ( (os.glosaqtdepf * fabrica.fn_porcentagem_disciplina_ss( os.scsid ) / 100 ) ) as glosaqtdepf
                                    , '<a id=\" '|| os.odsid ||'\" class=\"link generica\" title=\"Abrir Ordem de Serviço\">Aprovar/Reprovar</a>'
                                FROM fabrica.vw_ordem_servico os
                                INNER JOIN fabrica.solicitacaoservico ss
                                    ON os.scsid = ss.scsid
                                LEFT JOIN fabrica.analisesolicitacao ans
                                    ON ss.scsid = ans.scsid
                                LEFT JOIN demandas.sistemadetalhe sid
                                    ON ss.sidid = sid.sidid 
                                    
                                LEFT JOIN fabrica.fn_entrega_ordem_servico() entrega
                                    ON os.odsid = entrega.odsid                                    
                                WHERE os.esdid = ". WF_ESTADO_OS_APROVACAO . "
                                AND os.tosid 	= ". TIPO_OS_GERAL ;
        
        //exit($sqlOSEmAvaliacao);
        $cabecalho     = array("SS","OS","Sigla Sistema", "Previsão Término", "Dt. Entrega", "Status", "PF a Pagar", "Glosa(PF)", "Ação");
        $tamanho       = array("10%","10%","10%","10%", '10%', '10%', '10%', '10%', '10%', '10%');
        $alinhamento   = array("center","center","left","left", 'center', 'center', 'center', 'center', 'center',);

        ob_start();
        $this->_db->monta_lista( $sqlOSEmAvaliacao, $cabecalho, 100,5, 'N', 'center', '', "", $tamanho, $alinhamento );
        $listaSS = ob_get_contents();
        ob_end_clean();
        
        return $listaSS;
    }
    
    protected function listarOrdemServicoEmHomologacao()
    {
        $sqlOSEmHomologacao = "SELECT os.scsid, os.odsid, sid.siddescricao                                                                  
                                        , to_char(fabrica.fn_data_previsao_termino_os(os.odsid), 'DD/MM/YYYY' ) as odsdtprevtermino
                                        , to_char(entrega.data_entrega, 'DD/MM/YYYY' ) as data_entrega
                                        , CASE entrega.data_entrega <= fabrica.fn_data_previsao_termino_os(os.odsid)
                                            WHEN TRUE THEN 'OS EM DIA'
                                            ELSE '<span style=\" color:red; \">OS EM ATRASO</span>'
                                          END as status                               
                                        , fabrica.fn_calcula_valor_os(os.odsid) as valor
                                        , '<a id=\"'|| os.odsid ||'\" class=\"link generica\" title=\"Abrir Ordem de Serviço\">Homologar</a>'
                                FROM fabrica.vw_ordem_servico os
                                INNER JOIN fabrica.solicitacaoservico ss
                                    ON os.scsid = ss.scsid
                                LEFT JOIN fabrica.analisesolicitacao ans
                                    ON ss.scsid = ans.scsid
                                LEFT JOIN demandas.sistemadetalhe sid
                                    ON ss.sidid = sid.sidid
                                LEFT JOIN fabrica.fn_entrega_ordem_servico() entrega
                                    ON os.odsid = entrega.odsid 
                                WHERE os.esdid = ". WF_ESTADO_OS_ATESTO_TECNICO . "
                                AND os.tosid 	= ". TIPO_OS_GERAL ;
        
        $cabecalho     = array("SS","OS","Sistema", "Previsão Término", "Dt. Entrega", "Status", "Valor(R$) ", "Ação");
        $tamanho       = array("10%","10%","10%","10%", '10%', '10%', '10%', '10%');
        $alinhamento   = array("center","center","left","center", 'center', 'center', 'center', 'center');

        ob_start();
        $this->_db->monta_lista( $sqlOSEmHomologacao, $cabecalho, 100,5, 'N', 'center', '', "", $tamanho, $alinhamento );
        $listaSS = ob_get_contents();
        ob_end_clean();
        
        return $listaSS;
    }
    
    protected function listarOrdemServicoEmPagamento()
    {
        
        $sqlOSEmPagamento = "SELECT 
                                '<a id=\"\" class=\"link\" title=\"Abrir Solicitação de Serviço\" href=\"fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='|| os.scsid ||'&ansid='|| ans.ansid ||' \">'|| os.scsid ||'</a>'
                                , '<a id=\"'|| os.odsid ||'\" class=\"link generica\" title=\"Abrir Ordem de Serviço\">'|| os.odsid ||'</a>' as os
                                , sid.siddescricao   
                                , tps.tpsdsc, tpdps.tpdpsdsc
                                , ( (fabrica.fn_menor_valor_pf_os(os.odsid) * fabrica.fn_porcentagem_disciplina_ss(os.scsid)) / 100 )  as pf_liquido
                                , COALESCE(os.glosaqtdepf,0) as glosaqtdepf
                                , ( ( (fabrica.fn_menor_valor_pf_os(os.odsid) * fabrica.fn_porcentagem_disciplina_ss(os.scsid)) / 100 ) - COALESCE(os.glosaqtdepf,0) ) as pf_final
                                , fabrica.fn_calcula_valor_os(os.odsid) as valor 
                                FROM fabrica.vw_ordem_servico os
                                INNER JOIN fabrica.solicitacaoservico ss
                                    ON os.scsid = ss.scsid
                                LEFT JOIN fabrica.analisesolicitacao ans
                                    ON os.scsid = ans.scsid
                                LEFT JOIN fabrica.tiposervico tps
                                    ON ans.tpsid = tps.tpsid
                                LEFT JOIN fabrica.tipodespesa tpdps
                                    ON tps.tpdpsid = tpdps.tpdpsid
                                LEFT JOIN demandas.sistemadetalhe sid
                                    ON ss.sidid = sid.sidid                                         
                                WHERE os.esdid = ". WF_ESTADO_OS_AGUARDANDO_PAGAMENTO . "
                                AND os.tosid 	= ". TIPO_OS_GERAL ;
        
        $cabecalho     = array("SS","OS","Sistema", "Tipo Serviço", "Tipo Despesa", "PF Líquido", "Glosa(PF)", "PF Final", "Valor(R$)");
        $tamanho       = array("10%","10%","10%","10%", '10%', '10%', '10%', '10%', '10%');
        $alinhamento   = array("center","center","left","left", 'left', 'center', 'center', 'center', 'center');
        
        ob_start();
        $this->_db->monta_lista( $sqlOSEmPagamento, $cabecalho, 100,5, 'N', 'center', '', "", $tamanho, $alinhamento );
        $listaSS = ob_get_contents();
        ob_end_clean();
        
        return $listaSS;
    }
    
    protected function listarOrdemServicoEmPausa()
    {
        
        $sqlOSEmPagamento = "SELECT 
                                    '<a id=\"\" class=\"link\" title=\"Abrir Solicitação de Serviço\" href=\"fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='|| os.scsid ||'&ansid='|| ans.ansid ||' \">'|| os.scsid ||'</a>' as ss
                                    , '<a id=\"'|| os.odsid ||'\" class=\"link generica\" title=\"Abrir Ordem de Serviço\">'|| os.odsid ||'</a>' as os
                                    , sid.sidabrev
                                    , ss.scsnecessidade
                                    , to_char( fabrica.fn_data_previsao_termino_os(os.odsid), 'DD/MM/YYYY' ) as odsdtprevtermino
                                    , to_char( pausa.data_entrada_pausa, 'DD/MM/YYYY' ) as data_entrada_pausa
                                    , to_char( ( current_date - pausa.data_entrada_pausa), 'DD') as dias_em_pausa
                                    , pausa.cmddsc as motivo_pausa
                                FROM fabrica.vw_ordem_servico os
                                INNER JOIN fabrica.solicitacaoservico ss
                                    ON os.scsid = ss.scsid
                                LEFT JOIN fabrica.analisesolicitacao ans
                                    ON os.scsid = ans.scsid
                                LEFT JOIN fabrica.tiposervico tps
                                    ON ans.tpsid = tps.tpsid
                                LEFT JOIN fabrica.tipodespesa tpdps
                                    ON tps.tpdpsid = tpdps.tpdpsid
                                LEFT JOIN demandas.sistemadetalhe sid
                                    ON ss.sidid = sid.sidid                
                                 INNER JOIN 
                                        (
                                        SELECT  os.odsid, MAX(hd.htddata) as data_entrada_pausa, MAX(cmd.cmddsc) as cmddsc
                                        FROM fabrica.ordemservico os
                                        INNER JOIN workflow.documento dc
                                            ON dc.docid = os.docid
                                        INNER JOIN workflow.historicodocumento hd
                                            ON dc.docid = hd.docid
                                        LEFT JOIN workflow.comentariodocumento cmd
                                            ON hd.hstid = cmd.hstid
                                        INNER JOIN workflow.acaoestadodoc a
                                            ON a.aedid  = hd.aedid
                                        INNER JOIN workflow.estadodocumento ed
                                            ON ed.esdid = dc.esdid
                                        WHERE a.esdiddestino    = ". WF_ESTADO_OS_PAUSA ."
                                        AND ed.tpdid            = ". WORKFLOW_ORDEM_SERVICO ."
                                        GROUP BY os.odsid
                                        ) as pausa   
                                        ON os.odsid = pausa.odsid
                                WHERE os.esdid = ". WF_ESTADO_OS_PAUSA . "
                                AND os.tosid 	= ". TIPO_OS_GERAL ;
        
        $cabecalho     = array("SS","OS","Sigla Sistema", "Necessidade", "Previsão Término", "Pausa Em", "Dias em Pausa", "Motivo da Pausa");
        $tamanho       = array("5%","5%","10%","20%", '10%', '5%', '5%', '20%');
        $alinhamento   = array("center","center","left","left", 'center', 'center', 'center', 'center');

        ob_start();
        $this->_db->monta_lista( $sqlOSEmPagamento, $cabecalho, 100,5, 'N', 'center', '', "", $tamanho, $alinhamento );
        $listaSS = ob_get_contents();
        ob_end_clean();
        
        return $listaSS;
    }
    
    protected function listarOrdemServicoEstimadaEmAprovacao()
    {
        
        $sqlOSEmAprovacao = "SELECT os.scsid, os.odsid, sid.sidabrev
                                    , to_char( os.odsdtprevinicio, 'DD/MM/YYYY' ) as odsdtprevinicio
                                    , to_char( fabrica.fn_data_previsao_termino_os(os.odsid), 'DD/MM/YYYY' ) as odsdtprevtermino
                                    , to_char( entrega.data_entrega, 'DD/MM/YYYY' ) as data_entrega
                                    , osgen.subtotal_pf_estimado as pf_emp1
                                    , os.subtotal_pf_estimado as pf_emp2
                                    , '<a id=\"'|| os.odsid ||'\" class=\"link empresa2\" title=\"Abrir Ordem de Serviço\" href=\"fabrica.php?modulo=principal/cadContagemOS&acao=A&odsid='||os.odsid||'&scsid='||os.scsid || '\" >Aprovar/Reprovar</a>'
                                FROM fabrica.vw_ordem_servico os
                                INNER JOIN fabrica.solicitacaoservico ss
                                    ON os.scsid = ss.scsid
                                LEFT JOIN fabrica.analisesolicitacao ans
                                    ON os.scsid = ans.scsid                                
                                LEFT JOIN demandas.sistemadetalhe sid
                                    ON ss.sidid = sid.sidid                                                 
                                LEFT JOIN fabrica.fn_entrega_ordem_servico() entrega
                                    ON os.odsid = entrega.odsid
                                INNER JOIN fabrica.vw_ordem_servico osgen
                                    ON os.odsidpai = osgen.odsid
                                    AND osgen.tosid = ". TIPO_OS_GERAL ."
                                WHERE os.esdid = ". WF_ESTADO_CPF_APROVACAO . "
                                AND os.tosid 	= ". TIPO_OS_CONTAGEM_ESTIMADA ;

        $cabecalho     = array("SS","OS","Sigla Sistema", "Previsão Início", "Previsão Término", "Entrega Em", "PF Emp. 1", "PF Emp. 2", "Ação");
        $tamanho       = array("5%","5%","10%","5%", '10%', '5%', '5%', '20%', '5%');
        $alinhamento   = array("center","center","left","center", 'center', 'center', 'center', 'center', 'center');
        
        ob_start();
        $this->_db->monta_lista( $sqlOSEmAprovacao, $cabecalho, 100,5, 'N', 'center', '', "", $tamanho, $alinhamento );
        $listaSS = ob_get_contents();
        ob_end_clean();
        
        return $listaSS;
    }
    
    protected function listarOrdemServicoDetalhadaEmAprovacao()
    {
        
        $sqlOSEmAprovacao = "SELECT os.scsid, os.odsid, sid.sidabrev
                                    , to_char( os.odsdtprevinicio, 'DD/MM/YYYY' ) as odsdtprevinicio
                                    , to_char( fabrica.fn_data_previsao_termino_os(os.odsid), 'DD/MM/YYYY' ) as odsdtprevtermino
                                    , to_char( entrega.data_entrega, 'DD/MM/YYYY' ) as data_entrega
                                    , osgen.pf_detalhado as subtotal_pf_emp1
                                    , os.pf_detalhado as subtotal_pf_emp2
                                    , osgen.pf_a_pagar as pf_a_pagar_emp1
                                    , os.pf_a_pagar as pf_a_pagar_emp2
                                    , '<a id=\"'|| os.odsid ||'\" class=\"link empresa2\" title=\"Abrir Ordem de Serviço\" href=\"fabrica.php?modulo=principal/cadContagemOS&acao=A&odsid='||os.odsid||'&scsid='||os.scsid || '\" >Aprovar/Reprovar</a>'
                                FROM fabrica.vw_ordem_servico os
                                INNER JOIN fabrica.solicitacaoservico ss
                                    ON os.scsid = ss.scsid
                                LEFT JOIN fabrica.analisesolicitacao ans
                                    ON os.scsid = ans.scsid                                
                                LEFT JOIN demandas.sistemadetalhe sid
                                    ON ss.sidid = sid.sidid                                                 
                                LEFT JOIN fabrica.fn_entrega_ordem_servico() entrega
                                    ON os.odsid = entrega.odsid
                                INNER JOIN fabrica.vw_ordem_servico osgen
                                    ON os.odsidpai = osgen.odsid
                                    AND osgen.tosid = ". TIPO_OS_GERAL ."
                                WHERE os.esdid = ". WF_ESTADO_CPF_APROVACAO . "
                                AND os.tosid 	= ". TIPO_OS_CONTAGEM_DETALHADA ;
        
        $cabecalho     = array("SS","OS","Sigla Sistema", "Previsão Início", "Previsão Término", "Entrega Em", "Subtotal PF Emp. 1", "Subtotal PF Emp. 2", "PF a Pagar Emp. 1", "PF a Pagar Emp. 2", "Ação");
        $tamanho       = array("5%","5%","10%","5%", '10%', '5%', '5%', '5%', '5%', '5%', '5%');
        $alinhamento   = array("center","center","left","center", 'center', 'center', 'center', 'center', 'center', 'center', 'center');
        
        ob_start();
        $this->_db->monta_lista( $sqlOSEmAprovacao, $cabecalho, 100,5, 'N', 'center', '', "", $tamanho, $alinhamento );
        $listaSS = ob_get_contents();
        ob_end_clean();
        
        return $listaSS;
    }
    
    protected function listarOrdemServicoCPFEmPagamento()
    {
        $sqlOSEmAprovacao = "SELECT 
                                '<a id=\"\" class=\"link\" title=\"Abrir Solicitação de Serviço\" href=\"fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='|| os.scsid ||'&ansid='|| ans.ansid ||' \">'|| os.scsid ||'</a>' as ss
                                , '<a id=\"'|| os.odsid ||'\" class=\"link empresa2\" title=\"Abrir Ordem de Serviço\" href=\"fabrica.php?modulo=principal/cadContagemOS&acao=A&odsid='||os.odsid||'&scsid='||os.scsid || '\" >'|| os.odsid ||'</a>' as os
                                    , sid.siddescricao
                                    , os.tosdsc
                                    , os.pf_a_pagar as pf
                                    , COALESCE( os.glosaqtdepf, 0 ) as  glosaqtdepf
                                    , ( os.pf_a_pagar  - COALESCE( os.glosaqtdepf, 0 ) ) as pf_final
                                    , fabrica.fn_calcula_valor_os( os.odsid ) as valor
                                FROM fabrica.vw_ordem_servico os
                                INNER JOIN fabrica.solicitacaoservico ss
                                    ON os.scsid = ss.scsid
                                LEFT JOIN fabrica.analisesolicitacao ans
                                    ON os.scsid = ans.scsid                                
                                LEFT JOIN demandas.sistemadetalhe sid
                                    ON ss.sidid = sid.sidid                                                 
                                LEFT JOIN fabrica.fn_entrega_ordem_servico() entrega
                                    ON os.odsid = entrega.odsid
                                WHERE os.esdid = ". WF_ESTADO_CPF_AGUARDANDO_PAGAMENTO . "
                                AND os.tosid 	IN ( ". TIPO_OS_CONTAGEM_DETALHADA .", ". TIPO_OS_CONTAGEM_ESTIMADA ." )";

        $cabecalho     = array("SS","OS","Sistema", "Tipo OS", "PF", "Glosa(PF)", "PF Final", "Valor(R$)");
        $tamanho       = array("5%","5%","10%","10%", '5%', '5%', '5%', '5%');
        $alinhamento   = array("center","center","left","center", 'center', 'center', 'center', 'center');
        
        ob_start();
        $this->_db->monta_lista( $sqlOSEmAprovacao, $cabecalho, 100,5, 'N', 'center', '', "", $tamanho, $alinhamento );
        $listaSS = ob_get_contents();
        ob_end_clean();
        
        return $listaSS;
    }
    
    protected function listarOrdemServicoCPFEmDivergencia()
    {
        
        $sqlOSEmAprovacao = "SELECT 
                                    '<a id=\"\" class=\"link\" title=\"Abrir Solicitação de Serviço\" href=\"fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='|| os.scsid ||'&ansid='|| ans.ansid ||' \">'|| os.scsid ||'</a>' as ss
                                    , '<a id=\"'|| os.odsid ||'\" class=\"link empresa2\" title=\"Abrir Ordem de Serviço\" href=\"fabrica.php?modulo=principal/cadContagemOS&acao=A&odsid='||os.odsid||'&scsid='||os.scsid || '\" >'|| os.odsid ||'</a>' as os
                                    , sid.sidabrev
                                    , to_char( divergencia.data_entrada_divergencia, 'DD/MM/YYYY') as data_entrada_divergencia
                                    , to_char( (current_date -divergencia.data_entrada_divergencia) , 'DD'  ) as dias_divergencia
                                    , osgen.pf_detalhado as subtotal_pf_emp1
                                    , osgen.pf_a_pagar as qtd_pf_emp1
                                    , os.pf_detalhado as subtotal_pf_emp2
                                    , os.pf_a_pagar as qtd_pf_emp2                                                                        
                                FROM fabrica.vw_ordem_servico os
                                INNER JOIN fabrica.solicitacaoservico ss
                                    ON os.scsid = ss.scsid
                                LEFT JOIN fabrica.analisesolicitacao ans
                                    ON os.scsid = ans.scsid                                
                                LEFT JOIN demandas.sistemadetalhe sid
                                    ON ss.sidid = sid.sidid                                                 
                                LEFT JOIN fabrica.fn_entrega_ordem_servico() entrega
                                    ON os.odsid = entrega.odsid
                                INNER JOIN fabrica.vw_ordem_servico osgen
                                    ON os.odsidpai = osgen.odsid
                                    AND osgen.tosid = ". TIPO_OS_GERAL ."
                                INNER JOIN 
                                        (
                                        SELECT  os.odsid, MAX(hd.htddata) as data_entrada_divergencia, MAX(cmd.cmddsc) as cmddsc
                                        FROM fabrica.ordemservico os
                                        INNER JOIN workflow.documento dc
                                            ON dc.docid = os.docidpf
                                        INNER JOIN workflow.historicodocumento hd
                                            ON dc.docid = hd.docid
                                        LEFT JOIN workflow.comentariodocumento cmd
                                            ON hd.hstid = cmd.hstid
                                        INNER JOIN workflow.acaoestadodoc a
                                            ON a.aedid  = hd.aedid
                                        INNER JOIN workflow.estadodocumento ed
                                            ON ed.esdid = dc.esdid
                                        WHERE a.esdiddestino    = ". WF_ESTADO_CPF_DIVERGENCIA ."
                                        AND ed.tpdid            = ". WORKFLOW_CONTAGEM_PF."
                                        GROUP BY os.odsid
                                        ) as divergencia   
                                        ON os.odsid = divergencia.odsid
                                WHERE os.esdid = ". WF_ESTADO_CPF_DIVERGENCIA . "
                                AND os.tosid 	IN ( ". TIPO_OS_CONTAGEM_DETALHADA .", ". TIPO_OS_CONTAGEM_ESTIMADA ." )";
        
        $cabecalho     = array("SS","OS","Sigla Sistema", "Enviado para Divergência em:", "Dias em Divergência", "Subtotal de PF Empresa 1", "Qtd. PF Empresa 1", "Subtotal de PF Empresa 2", "Qtd. PF Empresa 2");
        $tamanho       = array("5%","5%","10%","10%", '5%', '5%', '5%', '5%', '5%');
        $alinhamento   = array("center","center","left","center", 'center', 'center', 'center', 'center', 'center');
        
        ob_start();
        $this->_db->monta_lista( $sqlOSEmAprovacao, $cabecalho, 100,5, 'N', 'center', '', "", $tamanho, $alinhamento );
        $listaSS = ob_get_contents();
        ob_end_clean();
        
        return $listaSS;
    }
    
    
    /**
     * Lista as ss que estejam em situação de análise
     * @return string 
     */
    protected function listarSolicitacaoEmAnalise()
    {
        
        $sqlSSEmAnalise = "SELECT ss.scsid
                                ,  sid.siddescricao, usu.usunome
                                , ss.scsnecessidade
                                , '<a id=\"\" class=\"link\" title=\"Abrir Solicitação de Serviço\" href=\"fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='|| ss.scsid ||' \">Enviar para Detalhamento</a>'
                            FROM fabrica.solicitacaoservico ss
                            INNER JOIN workflow.documento doc
                                ON ss.docid = doc.docid
                            INNER JOIN seguranca.usuario usu
                                ON ss.usucpfrequisitante = usu.usucpf
                            LEFT JOIN demandas.sistemadetalhe sid
                                ON ss.sidid = sid.sidid
                            LEFT JOIN fabrica.analisesolicitacao ans
                                ON ss.scsid = ans.scsid
                            WHERE ss.scsstatus = 'A' 
                            AND doc.esdid = ". WF_ESTADO_ANALISE;
        
        
        $cabecalho     = array("SS","Sistema","Requisitante","Necessidade", "Ação");
        $tamanho       = array("5%","15%","20%","50%", '10%');
        $alinhamento   = array("center","left","left","left", 'center');

        ob_start();
        $this->_db->monta_lista( $sqlSSEmAnalise,$cabecalho,100,5,'N','center','',"",$tamanho,$alinhamento );
        $listaSS = ob_get_contents();
        ob_end_clean();
        
        return $listaSS;
    }
    
    /**
     * Lista as ss que estejam em situação de análise
     * @param int $esdid-  código de um dos dois estados
     * 
     * @return string 
     */
    protected function listarSolicitacaoEmAprovacaoAvaliacao( $esdId )
    {
        $sqlSSEmAvaliacao = "SELECT ss.scsid,  sid.sidabrev
                                , to_char( ss.scsprevatendimento, 'DD/MM/YYYY' ) as expectiva_atendimento
                                , to_char( ans.ansprevtermino, 'DD/MM/YYYY' ) as data_termino
                                , CASE 
                                    WHEN ans.mensuravel IS NULL THEN 'Não Informado'
                                    WHEN ans.mensuravel = 't' THEN 'Sim'
                                    WHEN ans.mensuravel = 'f' THEN 'Nâo'
                                  END as mensuravel
                                , os.qtd_pf_estimado as pf_estimado_emp1
                                , CASE 
                                    WHEN ospf.esdid = ". WF_ESTADO_CPF_APROVACAO ." THEN ospf.qtd_pf_estimado::text
                                    WHEN ospf.esdid != ". WF_ESTADO_CPF_APROVACAO ." THEN ospf.esddsc
                                    WHEN ospf.odsid IS NULL THEN 'N/A'                                    
                                  END as pf_estimado_emp2
                                 , CASE 
                                    WHEN ospf.odsid IS NULL AND doc.esdid = ". WF_ESTADO_AVALIACAO ." THEN '<a id=\"\" class=\"link\" title=\"Abrir Solicitação de Serviço\" href=\"fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='|| ss.scsid ||' \">Enviar para Aprovação</a>'
                                    WHEN ospf.odsid IS NULL AND doc.esdid = ". WF_ESTADO_APROVACAO ." THEN '<a id=\"\" class=\"link\" title=\"Abrir Solicitação de Serviço\" href=\"fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='|| ss.scsid ||' \">Aprovar/Reprovar</a>'
                                    WHEN ospf.esdid = ". WF_ESTADO_CPF_AGUARDANDO_CONTAGEM ." THEN ''
                                    WHEN ospf.odsid IS NOT NULL AND doc.esdid = ". WF_ESTADO_AVALIACAO ." THEN '<a id=\"\" class=\"link\" title=\"Abrir Solicitação de Serviço\" href=\"fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='|| ss.scsid ||' \">Enviar para Aprovação</a>'
                                    WHEN ospf.odsid IS NOT NULL AND doc.esdid = ". WF_ESTADO_APROVACAO ." THEN '<a id=\"\" class=\"link\" title=\"Abrir Solicitação de Serviço\" href=\"fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='|| ss.scsid ||' \">Aprovar/Reprovar</a>'                                    
                                  END as acao
                            FROM fabrica.solicitacaoservico ss
                            INNER JOIN workflow.documento doc
                                ON ss.docid = doc.docid
                            INNER JOIN seguranca.usuario usu
                                ON ss.usucpfrequisitante = usu.usucpf
                            LEFT JOIN demandas.sistemadetalhe sid
                                ON ss.sidid = sid.sidid
                            LEFT JOIN fabrica.analisesolicitacao ans
                                ON ss.scsid = ans.scsid
                            LEFT JOIN fabrica.vw_ordem_servico os
                                ON ss.scsid = os.scsid
                                AND os.tosid = ". TIPO_OS_GERAL ."
                                AND os.esdid NOT IN ( ". WF_ESTADO_OS_CANCELADA_SEM_CUSTO ." )
                            LEFT JOIN fabrica.vw_ordem_servico ospf
                                ON ss.scsid = ospf.scsid
                                AND ospf.tosid = ". TIPO_OS_CONTAGEM_ESTIMADA ."
                                AND ospf.esdid NOT IN ( ". WF_ESTADO_CPF_CANCELADA .", ". WF_ESTADO_CPF_REVISAO .", ". WF_ESTADO_CPF_PENDENTE ." )
                            WHERE ss.scsstatus = 'A'
                            AND doc.esdid = ". $esdId;
        
        
        $cabecalho     = array("SS","Sigla Sistema", 'Expect. Atend.', 'Previsão Término',"Mensurável","PF Est. Empresa 1", 'PF Est. Empresa 2', "Ação");
        $tamanho       = array("5%","15%",'5%' , '5%',  "15%","20%", '20%', '30%');
        $alinhamento   = array("center","left", 'center', 'center', "left","center", 'center', 'center');
        
        ob_start();
        $this->_db->monta_lista( $sqlSSEmAvaliacao, $cabecalho, 100, 5, 'N', 'center', '', "", $tamanho, $alinhamento );
        $listaSS = ob_get_contents();
        ob_end_clean();
        
        return $listaSS;
    }
    
    protected function listarSolicitacaoEmPausa()
    {
        $sqlSSEmPausa = "SELECT 
                            '<a id=\"\" class=\"link\" title=\"Abrir Solicitação de Serviço\" href=\"fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='|| ss.scsid ||'&ansid='|| ans.ansid ||' \">'|| ss.scsid ||'</a>' as ss                                
                            ,  sid.siddescricao
                            , ss.scsnecessidade
                            , to_char( ss.scsprevatendimento, 'DD/MM/YYYY') as prevatendimento
                            , to_char( ans.ansprevtermino, 'DD/MM/YYYY') as ansprevtermino
                            , to_char( divergencia.data_entrada_pausa, 'DD/MM/YYYY') as pausa_em
                            , ( ( date( current_date ) - date( divergencia.data_entrada_pausa ) ) + 1)  as dias_em_pausa
                            , CASE
                                WHEN divergencia.cmddsc IS NOT NULL THEN divergencia.cmddsc
                                WHEN divergencia.cmddsc IS NULL THEN 'Motivo não informado'
                            END as motivo_pausa
                        FROM fabrica.solicitacaoservico ss
                        INNER JOIN workflow.documento doc
                            ON ss.docid = doc.docid
                        INNER JOIN seguranca.usuario usu
                            ON ss.usucpfrequisitante = usu.usucpf
                        LEFT JOIN demandas.sistemadetalhe sid
                            ON ss.sidid = sid.sidid
                        LEFT JOIN fabrica.analisesolicitacao ans
                            ON ss.scsid = ans.scsid
                        INNER JOIN 
                            (
                            SELECT  ss.scsid, MAX(hd.htddata) as data_entrada_pausa, MAX(cmd.cmddsc) as cmddsc
                            FROM fabrica.solicitacaoservico ss
                            INNER JOIN workflow.documento dc
                                ON dc.docid = ss.docid
                            INNER JOIN workflow.historicodocumento hd
                                ON dc.docid = hd.docid
                            LEFT JOIN workflow.comentariodocumento cmd
                                ON hd.hstid = cmd.hstid
                            INNER JOIN workflow.acaoestadodoc a
                                ON a.aedid  = hd.aedid
                            INNER JOIN workflow.estadodocumento ed
                                ON ed.esdid = dc.esdid
                            WHERE a.esdiddestino    = ". WF_ESTADO_SS_PAUSA ."
                            AND ed.tpdid            = ". WORKFLOW_SOLICITACAO_SERVICO ."
                            GROUP BY ss.scsid
                            ) as divergencia
                            ON ss.scsid = divergencia.scsid
                        WHERE ss.scsstatus = 'A'
                        AND doc.esdid = ". WF_ESTADO_SS_PAUSA. "
                        ORDER BY ss.scsid ASC";
        
        //ver( $sqlSSEmPausa );
        
        $cabecalho     = array("SS","Sistema","Necessidade","Expectativa de atendimento", 'Previsão de Témino', "Pausa em", 'Dias em Pausa', 'Motivo da Pausa');
        $tamanho       = array("3%","10%","15%","5%", '5%', '5%', '3%', '10%');
        $alinhamento   = array("center","left","left","center", 'center', 'center', 'CENTER', 'left');

        ob_start();
        $this->_db->monta_lista( $sqlSSEmPausa, $cabecalho, 100, 5, 'N', 'center', '', "", $tamanho, $alinhamento );
        $listaSS = ob_get_contents();
        ob_end_clean();
        
        return $listaSS;
    }
 
   /**
     * Exibe o painel operacional da empresa do item 1
     * 
     * @return string $painel - Html do painel montado
     */
    public function painelOperacionalEmpresaItem1() {
        
        
        $estadosSSPainelOperacional = array(
            WF_ESTADO_ANALISE, WF_ESTADO_AVALIACAO, WF_ESTADO_APROVACAO
            , WF_ESTADO_SS_PAUSA
        );
        
        $estadosOSPainelOperacional = array(
            WF_ESTADO_OS_VERIFICACAO, WF_ESTADO_OS_APROVACAO
            , WF_ESTADO_OS_ATESTO_TECNICO, WF_ESTADO_OS_AGUARDANDO_PAGAMENTO
            , WF_ESTADO_OS_PAUSA
        );
        
        
        $sqlSSPainelOperacional = "SELECT  '<a id=\"'|| esd.esdid ||'\" class=\"estadoSolicitacaoServico link\" >'
                                            || 
                                            CASE esd.esdid
                                                WHEN ". WF_ESTADO_SS_PAUSA ." THEN '<img title=\"Visualizar\" src=\"../imagens/icone_lupa.png\" /></a>' 
                                                ELSE '<img title=\"Ação\" src=\"../imagens/editar_nome.gif\" /></a>' 
                                             END as acao
                                            , esd.tpdid, esd.esdid, esd.esddsc
                                            , COUNT ( ss.scsid ) as total
                                            , (CASE
                                                    WHEN esd.esdid = ". WF_ESTADO_ANALISE ."   THEN 1 
                                                    WHEN esd.esdid = ". WF_ESTADO_AVALIACAO ." THEN 2 
                                                    WHEN esd.esdid = ". WF_ESTADO_APROVACAO ." THEN 3 
                                                    WHEN esd.esdid = ". WF_ESTADO_SS_PAUSA ."  THEN 4 
                                                END) as situacao
                                    FROM workflow.estadodocumento esd
                                    LEFT JOIN workflow.documento doc
                                        ON esd.esdid = doc.esdid
                                    LEFT JOIN fabrica.solicitacaoservico ss
                                        ON doc.docid = ss.docid                                     
                                    WHERE esd.esdid IN ( ". implode( ',', $estadosSSPainelOperacional ) ." )
                                    AND esd.tpdid = ". WORKFLOW_SOLICITACAO_SERVICO ."
                                    AND ss.scsstatus = 'A' 
                                    GROUP BY esd.tpdid, esd.esdid, esd.esddsc
                                    ORDER BY 3";
        
        $sqlOSPainelOperacional = "SELECT '<a id=\"'|| esd.esdid ||'\" class=\"estadoOrdemServico link\" >'
                                            || 
                                            CASE esd.esdid
                                                WHEN ". WF_ESTADO_OS_AGUARDANDO_PAGAMENTO ."  THEN '<img title=\"Visualizar\" src=\"../imagens/icone_lupa.png\" /></a>' 
                                                WHEN ". WF_ESTADO_OS_PAUSA ."  THEN '<img title=\"Visualizar\" src=\"../imagens/icone_lupa.png\" /></a>' 
                                                ELSE '<img title=\"Ação\" src=\"../imagens/editar_nome.gif\" /></a>'
                                            END as acao
                                            , esd.tpdid, esd.esdid, esd.esddsc
                                            , COUNT ( os.odsid ) as total
                                             , (CASE
                                                    WHEN esd.esdid = ". WF_ESTADO_OS_VERIFICACAO ."             THEN 1 
                                                    WHEN esd.esdid = ". WF_ESTADO_OS_APROVACAO ."               THEN 2 
                                                    WHEN esd.esdid = ". WF_ESTADO_OS_ATESTO_TECNICO ."          THEN 3 
                                                    WHEN esd.esdid = ". WF_ESTADO_OS_AGUARDANDO_PAGAMENTO ."    THEN 4 
                                                    WHEN esd.esdid = ". WF_ESTADO_OS_PAUSA ."                   THEN 5 
                                                END) as situacao
                                    FROM workflow.estadodocumento esd
                                    LEFT JOIN workflow.documento doc
                                        ON esd.esdid = doc.esdid
                                    LEFT JOIN fabrica.ordemservico os
                                        ON doc.docid = os.docid 
                                    WHERE esd.esdid IN ( ". implode( ',', $estadosOSPainelOperacional ) ." )
                                    AND os.tosid    = ". TIPO_OS_GERAL ."
                                    AND esd.tpdid   = ". WORKFLOW_ORDEM_SERVICO  ." 
                                    GROUP BY esd.tpdid, esd.esdid, esd.esddsc
                                    ORDER BY 3";
        
        $dadosPainelSS  = $this->_db->carregar( $sqlSSPainelOperacional );
        $subGridSS      = $this->_montaGridPainelOperacional('Solicitação de Serviço', $dadosPainelSS, 'SS');
        
        $dadosPainelOS  = $this->_db->carregar( $sqlOSPainelOperacional );
        $subGridOS      = $this->_montaGridPainelOperacional('Ordem de Serviço', $dadosPainelOS, 'OS');

        $gridPainel = '<table class="listagem">';
        $gridPainel .= '<tr><td colspan="2" class="TituloTabela center">Situação OS/SS </td>';
        $gridPainel .= '</tr>';
        $gridPainel .= $subGridSS;
        $gridPainel .= $subGridOS;
        $gridPainel .= '</table>';
        
        return $gridPainel;
    }
    
    
    /*
     * atencao retirar os comentarios dos status = 'A'
     */
    public function painelOperacionalGerenteProjetos($dados) {
        
        //var_dump($dados);exit;
        $constantesPainelAcoesSSpreAnalise = array(
            WF_ESTADO_PRE_ANALISE
        );
        $constantesPainelAcoesSSaprovacao = array(
            WF_ESTADO_APROVACAO
        );
        
        $constantesPainelAcoesOS = array(
            WF_ESTADO_OS_APROVACAO
        );
        
        $sqlSSPainelAcoes = "(SELECT  '<a id=\"". WF_ESTADO_PRE_ANALISE ."\" class=\"painelGerenteProjetosSS link\" >'
                                      || '<img title=\"Ação\" src=\"../imagens/editar_nome.gif\" /></a>'as acao
                                            , esd.tpdid, esd.esdid
                                            ,(CASE
                                                    WHEN esd.esdid = ". WF_ESTADO_PRE_ANALISE ."   THEN 'Realizar Pré Análise' 
                                                END) as esddsc
                                            , COUNT ( ss.scsid ) as total
                                            , (CASE
                                                    WHEN esd.esdid = ". WF_ESTADO_PRE_ANALISE ."   THEN 1 
                                                END) as situacao
                                    FROM workflow.estadodocumento esd
                                    LEFT JOIN workflow.documento doc
                                        ON esd.esdid = doc.esdid
                                    LEFT JOIN fabrica.solicitacaoservico ss
                                        ON doc.docid = ss.docid     
                                    LEFT JOIN demandas.sistemadetalhe s
                                        ON s.sidid = ss.sidid
                                    LEFT JOIN demandas.sistemacelula c
                                        ON s.sidid = c.sidid    
                                    WHERE esd.esdid IN ( ". implode( ',', $constantesPainelAcoesSSpreAnalise ) ." ) ";
                                        
                                    if ( !empty($dados["celid"]) ){
                                        $sqlSSPainelAcoes .= " AND c.celid = {$dados["celid"]}";
                                    }
                                    if ( !empty($dados["sidid"]) ){
                                        $sqlSSPainelAcoes .= " AND s.sidid = {$dados["sidid"]}";
                                    }

                                $sqlSSPainelAcoes .="   AND esd.tpdid = ". WORKFLOW_SOLICITACAO_SERVICO ."
                                        AND ss.scsstatus = 'A' 
                                    GROUP BY esd.tpdid, esd.esdid, esd.esddsc)
        
                                    UNION
       
        
                                (SELECT  '<a id=\"". WF_ESTADO_APROVACAO ."\" class=\"painelGerenteProjetosSS link\" >'
                                            || '<img title=\"Ação\" src=\"../imagens/editar_nome.gif\" /></a>' as acao
                                            , esd.tpdid, esd.esdid
                                            ,(CASE
                                                    WHEN esd.esdid = ". WF_ESTADO_APROVACAO ."   THEN 'Aprovar Execução do Serviço' 
                                                END) as esddsc
                                            , COUNT ( ss.scsid ) as total
                                            , (CASE
                                                    WHEN esd.esdid = ". WF_ESTADO_APROVACAO ."   THEN 2 
                                                END) as situacao
                                    FROM workflow.estadodocumento esd
                                    LEFT JOIN workflow.documento doc
                                        ON esd.esdid = doc.esdid
                                    LEFT JOIN fabrica.solicitacaoservico ss
                                        ON doc.docid = ss.docid     
                                    LEFT JOIN demandas.sistemadetalhe s
                                        ON s.sidid = ss.sidid
                                    LEFT JOIN demandas.sistemacelula c
                                        ON s.sidid = c.sidid    

                                    WHERE esd.esdid IN ( ". implode( ',', $constantesPainelAcoesSSaprovacao ) ." ) ";
                                        
                                    if ( !empty($dados["celid"]) ){
                                        $sqlSSPainelAcoes .= " AND c.celid = {$dados["celid"]}";
                                    }
                                    if ( !empty($dados["sidid"]) ){
                                        $sqlSSPainelAcoes .= " AND s.sidid = {$dados["sidid"]}";
                                    }

                                $sqlSSPainelAcoes .="   AND esd.tpdid = ". WORKFLOW_SOLICITACAO_SERVICO ."
                                        AND ss.scsstatus = 'A' 
                                    GROUP BY esd.tpdid, esd.esdid, esd.esddsc)
        
                                    UNION
        
                                    (SELECT '<a id=\"". WF_ESTADO_OS_APROVACAO ."\" class=\"painelGerenteProjetosSS link\" >'
                                            || 
                                            '<img title=\"Ação\" src=\"../imagens/editar_nome.gif\" /></a>' as acao
                                            , esd.tpdid, esd.esdid
                                             ,(CASE
                                                    WHEN esd.esdid = ". WF_ESTADO_OS_APROVACAO ." THEN 'Homologar Ordem de Serviço' 
                                                END) as esddsc
                                            , COUNT ( os.odsid ) as total
                                             , (CASE
                                                    WHEN esd.esdid = ". WF_ESTADO_OS_APROVACAO ." THEN 3 
                                                END) as situacao
                                    FROM workflow.estadodocumento esd
                                    LEFT JOIN workflow.documento doc
                                        ON esd.esdid = doc.esdid
                                    LEFT JOIN fabrica.ordemservico os
                                        ON doc.docid = os.docid
                                    LEFT JOIN fabrica.solicitacaoservico ss
                                            ON os.scsid = ss.scsid
                                    LEFT JOIN demandas.sistemadetalhe s
                                            ON s.sidid = ss.sidid
                                     LEFT JOIN demandas.sistemacelula c
                                            ON s.sidid = c.sidid
                                    WHERE esd.esdid IN ( ". implode( ',', $constantesPainelAcoesOS ) ." ) ";
                                    if ( !empty($dados["celid"]) ){
                                        $sqlSSPainelAcoes .= " AND c.celid = {$dados["celid"]}";
                                    }
                                    if ( !empty($dados["sidid"]) ){
                                        $sqlSSPainelAcoes .= " AND s.sidid = {$dados["sidid"]}";
                                    }

                                    $sqlSSPainelAcoes .= "AND os.tosid    = ". TIPO_OS_GERAL ."
                                    AND esd.tpdid   = ". WORKFLOW_ORDEM_SERVICO  ." 
                                    GROUP BY esd.tpdid, esd.esdid, esd.esddsc)
                                    ORDER BY situacao asc
                                    ";
                
         $constantesEstadoDetalhamento = array(
            WF_ESTADO_DETALHAMENTO
         );
         $constantesEstadoOsExecucao = array(
            WF_ESTADO_OS_EXECUCAO
         );
         $constanteEstadoOsPausa = array(
             WF_ESTADO_OS_PAUSA
         );
         
         $sqlSSPainelAcompanhamento = "(SELECT DISTINCT '<a id=\"". WF_ESTADO_DETALHAMENTO ."\" class=\"painelGerenteProjetosSS link\" >'
                                            || 
                                            '<img title=\"Ação\" src=\"../imagens/icone_lupa.png\" /></a>' as acao
                                            , esd.tpdid, esd.esdid
                                            ,(CASE
                                                    WHEN esd.esdid = ". WF_ESTADO_DETALHAMENTO ."   THEN 'SS Em Detalhamento' 
                                                END) as esddsc
                                            , COUNT ( ss.scsid ) as total
                                            , (CASE
                                                    WHEN esd.esdid = ". WF_ESTADO_DETALHAMENTO ."   THEN 1 
                                                END) as situacao
                                    FROM workflow.estadodocumento esd
                                    INNER JOIN workflow.documento doc
                                        ON esd.esdid = doc.esdid
                                    INNER JOIN fabrica.solicitacaoservico ss
                                        ON doc.docid = ss.docid     
                                        
                                    LEFT JOIN demandas.sistemadetalhe s
                                        ON s.sidid = ss.sidid
                                    LEFT JOIN demandas.sistemacelula c
                                        ON s.sidid = c.sidid    

                                    WHERE esd.esdid IN ( ". implode( ',', $constantesEstadoDetalhamento ) ." ) ";
                                        
                                    if ( !empty($dados["celid"]) ){
                                        $sqlSSPainelAcompanhamento .= " AND c.celid = {$dados["celid"]}";
                                    }
                                    if ( !empty($dados["sidid"]) ){
                                        $sqlSSPainelAcompanhamento .= " AND s.sidid = {$dados["sidid"]}";
                                    }
         
                                   $sqlSSPainelAcompanhamento .=" AND esd.tpdid = ". WORKFLOW_SOLICITACAO_SERVICO ."
                                        AND ss.scsstatus = 'A' 
                                    GROUP BY esd.tpdid, esd.esdid, esd.esddsc)
        
                                    UNION
        
                                    (SELECT DISTINCT  '<a id=\"". WF_ESTADO_OS_EXECUCAO ."\" class=\"painelGerenteProjetosSS link\" >'
                                      || '<img title=\"Ação\" src=\"../imagens/icone_lupa.png\" /></a>'as acao, esd.tpdid, esd.esdid
                                             ,(CASE
                                                    WHEN esd.esdid = ". WF_ESTADO_OS_EXECUCAO ."  THEN 'OS Em Execução' 
                                                END) as esddsc
                                            , COUNT ( os.odsid ) as total
                                             , (CASE
                                                    WHEN esd.esdid = ". WF_ESTADO_OS_EXECUCAO ." THEN 2
                                                END) as situacao
FROM fabrica.ordemservico os 
LEFT JOIN ( select odsidpai, count(odsid) as contador 
		from fabrica.ordemservico group by odsidpai ) osp ON osp.odsidpai = os.odsid 
LEFT JOIN fabrica.tipoordemservico tos ON tos.tosid=os.tosid 
LEFT JOIN fabrica.solicitacaoservico ss ON ss.scsid=os.scsid 
LEFT JOIN workflow.documento d ON d.docid=os.docid 
LEFT JOIN workflow.estadodocumento esd ON esd.esdid=d.esdid 
LEFT JOIN workflow.documento d2 ON d2.docid=os.docidpf 
LEFT JOIN workflow.estadodocumento ed2 ON ed2.esdid=d2.esdid 
LEFT JOIN demandas.sistemadetalhe sid ON sid.sidid=ss.sidid 
LEFT JOIN workflow.documento as wkd on wkd.docid = ss.docid 
LEFT JOIN fabrica.analisesolicitacao as fas on fas.scsid=ss.scsid 

                                    LEFT JOIN demandas.sistemadetalhe s
                                        ON s.sidid = ss.sidid
                                    LEFT JOIN demandas.sistemacelula c
                                        ON s.sidid = c.sidid  

WHERE tos.tosstatus = 'A' AND (
 (ed2.esdid in (". implode( ',', $constantesEstadoOsExecucao ) .") and tos.tosid in (2,3)) 
 OR (esd.esdid in (". implode( ',', $constantesEstadoOsExecucao ) .") and tos.tosid in (1)) )";



                                     
        
                                    if ( !empty($dados['celid']) ){
                                        $sqlSSPainelAcompanhamento .= " AND c.celid = {$dados['celid']}";
                                    }
                                    if ( !empty($dados['sidid']) ){
                                        $sqlSSPainelAcompanhamento .= " AND s.sidid = {$dados['sidid']}";
                                    }

                                    $sqlSSPainelAcompanhamento .="

                                    AND os.tosid    = ". TIPO_OS_GERAL ."
                                    AND esd.tpdid   = ". WORKFLOW_ORDEM_SERVICO  ." 
                                    GROUP BY esd.tpdid, esd.esdid, esd.esddsc)
                                   
                                    UNION
                                    
                                    (SELECT DISTINCT  '<a id=\"". WF_ESTADO_OS_PAUSA ."\" class=\"painelGerenteProjetosSS link\" >'
                                      || '<img title=\"Ação\" src=\"../imagens/icone_lupa.png\" /></a>'as acao, esd.tpdid, esd.esdid
                                             ,(CASE
                                                     WHEN esd.esdid = ". WF_ESTADO_OS_PAUSA ."  THEN 'OS Pausadas'     
                                                END) as esddsc
                                            , COUNT ( os.odsid ) as total
                                             , (CASE
                                                     WHEN esd.esdid = ". WF_ESTADO_OS_PAUSA ." THEN 3    
                                                END) as situacao
FROM fabrica.ordemservico os 
LEFT JOIN ( select odsidpai, count(odsid) as contador 
		from fabrica.ordemservico group by odsidpai ) osp ON osp.odsidpai = os.odsid 
LEFT JOIN fabrica.tipoordemservico tos ON tos.tosid=os.tosid 
LEFT JOIN fabrica.solicitacaoservico ss ON ss.scsid=os.scsid 
LEFT JOIN workflow.documento d ON d.docid=os.docid 
LEFT JOIN workflow.estadodocumento esd ON esd.esdid=d.esdid 
LEFT JOIN workflow.documento d2 ON d2.docid=os.docidpf 
LEFT JOIN workflow.estadodocumento ed2 ON ed2.esdid=d2.esdid 
LEFT JOIN demandas.sistemadetalhe sid ON sid.sidid=ss.sidid 
LEFT JOIN workflow.documento as wkd on wkd.docid = ss.docid 
LEFT JOIN fabrica.analisesolicitacao as fas on fas.scsid=ss.scsid 

                                    LEFT JOIN demandas.sistemadetalhe s
                                        ON s.sidid = ss.sidid
                                    LEFT JOIN demandas.sistemacelula c
                                        ON s.sidid = c.sidid  
                                        
WHERE tos.tosstatus = 'A' AND (
 (ed2.esdid in (". implode( ',', $constanteEstadoOsPausa ) .") and tos.tosid in (2,3)) OR (esd.esdid in (". implode( ',', $constanteEstadoOsPausa ) .") and tos.tosid in (1)) ) 
";
                                     
        
                                    if ( !empty($dados['celid']) ){
                                        $sqlSSPainelAcompanhamento .= " AND c.celid = {$dados['celid']}";
                                    }
                                    if ( !empty($dados['sidid']) ){
                                        $sqlSSPainelAcompanhamento .= " AND s.sidid = {$dados['sidid']}";
                                    }
                                        

                                    $sqlSSPainelAcompanhamento .="

                                    AND os.tosid    = ". TIPO_OS_GERAL ."
                                    AND esd.tpdid   = ". WORKFLOW_ORDEM_SERVICO  ." 
                                    GROUP BY esd.tpdid, esd.esdid, esd.esddsc)
                                    ORDER BY situacao asc
                                    ";
                                    
                                  //echo'<pre>';exit($sqlSSPainelAcompanhamento);
                                    
        $dadosPainelAcoes  = $this->_db->carregar( $sqlSSPainelAcoes );
        $subGridSS      = $this->_montaGridPainelOperacional('Ações', $dadosPainelAcoes, '');
        
        $dadosPainelAcompanhamento  = $this->_db->carregar( $sqlSSPainelAcompanhamento );
        $subGridOS      = $this->_montaGridPainelOperacional('Acompanhamento dos Serviços', $dadosPainelAcompanhamento, '');

        $gridPainel = '<table class="listagem">';
        //$gridPainel .= '<tr><td colspan="2" class="TituloTabela center">Situação OS/SS </td>';
        $gridPainel .= '</tr>';
        $gridPainel .= $subGridSS;
        $gridPainel .= $subGridOS;
        $gridPainel .= '</table>';
        
        return $gridPainel;
    }
    
    
    public function painelOperacionalEmpresaItem2()
    {
        $sqlOSPainelOperacional = "SELECT '<a id=\"'|| esd.esdid ||'\" class=\"estadoOrdemServico link\" >'
                                            || 
                                            CASE esd.esdid
                                                WHEN ". WF_ESTADO_CPF_AGUARDANDO_PAGAMENTO ."  THEN '<img title=\"Visualizar\" src=\"../imagens/icone_lupa.png\" /></a>' 
                                                WHEN ". WF_ESTADO_CPF_DIVERGENCIA ."  THEN '<img title=\"Visualizar\" src=\"../imagens/icone_lupa.png\" /></a>' 
                                                ELSE '<img title=\"Ação\" src=\"../imagens/editar_nome.gif\" /></a>' 
                                            END as acao
                                            , esd.tpdid, esd.esdid, esd.esddsc
                                            , COUNT ( os.odsid ) as total
                                            , '' as tipo	  
                                            , (CASE
                                                    WHEN esd.esdid = ". WF_ESTADO_CPF_AGUARDANDO_PAGAMENTO ."               THEN 3 
                                                    WHEN esd.esdid = ". WF_ESTADO_CPF_DIVERGENCIA ."             THEN 4 
                                                END) as situacao
                                        FROM workflow.estadodocumento esd
                                        LEFT JOIN workflow.documento doc
                                            ON esd.esdid = doc.esdid 
                                        LEFT JOIN fabrica.ordemservico os
                                            ON os.docidpf = doc.docid
                                            AND os.tosid    IN (". TIPO_OS_CONTAGEM_ESTIMADA .", ". TIPO_OS_CONTAGEM_DETALHADA ." )
                                        LEFT JOIN fabrica.tipoordemservico tos
                                            ON os.tosid = tos.tosid	
                                        WHERE esd.esdid IN ( ". WF_ESTADO_CPF_DIVERGENCIA .", ". WF_ESTADO_CPF_AGUARDANDO_PAGAMENTO ." )                                        
                                        AND esd.tpdid   = ". WORKFLOW_CONTAGEM_PF ." 
                                        GROUP BY esd.tpdid, esd.esdid, esd.esddsc

                                        UNION 

                                        SELECT '<a id=\"'|| esd.esdid ||'_".TIPO_OS_CONTAGEM_DETALHADA."\" class=\"estadoOrdemServico link\"  >'
                                            || '<img title=\"Ação\" src=\"../imagens/editar_nome.gif\" /></a>' as acao
                                            , esd.tpdid, esd.esdid, esd.esddsc
                                            , COUNT ( os.odsid ) as total	
                                            , 'Detalhada'	 as tipo  
                                            , 2 as situacao
                                        FROM workflow.estadodocumento esd
                                        LEFT JOIN workflow.documento doc
                                            ON esd.esdid = doc.esdid 
                                        LEFT JOIN fabrica.ordemservico os
                                            ON os.docidpf = doc.docid
                                            AND os.tosid    = ". TIPO_OS_CONTAGEM_DETALHADA ."
                                        LEFT JOIN fabrica.tipoordemservico tos
                                            ON os.tosid = tos.tosid	
                                        WHERE esd.esdid = ". WF_ESTADO_CPF_APROVACAO ."                                        
                                        AND esd.tpdid   = ". WORKFLOW_CONTAGEM_PF ." 
                                        GROUP BY esd.tpdid, esd.esdid, esd.esddsc

                                        UNION 

                                        SELECT '<a id=\"'|| esd.esdid ||'_".TIPO_OS_CONTAGEM_ESTIMADA."\" class=\"estadoOrdemServico link\" >'
                                            || '<img title=\"Ação\" src=\"../imagens/editar_nome.gif\" /></a>' as acao
                                            , esd.tpdid, esd.esdid, esd.esddsc
                                            , COUNT ( os.odsid ) as total	
                                            , 'Estimada' as tipo	  
                                            , 1 as situacao
                                        FROM workflow.estadodocumento esd
                                        LEFT JOIN workflow.documento doc
                                            ON esd.esdid = doc.esdid 
                                        LEFT JOIN fabrica.ordemservico os
                                            ON os.docidpf = doc.docid
                                            AND os.tosid    = ". TIPO_OS_CONTAGEM_ESTIMADA ."
                                        LEFT JOIN fabrica.tipoordemservico tos
                                            ON os.tosid = tos.tosid	
                                        WHERE esd.esdid = ". WF_ESTADO_CPF_APROVACAO ."                                        
                                        AND esd.tpdid   = ". WORKFLOW_CONTAGEM_PF ." 
                                        GROUP BY esd.tpdid, esd.esdid, esd.esddsc
                                        
                                        ORDER BY  7";
        
        
        $dadosPainelOS  = $this->_db->carregar( $sqlOSPainelOperacional );
        $subGridOS      = $this->_montaGridPainelOperacional('Ordem de Serviço', $dadosPainelOS, 'OS');
        
        $gridPainel = '<table class="listagem">';
        $gridPainel .= '<tr><td colspan="2" class="TituloTabela center">Situação OS </td>';
        $gridPainel .= '</tr>';
        $gridPainel .= $subGridSS;
        $gridPainel .= $subGridOS;
        $gridPainel .= '</table>';
        
        return $gridPainel;
    }
    
    
    
    
    /**
     * Monta uma sub-lista com dados informados
     * @param string $titulo
     * @param array $lista
     * @return string 
     */
    protected function _montaGridPainelOperacional( $titulo, $lista, $tipo )
    {
       $grid   = '';
       
        if (is_array($lista)){
        
        $totalRegistros = 0;
        
        
        $grid   .= "<tr><td class=\"TituloTabela center\">{$titulo}</td>";
        $grid   .= "<td class=\"TituloTabela center\">Qtd</td></tr>";
        
            foreach( $lista as $registroLista )
            {


                if( isset($registroLista['tipo']) && !empty($registroLista['tipo']) )
                    $registroLista['esddsc'] .= ' - '. $registroLista['tipo'];

                $linhaGrid = '<tr>';
                $linhaGrid .= "<td>{$registroLista['acao']} {$registroLista['esddsc']}</td>";
                $linhaGrid .= "<td class=\"center\">{$registroLista['total']}</td>";
                $linhaGrid .= '</tr>';

                $totalRegistros += $registroLista['total'];

                $grid .= $linhaGrid;
            }


            if( $tipo == 'SS' )
                $total = "Total SS: ";
            elseif( $tipo == 'OS' )
                $total = "Total OS: ";
            else
                $total = "Total: ";
        
        
        
            $grid   .= "<tr><td><strong>{$total}</strong></td>";
            $grid .= "<td class=\"center\"><strong>{$totalRegistros}</strong></td></tr>";

            
        }else{
            $grid   .= "<tr><td colspan=\"2\" class=\"TituloTabela center\">{$titulo}</td></tr>";
            $grid   .= "<tr><td colspan=\"2\" class=\"center\">Nenhum registro encontrado.</td></tr>";
            $grid   .= "<tr><td colspan=\"2\"><br /></td></tr>";
        }
            
            return $grid;
        
    }
    
    protected function listarRealizarPreAnalise($dados)
    {
        
        $esdId      = (int) $dados["esdId"];
        
        $sqlSSEmAnalise = "
                            SELECT
                                '<a id=\"\" class=\"link\" title=\"Abrir Solicitação de Serviço\" href=\"fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='|| ss.scsid ||'&ansid='|| ans.ansid ||' \">'|| ss.scsid ||'</a>' as ss
                                ,  sid.siddescricao
                                , usu.usunome
                                , ss.scsnecessidade
                                , '<a id=\"\" class=\"link\" title=\"Abrir Solicitação de Serviço\" href=\"fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='|| ss.scsid ||' \">Enviar para Análise</a>'
                            FROM fabrica.solicitacaoservico ss
                            INNER JOIN workflow.documento doc
                                ON ss.docid = doc.docid
                            INNER JOIN seguranca.usuario usu
                                ON ss.usucpfrequisitante = usu.usucpf
                            LEFT JOIN demandas.sistemadetalhe sid
                                ON ss.sidid = sid.sidid
                            LEFT JOIN demandas.sistemacelula c
                                ON sid.sidid = c.sidid         
                            LEFT JOIN fabrica.analisesolicitacao ans
                                ON ss.scsid = ans.scsid
                            WHERE ss.scsstatus = 'A' ";
        
                            if ( !empty($dados["celid"]) ){
                                $sqlSSEmAnalise .= " AND c.celid = {$dados["celid"]}";
                            }
                            if ( !empty($dados["sidid"]) ){
                                $sqlSSEmAnalise .= " AND sid.sidid = {$dados["sidid"]}";
                            }
        
                           $sqlSSEmAnalise .= "AND doc.esdid = ". WF_ESTADO_PRE_ANALISE;
        //echo'<pre>';exit($sqlSSEmAnalise);
        $cabecalho     = array("SS","Sistema","Requisitante","Necessidade", "Ação");
        $tamanho       = array("5%","15%","20%","50%", '10%');
        $alinhamento   = array("center","left","left","left", 'center');

        ob_start();
        //exit($sqlSSEmAnalise);
        $this->_db->monta_lista( $sqlSSEmAnalise, $cabecalho,100,5,'N','center','',"",$tamanho,$alinhamento );
        $listaSS = ob_get_contents();
        ob_end_clean();
        
        return $listaSS;
    }
    
    
    protected function listarRealizarAprovarExecucaodeServico($dados)
    {
        $esdId      = (int) $dados["esdId"];
        
        $sqlSSEmAnalise = "
                        SELECT distinct 
                               '<a id=\"". WF_ESTADO_APROVACAO ."\" class=\"painelGerenteProjetosSS link\" >'
                                  || '<input class=\"checkedGrid\" value=\"'|| fos.odsid ||'\" id=\"odsid\" name=\"odsid\" type=\"radio\" title=\"Ação\" src=\"../imagens/editar_nome.gif\" /></a>' as acaoSS,
                               '<a class=\"link\" title=\"Abrir Solicitação de Serviço\" href=\"fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='|| ss.scsid ||'&ansid='|| ans.ansid ||' \">'|| ss.scsid ||'</a>' as scsid,
                                sid.sidabrev,
                                to_char(ans.ansprevinicio,'dd/mm/YYYY') as ansprevinicio,
                                to_char(ans.ansdtrecebimento,'dd/mm/YYYY') as ansdtrecebimento,
                                
CASE WHEN (SELECT hpt.prevtermino 
						FROM fabrica.observacoes obs 
						INNER JOIN fabrica.historicoprevisaotermino hpt on hpt.obsid = obs.obsid
						WHERE obs.odsid = fos.odsid 
						AND prevtermino IS NOT NULL
						ORDER by hpt.hptid desc
						LIMIT 1
					)  is not null then
            (SELECT to_char(hpt.prevtermino, 'dd/mm/YYYY')
            FROM fabrica.observacoes obs 
            INNER JOIN fabrica.historicoprevisaotermino hpt on hpt.obsid = obs.obsid
            WHERE obs.odsid = fos.odsid 
                AND prevtermino IS NOT NULL
                ORDER by hpt.hptid desc
                LIMIT 1)
ELSE
    to_char(ans.ansprevtermino, 'dd/mm/YYYY')
END as ansprevtermino,

                                ( CASE
                                    WHEN ans.mensuravel = 't' THEN 'Sim'
                                    ELSE 'Não'
                                  END ) AS mensuravel,
                                fos.odsqtdpfestimada,
                                '<a id=\"\" class=\"link\" title=\"Abrir Solicitação de Serviço\" href=\"fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='|| ss.scsid ||' \">Aprovar/Reprovar</a>'
                                FROM 
                                    fabrica.solicitacaoservico ss
                                INNER JOIN fabrica.ordemservico fos
                                    ON fos.scsid = ss.scsid    
                                INNER JOIN workflow.documento doc
                                    ON ss.docid = doc.docid
                                INNER JOIN seguranca.usuario usu
                                    ON ss.usucpfrequisitante = usu.usucpf
                                    
                                LEFT JOIN demandas.sistemadetalhe sid
                                    ON ss.sidid = sid.sidid
                                LEFT JOIN demandas.sistemacelula c
                                    ON sid.sidid = c.sidid         
                                LEFT JOIN fabrica.analisesolicitacao ans
                                    ON ss.scsid = ans.scsid

                                WHERE ss.scsstatus = 'A' ";
        
                            if ( !empty($dados["celid"]) ){
                                $sqlSSEmAnalise .= " AND c.celid = {$dados["celid"]}";
                            }
                            if ( !empty($dados["sidid"]) ){
                                $sqlSSEmAnalise .= " AND sid.sidid = {$dados["sidid"]}";
                            }
                           $sqlSSEmAnalise .= "AND doc.esdid = ". WF_ESTADO_APROVACAO;
                           
        //echo '<pre>';die($sqlSSEmAnalise);
        
        $cabecalho     = array("Ação", "SS","Sigla Sistema","Expect. Atend.","Data de Abertura", "Previsão de Término", "Mensurável", "Qtd. PF Est. Empresa 1","Ação");
        $tamanho       = array("5%","5%","15%","15%","15%", '18%', '10%','10%','15%');
        $alinhamento   = array("center","center","left","left","left", "center","center","right","center");

        
        //exit($sqlSSEmAnalise);
        
        ob_start();
        $this->_db->monta_lista( $sqlSSEmAnalise,$cabecalho,100,5,'N','center','',"",$tamanho,$alinhamento );
        $listaSS = ob_get_contents();
        ob_end_clean();
        
        $listaSS_Botao = "<input type=\"button\" value=\"Replanejar\" id= \"replanejar\">";
        
        return $listaSS . $listaSS_Botao;
    }
    
    protected function listarHomologarOrdemDeServico($dados)
    {
         
        $esdId      = (int) $dados["esdId"];
        
        // ADCIONAR ID OU VALUE NO RADIO 
        $sqlHomologarOrdemDeServico = "SELECT
                                      '<a class=\"link\" title=\"Abrir Solicitação de Serviço\" href=\"fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='|| os.scsid ||'&ansid='|| ans.ansid ||' \">'|| os.scsid ||'</a>' as ss
                                      , '<a class=\"link generica\" title=\"Abrir Ordem de Serviço\" href=\"javascript:void(0);\" onclick=\"window.open(\'fabrica.php?modulo=principal/verDadosOs&acao=A&odsid='||os.odsid||'\',\'OS\',\'scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no\');\" >'|| os.odsid ||'</a>' as os
                                      , to_char(os.odsdtprevinicio,    'DD/MM/YYYY') as odsdtprevinicio
                                      , to_char(ss.scsprevatendimento, 'DD/MM/YYYY') as scsprevatendimento 
                                      , to_char(entrega.data_entrega, 'DD/MM/YYYY' ) as data_entrega
                                      , CASE entrega.data_entrega <= fabrica.fn_data_previsao_termino_os(os.odsid)
                                          WHEN 'f' THEN 'OS EM DIA'
                                          ELSE '<span style=\" color:red; \">OS EM ATRASO</span>'
                                          END as status
                                      , os.pf_detalhado as pf_detalhado
                                      , fabrica.fn_porcentagem_disciplina_ss( os.scsid ) || '%' as Disciplinas  
                                      , os.pf_a_pagar as pf_a_pagar
                                      , COALESCE(os.glosaqtdepf,0) || ' ' as glosaqtdepf 
                                      , ( (fabrica.fn_menor_valor_pf_os(os.odsid) * fabrica.fn_porcentagem_disciplina_ss(os.scsid)) / 100 ) - (COALESCE(os.glosaqtdepf,0)) as pf_liquido 
                                      , '<a class=\"link generica\" title=\"Verificar Produto\" href=\"fabrica.php?modulo=principal/gerencia-configuracao/listarArtefatos&acao=A&solicitacao='||os.scsid||'\" >Verificar Produto</a>' as verificar_produto
                                      , '<a class=\"link\" title=\"Abrir Solicitação de Serviço\" href=\"fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='|| os.scsid ||' \">Aprovar/Reprovar</a>' as aprovar
                                  FROM fabrica.vw_ordem_servico os
                                  INNER JOIN fabrica.solicitacaoservico ss
                                      ON os.scsid = ss.scsid
                                  LEFT JOIN fabrica.analisesolicitacao ans
                                      ON ss.scsid = ans.scsid
                                  LEFT JOIN demandas.sistemadetalhe sid
                                      ON ss.sidid = sid.sidid
                                  LEFT JOIN demandas.sistemacelula c
                                      ON sid.sidid = c.sidid           
                                  LEFT JOIN fabrica.fn_entrega_ordem_servico() entrega
                                      ON os.odsid = entrega.odsid  
                                  WHERE os.esdid = ". WF_ESTADO_OS_APROVACAO;

                                  if ( !empty($dados["celid"]) ){
                                      $sqlHomologarOrdemDeServico .= " AND c.celid = {$dados["celid"]}";
                                  }
                                  if ( !empty($dados["sidid"]) ){
                                      $sqlHomologarOrdemDeServico .= " AND sid.sidid = {$dados["sidid"]}";   
                                  }    
                                  $sqlHomologarOrdemDeServico.=" AND os.tosid 	= ". TIPO_OS_GERAL ;
        
        //exit($sqlHomologarOrdemDeServico);
        $cabecalho     = array("SS","OS","Previsão Início", "Previsão Término", "Dt. Entrega", "Status", "Qtd.PF detalhada Empresa1","Porcentagens das Disciplinas","PF a Pagar", "Glosa(PF)", "PF a Pagar Após Cálculo da Glosa","GC","Ação");
        $tamanho       = array("5%","5%","8%","9%", '8%', '8%', '8%', '8%', '8%', '8%', '8%', '20%', '10%');
        $alinhamento   = array("center","center","center","center", 'center', 'center', 'right', 'right', 'right', 'right', 'right', 'center', 'center');

        ob_start();
        $this->_db->monta_lista( $sqlHomologarOrdemDeServico, $cabecalho, 100,5, 'N', 'center', '', "", $tamanho, $alinhamento );
        $listaSS = ob_get_contents();
        ob_end_clean();
        
        $listaSSTes = "<input type=\"button\" value=\"Replanejar\" id= \"replanejar\">
                        &nbsp;&nbsp;&nbsp;&nbsp;
                        <input type=\"button\" value =\"Pausar\" id= \"pausar\">";
        
        
        return $listaSS;
    }
    
    
    protected function listarOrdemDeServicoEmPausa()
    {
        
        $sqlOrdemDeServicoEmPausa = "SELECT DISTINCT
                                    '<a id=\"\" class=\"link\" title=\"Abrir Solicitação de Serviço\" href=\"fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='|| os.scsid ||'&ansid='|| ans.ansid ||' \">'|| os.scsid ||'</a>' as ss
                                    , '<a id=\"'|| os.odsid ||'\" class=\"link generica\" title=\"Abrir Ordem de Serviço\" href=\"javascript:void(0);\" onclick=\"window.open(\'fabrica.php?modulo=principal/verDadosOs&acao=A&odsid='||os.odsid||'\',\'OS\',\'scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no\');\" >'|| os.odsid ||'</a>' as os
                                    , sid.sidabrev
                                    , ss.scsnecessidade
                                    , to_char( fabrica.fn_data_previsao_termino_os(os.odsid), 'DD/MM/YYYY' ) as odsdtprevtermino
                                    , to_char( pausa.data_entrada_pausa, 'DD/MM/YYYY' ) as data_entrada_pausa
                                    , to_char( ( current_date - pausa.data_entrada_pausa), 'DD') || '&nbsp;' as dias_em_pausa
                                    , pausa.cmddsc as motivo_pausa
                                FROM fabrica.vw_ordem_servico os
                                INNER JOIN fabrica.solicitacaoservico ss
                                    ON os.scsid = ss.scsid
                                LEFT JOIN fabrica.analisesolicitacao ans
                                    ON os.scsid = ans.scsid
                                LEFT JOIN fabrica.tiposervico tps
                                    ON ans.tpsid = tps.tpsid
                                LEFT JOIN fabrica.tipodespesa tpdps
                                    ON tps.tpdpsid = tpdps.tpdpsid
                                LEFT JOIN demandas.sistemadetalhe sid
                                    ON ss.sidid = sid.sidid 
                                LEFT JOIN demandas.sistemacelula c
                                      ON sid.sidid = c.sidid        
                                 INNER JOIN 
                                        (
                                        SELECT  os.odsid, MAX(hd.htddata) as data_entrada_pausa, MAX(cmd.cmddsc) as cmddsc
                                        FROM fabrica.ordemservico os
                                        INNER JOIN workflow.documento dc
                                            ON dc.docid = os.docid
                                        INNER JOIN workflow.historicodocumento hd
                                            ON dc.docid = hd.docid
                                        LEFT JOIN workflow.comentariodocumento cmd
                                            ON hd.hstid = cmd.hstid
                                        INNER JOIN workflow.acaoestadodoc a
                                            ON a.aedid  = hd.aedid
                                        INNER JOIN workflow.estadodocumento ed
                                            ON ed.esdid = dc.esdid
                                        WHERE a.esdiddestino    = ". WF_ESTADO_OS_PAUSA ."
                                        AND ed.tpdid            = ". WORKFLOW_ORDEM_SERVICO ."
                                        GROUP BY os.odsid
                                        ) as pausa   
                                        ON os.odsid = pausa.odsid
                                WHERE os.esdid = ". WF_ESTADO_OS_PAUSA;
                                    
                                if ( !empty($dados["celid"]) ){
                                    $sqlOrdemDeServicoEmPausa .= " AND c.celid = {$dados["celid"]}";
                                }
                                if ( !empty($dados["sidid"]) ){
                                    $sqlOrdemDeServicoEmPausa .= " AND sid.sidid = {$dados["sidid"]}";   
                                }   

                                $sqlOrdemDeServicoEmPausa .= " AND os.tosid 	= ". TIPO_OS_GERAL ;
        
        $cabecalho     = array("SS","OS","Sigla Sistema", "Necessidade", "Previsão Término", "Pausa Em", "Dias em Pausa", "Motivo da Pausa");
        $tamanho       = array("5%","5%","10%","20%", '10%', '5%', '5%', '20%');
        $alinhamento   = array("center","center","left","left", 'center', 'center', 'center', 'left');

        ob_start();
        $this->_db->monta_lista( $sqlOrdemDeServicoEmPausa, $cabecalho, 100,5, 'N', 'center', '', "", $tamanho, $alinhamento );
        $listaSS = ob_get_contents();
        ob_end_clean();
        
        return $listaSS;
    }
    
    protected function listarOrdemDeServicoExecucao()
    {
        $sqlOrdemDeServicoExecucao = "
                                    SELECT distinct
                                    '<a id=\"". WF_ESTADO_OS_EXECUCAO ."\" class=\"painelGerenteProjetosSS link\" >'
                                      || '<input class=\"checkedGrid\" value=\"'|| os.odsid ||'\" id=\"odsid\" name=\"odsid\" type=\"radio\" title=\"Ação\" src=\"../imagens/editar_nome.gif\" /></a>' as acaoSS
                                      
                , '<a class=\"link\" title=\"Abrir Solicitação de Serviço\" href=\"fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='|| os.scsid ||'&ansid='|| ans.ansid ||' \">'|| os.scsid ||'</a>' as ss
                , '<a id=\"'|| os.odsid ||'\" class=\"link generica\" title=\"Abrir Ordem de Serviço\" href=\"javascript:void(0);\" onclick=\"window.open(\'fabrica.php?modulo=principal/verDadosOs&acao=A&odsid='||os.odsid||'\',\'OS\',\'scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no\');\" >'|| os.odsid ||'</a>' as os
                , sid.sidabrev
                , to_char( os.odsdtprevinicio, 'DD/MM/YYYY' ) as odsdtprevinicio
                , to_char( fabrica.fn_data_previsao_termino_os(os.odsid), 'DD/MM/YYYY' ) as odsdtprevtermino
                , CASE entrega.data_entrega <= fabrica.fn_data_previsao_termino_os(os.odsid)
                  WHEN TRUE THEN 
                      'OS EM DIA'
                  ELSE 
                      --'<span style=\" color:red; \">OS EM ATRASO</span>'
                      'OS EM ATRASO'
                  END as status
                , os.pf_detalhado as pf_detalhado
                , fabrica.fn_porcentagem_disciplina_ss( os.scsid ) || '%' as disciplinas
                FROM fabrica.vw_ordem_servico os
                INNER JOIN fabrica.solicitacaoservico ss     ON os.scsid = ss.scsid
                LEFT JOIN fabrica.analisesolicitacao ans     ON os.scsid = ans.scsid
                LEFT JOIN fabrica.fn_entrega_ordem_servico() entrega ON os.odsid = entrega.odsid
                LEFT JOIN fabrica.tiposervico tps           ON ans.tpsid = tps.tpsid
                LEFT JOIN fabrica.tipodespesa tpdps         ON tps.tpdpsid = tpdps.tpdpsid
                LEFT JOIN demandas.sistemadetalhe sid       ON ss.sidid = sid.sidid
                LEFT JOIN demandas.sistemacelula c          ON sid.sidid = c.sidid
                WHERE os.esdid = ". WF_ESTADO_OS_EXECUCAO;
    
        if ( !empty($dados["celid"]) ){
            $sqlOrdemDeServicoExecucao .= " AND c.celid = {$dados["celid"]}";
        }
        if ( !empty($dados["sidid"]) ){
            $sqlOrdemDeServicoExecucao .= " AND sid.sidid = {$dados["sidid"]}";
        }
    
        $sqlOrdemDeServicoExecucao .= " AND os.tosid 	= ". TIPO_OS_GERAL ;
        
        //die($sqlOrdemDeServicoExecucao);
    
        $cabecalho     = array("Ação","SS","OS","Sistema", "Previsão Início", "Previsão Término", "Status", "Qtd. PF Detalhada Empresa 1", "Porcentagem das Disciplinas");
        $tamanho       = array("5%","5%","5%","50%","10%", '10%', '10%', '5%', '5%');
        $alinhamento   = array("center","center","center","left","center", 'center', 'center', 'right', 'right');
    
        ob_start();
        $this->_db->monta_lista( $sqlOrdemDeServicoExecucao, $cabecalho, 100,5, 'N', 'center', '', "", $tamanho, $alinhamento );
        
        $listaSS = ob_get_contents();
        ob_end_clean();
    
        $listaSSTes = "<input type=\"button\" value=\"Replanejar\" id= \"replanejar\">
                        &nbsp;&nbsp;&nbsp;&nbsp;
                        <input type=\"button\" value =\"Pausar\" id= \"pausar\">";
        
        return $listaSS . $listaSSTes;
    }
    
    
    protected function listarSsEmDetalhamento($dados)
    {
        
        $esdId      = (int) $dados["esdId"];
        
        $sqlSsEmDetalhamento = "
                            SELECT distinct
                                    '<a id=\"". WF_ESTADO_DETALHAMENTO ."\" class=\"painelGerenteProjetosSS link\" >'
                                      || '<input class=\"checkedGrid\" value=\"'|| ss.scsid ||'\" id=\"scsid\" name=\"scsid\" type=\"radio\" title=\"Ação\" src=\"../imagens/editar_nome.gif\" /></a>' as acaoSS
                                    , '<a class=\"link\" title=\"Abrir Solicitação de Serviço\" href=\"fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid='|| ss.scsid ||' \">'|| ss.scsid ||'</a>' as ss
                                    , sid.sidabrev
                                    , to_char( ss.scsprevatendimento, 'DD/MM/YYYY' ) as scsprevatendimento
                                    , to_char( (select whd.htddata from workflow.historicodocumento whd
                                            	inner join workflow.acaoestadodoc waed on waed.aedid = whd.aedid
                                            	where whd.htddata is not null
                                            	    and whd.docid = doc.docid
                                            	    and waed.esdiddestino = ". WF_ESTADO_DETALHAMENTO ."
                                            	    order by whd.htddata desc
                                            	limit 1
                                            	), 'DD/MM/YYYY') as htddata
                                    , situacao.status
                                    , case when os.odsid is null then
                                        '-'
                                    else
                                        '<a class=\"link generica\" title=\"Abrir Ordem de Serviço\" href=\"javascript:void(0);\" onclick=\"window.open(\'fabrica.php?modulo=principal/verDadosOs&acao=A&odsid='||os.odsid||'\',\'OS\',\'scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no\');\" >'|| os.odsid ||'</a>'                                    
                                    end as odsid
                                    ,to_char( a.ansprevinicio,  'DD/MM/YYYY' ) as ansprevinicio
                                    ,to_char( a.ansprevtermino, 'DD/MM/YYYY' ) as ansprevtermino
                               FROM   
                                     fabrica.solicitacaoservico ss
                               INNER JOIN seguranca.usuario usu
                                       ON ss.usucpfrequisitante = usu.usucpf
                               INNER JOIN workflow.documento doc
                                       ON ss.docid = doc.docid
                               LEFT JOIN fabrica.ordemservico os
                                      ON os.scsid = ss.scsid
                               LEFT JOIN fabrica.analisesolicitacao a
                                        ON ss.scsid = a.scsid       
                               LEFT JOIN demandas.sistemadetalhe sid
                                      ON ss.sidid = sid.sidid
                               LEFT JOIN demandas.sistemacelula c
                                      ON sid.sidid = c.sidid
                               LEFT JOIN (SELECT a.scsid,
                                                CASE WHEN a.ansdtrecebimento is null THEN 
                                                        'Não iniciada'
                                                ELSE	
                                                        CASE WHEN b.odsid is null THEN
                                                                'Iniciada e OS não criada'
                                                        ELSE
                                                                'OS Criada'
                                                        END
                                                END AS status
                                        FROM fabrica.analisesolicitacao a
                                        LEFT JOIN fabrica.ordemservico b
                                                on a.scsid = b.scsid) situacao ON ss.scsid =  situacao.scsid  
                            WHERE  ss.scsstatus = 'A' ";
        
                            if ( !empty($dados["celid"]) ){
                                $sqlSsEmDetalhamento .= " AND c.celid = {$dados["celid"]}";
                            }
                            if ( !empty($dados["sidid"]) ){
                                $sqlSsEmDetalhamento .= " AND sid.sidid = {$dados["sidid"]}";
                            }
        
                           $sqlSsEmDetalhamento .= "AND doc.esdid = ". WF_ESTADO_DETALHAMENTO;
                           
        $cabecalho     = array("Ação", "SS"," Sigla Sistema","Expect.Atend.","Data de envio para Fábrica", "Status", "OS","Prev. de Início", "Prev. de Fim");
        $tamanho       = array("5%","5%","15%","20%","15%", '10%', '10%', '10%', '10%');
        $alinhamento   = array("center","center","left","center","center", 'center', 'center', 'center', 'center');

        ob_start();
        //echo'<pre>';exit($sqlSsEmDetalhamento);
        $this->_db->monta_lista( $sqlSsEmDetalhamento, $cabecalho, 100, 5, 'N', 'center','',"",$tamanho,$alinhamento );
        $listaSS = ob_get_contents();
        ob_end_clean();
        
        $listaSSTes = "<input type=\"button\" value=\"Replanejar\" id= \"replanejar\">
                        &nbsp;&nbsp;&nbsp;&nbsp;
                       <input type=\"button\" value =\"Pausar\" id= \"pausar_ss\">";
        
        return $listaSS . $listaSSTes;
    }
    
}