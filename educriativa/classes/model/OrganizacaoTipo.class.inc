<?php
class Educriativa_Model_OrganizacaoTipo extends Modelo {
	
	/**
	 * Nome da tabela especificada
	 *
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "criatividadeeducacao.organizacao_tipo";
	
	/**
	 * Chave primaria.
	 *
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array (
		"ortid"
	);
	
	/**
	 * Atributos
	 *
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array (
		'ortid' => null,
		'ortdsc' => null,
		'gruid' => null,
	);

    public function getByGrupo($gruid = null)
    {
        $gruid = $gruid ? $gruid : 0;
        $sql = "select * from criatividadeeducacao.organizacao_tipo where gruid = $gruid";
        $tipos = $this->carregar($sql);
        return $tipos ? $tipos : array();
    }

    public function montarOptions($gruid = null)
    {
        $tipos = $this->getByGrupo($gruid);

        print simec_json_encode($tipos);
        die;
    }
}