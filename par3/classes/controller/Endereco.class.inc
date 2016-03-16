<?php
/**
 * Classe de controle do Endereço
 *
 * @category Class
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 29-09-2015
 * @link     no link
 */

/**
 * Par3_Controller_Endereco
 *
 * @category Class
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 29-09-2015
 * @link     no link
 */
class Par3_Controller_Endereco extends Modelo
{
	public function __construct()
	{
		parent::__construct();
	}


    /**
     * Função salvarEndereco
     * - retorna o endereco da prefeitura.
     *
     * @return retorna um objeto endereco prefeitura.
     *
     */
    public function salvarEndereco($arrDados)
    {
        $prefeitura = new Par3_Model_Endereco($arrDados['endid']);

        if ($prefeitura->endcep != $arrDados['endcep']) {
            unset($arrDados['endid']);
            $prefeitura = new Par3_Model_Endereco();
        }

        $arrDados['endcep'] = str_replace(array('.','-'), '', $arrDados['endcep']);

        $prefeitura->popularDadosObjeto($arrDados);

        $endid = $prefeitura->salvar();

        return $endid;

    }//end salvarEndereco()


}//end Class
?>