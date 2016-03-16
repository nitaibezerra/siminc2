<?php
class Model_Aspar_Proposicao extends Simec_Db_Table
{
	protected $_primary = 'prpid';
	protected $_schema  = 'aspar';
    protected $_name    = 'proposicao';

    public function lista($filtros)
    {
        return $this->getList($this->trabalhaParametrosFiltro($filtros));
    }

    public function trabalhaParametrosFiltro($in)
    {
        if(!is_array($in)){
            return $in;
        }
        
        if($in['casa'] || $in['numero']){
            if($in['casa'] == 'camara'){
                $in['prpnumerocamara'] = $in['numero'];
            }else if($in['casa'] == 'senado'){
                $in['prpnumerosenado'] = $in['numero'];
            }else{
                $in['prpnumerosenado'] = $in['prpnumerocamara'] = $in['numero'];
            }
        }
        unset($in['casa'],$in['numero'],$in['parecer']);
        return $in;
    }

    public function getList($whereIn)
    {
        $columns = array('prpid','prptextoementa','prpdtsolicitacao','prpnumerosenado','prpnumerocamara','prpdtparecer','prpdscparecer','prpprazo');
        $select = $this->select();
        $select->setIntegrityCheck(false);
        $select->from(array('prop' => $this->_schema.'.'.$this->_name), $columns);
        $select->joinLeft(array('pr' => 'aspar.prioridade'),'pr.priid = prop.priid',array('pridsc'));
        if($whereIn){
            if($whereIn['prpano']){
                $select->where('prpano = ?',$whereIn['prpano']);
            }
            if($whereIn['prpnumerocamara']){
                $select->where('prpnumerocamara = ?',$whereIn['prpnumerocamara']);
            }
            if($whereIn['prpnumerosenado']){
                $select->where('prpnumerosenado = ?',$whereIn['prpnumerosenado']);
            }
            if($whereIn['tprid']){
                $select->where('tprid = ?',$whereIn['tprid']);
            }
            if($whereIn['priid']){
                $select->where('pr.priid = ?',$whereIn['priid']);
            }
        }
        $select->order('prpid');
        return $this->fetchAll($select)->toArray();
    }

    public function salvar($dados)
    {
        try{
            $dados['tprid'] = $dados['tprid'] ? $dados['tprid'] : null;
            $dados['priid'] = $dados['priid'] ? $dados['priid'] : null;
            $dados['tipid'] = $dados['tipid'] ? $dados['tipid'] : null;
            $dados['prpdtsolicitacao'] = $dados['prpdtsolicitacao'] ? $dados['prpdtsolicitacao'] : null;
            $dados['prpprazo'] = $dados['prpprazo'] ? $dados['prpprazo'] : null;
            $dados['prpano'] = $dados['prpano'] ? $dados['prpano'] : null;
            $dados['usucpf'] = $_SESSION['usucpf'];
            if($dados['prpid']){
                $dados['prpdtinclusao'] = 'NOW()';
            }
            if($dados['casa'] == 'camara'){
                $dados['prpnumerosenado'] = '';
                $dados['prpnumerocamara'] = $dados['numero'];
            }else{
                $dados['prpnumerocamara'] = '';
                $dados['prpnumerosenado'] = $dados['numero'];
            }
            $this->beginTransaction();
            $id = $this->gravar($dados);
            $this->commit();
            return $id;
        }catch(Exception $e){
            $this->rollback();
            throw($e);
        }
    }

    public function remover($prpid)
    {
        try{
            $this->beginTransaction();
            $this->excluir(array('prpid = ?' => $prpid));
            $this->commit();
        } catch (Exception $ex) {
            $this->rollback();
            throw($ex);
        }
        
    }
}
