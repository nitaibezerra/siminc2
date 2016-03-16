<?php

class Model_UsuarioEntidade extends Abstract_Model
{

    /**
     * Nome da tabela
     * @var string
     */
    protected $_name = 'pesusuarioentidade';

    /**
     * Nome da chave primaria
     * @var string
     */
    protected $_primary = array('usucpf', 'aexano', 'entcodigo', 'uorcodigo', 'aexanouo');

}