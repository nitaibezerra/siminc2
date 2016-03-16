<?php

class Seguranca_Model_Usuario extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "seguranca.usuario";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "usucpf" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
    		'usucpf' => null,
    		'regcod' => null,
    		'usunome' => null,
    		'usuemail' => null,
    		'usustatus' => null,
    		'usufoneddd' => null,
    		'usufonenum' => null,
    		'ususenha' => null,
    		'usudataultacesso' => null,
    		'usunivel' => null,
    		'usufuncao' => null,
    		'ususexo' => null,
    		'orgcod' => null,
    		'unicod' => null,
    		'usuchaveativacao' => null,
    		'usutentativas' => null,
    		'usuprgproposto' => null,
    		'usuacaproposto' => null,
    		'usuobs' => null,
    		'ungcod' => null,
    		'usudatainc' => null,
    		'usuconectado' => null,
    		'pflcod' => null,
    		'suscod' => null,
    		'usunomeguerra' => null,
    		'orgao' => null,
    		'muncod' => null,
    		'usudatanascimento' => null,
    		'usudataatualizacao' => null,
    		'entid' => null,
    		'tpocod' => null,
    		'carid' => null,
	);

	protected $stOrdem = null;

	public function recuperarPorCPF($usucpf)
	{
		$sql = "SELECT *
			    FROM {$this->stNomeTabela}
			    WHERE usuario.usucpf = '{$usucpf}'";

		return $this->pegaLinha($sql);
	}

}