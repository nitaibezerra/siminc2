<?php

/**
 * Classe responsavel pela manipulacao dos dados da tabela de de solicitacao de servicos
 * @author Silas Matheus
 * @author rayner
 * @name SolicitacaoServico
 *
 */
class SolicitacaoServico extends ModeloFabrica
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "fabrica.solicitacaoservico";

    /**
     * Chave primaria
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "scsid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'scsid'              => null,
        'odsidorigem'        => null,
        'prgid'              => null,
        'prgcod'             => null,
        'prgano'             => null,
        'usucpfrequisitante' => null,
        'docid'              => null,
        'scsnecessidade'     => null,
        'scsjustificativa'   => null,
        'scsprevatendimento' => null,
        'scsstatus'          => null,
        'scsatesto'          => null,
        'scsdtatesto'        => null,
        'usucpforigem'       => null,
        'sidid'              => null,
        'dataabertura'       => null
    );

    public function setId( $id ) {
        $this->arAtributos['scsid'] = $id;
    }

    public function setIdOrdemServico( $idOrdemServico ) {
        $this->arAtributos['odsidorigem'] = $idOrdemServico;
    }

    public function setIdPrograma( $idPrograma ) {
        $this->arAtributos['prgid'] = $idPrograma;
    }

    public function setCodPrograma( $codPrograma ) {
        $this->arAtributos['prgcod'] = $codPrograma;
    }

    public function setAnoPrograma( $ano ) {
        $this->arAtributos['prgano'] = $ano;
    }

    public function setCpfRequisitante( $cpf ) {
        $this->arAtributos['usucpfrequisitante'] = $cpf;
    }

    public function setIdDocumento( $idDocumento ) {
        $this->arAtributos['docid'] = $idDocumento;
    }

    public function setNecessidade( $necessidade ) {
        $this->arAtributos['scsnecessidade'] = $necessidade;
    }

    public function setJustificativa( $justificativa ) {
        $this->arAtributos['scsjustificativa'] = $justificativa;
    }

    public function setPrevisaoAtendimento( $dataPrevisaoAtendimento ) {
        $this->arAtributos['scsprevatendimento'] = new DateTime( $dataPrevisaoAtendimento );
    }

    public function setStatus( $status ) {
        $this->arAtributos['scsstatus'] = $status;
    }

    public function setAtesto( $atesto ) {
        $this->arAtributos['scsatesto'] = $atesto;
    }

    public function setDataAtesto( $dataAtesto ) {
        $this->arAtributos['scsdtatesto'] = new DateTime( $dataAtesto );
    }

    public function setCpfUsuarioOrigem( $cpf ) {
        $this->arAtributos['usucpforigem'] = $cpf;
    }

    public function setIdSistema( $idSistema ) {
        $this->arAtributos['sidid'] = $idSistema;
    }

    public function setDataAbertura( $dataAbertura ) {
        $this->arAtributos['dataabertura'] = new DateTime( $dataAbertura );
    }

    public function getId() {
        return $this->arAtributos['scsid'];
    }

    public function getIdOrdemServico() {
        return $this->arAtributos['odsidorigem'];
    }

    public function getIdPrograma() {
        return $this->arAtributos['prgid'];
    }

    public function getCodPrograma() {
        return $this->arAtributos['prgcod'];
    }

    public function getAnoPrograma() {
        return $this->arAtributos['prgano'];
    }

    public function getCpfRequisitante() {
        return $this->arAtributos['usucpfrequisitante'];
    }

    public function getIdDocumento() {
        return $this->arAtributos['docid'];
    }

    public function getNecessidade() {
        return $this->arAtributos['scsnecessidade'];
    }

    public function getJustificativa() {
        return $this->arAtributos['scsjustificativa'];
    }

    public function getPrevisaoAtendimento() {
        return new DateTime( $this->arAtributos['scsprevatendimento'] );
    }

    public function getStatus() {
        return $this->arAtributos['scsstatus'];
    }

    public function getAtesto() {
        return $this->arAtributos['scsatesto'];
    }

    public function getDataAtesto() {
        return new DateTime( $this->arAtributos['scsdtatesto'] );
    }

    public function getCpfUsuarioOrigem() {
        return $this->arAtributos['usucpforigem'];
    }

    public function getIdSistema() {
        return $this->arAtributos['sidid'];
    }

    public function getDataAbertura() {
        return new DateTime( $this->arAtributos['dataabertura'] );
    }

    protected $stCampos = "ss.scsid, ss.odsidorigem, ss.prgid, ss.prgcod, ss.prgano,
		ss.usucpfrequisitante, ss.docid, ss.scsnecessidade, ss.scsjustificativa, ss.scsprevatendimento, 
		ss.scsstatus, ss.scsatesto, ss.scsdtatesto, ss.usucpforigem, ss.sidid, ss.dataabertura";

    public function recupereTodasSSPorStatus( $statusSolicitacaoServico ) {
        $sql = "SELECT * FROM fabrica.solicitacaoservico ss
					join workflow.documento d
						on ss.docid = d.docid
					where d.esdid = $statusSolicitacaoServico order by scsid";

        $resultSet = $this->carregar( $sql );
        foreach ( $resultSet as $linha ) {
            $listaDeSolicitacaoServico[] = $this->montaObjeto( $linha );
        }
        return $listaDeSolicitacaoServico;
    }

    private function montaObjeto( $linha ) {
        $ss = new SolicitacaoServico();
        $ss->setId( $linha['scsid'] );
        $ss->setIdOrdemServico( $linha['odsidorigem'] );
        $ss->setIdPrograma( $linha['prgid'] );
        $ss->setCodPrograma( $linha['prgcod'] );
        $ss->setAnoPrograma( $linha['prgano'] );
        $ss->setCpfRequisitante( $linha['usucpfrequisitante'] );
        $ss->setIdDocumento( $linha['docid'] );
        $ss->setNecessidade( $linha['scsnecessidade'] );
        $ss->setJustificativa( $linha['scsjustificativa'] );
        $ss->setPrevisaoAtendimento( $linha['scsprevatendimento'] );
        $ss->setStatus( $linha['scsstatus'] );
        $ss->setAtesto( $linha['scsatesto'] );
        $ss->setDataAtesto( $linha['scsdtatesto'] );
        $ss->setCpfUsuarioOrigem( $linha['usucpforigem'] );
        $ss->setIdSistema( $linha['sidid'] );
        $ss->setDataAbertura( $linha['dataabertura'] );
        return $ss;
    }

    /**
     * Recupera a os do tipo geral de uma determinada SS
     * 
     * @todo adicionar os campos a medida de necessidade
     * @param int $scsid
     * @return array
     */
    public function recuperarOSTipoGeral( $scsid ) {
        $sql = "SELECT ss.scsid, os.odsid, os.docid, os.tosid
                    FROM fabrica.solicitacaoservico ss
                    INNER JOIN fabrica.ordemservico os
                        ON ss.scsid = os.scsid
                    WHERE os.tosid = 1
                    AND ss.scsid = {$scsid}";

        return $this->pegaLinha( $sql );
    }
    
    /**
     * Verifica se todas as ordem de serviço
     * vinculadas há uma SS foram emitidas em algum memorando
     * 
     * @param int $scsid - Código da SS
     * @return bool 
     */
    
    public function possuiTodosAsOSEmitidasEmMemorando( $scsid )
    {
        $sql = "SELECT ss.scsid, os_geral_emitidas.total_emitida_geral
                , os_estimada_detalhada_emitidas.total_emitida_estimada_detalhada
                                    , COUNT ( os.odsid ) as total_os
                FROM fabrica.solicitacaoservico ss
                INNER JOIN fabrica.ordemservico os
                    ON ss.scsid = os.scsid	
                LEFT JOIN 
                    (
                    SELECT os.scsid, COUNT (  os.odsid ) as total_emitida_geral
                    FROM fabrica.ordemservico os 
                    LEFT JOIN fabrica.memorando memo
                        ON os.memoid = memo.memoid
                        AND memo.memostatus = 'IMPR'
                    INNER JOIN workflow.documento doc
                        ON os.docid = doc.docid
                    WHERE os.tosid = ". TIPO_OS_GERAL ."
                    AND doc.esdid IN ( ". WF_ESTADO_OS_FINALIZADA .", ". WF_ESTADO_OS_CANCELADA_COM_CUSTO .", ". WF_ESTADO_OS_CANCELADA_SEM_CUSTO ." )
                    GROUP BY os.scsid
                    ) os_geral_emitidas
                    ON ss.scsid = os_geral_emitidas.scsid
                LEFT JOIN 
                    (
                    SELECT os.scsid, COUNT (  os.odsid ) as total_emitida_estimada_detalhada
                    FROM fabrica.ordemservico os 
                    LEFT JOIN fabrica.memorando memo
                        ON os.memoid = memo.memoid
                        AND memo.memostatus = 'IMPR'
                    INNER JOIN workflow.documento doc
                        ON os.docidpf = doc.docid
                    WHERE os.tosid IN (". TIPO_OS_CONTAGEM_ESTIMADA .",". TIPO_OS_CONTAGEM_DETALHADA .")
                    AND doc.esdid IN ( ". WF_ESTADO_CPF_FINALIZADA .", ". WF_ESTADO_CPF_CANCELADA ." )
                    GROUP BY os.scsid
                    ) os_estimada_detalhada_emitidas
                    ON ss.scsid = os_estimada_detalhada_emitidas.scsid
                WHERE ss.scsid = {$scsid}
                GROUP BY ss.scsid
            , os_geral_emitidas.total_emitida_geral
            , os_estimada_detalhada_emitidas.total_emitida_estimada_detalhada";
                
        $dados   = $this->pegaLinha($sql);
        return (bool) ($dados['total_os'] == ($dados['total_emitida_geral'] + $dados['total_emitida_estimada_detalhada'] ) );
    }
    
    

}

