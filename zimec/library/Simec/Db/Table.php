<?php

class Simec_Db_Table extends Zend_Db_Table
{
	public function getCamposValidacao($dados = array())
	{
		return array();
	}
	
	/**
	 * Start new transaction.
	 *
	 * @return void
	 */
	public function beginTransaction() {
		$this->getAdapter()->beginTransaction();
	}
	
	/**
	 * Rollback exist transaction.
	 *
	 * @return void
	 */
	public function rollBack() {
		$this->getAdapter()->rollBack();
	}
	
	/**
	 * Commit exist transaction.
	 *
	 * @return void
	 */
	public function commit() {
		$this->getAdapter()->commit();
	}
	
	public function gravar(array $dados)
	{
		$primary = current($this->info('primary'));

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
		$this->posSave($dados, $row);
	
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

    /**
     * Retorna um array de dados Chave (Value) => Descrição (Option) para inserção no formSelect.
     * @param type $campo -> Array('chave' => 'campoDescricao') || Array('chave' => array('CampoDescricao','OutroCampoÀConcatenar)) || Null
     * @param string|array|Zend_Db_Table_Select $where OPCIONAL cláusula WHERE SQL OU Zend_Db_Table_Select object.
     * @param string|array $order
     * @return array()
     */
    public function getPreparedArray($campo = null, $where = null, $order = null)
    {
        if (is_array($campo)) {
            $chave = key($campo);
            $valor = current($campo);
        } else {
            $chave = current($this->info('primary'));
            $valor = $campo;
        }

        // Caso não seja informado nunhum campo para realizar a busca, tenta-se buscar pelo padrão de nomenclatura de campo de descricao
        if (!$campo) {
            $possibilidades = array('dsc', 'desc', 'descricao', 'nome');
            $prefixo = substr($chave, 0, 3);
            foreach ($possibilidades as $possibilidade) {
                $verificar = $prefixo . $possibilidade;
                if (in_array($verificar, $this->info('cols'))) {
                    $valor = $verificar;
                    break;
                }
            }
        }

        $order = $order ? $order : $valor;

        $rowSet = $this->fetchAll($where, $order);

        $preparedArray = array();
        if($chave && $valor){
            foreach ($rowSet as $row) {
                if(is_array($valor)){
                    $descricoes = array();
                    foreach($valor as $campoValor){
                        $descricoes[] = trim($row->{$campoValor});
                    }
                    $descricao = implode(' - ', $descricoes);
                } else {
                    $descricao = $row->{$valor};
                }
                $preparedArray[$row->{$chave}] = $descricao;
            }
        }
        return $preparedArray;
    }
}