<?php
/**
 * Classe de mapeamento da entidade par3.usuarioresponsabilidade
*
* @category Class
* @package  A1
* @author   Eduardo Dunice <eduardoneto@mec.gov.br>
* @license  GNU simec.mec.gov.br
* @version  Release: 28-09-2015
* @link     no link
*/

/**
 * Par_Modelo_UsuarioResponsabilidade
*
* @category Class
* @package  A1
* @author   Eduardo Dunice <eduardoneto@mec.gov.br>
* @license  GNU simec.mec.gov.br
* @version  Release: 28-09-2015
* @link     no link
*/
class Par3_Model_UsuarioResponsabilidade extends Modelo
{
	/**
	 * Nome da tabela especificada
	 *
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = 'par3.usuarioresponsabilidade';

	/**
	 * Chave primaria.
	 *
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array(
		'rpuid',
	);

	/**
	 * Chaves estrangeiras.
	 *
	 * @var array
	*/
	protected $arChaveEstrangeira = array(
		'muncod' => array('tabela' => 'territorios.municipio', 'pk' => 'muncod'),
	);

	/**
	 * Atributos
	 *
	 * @var array
	 * @access protected
	*/
	protected $arAtributos = array(
		'rpuid' => null,
		'pflcod' => null,
		'usucpf' => null,
		'rpustatus' => null,
		'rpudata_inc' => null,
		'muncod' => null,
		'entid' => null,
	);

	const SUPER_USUARIO			= 1341;
	const EQUIPE_MUNICIPAL  	= 1344;
	const EQUIPE_ESTADUAL   	= 1345;
	const DIRIGENTE_MUNICIPAL   = 1346;
	const DIRIGENTE_ESTADUAL    = 1347;
	const NUTRICIONISTA 		= 1348;

	/**
	* Função validarUsuarioResponsabilidade
	* - valida se o CPF possui perfil para determinado municipio.
	*
	* @param string $usucpf
	* @param string $pflcod
	* @param string $muncod (optional)
	*
	* @return void.
	*
	*/
	public function validarUsuarioResponsabilidade($usucpf, $pflcod, $muncod = null) {
		$where = array();


		 
		if ($usucpf) $where[] = "usucpf = '{$usucpf}'";
		if ($pflcod) $where[] = "pflcod = '{$pflcod}'";
		if ($muncod) $where[] = "muncod = '{$muncod}'";
		 
		$sql = "SELECT rpuid
				FROM par3.usuarioresponsabilidade
				WHERE ".implode(' AND ', $where);

     

		return $this->carregar($sql);
	}//end inativaEntidadesInuidPorTipo()

}//end Class
?>