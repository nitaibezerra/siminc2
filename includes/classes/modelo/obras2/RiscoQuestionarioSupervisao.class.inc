<?php

/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
class RiscoQuestionarioSupervisao extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'obras2.riscoquestionariosupervisao';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('rqsid');

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos = array(
        'rqsid' => null, 
        'sueid' => null,
        'qstid' => null,
        'mrqid' => null, 
        'sqtid' => null,
        'sueriscousuario' => null, 
        'suetiporiscoobs' => null, 
        'rsqstatus' => null, 
    );
    
    /**
     * 
     * @param integer $sqtid
     * @return array
     */
    public function pegaRespostaPorSqtid($sueid, $sqtid)
    {
        if (null === $sueid || $sueid < 1 && null === $sqtid || $sqtid < 1) {
            return array();
        }
        
        $strSQL = "SELECT * FROM {$this->stNomeTabela} WHERE sqtid = {$sqtid} AND sueid = {$sueid}";
        $dado = $this->pegaLinha($strSQL);
        return ($dado ? $dado : array());
    }
    
    /**
    
     * @param integer $sueid
     * @param integer $mrqid
     * @return array * 
     * @param integer $sueid
     * @param integer $mrqid
     * @return array
     */
    public function pegaRespostaPorSueidMrqid($sueid, $mrqid)
    {
        if (null === $sueid || $sueid < 1 && null === $mrqid || $mrqid < 1) {
            return array();
        }
        
        $strSQL = "SELECT * FROM {$this->stNomeTabela} WHERE sueid = {$sueid} AND mrqid = {$mrqid}";
        $dado = $this->pegaLinha($strSQL);
        return ($dado ? $dado : array());
    }
    
    /**
     *
     * @param integer $sueid
     * @return boolean @param type $sueid
     * @return boolean
     */
    public function deletaPorSueId($sueid)
    {
        if (null === $sueid || $sueid < 1) {
            return false;
        }
        
        $strSQL = "DELETE FROM {$this->stNomeTabela} WHERE sueid = {$sueid}";
        $return = $this->executar($strSQL);
        return ($return) ? true : false;
    }
    
    /**
     * Pega todas as resposta de restricao e ou inconformidade
     * pela supervisao da obra $sueid
     * 
     * @param integer $sueid
     * @return array $dados
     */
    public function getBySueid($sueid)
    {
        if (null === $sueid || $sueid < 1) {
            return array();
        }
        
        $strSQL = "

            SELECT
            r.*--, rsq.*
            FROM {$this->stNomeTabela} r
            JOIN obras2.questao qst ON qst.qstid = r.qstid
            JOIN obras2.questaosupervisao qts ON qts.qstid = qst.qstid AND r.sueid = qts.sueid
            LEFT JOIN obras2.subquestao sqt ON sqt.sqtid = r.sqtid
            LEFT JOIN obras2.respostasubquestao rsq ON rsq.qtsid = qts.qtsid AND sqt.sqtid = rsq.sqtid
            WHERE r.sueid = {$sueid} AND CASE WHEN rsq.rsqid IS NOT NULL THEN rsq.rsqstatus = 'A' ELSE true END
        ";
        $dados = $this->carregar($strSQL);
        
        return ($dados) ? $dados : array();
    }
}
