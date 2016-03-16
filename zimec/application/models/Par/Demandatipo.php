<?php
class Model_Par_DemandaTipo extends Zend_Db_Table
{
    protected $_schema = 'par';
    protected $_name   = 'demandatipo';

    public function getCamposValidacao($dados = array())
    {

        $exclude = empty($dados['dmtid'])
            ?null
            :array('field' => 'dmtid', 'value' => $dados['dmtid']);

        return array(
            'dmtid' => array('allowEmpty' => true, 'Digits'),
            'dmtnome' => array(new Zend_Validate_StringLength(array('max' => 50)), new Zend_Validate_Db_NoRecordExists(array(
                'table'  => 'par.demandatipo',
                'field'  => 'dmtnome',
                'exclude' => $exclude
            ))),
        );
    }

    public function gravar(array $dados)
    {
        $primary = 'dmtid';

        if (empty($dados[$primary])) {
            unset($dados[$primary]);
            $row = $this->createRow();
        } else {
            $filtro[$primary . ' = ?'] = $dados[$primary];
            $row = $this->fetchRow($filtro);
        }

        $row->setFromArray($dados);
        $this->validar($row->toArray());
        $this->preSave($dados, $row);

        $id = $row->save();
        $this->preSave($dados, $row);

        return $id;
    }

    protected function validar(array $dados)
    {
        $this->validarCampos($dados);
    }

    final protected function validarCampos(array $dados)
    {
        $fields = $this->getCamposValidacao($dados);
        $validate = new Zend_Filter_Input(array(), $fields, $dados);

        // Se não for válido lança a exception
        $aMensagem = $aCampo = array();
        if (!$validate->isValid()) {
            foreach($validate->getMessages() as $campo => $mensagem){
                $aMensagem[] = current($mensagem);
                $aCampo[] = $campo;
            }

            // -- Campos do formulário que apresentaram erro de validação
            Simec_Util::setSession('form_validation_error', $aCampo);
            Simec_Util::setSession('form_validation_data', $dados);
            throw new Simec_Db_Exception('Não foi possível realizar a operação.', $aMensagem);
        }
        return true;
    }

    public function excluir($where)
    {
        if(is_numeric($where)){
            $where = array('dmtid = ? ' => $where);
        }
        return $this->delete($where);
    }

    protected function preSave($dados, $row)
    {
        return true;
    }

    protected function posSave($dados, $row)
    {
        return true;
    }

    final public function getRow($id, $default = array())
    {

        $primary = current($this->info('primary'));

        $dadosErro = Simec_Util::getSession('form_validation_data');
        Simec_Util::clear('form_validation_data');

        // -- alteracao
        if ($id) {
            $row = $this->fetchRow(array("{$primary} = ?" => $id));
        }

        // -- insercao
        if (!$row) {
            $row = $this->createRow($default);
        }

        // -- sobrescrevendo alteracao/insercao em caso de erro
        if (!empty($dadosErro)) {
            $row->setFromArray($dadosErro);
        }

        return $row;
    }

    public function getQuery($dados)
    {
        $from = $this->_schema .'.'.$this->_name;
        $select = $this->getDefaultAdapter()->select()->from($from);

        if (isset($dados['filtro']) && is_array($dados['filtro'])) {
            foreach ($dados['filtro'] as $campo => $valor) {
                if ($valor) {
                    $select->where($campo . ' ilike ? ', '%' . $valor . '%');
                }
            }
        }

        if (!empty($dados['campo_ordenacao'])) {
            $select->order($dados['campo_ordenacao']);
        }
        return $select;
    }
}
