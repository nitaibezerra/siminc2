<?php
class Model_Seguranca_Menu extends Simec_Db_Table
{
	protected $_primary = 'mnuid';
    protected $_schema = 'seguranca';
    protected $_name   = 'menu';

    public function getCamposValidacao($dados = array())
    {
        return array(
            'mnuid'             => array('allowEmpty' => true, 'Digits'),
            'mnucod'            => array('Digits'),
            'mnudsc'            => array(new Zend_Validate_StringLength(array('max' => 50))),
            'mnulink'           => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 100))),
            'mnutipo'           => array('allowEmpty' => true, 'Digits'),
            'mnustile'          => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 200))),
            'mnuhtml'           => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 4000))),
            'mnusnsubmenu'      => array('allowEmpty' => true),
            'mnutransacao'      => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 50))),
            'mnushow'           => array('allowEmpty' => true),
            'abacod'            => array('allowEmpty' => true, 'Digits'),
            'mnuhelp'           => array('allowEmpty' => true),
            'sisid'             => array('allowEmpty' => true, 'Digits'),
            'mnuidpai'          => array('allowEmpty' => true, 'Digits'),
            'mnuimagem'         => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 200))),
        );
    }

    public function gravar(array $dados)
    {
        $primary = 'mnuid';

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

    public function excluir($where)
    {
        if (is_numeric($where)) {
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

    public function getMenuPai($sisid, $mnutipo)
    {
        $mnutipoPai = $mnutipo - 1;
        return $this->getPreparedArray(null, array('mnusnsubmenu = ?'=>'t', 'sisid = ?'=>$sisid, 'mnutipo = ?'=>$mnutipoPai));
    }
   
    public function getMenu($cpf, $sisid = null)
    {
    	$select = $this->select();
    	$select->setIntegrityCheck(false);
    	$select->from(array('menu' => 'seguranca.menu'), array('mnucod', 'mnuid', 'mnuidpai', 'mnudsc', 'mnustatus', 'mnulink', 'mnutipo', 'mnustile', 'mnuhtml', 'mnusnsubmenu', 'mnutransacao', 'mnushow', 'abacod'));
    	$select->joinInner(array('perfilmenu' => 'seguranca.perfilmenu'), "menu.mnuid = perfilmenu.mnuid");
    	$select->joinInner(array('perfil' => 'seguranca.perfil'), "perfilmenu.pflcod = perfil.pflcod");
    	$select->joinInner(array('perfilusuario' => 'seguranca.perfilusuario'), "perfil.pflcod = perfilusuario.pflcod");
    	$select->where("perfilusuario.usucpf = '{$cpf}'");
    	$select->where("menu.mnushow = 't'");
    	$select->where("menu.mnustatus = 'A'");
    	
    	if ($sisid) {
    		$select->where("menu.sisid = '{$sisid}'");
    	}
    	
    	$select->order(array('menu.mnutipo', 'menu.mnucod', 'menu.mnuid', 'menu.mnuidpai', 'menu.mnudsc'));
    	
    	return $this->fetchAll($select);
    }
}
