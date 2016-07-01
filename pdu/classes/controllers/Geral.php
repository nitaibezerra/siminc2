<?php

/**
 * Controle responsavel pelas entidades.
 *
 * @author Equipe simec - Consultores OEI
 * @since  17/10/2013
 *
 * @name       Board
 * @package    classes
 * @subpackage controllers
 * @version    $Id
 */
class Controller_Geral extends Abstract_Controller
{
    protected $_ufName = 'estuf';
    protected $_ufValue;
    protected $_municipioName = 'muncod';
    protected $_municipioValue;
    protected $_chosen = true;

    public function setChosen($value)
    {
        $this->_chosen = $value;
        return $this;
    }

    public function setUfValue($value)
    {
        $this->_ufValue = $value;
        return $this;
    }

    public function setUfName($value)
    {
        $this->_ufName = $value;
        return $this;
    }

    public function setMunicipioName($value)
    {
        $this->_municipioName = $value;
        return $this;
    }

    public function setMunicipioValue($value)
    {
        $this->_municipioValue = $value;
        return $this;
    }

    public function ufAction()
    {
        global $db;

        $id = trim($this->getPost('id'));
        if($id) $this->_ufValue = $id;

        $name = trim($this->getPost('name'));
        if($name) $this->setUfName($name);

        $sql = 'SELECT estuf as codigo, estdescricao as descricao FROM territorios.estado  ORDER BY 2;';
        $uf = (array)  $db->carregar($sql);
        $this->view->uf = $uf;
        $this->render(__CLASS__, __FUNCTION__);
    }

    public function municipioAction()
    {
        global $db;

        $id = trim($this->getPost('id'));
        if($id) $this->_municipioValue = $id;

        $name = trim($this->getPost('name'));
        if($name) $this->setMunicipioName($name);

        $uf = trim($this->getPost('estuf'));
        if(!$uf) $uf = $this->_ufValue;


        if($uf){
            if($uf) $where = " WHERE estuf = '{$uf}' ";
            else $where = '';

            $sql = "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio {$where} ORDER BY 2;";
            $municipios = (array) $db->carregar($sql);
        } else {
            $municipios = array();
        }

        $this->view->municipios = $municipios;
        $this->render(__CLASS__, __FUNCTION__);
    }
}