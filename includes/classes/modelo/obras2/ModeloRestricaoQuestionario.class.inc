<?php
	
class ModeloRestricaoQuestionario extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'obras2.modelorestricaoquestionario';	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('mrqid');

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos = array(
        'mrqid' => null, 
        'tprid' => null, 
        'mrqtitulo' => null, 
        'mrqdsc' => null, 
        'mrqdtinclusao' => null, 
        'mrqstatus' => null,
        'rstitem' => null 
    );

    public function carregaIdRelacaoPorSueid($sueid)
    {
        $sql = "SELECT
                    DISTINCT
                    q.mrqid,
                    qs.qtsid,
                    NULL::integer AS rsqid,
                    qs.qtsobs AS obs,
                    false AS sqtbloqrestricao
                FROM
                    obras2.questao q
                JOIN obras2.questaosupervisao qs ON qs.qstid = q.qstid AND qs.qtsstatus = 'A' AND qs.qtsresposta = 'f' AND qs.sueid = {$sueid}
                WHERE
                    q.qststatus = 'A' AND
                    q.mrqid IS NOT NULL
                UNION 
                SELECT
                    DISTINCT
                    sq.mrqid,
                    NULL::integer AS qtsid,
                    rsq.rsqid,
                    rsq.rsqobs AS obs,
                    sq.sqtbloqrestricao
                FROM
                    obras2.questao q
                JOIN obras2.subquestao sq ON sq.qstid = q.qstid AND sq.sqtstatus = 'A'
                JOIN obras2.questaosupervisao qs ON qs.qstid = q.qstid AND qs.qtsstatus = 'A' AND qs.sueid = {$sueid}
                JOIN obras2.respostasubquestao rsq ON rsq.sqtid = sq.sqtid AND rsq.rsqstatus = 'A' AND rsq.qtsid = qs.qtsid
                WHERE
                    sq.mrqid IS NOT NULL AND
                    q.qststatus = 'A'";
        
        $dados = $this->carregar( $sql );
        $dados = ($dados ? $dados : array());
        return $dados;
    }
    
    public function pegaQuestaoPorQtsid($qtsid)
    {
        $sql = "SELECT 
                    q.qstid, q.qstdsc, qs.qtsresposta, qs.qtsobs, q.qstctrlobs, qs.qtsid 
                FROM obras2.questaosupervisao qs
                    INNER JOIN obras2.questao q ON (q.qstid = qs.qstid)
                WHERE qtsid = '{$qtsid}'";
        
        $dados = $this->pegaLinha($sql);
        return ($dados ? $dados : array());
    }
    
    public function pegaQuestaoPorRsqid($rsqid)
    {
        $sql = "SELECT 
                    q.qstid, q.qstdsc, qs.qtsresposta, q.qstctrlobs, sq.sqtid, sq.sqtdsc, rsq.rsqobs, qs.qtsid 
                FROM obras2.respostasubquestao rsq 
                    INNER JOIN obras2.questaosupervisao qs ON (qs.qtsid = rsq.qtsid)
                    INNER JOIN obras2.subquestao sq ON (sq.sqtid = rsq.sqtid)
                    INNER JOIN obras2.questao q ON (q.qstid = qs.qstid)
                WHERE rsq.rsqid = '{$rsqid}'";
        
        $dados = $this->pegaLinha($sql);
        return ($dados ? $dados : array());
    }
}